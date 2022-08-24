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

#include "tag.h"
#include "particle.h"
#include "updatectx.h"
#include "trigger.h"
#include "../support/debug.h"

namespace Li {

static inline
bool is_point_inside_box(vec2_t point,
                         vec2_t centre,
                         real_t w,
                         real_t h)
{
  auto l = centre.x - w / 2,
       r = centre.x + w / 2,
       b = centre.y - h / 2,
       t = centre.y + h / 2;

  if (point.y > t || point.y < b || point.x < l || point.x > r)
    return false;
  else
    return true;
}

void Tag::do_update(const Update_Ctx &uc)
{
  auto &config = this->model;

  if (config.status == Dead || config.status == Vanished)
    return;

  auto  dt = uc.ticktack.delta_time_elpased;

  if (config.status == Dying)
  {
    model_status.dying_timer += dt;
    if (model_status.dying_timer > config.dying_duration)
    {
      if (auto trigger = static_cast<Trigger *>(uc.target.trigger))
        trigger->on_destroy(uc);

      config.status = Dead;
    }

    return;
  }

  if (config.status == Vanishing)
  {
    model_status.vanishing_timer += dt;
    if (model_status.vanishing_timer > config.vanishing_duration)
    {
      if (auto trigger = static_cast<Trigger *>(uc.target.trigger))
        trigger->on_vanish(uc);

      config.status = Vanished;
    }

    return;
  }

  if (config.status == Spawning)
  {
    model_status.spawning_timer += dt;

    if (model_status.spawning_timer > config.spawning_duration)
      config.status = Alive;

    return;
  }

  // config.status == Alive.
  auto particle = uc.target.particle->as<Particle>();

  if (config.off_screen_auto_collect)
  {
    auto off_screen = !is_point_inside_box(particle->get_position(),
                                           { 0, 0 },
                                           uc.world.width,
                                           uc.world.height);

    if (off_screen)
    {
      model_status.off_screen_collect_timer += uc.ticktack.delta_time_elpased;

      if (model_status.off_screen_collect_timer > config.off_screen_collect_span)
      {
        config.status = Dying;

        return;
      }
    }
  }

  if (config.auto_collect)
  {
    model_status.collect_timer += dt;
    if (model_status.collect_timer > config.collect_span)
    {
      config.status = Dying;

      return;
    }
  }
}

class C_Destory : public $chain( Task
                               , (Copy_Via_CCTOR, C_Destory)
                               , (Pretty_Runtime_Type, C_Destory))
{
public:
  using Super::Super;

private:
  virtual bool do_exec(const Update_Ctx &uc) override
  {
    auto *tag = uc.target.tag->as<Tag>();

    tag->model.status = Tag::Dying;

    return true;
  }
};

class C_Vanish : public $chain( Task
                              , (Copy_Via_CCTOR, C_Vanish)
                              , (Pretty_Runtime_Type, C_Vanish))
{
public:
  using Super::Super;

private:
  virtual bool do_exec(const Update_Ctx &uc) override
  {
    auto *tag = uc.target.tag->as<Tag>();

    tag->model.status = Tag::Vanishing;

    return true;
  }
};

attr_export Ref_Ptr<Task> c_destory()
{ return own<Task>(new C_Destory); }

attr_export Ref_Ptr<Task> c_vanish()
{ return own<Task>(new C_Vanish); }

} // namespace Li

