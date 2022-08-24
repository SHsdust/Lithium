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

#ifndef PAUSE_H_INCLUDED_DD8WSK7X
#define PAUSE_H_INCLUDED_DD8WSK7X

#include "config.h"

namespace Li {

class Game_Pause : public $chain( Page
                                , (Copy_Via_CCTOR, Game_Pause)
                                , (Pretty_Runtime_Type, Game_Pause))
{
public:
  enum Pause_Menu_Entry { PME_Resume
                        , PME_Back
                        // , PME_Restart
                        , PME_Quit
                        , PME_Total
                        };
private:
  Interval_Timer<real_t>  key_resp_interval;
  Interval_Timer<real_t>  route_interval;
  Page                   *play;

  int                     selection         = PME_Resume;
  bool                    done              = false;
public:
  Game_Pause(Page *play);

  virtual void update_page(Route *route, real_t dt, utick_t dticks) override final;
  virtual void render_page(Route *route, const Render_Context &rc)  override final;
private:
  virtual void do_update(const Update_Ctx &uc) override final;
};

} // namespace Li

#endif // end of include guard: PAUSE_H_INCLUDED_DD8WSK7X

