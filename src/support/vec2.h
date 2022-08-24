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

#ifndef VEC2_H_INCLUDED_FD1TVGXA
#define VEC2_H_INCLUDED_FD1TVGXA

#include "../prelude.h"

namespace Li {

template <typename R>
struct vec2
{
  union { R x; R i; R w; R u; };
  union { R y; R j; R h; R v; };

  template <typename U, typename W>
  vec2(U x, W y)
    : x(static_cast<R>(x))
    , y(static_cast<R>(y))
  { }

  vec2(R x = 0, R y = 0)
    : x(x), y(y)
  { }

  /**
   * 模的平方.
   */
  inline R length_sq() const
  {
    return x * x + y * y;
  }

  /**
   * 模.
   */
  inline R length() const
  {
    return std::sqrt(length_sq());
  }

  /**
   * v - *this.
   */
  inline attr_must_use_ret vec2 to(const vec2 &v) const
  {
    return { v.x - x, v.y - y };
  }

  /**
   * *this - v.
   */
  inline attr_must_use_ret vec2 from(const vec2 &v) const
  {
    return v.to(*this);
  }

  /**
   * 判断是否零向量.
   */
  inline bool is_zero() const
  {
    return x == 0 && y == 0;
  }

  /**
   * 判断是否单位向量.
   */
  inline bool is_e() const
  {
    // XXX: reminder 2016-03-18 18:56:01
    return length_sq() == 1;
  }

  /**
   * 旋转r弧度.
   */
  inline attr_must_use_ret vec2 rotate(R r) const
  {
    return { x * std::cos(r) - y * std::sin(r), x * std::sin(r) + y * std::cos(r) };
  }

  /**
   * 数乘r.
   */
  inline attr_must_use_ret vec2 scale(R r) const
  {
    return { x * r, y * r };
  }

  /**
   * 叠加.
   */
  inline attr_must_use_ret vec2 compose(const vec2 &v) const
  {
    return { x + v.x, y + v.y };
  }

  /**
   * 点积.
   */
  inline R dot(const vec2 &v) const
  {
    return x * v.x + y * v.y;
  }

  /**
   * ±叉积的模.
   */
  inline R cross(const vec2 &v) const
  {
    return x * v.y - y * v.x;
  }

  /**
   * 正规化向量. 不能在零向量上调用.
   */
  inline attr_must_use_ret vec2 normalize() const
  {
    return scale(1 / length());
  }

  // operators.

  inline attr_must_use_ret auto operator +(const vec2 &rhs) const
  {
    return compose(rhs);
  }

  inline attr_must_use_ret auto operator -(const vec2 &rhs) const
  {
    return compose(rhs.scale(-1));
  }

  inline attr_must_use_ret auto operator *(R r) const
  {
    return scale(r);
  }

  inline attr_must_use_ret auto operator /(R r) const
  {
    return scale(1/r);
  }

  inline attr_must_use_ret auto operator *(const vec2 &rhs) const
  {
    return dot(rhs);
  }

  inline attr_must_use_ret auto operator ^(const vec2 &rhs) const
  {
    return cross(rhs);
  }

  inline vec2 &operator +=(const vec2 &rhs)
  {
    return *this = *this + rhs;
  }

  inline vec2 &operator -=(const vec2 &rhs)
  {
    return *this = *this - rhs;
  }

  inline vec2 &operator *=(const vec2 &rhs)
  {
    return *this = *this * rhs;
  }

  inline vec2 &operator *=(R r)
  {
    return *this = *this * r;
  }

  inline vec2 &operator ^=(const vec2 &rhs)
  {
    return *this = *this ^ rhs;
  }
};


template <class U, class W>
static inline auto make_vec2(U x, W y)
{
  using R = decltype(x + y);
  return vec2<R>(x, y);
}

template <class U, class W>
static inline auto distance_sq(const U &l, const W &r)
{
  return (l - r).length_sq();
}

template <class U, class W>
static inline auto distance(const U &l, const W &r)
{
  return std::sqrt(distance_sq(l, r));
}

template <typename U>
static inline auto length_sq(vec2<U> v)
{ return v.length_sq(); }

template <class U, class W>
static inline auto cross_product(
  const vec2<U> &lhs,
  const vec2<W> &rhs)
{ return lhs.cross(rhs); }

template <class U, class W>
static inline auto dot_product(
  const vec2<U> &lhs,
  const vec2<W> &rhs)
{ return lhs.dot(rhs); }

} // namespace Li

#endif // end of include guard: VEC2_H_INCLUDED_FD1TVGXA

