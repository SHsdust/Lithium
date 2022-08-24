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

#ifndef SDLDEFS_H_INCLUDED_GBHOEKT0
#define SDLDEFS_H_INCLUDED_GBHOEKT0

#include <SDL2/SDL.h>

namespace Li {

void free_SDL_something(::SDL_Window   *);
void free_SDL_something(::SDL_Renderer *);
void free_SDL_something(::SDL_Surface  *);
void free_SDL_something(::SDL_Texture  *);

#define Display_SDL_Error(sdlcall)    \
  do {                                \
    Li_LogE("%s raised an error: %s", \
            sdlcall,                  \
            ::SDL_GetError());        \
  } while (false)

#define Assert_SDL_Call(...)            \
  do {                                  \
    if (__VA_ARGS__) { }                \
    else {                              \
      Display_SDL_Error(#__VA_ARGS__);  \
      Li_PanicF(#__VA_ARGS__ "Failed"); \
    }                                   \
  } while (false)

#define Assert_SDL_Acquire(...)                   \
  ({                                              \
   auto *something = (__VA_ARGS__);               \
   if (!something)                                \
   {                                              \
     Display_SDL_Error(#__VA_ARGS__);             \
     Li_PanicF(#__VA_ARGS__ "returned nullptr!"); \
   }                                              \
   /* return */ something;                        \
  })

} // namespace Li



#endif // end of include guard: SDLDEFS_H_INCLUDED_GBHOEKT0

