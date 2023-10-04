#ifndef __C_BUFFER__
#define __C_BUFFER__ 

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*ssize_t*/

typedef struct c_buffer c_buffer_t;

/*
* CBufferCreate Description:
* 	Creates the circular buffer.
*
* @Params:
* 	capacity  - amount of bytes in the the created buffer
*
* @Returns:
* 	pointer to the created circular buffer.
*
* @Complexity
* 	Time: O(1)
*/
c_buffer_t *CBufferCreate(size_t capacity);	

/*
* CBufferDestroy Description:
* 	Destroys the circular buffer.
*
* @Params:
* 	buffer - Circular buffer to be destroyed
*
* @Returns:
* 	None.
*
* @Complexity
* 	Time: O(1)
*/
void CBufferDestroy(c_buffer_t *buffer);	

/*
* CBufferWrite Description:
* 	Writes data to buffer.
*
* @Params:
* 	buffer - Circular buffer to write to.
* 	src - source buffer provided by user, to take data from.
* 	data_size - number of bytes to copy from source to buffer.
*
* @Returns:
* 	Number of bytes successfully written to the buffer or -1 if buffer is  Full.
* 	If there is no enough space for data_size in the buffer, as many bytes as 
* 	possible will be copied.
*
* @Complexity
* 	Time: O(n)
*/	
ssize_t CBufferWrite(c_buffer_t *buffer,const void *src, size_t data_size); 

/*
* CBufferRead Description:
* 	Reads data from buffer.
*
* @Params:
* 	buffer - buffer to read from.
* 	src - pointer to buffer, provided by user to write to.
* 	size - the number of bytes to read from buffer.
*
* @Returns:
* 	Number of bytes successfully read, and written to src, or -1 if buffer is Empty.
* 	If there is no enough bytes to read from buffer, as many bytes as 
* 	possible will be read.
*
* @Complexity
* 	Time: O(n)
*/
ssize_t CBufferRead(c_buffer_t *buffer,void *dest, size_t size);			

/*
* CBufferSize Description:
* 	Counts the number of written bytes in buffer.
*
* @Params:
* 	buffer  - buffer to count in.
*
* @Returns:
* 	the number of counted bytes.
*
* @Complexity
* 	Time: O(1)
*/
size_t CBufferSize(const c_buffer_t *buffer); 	
							
/*
* CBufferRead Description:
* 	Counts the number of free bytes in buffer.
*
* @Params:
* 	buffer - buffer to count in.
*
* @Returns:
* 	the number of counted bytes.
*
* @Complexity
* 	Time: O(1)
*/
size_t CBufferFreeSpace(const c_buffer_t *buffer);		
					
/*
* CBufferIsEmpty Description:
* 	Checks if the buffer is empty.
*
* @Params:
* 	buffer - buffer to check the emptiness.
*
* @Returns:
* 	1 - if empty.
	0 - if not.
*
* @Complexity
* 	Time: O(1)
*/
int CBufferIsEmpty(const c_buffer_t *buffer); 							


#endif

