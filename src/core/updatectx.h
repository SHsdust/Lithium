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

#ifndef UPDATECTX_H_INCLUDED_7LMZYSRU
#define UPDATECTX_H_INCLUDED_7LMZYSRU

#include "../config.h"

namespace Li {

class Unit;
class Component;

struct TickTack_Ctx
{
  utick_t global_tick;
  utick_t delta_tick;
  real_t  global_time_elpased;
  real_t  delta_time_elpased;
};

struct Target_Ctx
{
  Unit      *target;   ///< 当前正在更新的对象

  Component *particle, ///< 粒子组件
            *direction,
            *actor,
            *collider,
            *trigger,
            *tag,
            *render;
};

class  Game_Instance;  ///< the game instance.
class  Game_Update;    ///< the stage updater.

struct World_Ctx
{
  real_t width, height;

  Game_Instance *game_instance;
  Game_Update   *game_world;
};

struct Update_Ctx
{
  TickTack_Ctx ticktack;
  World_Ctx    world;
  Target_Ctx   target;
};

} // namespace Li

#endif // end of include guard: UPDATECTX_H_INCLUDED_7LMZYSRU

