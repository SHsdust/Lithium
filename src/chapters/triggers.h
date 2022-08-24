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

#ifndef TRIGGERS_H_INCLUDED_7CXHWXDF
#define TRIGGERS_H_INCLUDED_7CXHWXDF

#include "../core/updatectx.h"
#include "../core/trigger.h"

namespace Li {
namespace details {

attr_export
void insert_explo(const Update_Ctx &uc,
                  unsigned tcid,
                  unsigned ci,
                  unsigned nexplo,
                  unsigned pow);

} // namespace details

template <unsigned TCID, unsigned CI, unsigned nexplo, unsigned pow = 10>
struct On_Vanish_Explo
{
  static void on_vanish(const Update_Ctx &uc)
  { details::insert_explo(uc, TCID, CI, nexplo, pow); }
};

template <unsigned TCID, unsigned CI, unsigned nexplo, unsigned pow = 10>
struct On_Destory_Explo
{
  static void on_destroy(const Update_Ctx &uc)
  { details::insert_explo(uc, TCID, CI, nexplo, pow); }
};

template <unsigned IMG, unsigned CI, unsigned N, unsigned P>
class Explo_Trigger : public $chain( Trigger
                                   , (Copy_Via_CCTOR, Explo_Trigger<IMG, CI, N, P>)
                                   , (Pretty_Runtime_Type, Explo_Trigger<IMG, CI, N, P>)
                                   , (Default_Pre_Update)
                                   , (Default_Post_Update)
                                   , (T_On_Damage, Do_Nothing)
                                   , (T_On_Vanish, On_Vanish_Explo<IMG, CI, N, P>)
                                   , (T_On_Event, Do_Nothing)
                                   , (T_On_Desroy, On_Destory_Explo<IMG, CI, N, P>)
                                   , (T_On_Damage_Capacity, On_Damage_Capacity_Destroy)
                                   , (T_On_Hit, On_Hit_Vanish)
                                   , (T_On_Hit_By, Do_Nothing))
{ };

} // namespace Li

#endif // end of include guard: TRIGGERS_H_INCLUDED_7CXHWXDF

