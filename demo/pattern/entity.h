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

#ifndef ENTITY_H_INCLUDED_ALNWFRSV
#define ENTITY_H_INCLUDED_ALNWFRSV

#include "li.h"

namespace Li {

class Entity : public $chain( Unit
                            , (Copy_Via_CCTOR, Entity)
                            , (Pretty_Runtime_Type, Entity))
{
public:
  Entity(combinators::Combinator_Ptr &&task);
private:
  virtual void do_update(const Update_Info &ui) override final;
};

} // namespace Li

#endif // end of include guard: ENTITY_H_INCLUDED_ALNWFRSV

