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

#include "../route.h"
#include "../SDLdefs.h"
#include "../inithook.h"
#include "../instance.h"
#include "../RCmanager.h"
#include "../factory.h"
#include "../stage.h"
#include "../gameupdate.h"
#include "../rendercontext.h"
#include "../support/debug.h"
#include "../support/bootstrap.h"
#include "../inputsource.h"

#include "playconfig.h"
#include "play.h"
#include "main.h"
#include "pause.h"
#include "failed.h"
#include "congrat.h"

namespace Li {

class Game_Play::Game_Play_Impl
{
public:
  Ptr<Game_Update>    game_update;
  Statistic           accum;
  Statistic           updating;
  Player_Resource     player_resource;
  Difficulty_Config   difficulty_config;
  Ptr<Input_Source>   input_source;
  Game_Instance      *game_instance;

  Str                 player_key;
  Str_List            stage_prefix;
  size_t              idx = 0;
};

Game_Play::~Game_Play() { }

Game_Play::Game_Play(const Game_Play &)
{ Li_PanicF("DO NOT COPY-CONSTRUCT A GAME-PLAY!"); }

Game_Play::Game_Play(const Play_Config &pc,
                     Ptr<Input_Source> &&input_source,
                     Game_Instance *game_instance)
{
  pimpl              = own(new Game_Play_Impl { { nullptr }                 /* game_update */
                                              , { }                         /* statistic.accum */
                                              , { }                         /* updating  */
                                              , pc.player_resource          /* initial resource */
                                              , pc.difficulty_config        /* difficulty */
                                              , std::move(input_source)     /* input-source */
                                              , game_instance               /* game-instance */
                                              , pc.player_key               /* player-key */
                                              , pc.stage_prefix             /* stage-prefix */
                                              });

  auto player        = game_instance->get_factory()->acquire_obj(pc.player_key);

  auto stage         = make_ref_ptr<Stage>(pc.stage_prefix.at(0), game_instance);

  pimpl->game_update = own(new Game_Update(game_instance,
                                           &pimpl->updating,
                                           &pimpl->player_resource,
                                           std::move(stage),
                                           std::move(player),
                                           pimpl->input_source.get()));
}

void Game_Play::update_page(Route *route, real_t dt, utick_t dticks)
{
  if (::SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_ESCAPE])
  {
    route->push_page(std::make_shared<Game_Pause>(this));
    return;
  }

  if (will_route && route_interval.next(dt))
  {
    if (pimpl->game_update->passed())
      route->route_to(std::make_shared<Congrat>(pimpl->accum, std::move(pimpl->game_update)));
    else
      route->route_to(std::make_shared<Game_Failed>(pimpl->accum, std::move(pimpl->game_update)));
  }

  if (will_route)
    return;

  if (pimpl->game_update->finished())
  {
    pimpl->accum    = pimpl->accum.join(pimpl->updating);
    pimpl->updating = { };

    if (++pimpl->idx == pimpl->stage_prefix.size())
    {
      will_route = true;
      return;
    }

    auto stage         = make_ref_ptr<Stage>(pimpl->stage_prefix.at(pimpl->idx), route->get_instance());
    auto player        = pimpl->game_instance->get_factory()->acquire_obj(pimpl->player_key);

    pimpl->game_update = own(new Game_Update(route->get_instance(),
                                             &pimpl->updating,
                                             &pimpl->player_resource,
                                             std::move(stage),
                                             std::move(player),
                                             pimpl->input_source.get()));
    // goto Stage_Passed
  }

  pimpl->game_update->update(dt, dticks);
}

void Game_Play::render_page(Route * /*route*/, const Render_Context &rc)
{
  pimpl->game_update->render(rc);
}

void Game_Play::do_update(const Update_Ctx &)
{ }

} // namespace Li

