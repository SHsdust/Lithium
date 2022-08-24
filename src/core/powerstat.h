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

#ifndef POWERSTAT_H_INCLUDED_ZUULVQKE
#define POWERSTAT_H_INCLUDED_ZUULVQKE

#include "component.h"
#include "../books/prototype.h"

namespace Li {

class Power_Stat : public Component
{
public:
  virtual real_t query_power() = 0;
};

class Parameterized_Power_Stat : public $chain( Power_Stat
                                              , (Copy_Via_CCTOR, Parameterized_Power_Stat)
                                              , (Pretty_Runtime_Type, Parameterized_Power_Stat)
                                              , (Default_Pre_Update)
                                              , (Default_Post_Update))
{
  Ref_Wrapper<R_Source> power;
public:
  Parameterized_Power_Stat(R_Source_Ptr &&power)
    : power(std::move(power))
  { }

  Parameterized_Power_Stat(const Parameterized_Power_Stat &) = default;

  virtual real_t query_power() override final
  { return power->get_value(); }
private:
  virtual void do_update(const Update_Ctx &ui) override final
  { power->eval(ui); }
};

attr_export Unit *e_empower(Unit *entity, Ref_Ptr<R_Source> &&power);

} // namespace Li

#endif // end of include guard: POWERSTAT_H_INCLUDED_ZUULVQKE

