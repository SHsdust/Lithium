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

#ifndef CONFIG_H_INCLUDED_OP80IGJQ
#define CONFIG_H_INCLUDED_OP80IGJQ

#include "core/unit.h"
#include "core/updatectx.h"

namespace Li {

enum Key_Code { KC_Back
              , KC_Shoot
              , KC_Spell
              , KC_Special
              , KC_Left
              , KC_Right
              , KC_Up
              , KC_Down
              , KC_Mode
              , KC_Total
              , KC_Yes = KC_Shoot
              , KC_No  = KC_Back
              };

enum Updating_Group { UG_Player_Shoot   // -> Enemy_Ship
                    , UG_Player_Special // -> Enemy_Shoot, Enemy_Ship
                 // , UG_Player_Ship    // -> Item
                    , UG_Enemy_Shoot    // -> Player_Ship
                    , UG_Enemy_Special  // -> Player_Ship
                    , UG_Enemy_Ship     // -> Player_Ship
                    , UG_Item
                    , UG_Phoney
                    , UG_Total
                    };

constexpr static size_t Max_Layers = 6;
constexpr static real_t GAME_SCENE_W = 1000.0f;
constexpr static real_t GAME_SCENE_H = 1000.0f;

} // namespace Li

#endif // end of include guard: CONFIG_H_INCLUDED_OP80IGJQ

