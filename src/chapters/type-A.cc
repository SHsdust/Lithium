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
#include "../core/trigger.h"
#include "../core/powerstat.h"

namespace Li {

template <unsigned IMG, unsigned CI, unsigned N, unsigned P>
class Bullet_Explo_Trigger : public $chain( Trigger
                                          , (Copy_Via_CCTOR, Bullet_Explo_Trigger<IMG, CI, N, P>)
                                          , (Pretty_Runtime_Type, Bullet_Explo_Trigger<IMG, CI, N, P>)
                                          , (Default_Pre_Update)
                                          , (Default_Post_Update)
                                          , (T_On_Damage, Do_Nothing)
                                          , (T_On_Vanish, On_Vanish_Explo<IMG, CI, N, P>)
                                          , (T_On_Event, Do_Nothing)
                                          , (T_On_Desroy, On_Destory_Explo<IMG, CI, N, P>)
                                          , (T_On_Damage_Capacity, On_Damage_Capacity_Destroy)
                                          , (T_On_Hit, On_Hit_Vanish)
                                          , (T_On_Hit_By, Do_Nothing))
{ };

class TA_Trigger : public $chain( Trigger
                                , (Copy_Via_CCTOR, TA_Trigger)
                                , (Pretty_Runtime_Type, TA_Trigger)
                                , (Default_Pre_Update)
                                , (Default_Post_Update)
                                , (T_On_Damage, Do_Nothing)
                                , (T_On_Vanish, On_Vanish_Explo<TCID_PRE_IMG, CI_RED, 4>)
                                , (T_On_Event, Do_Nothing)
                                , (T_On_Desroy, On_Destory_Explo<TCID_PRE_IMG, CI_ORANGE, 20, 16>)
                                , (T_On_Damage_Capacity, Do_Nothing)
                                , (T_On_Hit, On_Hit_Vanish)
                                , (T_On_Hit_By, On_Hit_By_Destroy))
{
public:
  using Super::Super;
};

class TB_Trigger : public $chain( Trigger
                                , (Copy_Via_CCTOR, TB_Trigger)
                                , (Pretty_Runtime_Type, TB_Trigger)
                                , (Default_Pre_Update)
                                , (Default_Post_Update)
                                , (T_On_Damage, Do_Nothing)
                                , (T_On_Vanish, On_Vanish_Explo<TCID_PRE_IMG, CI_RED, 4>)
                                , (T_On_Event, Do_Nothing)
                                , (T_On_Desroy, On_Destory_Explo<TCID_PRE_IMG, CI_BLUE, 20, 16>)
                                , (T_On_Damage_Capacity, Do_Nothing)
                                , (T_On_Hit, On_Hit_Vanish)
                                , (T_On_Hit_By, On_Hit_By_Destroy))
{
public:
  using Super::Super;
};



class Type_A_Actor : public $chain( Task
                                  , (Copy_Via_CCTOR, Type_A_Actor)
                                  , (Pretty_Runtime_Type, Type_A_Actor))
{
public:
  Interval_Timer<utick_t> shoot_interval = { 4 };

  vec2_t off = { };
private:
  virtual bool do_exec(attr_unused const Update_Ctx &uc) override final
  {
    auto is = uc.world.game_world->get_input_source();

    shoot_interval.forward(uc.ticktack.delta_tick);

    vec2_t motion   = { 0, 0 };
    real_t velocity = 1000.0f / 100;

    if (is->query_key_state(KC_Mode))
      velocity *= .6;

    auto particle = static_cast<Particle *>(uc.target.particle);
    auto position = particle->get_position();

    if (is->query_key_state(KC_Left) && position.x > -500)
      motion.x -= velocity;

    if (is->query_key_state(KC_Right) && position.x < 500.0f)
      motion.x += velocity;

    if (is->query_key_state(KC_Down) && position.y > -500.0f)
      motion.y -= velocity;

    if (is->query_key_state(KC_Up) && position.y < 500.0f)
      motion.y += velocity;

    off += motion;

    particle->off_next_position(off);

    if (shoot_interval.is_timed_out() && is->query_key_state(KC_Shoot))
    {
      auto *factory = uc.world.game_instance->get_factory();

      if (is->query_key_state(KC_Mode))
      {
        for (auto i = -2; i != 3; ++i)
        {
          vec2_t target_pos = { 0, 1000.0f };

          auto locus  = t_query_locus({ i * 20,  std::abs(i) * 9 - 15 }, target_pos, TT_Quadratic, EP_Ease_In, 1.0f);
          auto bullet = factory->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

          e_beautify(bullet.get(), uc.world.game_instance, TCID_GUN_BULLET, CI_PURPLE, f_const(vec2_t { 24.0f, 24.0f }));
          e_strengthen(bullet.get(), 24.0f);
          e_empower(bullet.get(), f_const(24.0f));

          bullet->replace_component(CID_Trigger, own<Component>(new Bullet_Explo_Trigger<TCID_GUN_BULLET_FADE, CI_ORANGE, 8, 7>));

          uc.world.game_world->add_unit(std::move(bullet), UG_Player_Shoot);
        }
      }
      else
      {
        real_t dtheta = M_PI / 14;
        real_t base_theta = M_PI / 2;

        vec2_t radius = { 1000.0f, 0 };

        for (auto i = -2; i != 3; ++i)
        {
          auto target_pos = radius.rotate(base_theta + i * dtheta);
          auto locus = t_query_locus(target_pos, TT_Linear, EP_Ease_In, 1.0f);

          auto bullet = factory->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

          e_beautify(bullet.get(), uc.world.game_instance, TCID_GUN_BULLET, CI_ORANGE, f_const(vec2_t { 24.0f, 24.0f }));
          e_strengthen(bullet.get(), 24.0f);
          e_empower(bullet.get(), f_const(34.0f));

          bullet->replace_component(CID_Trigger, own<Component>(new Explo_Trigger<TCID_GUN_BULLET_FADE, CI_RED, 8, 12>));

          uc.world.game_world->add_unit(std::move(bullet), UG_Player_Shoot);
        }
      }

      shoot_interval.reset();
    }

    return true;
  }
};

class Trace_Enemy : public $chain( Task
                                 , (Copy_Via_CCTOR, Trace_Enemy)
                                 , (Pretty_Runtime_Type, Trace_Enemy))
{
  real_t velocity_scale;
  vec2_t last_velocity;
  vec2_t accum;

public:
  Trace_Enemy(real_t velocity_scale, vec2_t initial)
    : velocity_scale(velocity_scale)
    , last_velocity(initial)
    , accum({ })
  { }
private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto &elist = uc.world.game_world->get_enemy_list();

    if (!elist.empty())
    {
      auto &first_enemy = elist.back();

      auto *particle = first_enemy->qcomp(Particle);
      auto *self_p   = uc.target.target->qcomp(Particle);

      auto eposition = particle->get_position();
      auto sposition = self_p->get_position();

      auto dir = (eposition - sposition).normalize();

      last_velocity = last_velocity * 0.97f + dir * velocity_scale * 0.03f;
    }

    accum += last_velocity * uc.ticktack.delta_time_elpased;
    uc.target.particle->as<Particle>()->mark_next_position(accum);

    return true;
  }
};

