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
    LinkedListEntry *retval=NULL;
    if(list!=NULL){
        if(list->first==NULL) {
            list->first=list->last=ll_allocEntry(list, data);
            list->nodeCount++;
        } else {
            retval = ll_insert(list->last, LL_INSERT_AFTER, data);
        }
    }
    return retval;
}

LinkedListEntry *ll_prepend(LinkedList *list,void *data) {
    LinkedListEntry *retval=NULL;
    if(list!=NULL){
        if(list->last==NULL) {
            list->first=list->last=ll_allocEntry(list, data);
            list->nodeCount++;
        } else {
            retval = ll_insert(list->first, LL_INSERT_BEFORE, data);
        }
    }
    return retval;
}

LinkedListEntry *ll_insertBefore(LinkedListEntry *entry, void *data) {
    LinkedListEntry *newNode=NULL;
    LinkedList *list;
    if(entry!=NULL) {
        list=entry->owner;

        newNode=ll_allocEntry(list, data);
        if(entry==list->first) {
            list->first=newNode;
        }
        else{
            entry->previous->next=newNode;
        }
        
        newNode->previous=entry->previous;
        newNode->next=entry;
        newNode->next->previous=newNode;
        newNode->data=data;
        list->nodeCount++;
    }
    return newNode;
}

LinkedListEntry *ll_insertAfter(LinkedListEntry *entry, void *data) {
    LinkedListEntry *newNode=NULL;
    LinkedList *list;
    
    if(entry!=NULL) {
        list=entry->owner;
        newNode=ll_allocEntry(list, data);
        
        if(entry==list->last) {
            list->last=newNode;
        } else {
            entry->next->previous=newNode;
        }
        
        newNode->next=entry->next;
        newNode->previous=entry;
        newNode->previous->next=newNode;
        newNode->data=data;
        list->nodeCount++;
    }
    
    return newNode;
}

LinkedListEntry *ll_insert(LinkedListEntry *entry, int insertMode, void *data) {
    return insertMode == LL_INSERT_BEFORE?ll_insertBefore(entry, data) : ll_insertAfter(entry, data);
}

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

void ll_destroy(LinkedList *list, void *(cleanupFunc)(void *)) {
    if(list!=NULL) {
        ll_clear(list,cleanupFunc);
        free(list);
    }
}

void ll_clear(LinkedList *list, void *(cleanupFunc)(void *)) {
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
        list->first=list->last=NULL;
        list->nodeCount=0;
    }
}

void * ll_poll(LinkedList *list) {
    void *retval=NULL;
    if(list!=NULL && list->first!=NULL) {
        retval = ll_remove(list->first,NULL);
    }
    return retval;
}

void * ll_pop(LinkedList *list) {
    void *retval=NULL;
    if(list!=NULL && list->last!=NULL) {
        retval = ll_remove(list->last,NULL);
    }
    return retval;
}