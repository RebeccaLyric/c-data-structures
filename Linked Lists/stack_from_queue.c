/* CS261 - Assignment 3: Part 3 */
/* Name: Rebecca L. Taylor
 *       tayloreb@oregonstate.edu
 * Date: 30 April 2018
 * Solution description: Linked List Stack Implementation Using Two Queues:
   Includes functions to create and initialize a singly linked queue with
   add back and remove front operations. The second part implements a stack
   using two queues, including stack creation and push, pop, and top operations. */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef TYPE
#define TYPE int
#endif

/**************************************************
 All of the initial Queue functions
 ***************************************************/
struct link {
	TYPE value;
	struct link * next;
};

struct listQueue {
	struct link *firstLink;
	struct link *lastLink;
	int size;
};

/*
* This function takes a queue and allocates the memory. It then
* creates a sentinel and assigns the first link and last link
* to the sentinel.
*/
void listQueueInit(struct listQueue *q)
{
    // FIXME: you must write this
    /* Check queue not null */
    assert (q != 0);

    /* Allocate sentinel and check not null */
    struct link *sentinel = (struct link *) malloc(sizeof(struct link));
    assert (sentinel != 0);

    /* Sentinel's next is null */
    sentinel->next = 0;

    /* Assign sentinel to queue first link and last link */
    /* Discussed with students on Piazza post @245 */
    q->firstLink = q->lastLink = sentinel;

    /* Initialize queue size to zero */
    q->size = 0;
}

/*
 * This function creates a new queue. Parts of the create include
 * allocating the memory, initializing all of the values and returning
 * a pointer to the newly created queue.
 */
struct listQueue * listQueueCreate()
{
    //FIXME: you must write this
    /* Allocate new queue and check not null */
    struct listQueue *newListQueue = (struct listQueue*)malloc(sizeof(struct listQueue));
    assert (newListQueue != 0);

    /* Initialize the list queue */
    listQueueInit(newListQueue);

    /* Return the new list queue */
    return newListQueue;
}

/*
* This function returns a 1 or 0 based on the statement looking at
* the first link. If the next value is null it is empty, and will return 1
*/
int listQueueIsEmpty(struct listQueue *q)
{
    //FIXME: you must write this
    /* Check list is not null */
    assert (q != 0);

    /* Return 1 if list empty, 0 if has links */
    return q->firstLink==q->lastLink;
}

/*
 * This function adds a new link and value to the back of the queue. It
 * takes a list and a type variable, allocates the memory and adjusts the
 * proper links to make the connection. No return value.
*/
void listQueueAddBack(struct listQueue *q, TYPE e)
{
    // FIXME: you must write this
    //Resources: https://www.geeksforgeeks.org/queue-set-2-linked-list-implementation/
    /* Check list is not null */
    assert (q != 0);

    /* Allocate new link and check not null */
    struct link *newLink = malloc(sizeof(struct link));
    assert (newLink != 0);

    /* Set values of new link */
    newLink->next = 0;             //Next points to end of list (null)
    newLink->value = e;

    /* Reassign links to add new last to back of queue */
    q->lastLink->next = newLink;
    q->lastLink = newLink;

    /* Increment list size */
    q->size++;
}

/*
 * This function takes a list argument and removes the link at the front.
 */
void listQueueRemoveFront(struct listQueue *q)
{
    // FIXME: you must write this
    /* Check list is not null */
    assert (q != 0);

    /* Link to remove */
    struct link *deleted = q->firstLink;

    /* Proceed only if list not empty */
    if (!listQueueIsEmpty(q))
    {
        /* First link points to following link */
        q->firstLink = deleted->next;

        /* Free deleted link */
        free(deleted);

        /* Decrement queue size */
        q->size--;
    }
}

/*
 * Function returns the value at the front of the list.
 */

TYPE listQueueFront(struct listQueue *q)
{
    // FIXME: you must write this
    /* Check list is not null and not empty*/
    assert (q != 0);
    assert (!listQueueIsEmpty(q));

    return q->firstLink->next->value;
}

/*
* This function is a tester function that iterates through the list
* and prints out the values at each link.
*/
void printList(struct listQueue* l)
{
	assert(l != 0);
	
	struct link * printMe = l->firstLink->next;

	/* If/else statement added */
	if (listQueueIsEmpty(l))
    {
        printf("List is empty\n");
    }

	else
    {
        while (printMe!= NULL)
        {
            printf("Value: %d\n", printMe->value);
            printMe = printMe->next;
        }
    }
}

/**************************************************
Stack Implementation
***************************************************/

struct linkedListStack {
	struct listQueue *Q1;
	struct listQueue *Q2;
	int structSize;
};

/*
 * This function initializes the values of the created Stack. Initializes
 * both queues and the structSize.
 */
void linkedListStackInit(struct linkedListStack * s)
{
	// FIXME: you must write this
    /* Check stack not null */
    assert (s != 0);

    /* Initialize both queues and set struct size */
    s->Q1 = listQueueCreate();
    s->Q2 = listQueueCreate();
    s->structSize = 0;
 }

