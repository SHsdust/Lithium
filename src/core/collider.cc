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

#include "collider.h"
#include "../support/debug.h"

namespace Li {

bool Collider::colli(Unit *self, Unit *that)
{
  auto that_collider = that->query_component(CID_Collider)->as<Collider>();

  if (that_collider->query_collision_model_type() == CMT_Special)
    return that_collider->special_dispatch(that, self);

  Collision_Model_Data cmd = query_collision_model_data();
  Collision_Model_Type cmt = query_collision_model_type();

  switch (cmt)
  {
  case CMT_Special:
    return special_dispatch(self, that);
  case CMT_Circle:
    return that_collider->do_circle(that, self, cmd.v1, cmd.f2);
  case CMT_Box:
    return that_collider->do_box(that, self, cmd.v1, cmd.v2);
  case CMT_Segment:
    return that_collider->do_segment(that, self, cmd.v1, cmd.v2);
  default:
    return false;
  }
}

} // namespace Li

