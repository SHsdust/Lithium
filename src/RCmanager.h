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

#ifndef RCMANAGER_H_INCLUDED_JOA4Q0X9
#define RCMANAGER_H_INCLUDED_JOA4Q0X9

#include "config.h"
#include "texture.h"
#include "texclip.h"

namespace Li {

struct Texture;

class RC_Mgr
{
  Seq<Texture>                            textures;
  Map<Str, uid_t>                         textureid_by_name;
  std::array<Tex_Clip, TCID_Total + 1>    tex_clips;
public:
   RC_Mgr();
  ~RC_Mgr();

  uid_t register_texture(::SDL_Texture  *texture,  const Str &name);
  uid_t register_texture(const Str &path, const Str &name);

  uid_t register_texture(const Str &path, const Str &name, Uint8 r, Uint8 g, Uint8 b)
  {
    return register_texture(load_texture(path, r, g, b), name);
  }

  uid_t register_by_path(const Str &path);

  const Texture query_texture_by_name(const Str &name) const;
  const Texture query_texture_by_id(uid_t id)          const;

  uid_t         query_texture_id_by_name(const Str &name) const;

  static ::SDL_Texture *load_texture(const Str &path);
  static ::SDL_Texture *load_texture(const Str &path, Uint8 r, Uint8 g, Uint8 b);

  // name : ascii-text-$FONT-$SIZE-$R-$G-$B-$A
  uid_t load_ascii_text(const Str  &text,
                        const Str  &font_path,
                        int         size,
                        uint8_t     r,
                        uint8_t     g,
                        uint8_t     b,
                        uint8_t     a);

  const Texture query_text_texture(const Str &text,
                                   const Str &font_path,
                                   int        size,
                                   uint8_t    r = 255,
                                   uint8_t    g = 255,
                                   uint8_t    b = 255,
                                   uint8_t    a = 255);

  void register_tex_clips(uid_t tid,
                          Tex_Clip_ID tcid,
                          Seq<::SDL_Rect> &&clips);

  Tex_Clip *query_tex_clip(Tex_Clip_ID tcid);
};

attr_unused constexpr static const auto RC_Prefix = "res/";

} // namespace Li

#endif // end of include guard: RCMANAGER_H_INCLUDED_JOA4Q0X9

