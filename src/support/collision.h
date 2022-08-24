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

#ifndef COLLISION_H_INCLUDED_PGYMRBXD
#define COLLISION_H_INCLUDED_PGYMRBXD

#include "../config.h"
#include "math.h"

namespace Li {

namespace details {

static inline
bool same_dir_approx(vec2_t A, vec2_t B)
{
  return dot_product(A, B) > 0;
}

static inline
bool diff_dir_approx(vec2_t A, vec2_t B)
{
  return dot_product(A, B) < 0;
}

static inline
bool same_dir_approx_i(vec2_t A, vec2_t B)
{
  return dot_product(A, B) >= 0;
}

static inline
bool diff_dir_approx_i(vec2_t A, vec2_t B)
{
  return dot_product(A, B) <= 0;
}

} // namespace details

enum Collision_Model_Type
{
  CMT_Special,
  CMT_Point,
  CMT_Line,
  CMT_Ray,
  CMT_Segment,
  CMT_Box,
  CMT_Circle,
};

struct Collision_Model_Data
{
  union {
    vec2_t centre;
    vec2_t begin_point;
    vec2_t point;
    vec2_t v1;
  };

  union {
    vec2_t   direction;
    vec2_t   end_point;
    vec2_t   border;
    real_t radius;

    vec2_t   v2;
    real_t f2;
  };

  Collision_Model_Data(const vec2_t v1, const vec2_t v2)
    : v1(v1), v2(v2) { }

