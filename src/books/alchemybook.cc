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

#ifndef ALCHEMYBOOK_CC_INCLUDED_BM1YLWFA
#define ALCHEMYBOOK_CC_INCLUDED_BM1YLWFA

#include "alchemybook.h"

namespace Li {

class V_Constant_Rotated
  : public $chain( V_Source
                 , (Copy_Via_CCTOR, V_Constant_Rotated)
                 , (Pretty_Runtime_Type, V_Constant_Rotated))
{
  R_Source_Ptr  i;
  real_t        cos_theta;
  real_t        sin_theta;
public:
  V_Constant_Rotated(R_Source_Ptr &&i,
                     real_t         cos_theta,
                     real_t         sin_theta)
    : i(std::move(i))
    , cos_theta(cos_theta)
    , sin_theta(sin_theta)
  { }

  V_Constant_Rotated(const V_Constant_Rotated &vcr)
    : i(vcr.i->clone_as<R_Source>())
    , cos_theta(vcr.cos_theta)
    , sin_theta(vcr.sin_theta)
  { }

private:
  virtual vec2_t do_eval(const Update_Ctx &uc) override final
  {
    auto x = i->eval(uc);
    return { x * cos_theta, x * sin_theta };
  }
};

class V_Rotate
  : public $chain( V_Source
                 , (Copy_Via_CCTOR, V_Rotate)
                 , (Pretty_Runtime_Type, V_Rotate))
{
  R_Source_Ptr i;
  R_Source_Ptr theta;
public:
  V_Rotate(R_Source_Ptr &&i,
           R_Source_Ptr &&theta)
    : i(std::move(i))
    , theta(std::move(theta))
  { }

  V_Rotate(const V_Rotate &vcr)
    : i(vcr.i->clone_as<R_Source>())
    , theta(vcr.theta->clone_as<R_Source>())
  { }

private:
  virtual vec2_t do_eval(const Update_Ctx &uc) override final
  {
    auto x = i->eval(uc);
    auto t = theta->eval(uc);

    auto cos_theta = std::cos(t);
    auto sin_theta = std::sin(t);

    return { x * cos_theta, x * sin_theta };
  }
};

class V_Rotate_V
  : public $chain( V_Source
                 , (Copy_Via_CCTOR, V_Rotate_V)
                 , (Pretty_Runtime_Type, V_Rotate_V)
                 )
{
public:
  V_Source_Ptr v;
  R_Source_Ptr theta;

  V_Rotate_V(V_Source_Ptr &&v,
             R_Source_Ptr &&theta)
    : v(std::move(v))
    , theta(std::move(theta))
  { }

  V_Rotate_V(const V_Rotate_V &vrv)
    : v(vrv.v->clone_as<V_Source>())
    , theta(vrv.theta->clone_as<R_Source>())
  { }
private:
  virtual vec2_t do_eval(const Update_Ctx &uc) override final
  {
    auto t = theta->eval(uc);
    auto o = v->eval(uc);
    auto x = o.x, y = o.y;

    auto sin_theta = std::sin(t);
    auto cos_theta = std::cos(t);

    return { x * cos_theta - y * sin_theta
      ,      x * sin_theta + y * cos_theta };
  }
};
V_Source_Ptr f_rotate(R_Source_Ptr &&i,
                      real_t       theta)
{
  auto sin_theta = std::sin(theta);
  auto cos_theta = std::cos(theta);

  return own((V_Source *)
             new V_Constant_Rotated(std::move(i),
                                    sin_theta,
                                    cos_theta));
}

V_Source_Ptr f_rotate(R_Source_Ptr &&i,
                      R_Source_Ptr &&theta)
{
  return own((V_Source *)
             new V_Rotate(std::move(i),
                          std::move(theta)));
}

V_Source_Ptr f_rotate(V_Source_Ptr &&v,
                      R_Source_Ptr &&theta)
{
  return own((V_Source *)
             new V_Rotate_V(std::move(v),
                            std::move(theta)));
}



} // namespace Li

#endif // end of include guard: ALCHEMYBOOK_CC_INCLUDED_BM1YLWFA

