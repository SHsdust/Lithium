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

#ifndef DAMAGESTAT_H_INCLUDED_LYQVOHO9
#define DAMAGESTAT_H_INCLUDED_LYQVOHO9

#include "component.h"

namespace Li {

class Damage_Stat : public $chain( Component
                                 , (Copy_Via_CCTOR, Damage_Stat)
                                 , (Pretty_Runtime_Type, Damage_Stat)
                                 , (Default_Pre_Update)
                                 , (Default_Post_Update)
                                 , (Default_Update))
{
  real_t capacity, damage = 0;
public:
  Damage_Stat(real_t capacity)
    : capacity(capacity)
  { }

  Damage_Stat(const Damage_Stat &) = default;

  void do_damage(const Update_Ctx &ui, Unit *self, real_t d);

  inline bool dead() const
  { return damage > capacity; }

  inline real_t health_status() const
  {
    if (dead())
      return 0;

    return 1.0f - damage / capacity;
  }

  inline real_t get_capacity() const
  { return capacity; }

  inline void set_capacity(real_t capacity)
  { this->capacity = capacity; }

  inline void set_damage(real_t damage)
  { this->damage = damage; }

  inline real_t get_damage() const
  { return damage; }

  inline void reset()
  { damage = 0; }
};

attr_export Unit *e_bless(Unit *entity, real_t capacity);

} // namespace Li

#endif // end of include guard: DAMAGESTAT_H_INCLUDED_LYQVOHO9

