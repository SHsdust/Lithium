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

#include "inithook.h"
#include "instance.h"
#include "support.h"

namespace Li {
namespace game_bootstrap {

using Hook_Routine_List = Seq<Hook_Routine>;

static
Hook_Routine_List *g_hr_list()
{
  static Hook_Routine_List hr;
  return &hr;
}

void register_hook_routine(Hook_Routine &&hr)
{ g_hr_list()->emplace_back(std::move(hr)); }

void apply_hooks(Game_Instance *instance)
{
  for (auto &&routine : *g_hr_list())
    routine(instance);
}

} // namespace game_bootstrap
} // namespace Li

