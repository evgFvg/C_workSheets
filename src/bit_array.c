/*************************************************
 * developer: Evgenii F.
 * reviewer: Mor Z.
 * date: 31.01.2023
 *
 **************************************************/
#include <assert.h>
#include "bit_array.h"

#define BITS_IN_SIZE_T (sizeof(bit_arr_t) * (__CHAR_BIT__))

bit_arr_t BitArraySetOn(bit_arr_t bitarr, size_t index)
{
    assert(index < BITS_IN_SIZE_T);
    return bitarr | ((size_t)1 << index);
}

bit_arr_t BitArraySetOff(bit_arr_t bitarr, size_t index)
{
    assert(index < BITS_IN_SIZE_T);
    return bitarr & (~((size_t)1 << index));
}

bit_arr_t BitArraySetVal(bit_arr_t bitarr, size_t index, int bool_value)
{
    assert(index < BITS_IN_SIZE_T);
    return bool_value ? BitArraySetOn(bitarr, index) : BitArraySetOff(bitarr, index);
}

bit_arr_t BitArraySetAll(bit_arr_t bitarr)
{
    size_t mask = 0;
    return bitarr | (~mask);
}

bit_arr_t BitArrayResetAll(bit_arr_t bitarr)
{
    return bitarr & (size_t)0;
}

bit_arr_t BitArrayFlip(bit_arr_t bitarr, size_t index)
{
    assert(index < BITS_IN_SIZE_T);
    return bitarr ^ ((size_t)1 << index);
}

bit_arr_t BitArrayMirror(bit_arr_t bitarr)
{
    bitarr = (((bitarr & 0xaaaaaaaa) >> 1) | ((bitarr & 0x55555555) << 1));
    bitarr = (((bitarr & 0xcccccccc) >> 2) | ((bitarr & 0x33333333) << 2));
    bitarr = (((bitarr & 0xf0f0f0f0) >> 4) | ((bitarr & 0x0f0f0f0f) << 4));
    bitarr = (((bitarr & 0xff00ff00) >> 8) | ((bitarr & 0x00ff00ff) << 8));
    bitarr = ((bitarr >> 16) | (bitarr << 16));
    bitarr = ((bitarr >> 32) | (bitarr << 32));

    return bitarr;
}

size_t BitArrayCountOn(bit_arr_t bitarr)
{
    size_t count = 0;
    while (0 != bitarr)
    {
        bitarr &= (bitarr - (bit_arr_t)1);
        ++count;
    }
    return count;
}
size_t BitArrayCountOff(bit_arr_t bitarr)
{
    return BITS_IN_SIZE_T - BitArrayCountOn(bitarr);
}
int BitArrayGet(bit_arr_t bitarr, size_t index)
{
    return (((size_t)1 << index) & bitarr) > 0 ? 1 : 0;
}
char *BitArrayToString(bit_arr_t bitarr, char *dest)
{
    size_t i = 0;
    for (i = 0; i < BITS_IN_SIZE_T; ++i)
    {
        dest[BITS_IN_SIZE_T - i - 1] = (BitArrayGet(bitarr, i) == 1 ? '1' : '0');
    }
    dest[BITS_IN_SIZE_T] = '\0';
    return dest;
}

bit_arr_t BitArrayRotateRight(bit_arr_t bitarr, size_t step)
{
    step %= BITS_IN_SIZE_T; /*if step > 64*/
    return (bitarr >> step) | (bitarr << (BITS_IN_SIZE_T - step));
}

bit_arr_t BitArrayRotateLeft(bit_arr_t bitarr, size_t step)
{
    step %= BITS_IN_SIZE_T; /*if step > 64*/
    return (bitarr << step) | (bitarr >> (BITS_IN_SIZE_T - step));
}
