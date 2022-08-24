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

#include "direction.h"
#include "particle.h"

namespace Li {

vec2_t Direction::get_direction(const Update_Ctx &uc)
{ return dir.ptr->eval(uc); }

class D_Velocity_Direction : public $chain( V_Source
                                          , (Pretty_Runtime_Type, D_Velocity_Direction)
                                          , (Copy_Via_CCTOR, D_Velocity_Direction))
{
private:
  virtual vec2_t do_eval(const Update_Ctx &uc) override final
  { return uc.target.particle->as<Particle>()->get_velocity(); }
};

V_Source_Ptr d_velocity_direction()
{
  return own<V_Source>(new D_Velocity_Direction);
}

Direction::Direction()
  : dir(d_velocity_direction())
{ }

attr_export Unit *e_direct(Unit *entity, V_Source_Ptr &&head)
{
  entity->replace_component(CID_Direction,
                            own<Component>(new Direction(std::move(head))));

  return entity;
}

} // namespace Li

