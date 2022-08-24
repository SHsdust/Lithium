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

#ifndef MATHBOOK_H_INCLUDED_6KWAXZL2
#define MATHBOOK_H_INCLUDED_6KWAXZL2

#include "prototype.h"
#include "../support/memberdetect.h"

namespace Li {

namespace d {
namespace details {

struct Nil;

template <class T, bool = std::is_same<T, typename T::simpilified>::value>
struct Do_Reduce
{
  using result = typename Do_Reduce<typename T::simpilified>::result;
};

template <class T>
struct Do_Reduce<T, true>
{
  using result = T;
};

template <class T>
using Reduce = typename Do_Reduce<T>::result;

Li_Generate_Typedef_Detector(is_constant);
Li_Generate_Typedef_Detector(is_zero);

#define _gen_args(x) attr_unused double x = 0
#define args         Li_Map(_gen_args, $0, $1, $2, $3, $4, $5, $6, $7, $8, $9)
#define $args        $0, $1, $2, $3, $4, $5, $6, $7, $8, $9

#define _gen_template_arg(x) class x
#define _gen_function_arg(x) const x &
#define _gen_apply_arg(x)    x

#define _gen_template(...)  template <Li_Map(_gen_template_arg, __VA_ARGS__)>
#define $match(...)  _gen_template(__VA_ARGS__) static constexpr int _match()
#define match $match(L_, R_)
#define with(...)   _gen_template(__VA_ARGS__)                                  \
  static auto _match_case(Li_Map(_gen_function_arg, __VA_ARGS__))         \
    -> std::enable_if_t<_match<Li_Map(_gen_apply_arg, __VA_ARGS__)>() ==
#define when(N) with(L_, R_) N,
#define then(...)  __VA_ARGS__>

template <bool p>
struct mark_constant_if { };

template <>
struct mark_constant_if<true>
{ using is_constant = std::true_type; };

template <bool p>
struct mark_zero_if { };

template <>
struct mark_zero_if<true>
{ using is_zero = std::true_type; };


template <class V>
struct Constant
  : mark_constant_if<true>
  , mark_zero_if<V::value() == 0>
{
  using value_type  = decltype(V::value());

  constexpr static inline auto value()
  { return V::value(); }

  constexpr static inline double apply(args)
  { return V::value(); }

  struct zero { constexpr static inline int value() { return 0; } };

  using deriv       = Constant<zero>;
  using simpilified = Constant<V>;

  static inline Str dump(attr_unused int level)
  {
    return std::to_string(value());
  }
};

template <typename T, T v>
struct Constant_Of_Helper
{
  constexpr static inline T value()
  { return v; }
};

template <typename T, T v>
using Constant_Of = Constant<Constant_Of_Helper<T, v>>;

#define $autod_make_constant(v)                           \
  Li_Prefix combinators::d::details::Constant<decltype(({ \
    struct _ { constexpr static inline auto value()       \
      { return v; } }; /* return */ _ result; }))>

struct Variable
{
  using is_variable = std::true_type;

  static inline double apply(args)
  { return $0; }

  using deriv       = Constant_Of<int, 1>;
  using simpilified = Variable;

