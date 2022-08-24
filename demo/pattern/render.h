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

#ifndef RENDER_H_INCLUDED_PIRGYLFN
#define RENDER_H_INCLUDED_PIRGYLFN

#include "li.h"
#include "SDLdefs.h"

namespace Li {
namespace components {

class SDL_Render : public $chain( Render
                                , (Copy_Via_CCTOR, SDL_Render)
                                , (Pretty_Runtime_Type, SDL_Render)
                                , (Default_Update)
                                , (Default_Pre_Update)
                                , (Default_Post_Update))
{
public:
  virtual void render(const Render_Context &rc, const Update_Info &ui) override final;
};

} // namespace components
} // namespace Li

#endif // end of include guard: RENDER_H_INCLUDED_PIRGYLFN

