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

#ifndef REFPTR_H_INCLUDED_T8NEH6Z3
#define REFPTR_H_INCLUDED_T8NEH6Z3

#include "../prelude.h"

namespace Li {

template <class T>
using Ref_Ptr = Ptr<T>;

template <class T, class ...Args>
static inline Ref_Ptr<T> make_ref_ptr(Args &&...args)
{ return std::make_unique<T>(std::forward<Args>(args)...); }

template <class T>
static inline Ref_Ptr<T> own(T *ptr)
{ return Ref_Ptr<T>(ptr); }

template <class Y, class T>
static inline Ref_Ptr<Y> own(T *ptr)
{ return Ref_Ptr<Y>((Y *)ptr); }

template <class T>
struct Ref_Wrapper
{
  Ref_Ptr<T> ptr;

  Ref_Wrapper(const Ref_Wrapper &rw)
    : ptr(rw.ptr->clone()->template as<T>())
  { }

  Ref_Wrapper(const Ref_Ptr<T> &ptr)
    : ptr(ptr->clone()->template as<T>())
  { }

  Ref_Wrapper(Ref_Ptr<T> &ptr)
    : ptr(std::move(ptr))
  { }

  Ref_Wrapper() = default;

  Ref_Wrapper(Ref_Wrapper &&) = default;

  Ref_Wrapper &operator =(Ref_Wrapper &&)    = default;
  Ref_Wrapper &operator =(const Ref_Wrapper &rw)
  {
    ptr = own<T>(rw.ptr->clone());
    return *this;
  }

  inline T *operator ->() { return ptr.get(); }
  inline T *operator ->() const { return ptr.get(); }
};

template <class T>
static inline Ref_Wrapper<T> wrap(T *ptr)
{ return { own(ptr) }; }

template <class T, typename ...Args>
static inline Ref_Wrapper<T> make_ref_wrapper(Args &&...args)
{ return wrap(make_ref_ptr<T>(std::forward<Args>(args)...)); }

} // namespace Li

#endif // end of include guard: REFPTR_H_INCLUDED_T8NEH6Z3

