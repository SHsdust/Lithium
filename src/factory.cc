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

#include "factory.h"
#include "support/misc.h"
#include "support/debug.h"
#include "entity.h"
#include "books/spellbook.h"
#include "books/alchemybook.h"
#include "core/particle.h"
#include "core/CID.h"

namespace Li {

void Obj_Factory::register_obj(const Str  &key,
                               Ptr<Unit> &&obj)
{
  Li_Assert(obj.get() != nullptr);

  if (u_has(reg, key))
    Li_PanicF("[Obj_Factory] obj prototype [%s] already existed!", key);

  reg[key] = std::move(obj);
}

Ref_Ptr<Unit> Obj_Factory::acquire_obj(const Str &key, bool silence)
{
  if (!u_has(reg, key))
  {
    if (!silence)
      Li_WarnF("[Obj_Factory] query [%s] not found.", key);

    return nullptr;
  }

  auto &prototype = reg[key];

  return own(prototype->clone_as<Unit>());
}

Obj_Factory::Obj_Factory()
{ }

Ref_Ptr<Unit> Obj_Factory::acquire_simple_bullet(Unit *spawner,
                                                 Ptr<Task> &&task)
{
  vec2_t initial_position = { };

  if (spawner)
  {
    if (auto *particle = spawner->query_component(CID_Particle)->as<Particle>())
      initial_position = particle->get_position();
  }

  auto body = e_body(initial_position);

  e_join(body.get(), std::move(task));

  /*
  if (!spawner)
    return own<Unit>(bullet);

  if (auto *particle = spawner->query_component(CID_Particle)->as<Particle>())
  {
    auto position = particle->get_position();

    auto bp = bullet->query_component(CID_Particle)->as<Particle>();

    bp->mark_next_position(particle->get_position());
    bp->mark_next_velocity(particle->get_velocity());
    bp->set_position(particle->get_position());
    bp->set_velocity(particle->get_velocity());

    bullet->join_task(c_move_rel({ f_const(position), f_const(vec2_t { } ) }));
  }
  */

  return body;
}

} // namespace Li

