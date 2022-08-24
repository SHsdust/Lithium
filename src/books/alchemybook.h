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

#ifndef ALCHEMYBOOK_H_INCLUDED_X6XK4NVF
#define ALCHEMYBOOK_H_INCLUDED_X6XK4NVF

#include "prototype.h"

namespace Li {

namespace details {
#define $impl(T...) $chain(Source<UT>, (Copy_Via_CCTOR, T), (Pretty_Runtime_Type, T))

template <class T>
using F_Ptr = Ref_Ptr<Source<T>>;

template <typename UT>
class Const : public $impl(Const<UT>)
{
  UT constant;

public:
  Const(UT constant) : constant(constant)
  { }

private:
  virtual UT do_eval(attr_unused const Update_Ctx &uc) override final
  { return constant; }
};

template <typename UT>
class Inc : public $impl(Inc<UT>)
{
  UT               initial;
  F_Ptr<UT>        inc;

public:
  Inc(UT          initial,
      F_Ptr<UT> &&inc)
    : initial(initial)
    , inc(std::move(inc))
  { }

  Inc(const Inc &inc)
    : initial(inc.initial)
    , inc(inc.inc->clone()->template as<Source<UT>>())
  { }

public:
  virtual UT do_eval(const Update_Ctx &uc) override final
  {
    return initial += inc->eval(uc);
  }
};

template <class UT, template <typename> class Op>
class BinOp : public $impl(BinOp<UT, Op>)
{
  Op<UT> op;
  F_Ptr<UT> lhs, rhs;

public:
  BinOp(F_Ptr<UT> &&lhs,
        F_Ptr<UT> &&rhs)
    : lhs(std::move(lhs))
    , rhs(std::move(rhs))
  { }

  BinOp(const BinOp &binop)
    : lhs(binop.lhs->clone()->template as<Source<UT>>())
    , rhs(binop.rhs->clone()->template as<Source<UT>>())
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  {
    auto lhsv = lhs->eval(uc);
    auto rhsv = rhs->eval(uc);

    return op(lhsv, rhsv);
  }
};

template <typename UT>
class Neg : public $impl(Neg<UT>)
{
  F_Ptr<UT> val;

public:
  Neg(F_Ptr<UT> &&val) : val(std::move(val))
  { }

  Neg(const Neg &neg)
    : val(neg.val->clone()->template as<Source<UT>>())
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  { return -1 * val->eval(uc); }
};


template <class T, typename ...Args>
auto f_ptr(Args &&...args)
{
  return own((Source<typename T::underlying_type> *)
             new T(std::forward<Args>(args)...));
}

template <typename UT>
static inline auto f_const(UT constant)
{ return f_ptr<Const<UT>>(constant); }

template <typename UT>
static inline auto f_inc(UT          initial,
                         F_Ptr<UT> &&inc)
{ return f_ptr<Inc<UT>>(initial, std::move(inc)); }

template <typename UT>
static inline auto f_plus(F_Ptr<UT> &&lhs,
                          F_Ptr<UT> &&rhs)
{
  return f_ptr<BinOp<UT, std::plus>>(std::move(lhs),
                                     std::move(rhs));
}

template <typename UT>
static inline auto f_minus(F_Ptr<UT> &&lhs,
                           F_Ptr<UT> &&rhs)
{
  return f_ptr<BinOp<UT, std::minus>>(std::move(lhs),
                                      std::move(rhs));
}

template <typename UT>
static inline auto f_multiplies(F_Ptr<UT> &&lhs,
                                F_Ptr<UT> &&rhs)
{
  return f_ptr<BinOp<UT, std::multiplies>>(std::move(lhs),
                                           std::move(rhs));
}

template <typename UT>
static inline auto f_divides(F_Ptr<UT> &&lhs,
                             F_Ptr<UT> &&rhs)
{
  return f_ptr<BinOp<UT, std::divides>>(std::move(lhs),
                                        std::move(rhs));
}

template <typename UT, template <typename> class Op>
static inline auto f_bin_op(F_Ptr<UT> &&lhs,
                            F_Ptr<UT> &&rhs)
{
  return f_ptr<BinOp<UT, Op>>(std::move(lhs),
                              std::move(rhs));
}

template <typename UT>
static inline auto f_neg(F_Ptr<UT> &&val)
{ return f_ptr<Neg<UT>>(std::move(val)); }

template <typename UT>
using Lambda_Of = std::function<UT (real_t                        /* time_elpased */,
                                    utick_t                       /* ticks */,
                                    const Update_Ctx &  /* uc */ )>;

template <typename UT>
class Lambda : public $impl(Lambda<UT>)
{
public:
  using Lambda_Fn = Lambda_Of<UT>;
private:
  Lambda_Fn fn;
public:
  Lambda(const Lambda &) = default;
  Lambda(Lambda_Fn &&fn) : fn(std::move(fn)) { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  { return fn(uc); }
};

template <typename UT>
static inline auto f_lambda(Lambda_Of<UT> &&fn)
{ return f_ptr<Lambda<UT>>(std::move(fn)); }

template <typename UT>
class Compose : public $impl(Compose<UT>)
{
  F_Ptr<UT> outer;
  F_Ptr<UT> inner;

public:
  Compose(F_Ptr<UT> &&outer, F_Ptr<UT> &&inner)
    : inner(std::move(inner))
    , outer(std::move(outer))
  { }