  Collision_Model_Data(const vec2_t v1, real_t f2)
    : v1(v1), f2(f2) { }
};

struct Collision_Model
{
  Collision_Model_Type type;
  Collision_Model_Data data;
};

namespace details {
template <uint32_t E, uint32_t T>
struct Model_Tag
{
  struct prio { enum uint32_t { value = E }; };
  struct take { enum uint32_t { value = T }; };
};

using Point_Model_Tag   = Model_Tag<CMT_Point, 1>;
using Line_Model_Tag    = Model_Tag<CMT_Line, 2>;
using Ray_Model_Tag     = Model_Tag<CMT_Ray, 2>;
using Segment_Model_Tag = Model_Tag<CMT_Segment, 2>;
using Box_Model_Tag     = Model_Tag<CMT_Box, 2>;
using Circle_Model_Tag  = Model_Tag<CMT_Circle, 2>;

} // namespace details

constexpr static details::Point_Model_Tag   point_model_tag   = { };
constexpr static details::Line_Model_Tag    line_model_tag    = { };
constexpr static details::Ray_Model_Tag     ray_model_tag     = { };
constexpr static details::Segment_Model_Tag segment_model_tag = { };
constexpr static details::Box_Model_Tag     box_model_tag     = { };
constexpr static details::Circle_Model_Tag  circle_model_tag  = { };

namespace details {

/**
 * Point x Point
 */
static inline
bool is_colliding(const Point_Model_Tag &,
                  const Point_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t rpoint)
{
  return almost_equal(lpoint, rpoint);
}

/**
 * Point x Line
 */
static inline
bool is_colliding(const Point_Model_Tag &,
                  const Line_Model_Tag  &,
                  vec2_t lpoint,
                  vec2_t rpoint,
                  vec2_t rdirection)
{
  return almost_zero(cross_product(rpoint - lpoint, rdirection));
}

/**
 * Point x Ray
 */
static inline
bool is_colliding(const Point_Model_Tag &,
                  const Ray_Model_Tag   &,
                  vec2_t lpoint,
                  vec2_t rpoint,
                  vec2_t rdirection)
{
  return is_colliding(point_model_tag, line_model_tag, lpoint, rpoint, rdirection)
    &&   details::same_dir_approx_i(lpoint - rpoint, rdirection);
}

/**
 * Point x Segment
 */
static inline
bool is_colliding(const Point_Model_Tag   &,
                  const Segment_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t rbegin_point,
                  vec2_t rend_point)
{
  return is_colliding(point_model_tag, ray_model_tag, lpoint, rbegin_point, rend_point - rbegin_point)
    &&   is_colliding(point_model_tag, ray_model_tag, lpoint, rbegin_point, rbegin_point - rend_point);
}

/**
 * Point x Box
 */
static inline
bool is_colliding(const Point_Model_Tag &,
                  const Box_Model_Tag   &,
                  vec2_t lpoint,
                  vec2_t rcentre,
                  vec2_t rborder)
{
  if ( lpoint.x > rcentre.x + rborder.x
    || lpoint.x < rcentre.x - rborder.x
    || lpoint.y > rcentre.y + rborder.y
    || lpoint.y < rcentre.y - rborder.y)
  {
    return false;
  }

  return true;
}

/**
 * Point x Circle
 */
static inline
bool is_colliding(const   Point_Model_Tag  &,
                  const   Circle_Model_Tag &,
                  vec2_t   lpoint,
                  vec2_t   rcentre,
                  real_t radius)
{
  return radius * radius >= length_sq(rcentre - lpoint);
}

/**
 * Line x Line
 */
static inline
bool is_colliding(const Line_Model_Tag &,
                  const Line_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rpoint,
                  vec2_t rdirection)
{
  return !almost_parallel(ldirection, rdirection)
    &&   !almost_parallel(ldirection, lpoint - rpoint);
}

/**
 * Line x Ray
 */
static inline
bool is_colliding(const Line_Model_Tag &,
                  const Ray_Model_Tag  &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rpoint,
                  vec2_t rdirection)
{
  if (almost_parallel(ldirection, rdirection))
    return !almost_parallel(ldirection, lpoint - rpoint);

  auto cross = intersection_point(lpoint, ldirection, rpoint, rdirection);

  return details::same_dir_approx_i(cross - lpoint, ldirection);
}

/**
 * Line x Segment
 */
static inline
bool is_colliding(const Line_Model_Tag    &,
                  const Segment_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rbegin_point,
                  vec2_t rend_point)
{
  if (almost_parallel(ldirection, rend_point - rbegin_point))
    return !almost_parallel(ldirection, rbegin_point - lpoint);

  auto cross = intersection_point(lpoint, ldirection, rbegin_point, rend_point - rbegin_point);

  return details::same_dir_approx_i(cross - rbegin_point, cross - rend_point);
}

/**
 * Line x Box
 */
static inline
bool is_colliding(const Line_Model_Tag &,
                  const Box_Model_Tag  &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rcentre,
                  vec2_t rborder)
{
  vec2_t vertices[] =
  { { rcentre.x - rborder.x, rcentre.y + rborder.y }
  , { rcentre.x + rborder.x, rcentre.y + rborder.y }
  , { rcentre.x + rborder.x, rcentre.y - rborder.y }
  , { rcentre.x - rborder.x, rcentre.y - rborder.y } };

  if ( is_colliding(line_model_tag, segment_model_tag, lpoint, ldirection, vertices[0], vertices[1])
    || is_colliding(line_model_tag, segment_model_tag, lpoint, ldirection, vertices[1], vertices[2])
    || is_colliding(line_model_tag, segment_model_tag, lpoint, ldirection, vertices[2], vertices[3])
    || is_colliding(line_model_tag, segment_model_tag, lpoint, ldirection, vertices[3], vertices[0]))
  {
    return true;
  }

  return false;
}

/**
 * Line x Circle
 */
static inline
bool is_colliding(const   Line_Model_Tag   &,
                  const   Circle_Model_Tag &,
                  vec2_t   lpoint,
                  vec2_t   ldirection,
                  vec2_t   rcentre,
                  real_t rradius)
{
  auto altitude = rcentre - intersection_point(lpoint, ldirection, rcentre, { -ldirection.y, ldirection.x });

  return length_sq(altitude) <= rradius * rradius;
}

/**
 * Ray x Ray
 */
static inline
bool is_colliding(const Ray_Model_Tag &,
                  const Ray_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rpoint,
                  vec2_t rdirection)
{
  if (almost_parallel(ldirection, rdirection))
    return is_colliding(point_model_tag, line_model_tag, lpoint, rpoint, rdirection);

  vec2_t cross = intersection_point(lpoint, ldirection, rpoint, rdirection);

  if ( details::diff_dir_approx(cross - lpoint, ldirection)
    || details::diff_dir_approx(cross - rpoint, rdirection))
  {
    return false;
  }

  return true;
}

/**
 * Ray x Segment
 */

static inline
bool is_colliding(const Ray_Model_Tag     &,
                  const Segment_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rbegin_point,
                  vec2_t rend_point)
{
  if (almost_parallel(ldirection, rbegin_point - rend_point))
    return !almost_parallel(ldirection, rbegin_point - lpoint);

  auto cross = intersection_point(lpoint, ldirection, rbegin_point, rend_point - rbegin_point);

  if (details::same_dir_approx_i(cross - lpoint, ldirection))
    return true;

  return details::same_dir_approx_i(cross - rbegin_point, cross - rend_point);
}

/**
 * Ray x Box
 */
static inline
bool is_colliding(const Ray_Model_Tag &,
                  const Box_Model_Tag &,
                  vec2_t lpoint,
                  vec2_t ldirection,
                  vec2_t rcentre,
                  vec2_t rborder)
{
  const vec2_t vertices[] =
  { { rcentre.x - rborder.x, rcentre.y + rborder.y }
  , { rcentre.x + rborder.x, rcentre.y + rborder.y }
  , { rcentre.x + rborder.x, rcentre.y - rborder.y }
  , { rcentre.x - rborder.x, rcentre.y - rborder.y } };

  if ( is_colliding(ray_model_tag, segment_model_tag, lpoint, ldirection, vertices[0], vertices[1])
    || is_colliding(ray_model_tag, segment_model_tag, lpoint, ldirection, vertices[1], vertices[2])
    || is_colliding(ray_model_tag, segment_model_tag, lpoint, ldirection, vertices[2], vertices[3])
    || is_colliding(ray_model_tag, segment_model_tag, lpoint, ldirection, vertices[3], vertices[0]))
  {
    return true;
  }

  return false;
}

/**
 * Ray x Circle
 */
static inline
bool is_colliding(const   Ray_Model_Tag    &,
                  const   Circle_Model_Tag &,
                  vec2_t   lpoint,
                  vec2_t   ldirection,
                  vec2_t   rcentre,
                  real_t rradius)
{
  if (is_point_inside_circle(lpoint, rcentre, rradius))
    return true;

  vec2_t cross = intersection_point(lpoint, ldirection, rcentre, { -ldirection.y, ldirection.x });

  if (details::same_dir_approx_i(cross - lpoint, ldirection))
    return true;

  return false;
}

/**
 * Segment x Segment
 */
static inline
bool is_colliding(const Segment_Model_Tag &,
                  const Segment_Model_Tag &,
                  vec2_t lbegin_point,
                  vec2_t lend_point,
                  vec2_t rbegin_point,
                  vec2_t rend_point)
{
  return is_intersect({ lbegin_point, lend_point }, { rbegin_point, rend_point });
}

/**
 * Segment_Model_Tag & Box_Model_Tag
 */
static inline
bool is_colliding(const Segment_Model_Tag &,
                  const Box_Model_Tag     &,
                  vec2_t lbegin_point,
                  vec2_t lend_point,
                  vec2_t rcentre,
                  vec2_t rborder)
{
  if ( is_colliding(point_model_tag, box_model_tag, lbegin_point, rcentre, rborder)
    || is_colliding(point_model_tag, box_model_tag, lend_point, rcentre, rborder))
  {
    return true;
  }

  const vec2_t vertices[] =
  { { rcentre.x - rborder.x, rcentre.y + rborder.y }
  , { rcentre.x + rborder.x, rcentre.y + rborder.y }
  , { rcentre.x + rborder.x, rcentre.y - rborder.y }
  , { rcentre.x - rborder.x, rcentre.y - rborder.y } };

  if ( is_colliding(segment_model_tag, segment_model_tag, lbegin_point, lend_point, vertices[0], vertices[1])
    || is_colliding(segment_model_tag, segment_model_tag, lbegin_point, lend_point, vertices[1], vertices[2])
    || is_colliding(segment_model_tag, segment_model_tag, lbegin_point, lend_point, vertices[2], vertices[3])
    || is_colliding(segment_model_tag, segment_model_tag, lbegin_point, lend_point, vertices[3], vertices[0]))
  {
    return true;
  }

  return false;
}

/**
 * Segment x Circle
 */
static inline
bool is_colliding(const   Segment_Model_Tag &,
                  const   Circle_Model_Tag  &,
                  vec2_t   lbegin_point,
                  vec2_t   lend_point,
                  vec2_t   rcentre,
                  real_t rradius)
{
  if ( is_point_inside_circle(lbegin_point, rcentre, rradius)
    || is_point_inside_circle(lend_point, rcentre, rradius))
  {
    return true;
  }

  if ( is_colliding(ray_model_tag, circle_model_tag, lbegin_point, lend_point - lbegin_point, rcentre, rradius)
    && is_colliding(ray_model_tag, circle_model_tag, lend_point, lbegin_point - lend_point, rcentre, rradius))
  {
    return true;
  }

  return false;
}

/**
 * Box x Box
 */
static inline
bool is_colliding(const Box_Model_Tag &,
                  const Box_Model_Tag &,
                  vec2_t lcentre,
                  vec2_t lborder,
                  vec2_t rcentre,
                  vec2_t rborder)
{
  if ( lcentre.x + lborder.x < rcentre.x - rborder.x
    || lcentre.x - lborder.x > rcentre.x + rborder.x
    || lcentre.y + lborder.y < rcentre.y - rborder.y
    || lcentre.y - lborder.y > rcentre.y + rborder.y)
  {
    return false;
  }

  return true;
}

/**
 * Box x Circle
 */
static inline
bool is_colliding(const   Box_Model_Tag    &,
                  const   Circle_Model_Tag &,
                  vec2_t   lcentre,
                  vec2_t   lborder,
                  vec2_t   rcentre,
                  real_t rradius)
{
  // XXX: 2016-03-08 08:22:50
  const vec2_t vertices[] =
  { { lcentre.x - lborder.x, lcentre.y + lborder.y }
  , { lcentre.x + lborder.x, lcentre.y + lborder.y }
  , { lcentre.x + lborder.x, lcentre.y - lborder.y }
  , { lcentre.x - lborder.x, lcentre.y - lborder.y } };

  if ( is_colliding(point_model_tag, circle_model_tag, vertices[0], rcentre, rradius)
    || is_colliding(point_model_tag, circle_model_tag, vertices[1], rcentre, rradius)
    || is_colliding(point_model_tag, circle_model_tag, vertices[2], rcentre, rradius)
    || is_colliding(point_model_tag, circle_model_tag, vertices[3], rcentre, rradius))
  {
    return true;
  }

  if ( is_colliding(segment_model_tag, circle_model_tag, vertices[0], vertices[1], rcentre, rradius)
    || is_colliding(segment_model_tag, circle_model_tag, vertices[1], vertices[2], rcentre, rradius)
    || is_colliding(segment_model_tag, circle_model_tag, vertices[2], vertices[3], rcentre, rradius)
    || is_colliding(segment_model_tag, circle_model_tag, vertices[3], vertices[0], rcentre, rradius))
  {
    return true;
  }

  auto rdir = (rcentre - lcentre).normalize() * rradius;

  return is_colliding(point_model_tag, box_model_tag, rdir, lcentre, lborder);
}

/**
 * Circle x Circle
 */
static inline
bool is_colliding(const Circle_Model_Tag &,
                  const Circle_Model_Tag &,
                  vec2_t lcentre,
                  real_t lradius,
                  vec2_t rcentre,
                  real_t rradius)
{
  return length_sq(lcentre - rcentre) <= lradius * lradius + rradius * rradius;
}


template <class M1, class M2, bool = (uint32_t(M1::prio::value) < uint32_t(M2::prio::value))>
struct Collision_Test_Dispatch_Helper
{
  template <class ...Args>
  static inline
  bool apply(const M1   &m1,
             const M2   &m2,
             const Args &...args)
  {
    return ::Li::details::is_colliding(m1, m2, args...);
  }
};

template <unsigned N>
struct Shift2;

template <>
struct Shift2<0u>
{
  template <class M1, class M2, typename ...Args>
  static inline
  bool apply(const M1   &m1,
             const M2   &m2,
             const Args &...args)
  {
    return ::Li::details::is_colliding(m1, m2, args...);
  }
};

template <unsigned N>
struct Shift2
{
  template <class M1, class M2, typename A1, typename ...Args>
  static inline
  bool apply(const M1   &m1,
             const M2   &m2,
             const A1   &a1,
             const Args &...args)
  {
    return Shift2<N - 1>::template apply(m1, m2, args..., a1);
  }
};

template <class M1, class M2>
struct Collision_Test_Dispatch_Helper<M1, M2, false>
{

  template <class ...Args>
  static inline
  bool apply(const M1   &m1,
             const M2   &m2,
             const Args &...args)
  {
    return Shift2<M1::take::value>::template apply(m2, m1, args...);
  }
};

} // namespace details

template <class M1, class M2, class ...Args>
static inline bool is_colliding(const M1   &m1,
                                const M2   &m2,
                                const Args &...args)
{
  return details::Collision_Test_Dispatch_Helper<M1, M2>::template apply(m1, m2, args...);
}


} // namespace Li

#endif // end of include guard: COLLISION_H_INCLUDED_PGYMRBXD

