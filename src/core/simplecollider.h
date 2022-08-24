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

#ifndef SIMPLECOLLIDER_H_INCLUDED_KQRFNDA2
#define SIMPLECOLLIDER_H_INCLUDED_KQRFNDA2

#include "collider.h"

namespace Li {

template <class C>
class Colli_Dummy_Special_Dispatch : public C
{
public:
  virtual bool special_dispatch(Unit *, Unit *) override final
  { return false; }
};


template <class C>
class Colli_Simple_Do_Box : public C
{
public:
  virtual bool do_box(attr_unused Unit *self,
                      attr_unused Unit *that,
                      vec2_t centre,
                      vec2_t border) override final
  {
    auto cmt = this->query_collision_model_type();
    auto cmd = this->query_collision_model_data();

    switch (cmt)
    {
    case CMT_Circle:
      return is_colliding(circle_model_tag, box_model_tag,
                          cmd.v1, cmd.f2,
                          centre, border);

    case CMT_Box:
      return is_colliding(box_model_tag, box_model_tag,
                          cmd.v1, cmd.v2,
                          centre, border);

    case CMT_Segment:
      return is_colliding(segment_model_tag, box_model_tag,
                          cmd.v1, cmd.v2,
                          centre, border);
    default:
      return false;
    }
  }
};

template <class C>
class Colli_Simple_Do_Circle : public C
{
public:
  virtual bool do_circle(attr_unused Unit *self,
                         attr_unused Unit *that,
                         vec2_t centre,
                         real_t radius) override final
  {
    auto cmt = this->query_collision_model_type();
    auto cmd = this->query_collision_model_data();

    switch (cmt)
    {
    case CMT_Circle:
      return is_colliding(circle_model_tag, circle_model_tag,
                          cmd.v1, cmd.f2,
                          centre, radius);

    case CMT_Box:
      return is_colliding(box_model_tag, circle_model_tag,
                          cmd.v1, cmd.v2,
                          centre, radius);

    case CMT_Segment:
      return is_colliding(segment_model_tag, circle_model_tag,
                          cmd.v1, cmd.v2,
                          centre, radius);
    default:
      return false;
    }
  }
};

template <class C>
class Colli_Simple_Do_Segment : public C
{
public:
  virtual bool do_segment(attr_unused Unit *self,
                          attr_unused Unit *that,
                          vec2_t begin_point,
                          vec2_t end_point) override final
  {
    auto cmt = this->query_collision_model_type();
    auto cmd = this->query_collision_model_data();

    switch (cmt)
    {
    case CMT_Circle:
      return is_colliding(circle_model_tag, segment_model_tag,
                          cmd.v1, cmd.f2,
                          begin_point, end_point);

    case CMT_Box:
      return is_colliding(box_model_tag, segment_model_tag,
                          cmd.v1, cmd.v2,
                          begin_point, end_point);

    case CMT_Segment:
      return is_colliding(segment_model_tag, segment_model_tag,
                          cmd.v1, cmd.v2,
                          begin_point, end_point);
    default:
      return false;
    }
  }
};


class Simple_Collider : public $chain( Collider
                                     , (Colli_Dummy_Special_Dispatch)
                                     , (Colli_Simple_Do_Box)
                                     , (Colli_Simple_Do_Circle)
                                     , (Colli_Simple_Do_Segment)
                                     , (Default_Pre_Update)
                                     , (Default_Post_Update)
                                     , (Pretty_Runtime_Type, Simple_Collider)
                                     , (Copy_Via_CCTOR, Simple_Collider))
{
  Collision_Model_Type cmt;
  Collision_Model_Data cmd;
public:
  Simple_Collider(Collision_Model_Type cmt,
                  Collision_Model_Data cmd)
    : cmt(cmt), cmd(cmd)
  { }

  Simple_Collider(const Simple_Collider &) = default;
public:
  virtual Collision_Model_Type query_collision_model_type() const override final
  { return cmt; }

  virtual Collision_Model_Data query_collision_model_data() const override final
  { return cmd; }
private:
  virtual void do_update(const Update_Ctx &ui) override final;
};

static inline
Unit *e_strengthen(Unit *u, real_t radius)
{
  u->replace_component(CID_Collider,
                       own<Component>(new Simple_Collider(CMT_Circle, { vec2_t {}, radius })));

  return u;
}

} // namespace Li

#endif // end of include guard: SIMPLECOLLIDER_H_INCLUDED_KQRFNDA2

