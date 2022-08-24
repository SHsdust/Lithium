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

#include "triggers.h"
#include "config.h"
#include "render.h"
#include "../RCmanager.h"

namespace Li {
namespace details {

void insert_explo(const Update_Ctx &uc,
                  unsigned tcid,
                  unsigned ci,
                  unsigned nexplo,
                  unsigned pow)
{
  auto *world = uc.world.game_world;
  auto *inst  = uc.world.game_instance;
  auto *rc    = inst->get_rc_mgr();
  auto *is    = world->get_input_source();
  auto *cr    = inst->get_factory();


  for (unsigned i = 3; i != nexplo + 3; ++i)
  {
    auto radius = is->acquire_random(0, 1000) / 1000.0f * 50.0f * pow / 2;
    auto theta  = is->acquire_random(0, 1000) / 1000.0f * M_PI * 2;
    auto span   = is->acquire_random(0, 1000) / 1000.0f * 2.9f * pow / 10;
    auto expr   = is->acquire_random(0, 1000) / 1000.0f * 20.0f * pow / 10;
    auto dest   = vec2_t { 0, radius }.rotate(theta);
    auto locus  = t_query_locus(dest, TT_Linear, EP_Ease_In, span);
    auto fade   = t_query_locus(vec2_t { expr, expr }, vec2_t { 0, 0 }, TT_Linear, EP_Ease_In, span);
    auto explo  = cr->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));
    auto clip   = rc->query_tex_clip(Tex_Clip_ID(tcid));

    e_reset(explo.get(), c_compose_eagerly(c_move(std::move(locus)), c_chain(c_guard(c_unit(), span), c_destory())));

    if (ci == 99)
      e_beautify(explo.get(), clip, Clip_Index(is->acquire_random(0, 8)), std::move(fade));
    else
      e_beautify(explo.get(), clip, Clip_Index(ci), std::move(fade));

    world->add_unit(std::move(explo), UG_Phoney);
  }
}

} // namespace details
} // namespace Li

