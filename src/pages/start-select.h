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

#ifndef START_SELECT_H_INCLUDED_LR8YP3HX
#define START_SELECT_H_INCLUDED_LR8YP3HX

#include "config.h"

namespace Li {

class Start_Select : public $chain( Page
                                  , (Copy_Via_CCTOR, Start_Select)
                                  , (Pretty_Runtime_Type, Start_Select))
{
public:
  enum Ship_Type { ST_Type_A
                 , ST_Type_B
                 , ST_Back_To_Main_Menu
                 , ST_Total
                 };
private:
  Interval_Timer<real_t> key_resp_interval;
  Interval_Timer<real_t> route_interval;

  int                    selection        = ST_Type_A;
  bool                   done             = false;
  int                    initial_life;
public:
  Start_Select(int initial_life);

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;
private:
  virtual void do_update(const Update_Ctx &uc) override final;

};

} // namespace Li

#endif // end of include guard: START_SELECT_H_INCLUDED_LR8YP3HX

