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

#ifndef TEXTURE_H_INCLUDED_DOBXJW19
#define TEXTURE_H_INCLUDED_DOBXJW19

#include "config.h"
#include "SDLdefs.h"

namespace Li {

struct Texture
{
  struct SDL_Deleter
  {
    inline void operator ()(::SDL_Texture *rc)
    { free_SDL_something(rc); }
  };

  Arc<::SDL_Texture > texture;
  int                 w, h;

  Texture(::SDL_Texture *texture, int w, int h)
    : texture(texture, SDL_Deleter())
    , w(w)
    , h(h)
  { }
};

} // namespace Li

#endif // end of include guard: TEXTURE_H_INCLUDED_DOBXJW19

