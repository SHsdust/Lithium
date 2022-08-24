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

#ifndef ACTOR_H_INCLUDED_6IPMNIR5
#define ACTOR_H_INCLUDED_6IPMNIR5

#include "config.h"

namespace Li {

class Task;

class Actor : public $chain( Component
                           , (Copy_Via_CCTOR, Actor)
                           , (Pretty_Runtime_Type, Actor)
                           , (Default_Pre_Update)
                           , (Default_Post_Update))
{
public:
  Ref_Wrapper<Task> task;
public:
  Actor();
  Actor(Ref_Ptr<Task> &&task);
private:
  virtual void do_update(const Update_Ctx &ui) override;
};

} // namespace Li

#endif // end of include guard: ACTOR_H_INCLUDED_6IPMNIR5

