//
//  llist.h
//  llist
//
//  Created by Kevin Carter on 5/13/14.
//  Copyright (c) 2014 Kevin Carter. All rights reserved.
//

#ifndef llist_llist_h
#define llist_llist_h

#define LL_INSERT_AFTER 0
#define LL_INSERT_BEFORE 1

typedef struct LinkedListEntry LinkedListEntry;
typedef struct LinkedList LinkedList;

struct LinkedListEntry {
    LinkedListEntry *next;
    LinkedListEntry *previous;
    LinkedList *owner;
    void *data;
};

struct LinkedList {
    LinkedListEntry *first;
    LinkedListEntry *last;
    long nodeCount;
};

LinkedList *ll_init();

/*
 * Walks the list list until searchFunc returns 1. A reaturn of 1 from
 * searchFunc indicates the item desired has been found ,searchFunc
 * will be passed the LinkedListEntry.data member for evaluation as
 * the first argument and the searchParam argument as the second.
 * ll_search will return NULL if list is NULL, searchFunc is NULL,
 * or if the item cannot be located.
 */
void *ll_search(LinkedList *list, void * searchParam, int (searchFunc)(void *,void *));

LinkedListEntry *ll_append(LinkedList *list,void *data);
LinkedListEntry *ll_prepend(LinkedList *list,void *data);
LinkedListEntry *ll_insert(LinkedListEntry *entry, int insertMode, void *data);
LinkedListEntry *ll_insertBefore(LinkedListEntry *entry, void *data);
LinkedListEntry *ll_insertAfter(LinkedListEntry *entry, void *data);

/*
 * cleanupFunc is optional; if supplied it will be called against the
 * LinkedListEntry.data member prior to entry deallocation. The return
 * value of cleanupFunc will be returned by ll_remove if cleanupFunc
 * is supplied otherwise the value in LinkedListEntry->data is returned.
 */
void * ll_remove(LinkedListEntry *entry, void *(cleanupFunc)(void *));

/*
 * Removes the item from the head of the list and returns the object
 * pointed to by LinkedListEntry.data. Returns NULL on empty list.
 */
void * ll_poll(LinkedList *list);

/*
 * Removes item from the tail of the list and returns the object
 * pointed to by LinkedListEntry.data. Returns NULL on empty list.
 */
void * ll_pop(LinkedList *list);

/*
 * cleanupFunc is optional; if supplied it will be called against the
 * LinkedListEntry.data member prior to entry deallocation. The return
 * value of cleanupFunc is ignored in this case.
 */
void ll_clear(LinkedList *list, void *(cleanupFunc(void *)));

/*
 * Calls ll_clear and then frees the list. 
 *
* See ll_clear for cleanupFunc notes.
 */
void ll_destroy(LinkedList *list, void *(cleanupFunc)(void *));

/*
 * Applies mapFunc to every element contained in list. The pointer
 * LinkedListEntry.data will be assigned to the return value of mapFunc.
 * The first argument to mapFunc will be LinkedListEntry.data and the
 * second will be mapParam.
 */
void ll_mapInline(LinkedList *list, void *mapParam, void *(mapFunc)(void *,void *));

/*
 * Applies filterFunc to every element in list. If filterFunc returns 1
 * the element will be removed. If cleanup on the data contained in the
 * list entry (LinkedListEntry.data) filterFunc must perform the 
 * cleanup operation before it returns. The first argument to 
 * filterFunc will be the pointer LinkedListEntry.data and the second
 * will be filterParam.
 */
void ll_filterInline(LinkedList *list, void *filterParam, int (filterFunc)(void *, void *));

#endif
