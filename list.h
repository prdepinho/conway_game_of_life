/*
 * This is a simple API to work with dynamic doubly-linked circular lists.
 *
 * @author Paulo Rogerio de Pinho Filho.
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

/*
 * Element structure.
 * This is the basic object to encapsulate the values and organize
 * them in the list.
 * It has a pointer containing the value itself, a pointer
 * to the element that comes before it and a pointer to the element
 * that comes after it in the list.
 */
struct Element{
	void *value;
	struct Element *before;
	struct Element *after;
};

/*
 * List structure.
 * The dynamic linked list. It has a pointer to the first element that
 * entered the list and a cursor, on which every operation on the 
 * elements of the list are based.
 */
struct List{
	int size;
	struct Element *first;
	struct Element *cursor;
};

typedef struct List List;
typedef struct Element Element;

/*
 * Creates an element encapsulating the object withing it.
 */
Element* Element_Create(void *object);

/*
 * Returns the pointer to the value of the element.
 * Since it is a void pointer, it must be cast to the original value
 * the it was pointing to in order to be used.
 */
void* Element_Get(Element *element);

/*
 * Creates a list, no elements inside, ready to be used.
 * Lists created this way must be freed only with the List_Delete
 * function.
 * It must be tested though, if memory could not be allocated, it 
 * returns NULL instead. 
 */
List* List_Create();

/*
 * Deletes a list. It must be noted that all the elements in the list will
 * be deleted, but the values they were holding prevail, so, there
 * must still be pointers to them outside the list to prevent memory leak.
 * Returns -1 if the list is NULL.
 */
int List_Delete(List *list);

/*
 * Moves the cursor of the list. It is moved 'offset' positions from
 * the 'origin' element. 
 * The 'origin' element must be in the list, if it exists in another list, 
 * it will result in the cursor to point to an element of that foreign list; 
 * if the element is in no list at all, the result is unknown.
 * If the 'origin' is NULL, it will move the cursor starting
 * from it's own position, though. 
 * If the list has one or less elements, there is no room to the cursor
 * to move, it returns -1 instead.
 */
int List_MoveCursor(List *list, int offset, Element *origin);

/*
 * Adds an element to the list, it will be added just after the cursor, 
 * that will move to it, afterwards.
 */
void List_Add(List *list, Element *element);

/*
 * Removes the element that is pointed by the cursor.
 * If the list's first element is removed this way, the next
 * element becomes the first.
 * Returns -1 if the list is empty.
 */
int List_Remove(List *list);

/*
 * returns the number of elements in the list.
 */
int List_Size(List *list);

/*
 * returns the pointer to the Element on which cursor is.
 */
Element* List_Get(List *list);

/*
 * Inserts an element to the list in order.
 * The comparison that will be made between the elements is
 * specified by the function pointer.
 * The return value of the comparison function must return 0 if
 * the elements are equal, negative if elm_1 < elm_2 and
 * positive if elm_1 > elm_2.
 */
void List_Insert(List *list,
		void *value,
		int (*compare)(void *elm_1, void *elm_2));

/*
 * Returns the value of the element pointed by the cursor
 */

void* List_GetElement(List *list);

#endif
