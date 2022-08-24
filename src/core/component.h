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

#ifndef COMPONENT_H_INCLUDED_EMSYPQFK
#define COMPONENT_H_INCLUDED_EMSYPQFK

#include "unit.h"

namespace Li {

/**
 * component interface.
 */
class Component : public Unit
{
public:
  void pre_update_component(const Update_Ctx &ui);
  void post_update_component(const Update_Ctx &ui);
private:
  /**
   * before update.
   */
  virtual void do_pre_update_component(const Update_Ctx &ui)  = 0;

  /**
   * after update.
   */
  virtual void do_post_update_component(const Update_Ctx &ui) = 0;
};

template <class Component>
class Default_Pre_Update : public Component
{
private:
  virtual void do_pre_update_component(attr_unused const Update_Ctx &ui) override final
  { /* nothing to be done. */ }
};

template <class Component>
class Default_Post_Update : public Component
{
private:
  virtual void do_post_update_component(attr_unused const Update_Ctx &ui) override final
  { /* nothing to be done. */ }
};


} // namespace Li

#endif // end of include guard: COMPONENT_H_INCLUDED_EMSYPQFK


