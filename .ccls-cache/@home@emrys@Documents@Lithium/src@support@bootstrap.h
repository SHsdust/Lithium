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

#ifndef BOOTSTRAP_H_INCLUDED_WNIM1RZ0
#define BOOTSTRAP_H_INCLUDED_WNIM1RZ0

#include "../config.h"

namespace Li {
namespace bootstrap {

struct Bootstrap_Reg;
struct Bootstrap_Args;

using Bootstrap_Func    = void (Bootstrap_Args &, const Bootstrap_Reg &);

using Bootstrap_Routine = std::function<Bootstrap_Func>;

struct Bootstrap_Args
{
  Str          app_name; ///> argv[0]
  Str_List     args;     ///> argv + 1
};

struct Bootstrap_Reg
{
  Str           name;
  Set<Str>      deps;

  struct Task_Pack
  {
    Bootstrap_Routine early_init,
                      late_init,
                      early_fini,
                      late_fini;
  };

  Task_Pack tasks;

};

static inline auto make_empty_routine()
{
  return [](Bootstrap_Args &, const Bootstrap_Reg &) { };
}

static inline
auto make_bootstrap_reg(const Str &name,
                        const Set<Str> &deps = { },
                        Bootstrap_Routine &&early_init = nullptr,
                        Bootstrap_Routine &&late_init  = nullptr,
                        Bootstrap_Routine &&early_fini = nullptr,
                        Bootstrap_Routine &&late_fini  = nullptr)
{
  // do the fallback.
  if (!early_init) early_init = make_empty_routine();
  if (!late_init)  late_init  = make_empty_routine();
  if (!early_fini) early_fini = make_empty_routine();
  if (!late_fini)  late_fini  = make_empty_routine();

  return Bootstrap_Reg { name, deps, {
    std::move(early_init), std::move(late_init),
    std::move(early_fini), std::move(late_fini) } };
}

attr_export
Bootstrap_Reg make_bootstrap_reg_scheme(const Str &name,
                                        const Str &scheme = "",
                                        Bootstrap_Routine &&early_init = nullptr,
                                        Bootstrap_Routine &&late_init  = nullptr,
                                        Bootstrap_Routine &&early_fini = nullptr,
                                        Bootstrap_Routine &&late_fini  = nullptr);

/**
 * an obaque handle represents an isolated initialize scope.
 */
struct Init_Scope;

/**
 * obtain a handle.
 *
 * @note it is the caller's duty to release the handle manually
 * by call <code>release_handle</code>
 */
attr_export Init_Scope *acquire_handle(const Str &domain);

/**
 * release the handle.
 *
 * @note do not call this on <code>g_init_scope()</code>
 */
attr_export void release_handle(Init_Scope *);

/**
 * register a reg into an initialize scope.
 *
 * @note throws std::runtime_error if <code>init_init</code> has been
 * called on this handle.
 */
attr_export void init_register(Init_Scope *handle, Bootstrap_Reg &&);

/**
 * append dep.
 */
attr_export void init_register_add_dependence(Init_Scope *handle,
                                              const Str  &path,
                                              const Str  &dep);

/**
 * append deps.
 */
attr_export void init_register_add_dependences(Init_Scope     *handle,
                                               const Str      &path,
                                               const Set<Str> &deps);

/**
 * append deps.
 */
attr_export void init_register_add_dependences_scheme(Init_Scope *handle,
                                                      const Str  &path,
                                                      const Str  &scheme);

/**
 * append procedure.
 */
attr_export void init_append_early_init(Init_Scope         *handle,
                                        const Str          &path,
                                        Bootstrap_Routine &&routine);

/**
 * late-init version of above.
 */
attr_export void init_append_late_init(Init_Scope           *handle,
                                       const Str            &path,
                                       Bootstrap_Routine   &&routine);

/**
 * early-fini version of above.
 */
attr_export void init_append_early_fini(Init_Scope         *handle,
                                        const Str          &path,
                                        Bootstrap_Routine &&routine);
/**
 * early-fini version of above.
 */
attr_export void init_append_late_fini(Init_Scope           *handle,
                                       const Str            &path,
                                       Bootstrap_Routine   &&routine);


/**
 * do the initialization procedure, ordered by the dependence graph.
 *
 * @note throws std::runtime_error if dependence could not be satisfied.
 */
attr_export void init_init(Init_Scope *handle, Bootstrap_Args &args, bool const_args = false);

/**
 * do the finalization procedure.
 */
attr_export void init_fini(Init_Scope *handle);

/**
 * obtain the default/global init handle.
 */
attr_export Init_Scope *g_init_scope();

} // namespace bootstrap
} // namespace Li

#define Li_Bootstrap_Init_Prio (Li_Internal_Init_Prio + 500)

#define $Li_Bootstrap_Scope()                                         \
  namespace { namespace Li_Cat2($Uniq_BS_, __LINE__) {                \
  static attr_init(Li_Bootstrap_Init_Prio)                            \
  void Li_Join(bootstrapper, __LINE__)(); } }                         \
  void Li_Cat2($Uniq_BS_, __LINE__)::Li_Join(bootstrapper, __LINE__)()

#define Li_Bootstrap_Register(path, scheme)            \
  $Li_Bootstrap_Scope()                                \
  {                                                    \
    using namespace ::Li::bootstrap;                   \
    init_register(g_init_scope(),                      \
                  make_bootstrap_reg_scheme(path,      \
                                            scheme));  \
  }

#define Li_Bootstrap_Append_Routine(path, stage)                                    \
  namespace { namespace Li_Cat2($Uniq_BAR_, __LINE__) {                             \
    static                                                                          \
    void bootstrap_routine(      ::Li::bootstrap::Bootstrap_Args &,                 \
                           const ::Li::bootstrap::Bootstrap_Reg &);                 \
  } }                                                                               \
  $Li_Bootstrap_Scope()                                                             \
  {                                                                                 \
    using namespace ::Li::bootstrap;                                                \
    Li_Cat2(init_append_, stage)(g_init_scope(),                                    \
                                 path,                                              \
                                 Li_Cat2($Uniq_BAR_, __LINE__)::bootstrap_routine); \
  } void Li_Cat2($Uniq_BAR_, __LINE__)::bootstrap_routine(                          \
    attr_unused       ::Li::bootstrap::Bootstrap_Args &$args,                       \
    attr_unused const ::Li::bootstrap::Bootstrap_Reg  &$reg)

#define Li_Bootstrap_Append_Dependences(path, scheme)                   \
  $Li_Bootstrap_Scope()                                                 \
  {                                                                     \
    using namespace ::Li::bootstrap;                                    \
    init_register_add_dependences_scheme(g_init_scope(), path, scheme); \
  }

#endif // end of include guard: BOOTSTRAP_H_INCLUDED_WNIM1RZ0

