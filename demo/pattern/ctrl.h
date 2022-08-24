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

#ifndef CTRL_H_INCLUDED_PSDURGJO
#define CTRL_H_INCLUDED_PSDURGJO

#include "li.h"

namespace Li {

class Ctrl : public $chain( Unit
                          , (Copy_Via_CCTOR, Ctrl)
                          , (Pretty_Runtime_Type, Ctrl)
                          )
{
public:
private:
  Interval_Timer<utick_t> shoot_interval = { 30 },
                          sleep_interval = { 90 },
                          spawn_interval = {  5 };

  Ref_Wrapper<R_Filter>   x_motion = t_query_tween(combinators::CT_Sinusoidal, combinators::EP_Ease_In, 100.0f, 100.0f),
                          y_motion = t_query_tween(combinators::CT_Sinusoidal, combinators::EP_Ease_In, 100.0f, 80.0f);

  virtual void do_update(const Update_Info &ui) override final;
};

} // namespace Li

#endif // end of include guard: CTRL_H_INCLUDED_PSDURGJO

