/* CS261 - Assignment 3: Part 1 */
/* Name: Rebecca L. Taylor
 *       tayloreb@oregonstate.edu
 * Date: 30 April 2018
 * Solution description: Linked List Deque and Bag Implementation:
   Includes functions to initialize a linked list and add and remove items from
   both front and back (double-ended queue). Bag interface functions check to
   see if a value is contained, and add and remove values. */

#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// Double link
struct Link
{
    TYPE value;
    struct Link* next;
    struct Link* prev;
};

// Double linked list with front and back sentinels
struct LinkedList
{
    int size;
    struct Link* frontSentinel;
    struct Link* backSentinel;
};

/**
 * Allocates the list's sentinel and sets the size to 0.
 * The sentinels' next and prev should point to each other or NULL
 * as appropriate.
 */
static void init(struct LinkedList* list)
{
    // FIXME: you must write this
    /* Check that list is not null */
    assert (list != 0);

    /* Allocate front and back sentinels and check not null*/
    list->frontSentinel = malloc(sizeof(struct Link));
    assert (list->frontSentinel != 0);

    list->backSentinel = malloc(sizeof(struct Link));
    assert(list->backSentinel != 0);

    /* Set pointer values of sentinels */
    list->frontSentinel->next = list->backSentinel;        //Front points to back
    list->backSentinel->prev = list->frontSentinel;        //And vice-versa
    list->frontSentinel->prev = list->backSentinel->next = 0;

    /* Set size to 0 */
    list->size = 0;
}

/**
 * Adds a new link with the given value before the given link and
 * increments the list's size.
 */
static void addLinkBefore(struct LinkedList* list, struct Link* link, TYPE value)
{
    // FIXME: you must write this
    /* Allocate new link and check not null*/
    struct Link *newLink = malloc(sizeof(struct Link));
    assert(newLink != 0);

    /* Assign value to new link */
    newLink->value = value;

    /* Assign new link next and previous */
    newLink->next = link;
    newLink->prev = link->prev;

    /* Reassign next and prev of adjacent links */
    link->prev->next = newLink;
    link->prev = newLink;

    /* Increment list size */
    list->size++;

}

/**
 * Removes the given link from the list and
 * decrements the list's size.
 */
static void removeLink(struct LinkedList* list, struct Link* link)
{
    // FIXME: you must write this
    /* Check list is not null */
    assert(list != 0);

    /* Proceed only if list is not empty */
    if(!linkedListIsEmpty(list))
    {
        /* Reassign pointers of adjacent links */
        link->prev->next = link->next;
        link->next->prev = link->prev;

        /* Free removed link */
        free(link);

        /* Decrement list size */
        list->size--;
    }
}

/**
 * Allocates and initializes a list.
 */
struct LinkedList* linkedListCreate()
{
    struct LinkedList* newDeque = malloc(sizeof(struct LinkedList));
    init(newDeque);
    return newDeque;
}

/**
 * Deallocates every link in the list including the sentinels,
 * and frees the list itself.
 */
void linkedListDestroy(struct LinkedList* list)
{
    while (!linkedListIsEmpty(list))
    {
        linkedListRemoveFront(list);
    }
    free(list->frontSentinel);
    free(list->backSentinel);
    free(list);
}

/**
 * Adds a new link with the given value to the front of the deque.
 */
void linkedListAddFront(struct LinkedList* list, TYPE value)
{
    // FIXME: you must write this
    /* Check list is not null */
    assert (list != 0);

    /* Add link with passed-in value */
    addLinkBefore(list, list->frontSentinel->next, value);
}

/**
 * Adds a new link with the given value to the back of the deque.
 */
void linkedListAddBack(struct LinkedList* list, TYPE value)
{
    // FIXME: you must write this
    /* Check that list is not null */
    assert((list != 0));

    /* Add link with passed-in value */
    addLinkBefore(list, list->backSentinel, value);
}

/**
 * Returns the value of the link at the front of the deque.
 */
TYPE linkedListFront(struct LinkedList* list)
{
    // FIXME: you must write this
    /* Check that list is not null */
    assert((list != 0));

    return list->frontSentinel->next->value;
}

/**
 * Returns the value of the link at the back of the deque.
 */
TYPE linkedListBack(struct LinkedList* list)
{
    // FIXME: you must write this
    /* Check that list is not null */
    assert((list != 0));

    return list->backSentinel->prev->value;
}

/**
 * Removes the link at the front of the deque.
 */

void linkedListRemoveFront(struct LinkedList* list)
{
    // FIXME: you must write this
    /* Check that list is not null */
    assert(list != 0);

    /* Proceed only if list is not empty */
    if (!linkedListIsEmpty(list))
    {
        removeLink(list, list->frontSentinel->next);
    }
}

/**
 * Removes the link at the back of the deque.
 */

void linkedListRemoveBack(struct LinkedList* list)
{
    // FIXME: you must write this
    /* Check that list is not null */
    assert((list != 0));

    /* Proceed only if list is not empty */
    if (!linkedListIsEmpty(list))
    {
        removeLink(list, list->backSentinel->prev);
    }
}

/**
 * Returns 1 if the deque is empty and 0 otherwise.
 */

int linkedListIsEmpty(struct LinkedList* list)
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
void linkedListPrint(struct LinkedList* list)
{
    // FIXME: you must write this
    /* Check that list not null */
    assert(list != 0);

    /* Print link starts as first link */
    struct Link *printLink = list->frontSentinel->next;

    printf("List: \n");

    /* If list is empty */
    if (list->size == 0)
    {
        printf("List is empty\n");
    }

    /* Print values if list not empty */
    else
    {
        /* Iterate until list empty */
        while (printLink != list->backSentinel)
        {
            printf("  %i\n", printLink->value);    //Print value of print link
            printLink = printLink->next;           //Move to next link
        }
        printf("\n");
    }
}

/**
 * Adds a link with the given value to the bag.
 */
void linkedListAdd(struct LinkedList* list, TYPE value)
{
    // FIXME: you must write this
    /* Check that list not null */
    assert(list != 0);

    /* Add item to front of list (since order doesn't matter */
    linkedListAddFront(list, value);
}

/**
 * Returns 1 if a link with the value is in the bag and 0 otherwise.
 */
int linkedListContains(struct LinkedList* list, TYPE value)
{
    // FIXME: you must write this
    /* Check that list not null */
    assert(list != 0);

    int found = 0;                     //Set found to false (0)

    /* Temp link to iterate through list, starting at front */
    struct Link *check = list->frontSentinel->next;

    /* If list not empty, iterate through list */
    if (!linkedListIsEmpty(list))
    {
        /* While search has not reached back sentinel */
        while (check != list->backSentinel)
        {
            if (check->value == value)
            {
                found = 1;             //If value match, found set to true (1)
            }
            check = check->next;       //Move to next item in list
        }
    }

    return found;
}


/**
 * Removes the first occurrence of a link with the given value.
 */
void linkedListRemove(struct LinkedList* list, TYPE value)
{
    // FIXME: you must write this
    /* Check that list not null */
    assert(list != 0);

    /* Temp link to iterate through list, starting at front */
    struct Link *check = list->frontSentinel->next;

    /* Proceed only if list not empty */
    if (!linkedListIsEmpty(list))
    {
        /* While search has not reached back sentinel */
        while (check != list->backSentinel)
        {
            if (check->value == value)
            {
                removeLink(list, check);    //Remove if found
                break;                      //Only remove first instance
            }
            check = check->next;            //Move to next item in list
        }
    }
}
