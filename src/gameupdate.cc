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

#include "gameupdate.h"
#include "support.h"
#include "inputsource.h"
#include "core/tag.h"
#include "core/trigger.h"
#include "core/collider.h"
#include "core/render.h"
#include "core/damagestat.h"
#include "core/powerstat.h"
#include "core/particle.h"
#include "statistic.h"
#include "stage.h"
#include "rendercontext.h"
#include "SDLdefs.h"
#include "instance.h"
#include "RCmanager.h"
#include "inithook.h"

namespace Li {

using Unit_List = List<Ptr<Unit>>;
using Unit_Pos  = Unit_List::iterator;

class Game_Update::Game_Update_Impl
{
public:
  Ref_Ptr<Stage>       stage;
  Ref_Ptr<Unit>        player;

  Unit_List updat_grps    [UG_Total];

  Game_Instance    *game_instance;

  TickTack_Ctx  ticktack = { };
  World_Ctx     world    = { };

  Input_Source *input_source;

  Statistic        *statistic;
  Player_Resource  *player_resource;

  Interval_Timer<real_t> respawn_timer = { 1.0f };

  bool              updating_finished = false;

  Ref_Ptr<Unit>        prototype;

  Statistic        cp_statistic;
  Player_Resource  cp_player_resource;

  Game_Update_Impl(Statistic          *statistic,
                   Player_Resource    *player_resource,
                   Ref_Ptr<Stage>    &&stage,
                   Ref_Ptr<Unit>     &&player,
                   Game_Instance      *game_instance,
                   Input_Source       *input_source)
    : stage(std::move(stage))
    , player(std::move(player))
    , game_instance(game_instance)
    , input_source(input_source)
    , statistic(statistic)
    , player_resource(player_resource)
  {
    // TODO: remove world.width/world.height 2016-07-18 13:06:00
    world.width  = 1000.0f;
    world.height = 1000.0f;

    prototype = own<Unit>(this->player->clone());
  }

  bool update_one(Unit *);

  void update(real_t dt, utick_t dticks);
  void render(const Render_Context &rc);

  static inline auto collect_target_info(Unit *target)
  {
    return Target_Ctx { target
                      , target->query_component(CID_Particle)
                      , target->query_component(CID_Direction)
                      , target->query_component(CID_Actor)
                      , target->query_component(CID_Collider)
                      , target->query_component(CID_Trigger)
                      , target->query_component(CID_Tag)
                      , target->query_component(CID_Render)
                      };
  }

