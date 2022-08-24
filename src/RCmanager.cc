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


#include "RCmanager.h"
#include "support/misc.h"
#include "support/debug.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace Li {

RC_Mgr::RC_Mgr()
{
  textures.emplace_back(nullptr, 0, 0);
}

RC_Mgr::~RC_Mgr()
{ }

uid_t RC_Mgr::register_texture(::SDL_Texture *texture,
                               const Str &name)
{
  Li_Assert(texture);
  Li_Assert(!u_has(textureid_by_name, name));

  uid_t next_id = static_cast<uid_t>(textures.size());

  textureid_by_name[name] = next_id;

  int w, h;
  ::SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

  textures.emplace_back(texture,  w, h);

  // Li_LogI("added texture %dx%d. [%s] / #%u / %p", w, h, name, next_id, texture);

  return next_id;
}

uid_t RC_Mgr::query_texture_id_by_name(const Str &name) const
{
  if (auto id = u_find(textureid_by_name, name))
    return *id;
  else
    return 0;
}

uid_t RC_Mgr::register_texture(const Str &path, const Str &name)
{
  if (auto *texture = load_texture(path))
    return register_texture(texture, name);

  Li_LogE("failed to load texture [%s] as [%s]", path, name);

  return 0;
}

const Texture RC_Mgr::query_texture_by_name(const Str &name) const
{
  if (auto id = u_find(textureid_by_name, name))
    return query_texture_by_id(*id);

  Li_LogE("[RC_Mgr] texture [%s] not found!");

  return query_texture_by_id(0);
}

const Texture RC_Mgr::query_texture_by_id(uid_t id) const
{
  Li_Assert(id < textures.size());

  return textures.at(id);
}

::SDL_Texture *RC_Mgr::load_texture(const Str &path)
{
  return ::IMG_LoadTexture(get_renderer(), path.c_str());
}

::SDL_Texture *RC_Mgr::load_texture(const Str &path, Uint8 r, Uint8 g, Uint8 b)
{
  auto surface = ::IMG_Load(path.c_str());
  auto key     = ::SDL_MapRGB(surface->format, r, g, b);

  ::SDL_SetColorKey(surface, SDL_TRUE, key);

  auto *texture = ::SDL_CreateTextureFromSurface(get_renderer(), surface);

  free_SDL_something(surface);

  return texture;
}

uid_t RC_Mgr::load_ascii_text(const Str  &text,
                              const Str  &font_path,
                              int         size,
                              uint8_t     r,
                              uint8_t     g,
                              uint8_t     b,
                              uint8_t     a)
{
  auto name = format("ascii-text-%s-%s-%d-%d-%d-%d", text, font_path, size, r, g, b, a);

  if (u_has(textureid_by_name, name))
    return textureid_by_name[name];

  ::TTF_Font    *font    = Assert_SDL_Acquire(::TTF_OpenFont(font_path.c_str(), size));
  ::SDL_Surface *surface = Assert_SDL_Acquire(::TTF_RenderText_Blended(font, text.c_str(), { r, g, b, a }));
  ::SDL_Texture *texture = Assert_SDL_Acquire(::SDL_CreateTextureFromSurface(get_renderer(), surface));

  free_SDL_something(surface);
  ::TTF_CloseFont(font);

  return register_texture(texture, name);
}

const Texture RC_Mgr::query_text_texture(const Str &text,
                                         const Str &font_path,
                                         int        size,
                                         uint8_t    r,
                                         uint8_t    g,
                                         uint8_t    b,
                                         uint8_t    a)
{
  return query_texture_by_id(load_ascii_text(text,
                                             font_path,
                                             size,
                                             r,
                                             g,
                                             b,
                                             a));
}

void RC_Mgr::register_tex_clips(uid_t tid, Li::Tex_Clip_ID tcid, Seq<::SDL_Rect> &&clips)
{
  auto idx = tcid % 100;
  auto tex = query_texture_by_id(tid);

  tex_clips[idx] = Tex_Clip(tex, std::move(clips));
}


Tex_Clip *RC_Mgr::query_tex_clip(Tex_Clip_ID tcid)
{
  return &tex_clips[tcid % 100];
}

} // namespace Li

