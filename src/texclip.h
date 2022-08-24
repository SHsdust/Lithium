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

#ifndef TEXCLIP_H_INCLUDED_3JRXW10C
#define TEXCLIP_H_INCLUDED_3JRXW10C

#include "config.h"
#include "SDLdefs.h"
#include "texture.h"

namespace Li {

enum Tex_Clip_ID { TCID_PRE_IMG            = 1000 + 100 +  1
                 , TCID_GUN_BULLET         = 1000 + 200 +  2
                 , TCID_GUN_BULLET_FADE    = 1000 + 300 +  3
                 , TCID_BALL_MID           = 1000 + 400 +  4
                 , TCID_BALL_SMALL         = 2000 + 100 +  5
                 , TCID_BALL_BIG           = 2000 + 200 +  6
                 , TCID_BALL_HUGE          = 2000 + 300 +  7
                 , TCID_STAR_SMALL         = 2000 + 400 +  8
                 , TCID_STAR_BIG           = 2000 + 500 +  9
                 , TCID_GRAIN              = 2000 + 600 + 10
                 , TCID_WATER_GROP         = 3000 + 700 + 11
                 , TCID_LAZER              = 4000 + 100 + 12
                 , TCID_Type_A             = 5000 + 100 + 13
                 , TCID_Type_B             = 5000 + 200 + 14
                 , TCID_Enemy_A            = 6000 + 100 + 15
                 , TCID_Enemy_B            = 6000 + 200 + 16
                 , TCID_Enemy_Boss         = 6000 + 300 + 17
                 , TCID_Total              = 17 /* this bites! */
                 };

enum Clip_Index { CI_RED
                , CI_PURPLE
                , CI_BLUE
                , CI_CYAN
                , CI_GREEN
                , CI_YELLOW
                , CI_ORANGE
                , CI_BLACK
                , CI_NO_INCLINE = 0
                , CI_INCLINE_RIGHT
                , CI_INCLINE_INCLINE_RIGHT
                , CI_INCLINE_LEFT
                , CI_INCLINE_INCLINE_LEFT
                };

class Tex_Clip
{
  Texture         texture;
  Seq<::SDL_Rect> clips;
public:
  Tex_Clip(Texture texture, Seq<::SDL_Rect> &&clips)
    : texture(std::move(texture))
    , clips(std::move(clips))
  { }

  Tex_Clip()
    : texture(nullptr, 0, 0)
    , clips()
  { }

  void render_clip(::SDL_Renderer *, ::SDL_Rect dest, Clip_Index ci) const;
  void render_clip(::SDL_Renderer *, ::SDL_Rect dest, Clip_Index ci, real_t rot) const;
};

} // namespace Li

#endif // end of include guard: TEXCLIP_H_INCLUDED_3JRXW10C

