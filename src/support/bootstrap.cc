/*
 *       _,.--.,_                             _,.--.,_
 *      |  _____ |                           | _____  |
 *      |-'     `'._________________________,'`     `-|
 *    ,'    ___    `.                     ,'     .-.   ``.
 *   /     |   |     \                   /      (/_\)     \
 *  !  ___  \ /  ___  |                 !   ,-.  `-'  ,-.  |
 *  | |___>     <___| !_SELECT   START__|  ([ ])     ( O ) !
 *  !       / \      ____`-.       .-'____  `-'  .-.  `-'  |
 *  |\     |___|   ,'    `. \     / ,'    `.    ( X )     /|
 *  | `~          /        \ |   | /        \    `-'    ,' |
 *  |   `-~___,-. \        / |___| \        / ,-.____,-'   |
 *  |           '\ `.____,' /     \ `.____,' /`.           |
 *  |          /  `-.____,-'       `-.____,-'   \          |
 *  \         /                                  \         /
 *   \       /                                    \       /
 *    `.__,-'                                      `-.__,'
 *
 *
 */

#include "debug.h"
#include "assert.h"
#include "bootstrap.h"
#include "misc.h"
#include "string.h"
#include "span.h"

namespace Li {
namespace bootstrap {

Bootstrap_Reg make_bootstrap_reg_scheme(const Str &name,
                                        const Str &scheme,
                                        Bootstrap_Routine &&early_init,
                                        Bootstrap_Routine &&late_init,
                                        Bootstrap_Routine &&early_fini,
                                        Bootstrap_Routine &&late_fini)
{
  auto deps = str_split(scheme, ",");

  return make_bootstrap_reg(name,
                            { deps.begin(), deps.end() },
                            std::move(early_init),
                            std::move(late_init),
                            std::move(early_fini),
                            std::move(late_fini));
}


struct Bootstrap_Node;

using Node_List = Seq<Bootstrap_Node *>;
using Deps      = Set<Str>;

enum Init_Status
{
  Waiting,
  Initializing,
  Finalizing,
  Done_Initializaiton,
  Done_Finalization,
};

struct Bootstrap_Node
{
  Ptr<Bootstrap_Reg> reg;

  Set<Ptr<Bootstrap_Node>>   pool;
  Map<Str, Bootstrap_Node *> entries;

  Bootstrap_Node            *root;
  Bootstrap_Node            *parent;
  unsigned                   depth;

  Node_List                  queue;
  Node_List                  pending;

  Str                        path;

  bool                       activated;

  Init_Status                status;

  Bootstrap_Node(Bootstrap_Reg &&reg, bool activated = false)
    : reg(std::make_unique<Bootstrap_Reg>(std::move(reg)))
    , root(this)
    , parent(nullptr)
    , depth(0)
    , activated(activated)
    , status(Waiting)
  {
    path = this->reg->name;
  }

  Bootstrap_Node() : Bootstrap_Node(make_bootstrap_reg("<root>")) { }

  inline
  Bootstrap_Node *add_entry(Ptr<Bootstrap_Node> &&entry)
  {
    auto *save = entry.get();

    pool.insert(std::move(entry));
    entries.emplace(save->reg->name, save);

    save->parent = this;
    save->root   = root;
    save->depth  = depth + 1;

    if (this == root)
      save->path = save->reg->name;
    else
      save->path = path + "." + save->reg->name;

    return save;
  }

  inline
  Bootstrap_Node *prepare_path(const Str_List &path, bool active = false)
  {
    auto *iter = this;

    for (auto &&entry : path)
    {
      if (!u_has(iter->entries, entry))
      {
        iter->add_entry(std::make_unique<Bootstrap_Node>(make_bootstrap_reg(entry)));
      }

      iter->activated = iter->activated || active;
      iter            = iter->entries.at(entry);

      Li_Runtime_Assert(iter->reg->tasks.early_init);
      Li_Runtime_Assert(iter->reg->tasks.late_init);
      Li_Runtime_Assert(iter->reg->tasks.early_fini);
      Li_Runtime_Assert(iter->reg->tasks.late_fini);
    }

    return iter;
  }

