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


#ifndef SCIENCEBOOK_H_INCLUDED_SQUKEP8A
#define SCIENCEBOOK_H_INCLUDED_SQUKEP8A

#include "prototype.h"
#include "alchemybook.h"

namespace Li {

// Tween.
enum Tween_Type { TT_Linear
                , TT_Quadratic
                , TT_Cubic
                , TT_Quartic
                , TT_Quintic
                , TT_Sinusoidal
                , TT_Exponential
                , TT_Circular
             // , TT_Elastic
             // , TT_Back
             // , TT_Bounce
                };

enum Ease_Policy { EP_Ease_In
                 , EP_Ease_Out
                 , EP_Ease_In_Out
                 , EP_Ease_Out_In
                 };

struct Tween_Param
{
  Tween_Type  tween_type;
  Ease_Policy ease_policy;
  real_t      change_value;
  real_t      duration;
  real_t      base_value = 0;
};

attr_export R_Source_Ptr t_query_tween(Tween_Type tt,
                                       Ease_Policy ep,
                                       real_t change_value,
                                       real_t duration,
                                       real_t base_value = 0);

static inline
auto t_query_tween(Tween_Param param)
{
  return t_query_tween(param.tween_type,
                       param.ease_policy,
                       param.change_value,
                       param.duration,
                       param.base_value);
}

attr_export
V_Source_Ptr
t_query_locus(vec2_t from, vec2_t to, Tween_Type tt, Ease_Policy ep, real_t duration);

attr_export
V_Source_Ptr
t_query_locus(vec2_t to, Tween_Type tt, Ease_Policy ep, real_t duration);


} // namespace Li

#endif // end of include guard: SCIENCEBOOK_H_INCLUDED_SQUKEP8A