class Type_B_Actor : public $chain( Task
                                  , (Copy_Via_CCTOR, Type_B_Actor)
                                  , (Pretty_Runtime_Type, Type_B_Actor))
{
public:
  Interval_Timer<utick_t> shoot_interval = { 2 };

  vec2_t off = { };
private:
  virtual bool do_exec(attr_unused const Update_Ctx &uc) override final
  {
    auto is = uc.world.game_world->get_input_source();

    shoot_interval.forward(uc.ticktack.delta_tick);

    vec2_t motion   = { 0, 0 };
    real_t velocity = 1000.0f / 95;

    if (is->query_key_state(KC_Mode))
      velocity *= .7;

    auto particle = static_cast<Particle *>(uc.target.particle);
    auto position = particle->get_position();

    if (is->query_key_state(KC_Left) && position.x > -500)
      motion.x -= velocity;

    if (is->query_key_state(KC_Right) && position.x < 500.0f)
      motion.x += velocity;

    if (is->query_key_state(KC_Down) && position.y > -500.0f)
      motion.y -= velocity;

    if (is->query_key_state(KC_Up) && position.y < 500.0f)
      motion.y += velocity;

    off += motion;

    particle->off_next_position(off);

    if (shoot_interval.is_timed_out() && is->query_key_state(KC_Shoot))
    {
      auto *factory = uc.world.game_instance->get_factory();

      if (is->query_key_state(KC_Mode))
      {
        for (auto i = -2; i != 3; ++i)
        {
          vec2_t target_pos = { 0, 1000.0f };

          auto locus  = t_query_locus({ i * 20,  std::abs(i) * 9 - 15 }, target_pos, TT_Quadratic, EP_Ease_In, 1.0f);
          auto bullet = factory->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

          e_beautify(bullet.get(), uc.world.game_instance, TCID_GUN_BULLET, CI_BLUE, f_const(vec2_t { 24.0f, 24.0f }));
          e_strengthen(bullet.get(), 12.0f);
          e_empower(bullet.get(), f_const(32.0f));

          bullet->replace_component(CID_Trigger, own<Component>(new Bullet_Explo_Trigger<TCID_PRE_IMG, CI_BLACK, 8, 12>));

          uc.world.game_world->add_unit(std::move(bullet), UG_Player_Shoot);
        }
      }
      else
      {
        for (auto i = -2; i != 3; ++i)
        {
          auto initial_velocity = vec2_t { 0, 1000.0f }.rotate(M_PI / 5 * i);

          auto bullet = factory->acquire_simple_bullet(uc.target.target, own<Task>(new Trace_Enemy(1000.0f, initial_velocity)));

          e_beautify(bullet.get(), uc.world.game_instance, TCID_GUN_BULLET, CI_BLUE, f_const(vec2_t { 24.0f, 24.0f }));
          e_strengthen(bullet.get(), 12.0f);
          e_empower(bullet.get(), f_const(12.0f));
          bullet->replace_component(CID_Trigger, own<Component>(new Bullet_Explo_Trigger<TCID_PRE_IMG, CI_BLACK, 8, 12>));

          uc.world.game_world->add_unit(std::move(bullet), UG_Player_Shoot);
        }
      }

      shoot_interval.reset();
    }

    return true;
  }
};

