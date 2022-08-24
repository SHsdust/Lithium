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

#include "stage.h"
#include "chapter.h"
#include "gameupdate.h"
#include "instance.h"
#include "factory.h"
#include "core/updatectx.h"
#include "books/prototype.h"
#include "support/format.h"
#include "support/debug.h"
#include "support/string.h"
#include "rendercontext.h"
#include "RCmanager.h"
#include "SDLdefs.h"
#include "inithook.h"

namespace Li {

#define Load_Query_Max   1000

Stage::~Stage() { }

Stage::Stage(const Str &prefix, Game_Instance *game_instance)
  : prefix(prefix)
  , idx(0)
  , game_instance(game_instance)
  , state('I')
  , sleep(120)
{
  auto factory = game_instance->get_factory();

  Str_List loaded;
  for (size_t i = 0; i != Load_Query_Max + 1; ++i)
  {
    auto chapter_key = format("chapters.%s-%04zu", prefix, i);

    if (auto chapter = factory->acquire_obj(chapter_key, true))
    {
      chapters.emplace_back(own<Chapter>(chapter->clone()));
      loaded.push_back(chapter_key);
    }
  }

  Li_TraceF("loaded chapters :\n%s",
            str_map_join(loaded,
                         "\n",
                         [](const Str &chapter)
                         {
                           return "[] " + chapter;
                         }));

  bg_first_id = game_instance->get_rc_mgr()->query_texture_id_by_name("bg-1");
}

// F : finished
// S : sleeping
// I : initalizing
// U : updating
void Stage::do_update(const Update_Ctx &uc)
{
  if (state == 'S')
  {
    if (sleep.next(uc.ticktack.delta_tick))
      state = 'I';
  }
  else if (state == 'U')
  {
    auto chapter = chapters.at(idx).ptr.get();

    if (!chapter->exec(uc))
    {
      chapter->on_leave(uc.world.game_world, uc.world.game_instance);

      if (++idx == chapters.size())
        state = 'F';
      else
        state = 'S';
    }
  }
  else if (state == 'I')
  {
    Li_TraceF("entered chapter #%zu", idx);
    auto chapter = chapters.at(idx).ptr.get();
    chapter->on_enter(uc.world.game_world, uc.world.game_instance);
    chapter->exec(uc);
    state = 'U';
  }

  if (get_ticks() % 2 == 0)
    --bg_yoff;

  if (bg_yoff == -800 * 11)
    bg_yoff = 0;
}

void Stage::render_stage(const Update_Ctx &uc, const Render_Context &rc)
{
  auto *renderer = get_renderer();
  auto part      =::SDL_Rect { rc.view_x, rc.view_y - bg_yoff, rc.view_w, rc.view_h };

  for (int i = 0; i != bg_total; ++i)
  {
    auto  tid = bg_first_id + i;
    auto &tex = uc.world.game_instance->get_rc_mgr()->query_texture_by_id(tid);

    ::SDL_RenderCopy(renderer, tex.texture.get(), nullptr, &part);

    part.y -= rc.view_h;
  }
}

static inline
void load_bg(RC_Mgr *mgr)
{
  for (int i = 1; i != 13; ++i)
  {
    mgr->register_texture(format("res/st2_%d.bmp", i),
                          format("bg-%d", i));
  }
}

Li_Game_Init("reg.load.bg", "SDL-init", inst)
{
  auto handle = inst->get_init_handle();

  bootstrap::init_append_early_init(handle, "load.bg",
                                    [inst](bootstrap::Bootstrap_Args &,
                                           const bootstrap::Bootstrap_Reg &)
                                    {
                                      load_bg(inst->get_rc_mgr());
                                    });
}

} // namespace Li

