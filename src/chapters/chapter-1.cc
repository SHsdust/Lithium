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

#include "config.h"
#include "render.h"
#include "../support.h"
#include "triggers.h"
#include "../RCmanager.h"
#include "../chapter.h"
#include "../books.h"
#include "../core/simplecollider.h"
#include "../core/particle.h"

namespace Li {

class Trace_Bullet_1 : public $chain( Task
                                 , (Copy_Via_CCTOR, Trace_Bullet_1)
                                 , (Pretty_Runtime_Type, Trace_Bullet_1))
{
private:
  Interval_Timer<utick_t> shoot_interval = {  23 },
                          sleep_interval = { 125 },
                          spawn_interval = {   2 };

  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto ticks = uc.ticktack.delta_tick;

    if (sleep_interval.forward(ticks))
    {
      if (shoot_interval.next(ticks))
      {
        sleep_interval.reset();
      }
      else if (spawn_interval.next(ticks))
      {
        auto player_position = uc.world.game_world->get_player()->query_component(CID_Particle)->as<Particle>()->get_position();
        auto position = uc.target.particle->as<Particle>()->get_position();


        for (auto i = -1; i != 2; ++i)
        {
          auto e = (player_position - position).normalize();

          auto locus = f_rotate(t_query_locus(e * 700.0f, TT_Cubic, EP_Ease_In, 3.4f), f_const(real_t(M_PI / 5 * i)));

          auto bullet = uc.world.game_instance->get_factory()->acquire_simple_bullet(uc.target.target,
                                                                                     c_move(std::move(locus)));

          e_strengthen(e_beautify(bullet.get(), uc.world.game_instance, TCID_GRAIN, CI_RED, f_const(vec2_t { 22.0f, 22.0f })), 23.0f);

          uc.world.game_world->add_unit(std::move(bullet), UG_Enemy_Shoot);
        }
      }
    }

    return true;
  }
};

class Chapter_1 : public $chain( Chapter
                               , (Copy_Via_CCTOR, Chapter_1)
                               , (Pretty_Runtime_Type, Chapter_1)
                               , (C_Dummy_On_Leave)
                               , (C_Dummy_On_Enter))
{
public:
private:
  Interval_Timer<real_t> generate_timer = {  2.5f };
  Interval_Timer<real_t> pass_timer     = { 24.0f };

  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto dt    = uc.ticktack.delta_time_elpased;
    auto gw    = uc.world.game_world;

    if (pass_timer.forward(dt))
      return false;

    if (generate_timer.get_span() > 1.5f)
      generate_timer.set_span(generate_timer.get_span() - 0.01f);

    if (generate_timer.next(dt))
    {
      auto *is = gw->get_input_source();

      auto x = is->acquire_random(-500, 500);
      auto y = is->acquire_random(200, 500);

      auto initialze_pos = vec2_t { x, y };
      auto stop_pos_1    = vec2_t { x, y - 300.0f } - initialze_pos;
      auto stop_pos_2    = gw->get_player()->qcomp(Particle)->get_position();

      auto step_1 = t_query_locus(stop_pos_1, TT_Cubic, EP_Ease_In_Out, 1.0f);
      auto step_2 = t_query_locus(stop_pos_1, stop_pos_2, TT_Linear, EP_Ease_In, 2.3f);

      auto locus = f_plus(f_const(initialze_pos),
                          f_chain(std::move(step_1),
                                  1.0f,
                                  std::move(step_2)));

      auto motion = c_compose_patiently(c_move(std::move(locus)),
                                        own<Task>(new Trace_Bullet_1));

      auto ship = uc.world.game_instance->get_factory()->acquire_simple_bullet(nullptr, std::move(motion));

      auto *pship = ship.get();

      e_beautify(pship,
                 uc.world.game_instance->get_rc_mgr()->query_tex_clip(TCID_Enemy_B),
                 Clip_Index(0),
                 f_const(vec2_t { 42.0f, 49.0f }));

      e_strengthen(pship, 50.0f);

      pship->replace_component(CID_Trigger, own<Trigger>(new Explo_Trigger<TCID_PRE_IMG, CI_PURPLE, 10, 10>));
      pship->replace_component(CID_Damage_Stat, own<Component>(new Damage_Stat(1600.0f)));

      gw->add_unit(std::move(ship), UG_Enemy_Ship);
    }

    return true;
  }
};

Li_Game_Init("chapters.s-0011", "", inst)
{ inst->get_factory()->register_obj("chapters.s-0011", own<Unit>(new Chapter_1)); }

} // namespace Li

