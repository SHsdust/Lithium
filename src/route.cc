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

#include "route.h"
#include "page.h"
#include "books/historybook.h"
#include "support/debug.h"
#include "core/updatectx.h"

namespace Li {

class Route::Route_Impl
{
public:
  Seq<Arc<Page>>  page_stack;
  Game_Instance  *game_instance;
  TickTack_Ctx    ticktack;
  World_Ctx       world;

  bool            terminated = false;

  Route_Impl(Game_Instance *game_instance)
    : game_instance(game_instance)
  { }
};

Route::Route(Game_Instance *game_instance)
  : pimpl(new Route_Impl(game_instance))
{ }

Route::~Route() { }

void Route::route_to(Arc<Page> to_page)
{
  auto &page_stack = pimpl->page_stack;

  if (page_stack.empty())
    page_stack.emplace_back(std::move(to_page));
  else
    page_stack.back() = std::move(to_page);
}

Game_Instance *Route::get_instance() const
{
  return pimpl->game_instance;
}

void Route::push_page(Arc<Page> page)
{
  pimpl->page_stack.emplace_back(std::move(page));
}

void Route::pop_page()
{
  auto &page_stack = pimpl->page_stack;

  Li_Runtime_Assert(!page_stack.empty());

  if (page_stack.size() == 1)
    Li_WarnF("[Route] after pop, page-stack will become empty.");

  page_stack.pop_back();
}

void Route::update(real_t dt, utick_t dticks)
{
  /* Li_Assert(!page_stack.empty()); */

  auto &page_stack = pimpl->page_stack;

  auto *page = page_stack.back().get();

  pimpl->ticktack.global_time_elpased += dt;
  pimpl->ticktack.delta_time_elpased   = dt;
  pimpl->ticktack.global_tick         += dticks;
  pimpl->ticktack.delta_tick           = dticks;

  page->update({ pimpl->ticktack, pimpl->world, { page, { }, { }, { }, { }, { }, { }, { } } });
  page->update_page(this, dt, dticks);
}

void Route::render(const Render_Context &rc)
{
  auto &page_stack = pimpl->page_stack;

  auto *page = page_stack.back().get();

  page->render_page(this, rc);
}

bool Route::has_page() const
{
  return !pimpl->page_stack.empty();
}

void Route::clear_pages()
{
  pimpl->page_stack.clear();
}

void Route::mark_terminated()
{
  pimpl->terminated = true;
}

bool Route::is_terminated() const
{
  return pimpl->terminated;
}

} // namespace Li