/*
 * This function creates the linked list stack. It allocates the memory and
 * calls the initialization function to initialize all of the values.
 * It then returns the stack.
*/
struct linkedListStack * linkedListStackCreate()
{
    // FIXME: you must write this
    /* Allocate new new stack and check not null */
    struct linkedListStack *newStack = malloc(sizeof(struct linkedListStack));
    assert (newStack != 0);

    /* Initialize the new stack */
    linkedListStackInit(newStack);

	return newStack;
}

/*
* This function returns 1 if the linked list stack is empty and otherwise 
returns a 0.
*/
int linkedListStackIsEmpty(struct linkedListStack *s)
{
	// FIXME: you must write this
    /* Check stack not null */
    assert (s != 0);

    return s->structSize == 0;
}

/*
 * This is the linked list acting as a stack push function. It takes 
 * a linked list stack argument and a value and pushes it onto the stack.
 * The function then also increases the size of the stack by 1.
 */
void linkedListStackPush(struct linkedListStack *s, TYPE d)
{
    // FIXME: you must write this
    //Resources: https://leetcode.com/articles/implement-stack-using-queues/

    /* Check stack not null */
    assert (s != 0);

    /* Push value to Q1 */
    listQueueAddBack(s->Q1, d);

    /* Increment stack size */
    s->structSize++;
}

/*
 * This function pops a value off of the stack. It does this by moving all
 * values that are currently on the stack to the other queue. The stack top is
 * maintained at the back of the queue list.
 */
void linkedListStackPop(struct linkedListStack *s)
{
    // FIXME: you must write this
    //Resources: https://leetcode.com/articles/implement-stack-using-queues/

    /* Check stack not null */
    assert (s != 0);

    TYPE temp;                //Store value as being moved between queues

    /* Proceed only if stack is not empty */
    if (!linkedListStackIsEmpty(s))
    {
        /* Reverse Q1 elements by moving all but last to Q2 */
        while (s->Q1->size > 1)
        {
            temp = listQueueFront(s->Q1);

            /* Remove element from front of Q1 and add to back of Q2 */
            listQueueRemoveFront(s->Q1);
            listQueueAddBack(s->Q2, temp);
        }

        /* Remove last element from Q1 */
        listQueueRemoveFront(s->Q1);        //Last in queue is top of stack

        /* Exchange queue names so Q1 holds values of stack */
        struct listQueue *tempQueue = s->Q1;
        s->Q1 = s->Q2;
        s->Q2 = tempQueue;

        /* Decrement stack size */
        s->structSize--;
    }

    else if (linkedListStackIsEmpty(s))
    {
        printf("\nError: Stack is empty\n");
    }
}

/*
 * This function returns the value that is at the back of the queue that
 * is maintaining the values of the stack.
 */
TYPE linkedListStackTop(struct linkedListStack *s)
{
    // FIXME: you must write this
    /* Check stack not null */
    assert (s != 0);

    return s->Q1->lastLink->value;     //Back of queue is top of stack
}

/*
 * This function goes through the stack and removes each link in the
 * queue. It then frees the struct itself.
 */
void linkedListStackFree(struct linkedListStack *s){
	
    assert(s != 0);
	
	while (s->structSize != 0) 
	{
		linkedListStackPop(s);
	}

	free(s->Q1->firstLink);
	free(s->Q2->firstLink);
	free(s->Q1);
	free(s->Q2);

	free(s);
}

/*
 * Main is used to test the stack ADT.
 */
int main(int argc, char* argv[])
{
    struct linkedListStack *stack = linkedListStackCreate();

    //Test Stack
 	//Push 4 values onto the stack
	printf("Pushing the value: 1\n");
	linkedListStackPush(stack, 1);
	printf("Pushed.\n\n");

	printf("Pushing the value: 2\n");
	linkedListStackPush(stack, 2);
	printf("Pushed.\n\n");

	printf("Pushing the value: 3\n");
	linkedListStackPush(stack, 3);
	printf("Pushed.\n\n");

	printf("Pushing the value: 4\n");
	linkedListStackPush(stack, 4);
	printf("Pushed.\n\n");

	//Print value at the top and then remove it
	printf("Value at the top of stack (%d) now being popped. \n",linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	printf("Value at the top of stack: %d now being popped. \n", linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	printf("Value at the top of stack: %d now being popped. \n", linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	printf("Value at the top of stack: %d now being popped. \n", linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	//Try to pop when the stack is empty prints error:
	printf("Trying to pop empty stack:\n");

	linkedListStackPop(stack);


	//Push and Pop alternating
	printf("Pushing the value: 10\n");
	linkedListStackPush(stack, 10);
	printf("Pushed.\n\n");

	printf("Pushing the value: 11\n");
	linkedListStackPush(stack, 11);
	printf("Pushed.\n\n");

	printf("One more pop:\n");
	linkedListStackPop(stack);
	printf("Value at the top of stack: %d\n",
    linkedListStackTop(stack));

	linkedListStackFree(stack);

	return 0;

}


