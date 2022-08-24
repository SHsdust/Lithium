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

#include "entity.h"
#include "core/direction.h"
#include "core/updatectx.h"
#include "core/component.h"
#include "core/trigger.h"
#include "components.h"
#include "books/spellbook.h"
#include "books/alchemybook.h"

namespace Li {

class Entity : public $chain( Unit
                            , (Copy_Via_CCTOR, Entity)
                            , (Pretty_Runtime_Type, Entity))
{
public:
  Entity(vec2_t initial_position)
  {
    auto *tag       = new Tag             { { }, { } };
    auto *actor     = new Actor           { };
    auto *particle  = new Particle        { initial_position };
    auto *trigger   = new Default_Trigger { };
    auto *direction = new Direction       { };

    tag->model.vanishing_duration      = 0.035f;
    tag->model.dying_duration          = 0.08f;
    tag->model.spawning_duration       = 0.03f;
    tag->model.off_screen_collect_span = 0.3f;

    actor->task = c_unit();

    replace_component(CID_Particle,  own<Component>(particle));
    replace_component(CID_Actor,     own<Component>(actor));
    replace_component(CID_Tag,       own<Component>(tag));
    replace_component(CID_Trigger,   own<Component>(trigger));
    replace_component(CID_Direction, own<Component>(direction));
  }

  void join_task(Ref_Ptr<Task> &&task)
  {
    auto *actor = query_component(CID_Actor)->as<Actor>();

    auto before = std::move(actor->task.ptr);
    actor->task = c_compose_eagerly(std::move(task), std::move(before));
  }

  void reset_task(Ref_Ptr<Task> &&task)
  {
    query_component(CID_Actor)->as<Actor>()->task = std::move(task);
  }
};


Unit *e_bless(Unit *entity, real_t capacity)
{
  entity->replace_component(CID_Damage_Stat,
                            own<Component>(new Damage_Stat(capacity)));

  return entity;
}

Unit *e_empower(Unit *entity, real_t p)
{
  entity->replace_component(CID_Power_Stat,
                            own<Component>(new Parameterized_Power_Stat(
                                f_const(p))));

  return entity;
}

Unit *e_join(Unit *entity, Ref_Ptr<Task> &&task)
{
  static_cast<Entity *>(entity)->join_task(std::move(task));

  return entity;
}

Unit *e_reset(Unit *entity, Ref_Ptr<Task> &&task)
{
  static_cast<Entity *>(entity)->reset_task(std::move(task));

  return entity;
}

Ref_Ptr<Unit> e_body(vec2_t initial_position)
{
  auto body = new Entity(initial_position);

  return own<Unit>(body);
}

} // namespace Li

