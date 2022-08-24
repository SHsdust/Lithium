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

namespace Li {

class Player_Actor : public $chain( Task
                                  , (Copy_Via_CCTOR, Player_Actor)
                                  , (Pretty_Runtime_Type, Player_Actor))
{
public:
  Interval_Timer<utick_t> shoot_interval = { 2 };

  vec2_t off = { };
private:
  virtual bool do_exec(attr_unused const Update_Ctx &uc) override final
  {
    auto key_status = ::SDL_GetKeyboardState(nullptr);

    shoot_interval.forward(uc.ticktack.delta_tick);

    vec2_t motion   = { 0, 0 };
    real_t velocity = 1000.0f / 100;

    if (key_status[SDL_SCANCODE_LSHIFT])
      velocity *= .6;

    auto particle = static_cast<Particle *>(uc.target.particle);
    auto position = particle->get_position();

    if (key_status[SDL_SCANCODE_LEFT] && position.x > -500)
      motion.x -= velocity;

    if (key_status[SDL_SCANCODE_RIGHT] && position.x < 500.0f)
      motion.x += velocity;

    if (key_status[SDL_SCANCODE_DOWN] && position.y > -500.0f)
      motion.y -= velocity;

    if (key_status[SDL_SCANCODE_UP] && position.y < 500.0f)
      motion.y += velocity;

    off += motion;

    particle->off_next_position(off);

    if (shoot_interval.is_timed_out() && key_status[SDL_SCANCODE_Z])
    {
      auto *factory = uc.world.game_instance->get_factory();

      if (key_status[SDL_SCANCODE_LSHIFT])
      {
        for (auto i = -2; i != 3; ++i)
        {
          vec2_t target_pos = { 0, 1000.0f };

          auto locus  = t_query_locus({ i * 20,  std::abs(i) * 9 - 15 }, target_pos, TT_Quadratic, EP_Ease_In, 1.0f);
          auto bullet = factory->acquire_simple_bullet(uc.target.target, c_move(std::move(locus)));

          e_beautify(bullet.get(), uc.world.game_instance, TCID_GUN_BULLET, CI_PURPLE, f_const(vec2_t { 24.0f, 24.0f }));
          e_strengthen(bullet.get(), 12.0f);

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

          e_beautify(bullet.get(), uc.world.game_instance, TCID_GUN_BULLET, CI_CYAN, f_const(vec2_t { 24.0f, 24.0f }));
          e_strengthen(bullet.get(), 12.0f);

          uc.world.game_world->add_unit(std::move(bullet), UG_Player_Shoot);
        }
      }

      shoot_interval.reset();
    }

    /*
    auto *tag = uc.target.tag->as<Tag>();

    if (tag->model.status == Tag::Dead
      || tag->model.status == Tag::Vanished)
    {
      tag->model.status                = Tag::Spawning;
      tag->model_status.spawning_timer = 0;
    }
    */

    return true;
  }
};

Li_Game_Init("player", "", inst)
{
  auto player_template = e_body({ 0.0f, -400.0f });

  e_debug_beautify(player_template.get());
  e_join(player_template.get(), own<Task>(new Player_Actor));
  e_strengthen(player_template.get(), 12.0f);

  inst->get_factory()->register_obj("player", std::move(player_template));
}

} // namespace Li

