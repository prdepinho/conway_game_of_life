
#include <stdlib.h>
#include "stack.h"

Stack* Stack_Create()
{
	Stack *stack = List_Create();
	return stack;
}

int Stack_Delete(Stack *stack)
{
	if(stack == NULL)
		return -1;

	List_Delete(stack);
	stack = NULL;
	return 0;
}

void Stack_Push(Stack *stack, void *value)
{
	struct Element *element = Element_Create(value);
	List_Add(stack, element);
}

void* Stack_Pop(Stack *stack)
{
	void *value = Element_Get(List_Get(stack));
	List_Remove(stack);
	List_MoveCursor(stack, -1, NULL);
	return value;
}

int Stack_Size(Stack *stack)
{
	return stack->size;
}

