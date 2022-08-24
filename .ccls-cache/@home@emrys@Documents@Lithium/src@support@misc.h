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

#ifndef MISC_H_INCLUDED_UBK6EJIX
#define MISC_H_INCLUDED_UBK6EJIX

#include "../prelude/stdincl.h"
#include "../prelude/stdalias.h"

namespace Li {

// {{{ u_* utilities

// {{{ map
template < template <typename ...> class Container = std::vector
         , typename Input
         , typename Lambda
         >
static inline
auto u_map(const Input  &input,
           Lambda      &&lambda)
{
  using Element = std::decay_t<decltype (lambda(*std::cbegin(input)))>;
  using Output  = Container<Element>;

  Output output;
  std::transform(std::cbegin(input),
                 std::cend(input),
                 std::back_inserter(output),
                 std::forward<Lambda>(lambda));

  return output;
}



template < template <typename ...> class Container = std::vector
         , typename Element
         , typename Lambda
         >
static inline
auto u_map(const std::initializer_list<Element>  &input,
           Lambda                               &&lambda)
{
  using Output  = Container<Element>;

  Output output;
  std::transform(std::cbegin(input),
                 std::cend(input),
                 std::back_inserter(output),
                 std::forward<Lambda>(lambda));

  return output;
}

// }}}

// {{{ filter
template < class    Container
         , typename Lambda
         >
static inline
auto u_filter_cp(const Container  &input,
                 Lambda          &&lambda)
{
  Container output;

  auto out_iter = std::back_inserter(output);

  for (auto &&ele : input)
    if (std::forward<Lambda>(lambda)(ele))
      out_iter = ele;

  return output;
}



template < class    Container
         , typename Lambda
         >
static inline
auto u_filter_inp(Container &input,
                  Lambda   &&lambda)
{
  input.erase(std::remove_if(std::begin(input),
                             std::end(input),
                             std::forward<Lambda>(lambda)));

  return input;
}



template < typename T
         , typename Lambda
         >
static inline
auto u_filter_inp(std::list<T>  &input,
                  Lambda       &&lambda)
{
  input.remove_if(std::begin(input),
                  std::end(input),
                  std::forward<Lambda>(lambda));

  return input;
}

// }}}

// {{{ fold
template < typename Output
         , typename Input
         , typename Lambda
         >
static inline
Output u_foldl(Output        output,
               const Input  &input,
               Lambda      &&lambda)
{
  for (auto &&ele : input)
    output = lambda(output, ele);

  return output;
}



template < typename Output
         , typename Input
         , typename Lambda
         >
static inline
Output u_foldr(Output        output,
               const Input  &input,
               Lambda      &&lambda)
{
  for (auto iter = std::crbegin(input); iter != std::crend(input); ++iter)
    output = lambda(output, *iter);

  return output;
}



template < typename Output
         , typename Element
         , typename Lambda
         >
static inline
Output u_foldl(Output                                 output,
               const std::initializer_list<Element>  &input,
               Lambda                               &&lambda)
{
  for (auto &&ele : input)
    output = lambda(output, ele);

  return output;
}



template < typename Output
         , typename Element
         , typename Lambda
         >
static inline
Output u_foldr(Output                                 output,
               const std::initializer_list<Element>  &input,
               Lambda                               &&lambda)
{
  for (auto iter = std::crbegin(input); iter != std::crend(input); ++iter)
    output = lambda(output, *iter);

  return output;
}

// }}}

// {{{ find / has
template <class T, class K>
static inline
auto u_find(const T &c, const K &k)
{
  using mapped_type = typename T::mapped_type;

  auto found = c.find(k);

  if (found == c.end())
    return Opt<mapped_type> { };

  return Opt<mapped_type> { found->second };
}


template <class T, class K>
static inline
auto u_find(const T &c, const K &k, const T &alt)
{
  if (auto found = u_find(c, k))
    return *found;
  else
    return alt;
}

template <class T, class K>
static inline
auto u_find_ptr(const T &c, const K &k)
{
  auto found = c.find(k);

  if (found == c.end())
    return nullptr;

  return &found->second;
}

template <class T, class K>
static inline
bool u_has(const T &c, const K &k)
{
  return c.find(k) != c.end();
}

template <class T, class ...Args>
static inline
T *u_construct(void *ptr, Args &&...args)
{
  return new (ptr) T(std::forward<Args>(args)...);
}

template <class T>
static inline
std::enable_if_t<!std::is_trivially_destructible<T>::value>
u_destruct(T *ptr)
{
  ptr->~T();
}

template <class T>
static inline
std::enable_if_t<std::is_trivially_destructible<T>::value>
u_destruct(T *) { }

template <class T, class ...Args>
static inline
T *u_reconstruct(T *ptr, Args &&...args)
{
  u_destruct(ptr);
  return u_construct<T>(ptr, std::forward<Args>(args)...);
}

// }}}


// {{{ defer

namespace details {

using RAII_Helper_Closure = std::function<void (void)>;

class RAII_Helper
{
  RAII_Helper_Closure closure;
public:
  RAII_Helper() : closure() { }

  template <class Lambda>
  RAII_Helper(Lambda &&lambda)
    : closure(std::forward<Lambda>(lambda))
  { }

  RAII_Helper(RAII_Helper &&)       = default;
  RAII_Helper(const RAII_Helper &)  = delete;

  RAII_Helper &operator =(RAII_Helper &&)      = default;
  RAII_Helper &operator =(const RAII_Helper &) = default;

  inline void retract() { closure = [] { }; }

  inline void commit()
  {
    this->~RAII_Helper();
    new (this) RAII_Helper;
  }

  ~RAII_Helper() { if (closure) closure(); }

  template <typename T, class D>
  inline T *manage(T *ptr, D &&deletor)
  {
    auto old = std::move(closure);
    closure  = [ptr, pre = std::move(old), deletor = std::forward<D>(deletor)]
    {
      deletor(ptr);
      if (pre)
        pre();
    };

    return ptr;
  }

  template <typename T, class D>
  inline const T *manage(const T *ptr, D &&deletor)
  {
    auto old = std::move(closure);
    closure  = [ptr, pre = std::move(old), deletor = std::forward<D>(deletor)]
    {
      deletor(ptr);
      if (pre)
        pre();
    };

    return ptr;
  }

  template <typename T>
  inline T *manage(T *ptr)
  {
    return manage(ptr, [] (T *ptr) { delete ptr; });
  }

  template <typename T>
  inline const T *manage(const T *ptr)
  {
    return manage(ptr, [] (T *ptr) { delete ptr; });
  }
};

struct RAII_Helper_Candy
{
  template <class Lambda>
  inline RAII_Helper operator +(Lambda &&lambda) const
  {
    return RAII_Helper(std::forward<Lambda>(lambda));
  }
};

} // namespace details

#define $defer           ::Li::details::RAII_Helper_Candy() + []
#define $defer_with(...) ::Li::details::RAII_Helper_Candy() + [__VA_ARGS__]


static inline
bool u_check_bit(uint32_t pattern, uint32_t mask)
{
  return (pattern & mask) == mask;
}

static inline
bool u_check_bit(uint64_t pattern, uint64_t mask)
{
  return (pattern & mask) == mask;
}

attr_export Str u_name_demangle(const Str &mangled_name);

template <typename T>
static inline
Str u_pretty_typename()
{
  return u_name_demangle(typeid (T).name());
}

// }}}

// }}}

} // namespace Li

#endif // end of include guard: MISC_H_INCLUDED_UBK6EJIX