  inline
  Bootstrap_Node *prepare_path(const Str &path, bool active = false)
  {
    return prepare_path(u_map(str_split(path, "."), str_trim), active);
  }

  inline
  void complete_deep_dependences(const Bootstrap_Node *dep)
  {
    Li_Assert(root == dep->root);

    auto *node = this;

    while (node->depth > dep->depth)
      node = node->parent;

    while (dep->depth > node->depth)
      dep = dep->parent;

    while (node->parent != dep->parent)
    {
      node = node->parent;
      dep  = dep->parent;
    }

    node->reg->deps.insert(dep->reg->name);
  }

  inline void add_dependence(const Str &depname)
  {
    Li_Assert(!depname.empty());

    if (depname[0] == '@')
      reg->deps.insert(depname.substr(1));
    else
      complete_deep_dependences(root->prepare_path(depname));
  }

  inline
  Opt<size_t> probe_install_pos(Bootstrap_Node *entry)
  {
    auto ndeps = entry->reg->deps.size();

    if (ndeps == 0)
      return { 0 };

    for (size_t pos = 0; pos != queue.size(); ++pos)
    {
      auto that = queue.at(pos);

      if (u_has(entry->reg->deps, that->reg->name)
        && that->activated
        && --ndeps == 0)
      {
        return { pos + 1 };
      }
    }

    Li_LogI("will retry later for [%s]", entry->path);

    return { };
  }

  inline
  void install(Bootstrap_Node *entry, size_t pos)
  { queue.insert(queue.begin() + pos, entry); }

  inline void resolve_pending()
  {
    for (auto iter = pending.begin(); iter != pending.end(); ++iter)
    {
      if (auto pos = probe_install_pos(*iter))
      {
        install(*iter, *pos);
        pending.erase(iter);

        return resolve_pending();
      }
    }
  }

  inline void resolve_one(Bootstrap_Node *one)
  {
    if (auto pos = probe_install_pos(one))
    {
      install(one, *pos);
      resolve_pending();
    }
    else
    {
      pending.push_back(one);
    }
  }

  inline void resolve()
  {
    for (auto &entry : pool)
      resolve_one(entry.get());
  }

  void sort()
  {
    if (status != Waiting)
      Li_PanicF("Multiply initialization detected!");

    resolve();

    if (!pending.empty())
    {
      auto unresolved = str_map_join(pending, "\n", [] (auto *node)
                                     {
                                       return node->path
                                        +     " requires ["
                                        +     str_join(node->reg->deps, ", ")
                                        +     "]";
                                     });

      auto msg = "Unresolved deps of " + path + " :\n" + unresolved;

      Li_PanicF("%s", msg);
    }

    Li_TraceF("sequenced: [%s]", path);

    for (auto *entry : queue)
      entry->sort();
  }

  void initialize(Bootstrap_Args &args)
  {
    status = Initializing;
    auto _ = $defer_with (this)
    {
      status = Done_Initializaiton;
    };

    reg->tasks.early_init(args, *reg);

    for (auto &&entry : queue)
      entry->initialize(args);

    reg->tasks.late_init(args, *reg);
  }

  void finalize(Bootstrap_Args &args)
  {
    status = Finalizing;
    auto _ = $defer_with (this)
    {
      status = Done_Finalization;
    };

    reg->tasks.early_fini(args, *reg);

    for (auto pent = queue.rbegin(); pent != queue.rend(); ++pent)
      (*pent)->finalize(args);

    reg->tasks.late_fini(args, *reg);
  }

  Str dump(int ind = 0) const
  {
    auto padding   = Str(ind, ' ');
    auto deps_text = reg->deps.empty()
                   ? Str("")
                   : "requires " + str_join(u_map(reg->deps, [](auto s) { return "@" + s; }), ", ");
    auto dump_text = format("%s@%s %s", padding, reg->name, deps_text);

    if (!pool.empty())
    {
      dump_text += format("\n%s{\n", padding)
                +  str_map_join(pool,
                                "",
                                [ind] (auto &i) { return i->dump(ind + 4); })
                +  format("%s}\n", padding);
    }
    else
    {
      dump_text += format(" { - }\n");
    }


    return dump_text;
  }

