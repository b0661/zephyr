..
    Copyright (c) 2015 Paul Mensonides
    Copyright (c) 2018 Bobby Noelte
    SPDX-License-Identifier: Apache-2.0

.. _chaos-pp-document-example-cartesian-product-array:

Example: Populate an array with the cartesian product of two other arrays
#########################################################################

Populate an array with the cartesian product of two other arrays.
They contain 4-bit values, which go to the lower and upper
nibble of the element in the resulting array.

Taken from `stackoverflow <https://stackoverflow.com/questions/31267582/cartesian-product-of-two-arrays-in-c-using-preprocessor/31282963#31282963>`_.

.. code-block:: C

    #include <stdio.h>
    #include <stdlib.h>

    #include <chaos/preprocessor/algorithm/for_each_product.h>
    #include <chaos/preprocessor/recursion/expr.h>
    #include <chaos/preprocessor/seq/core.h>
    #include <chaos/preprocessor/seq/elem.h>
    #include <chaos/preprocessor/seq/enumerate.h>

    #define A(l, h) \
        const unsigned char \
            a1[] = { CHAOS_PP_SEQ_ENUMERATE(l) }, \
            a2[] = { CHAOS_PP_SEQ_ENUMERATE(h) }, \
            a21[] = { \
                CHAOS_PP_SEQ_ENUMERATE( \
                    CHAOS_PP_EXPR(CHAOS_PP_FOR_EACH_PRODUCT( \
                        B, \
                        ((CHAOS_PP_SEQ) l) \
                        ((CHAOS_PP_SEQ) h) \
                    )) \
                ) \
            }; \
        /**/
    #define B(s, seq) \
        (CHAOS_PP_SEQ_ELEM(0, seq) | (CHAOS_PP_SEQ_ELEM(1, seq) << 4)) \
        /**/

    A((0x1)(0x2), (0x3)(0x4))

    #undef A
    #undef B

    int main() {
        for (int i = 0; i != sizeof(a21) / sizeof(unsigned char); ++i) {
            printf("0x%x\n", a21[i]);
        }
        return EXIT_SUCCESS;
    }


Resulting array:

.. code-block:: C

    const char a1[] = {0x1, 0x2};
    const char a2[] = {0x3, 0x4};
    const char a21[] = {0x31, 0x32, 0x41, 0x42};
