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


#include "spellbook.h"
#include "alchemybook.h"
#include "../core/particle.h"
#include "../core/updatectx.h"

namespace Li {

#define $impl(Klass) $chain(Task, (Copy_Via_CCTOR, Klass), (Pretty_Runtime_Type, Klass))

class C_Nil : public $impl(C_Nil)
{
public:
  using Super::Super;

private:
  virtual bool do_exec(attr_unused const Update_Ctx &uc) override final
  { return false; }
};

class C_Unit : public $impl(C_Unit)
{
public:
  using Super::Super;

private:
  virtual bool do_exec(attr_unused const Update_Ctx &uc) override final
  { return true; }
};

class C_Id : public $impl(C_Id)
{
  Task_Ptr id;
public:
  C_Id(Task_Ptr &&id) : id(std::move(id))
  { }

  C_Id(const C_Id &id)
    : id(id.id->clone()->as<Task>())
  { }

private:
  virtual bool do_exec(attr_unused const Update_Ctx &uc) override final
  { return id->exec(uc); }
};

class C_Interval : public $impl(C_Interval)
{
  Task_Ptr combined;
  R_Source_Ptr   interval;
  real_t         slept = 0;
public:
  C_Interval(Task_Ptr &&combined,
             R_Source_Ptr   &&interval)
    : combined(std::move(combined))
    , interval(std::move(interval))
  { }

  C_Interval(const C_Interval &interval)
    : C_Interval(own(interval.combined->clone_as<Task>()),
                 own(interval.interval->clone_as<R_Source>()))
  { }
private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (slept += uc.ticktack.delta_time_elpased > interval->eval(uc))
    {
      slept = 0;
      return combined->exec(uc);
    }

    return true;
  }
};

class C_Tick_Interval : public $impl(C_Tick_Interval)
{
  Task_Ptr combined;
  U_Source_Ptr   interval;
  utick_t        slept = 0;
public:
  C_Tick_Interval(Task_Ptr &&combined,
                  U_Source_Ptr   &&interval)
    : combined(std::move(combined))
    , interval(std::move(interval))
  { }

  C_Tick_Interval(const C_Tick_Interval &interval)
    : C_Tick_Interval(own(interval.combined->clone_as<Task>()),
                      own(interval.interval->clone_as<U_Source>()))
  { }
private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (slept += uc.ticktack.delta_tick > interval->eval(uc))
    {
      slept = 0;
      return combined->exec(uc);
    }

    return true;
  }
};

class C_Guard : public $impl(C_Guard)
{
  Task_Ptr combined;
  real_t         span;
public:
  C_Guard(Task_Ptr &&combined, real_t span)
    : combined(std::move(combined))
    , span(span)
  { }

  C_Guard(const C_Guard &guard)
    : C_Guard(own(guard.combined->clone_as<Task>()), guard.span)
  { }

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (get_time_elpased() > span)
      return false;
    else
      return combined->exec(uc);
  }
};

class C_Tick_Guard : public $impl(C_Tick_Guard)
{
  Task_Ptr combined;
  utick_t        ticks;
public:
  C_Tick_Guard(Task_Ptr &&combined, utick_t ticks)
    : combined(std::move(combined))
    , ticks(ticks)
  { }

  C_Tick_Guard(const C_Tick_Guard &guard)
    : C_Tick_Guard(own(guard.combined->clone_as<Task>()), guard.ticks)
  { }

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (get_ticks() > ticks)
      return false;
    else
      return combined->exec(uc);
  }
};

class C_Chain : public $impl(C_Chain)
{
  Task_Ptr first, second;
public:
  C_Chain(Task_Ptr &&first, Task_Ptr &&second)
    : first(std::move(first))
    , second(std::move(second))
  { }

  C_Chain(const C_Chain &chain)
    : C_Chain(own(chain.first->clone_as<Task>()),
              own(chain.second->clone_as<Task>()))
  { }

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (first && !first->exec(uc))
      first = nullptr;
    if (first)
      return true;

    if (second && !second->exec(uc))
      second = nullptr;
    if (second)
      return true;

    return false;
  }
};

class C_Compose_Eagerly : public $impl(C_Compose_Eagerly)
{
  Task_Ptr first, second;
public:
  C_Compose_Eagerly(Task_Ptr &&first, Task_Ptr &&second)
    : first(std::move(first))
    , second(std::move(second))
  { }

  C_Compose_Eagerly(const C_Compose_Eagerly &compose)
    : C_Compose_Eagerly(own(compose.first->clone_as<Task>()),
                        own(compose.second->clone_as<Task>()))
  { }

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  { return first->exec(uc) && second->exec(uc); }
};

class C_Compose_Patiently : public $impl(C_Compose_Patiently)
{
  Task_Ptr first, second;
public:
  C_Compose_Patiently(Task_Ptr &&first, Task_Ptr &&second)
    : first(std::move(first))
    , second(std::move(second))
  { }

  C_Compose_Patiently(const C_Compose_Patiently &compose)
    : C_Compose_Patiently(own(compose.first->clone_as<Task>()),
                        own(compose.second->clone_as<Task>()))
  { }

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto result  = first->exec(uc);
    auto result2 = second->exec(uc);
    return result || result2;
  }
};

