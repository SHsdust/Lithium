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

#ifndef COLLIDER_H_INCLUDED_GHIKETOL
#define COLLIDER_H_INCLUDED_GHIKETOL

#include "config.h"
#include "../support/collision.h"

namespace Li {

class Collider : public Component
{
public:
  virtual Collision_Model_Type query_collision_model_type() const = 0;
  virtual Collision_Model_Data query_collision_model_data() const = 0;

  bool colli(Unit *self, Unit *that);
private:
  virtual bool special_dispatch(Unit *self,
                                Unit *that) = 0;

  virtual bool do_box(Unit *self,
                      Unit *that, /* that is of Box Model */
                      vec2_t centre,
                      vec2_t border) = 0;

  virtual bool do_circle(Unit *self,
                         Unit *that, /* that is of Circle Model */
                         vec2_t centre,
                         real_t radius) = 0;

  virtual bool do_segment(Unit *self,
                          Unit *that, /* that is of Segment Model */
                          vec2_t begin,
                          vec2_t end) = 0;
};

} // namespace Li

#endif // end of include guard: COLLIDER_H_INCLUDED_GHIKETOL

