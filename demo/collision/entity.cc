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
#include "render.h"

namespace Li {

void Entity::do_update(const Update_Info &ui)
{
  auto &target_info = ui.target;

  Component *comps[] = {
    target_info.actor,
    target_info.tag,
    target_info.render,
    target_info.collider,
    target_info.particle };

  for (auto *com : comps)
    if (com)
      com->pre_update_component(ui);

  for (auto *com : comps)
    if (com)
      com->update(ui);

  for (auto *com : comps)
    if (com)
      com->post_update_component(ui);
}

Entity::Entity(combinators::Combinator_Ptr &&task)
{
  auto *tag      = new components::Tag { { }, { } };
  auto *actor    = new components::Actor;
  auto *particle = new components::Particle;
  auto *render   = new components::SDL_Render;

  actor->combinator = std::move(task);

  replace_component(CID_Particle, own((Component *)particle));
  replace_component(CID_Actor, own((Component *)actor));
  replace_component(CID_Tag, own((Component *)tag));
  replace_component(CID_Render, own((Component *)render));
}

} // namespace Li

