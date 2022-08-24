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
#include "triggers.h"
#include "../support.h"
#include "../RCmanager.h"
#include "../chapter.h"
#include "../books.h"
#include "../core/simplecollider.h"
#include "../core/particle.h"
#include "../core/direction.h"
#include "../core/damagestat.h"

namespace Li {

// {{{
class Rnd_Shoot : public $chain( Task
                               , (Copy_Via_CCTOR, Rnd_Shoot)
                               , (Pretty_Runtime_Type, Rnd_Shoot))
{
public:
  Rnd_Shoot(R_Source_Ptr &&radius_proto,
            R_Source_Ptr &&spin_proto)
    : spin_proto(std::move(spin_proto))
    , radius_proto(std::move(radius_proto))
  {
    spin   = this->spin_proto;
    radius = this->radius_proto;
  }

private:
  Interval_Timer<utick_t> shoot_interval = {  24 },
                          sleep_interval = {  34 },
                          spawn_interval = {   1 };

  Ref_Wrapper<R_Source>           spin;
  Ref_Wrapper<R_Source>           radius;

  Ref_Wrapper<R_Source>           spin_proto;
  Ref_Wrapper<R_Source>           radius_proto;

  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto ticks = uc.ticktack.delta_tick;

    if (sleep_interval.forward(ticks))
    {
      if (shoot_interval.next(ticks))
      {
        sleep_interval.reset();

        spin   = spin_proto;
        radius = radius_proto;
      }
      else if (spawn_interval.next(ticks))
      {
#define N_WAY 3
        for (int i = 0; i != N_WAY; ++i)
        {
          auto theta = M_PI * 2 / N_WAY * i + get_time_elpased();

          auto locus = f_rotate(own<R_Source>(radius.ptr->clone()), spin->eval(uc) + theta);
          auto bullet = uc.world.game_instance->get_factory()->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

          e_direct(e_strengthen(e_beautify(bullet.get(),
                                           uc.world.game_instance,
                                           TCID_STAR_SMALL,
                                           CI_BLUE,
                                           f_const(vec2_t { 16.0f, 16.0f })),
                       12.0f),
                   f_rotate(f_const(vec2_t { 1.0f, 0.0f }),
                            f_inc(0.0f, f_const(.1f))));

          uc.world.game_world->add_unit(std::move(bullet), UG_Enemy_Shoot);
        }

        for (int i = 0; i != N_WAY; ++i)
        {
          auto theta = M_PI * 2 / N_WAY * i - get_time_elpased();

          auto locus = f_rotate(own<R_Source>(radius.ptr->clone()), spin->eval(uc) + theta);
          auto bullet = uc.world.game_instance->get_factory()->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

          e_direct(e_strengthen(e_beautify(bullet.get(),
                                           uc.world.game_instance,
                                           TCID_STAR_SMALL,
                                           CI_RED,
                                           f_const(vec2_t { 16.0f, 16.0f })),
                       12.0f),
                   f_rotate(f_const(vec2_t { 1.0f, 0.0f }),
                            f_inc(0.0f, f_const(0.1f))));

          uc.world.game_world->add_unit(std::move(bullet), UG_Enemy_Shoot);
        }
      }
    }

    return true;
  }
};
// }}}

class Chapter_5 : public $chain( Chapter
                               , (Copy_Via_CCTOR, Chapter_5)
                               , (Pretty_Runtime_Type, Chapter_5)
                               , (C_Dummy_On_Leave))
{
  Interval_Timer<real_t> pass_timer     = { 34.0f };
public:
  virtual void on_enter(Game_Update *game_update, Game_Instance *game_instance) override final
  {
    auto *rc_mgr  = game_instance->get_rc_mgr();
    auto *creator = game_instance->get_factory();

    auto pos_1 = vec2_t { -200, 200 };
    auto pos_2 = vec2_t {  200, 200 };
    auto pos_3 = vec2_t {    0, 400 };

    auto step_1 = t_query_locus(pos_1, pos_2, TT_Quadratic, EP_Ease_In_Out, 3.0f);
    auto step_2 = t_query_locus(pos_2, pos_3, TT_Quadratic, EP_Ease_In_Out, 3.0f);
    auto step_3 = t_query_locus(pos_3, pos_1, TT_Quadratic, EP_Ease_In_Out, 3.0f);

    auto cyc = f_chain(std::move(step_1), 3.0f,
                       f_chain(std::move(step_2), 3.0f,
                               std::move(step_3)));

    auto rep = f_repeat(std::move(cyc), f_const(vec2_t { }), 9.0f, INF_LOOP);

    auto action1 = c_compose_patiently(c_move(std::move(rep)),
                                       own<Task>(new Rnd_Shoot(t_query_tween(TT_Quartic, EP_Ease_Out_In,  400.0f, 3.4f),
                                                               f_const(real_t(M_PI / 2)))));
    auto action2 = c_compose_patiently(std::move(action1),
                                       own<Task>(new Rnd_Shoot(t_query_tween(TT_Quartic, EP_Ease_In,  600.0f, 3.4f),
                                                               f_const(real_t(M_PI / 2 + 0.2f)))));

    auto boss = creator->acquire_simple_bullet(nullptr, std::move(action2));

    e_beautify(boss.get(),
               rc_mgr->query_tex_clip(TCID_Enemy_Boss),
               Clip_Index(0),
               f_const(vec2_t { 150.0f, 300.0f }));

    e_strengthen(boss.get(), 140.0f);

    e_direct(boss.get(), f_const(vec2_t { -1.0f,  0.0f }));

    boss->replace_component(CID_Damage_Stat, own<Component>(new Damage_Stat(18000.0f * 2)));
    boss->replace_component(CID_Trigger, own<Component>(new Explo_Trigger<TCID_PRE_IMG, 99, 100, 35>()));

    game_update->add_unit(std::move(boss), UG_Enemy_Ship);
  }

  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    return !pass_timer.next(uc.ticktack.delta_time_elpased);
  }
};

Li_Game_Init("chapters.s-0000", "", inst)
{ inst->get_factory()->register_obj("chapters.s-0000", own<Unit>(new Chapter_5)); }

Li_Game_Init("chapters.s-0106", "", inst)
{ inst->get_factory()->register_obj("chapters.s-0106", own<Unit>(new Chapter_5)); }

} // namespace Li


