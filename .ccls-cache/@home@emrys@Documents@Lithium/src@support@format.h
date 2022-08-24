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

#ifndef FORMAT_H_INCLUDED_YDJDCX68
#define FORMAT_H_INCLUDED_YDJDCX68

#include "../prelude.h"
#include "traits.h"

namespace Li {
namespace details {

struct Formatter { };

template <size_t N>
static inline const char *
operator <<(const Formatter &, const char (&c_str)[N])
{
  return c_str;
}

static inline const char *
operator <<(const Formatter &, const Str &s)
{
  return s.c_str();
}

static inline const char *
operator <<(const Formatter &, const Str_View &sv)
{
  return sv.data();
}

template <typename T, class Y, bool = std::is_class<std::decay_t<T>>::value>
struct Format_Unwrapper
{
  static inline auto unwrap(const T &arg)
  {
    Formatter unwrapper;

    using N = decltype(unwrapper << arg);
    using X = decltype(std::tuple_cat(std::tuple<T>(std::declval<T>()),
                                      std::declval<Y>()));

    static_assert(!traits::has_dup_type_t<X>::value, "Bad recursion detected!!!");

    return Format_Unwrapper<N, X>::unwrap(unwrapper << arg);
  }
};

template <typename T, class Y>
struct Format_Unwrapper<T, Y, false>
{
  static inline auto unwrap(const T &arg)
  {
    return arg;
  }
};

template <typename T>
static inline auto format_unwrap(const T &arg)
{
  return Format_Unwrapper<T, std::tuple<traits::details::Nil>>::unwrap(arg);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"

template <typename ...Args>
static inline
Str format_impl_inplacement(char          *buff,
                            size_t         buff_len,
                            size_t         fmt_len,
                            const char    *fmt,
                            const Args &...args)
{
#ifdef LI_FORMAT_ALLOC
  if (buff_len < fmt_len)
  {
    auto new_buff = Ptr<char []>(new char[fmt_len]);

    return format_impl_inplacement(new_buff.get(),
                                   fmt_len,
                                   fmt_len,
                                   fmt_and_args);
  }
#else
  if (buff_len < fmt_len)
  {
    using namespace std::string_literals;

    return __PRETTY_FUNCTION__ + " : BUFF太小！！！"s;
  }
#endif

  std::snprintf(buff, buff_len, fmt, args...);

  return buff;
}

template <typename ...Args>
static inline
Str format_impl(const char *fmt, const Args &...args)
{
  constexpr size_t DEFAULT_BUFF_SIZE = 512 * 8;

  char buff[DEFAULT_BUFF_SIZE + 1];

  return format_impl_inplacement(buff,
                                 DEFAULT_BUFF_SIZE,
                                 std::snprintf(nullptr, 0, fmt, args...),
                                 fmt,
                                 args...);
}

#pragma GCC diagnostic pop

template <class TArgs, std::size_t ...I>
static inline
Str invoke_format(const char *fmt, const TArgs &targs, std::index_sequence<I...>)
{
  return format_impl(fmt, std::get<I>(targs)...);
}

} // namespace details

template <typename ...Args>
static inline
Str format(const Str &fmt, const Args &...args)
{
  auto targs = std::make_tuple(details::format_unwrap(args)...);

  return details::invoke_format(fmt.c_str(),
                                targs,
                                std::make_index_sequence<std::tuple_size<decltype(targs)>::value>());
}

} // namespace Li

#endif // end of include guard: FORMAT_H_INCLUDED_YDJDCX68

