/*
 * A simple API for working with stacks, based my list API.
 * Unlike the List, There's no need to manually encapsulate the 
 * pointer of the values to an Element structure, the functions that 
 * deal with the stack do it automatically.
 *
 * @author Paulo Rogerio de Pinho Filho
 */

#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include "list.h"

/*
 * The stack is a list, in a basic form, only the operations used
 * with it are different.
 */
typedef struct List Stack;


/*
 * This creates a stack. May return NULL if no memory is accessible.
 */
Stack* Stack_Create();

/*
 * Deletes the stack and its elements, pointers to the values prevail.
 */
int Stack_Delete(Stack *stack);

/*
 * Puts a new element to the stack.
 */
void Stack_Push(Stack *stack, void *value);

/*
 * Removes the top element from the stack. The returned pointer must be 
 * type-cast to be used.
 */
void* Stack_Pop(Stack *stack);

/*
 * Returns the number of elements of the stack
 */
int Stack_Size(Stack *stack);

#endif