  static inline Str dump(attr_unused int level)
  { return "t"; }
};

template <class L, class R>
struct Plus
  : mark_constant_if<has_typedef_is_constant<L>::value
                &&   has_typedef_is_constant<R>::value>
  , mark_zero_if<has_typedef_is_zero<L>::value
            &&   has_typedef_is_zero<R>::value>
{
  constexpr static auto value()
  { return apply(0); }

  constexpr static auto apply(args)
  { return L::apply($args) + R::apply($args); }

  static inline Str dump(int level)
  {
    if (level > 1)
      return "(" + dump(0) + ")";
    else
      return L::dump(0) + " + " + R::dump(0);
  }

  match
  {
    constexpr bool C = has_typedef_is_constant<L>::value
      &&               has_typedef_is_constant<R>::value;

    constexpr bool L_0 = has_typedef_is_zero<L>::value;
    constexpr bool R_0 = has_typedef_is_zero<R>::value;

    if (C)   return 0;
    if (L_0) return 1;
    if (R_0) return 2;

    return -1;
  }

  template <class L_, class R_>
  struct BinOp_Helper
  {
    constexpr static inline auto value()
    { return L_::apply(0) + R_::apply(0); }
  };

  when(0) then(Constant<BinOp_Helper<L_, R_>>);
  when(1) then(R_);
  when(2) then(L_);
  when(-1) then(Plus<L_, R_>);

  using SL = Reduce<L>;
  using SR = Reduce<R>;

  using simpilified = decltype(_match_case(SL(), SR()));

  using LD = typename L::deriv;
  using RD = typename R::deriv;

  using deriv = Plus<LD, RD>;
};

template <class L, class R>
struct Minus
  : mark_constant_if<has_typedef_is_constant<L>::value
                &&   has_typedef_is_constant<R>::value>
  , mark_zero_if<has_typedef_is_zero<L>::value
            &&   has_typedef_is_zero<R>::value>
{
  constexpr static auto value()
  { return apply(0); }

  constexpr static auto apply(args)
  { return L::apply($args) - R::apply($args); }

  static inline Str dump(int level)
  {
    if (level > 1)
      return "(" + dump(0) + ")";
    else
      return L::dump(0) + " - " + R::dump(0);
  }

  match
  {
    constexpr bool C = has_typedef_is_constant<L>::value
      &&               has_typedef_is_constant<R>::value;

    constexpr bool L_0 = has_typedef_is_zero<L>::value;
    constexpr bool R_0 = has_typedef_is_zero<R>::value;

    if (C)   return 0;
    if (L_0) return 1;
    if (R_0) return 2;

    return -1;
  }

  template <class L_, class R_>
  struct BinOp_Helper
  {
    constexpr static inline auto value()
    { return L_::apply(0) - R_::apply(0); }
  };

  when(0) then(Constant<BinOp_Helper<L_, R_>>);
  when(1) then(R_);
  when(2) then(L_);
  when(-1) then(Minus<L_, R_>);

  using SL = Reduce<L>;
  using SR = Reduce<R>;

  using simpilified = decltype(_match_case(SL(), SR()));

  using LD = typename L::deriv;
  using RD = typename R::deriv;

  using deriv = Minus<LD, RD>;
};

template <class L, class R>
struct Multiplies
  : mark_constant_if<has_typedef_is_constant<L>::value
                &&   has_typedef_is_constant<R>::value>
  , mark_zero_if<has_typedef_is_zero<L>::value
            &&   has_typedef_is_zero<R>::value>
{
  constexpr static auto value()
  { return apply(0); }

  constexpr static auto apply(args)
  { return L::apply($args) * R::apply($args); }

  static inline Str dump(int level)
  {
    if (level > 1)
      return "(" + dump(1) + ")";
    else
      return L::dump(1) + " * " + R::dump(1);
  }

  match
  {
    constexpr bool C = has_typedef_is_constant<L>::value
      &&               has_typedef_is_constant<R>::value;

    constexpr bool L_0 = has_typedef_is_zero<L>::value;
    constexpr bool R_0 = has_typedef_is_zero<R>::value;

    if (C)   return 0;
    if (L_0) return 1;
    if (R_0) return 1;

    return -1;
  }

  when(0) then(Constant<Multiplies<L_, R_>>);
  when(1) then(Constant_Of<int, 0>);
  when(-1) then(Multiplies<L_, R_>);

  using SL = Reduce<L>;
  using SR = Reduce<R>;

  using simpilified = decltype(_match_case(SL(), SR()));

  using LD = typename L::deriv;
  using RD = typename R::deriv;

  using deriv = Plus<Multiplies<LD, R>, Multiplies<L, RD>>;
};

#undef _gen_args
#undef args
#undef $args
#undef _gen_template_arg
#undef _gen_function_arg
#undef _gen_apply_arg
#undef _gen_template
#undef $match
#undef match
#undef with
#undef when
#undef then

} // namespace details
} // namespace d

} // namespace Li

#endif // end of include guard: MATHBOOK_H_INCLUDED_6KWAXZL2

