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

#ifndef PARTICLE_H_INCLUDED_WJC02SIF
#define PARTICLE_H_INCLUDED_WJC02SIF

#include "config.h"

namespace Li {


class Particle : public $chain
                        ( Component
                        , (Default_Pre_Update)
                        , (Default_Update)
                        , (Copy_Via_CCTOR, Particle)
                        , (Pretty_Runtime_Type, Particle)
                        )
{
  vec2_t cyc[2];
  vec2_t initial_position;
  vec2_t velocity;

  size_t        in_using = 0;

  inline vec2_t &get_pure_in_using()
  { return cyc[in_using]; }

  inline vec2_t &get_pure_next()
  { return cyc[get_next()]; }

  inline void apply_next_state(real_t dt)
  {
    velocity = (get_next_position() - get_position()) / dt;

    // cyc[in_using] = cyc[get_next()];
    in_using = get_next();
    cyc[get_next()] = { };
  }

  inline size_t get_in_using() const
  { return in_using; }

  inline size_t get_next() const
  { return 1u - in_using; }

public:
  inline vec2_t  get_pure_in_using() const
  { return cyc[in_using]; }

  inline vec2_t  get_pure_next() const
  { return cyc[get_next()]; }

public:
  Particle(vec2_t initial_position = { 0, 0 })
    : cyc { }
    , initial_position(initial_position)
    , velocity { }
  { }

public:
  using Super::Super;
public:
  inline void mark_next_position(vec2_t position)
  { get_pure_next() = position; }

  inline void set_position(vec2_t position)
  { get_pure_in_using() = position; }

  inline void off_next_position(vec2_t position)
  { get_pure_next() += position; }

  inline vec2_t get_position() const
  { return get_pure_in_using() + initial_position; }

  inline vec2_t get_next_position() const
  { return get_pure_next() + initial_position; }

  inline vec2_t get_velocity() const
  { return velocity; }
private:
  /* */
  virtual void do_post_update_component(const Update_Ctx &ui) override final;
};

} // namespace Li

#endif // end of include guard: PARTICLE_H_INCLUDED_WJC02SIF

