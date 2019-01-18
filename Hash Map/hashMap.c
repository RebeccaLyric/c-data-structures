/* CS261 - Assignment 5: Part 1 */
/* Name: Rebecca L. Taylor
 *       tayloreb@oregonstate.edu
 * Date: 4 June 2018
 * Solution description: Hash Map
 * Includes functions to add, remove, and search for values in the hash map
 * Including auxiliary functions such as initializing, table resizing, and cleanup */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    // FIXME: implement
    assert (map != 0);

    /* First delete all links in map table */
    for (int i = 0; i < map->capacity; i++)           //Check all buckets
    {
        struct HashLink *garbage = map->table[i];
        struct HashLink *next;

        while (garbage != 0)
        {
            next = garbage->next;
            hashLinkDelete(garbage);
            garbage = next;
        }
    }

    /* Then free map memory and reset variables to 0 */
    free(map->table);
    map->size = 0;
    map->capacity = 0;
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    // FIXME: implement
    assert (map != 0);

    /* Get index and ensure not negative */
    int index = HASH_FUNCTION(key) % map->capacity;
    if (index < 0)
    {
        index += map->capacity;
    }

    /* Search linked list at bucket */
    struct HashLink *current = map->table[index];

    while (current != 0)
    {
        if (strcmp(key, current->key) == 0)
        {
            return &current->value;
        }
        current = current->next;
    }

    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity. After allocating the new table, all of the links need to be
 * rehashed into it because the capacity has changed.
 *
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 *
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    // FIXME: implement
    assert (map != 0);

    /* Create new hash map */
    struct HashMap *newMap = hashMapNew(capacity);    //2x capacity passed in
    assert (newMap != 0);

    /* Copy table data */
    for (int i = 0; i < hashMapCapacity(map); i++)    //Check all table places
    {
        struct HashLink *current = map->table[i];
        while (current != 0)                          //If link at current
        {
            /* Copy all links (rehash in function) and move to next link */
            hashMapPut(newMap, current->key, current->value);
            current = current->next;
            //free(current);
        }
    }

    /* Free memory */
    hashMapCleanUp(map);                    //Frees links and table (not map)
    map->table = newMap->table;
    map->capacity = newMap->capacity;
    map->size = newMap->size;

    free(newMap);                           //Free memory allocated for new map
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXME: implement
    assert (map != 0);

    /* Get hash index and ensure positive value */
    int index = HASH_FUNCTION(key) % map->capacity;
    if (index < 0)
    {
        index += map->capacity;
    }

    /* Check if key already exists, remove existing */
    if (hashMapContainsKey(map, key))
    {
        hashMapRemove(map, key);
    }

    /* Create new link (function assigns values), add to map, increment size */
    struct HashLink *newHL = hashLinkNew(key, value, map->table[index]);
    assert (newHL != 0);

    map->table[index] = newHL;
    map->size++;

    /* Resize hash table if necessary */
    if (hashMapTableLoad(map) >= MAX_TABLE_LOAD)
    {
        resizeTable(map, 2 * hashMapCapacity(map));
    }
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement
    assert (map != 0);

    /* Get hash index and ensure positive value*/
    int index = HASH_FUNCTION(key) % map->capacity;
    if (index < 0)
    {
        index += map->capacity;
    }

    /* Set current to index and begin search of linked list */
    struct HashLink *current = map->table[index];
    struct HashLink *prev = map->table[index];

    while (current != 0)
    {
        if (strcmp(key, current->key) == 0)
        {
            struct HashLink *garbage = current;

            /* If link to remove is first in linked list at table */
            if (garbage == map->table[index])
            {
                map->table[index] = garbage->next;
            }

            else /* If link is not first in list at index */
            {
                prev->next  = garbage->next;
            }

            /* Free garbage and decrement count */
            hashLinkDelete(garbage);
            map->size--;
            return;                    //Return to only remove first instance
        }

        /* If no match found, go to next element in list */
        prev = current;
        current = current->next;
    }
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXME: implement
    assert (map != 0);

    /* Get hash index and ensure positive value */
    int index = HASH_FUNCTION(key) % map->capacity;
    if (index < 0)
    {
        index += map->capacity;
    }

    /* Set current to index and begin search of linked list */
    struct HashLink *current = map->table[index];

    while (current != 0)
    {
        if (strcmp(key, current->key) == 0)
        {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement
    assert (map != 0);
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement
    assert (map != 0);
    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXME: implement
    assert (map != 0);

    int bucketCount = 0;
    for (int i = 0; i < hashMapCapacity(map); i++)
    {
        if (map->table[i] == 0)
        {
            bucketCount++;
        }
    }

    return bucketCount;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXME: implement
    return (float)map->size/map->capacity;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}

