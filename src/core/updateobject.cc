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

#include "updateobject.h"
#include "updatectx.h"

namespace Li {

bool Update_Object::need_update(const Update_Ctx &ui) const
{
  return last_tick != ui.ticktack.global_tick;
}

void Update_Object::update(const Update_Ctx &ui)
{
  if (last_tick == ui.ticktack.global_tick)
    return;

  last_tick     = ui.ticktack.global_tick;

  ticks        += ui.ticktack.delta_tick;
  time_elpased += ui.ticktack.delta_time_elpased;

  do_update(ui);
}

} // namespace Li

