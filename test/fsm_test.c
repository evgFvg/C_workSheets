
#include <stdio.h>     /* print */
#include <assert.h>

#include "fsm.h"


int main()
{
    assert(1 == IsValidString("01110"));
    assert(0 == IsValidString("0"));
    assert(0 == IsValidString("11001"));
    assert(0 == IsValidString("01111"));

    printf("All tests have been passed\n");

    return 0;
}


