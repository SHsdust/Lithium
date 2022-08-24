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

#include "SDLinput.h"
#include "SDLdefs.h"
#include "support.h"
#include <cstdlib>

namespace Li {

bool SDL_Input_Source::query_key_state(Key_Code kc) const
{
  auto scancode =
    kc == KC_Back    ? SDL_SCANCODE_ESCAPE
  : kc == KC_Up      ? SDL_SCANCODE_UP
  : kc == KC_Down    ? SDL_SCANCODE_DOWN
  : kc == KC_Left    ? SDL_SCANCODE_LEFT
  : kc == KC_Right   ? SDL_SCANCODE_RIGHT
  : kc == KC_Yes     ? SDL_SCANCODE_Z
  : kc == KC_No      ? SDL_SCANCODE_X
  : kc == KC_Special ? SDL_SCANCODE_C
  : kc == KC_Mode    ? SDL_SCANCODE_LSHIFT
  :                    -1;

  Li_Assert(scancode != -1);

  return ::SDL_GetKeyboardState(nullptr)[scancode] != 0;
}

int SDL_Input_Source::acquire_random(int from_inclusive,
                                     int to_exclusive)
{
  return std::rand() % (to_exclusive - from_inclusive) + from_inclusive;
}


void SDL_Input_Source::next()
{ }

} // namespace Li