class C_If_Then_Else : public $impl(C_If_Then_Else)
{
  Predict pred;
  Task_Ptr then_procedure;
  Task_Ptr else_procedure;
public:
  C_If_Then_Else(Predict        &&pred,
                 Task_Ptr &&then_procedure,
                 Task_Ptr &&else_procedure)
    : pred(std::move(pred))
    , then_procedure(std::move(then_procedure))
    , else_procedure(std::move(else_procedure))
  { }

  C_If_Then_Else(const C_If_Then_Else &if_then_else)
    : C_If_Then_Else(Predict(if_then_else.pred),
                     own(if_then_else.then_procedure->clone_as<Task>()),
                     own(if_then_else.else_procedure->clone_as<Task>()))
  { }

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  { return pred(uc) ? then_procedure->exec(uc) : else_procedure->exec(uc); }
};

using V_Pair = std::pair<V_Source_Ptr, V_Source_Ptr>;

class C_Move : public $impl(C_Move)
{
  Ref_Wrapper<V_Source> motion;
public:
  C_Move(V_Source_Ptr &&motion)
    : motion(std::move(motion))
  { }

  C_Move(const C_Move &move) = default;

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto *particle = uc.target.particle->as<Particle>();

    auto next_position = motion->eval(uc);

    particle->mark_next_position(next_position);

    return true;
  }
};

class C_Move_Rel : public $impl(C_Move_Rel)
{
  Ref_Wrapper<V_Source> position;
public:
  C_Move_Rel(V_Source_Ptr &&motion)
    : position(std::move(motion))
  { }

  C_Move_Rel(const C_Move_Rel &move) = default;

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    auto *particle = uc.target.particle->as<Particle>();

    auto next_position = position->eval(uc);

    particle->off_next_position(next_position);

    return true;
  }
};

class C_Wait : public $impl(C_Wait)
{
  Arc<bool> signal;
  Ref_Wrapper<Task> then;
public:
  C_Wait(Arc<bool> signal, Ref_Ptr<Task> &&then)
    : signal(signal)
    , then(std::move(then))
  { }

  C_Wait(const C_Wait &) = default;

private:
  virtual bool do_exec(const Update_Ctx &uc) override final
  {
    if (signal && !*signal)
      return true;

    if (signal && *signal)
      signal.reset();

    return then->exec(uc);
  }
};

template <class T, typename ...Args>
static inline
Task_Ptr mk(Args &&...args)
{
  return own(static_cast<Task *>(new T(std::forward<Args>(args)...)));
}

Task_Ptr c_nil()  { return mk<C_Nil>();  }
Task_Ptr c_unit() { return mk<C_Unit>(); }

Task_Ptr c_id(Task_Ptr &&id)
{ return mk<C_Id>(std::move(id)); }

Task_Ptr c_interval(Task_Ptr &&combined, real_t interval)
{ return c_interval(std::move(combined), f_const(interval)); }

Task_Ptr c_interval(Task_Ptr &&combined, R_Source_Ptr &&interval)
{ return mk<C_Interval>(std::move(combined), std::move(interval)); }

Task_Ptr c_interval(Task_Ptr &&combined, utick_t interval)
{ return c_interval(std::move(combined), f_const(interval)); }

Task_Ptr c_interval(Task_Ptr &&combined, U_Source_Ptr &&interval)
{ return mk<C_Tick_Interval>(std::move(combined), std::move(interval)); }

Task_Ptr c_guard(Task_Ptr &&combined, real_t span)
{ return mk<C_Guard>(std::move(combined), span); }

Task_Ptr c_guard(Task_Ptr &&combined, utick_t ticks)
{ return mk<C_Tick_Guard>(std::move(combined), ticks); }

Task_Ptr c_chain(Task_Ptr &&first, Task_Ptr &&second)
{ return mk<C_Chain>(std::move(first), std::move(second)); }

Task_Ptr c_compose_eagerly(Task_Ptr &&first, Task_Ptr &&second)
{ return mk<C_Compose_Eagerly>(std::move(first), std::move(second)); }

Task_Ptr c_compose_patiently(Task_Ptr &&first, Task_Ptr &&second)
{ return mk<C_Compose_Patiently>(std::move(first), std::move(second)); }

Task_Ptr c_if_then_else(Predict  &&pred,
                        Task_Ptr &&then_procedure,
                        Task_Ptr &&else_procedure)
{
  return mk<C_If_Then_Else>(std::move(pred),
                            std::move(then_procedure),
                            std::move(else_procedure));
}

Task_Ptr c_move(V_Source_Ptr &&func)
{ return mk<C_Move>(std::move(func)); }

Task_Ptr c_move_rel(V_Source_Ptr &&func)
{ return mk<C_Move_Rel>(std::move(func)); }

Task_Ptr c_wait(Arc<bool> signal, Task_Ptr &&then)
{ return mk<C_Wait>(signal, std::move(then)); }

} // namespace Li

