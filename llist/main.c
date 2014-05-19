//
//  main.c
//  llist
//
//  Created by Kevin Carter on 5/13/14.
//  Copyright (c) 2014 Kevin Carter. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "llist.h"

void printList(LinkedList *list) {
    LinkedListEntry *entry;
    long i;
    printf("Length: %li\n",ll_getCount(list));
    printf("Forward\n");
    for(i=0,entry=list->first;entry!=NULL;entry=entry->next,i++) {
        printf("%i\n",(int)entry->data);
    }
    printf("Reverse\n");
    for(i=ll_getCount(list)-1,entry=list->last;entry!=NULL;entry=entry->previous,i--) {
            printf("%i\n",(int)entry->data);
    }
    printf("\n\n");
}

void *printCleanupFunc(void *data) {
    printf("Removing: %i\n",(int) data);
    return data;
}

void *mapTestFunc(void * data, void *mapParam) {
    return (void *)((long) data * 2);
}

void *mapTestFunc2(void * data, void *mapParam) {
    *(int *) data *= *(int *) mapParam;
    return  data;
}

int filterTestFunc(void * data, void *mapParam) {
    return (int)((long) data %2);
}

int filterTestFunc2(void * data, void *mapParam) {
    return *(int *)data %2;
}

void * freeingCleanupFunction(void *data) {
    free(data);
    return NULL;
}

int searchFunc(void *data, void *param) {
    int retval = 0;
    if((int) data % 2) {
        if(param!=NULL) {
            (*(int *)param)++;
            retval = *(int *) param==2?-1:1;
        } else {
            retval = 1;
        }
    }
    return retval;
}

void printList2(LinkedList *list) {
    LinkedListEntry *entry;
    long i;
    printf("Length: %li\n",ll_getCount(list));
    printf("Forward\n");
    for(i=0,entry=list->first;entry!=NULL;entry=entry->next,i++) {
        printf("%p = %i\n",entry->data,*(int *)entry->data);
    }
    
    printf("\n\n");
}

void printSearchList(LinkedList *list) {
    LinkedListEntry *entry;
    long i;
    printf("Length: %li\n",ll_getCount(list));
    printf("Forward\n");
    for(i=0,entry=list->first;entry!=NULL;entry=entry->next,i++) {
        printf("%p -> %p -> %i\n",
               entry,
               entry->data,
               (int)((LinkedListEntry *) entry->data)->data
               );
    }
    
    printf("\n\n");
}

void *deepCopyTestFunc(void *data, void *param) {
    int *newData=malloc(sizeof(*newData));
    *newData=*(int *) data * (param==NULL?1:*(int *)param);
    return newData;
}

void copyTests() {
    LinkedList *oldList;
    LinkedList *newList;
    LinkedListEntry *entry;
    int multiplier=3;
    int i;
    printf("\n\n");
    oldList=ll_create();
    
    for(i=0;i<5;i++) {
        entry=ll_prepend(oldList, malloc(sizeof(i)));
        *(int *)entry->data=i;
    }
    
    newList=ll_copy(oldList);
    
    printf("Original\n--------\n");
    printList2(oldList);
    printf("Shallow copy\n------------\n");
    printList2(newList);
    ll_destroy(newList, NULL);
    

    newList=ll_copyAdvanced(oldList,NULL,NULL,NULL,NULL);
    
    printf("Original\n--------\n");
    printList2(oldList);
    printf("Shallow copy2\n-------------\n");
    printList2(newList);
    ll_destroy(newList, NULL);
    
    newList=ll_copyAdvanced(oldList,NULL,NULL,NULL,deepCopyTestFunc);
    
    printf("Original\n--------\n");
    printList2(oldList);
    printf("Deep copy no filter\n-------------------\n");
    printList2(newList);
    
    ll_filterInline(newList, NULL, filterTestFunc2,freeingCleanupFunction);
    
    printf("Original\n--------\n");
    printList2(oldList);
    printf("Deep copy post copy filter\n-------------------\n");
    printList2(newList);

    ll_mapInline(newList, (void *)&multiplier, mapTestFunc2);
    
    printf("Original\n--------\n");
    printList2(oldList);
    printf("Deep copy post copy map\n-------------------\n");
    printList2(newList);

    
    ll_destroy(newList, freeingCleanupFunction);
    
    newList=ll_copyAdvanced(oldList, NULL, filterTestFunc2, (void *) &multiplier, deepCopyTestFunc);
    
    printf("Original\n--------\n");
    printList2(oldList);
    printf("Deep copy in-copy filter and map\n-------------------\n");
    printList2(newList);
    
    ll_destroy(newList, freeingCleanupFunction);
    
    ll_destroy(oldList, freeingCleanupFunction);
    
}

