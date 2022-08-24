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

  auto ticks = ui.ticktack.delta_tick;

  if (sleep_interval.forward(ticks))
  {
    if (shoot_interval.next(ticks))
    {
      sleep_interval.reset();
    }
    else if (spawn_interval.next(ticks))
    {
      for (auto i = 0; i != 6; ++i)
      {
        real_t theta = M_PI * 2 / 6 * i;

        vec2_t radius      = { 200.0f, 0.0f };
        vec2_t spin_radius = { 100.0f, 0.0f };
        vec2_t spawn_pos   = { 0.0f, 250.0f };

        auto a1 = radius.rotate(theta + get_time_elpased());
        auto a2 = spin_radius.rotate(theta) + a1;

        auto step_1 = t_query_locus(a1, CT_Linear, EP_Ease_In, 2.0f);
        auto step_2 = t_query_locus(a1, a2, CT_Linear, EP_Ease_In, 2.0f);

        auto locus = f_plus(f_const(spawn_pos), f_chain(std::move(step_1), 2.0f, std::move(step_2)));

        auto move_action = c_move({ std::move(locus), f_const(vec2_t { 0, 0 }) });

        spawn(gl, std::move(move_action));
      }
    }
  }
}

} // namespace Li