  Compose(const Compose &c)
    : inner(c.inner->clone()->template as<Source<UT>>())
    , outer(c.outer->clone()->template as<Source<UT>>())
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  { return outer->eval(inner->eval(uc)); }
};

template <typename UT>
static inline
auto f_compose(F_Ptr<UT> &&outer, F_Ptr<UT> &&inner)
{
  return f_ptr<Compose<UT>>(std::move(outer), std::move(inner));
}

} // namespace details

using details::f_const;
using details::f_inc;
using details::f_plus;
using details::f_minus;
using details::f_multiplies;
using details::f_divides;
using details::f_bin_op;
using details::f_neg;
using details::f_lambda;
using details::f_compose;

namespace details {

template <typename UT>
class Chain : public $impl(Chain<UT>)
{
  F_Ptr<UT>   span_fn;
  real_t      span = 0;
  F_Ptr<UT>   next_fn;

public:
  Chain(F_Ptr<UT> &&span_fn,
        real_t      span,
        F_Ptr<UT> &&next_fn)
    : span_fn(std::move(span_fn))
    , span(span)
    , next_fn(std::move(next_fn))
  { }

  Chain(const Chain &chain)
    : span_fn(chain.span_fn->clone()->template as<Source<UT>>())
    , span(chain.span)
    , next_fn(chain.next_fn->clone()->template as<Source<UT>>())
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  {
    if (static_cast<Update_Object *>(this)->get_time_elpased() > span)
      return next_fn->eval(uc);
    else
      return span_fn->eval(uc);
  }
};

template <typename UT>
class Tick_Chain : public $impl(Tick_Chain<UT>)
{
  F_Ptr<UT>   span_fn;
  utick_t     span = 0;
  F_Ptr<UT>   next_fn;

public:
  Tick_Chain(F_Ptr<UT> &&span_fn,
             utick_t     span,
             F_Ptr<UT> &&next_fn)
    : span_fn(std::move(span_fn))
    , span(span)
    , next_fn(std::move(next_fn))
  { }

  Tick_Chain(const Tick_Chain &chain)
    : span_fn(chain.span_fn->clone()->template as<Source<UT>>())
    , span(chain.span)
    , next_fn(chain.next_fn->clone()->template as<Source<UT>>())
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  {
    if (static_cast<Update_Object *>(this)->get_ticks() > span)
      return next_fn->eval(uc);
    else
      return span_fn->eval(uc);
  }
};

template <typename UT>
static inline auto f_chain(F_Ptr<UT> &&span_fn,
                           real_t      span,
                           F_Ptr<UT> &&next_fn)
{
  return f_ptr<Chain<UT>>(std::move(span_fn),
                          span,
                          std::move(next_fn));
}

template <typename UT>
static inline auto f_chain(F_Ptr<UT> &&span_fn,
                           utick_t     ticks,
                           F_Ptr<UT> &&next_fn)
{
  return f_ptr<Tick_Chain<UT>>(std::move(span_fn),
                               ticks,
                               std::move(next_fn));
}

static constexpr const unsigned INF_LOOP = std::numeric_limits<unsigned>::max();

template <typename UT>
class Repeat : public $impl(Repeat<UT>)
{
  F_Ptr<UT> prototype;
  F_Ptr<UT> rest;

  F_Ptr<UT> body;
  unsigned  count;
  real_t    duration;

