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

#ifndef PP_H_INCLUDED_QWDULJKZ
#define PP_H_INCLUDED_QWDULJKZ

namespace Li { namespace details { } }

/* {{{ core */
#define Li_Prefix                  ::Li::
#define Li_Impl_Prefix             ::Li::details::

#define Li_Std_Prefix              ::std::

#define Li_Cat2(a, b)              Li_Cat2_Prim(a, b)
#define Li_Cat2_Prim(a, b)         a ## b

#define Li_To_Str(s)               Li_To_Str_Prim(s)
#define Li_To_Str_Prim(s)          #s

#define Li_Car(x, ...)             x
#define Li_Cdr(x, ...)             __VA_ARGS__

/* }}} */

/* {{{ nargs */

#define $Li_Nargs_Select( _0, _1, _2, _3 \
                        , _4, _5, _6, _7 \
                        , _8, _9, _A, _B \
                        , _C, _D, _E, _F \
                        , _G,  Q, ...)   Q

#define $Li_Nargs_Fwd(...) $Li_Nargs_Select(__VA_ARGS__)
#define $Li_Nargs_Dummy    16, 15, 14, 13, 12, 11, 10,  9 \
                         ,  8,  7,  6,  5,  4,  3,  2,  1 \
                         ,  0

#define Li_Nargs(...) $Li_Nargs_Fwd($, ##__VA_ARGS__, $Li_Nargs_Dummy)

/* }}} */

/* {{{ xgen */

#define $Li_Join_2(L, R)           Li_Cat2(L, R)
#define $Li_Join_3(L, R...)        Li_Cat2(L, $Li_Join_2(R))
#define $Li_Join_4(L, R...)        Li_Cat2(L, $Li_Join_3(R))
#define $Li_Join_5(L, R...)        Li_Cat2(L, $Li_Join_4(R))
#define $Li_Join_6(L, R...)        Li_Cat2(L, $Li_Join_5(R))
#define $Li_Join_7(L, R...)        Li_Cat2(L, $Li_Join_6(R))
#define $Li_Join_8(L, R...)        Li_Cat2(L, $Li_Join_7(R))
#define $Li_Join_9(L, R...)        Li_Cat2(L, $Li_Join_8(R))
#define $Li_Join_10(L, R...)       Li_Cat2(L, $Li_Join_9(R))
#define $Li_Join_11(L, R...)       Li_Cat2(L, $Li_Join_10(R))
#define $Li_Join_12(L, R...)       Li_Cat2(L, $Li_Join_11(R))
#define $Li_Join_13(L, R...)       Li_Cat2(L, $Li_Join_12(R))
#define $Li_Join_14(L, R...)       Li_Cat2(L, $Li_Join_13(R))
#define $Li_Join_15(L, R...)       Li_Cat2(L, $Li_Join_14(R))

#define Li_Join(R...)              Li_Cat2($Li_Join_, Li_Nargs(R))(R)

#define $Li_Map_1(M, A, R...)      M(A)
#define $Li_Map_2(M, A, R...)      M(A), $Li_Map_1(M, ##R)
#define $Li_Map_3(M, A, R...)      M(A), $Li_Map_2(M, ##R)
#define $Li_Map_4(M, A, R...)      M(A), $Li_Map_3(M, ##R)
#define $Li_Map_5(M, A, R...)      M(A), $Li_Map_4(M, ##R)
#define $Li_Map_6(M, A, R...)      M(A), $Li_Map_5(M, ##R)
#define $Li_Map_7(M, A, R...)      M(A), $Li_Map_6(M, ##R)
#define $Li_Map_8(M, A, R...)      M(A), $Li_Map_7(M, ##R)
#define $Li_Map_9(M, A, R...)      M(A), $Li_Map_8(M, ##R)
#define $Li_Map_10(M, A, R...)     M(A), $Li_Map_9(M, ##R)
#define $Li_Map_11(M, A, R...)     M(A), $Li_Map_10(M, ##R)
#define $Li_Map_12(M, A, R...)     M(A), $Li_Map_11(M, ##R)
#define $Li_Map_13(M, A, R...)     M(A), $Li_Map_12(M, ##R)
#define $Li_Map_14(M, A, R...)     M(A), $Li_Map_13(M, ##R)
#define $Li_Map_15(M, A, R...)     M(A), $Li_Map_14(M, ##R)

#define Li_Map(M, R...)            Li_Cat2($Li_Map_, Li_Nargs(R))(M, ##R)

/* }}} */

#endif // end of include guard: PP_H_INCLUDED_QWDULJKZ

