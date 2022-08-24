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

#ifndef UNIT_H_INCLUDED_J1JYNKIM
#define UNIT_H_INCLUDED_J1JYNKIM

#include "object.h"
#include "updateobject.h"
#include "CID.h"

namespace Li {

struct Update_Ctx;
class  Component;

/**
 * Component container.
 */
class Unit : public Update_Object
{
  std::array<Ref_Ptr<Component>, CID::Max_Components> components;
public:
  Unit();
  Unit(const Unit &unit);

  ~Unit();


  /**
   * query the component of `slot_id`
   *
   *  @return the component, or nullptr if no such slot.
   */
  const Component *query_component(uid_t slot_id) const;

  /**
   * non-const version of above.
   */
        Component *query_component(uid_t slot_id);


  /**
   * replace the component of given slot_id.
   */
  Ref_Ptr<Component> replace_component(uid_t CID,
                                       Ref_Ptr<Component> &&new_comp = nullptr);

  /**
   * remove a component.
   */
  Ref_Ptr<Component> uninstall_component(uid_t CID);

private:
  virtual void do_update(const Update_Ctx &ui) override;
};

} // namespace Li

#endif // end of include guard: UNIT_H_INCLUDED_J1JYNKIM