  Str_List collect() const
  {
    Str_List result = { path };
    for (auto &&sub : queue)
    {
      auto sublist = sub->collect();

      result.insert(result.end(),
                    sublist.begin(),
                    sublist.end());
    }

    return result;
  }
};

struct Init_Scope
{
  Ptr<Bootstrap_Node> root;
  Bootstrap_Args      args;
  Str                 domain;

  Init_Scope(const Str &domain)
    : root(new Bootstrap_Node)
    , args()
    , domain(domain)
  { }
};

Init_Scope *g_init_scope()
{
  static auto the_scope = Init_Scope("_G");
  return &the_scope;
}

Init_Scope *acquire_handle(const Str &domain)
{ return new Init_Scope(domain); }

void release_handle(Init_Scope *handle)
{ delete handle; }

static inline
Bootstrap_Routine routine_concat(Bootstrap_Routine &&firstly,
                                 Bootstrap_Routine &&then)
{
  return [ firstly = std::move(firstly)
         , then    = std::move(then) ]
         (Bootstrap_Args &args, const Bootstrap_Reg &reg)
  {
    firstly(args, reg);
    then(args, reg);
  };
}


void init_register(Init_Scope *handle,
                   Bootstrap_Reg &&reg)
{
  Li_LogI("register: %s : [%s]",
          reg.name,
          str_join(reg.deps, ", "));

  Li_Runtime_Assert(handle->root->status == Waiting);

  auto *node = handle->root->prepare_path(reg.name);

  for (auto &&dep : reg.deps)
    node->add_dependence(dep);

  node->activated  = true;
#define Copy(S) \
  node->reg->tasks.S = routine_concat(std::move(reg.tasks.S), std::move(node->reg->tasks.S))
  Copy(early_init);
  Copy(early_fini);
  Copy(late_init);
  Copy(late_fini);
}

void init_init(Init_Scope *handle, Bootstrap_Args &args, bool const_args)
{
  handle->args = args;

  handle->root->activated = true;
  handle->root->sort();

  Li_LogI("[%s] dependences well resolved, with dep-graph:\n%s",
          handle->domain,
          handle->root->dump());

  Li_LogI("[%s] and the nice init sequence:\n%s",
          handle->domain,
          str_join(handle->root->collect(), "\n"));

  handle->root->initialize(handle->args);

  if (!const_args)
    args = handle->args;
}

void init_fini(Init_Scope *handle)
{
  handle->root->finalize(handle->args);
}

void init_register_add_dependence(Init_Scope *handle,
                                  const Str  &path,
                                  const Str  &dep)
{
  auto *node = handle->root->prepare_path(path);
  node->add_dependence(dep);
}

void init_register_add_dependences(Init_Scope     *handle,
                                   const Str      &path,
                                   const Set<Str> &deps)
{
  auto *node = handle->root->prepare_path(path);
  for (auto &&dep : deps)
    node->add_dependence(dep);
}

void init_register_add_dependences_scheme(Init_Scope *handle,
                                          const Str  &path,
                                          const Str  &scheme)
{
  auto deps = str_split(scheme, ",");

  init_register_add_dependences(handle,
                                path,
                                { deps.begin(), deps.end() });
}

#define do_append_routine_for(stage)          \
  node->reg->tasks.stage = routine_concat(    \
    std::move(node->reg->tasks.stage),        \
    std::move(routine))

#define gen_append_for(stage)                                  \
void Li_Cat2(init_append_, stage)(Init_Scope         *handle,  \
                                  const Str          &path,    \
                                  Bootstrap_Routine &&routine) \
{                                                              \
  auto *node = handle->root->prepare_path(path);               \
  do_append_routine_for(stage);                                \
}

gen_append_for(early_init) gen_append_for(late_init)
gen_append_for(early_fini) gen_append_for(late_fini)

} // namespace bootstrap
} // namespace Li

