/* CS261 - Assignment 3: Part 2 */
/* Name: Rebecca L. Taylor
 *       tayloreb@oregonstate.edu
 * Date: 30 April 2018
 * Solution description: Circularly Linked List Deque Implementation:
   Includes functions to initialize a circular doubly linked list and add and
   remove items from both front and back (double-ended queue). The final
   function reverses the list by exchanging all links' next and prev pointers. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "circularList.h"

// Double link
struct Link
{
	TYPE value;
	struct Link * next;
	struct Link * prev;
};

struct CircularList
{
	int size;
	struct Link* sentinel;
};

/**
 * Allocates the list's sentinel and sets the size to 0.
 * The sentinel's next and prev should point to the sentinel itself.
 */
static void init(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check that list is not null */
    assert (list != 0);

    /* Allocate the sentinel and check not null */
    list->sentinel = malloc(sizeof(struct Link));
    assert (list->sentinel != 0);

    /* Set sentinel next and values to point to itself */
    list->sentinel->next = list->sentinel;
    list->sentinel->prev = list->sentinel;

    /* Set size to 0 */
    list->size = 0;
}

/**
 * Creates a link with the given value and NULL next and prev pointers.
 */
static struct Link* createLink(TYPE value)
{
	// FIXME: you must write this
    /* Allocate the link and check not null */
    struct Link *newLink = malloc(sizeof(struct Link));
    assert (newLink != 0);

    /* Set next and prev pointers to NULL, assign passed-in value */
    newLink->next = 0;
    newLink->prev = 0;
    newLink->value = value;

	return newLink;
}

/**
 * Adds a new link with the given value after the given link and
 * increments the list's size.
 */

static void addLinkAfter(struct CircularList* list, struct Link* link, TYPE value)
{
	// FIXME: you must write this
    /* Check that list and link not null */
    assert (list != 0);
    assert (link != 0);

    /* Create new link and assign value */
    struct Link *newLink = createLink(value);

    /* Set pointers to position in circular list */
    newLink->next = link->next;             //new link next set to link next
    newLink->prev = link;                   //new link prev set to link

    link->next->prev = newLink;             //link next prev set to new link
    link->next = newLink;                   //link next set to new link

    /* Increment list size */
    list->size++;
};

/**
 * Removes the given link from the list and
 * decrements the list's size.
 */
static void removeLink(struct CircularList* list, struct Link* link)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Proceed only if list not empty */
    if (!circularListIsEmpty(list))
    {
        /* Reassign pointers of adjacent links */
        link->prev->next = link->next;      //link prev next is link next
        link->next->prev = link->prev;      //link next prev is link prev

        /* Free removed link */
        free(link);

        /* Decrement list size */
        list->size--;
    }
}

/**
 * Allocates and initializes a list.
 */
struct CircularList* circularListCreate()
{
	struct CircularList* list = malloc(sizeof(struct CircularList));
	init(list);
	return list;
}

/**
 * Deallocates every link in the list and frees the list pointer.
 */
void circularListDestroy(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Iterate through list to deallocate all links */
    while (!circularListIsEmpty(list))
    {
        circularListRemoveFront(list);
    }
    free(list->sentinel);

    /* Free the list pointer */
    free(list);
}

/**
 * Adds a new link with the given value to the front of the deque.
 */
void circularListAddFront(struct CircularList* list, TYPE value)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Insert new link at front of list (after sentinel) */
    addLinkAfter(list, list->sentinel, value);   //Function increments size
}

/**
 * Adds a new link with the given value to the back of the deque.
 */
void circularListAddBack(struct CircularList* list, TYPE value)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Insert new link at back of list (after sentinel prev) */
    addLinkAfter(list, list->sentinel->prev, value);   //Function increments size
}

/**
 * Returns the value of the link at the front of the deque.
 */
TYPE circularListFront(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

	return list->sentinel->next->value;
}

/**
 * Returns the value of the link at the back of the deque.
 */
TYPE circularListBack(struct CircularList* list)
{
	// FIXME: you must write this

    /* Check list is not null */
    assert (list != 0);

    return list->sentinel->prev->value;
}

/**
 * Removes the link at the front of the deque.
 */
void circularListRemoveFront(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Remove link at front of list (after sentinel) */
    removeLink(list, list->sentinel->next);      //Function decrements size
}

/**
 * Removes the link at the back of the deque.
 */
void circularListRemoveBack(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Remove link at back of list (before sentinel) */
    removeLink(list, list->sentinel->prev);      //Function decrements size
}

/**
 * Returns 1 if the deque is empty and 0 otherwise.
 */
int circularListIsEmpty(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Return 1 if list empty, 0 if has links */
	return list->size == 0;
}

/**
 * Prints the values of the links in the deque from front to back.
 */
void circularListPrint(struct CircularList* list)
{
	// FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Print link starts as first link */
    struct Link *printLink = list->sentinel->next;

    printf("List: \n");

    /* If list is empty */
    if (circularListIsEmpty(list))
    {
        printf("List is empty\n");
    }

    /* Print values if list is not empty */
    else
    {
        /* Iterate until list empty */
        while (printLink != list->sentinel)
        {
            printf("  %f\n", printLink->value);    //Print value of print link
            printLink = printLink->next;           //Move to next link
        }
        printf("\n");
    }
}

/**
 * Reverses the deque.
 */
void circularListReverse(struct CircularList* list)
{
	// FIXME: you must write this
    //Resources: https://www.geeksforgeeks.org/reverse-a-doubly-linked-list/
    //https://www.geeksforgeeks.org/reverse-circular-linked-list/

    /* Check list is not null */
    assert (list != 0);

    /* Temporary pointers to store list placement while reversing */
    struct Link *current = list->sentinel;
    struct Link *temp = 0;

    /* Proceed only if list not empty */
    if (!circularListIsEmpty(list))
    {
        do /* Iterate through the list to reverse all prev and next pointers */
        {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        while (current != list->sentinel);       //Stop when reach sentinel
    }
}
