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

#ifndef SCIENCEBOOK_CC_INCLUDED_MOB6RVZD
#define SCIENCEBOOK_CC_INCLUDED_MOB6RVZD

#include "sciencebook.h"
#include "../support.h"
#include <cmath>

namespace Li {

/*
 * c: change-value
 * d: duration
 * t: time            (0 -> d)
 * b: base-value
 */

/*
 * x: (0 -> d) ==> d * (0 -> 1)
 */

/*
 * @-tween(c, d, t, b) => b + c * @-fn(t / d)
 */


static inline
real_t Linear_fn_p(real_t x) { return x; }

static inline
real_t Linear_fn_v(real_t  ) { return 1; }

static inline
real_t Quadratic_fn_p(real_t x) { return x * x; }

static inline
real_t Quadratic_fn_v(real_t x) { return 2 * x; }

static inline
real_t Cubic_fn_p(real_t x) { return x * x * x; }

static inline
real_t Cubic_fn_v(real_t x) { return 3 * x * x; }

static inline
real_t Quartic_fn_p(real_t x) { return x * x * x * x; }

static inline
real_t Quartic_fn_v(real_t x) { return 4 * x * x * x; }

static inline
real_t Quintic_fn_p(real_t x) { return x * x * x * x * x; }

static inline
real_t Quintic_fn_v(real_t x) { return 5 * x * x * x * x; }

static inline
real_t Sinusoidal_fn_p(real_t x)
{ return std::sin(M_PI / 2 * x); }

static inline
real_t Sinusoidal_fn_v(real_t x)
{ return std::cos(M_PI / 2 * x) * M_PI / 2; }

static inline
real_t Exponential_fn_p(real_t x)
{ return std::pow(real_t(2), x); }

static inline
real_t Exponential_fn_v(real_t x)
{ return std::pow(real_t(2), x) * std::log(2); }

static inline
real_t Circular_fn_p(real_t x)
{ return std::sqrt(1 - x * x); }

static inline
real_t Circular_fn_v(real_t x)
{ return x * std::sqrt(1 / (1 - x * x)); }

#define Gen_Ease_In(Tween)                         \
  static inline real_t Li_Join(Tween, _Ease_In_p)  \
  (real_t b, real_t c, real_t d, real_t t)         \
  { return b + c * Li_Join(Tween, _fn_p)(t / d); } \
  static inline real_t Li_Join(Tween, _Ease_In_v)  \
  (real_t b, real_t c, real_t d, real_t t)         \
  { return b + c * Li_Join(Tween, _fn_v)(t / d); } \

#define Gen_Ease_Out(Tween)                                  \
  static inline real_t Li_Join(Tween, _Ease_Out_p)           \
  (real_t b, real_t c, real_t d, real_t t)                   \
  { return b + c * (1 - Li_Join(Tween, _fn_p)(1 - t / d)); } \
  static inline real_t Li_Join(Tween, _Ease_Out_v)           \
  (real_t b, real_t c, real_t d, real_t t)                   \
  { return b + c * (1 - Li_Join(Tween, _fn_v)(1 - t / d)); } \

#define Gen_Ease_In_Out(Tween)                                    \
  static inline real_t Li_Join(Tween, _Ease_In_Out_p)             \
  (real_t b, real_t c, real_t d, real_t t)                        \
  {                                                               \
    return t < d / 2                                              \
      ? b + c * Li_Join(Tween, _fn_p)(t / d * 2) / 2              \
      : b + c * (1 - Li_Join(Tween, _fn_p)(2 * (d - t) / d) / 2); \
  }                                                               \
  static inline real_t Li_Join(Tween, _Ease_In_Out_v)             \
  (real_t b, real_t c, real_t d, real_t t)                        \
  {                                                               \
    return t < d / 2                                              \
      ? b + c * Li_Join(Tween, _fn_v)(t / d * 2) / 2              \
      : b + c * (1 - Li_Join(Tween, _fn_v)(2 * (d - t) / d) / 2); \
  }

#define Gen_Ease_Out_In(Tween)                                  \
  static inline real_t Li_Join(Tween, _Ease_Out_In_p)           \
  (real_t b, real_t c, real_t d, real_t t)                      \
  {                                                             \
    return t < d / 2                                            \
      ? b + c * (1 - Li_Join(Tween, _fn_p)(1 - t / d * 2)) / 2  \
      : b + c * (1 + Li_Join(Tween, _fn_p)(t / d * 2 - 1)) / 2; \
  }                                                             \
  static inline real_t Li_Join(Tween, _Ease_Out_In_v)           \
  (real_t b, real_t c, real_t d, real_t t)                      \
  {                                                             \
    return t < d / 2                                            \
      ? b + c * (1 - Li_Join(Tween, _fn_v)(1 - t / d * 2)) / 2  \
      : b + c * (1 + Li_Join(Tween, _fn_v)(t / d * 2 - 1)) / 2; \
  }

#define Gen(G)   \
  G(Linear)      \
  G(Quadratic)   \
  G(Cubic)       \
  G(Quartic)     \
  G(Quintic)     \
  G(Sinusoidal)  \
  G(Exponential) \
  G(Circular)

Gen(Gen_Ease_In)     Gen(Gen_Ease_Out)
Gen(Gen_Ease_In_Out) Gen(Gen_Ease_Out_In)

using fn_t = real_t (*)(real_t, real_t, real_t, real_t);

class Tween : public $chain( R_Source
                           , (Copy_Via_CCTOR, Tween)
                           , (Pretty_Runtime_Type, Tween))
{
  fn_t tween_fn;
  real_t change_value,
         duration,
         base_value;
public:
  Tween(fn_t tween_fn,
        real_t change_value,
        real_t duration,
        real_t base_value)
    : tween_fn(tween_fn)
    , change_value(change_value)
    , duration(duration)
    , base_value(base_value)
  { }

private:
  real_t do_eval(const Update_Ctx &) override final
  {
    return tween_fn(base_value, change_value, duration, get_time_elpased());
  }
};

#define t_lambda(...) own((R_Source *) new Tween(__VA_ARGS__))


R_Source_Ptr t_query_tween(Tween_Type tt,
                           Ease_Policy ep,
                           real_t change_value,
                           real_t duration,
                           real_t base_value)
{
#define G_Case(Tween)                           \
  if (tt == Li_Join(TT_, Tween))                \
    return t_lambda(Li_Join(Tween, _, EP, _p),  \
                    change_value,               \
                    duration,                   \
                    base_value);

  if (ep == EP_Ease_In)
  {
#define EP Ease_In
    Gen(G_Case)
#undef  EP
  }

  if (ep == EP_Ease_Out)
  {
#define EP Ease_Out
    Gen(G_Case)
#undef  EP
  }

  if (ep == EP_Ease_In_Out)
  {
#define EP Ease_In_Out
    Gen(G_Case)
#undef  EP
  }

  if (ep == EP_Ease_Out_In)
  {
#define EP Ease_Out_In
    Gen(G_Case)
#undef  EP
  }

  Li_LogE("Unknown tween type. %d - %d.", tt, ep);

  return f_const(real_t(0));
#undef G_Case
}

R_Source_Ptr t_query_tween_v(Tween_Type tt,
                             Ease_Policy ep,
                             real_t change_value,
                             real_t duration,
                             real_t base_value = 0.0f)
{
#define G_Case(Tween)                           \
  if (tt == Li_Join(TT_, Tween))                \
    return t_lambda(Li_Join(Tween, _, EP, _v),  \
                    change_value,               \
                    duration,                   \
                    base_value);

  if (ep == EP_Ease_In)
  {
#define EP Ease_In
    Gen(G_Case)
#undef  EP
  }

  if (ep == EP_Ease_Out)
  {
#define EP Ease_Out
    Gen(G_Case)
#undef  EP
  }

  if (ep == EP_Ease_In_Out)
  {
#define EP Ease_In_Out
    Gen(G_Case)
#undef  EP
  }

  if (ep == EP_Ease_Out_In)
  {
#define EP Ease_Out_In
    Gen(G_Case)
#undef  EP
  }
  Li_LogE("Unknown tween type. %d - %d.", tt, ep);

  return f_const(real_t(0));
}


V_Source_Ptr
t_query_locus(vec2_t to, Tween_Type tt, Ease_Policy ep, real_t duration)
{
  auto x_tween = t_query_tween(tt, ep, to.x, duration);
  auto y_tween = t_query_tween(tt, ep, to.y, duration);

  return f_make_vector(std::move(x_tween), std::move(y_tween));
}

V_Source_Ptr
t_query_locus(vec2_t from, vec2_t to, Tween_Type tt, Ease_Policy ep, real_t duration)
{
  auto x_tween = t_query_tween(tt, ep, to.x - from.x, duration, from.x);
  auto y_tween = t_query_tween(tt, ep, to.y - from.y, duration, from.y);

  return f_make_vector(std::move(x_tween), std::move(y_tween));
}


} // namespace Li

#endif // end of include guard: SCIENCEBOOK_CC_INCLUDED_MOB6RVZD

