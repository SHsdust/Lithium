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

#ifndef INITHOOK_H_INCLUDED_ZEAOSZBL
#define INITHOOK_H_INCLUDED_ZEAOSZBL

#include "support/bootstrap.h"

namespace Li {

class Game_Instance;

namespace game_bootstrap {

using Hook_Routine = std::function<void (Game_Instance *)>;

attr_export void register_hook_routine(Hook_Routine &&hr);
attr_export void apply_hooks(Game_Instance *instance);

} // namespace game_bootstrap
} // namespace Li

#define Li_Game_Init(path, depscheme, inst)                                       \
  namespace { namespace Li_Cat2($Uniq_GI_, __LINE__) {                            \
    static                                                                        \
    void game_init_hook(::Li::Game_Instance *inst);                               \
  } }                                                                             \
  Li_Bootstrap_Append_Routine(Li_Prefix Str("game-init.") + path, early_init)     \
  { ::Li::game_bootstrap::register_hook_routine(                                  \
       Li_Cat2($Uniq_GI_, __LINE__)::game_init_hook); }                           \
  void Li_Cat2($Uniq_GI_, __LINE__)::game_init_hook(                              \
    ::Li::Game_Instance *inst)

#endif // end of include guard: INITHOOK_H_INCLUDED_ZEAOSZBL


