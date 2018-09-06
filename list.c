

#include "list.h"


Element* Element_Create(void *object)
{
	struct Element *element = malloc(sizeof(struct Element));
	element->value = object;
	return element;
}

void* Element_Get(Element *element)
{
	return element->value;
}

List* List_Create()
{
	struct List *list = malloc(sizeof(struct List));
	list->first = NULL;
	list->cursor = NULL;
	return list;
}

int List_Delete(List *list)
{
	struct Element *aux = NULL;
	int a;

	if(list == NULL)
		return -1;

	list->cursor = list->first;
	for(a = 0; a < list->size; a++){
		aux = list->cursor;
		list->cursor = list->cursor->after;
		free(aux);
	}	
	free(list);
	list = NULL;
	return 0;
}

int List_MoveCursor(List *list, int offset, Element *origin)
{
	if(list->size < 2)
		return -1;

	if(origin == NULL)
		origin = list->cursor;

	list->cursor = origin;
	if(offset > 0){
		do{
			list->cursor = list->cursor->after;
		}while(--offset);
	}else if(offset < 0){
		do{
			list->cursor = list->cursor->before;
		}while(++offset);
	}
	return 0;
}

void List_Add(List *list, Element *element)
{
	if(list->size == 0){
		list->first = element;
		element->before = element;
		element->after = element;
	}else{
		list->cursor->after->before = element;
		element->before = list->cursor;
		element->after = list->cursor->after;
		list->cursor->after = element;
	}
	list->cursor = element;
	list->size++;
}

int List_Remove(List *list)
{
	struct Element *aux = NULL;

	switch(list->size){
	case 0:
		return -1;
	case 1:
		free(list->cursor);
		break;
	default:
		if(list->first == list->cursor)
			list->first = list->cursor->after;
		list->cursor->after->before = list->cursor->before;
		list->cursor->before->after = list->cursor->after;
		aux = list->cursor->after;
		free(list->cursor);
		break;
	}
	list->cursor = aux;
	list->size--;
	return 0;
}

int List_Size(List *list)
{
	return list->size;
}

Element* List_Get(List *list)
{
	return list->cursor;
}

void List_Insert(List *list,
		void *value,
		int (*compare)(void *elm_1, void *elm_2))
{
	int a;
	struct Element *element = Element_Create(value);

	if(list->size <= 0){
		List_Add(list, element);
		return;
	}
	list->cursor = list->first->before;
	for(a = 0; a < list->size; a++){
		if((*compare)(value, list->cursor->value) > 0){
			List_Add(list, element);
			return;
		}else
			list->cursor = list->cursor->before;
	}
	/*if the element is the smallest, it becomes the
	 * beginning of the list*/
	List_Add(list, element);
	list->first = list->cursor;
}

void* List_GetElement(List *list)
{
	return list->cursor->value;
}