#define SHIP_BODY_W 50.0f
#define SHIP_BODY_H 80.0f

class Type_A_Render : public $chain( Render
                                   , (Copy_Via_CCTOR, Type_A_Render)
                                   , (Pretty_Runtime_Type, Type_A_Render)
                                   , (Default_Update)
                                   , (Default_Pre_Update)
                                   , (Default_Post_Update))
{
  Tex_Clip *tex_clip;
public:
  Type_A_Render(Tex_Clip *tex_clip)
    : tex_clip(tex_clip)
  { }

  virtual void render(const Render_Context &rc, const Update_Ctx &uc) override final
  {
    auto renderer = get_renderer();
    auto particle = uc.target.particle->as<Particle>();
    auto position = particle->get_position();
    auto velocity = particle->get_velocity();
    auto obj_rect = rc.transform(position, { SHIP_BODY_W, SHIP_BODY_H });

    if (uc.target.tag->as<Tag>()->model.status == Tag::Dead)
    {
      return;
    }

    if (uc.target.tag->as<Tag>()->model.status == Tag::Spawning)
    {
      if (get_ticks() % 5 == 0)
        return;
    }


    auto incline = velocity.x >  0.1 ? CI_INCLINE_INCLINE_RIGHT
                 : velocity.x < -0.1 ? CI_INCLINE_INCLINE_LEFT
                 :                     CI_NO_INCLINE;

    tex_clip->render_clip(renderer, obj_rect, incline);
  }
};

constexpr ::SDL_Rect type_A[] = { {   0, 0, 43, 48 }
                                , {  44, 0, 42, 50 }
                                , {  87, 0, 38, 50 }
                                , { 126, 0, 42, 50 }
                                , { 169, 0, 38, 50 } };

constexpr ::SDL_Rect type_B[] = { {   0, 50, 41, 58 }
                                , {  42, 50, 40, 58 }
                                , {  83, 50, 36, 58 }
                                , { 120, 50, 41, 58 }
                                , { 162, 50, 35, 58 } };


static inline
void load(RC_Mgr *mgr)
{
  auto tid = mgr->register_texture("res/player.png", "player", 134, 244, 244);

  mgr->register_tex_clips(tid, TCID_Type_A, { std::begin(type_A), std::end(type_A) });
  mgr->register_tex_clips(tid, TCID_Type_B, { std::begin(type_B), std::end(type_B) });
}

Li_Game_Init("reg.load.Type-A/B", "SDL-init", inst)
{
  auto handle = inst->get_init_handle();

  bootstrap::init_append_early_init(handle, "load.Type-A/B",
                                    [inst](bootstrap::Bootstrap_Args &,
                                           const bootstrap::Bootstrap_Reg &)
                                    {
                                      load(inst->get_rc_mgr());
                                    });
}

static inline void install_type_A(Game_Instance *inst)
{
  auto player_template = e_body({ 0.0f, -400.0f });

  auto clip = inst->get_rc_mgr()->query_tex_clip(TCID_Type_A);

  player_template->replace_component(CID_Render,
                                     own<Render>(
                                       new Type_A_Render(clip)));

  e_join(player_template.get(), own<Task>(new Type_A_Actor));
  e_strengthen(player_template.get(), 12.0f);

  player_template->replace_component(CID_Trigger, own<Component>(new TA_Trigger));

  auto *tag = player_template->qcomp(Tag);
  tag->model.spawning_duration = 2.0f;
  tag->model.off_screen_auto_collect = false;

  inst->get_factory()->register_obj("player-ship.Type-A", std::move(player_template));
}

static inline void install_type_B(Game_Instance *inst)
{
  auto player_template = e_body({ 0.0f, -400.0f });

  auto clip = inst->get_rc_mgr()->query_tex_clip(TCID_Type_B);

  player_template->replace_component(CID_Render,
                                     own<Render>(
                                       new Type_A_Render(clip)));

  e_join(player_template.get(), own<Task>(new Type_B_Actor));
  e_strengthen(player_template.get(), 11.0f);

  player_template->replace_component(CID_Trigger, own<Component>(new TB_Trigger));

  auto *tag = player_template->qcomp(Tag);
  tag->model.spawning_duration = 2.0f;
  tag->model.off_screen_auto_collect = false;

  inst->get_factory()->register_obj("player-ship.Type-B", std::move(player_template));
}


Li_Game_Init("reg.player-ship.Type-A/B", "", inst)
{
  auto handle = inst->get_init_handle();

  bootstrap::init_append_early_init(handle, "player-ship..Type-A/B",
                                    [inst](bootstrap::Bootstrap_Args &,
                                           const bootstrap::Bootstrap_Reg &)
                                    {
                                      install_type_A(inst);
                                      install_type_B(inst);
                                    });
}

} // namespace Li


