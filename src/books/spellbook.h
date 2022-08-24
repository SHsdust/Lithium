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

#ifndef SPELLBOOK_H_INCLUDED_3AWFWDT4
#define SPELLBOOK_H_INCLUDED_3AWFWDT4

#include "config.h"

namespace Li {

attr_export Task_Ptr c_nil();
attr_export Task_Ptr c_unit();

attr_export Task_Ptr c_id(Task_Ptr &&);

attr_export Task_Ptr c_interval(Task_Ptr &&, real_t interval_val);
attr_export Task_Ptr c_interval(Task_Ptr &&, R_Source_Ptr &&interval_val);

attr_export Task_Ptr c_interval(Task_Ptr &&, utick_t ticks);
attr_export Task_Ptr c_interval(Task_Ptr &&, U_Source_Ptr &&ticks);

attr_export Task_Ptr c_guard(Task_Ptr &&, real_t span);

attr_export Task_Ptr c_guard(Task_Ptr &&, utick_t ticks);

attr_export Task_Ptr c_chain(Task_Ptr &&, Task_Ptr &&);

attr_export Task_Ptr c_compose_eagerly(Task_Ptr &&, Task_Ptr &&);

attr_export Task_Ptr c_compose_patiently(Task_Ptr &&, Task_Ptr &&);

using Predict = std::function<bool (const Update_Ctx &)>;

attr_export Task_Ptr c_if_then_else(Predict &&pred,
                                    Task_Ptr &&then_procedure,
                                    Task_Ptr &&else_procedure);

attr_export Task_Ptr c_move(V_Source_Ptr &&);

attr_export Task_Ptr c_move_rel(V_Source_Ptr &&);

attr_export Task_Ptr c_wait(Arc<bool> signal, Task_Ptr  &&job);

} // namespace Li

#endif // end of include guard: SPELLBOOK_H_INCLUDED_3AWFWDT4