  inline Update_Ctx uc(Unit *u)
  { return { ticktack, world, collect_target_info(u) }; }
};

bool Game_Update::Game_Update_Impl::update_one(Unit *u)
{
  auto target = collect_target_info(u);
  u->update(uc(u));

  auto status = target.tag->as<Tag>()->model.status;

  return status == Tag::Dead
    ||   status == Tag::Vanished;
}

#define qcomp(T) query_component(Li_Join(CID_, T))->as<T>()


// {{{ the horrible update method.
void Game_Update::Game_Update_Impl::update(real_t     dt,
                                           utick_t    dticks)
{
  if (stage->finished())
    return;

  ticktack.global_time_elpased += dt;
  ticktack.delta_time_elpased   = dt;
  ticktack.global_tick         += dticks;
  ticktack.delta_tick           = dticks;

  stage->update(uc(stage.get()));

  auto *player          = this->player.get();
  auto *player_tag      = player->qcomp(Tag);

  if (player_tag->model.status == Tag::Dead && player_resource->life == 0)
  {
    Li_LogI("满身疮痍.");

    updating_finished = true;
    return;
  }

  if (player_tag->model.status == Tag::Dead)
  {
    if (respawn_timer.next(dt))
    {
      player_resource->life--;

      this->player   = own<Unit>(prototype->clone());
      player         = this->player.get();
      player_tag     = player->qcomp(Tag);
    }
  }
  else
  {
    player->update(uc(player));
  }

  auto *player_collider = player->qcomp(Collider);

  auto &phoney = updat_grps[UG_Phoney];

  // {{{ Phoney, Just update.
  for (auto pu = phoney.begin(); pu != phoney.end(); )
  {
    if (update_one(pu->get()))
      pu = phoney.erase(pu);
    else
      ++pu;
  }
  // }}}

  auto &items = updat_grps[UG_Item];

  // {{{ Player-Ship x Item
  for (auto pu = items.begin(); pu != items.end(); )
  {
    if (update_one(pu->get()))
    {
      pu = items.erase(pu);
      continue;
    }

    auto *u = pu->get();
    if (u->qcomp(Tag)->model.status == Tag::Alive && u->qcomp(Collider)
      && player_collider->colli(player, u))
    {
      u->qcomp(Trigger)->on_hit(uc(u), player);
    }

    ++pu;
  }
  // }}}

  auto &psh = updat_grps[UG_Player_Shoot];

  // {{{ Player-Shoot, Just update, will do colli later.
  for (auto pu = psh.begin(); pu != psh.end(); )
  {
    if (update_one(pu->get()))
      pu = psh.erase(pu);
    else
      ++pu;
  }
  // }}}

  auto &esl = updat_grps[UG_Enemy_Special];

  // {{{ Enemy-Special x Player-Ship
  for (auto pu = esl.begin(); pu != esl.end(); )
  {
    if (update_one(pu->get()))
      pu = esl.erase(pu);
    else
    {
      auto *u = pu->get();

      if (player->qcomp(Tag)->model.status == Tag::Alive
        && u->query_component(CID_Collider)
        && player_collider->colli(player, u))
      {
        player->qcomp(Trigger)->on_hit_by(uc(player), u);
        u->qcomp(Trigger)->on_hit(uc(u), player);
      }

      ++pu;
    }
  }
  // }}}

  auto &esp = updat_grps[UG_Enemy_Ship];
  // {{{ Player-Bullet x Enemy-Ship
  for (auto pu = esp.begin(); pu != esp.end(); )
  {
    if (update_one(pu->get()))
    {
      pu = esp.erase(pu);
      continue;
    }

    auto *u = pu->get();
    if (u->qcomp(Tag)->model.status == Tag::Alive && u->qcomp(Collider))
    {
      // {{{ Enemy-Ship x Player-Ship
      if (player->qcomp(Tag)->model.status == Tag::Alive
        && player_collider->colli(player, u))
      {
        player->qcomp(Trigger)->on_hit_by(uc(player), u);
      }
      // }}}

      for (auto &ps : psh)
      {
        auto *p = ps.get();

        if (auto *p_collider = p->qcomp(Collider))
        {
          if (p->qcomp(Tag)->model.status == Tag::Alive && p_collider->colli(p, u))
          {
            p->qcomp(Trigger)->on_hit(uc(p), u);
            u->qcomp(Trigger)->on_hit_by(uc(u), p);

            // enemy-ship received the damage of `power'
            if (auto pstat = p->qcomp(Power_Stat))
              if (auto dstat = u->qcomp(Damage_Stat))
                dstat->do_damage(uc(u), u, pstat->query_power());

            auto *tag = u->qcomp(Tag);

            if (tag->model.status != Tag::Alive)
            {
              statistic->destroyed_enemy_ships++;
            }
          }
        }
      }
    }

    ++pu;
  }
  // }}}

  auto &est = updat_grps[UG_Enemy_Shoot];

  // {{{ Enemy-Bullet x Player
  for (auto pb = est.begin(); pb != est.end(); )
  {
    if (update_one(pb->get()))
    {
      pb = est.erase(pb);
      continue;
    }

    auto *b = pb->get();
    if (b->qcomp(Tag)->model.status == Tag::Alive && b->qcomp(Collider))
    {
      if (player->qcomp(Tag)->model.status == Tag::Alive
        && player_collider->colli(player, b))
      {
        b->qcomp(Trigger)->on_hit(uc(b), player);
        player->qcomp(Trigger)->on_hit_by(uc(player), b);
      }
    }

    ++pb;
  }
  // }}}

  auto &psp = updat_grps[UG_Player_Special];
  // {{{ Player-Special x Enemy-Shoot, Enemy-Ship
  for (auto ps = psp.begin(); ps != psp.end(); )
  {
    if (update_one(ps->get()))
    {
      ps = psp.erase(ps);
      continue;
    }

    auto *s = ps->get();
    if (auto *s_collider = s->qcomp(Collider))
    {
      for (auto &pe : esp)
      {
        auto *e = pe.get();

        if (e->qcomp(Tag)->model.status == Tag::Alive
          && e->query_component(CID_Collider)
          && s_collider->colli(s, e))
        {
          auto power = s->qcomp(Power_Stat)->query_power();
          auto dstat = e->qcomp(Damage_Stat);

          s->qcomp(Trigger)->on_hit(uc(s), e);
          e->qcomp(Trigger)->on_hit_by(uc(e), s);

          dstat->do_damage(uc(e), e, power);

          auto *tag = e->qcomp(Tag);

          if (tag->model.status != Tag::Alive)
          {
            statistic->destroyed_enemy_ships++;
          }
        }
      }

      for (auto &pe : est)
      {
        auto *e = pe.get();

        if (e->qcomp(Tag)->model.status == Tag::Alive
          && e->query_component(CID_Collider)
          && s_collider->colli(s, e))
        {
          s->qcomp(Trigger)->on_hit(uc(s), e);
          e->qcomp(Trigger)->on_hit_by(uc(e), s);
        }
      }
    }

    ++ps;
  }
  // }}}

  cp_statistic       = *statistic;
  cp_player_resource = *player_resource;
}
// }}}

void Game_Update::Game_Update_Impl::render(const Render_Context &rc)
{
  stage->render_stage({ ticktack, world, { } }, rc);
  auto *player = this->player.get();

  if (auto *render = player->query_component(CID_Render))
    render->as<Render>()->render(rc, uc(player));

  for (size_t ug = 0; ug != UG_Total; ++ug)
  {
    for (auto &&pu : updat_grps[ug])
    {
      auto *u = pu.get();

      if (auto *render = u->query_component(CID_Render))
        render->as<Render>()->render(rc, uc(u));
    }
  }

  auto left  = ::SDL_Rect { 0, 0, rc.view_x, rc.full_h };
  auto right = ::SDL_Rect { rc.view_x + rc.view_w, 0, rc.full_w, rc.full_h };

  auto *renderer = get_renderer();

  ::SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
  ::SDL_RenderFillRect(renderer, &left);
  ::SDL_RenderFillRect(renderer, &right);

  auto mgr = game_instance->get_rc_mgr();

  auto pt_msg = format("gp   : %d", cp_statistic.destroyed_enemy_ships);
  auto ht_msg = format("life : %d", cp_player_resource.life);

  auto pt_msg_tex = mgr->query_text_texture(pt_msg, "res/opensans.ttf", 24);
  auto ht_msg_tex = mgr->query_text_texture(ht_msg, "res/opensans.ttf", 24);

  auto pt_pos  = rc.transform({ 800, 400 });
  auto pt_rect = ::SDL_Rect { pt_pos.x, pt_pos.y, pt_msg_tex.w, pt_msg_tex.h };

  ::SDL_RenderCopy(renderer, pt_msg_tex.texture.get(), nullptr, &pt_rect);

  auto ht_pos  = rc.transform({ 800, 300 });
  auto ht_rect = ::SDL_Rect { ht_pos.x, ht_pos.y, ht_msg_tex.w, ht_msg_tex.h };

  ::SDL_RenderCopy(renderer, ht_msg_tex.texture.get(), nullptr, &ht_rect);
}

void Game_Update::update(real_t dt, utick_t dticks)
{
  pimpl->update(dt, dticks);
}

bool Game_Update::finished() const
{
  if (pimpl->updating_finished)
    return true;

  if (pimpl->stage->finished())
    return true;

  return false;
}

void Game_Update::render(const Render_Context &rc)
{ pimpl->render(rc); }

void Game_Update::add_unit(Ref_Ptr<Unit> &&unit, Updating_Group ug)
{
  auto &grp = pimpl->updat_grps[ug];

  grp.emplace_front(std::move(unit));
}

Game_Update::Game_Update(Game_Instance   *game_instance,
                         Statistic       *statistic,
                         Player_Resource *player_resource,
                         Ref_Ptr<Stage> &&stage,
                         Ref_Ptr<Unit>  &&player,
                         Input_Source    *input_source)
  : pimpl(new Game_Update::Game_Update_Impl
          ( statistic
          , player_resource
          , std::move(stage)
          , std::move(player)
          , game_instance
          , input_source))
{
  pimpl->world.game_world    = this;
  pimpl->world.game_instance = game_instance;
}

Game_Update::~Game_Update() { }

Unit *Game_Update::get_player()
{ return pimpl->player.get(); }

void Game_Update::clear_units()
{
  for (int i = 0; i != UG_Total; ++i)
    clear_group(static_cast<Updating_Group>(i));
}

void Game_Update::clear_group(Updating_Group ug)
{
  auto &grp = pimpl->updat_grps[ug];
  grp.clear();
}

Statistic       *Game_Update::get_statistic()
{ return pimpl->statistic; }

Player_Resource *Game_Update::get_player_resource()
{ return pimpl->player_resource; }

Input_Source *Game_Update::get_input_source()
{ return pimpl->input_source; }

bool Game_Update::passed() const
{
  auto   status = pimpl->player->query_component(CID_Tag)->as<Tag>()->model.status;

  return pimpl->player_resource->life != 0 || status != Tag::Dead;
}

const List<Ref_Ptr<Unit>> &Game_Update::get_enemy_list() const
{
  return pimpl->updat_grps[UG_Enemy_Ship];
}

Li_Game_Init("reg.load.cache-msg", "SDL-init", inst)
{
  auto handle = inst->get_init_handle();

  bootstrap::init_append_early_init(handle, "load.cache-msg",
                                    [inst](bootstrap::Bootstrap_Args &,
                                           const bootstrap::Bootstrap_Reg &)
                                    {
                                      auto mgr = inst->get_rc_mgr();

                                      for (int i = 0; i != 1300; ++i)
                                      {
                                        auto pt_msg = format("gp   : %d", i);
                                        auto ht_msg = format("life : %d", i);

                                        mgr->query_text_texture(pt_msg, "res/opensans.ttf", 24);
                                        mgr->query_text_texture(ht_msg, "res/opensans.ttf", 24);
                                      }
                                    });
}

} // namespace Li

