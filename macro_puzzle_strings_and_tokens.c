/**
 * puzzle about "stringize" and "tokenize" macro operators.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 * Copyright (c) 2022 Alexander Grishankov
 */


#include <stdio.h>


/* Join a and b into a single token: ab */
#define JOIN_UNEXPANDED_TOKENS(a, b) a##b
/* Expand a and b and join resulting tokens into one */
#define JOIN_EXPANDED_TOKENS(a, b) JOIN_UNEXPANDED_TOKENS(a, b)
/* Double-Quote symbols inside parenthesis */
#define CREATE_UNEXPANDED_STRING(...) #__VA_ARGS__
/* Expand symbols inside parenthesis and double-quote the result */
#define CREATE_EXPANDED_STRING(...) CREATE_UNEXPANDED_STRING(__VA_ARGS__)


#define a this_is_a
#define b this_is_b


void f(void)
{
    const char *JOIN_UNEXPANDED_TOKENS(a, b)
        = CREATE_UNEXPANDED_STRING(a b);
    const char *JOIN_EXPANDED_TOKENS(a, b)
        = CREATE_EXPANDED_STRING(a b);
    printf(
        CREATE_EXPANDED_STRING(JOIN_UNEXPANDED_TOKENS(a, b)) " = %s\n",
        JOIN_UNEXPANDED_TOKENS(a, b)
    );
    printf(
        CREATE_EXPANDED_STRING(JOIN_EXPANDED_TOKENS(a, b)) " = %s\n",
        JOIN_EXPANDED_TOKENS(a, b)
    );
}


int main(void)
{
    f();

    return 0;
}