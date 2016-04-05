/*
 * MIT License
 *
 * TARGA Copyright (c) 2016 Sebastien Serre <ssbx@sysmo.io>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <targa.h>

// Minunit include BEGIN
/* Copyright (C) 2002 John Brewer */
#define mu_assert(message, test) do { \
    if (!(test)) return message; \
} while (0)
#define mu_run_test(test) do { \
    char *message = test(); \
    tests_run++; \
    if (message) return message; \
} while (0)
int tests_run = 0;
// Minunit include END


static char* test_targaLoad() {

    return NULL;

}

static char* targa_test(char* test_name) {

    return NULL;

}

int main(int argc, char* argv[])
{

    int loadStatus;
    unsigned char w, h;
    targaLoad(argv[1], &loadStatus, &w, &h);

    printf("status is: %d\n", loadStatus);

    /*
    char* result = obj_test(argv[1]);

    if (result != 0)
        printf("%s\n", result);

    return result != NULL;
    */

}

