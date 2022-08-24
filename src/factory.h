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

#ifndef FACTORY_H_INCLUDED_O3FYTZJ1
#define FACTORY_H_INCLUDED_O3FYTZJ1

#include "gamecfg.h"

namespace Li {

class Task;

class Obj_Factory
{
  Map<Str, Ptr<Unit>> reg;
public:
  Obj_Factory();

  void  register_obj(const Str  &key, Ptr<Unit> &&obj);

  Ref_Ptr<Unit> acquire_obj(const Str &key, bool silence = false);

  /**
   * 调用者有释放资源的责任(delete)
   */
  Unit         *acquire_obj_unsafe(const Str &key, bool silence = false);

  /**
   * 简单飞行物, 无碰撞，默认trigger为Destory.
   */
  Ref_Ptr<Unit> acquire_simple_bullet(Unit *spawner,
                                      Ptr<Task> &&task);

  /**
   * 简单渲染器,旋转角度由速度方向+<code>theta_off</code>指定.由<code>resource_id</code>指定贴图.
   */
  Ref_Ptr<Component> acquire_simple_render(uid_t resource_id, real_t theta_off);

  /**
   * 简单渲染器,旋转角度固定.由<code>resource_id</code>指定贴图.
   */
  Ref_Ptr<Component> acquire_dirfree_render(uid_t resource_id, real_t theta);
};

} // namespace Li

#endif // end of include guard: FACTORY_H_INCLUDED_O3FYTZJ1

