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

LinkedListEntry * ll_search(LinkedList *list, void * searchParam, int (searchFunc)(void *, void *)) {
    LinkedListEntry *entry=NULL;
    
    if(list!=NULL && searchFunc!=NULL){
        for(entry=list->first;
            entry!=NULL && !searchFunc(entry->data, searchParam);
            entry=entry->next);
    }
    
    return entry;
}

LinkedListEntry *ll_append(LinkedList *list,void *data) {
    LinkedListEntry *retval=NULL;
    if(list!=NULL){
        if(list->first==NULL) {
            retval=list->first=list->last=ll_allocEntry(list, data);
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
            retval=list->first=list->last=ll_allocEntry(list, data);
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

void ll_mapInline(LinkedList *list, void *mapParam, void *(mapFunc)(void *,void *)) {
    LinkedListEntry *entry;
    if(list!=NULL && mapFunc!=NULL) {
        for(entry=list->first;entry!=NULL;entry=entry->next) {
            entry->data = mapFunc(entry->data,mapParam);
        }
    }
}

void ll_filterInline(LinkedList *list, void *filterParam, int (filterFunc)(void *, void *)) {
    LinkedListEntry tempEntry;
    LinkedListEntry *entry;
    if(list !=NULL && filterFunc!=NULL) {
        for(entry=list->first;entry!=NULL;entry=entry->next) {
            if(filterFunc(entry->data,filterParam)) {
                tempEntry.next=entry->next;
                ll_remove(entry,NULL);
                entry=&tempEntry;
            }
        }
    }
}

LinkedList *ll_copy(LinkedList *list) {
    return ll_copyAdvanced(list, NULL, NULL, NULL, NULL);
}

void *defaultDeepCopyFunc(void *data,void *param) {
    return data;
}

LinkedList * ll_copyAdvanced(LinkedList *list,
                             void *filterParam,
                             int(filterFunc)(void *, void *),
                             void *deepCopyFuncParam,
                             void *(deepCopyFunc)(void *, void *)) {
    LinkedList *retval = NULL;
    LinkedListEntry *entry;
    if(list!=NULL) {
        if(deepCopyFunc==NULL) {
            deepCopyFunc=defaultDeepCopyFunc;
        }
        retval=ll_init();
        for(entry=list->first;entry!=NULL;entry=entry->next) {
            if(filterFunc==NULL || !filterFunc(entry->data,filterParam)) {
                ll_append(retval,deepCopyFunc(entry->data,deepCopyFuncParam));
            }
        }
    }
    
    return retval;
}

LinkedList *ll_searchFindAll(LinkedList *list, void * searchParam, int (searchFunc)(void *,void *)) {
    LinkedList *retval=NULL;
    LinkedListEntry *entry;

    int sfRes=0;
    if(list!=NULL && searchFunc!=NULL) {
        retval = ll_init();
        for(entry=list->first;entry!=NULL && sfRes!=-1;entry=entry->next) {
            if((sfRes=searchFunc(entry->data,searchParam))) {
                ll_append(retval, entry);
            }
        }
    }
    
    return retval;
}