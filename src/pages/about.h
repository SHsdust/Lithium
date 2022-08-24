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

#ifndef ABOUT_H_INCLUDED_8HXOTISQ
#define ABOUT_H_INCLUDED_8HXOTISQ

#include "config.h"

namespace Li {

class About_Page : public $chain( Page
                                , (Copy_Via_CCTOR, About_Page)
                                , (Pretty_Runtime_Type, About_Page))
{
private:
  Interval_Timer<real_t>  key_resp_interval = { 0.2f };
  Interval_Timer<real_t>  route_interval    = { 0.2f };
  bool done = false;

public:

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;
private:
  virtual void do_update(const Update_Ctx &uc) override final;
};

} // namespace Li

#endif // end of include guard: ABOUT_H_INCLUDED_8HXOTISQ

