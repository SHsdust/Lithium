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

#ifndef GAMEUPDATE_H_INCLUDED_V6DMTQYW
#define GAMEUPDATE_H_INCLUDED_V6DMTQYW

#include "gamecfg.h"
#include "statistic.h"

namespace Li {

class Stage;
class Input_Source;
struct Render_Context;

class Game_Update
{
  class Game_Update_Impl;
  Ptr<Game_Update_Impl> pimpl;
public:
   Game_Update(Game_Instance   *game_instance,
               Statistic       *statistic,
               Player_Resource *player_resource,
               Ref_Ptr<Stage> &&stage,
               Ref_Ptr<Unit>  &&player,
               Input_Source    *input_source);

  ~Game_Update();

  void update(real_t  dt, utick_t dticks);

  void render(const Render_Context &rc);

  void add_unit(Ref_Ptr<Unit> &&unit, Updating_Group ug);

  void clear_group(Updating_Group ug);

  void clear_units();

  bool finished() const;

  Statistic       *get_statistic();

  Player_Resource *get_player_resource();

  Unit            *get_player();

  const List<Ref_Ptr<Unit>> &get_enemy_list() const;

  Input_Source    *get_input_source();

  bool passed() const;
};

} // namespace Li

#endif // end of include guard: GAMEUPDATE_H_INCLUDED_V6DMTQYW

