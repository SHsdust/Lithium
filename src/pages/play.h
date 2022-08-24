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

#ifndef PLAY_H_INCLUDED_802UTVUP
#define PLAY_H_INCLUDED_802UTVUP

#include "config.h"

namespace Li {

struct Play_Config;
class  Input_Source;

class Game_Play : public $chain( Page
                               , (Copy_Via_CCTOR, Game_Play)
                               , (Pretty_Runtime_Type, Game_Play))
{
  class Game_Play_Impl;
  Ptr<Game_Play_Impl>     pimpl;
  Interval_Timer<real_t>  route_interval = { 0.9f };
  bool                    will_route     = false;
public:
  Game_Play(const Play_Config &pc,
            Ptr<Input_Source> &&input_source,
            Game_Instance *game_instance);

  Game_Play(const Game_Play &);

  ~Game_Play();

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;
private:
  virtual void do_update(const Update_Ctx &uc) override final;
};

} // namespace Li

#endif // end of include guard: PLAY_H_INCLUDED_802UTVUP

