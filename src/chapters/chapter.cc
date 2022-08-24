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
#include "support.h"
#include "triggers.h"
#include "../RCmanager.h"
#include "../chapter.h"
#include "../books.h"
#include "../core/simplecollider.h"
#include "../core/particle.h"
#include "../core/damagestat.h"

namespace Li {

// {{{
class TraceBullet : public $chain( Task
                                 , (Copy_Via_CCTOR, TraceBullet)
                                 , (Pretty_Runtime_Type, TraceBullet))
{
private:
  Interval_Timer<utick_t> shoot_interval = {  21 },
                          sleep_interval = {  60 },
                          spawn_interval = {   3 };

  bool done = false;
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (done)
      return false;

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

        auto e = (player_position - position).normalize();

        auto locus = t_query_locus(e * 700.0f, TT_Cubic, EP_Ease_In, 2.4f);

        auto bullet = uc.world.game_instance->get_factory()->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

        e_strengthen(e_beautify(bullet.get(), uc.world.game_instance, TCID_BALL_HUGE, CI_RED, f_const(vec2_t { 32.0f, 32.0f })), 12.0f);

        uc.world.game_world->add_unit(std::move(bullet), UG_Enemy_Shoot);

        done = true;
      }
    }

    return true;
  }
};
// }}}

class Test_Trigger : public $chain( Trigger
                                  , (Copy_Via_CCTOR, Test_Trigger)
                                  , (Pretty_Runtime_Type, Test_Trigger)
                                  , (Default_Pre_Update)
                                  , (Default_Post_Update)
                                  , (T_On_Damage, Do_Nothing)
                                  , (T_On_Vanish, On_Vanish_Explo<TCID_PRE_IMG, CI_RED, 9>)
                                  , (T_On_Event, Do_Nothing)
                                  , (T_On_Desroy, On_Destory_Explo<TCID_PRE_IMG, CI_RED, 9>)
                                  , (T_On_Damage_Capacity, On_Damage_Capacity_Destroy)
                                  , (T_On_Hit, Do_Nothing)
                                  , (T_On_Hit_By, Do_Nothing))
{
public:
  using Super::Super;
};

class Test_Chapter : public $chain( Chapter
                                  , (Copy_Via_CCTOR, Test_Chapter)
                                  , (Pretty_Runtime_Type, Test_Chapter)
                                  , (C_Dummy_On_Leave))
{
public:
  virtual void on_enter(Game_Update *, Game_Instance *) override final
  {
    Li_LogI("diff = %d", diffi);
  }
private:
  Interval_Timer<utick_t> sleep_interval = { 100,100 },
                          finish_wait    = { 100 };

  Ref_Wrapper<R_Source>   x_motion = t_query_tween(TT_Sinusoidal, EP_Ease_In, 100.0f, 100.0f),
                          y_motion = t_query_tween(TT_Sinusoidal, EP_Ease_In, 100.0f, 80.0f);

  Arc<bool>               emittor = Arc<bool>(new bool);

  real_t                  theta_off = 0;

  int                     n_way = 1;

  int                     diffi;

  bool                    done = false;
  Interval_Timer<real_t>  pass_wait = { 10.0f };
public:
  Test_Chapter(int diffi)
    : diffi(diffi)
  { }

  Test_Chapter(const Test_Chapter &) = default;
private:

  // {{{ the horrible do-exec method.
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto gw    = uc.world.game_world;
    auto ticks = uc.ticktack.delta_tick;

    if (done && pass_wait.next(ticks))
      return false;

    if (done)
      return true;

    if (n_way <= 13 * diffi && sleep_interval.next(ticks))
    {
      theta_off += M_PI / 12;
      *emittor   = true;
      emittor    = Arc<bool>(new bool(false));

      for (auto i = 0; i != n_way * diffi; ++i)
      {
        real_t theta = M_PI * 2 / (n_way * diffi) * i + theta_off;// + M_PI * std::sin(get_time_elpased());

        vec2_t radius      = { 200.0f, 0.0f };
        vec2_t spin_radius = { 100.0f, 0.0f };
        vec2_t spawn_pos   = {   0.0f, 0.0f };

        auto a1 = radius.rotate(theta);
        auto a2 = spin_radius.rotate(theta + get_time_elpased()) + a1;

        auto step_1 = t_query_locus(a1, TT_Linear, EP_Ease_In, 1.0f);
        auto step_2 = t_query_locus(a1, a2, TT_Cubic, EP_Ease_In, 2.0f);

        auto locus = f_plus(f_const(spawn_pos),
                            f_chain(std::move(step_1), 1.0f, std::move(step_2)));

        auto action = c_compose_patiently(c_move(std::move(locus)),
                                          c_wait(emittor, own<Task>(new TraceBullet)));

        auto bullet = uc.world.game_instance->get_factory()->acquire_simple_bullet(
          nullptr,
          std::move(action));

        auto *particle = bullet->query_component(CID_Particle)->as<Particle>();

        particle->set_position(spawn_pos);

        e_beautify(bullet.get(),
                   uc.world.game_instance->get_rc_mgr()->query_tex_clip(TCID_Enemy_A),
                   Clip_Index(0),
                   f_const(vec2_t { 42.0f, 49.0f }));

        e_strengthen(bullet.get(), 8.0f);

        bullet->replace_component(CID_Trigger, own<Component>(new Test_Trigger));

        bullet->replace_component(CID_Damage_Stat, own<Component>(new Damage_Stat(410.0f)));

        gw->add_unit(std::move(bullet), UG_Enemy_Ship);
      }

      ++n_way;
      auto span = sleep_interval.get_span();

      if (span > 70)
        sleep_interval.set_span(span - span / 10);
    }

    if (n_way > 13 * diffi && finish_wait.next(ticks))
      done = true;

    return true;
  }
  // }}}
};

Li_Game_Init("chapters.s-0101", "", inst)
{ inst->get_factory()->register_obj("chapters.s-0101", own<Unit>(new Test_Chapter(1))); }

Li_Game_Init("chapters.s-0102", "", inst)
{ inst->get_factory()->register_obj("chapters.s-0102", own<Unit>(new Test_Chapter(2))); }

Li_Game_Init("chapters.s-0103", "", inst)
{ inst->get_factory()->register_obj("chapters.s-0103", own<Unit>(new Test_Chapter(3))); }

} // namespace Li