int sortComparator(LinkedListEntry *context[],void *newData) {
    int retval = LL_SORT_DO_NOT_INSERT_YET;
    LinkedListEntry *previousentry = context[LL_SORT_CONTEXT_PREVIOUS];
    LinkedListEntry *currententry = context[LL_SORT_CONTEXT_CURRENT];
    
    if((int) newData < (int) currententry->data) {
        if(previousentry==NULL || (int) newData > (int) previousentry->data) {
            return LL_SORT_INSERT_BEFORE_CURRENT;
        }
    } else if((int) newData > (int) currententry->data) {
        return LL_SORT_INSERT_AFTER_CURRENT;
    }
    
    return retval;
}

void printBookeeping() {
    printf("\nIMPLEMENT ME\n");
}

int main(int argc, const char * argv[])
{
    long i;
    long searchStopVar=0;
    LinkedList *newList;
    LinkedList * list;
    initializeFreeList();
    printBookeeping();
    list= ll_create();
    srand((unsigned int)time(NULL));
    ll_prepend(list,(void *)9999999);
    for( i=0;i<10;i++) {
        ll_append(list, (void *)i);
    }
    printList(list);
    ll_prepend(list, (void *)77);
    ll_append(list, (void *)99);
    printList(list);
    ll_insertAfter(list->first->next, (void *)111);
    ll_insertBefore(list->last,(void *)222);
    printList(list);
    
    
    ll_remove(list->first->next,printCleanupFunc);
    printList(list);
    
    ll_remove(list->first, printCleanupFunc);
    ll_remove(list->last,printCleanupFunc);
    printList(list);
    
    printf("Poll: %i\n",(int)ll_poll(list));
    printf("Pop: %i\n", (int)ll_pop(list));
    
    printf("\n\nll_search remove test\n");
    ll_remove(ll_search(list, NULL, searchFunc),printCleanupFunc);
    
    printf("\n\nll_searchFindAll test no stop\n");
    newList = ll_searchFindAll(list, NULL, searchFunc);
    printSearchList(newList);

    ll_destroy(newList,NULL);

    
    printf("\n\nll_searchFindAll test with stop\n");
    newList = ll_searchFindAll(list, &searchStopVar, searchFunc);
    printSearchList(newList);
    ll_destroy(newList,NULL);
    
    ll_clear(list,printCleanupFunc);
    printList(list);
    
    ll_append(list,(void *)0);
    printList(list);
    ll_remove(list->first, printCleanupFunc);
    printList(list);
    
    ll_append(list,(void *)1);
    ll_append(list,(void *)2);
    ll_append(list,(void *)3);
    ll_append(list, (void *) 4);
    ll_append(list, (void *) 5);
    
    printList(list);
    
    ll_filterInline(list,NULL,filterTestFunc,printCleanupFunc);
    printList(list);
    
    ll_mapInline(list,NULL,mapTestFunc);
    printList(list);
    
    ll_destroy(list, printCleanupFunc);
    
    copyTests();
    
    list = ll_create();
    list->sortCompareFunc=sortComparator;
    ll_append(list,(void *)3);
    ll_append(list, (void *) 5);
    ll_append(list,(void *)1);
    ll_append(list, (void *) 4);
    ll_append(list,(void *)2);
    
    printList(list);
    ll_destroy(list, printCleanupFunc);
    

    printBookeeping();
    
    return 0;
}