  unsigned  applied_count = 0;

public:
  Repeat(F_Ptr<UT> &&prototype,
         F_Ptr<UT> &&rest,
         unsigned count,
         real_t duration)
    : prototype(std::move(prototype))
    , rest(std::move(rest))
    , count(count)
    , duration(duration)
  {
    body = own<Source<UT>>(this->prototype->clone());
  }

  Repeat(const Repeat &repeat)
    : prototype(repeat.prototype->clone()->template as<Source<UT>>())
    , rest(repeat.rest->clone()->template as<Source<UT>>())
    , body(repeat.body->clone()->template as<Source<UT>>())
    , count(repeat.count)
    , duration(repeat.duration)
    , applied_count(repeat.applied_count)
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  {
    if (static_cast<Update_Object *>(this)->get_time_elpased() > duration * applied_count)
    {
      if (count != INF_LOOP && applied_count == count)
        return rest->eval(uc);

      /* next cycle. */
      body = own(prototype->clone()->template as<Source<UT>>());
      ++applied_count;
    }

    return body->eval(uc);
  }
};

template <typename UT>
class Tick_Repeat : public $impl(Tick_Repeat<UT>)
{
  F_Ptr<UT> prototype;
  F_Ptr<UT> rest;

  F_Ptr<UT> body;
  unsigned  count;
  utick_t   ticks;

  unsigned  applied_count = 0;

public:
  Tick_Repeat(F_Ptr<UT> &&prototype,
              F_Ptr<UT> &&rest,
              unsigned count,
              utick_t  ticks)
    : prototype(std::move(prototype))
    , rest(std::move(rest))
    , count(count)
    , ticks(ticks)
  { }

  Tick_Repeat(const Tick_Repeat &repeat)
    : prototype(repeat.prototype->clone()->template as<Source<UT>>())
    , rest(repeat.rest->clone()->template as<Source<UT>>())
    , body(repeat.body->clone()->template as<Source<UT>>())
    , count(repeat.count)
    , ticks(repeat.ticks)
    , applied_count(repeat.applied_count)
  { }

private:
  virtual UT do_eval(const Update_Ctx &uc) override final
  {
    if (static_cast<Update_Object *>(this)->get_ticks() > ticks * applied_count)
    {
      if (count != INF_LOOP && applied_count == count)
        return rest->eval(uc);

      /* next cycle. */
      body = own(prototype->clone()->template as<Source<UT>>());
      ++applied_count;
    }

    return body->eval(uc);
  }
};

template <typename UT>
static inline auto f_repeat(F_Ptr<UT> &&body_prototype,
                            F_Ptr<UT> &&rest,
                            real_t      duration,
                            unsigned    count = INF_LOOP)
{
  return f_ptr<Repeat<UT>>(std::move(body_prototype),
                           std::move(rest),
                           count,
                           duration);
}

template <typename UT>
static inline auto f_repeat(F_Ptr<UT> &&body_prototype,
                            F_Ptr<UT> &&rest,
                            utick_t     ticks,
                            unsigned    count = INF_LOOP)
{
  return f_ptr<Tick_Repeat<UT>>(std::move(body_prototype),
                                std::move(rest),
                                count,
                                ticks);
}

} // namespace details

using details::f_chain;
using details::f_repeat;
using details::INF_LOOP;

static inline auto f_make_vector(R_Source_Ptr x,
                                 R_Source_Ptr y)
{
  class V_Motion : public $chain( V_Source
                                , (Copy_Via_CCTOR, V_Motion)
                                , (Pretty_Runtime_Type, V_Motion))
  {
    Ref_Wrapper<R_Source> x, y;

  public:
    V_Motion(R_Source_Ptr &&x,
             R_Source_Ptr &&y)
      : x(std::move(x))
      , y(std::move(y))
    { }

    V_Motion(const V_Motion &) = default;

  private:
    virtual vec2_t do_eval(const Update_Ctx &uc) override final
    { return { x->eval(uc), y->eval(uc) }; }
  };

  return details::f_ptr<V_Motion>(std::move(x), std::move(y));
}

attr_export
V_Source_Ptr f_rotate(R_Source_Ptr &&i,
                      real_t       theta);

attr_export
V_Source_Ptr f_rotate(R_Source_Ptr &&i,
                      R_Source_Ptr &&theta);

attr_export
V_Source_Ptr f_rotate(V_Source_Ptr &&v,
                      R_Source_Ptr &&theta);


} // namespace Li

#undef $impl

#endif // end of include guard: ALCHEMYBOOK_H_INCLUDED_X6XK4NVF

