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

#ifndef ENTITY_H_INCLUDED_OHMYPI2S
#define ENTITY_H_INCLUDED_OHMYPI2S

#include "gamecfg.h"
#include "books/prototype.h"

namespace Li {

attr_export Ref_Ptr<Unit> e_body(vec2_t initial_position = { });

attr_export Unit *e_join(Unit *entity, Ref_Ptr<Task> &&task);
attr_export Unit *e_reset(Unit *entity, Ref_Ptr<Task> &&task);

} // namespace Li

#endif // end of include guard: ENTITY_H_INCLUDED_OHMYPI2S

