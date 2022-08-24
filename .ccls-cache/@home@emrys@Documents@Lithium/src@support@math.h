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

#ifndef MATH_H_INCLUDED_CCSKHOYQ
#define MATH_H_INCLUDED_CCSKHOYQ

#include "../config.h"

namespace Li {

constexpr static real_t epsilon = 0.0001f;

static inline
bool almost_equal(real_t a, real_t b)
{ return std::abs(a - b) < epsilon; }

static inline
bool almost_equal(vec2_t a, vec2_t b)
{
  return almost_equal(a.x, b.x)
    &&   almost_equal(a.y, b.y);
}

static inline
bool almost_zero(real_t f)
{ return almost_equal(f, 0); }

static inline
bool almost_zero(vec2_t v)
{
  return almost_zero(v.x) && almost_zero(v.y);
}

static inline
bool almost_parallel(vec2_t a, vec2_t b)
{ return almost_equal(a.normalize(), b.normalize()); }

static inline
vec2_t intersection_point(vec2_t lpoint, vec2_t lnormal,
                          vec2_t rpoint, vec2_t rnormal)
{
  if (almost_parallel(lnormal, rnormal))
    return { };

  auto C1 = cross_product(lnormal, lpoint),
       C2 = cross_product(rnormal, rpoint),
       D  = cross_product(rnormal, lnormal),
       x  = cross_product(vec2_t { lnormal.x, rnormal.x }, vec2_t { C1, C2 }) / D,
       y  = cross_product(vec2_t { lnormal.y, rnormal.y }, vec2_t { C1, C2 }) / D;

  return { x, y };
}

static inline
bool same_sign(real_t a, real_t b)
{
  return (a > 0 && b > 0)
    ||   (a < 0 && b < 0);
}

static inline
bool diff_sign(real_t a, real_t b)
{
  return (a > 0 && b < 0)
    ||   (a < 0 && b > 0);
}

static inline
bool is_intersect(std::pair<vec2_t, vec2_t> L1,
                  std::pair<vec2_t, vec2_t> L2)
{
  auto &A1 = L1.first,
       &A2 = L1.second,
       &B1 = L2.first,
       &B2 = L2.second;

  auto  S1 = B1 - A1,
        S2 = B1 - A2,
        S3 = B2 - A1,
        S4 = B2 - A2;

  if ( cross_product(S1, S2) * cross_product(S3, S4) < 0
    && cross_product(S1, S3) * cross_product(S2, S4) < 0)
  {
    return true;
  }

  return false;
}

static inline
bool is_point_inside_box(vec2_t point,
                         vec2_t centre,
                         vec2_t border)
{
  if ( point.x > centre.x + border.x
    || point.x < centre.x - border.x
    || point.y > centre.y + border.y
    || point.y < centre.y - border.y)
  {
    return false;
  }

  return true;
}

static inline
bool is_point_inside_circle(vec2_t point,
                            vec2_t centre,
                            real_t radius)
{
  return distance(point, centre) <= radius;
}

static inline
bool is_point_inside_triangle(vec2_t P,
                              vec2_t A, vec2_t B, vec2_t C)
{
  auto PA = A - P, PB = B - P, PC = C - P;
  auto AB = B - A, AC = C - A;

  auto sum = std::abs(cross_product(PA, PB))
    +        std::abs(cross_product(PC, PB))
    +        std::abs(cross_product(PA, PC));

  auto tri = std::abs(cross_product(AB, AC));

  if (sum > tri)
    return false;

  return true;
}

static inline
real_t side(vec2_t lpoint,
            std::pair<vec2_t, vec2_t> line)
{
  auto rpoint     = line.first,
       rdirection = line.second;

  return cross_product(lpoint - rpoint, lpoint - rpoint - rdirection);
}


static inline
real_t angle(vec2_t v, real_t alt = 0.0f)
{
  if (almost_zero(v))
    return alt;

  if (almost_equal(v.x, 0.001))
  {
    if (v.y > 0)
      return  M_PI / 2;
    else
      return -M_PI / 2;
  }

  if (almost_equal(v.y, 0.001))
  {
    if (v.x > 0)
      return 0;
    else
      return M_PI;
  }

  auto ang = std::atan(v.y / v.x);

  if (v.x < 0 && v.y < 0)
    return ang + M_PI;

  if (v.x < 0 && v.y > 0)
    return ang + M_PI;

  return ang;
}

static inline
vec2_t from_rou_theta(vec2_t rou_theta)
{
  auto sin_theta = std::sin(rou_theta.x);
  auto cos_theta = std::cos(rou_theta.x);

  return { rou_theta.y * cos_theta, rou_theta.y * sin_theta };
}

static inline
vec2_t from_x_y(vec2_t x_y)
{
  auto x = x_y.x, y = x_y.y;

  if (almost_zero(y))
  {
    return x < 0 ? vec2_t { - x, -M_PI / 2 } : vec2_t {   x,  M_PI / 2 };
  }

  return { x_y.length(), std::atan(x / y) };
}

} // namespace Li

#endif // end of include guard: MATH_H_INCLUDED_CCSKHOYQ

