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

#include "config.h"
#include "../SDLdefs.h"
#include "../texclip.h"
#include "../core/render.h"

namespace Li {

class SDL_Debug_Render : public $chain( Render
                                      , (Copy_Via_CCTOR, SDL_Debug_Render)
                                      , (Pretty_Runtime_Type, SDL_Debug_Render)
                                      , (Default_Update)
                                      , (Default_Pre_Update)
                                      , (Default_Post_Update))
{
public:
  virtual void render(const Render_Context &rc, const Update_Ctx &uc) override final;
};


class SDL_Texture_Render : public $chain( Render
                                        , (Copy_Via_CCTOR, SDL_Texture_Render)
                                        , (Pretty_Runtime_Type, SDL_Texture_Render)
                                        , (Default_Update)
                                        , (Default_Pre_Update)
                                        , (Default_Post_Update))
{
  Tex_Clip              *tex_clip;
  Clip_Index             clip_index;
  Ref_Wrapper<V_Source>  size;
public:
  SDL_Texture_Render(Tex_Clip *tex_clip,
                     Clip_Index clip_index,
                     V_Source_Ptr &&size)
    : tex_clip(tex_clip)
    , clip_index(clip_index)
    , size(std::move(size))
  { }

  SDL_Texture_Render(Game_Instance *,
                     Tex_Clip_ID tcid,
                     Clip_Index,
                     V_Source_Ptr &&);

public:
  virtual void render(const Render_Context &rc, const Update_Ctx &uc) override final;
};

static inline Unit *e_debug_beautify(Unit *unit)
{
  unit->replace_component(CID_Render,
                          own<Component>(new SDL_Debug_Render));

  return unit;
}

static inline Unit *e_beautify(Unit            *unit,
                               Tex_Clip        *tex_clip,
                               Clip_Index       clip_index,
                               V_Source_Ptr   &&size)
{
  unit->replace_component(CID_Render,
                          own<Component>(new SDL_Texture_Render(tex_clip,
                                                                clip_index,
                                                                std::move(size))));

  return unit;
}

static inline Unit *e_beautify(Unit            *unit,
                               Game_Instance   *game_instance,
                               Tex_Clip_ID      tcid,
                               Clip_Index       clip_index,
                               V_Source_Ptr   &&size)
{
  unit->replace_component(CID_Render,
                          own<Component>(new SDL_Texture_Render(game_instance,
                                                                tcid,
                                                                clip_index,
                                                                std::move(size))));

  return unit;
}

} // namespace Li

#endif // end of include guard: RENDER_H_INCLUDED_PIRGYLFN

