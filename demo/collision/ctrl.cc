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

#include "ctrl.h"
#include "spawn.h"
#include "support.h"

namespace Li {

void Ctrl::do_update(const Update_Info &ui)
{
  auto *gl = ui.world.game_loop;

  using namespace combinators;

  if (get_ticks() % 6 == 0)
  {
    auto theta_base_curve = t_query_curve(CT_Linear, EP_Ease_In_Out, 10.0f, 5.0f);
    auto theta_off        = f_const(get_time_elpased());
    auto rou_base_curve   = t_query_curve(CT_Linear, EP_Ease_Out_In, 400.f + (get_ticks() % 7) * 10, 5.0f);

    spawn(gl,
          c_move(std::make_pair(t_rotate(own(rou_base_curve->clone()->as<R_Filter>()),
                                         f_plus(own(theta_base_curve->clone()->as<R_Filter>()),
                                                f_plus(f_const(real_t(M_PI / 2)),
                                                       own(theta_off->clone()->as<R_Filter>())))),
                                f_const(vec2_t { 0, 0 }))));

    Li_TraceF("spawn.");
  }
}

} // namespace Li

