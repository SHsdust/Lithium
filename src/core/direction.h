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

#ifndef DIRECTION_H_INCLUDED_HIFXETO5
#define DIRECTION_H_INCLUDED_HIFXETO5

#include "config.h"
#include "../books/prototype.h"

namespace Li {

class Direction : public $chain( Component
                               , (Default_Pre_Update)
                               , (Default_Post_Update)
                               , (Default_Update)
                               , (Copy_Via_CCTOR, Direction)
                               , (Pretty_Runtime_Type, Direction))
{
  Ref_Wrapper<V_Source>   dir;
public:
  Direction(V_Source_Ptr &&dir)
    : dir(std::move(dir))
  { }

  Direction();

  virtual vec2_t get_direction(const Update_Ctx &uc);
};

attr_export V_Source_Ptr d_velocity_direction();

attr_export Unit *e_direct(Unit *, V_Source_Ptr &&head);

} // namespace Li

#endif // end of include guard: DIRECTION_H_INCLUDED_HIFXETO5

