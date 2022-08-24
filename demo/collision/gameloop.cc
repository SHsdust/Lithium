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

#include "support.h"
#include "gameloop.h"
#include "render.h"
#include "rendercontext.h"

namespace Li {

struct Game_Loop::Game_Loop_Impl
{
  std::list<Ref_Ptr<Unit>> units;
  Update_TickTack_Info     ticktack;
  Update_World_Info        world;
};

Game_Loop::Game_Loop(real_t w, real_t h)
  : pimpl(new Game_Loop_Impl {
            { },
            { NIL_TICK, 0, 0, 0 },
            { w, h, this }
          })
{ }

Game_Loop::~Game_Loop() { }

void Game_Loop::add_unit(Ref_Ptr<Unit> &&unit)
{
  pimpl->units.emplace_front(std::move(unit));
}

void Game_Loop::update(real_t dt, utick_t ticks)
{
  auto &units = pimpl->units;

  pimpl->ticktack.global_tick += ticks;
  pimpl->ticktack.delta_tick   = ticks;
  pimpl->ticktack.global_time_elpased += dt;
  pimpl->ticktack.delta_time_elpased   = dt;

  Update_Info ui = { pimpl->ticktack, { }, pimpl->world };

  for (auto punit = units.begin(); punit != units.end(); )
  {
    auto *unit = punit->get();

    auto *particle = unit->query_component(CID_Particle),
         *collider = unit->query_component(CID_Collider),
         *actor    = unit->query_component(CID_Actor),
         *tag      = unit->query_component(CID_Tag);

    ui.target.particle = particle;
    ui.target.actor    = actor;
    ui.target.collider = collider;
    ui.target.tag      = tag;

    ui.target.target   = unit;

    unit->update(ui);

    if (!tag)
    {
      ++punit;
      continue;
    }
    auto status = tag->as<components::Tag>()->model.status;

    if (status == components::Tag::Dead || status == components::Tag::Vanished)
      punit = units.erase(punit);
    else
      ++punit;
  }
}

void Game_Loop::render(::SDL_Renderer *renderer, int x, int y, int w, int h)
{
  auto &units    = pimpl->units;
  Update_Info ui = { pimpl->ticktack, { }, pimpl->world };

  for (auto punit = units.begin(); punit != units.end(); ++punit)
  {
    auto *unit = punit->get();

    auto *particle = unit->query_component(CID_Particle),
         *collider = unit->query_component(CID_Collider),
         *actor    = unit->query_component(CID_Actor),
         *tag      = unit->query_component(CID_Tag),
         *render   = unit->query_component(CID_Render);

    ui.target.particle = particle;
    ui.target.actor    = actor;
    ui.target.collider = collider;
    ui.target.tag      = tag;
    ui.target.render   = render;

    ui.target.target   = unit;

    if (render)
      render->as<components::Render>()->render({ renderer, x, y, w, h }, ui);
  }
}

} // namespace Li

