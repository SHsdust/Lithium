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

#ifndef CONGRAT_H_INCLUDED_BCXL24PD
#define CONGRAT_H_INCLUDED_BCXL24PD




#include "config.h"
#include "../statistic.h"
#include "../gameupdate.h"

namespace Li {

struct Play_Config;
class  Input_Source;

class  Congrat : public $chain( Page
                              , (Copy_Via_CCTOR, Congrat)
                              , (Pretty_Runtime_Type, Congrat))
{
  Statistic           statistic;
  Ptr<Game_Update>    game_update;

  Interval_Timer<real_t>  route_interval = { 0.3f };
  bool                    done = false;
public:
  Congrat(Statistic statistic, Ptr<Game_Update> &&game_update)
    : statistic(statistic)
    , game_update(std::move(game_update))
    , route_interval(0.3f)
  { }

  Congrat(const Congrat &g);

  ~Congrat();

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;

private:
  virtual void do_update(const Update_Ctx &uc) override;
};

} // namespace Li


#endif // end of include guard: CONGRAT_H_INCLUDED_BCXL24PD

