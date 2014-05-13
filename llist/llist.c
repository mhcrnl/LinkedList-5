//
//  llist.c
//  llist
//
//  Created by Kevin Carter on 5/13/14.
//  Copyright (c) 2014 Kevin Carter. All rights reserved.
//
#include <stdlib.h>
#include <stddef.h>
#include "llist.h"


static LinkedListEntry * ll_allocEntry(LinkedList *owner,void *data) {
    LinkedListEntry *newNode;
    if(owner!=NULL) {
        newNode= calloc(1,sizeof(*newNode));
        newNode->owner=owner;
        newNode->data=data;
    }
    return newNode;
}

LinkedList *ll_init() {
    return calloc(1,sizeof(LinkedList));
}

/*
 * Walks the list list until searchFunc returns 1. A reaturn of 1 from
 * searchFunc indicates the item desired has been found ,searchFunc
 * will be passed the LinkedListEntry.data member for evaluation as
 * the first argument and the searchParam argument as the second.
 * ll_search will return NULL if list is NULL, searchFunc is NULL,
 * or if the item cannot be located.
 */
void *ll_search(LinkedList *list, void * searchParam, int (searchFunc)(void *, void *)) {
    LinkedListEntry *entry=NULL;
    
    if(list!=NULL && searchFunc!=NULL){
        for(entry=list->first;
            entry!=NULL && searchFunc(entry->data, searchParam)!=1;
            entry=entry->next);
    }
    
    return (entry == NULL)?NULL:entry->data;
}

LinkedListEntry *ll_append(LinkedList *list,void *data) {
    LinkedListEntry *newNode=NULL;
    if(list!=NULL) {
        
        newNode=ll_allocEntry(list, data);
        if(newNode !=NULL){
            newNode->previous=list->last;
            if(list->first !=NULL){
                list->last->next=newNode;
            } else {
                list->first = newNode;
            }
            list->last=newNode;
            list->nodeCount++;
        }
    }
    return newNode;
}

LinkedListEntry *ll_prepend(LinkedList *list,void *data) {
    LinkedListEntry *newNode=NULL;
    if(list!=NULL) {
        
        newNode = ll_allocEntry(list, data);
        if(newNode !=NULL){
            newNode->next=list->first;
            if(list->first !=NULL) {
                list->first->previous=newNode;
            } else {
                list->last=newNode;
            }
            
            list->first=newNode;
            list->nodeCount++;
        }
    }
    return newNode;
}



LinkedListEntry *ll_insert(LinkedListEntry *entry, int insertMode, void *data) {
    LinkedListEntry *newNode=NULL;
    LinkedList *list;
    if(entry!=NULL){
        list=entry->owner;
        newNode = ll_allocEntry(list, data);
        if(newNode!=NULL){
            if(insertMode) {
                newNode->next=entry;
                newNode->previous=entry->previous;
                entry->previous=newNode;
                if(entry==list->first){
                    list->first=newNode;
                }
            } else {
                newNode->previous=entry;
                newNode->next=entry->next;
                entry->next=newNode;
                if(entry==list->last) {
                    list->last=entry;
                }
            }
            list->nodeCount++;
        }
    }
    
    return newNode;
}

/*
 * cleanupFunc is optional; if supplied it will be called against the
 * LinkedListEntry.data member prior to entry deallocation. The return
 * value of cleanupFunc will be returned by ll_remove if cleanupFunc
 * is supplied otherwise the value in LinkedListEntry->data is returned.
 */
void * ll_remove(LinkedListEntry *entry, void *(cleanupFunc)(void *)) {
    void *retval = NULL;
    LinkedList *list;
    if(entry!=NULL) {
        list=entry->owner;
        if(entry==list->first){
            list->first=entry->next;
        } else {
            entry->previous->next=entry->next;
        }
        
        if(entry==list->last) {
            list->last=entry->previous;
        } else {
            entry->next->previous=entry->previous;
        }
        retval = entry->data;
        
        if(cleanupFunc!=NULL) {
            retval = cleanupFunc(retval);
        }
        
        free(entry);
        list->nodeCount--;
    }
    
    return retval;
}


/*
 * cleanupFunc is optional; if supplied it will be called against the
 * LinkedListEntry.data member prior to entry deallocation. The return
 * value of cleanupFunc is ignored in this case.
 */
void ll_destroy(LinkedList *list, void *(cleanupFunc)(void *)) {
    LinkedListEntry *toDelete=NULL;
    LinkedListEntry *current;
    if(list!=NULL){
        current=list->first;
        while(current!=NULL) {
            toDelete=current;
            current=current->next;
            if(cleanupFunc!=NULL) {
                cleanupFunc(toDelete->data);
            }
            free(toDelete);
        }
        free(list);
    }
}