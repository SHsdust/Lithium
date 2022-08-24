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


#include "unit.h"
#include "component.h"
#include "../support/debug.h"
#include "../support/misc.h"

namespace Li {

Unit::Unit() { }

Unit::~Unit() { }

Unit::Unit(const Unit &unit)
{
  for (size_t i = 0; i != CID::Max_Components; ++i)
  {
    if (auto *comp = unit.components.at(i).get())
      components.at(i) = own(comp->clone_as<Component>());
  }
}

const Component *Unit::query_component(uid_t slot_id) const
{
  Li_Assert(slot_id < CID::Max_Components);

  return components.at(slot_id).get();
}

Component *Unit::query_component(uid_t slot_id)
{
  auto const_this = const_cast<const Unit *>(this);

  return const_cast<Component *>(const_this->query_component(slot_id));
}


Ref_Ptr<Component> Unit::replace_component(uid_t CID,
                                           Ref_Ptr<Component> &&new_component)
{
  Li_Assert(CID < Max_Components);

  auto old_comp = std::move(components.at(CID));

  components.at(CID) = std::move(new_component);

  return old_comp;
}

void Unit::do_update(const Update_Ctx &ui)
{
  for (auto &&com : components)
    if (com) com->pre_update_component(ui);

  for (auto &&com : components)
    if (com) com->update(ui);

  for (auto &&com : components)
    if (com) com->post_update_component(ui);
}

} // namespace Li

