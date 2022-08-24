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

#ifndef STRING_H_INCLUDED_97J8CGTJ
#define STRING_H_INCLUDED_97J8CGTJ

#include "../prelude/stdincl.h"
#include "../prelude/stdalias.h"

namespace Li {

namespace details {

template <typename T>
static inline
std::enable_if_t<std::is_integral<T>::value, Str>
stringify_impl(T t)
{
  return std::to_string(t);
}

template <typename ...Args>
static inline
std::enable_if_t<std::is_constructible<Str, Args...>::value, Str>
stringify_impl(Args &&...args)
{
  return Str(std::forward<Args>(args)...);
}

template <typename T>
static inline
std::enable_if_t<!std::is_constructible<Str, T>::value && !std::is_integral<T>::value , Str>
stringify_impl(const T &t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

} // namespace details


template <typename ...Args>
static inline Str stringify(Args &&...args)
{
  return details::stringify_impl(std::forward<Args>(args)...);
}

// {{{ split / trim / join
template <template <typename ...> class Container = Seq>
static inline
Container<Str> str_split(const Str &input, const Str &sep)
{
  Container<Str> output;

  Str::size_type npos = Str::npos, f = 0, s = 0;
  auto           iter = std::back_inserter(output);

  while ((s = input.find_first_not_of(sep, f)) != npos)
  {
    f    = input.find_first_of(sep, s);
    iter = input.substr(s, f - s);
  }

  return output;
}



static inline Str str_trim_left(const Str &poor_guy)
{
  auto pos = poor_guy.find_first_not_of(" \t\n");

  return pos == Str::npos ? Str() : poor_guy.substr(pos);
}



static inline Str str_trim_right(const Str &poor_guy)
{
  auto pos = poor_guy.find_last_not_of(" \t\n");

  return pos == Str::npos ? Str() : poor_guy.substr(0, pos + 1);
}



static inline Str str_trim(const Str &poor_poor_guy)
{
  return str_trim_right(str_trim_left(poor_poor_guy));
}


template <class Input, class Lambda = std::function<Str (const Str &)>>
static inline
Str str_map_join(const Input &input,
                 const Str   &sep,
                 Lambda     &&lambda)
{
  auto iter = std::cbegin(input);
  if (iter == std::cend(input))
    return {};

  Str out = lambda(*iter++);

  while (iter != std::cend(input))
  {
    out += sep;
    out += lambda(*iter++);
  }

  return out;
}

template <class Input>
static inline Str str_join(const Input &input,
                           const Str   &sep)
{
  return str_map_join(input, sep, [](const Str &x) { return x; });
}

// }}}

// {{{ str_pad
static inline
Str str_pad_right(const Str &src, size_t pad, char fill = ' ')
{
  if (src.length() >= pad)
    return src;
  else
    return src + Str(pad - src.length(), fill);
}

static inline
Str str_pad_left(const Str &src, size_t pad, char fill = ' ')
{
  if (src.length() >= pad)
    return src;
  else
    return Str(pad - src.length(), fill) + src;
}
// }}}

} // namespace Li

#endif // end of include guard: STRING_H_INCLUDED_97J8CGTJ

