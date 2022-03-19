/**
 * Show step by step deciphering of obscure and "recursive" macros.
 * 
 * Sources used:
 * https://gcc.gnu.org/onlinedocs/cpp/Macros.html
 * https://gcc.gnu.org/onlinedocs/cppinternals/Macro-Expansion.html
 * https://stackoverflow.com/questions/12447557/can-we-have-recursive-macros
 */


#include <stdio.h>


#define HA_TOKENIZE_UNEXPANDED(tok1, tok2) tok1##tok2
#define HA_TOKENIZE(tok1, tok2) HA_TOKENIZE_UNEXPANDED(tok1, tok2)
#define HA_STRINGIZE_UNEXPANDED(...) #__VA_ARGS__
#define HA_STRINGIZE(...) HA_STRINGIZE_UNEXPANDED(__VA_ARGS__)

#define EMPTY(...)
#define DEFER(...) __VA_ARGS__ EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define pr_id() pr
#define pr(n) ((n == 1) ? 1 : DEFER(pr_id)()(n - 1))


/**
 * Symbols used:
 *      : macro under expansion
 *      % definition of macro under expansion
 *      ? parameter under expansion
 *      ^ expanded parameter
 *      ! macros disabled for expansion
 *      > expanded macro
 *      ~ disabled token
 */
int main(void)
{
    #define f(x) x
    #define a(x) f(x)
    printf("%s\n", HA_STRINGIZE(f(a(f(2)))));
    /**
     * HA_STRINGIZE(f(a(f(2))))
     *  : HA_STRINGIZE(f(a(f(2))))
     *  % HA_STRINGIZE(...) HA_STRINGIZE_UNEXPANDED(__VA_ARGS__)
     *  ? __VA_ARGS__
     *  ^ f(a(f(2)))
     *      : f(a(f(2)))
     *      % f(x) x
     *      ? x
     *      ^ a(f(2))
     *          : a(f(2))
     *          % a(x) f(x)
     *          ? x
     *          ^ f(2)
     *              : f(2)
     *              % f(x) x
     *              ? x
     *              ^ 2
     *              ! f
     *              > 2
     *          ! a
     *          > f(2)
     *              : f(2)
     *              % f(x) x
     *              ? x
     *              ^ 2
     *              ! a f
     *              > 2
     *          > 2
     *      ^ 2
     *      ! f
     *      > 2
     *  ^ 2
     *  ! HA_STRINGIZE
     *  > HA_STRINGIZE_UNEXPANDED(2)
     *      : HA_STRINGIZE_UNEXPANDED(2)
     *      % HA_STRINGIZE_UNEXPANDED(...) #__VA_ARGS__
     *      ? __VA_ARGS__
     *      ^ 2
     *      ! HA_STRINGIZE HA_STRINGIZE_UNEXPANDED
     *      > "2"
     *  > "2"
     */
    #undef f
    #undef a

    #define a baz
    #define b (2)
    #define foo(x) x
    #define bar a b a (b) foo(foo) foo(baz) (b) foo(baz)a (b)
    #define baz(x) x
    printf("%s\n", HA_STRINGIZE(bar));
    /**
     * HA_STRINGIZE(bar)
     *  : HA_STRINGIZE(bar)
     *  % HA_STRINGIZE(...) HA_STRINGIZE_UNEXPANDED(__VA_ARGS__)
     *  ? __VA_ARGS__
     *  ^ bar
     *      : bar
     *      % bar a b a (b) foo(foo) foo(baz) (b) foo(baz)a (b)
     *      ! bar
     *      > a b a (b) foo(foo) foo(baz) (b) foo(baz)a (b)
     *          : a
     *          % a baz
     *          ! bar a
     *          > baz
     *      > baz b a (b) foo(foo) foo(baz) (b) foo(baz)a (b)
     *          : b
     *          % b (2)
     *          ! bar b
     *          > (2)
     *      > baz (2) a (b) foo(foo) foo(baz) (b) foo(baz)a (b)
     *          : a
     *          % a baz
     *          ! bar a
     *          > baz
     *      > baz (2) baz (b) foo(foo) foo(baz) (b) foo(baz)a (b)
     *          : baz (b)
     *          % baz(x) x
     *          ? x
     *          ^ b
     *              : b
     *              % b (2)
     *              ! bar b
     *              > (2)
     *          ^ (2)
     *          ! bar baz
     *          > (2)
     *      > baz (2) (2) foo(foo) foo(baz) (b) foo(baz)a (b)
     *          : foo(foo)
     *          % foo(x) x
     *          ? x
     *          ^ foo
     *          ! bar foo
     *          > ~foo
     *      > baz (2) (2) ~foo foo(baz) (b) foo(baz)a (b)
     *          : foo(baz)
     *          % foo(x) x
     *          ? x
     *          ^ baz
     *          ! bar foo
     *          > baz
     *      > baz (2) (2) ~foo baz (b) foo(baz)a (b)
     *          : baz (b)
     *          % baz(x) x
     *          ? x
     *          ^ b
     *              : b
     *              % b (2)
     *              ! bar b
     *              > (2)
     *          ^ (2)
     *          ! bar baz
     *          > (2)
     *      > baz (2) (2) ~foo (2) foo(baz)a (b)
     *          : foo(baz)
     *          % foo(x) x
     *          ? x
     *          ^ baz
     *          ! bar foo
     *          > baz
     *      > baz (2) (2) ~foo (2) baza (b)
     *          : a
     *          % a baz
     *          ! bar a
     *          > baz
     *      > baz (2) (2) ~foo (2) bazbaz (b)
     *          : baz (b)
     *          % baz(x) x
     *          ? x
     *          ^ b
     *              : b
     *              % b (2)
     *              ! bar b
     *              > (2)
     *          ^ (2)
     *          ! bar baz
     *          > (2)
     *      > baz (2) (2) ~foo (2) baz(2)
     *  ^ baz (2) (2) ~foo (2) baz(2)
     *  ! HA_STRINGIZE
     *  > HA_STRINGIZE_UNEXPANDED(baz (2) (2) ~foo (2) baz(2))
     *      : HA_STRINGIZE_UNEXPANDED(baz (2) (2) ~foo (2) baz(2))
     *      % HA_STRINGIZE_UNEXPANDED(...) #__VA_ARGS__
     *      ? __VA_ARGS__
     *      ^ baz (2) (2) ~foo (2) baz(2)
     *          : baz (2)
     *          % baz(x) x
     *          ? x
     *          ^ 2
     *          ! HA_STRINGIZE baz
     *          > 2
     *      ^ 2 (2) ~foo (2) baz(2)
     *          : baz(2)
     *          % baz(x) x
     *          ? x
     *          ^ 2
     *          ! HA_STRINGIZE baz
     *          > 2
     *      ^ 2 (2) ~foo (2) 2
     *      ! HA_STRINGIZE HA_STRINGIZE_UNEXPANDED
     *      > "baz (2) (2) foo (2) baz(2)"
     *  > "baz (2) (2) foo (2) baz(2)"
     */
    #undef a
    #undef b
    #undef foo
    #undef bar
    #undef baz

    printf("%s\n", HA_STRINGIZE(OBSTRUCT(pr_id)));
    /** TODO
     * 
     * 
     */

    printf("%s\n", HA_STRINGIZE(pr(5)));
    /** TODO
     * 
     * 
     */

    printf("%s\n", HA_STRINGIZE(EXPAND(pr(5))));
    /** TODO
     * 
     * 
     */

    printf("%s\n", HA_STRINGIZE(EXPAND(EXPAND(pr(5)))));
    /** TODO
     * 
     * 
     */

    return 0;
}