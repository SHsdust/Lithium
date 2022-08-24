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

#ifndef FAILED_H_INCLUDED_DVQJTXBU
#define FAILED_H_INCLUDED_DVQJTXBU

#include "config.h"
#include "../statistic.h"
#include "../gameupdate.h"

namespace Li {

struct Play_Config;
class  Input_Source;

class  Game_Failed : public $chain( Page
                                  , (Copy_Via_CCTOR, Game_Failed)
                                  , (Pretty_Runtime_Type, Game_Failed))
{
  Statistic           statistic;
  Ptr<Game_Update>    game_update;

  Interval_Timer<real_t>  route_interval = { 0.3f };
  bool                    done = false;
public:
  Game_Failed(Statistic statistic, Ptr<Game_Update> &&game_update)
    : statistic(statistic)
    , game_update(std::move(game_update))
    , route_interval(0.3f)
  { }

  Game_Failed(const Game_Failed &g);

  ~Game_Failed();

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;

private:
  virtual void do_update(const Update_Ctx &uc) override;
};

} // namespace Li

#endif // end of include guard: FAILED_H_INCLUDED_DVQJTXBU

