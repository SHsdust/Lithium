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

#ifndef MAIN_H_INCLUDED_F9X2TKWI
#define MAIN_H_INCLUDED_F9X2TKWI

#include "config.h"

namespace Li {

class Main_Page : public $chain( Page
                               , (Copy_Via_CCTOR, Main_Page)
                               , (Pretty_Runtime_Type, Main_Page))
{
public:
  enum Main_Menu_Entry { MME_Start /* select difficulty, player, then GO. */
                       , MME_Practice_Start  /* */
                       , MME_About
                       , MME_Quit
                       , MME_Total
                       };
private:
  Interval_Timer<real_t>  key_resp_interval;
  Interval_Timer<real_t>  route_interval;
  int                     selection         = MME_Start;
  bool                    done              = false;
public:
  Main_Page();

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;
private:
  virtual void do_update(const Update_Ctx &uc) override final;
};

} // namespace Li

#endif // end of include guard: MAIN_H_INCLUDED_F9X2TKWI

