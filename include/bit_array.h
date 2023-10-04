#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include<stddef.h> /*size_t*/
#include <assert.h>

typedef size_t bit_arr_t;


/* 
* SetOn Description:
* Set bit value in a specific index to 1
*
* @param:
*	bitarr - the bit arr to set the bit in.	 
*	index - the index to set the bit in, should be 0-63.
*	sending an index outside this range could cause undefiend behaviour.
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(1)
*/
bit_arr_t BitArraySetOn(bit_arr_t bitarr, size_t index);




/* 
* SetOff Description
*	Set bit value in a specific index to 0
*
* @param:
*	bit_arr - the bit arr to set the bit in.	 
*	index - the index to set the bit in, should be 0-63.
*	sending an index outside this range could cause undefiend behaviour.
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(1)
*/
bit_arr_t BitArraySetOff(bit_arr_t bitarr, size_t index);



/* 
* SetVal Description
*	Set bit value in a specific index to either 1 or 0
*
* @param:
*	bit_arr - the bit arr to set the bit in.
*	value - the value to change the bit, should be either 0 or any number
*	index - the index to set the bit in, should be 0-63.
*	sending an index outside this range could cause undefiend behaviour.
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(1)
*/
bit_arr_t BitArraySetVal(bit_arr_t bitarr, size_t index, int bool_value);


/* 
* SetAll Description
*	Set all bits in arr to 1
*
* @param:
*	bit_arr - the bit arr to set the bits in.
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(n) - 64
*/
bit_arr_t BitArraySetAll(bit_arr_t bitarr);




/* 
* ResetAll Description
*	Set all bits in the arr to 0
*
* @param:
*	bit_arr - the bit arr to reset the bits in.	 
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(n) - 64
*/
bit_arr_t BitArrayResetAll(bit_arr_t bitarr);




/* 
* Flip Description
*	Flips the bit value in a specific index
*
* @param:
*	bit_arr - the bit arr to flip the bit in.	 
*	index - the index to flip the bit in, should be 0-63.
*	sending an index outside this range could cause undefiend behaviour.
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(1)
*/
bit_arr_t BitArrayFlip(bit_arr_t bitarr, size_t index);




/* 
* Mirror Description
*	Mirrors the bit arrr (0100 -> 0010)
*
* @param:
*	bit_arr - the bit arr to mirror. 
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(n) - 64
*/
bit_arr_t BitArrayMirror(bit_arr_t bitarr);




/* 
* Description
*	Rotates the bit array left 'step' times 
*
* @param:
*	bit_arr - the bit arr to rotate the bits.
*	step - the number of rotations to do
*	sending an index outside this range could cause undefiend behaviour.	 
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(n) - rep
*/
bit_arr_t BitArrayRotateLeft(bit_arr_t bitarr, size_t step);




/* 
* Description
*	Rotates the bit array right 'step' times 
*
* @param:
*	bit_arr - the bit arr to rotate the bits.	 
*	step - the number of rotations to do
*	sending an index outside this range could cause undefiend behaviour.
*
* @Return
*	returns the updated bit array
*
* @Complexity
*	Time: O(n) - rep
*/
bit_arr_t BitArrayRotateRight(bit_arr_t bitarr, size_t step);




/* 
* CountOn Description
*	Counts the number of set bits  
*
* @param:
*	bit_arr - the bit arr to count the bits in.	 
*
* @Return
*	returns the number of set bits
*
* @Complexity
*	Time: O(n) - 64
*/
size_t BitArrayCountOn(bit_arr_t bitarr);




/* 
* CountOff Description
*	Counts the number of off bits 
*
* @param:
*	bit_arr - the bit arr to count the bits in.	 
*
* @Return
*	returns the number of off bits 
*
* @Complexity
*	Time: O(n) - 64
*/
size_t BitArrayCountOff(bit_arr_t bitarr);




/* 
* GetVal Description
*	Get the bit's value in a specific index 
*
* @param:
*	bit_arr - the bit arr to get the value from.	 
*	index - the specific index to get the bit value from, should be 0-63.
*	sending an index outside this range could cause undefiend behaviour.
*
* @Return
*	returns the bit value on a specific index
*
* @Complexity
*	Time: O(1)
*/
int BitArrayGet(bit_arr_t bitarr, size_t index);




/* 
* ToString Description
*	Returns the string representation of the bit_array
*
* @param:
*	bit_arr - the bit arr to convert to string.	 
*
* @Return
*	returns string representation of the array
*
* @Complexity
*	Time: O(n) - 64
*/
char *BitArrayToString(bit_arr_t bitarr, char *dest);


#endif
