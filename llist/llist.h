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

/*
 * cleanupFunc is optional; if supplied it will be called against the
 * LinkedListEntry.data member prior to entry deallocation. The return
 * value of cleanupFunc will be returned by ll_remove if cleanupFunc
 * is supplied otherwise the value in LinkedListEntry->data is returned.
 */
void * ll_remove(LinkedListEntry *entry, void *(cleanupFunc)(void *));

/*
 * cleanupFunc is optional; if supplied it will be called against the
 * LinkedListEntry.data member prior to entry deallocation. The return
 * value of cleanupFunc is ignored in this case.
 */
void ll_destroy(LinkedList *list, void *(cleanupFunc)(void *));
#endif
