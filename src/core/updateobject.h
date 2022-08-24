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

#ifndef UPDATEOBJECT_H_INCLUDED_JNIWBMAP
#define UPDATEOBJECT_H_INCLUDED_JNIWBMAP

#include "object.h"

namespace Li {

static constexpr utick_t NIL_TICK = std::numeric_limits<utick_t>::max();

struct Update_Ctx;

/**
 * the minimum unit of game updating.
 */
class Update_Object : public Object
{
  utick_t last_tick    = NIL_TICK;
  utick_t ticks        = 0;
  real_t  time_elpased = 0;

public:
  void update(const Update_Ctx &ui);
  bool need_update(const Update_Ctx &ui) const;

  inline auto get_ticks() const
  { return ticks; }

  inline auto get_time_elpased() const
  { return time_elpased; }
private:
  /**
   * update states.
   *
   * this method will be invoked in every frame, on every active
   * objects(units).
   *
   * `ui` contains informations about this update.
   */
  virtual void do_update(const Update_Ctx &ui) = 0;
};

template <class Component>
class Default_Update : public Component
{
private:
  virtual void do_update(attr_unused const Update_Ctx &ui) override final
  { /* nothing to be done. */ }
};


} // namespace Li

#endif // end of include guard: UPDATEOBJECT_H_INCLUDED_JNIWBMAP

