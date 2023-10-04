#include <stdlib.h> /*strtol*/
#include <stdio.h> /*printf*/
#include "bit_array.h"

static void TestBinaryArr();

int main()
{
    TestBinaryArr();
    return 0;
}

static void TestBinaryArr()
{
    size_t res = 0;
    bit_arr_t test = 0;
    size_t i = 0;
    char buffer[128] = {0};
    for (i = 0; i < 4; ++i)
    {
        test =  BitArraySetOn(test, i); /*1111*/
    }
    assert(15 == test);
    test =  BitArraySetOff(test, 3);/*0111*/
    assert(7 == test);

    test =  BitArraySetVal(test, 2, 0); /*0011*/
    assert(3 == test);

    test = BitArraySetAll(test);
    assert(test == 0xFFFFFFFFFFFFFFFF);
    res = BitArrayCountOn(test);
    assert(res == 64);

    test  = BitArrayResetAll(test);
    assert(0 == test);
    res = BitArrayCountOff(test);
    assert(res == 64);

    test  = BitArrayFlip(test, 1);
    assert(2 == test);
    test = BitArrayMirror(test); /*lsb 1 to msb 1*/
    assert(0x4000000000000000 == test); 

    res =  BitArrayGet(test, 62);
    assert(1 == res);

    test = 15; /*1111*/
    test = BitArrayRotateLeft(test, 4); /*1111 0000*/
    assert(test == 0xF0);
    test = BitArrayRotateRight(test, 4); /*0000 1111*/
    assert(test == 0xF);

    assert(strtol(BitArrayToString(test, buffer), NULL, 2) == 15);

    printf("All binary_array tests have been passed successfully\n");
    
}
