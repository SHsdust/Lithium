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

#ifndef CONFIG_H_INCLUDED_NKLJLOBD
#define CONFIG_H_INCLUDED_NKLJLOBD

#include "../inithook.h"
#include "../instance.h"
#include "../gameupdate.h"
#include "../factory.h"
#include "../RCmanager.h"
#include "../core/updatectx.h"
#include "../inputsource.h"
#include "../SDLdefs.h"
#include "../entity.h"
#include "../components.h"
#include "../books.h"
#include "../core/simplecollider.h"
#include "../rendercontext.h"

#define qcomp(T) query_component(Li_Join(CID_, T))->as<T>()

namespace Li {

attr_export
Task_Ptr c_spawn_with_interval(Interval_Timer<real_t> timer,
                               V_Source_Ptr locus);

attr_export
V_Source_Ptr f_towards_player(real_t scale);

attr_export
V_Source_Ptr f_towards_player(Tween_Param tp);

} // namespace Li

#endif // end of include guard: CONFIG_H_INCLUDED_NKLJLOBD

