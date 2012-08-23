//
//  LinkedList.c
//  bits
//
//  Created by Sean Allred on 7/18/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "LinkedList.h"

LinkedListRef __CLASS__LINKEDLIST__ADVANCE(LinkedListRef obj, int count) {
    size_t len = LinkedList.length(obj);
    if (count < 0) {
        count += len;
    }
    
    while (count > 0) {
        obj = obj->__next;
        count -= 1;
    }
    
    return obj;
}

LinkedListRef __CLASS__LINKEDLIST__ALLOC(void) {
    return malloc(sizeof(struct __CLASS__LINKEDLIST__MODEL));
}

void __CLASS__LINKEDLIST__INIT(LinkedListRef obj) {
    obj->__data = NULL;
    obj->__prev = NULL;
    obj->__next = NULL;
}

LinkedListRef __CLASS__LINKEDLIST__COPY(LinkedListRef obj) {
    return NULL; // this is going to be hard as hell. don't think you can just memcpy it.
}

void __CLASS__LINKEDLIST__DEALLOC(LinkedListRef obj) {
    free(obj);
}

size_t __CLASS__LINKEDLIST__LENGTH(LinkedListRef obj) {
    size_t l = 0;
    while (obj->__next != NULL) {
        obj = obj->__next;
        l++;
    }
    return l;
}

const void * __CLASS__LINKEDLIST_GET(LinkedListRef obj, int index) {
    obj = LinkedList._advance(obj, index);
    /**
     * Note that given the minimal nature of this structure,
     * this can also be done recursively:
     * return index == 0 ? obj->__data : LinkedList.get(r->__next, index - 1);
     */
    return obj->__data;
}

void __CLASS__LINKEDLIST_SET(LinkedListRef obj, int index, const void *newvalue) {
    obj = LinkedList._advance(obj, index);
    obj->__data = (void *)newvalue;
}

void __CLASS__LINKEDLIST_INSERT(LinkedListRef obj, int index, const void *value) {
    if (obj->__data == NULL && obj->__prev == NULL && obj->__next == NULL) {
        assert(index == 0);
        obj->__data = (void *)value;
    } else {
        obj = LinkedList._advance(obj, index);
    
        LinkedListRef node = LinkedList.alloc();
        node->__data = (void *)value;
    
        LinkedListRef prev = obj->__prev;
    
        prev->__next = node;
        obj->__prev = node;
    
        node->__prev = prev;
        node->__next = obj;
    }
}

void __CLASS__LINKEDLIST_REMOVE(LinkedListRef obj, int index) {
    obj = LinkedList._advance(obj, index);
    obj->__prev->__next = obj->__next;
    obj->__next->__prev = obj->__prev;
    obj->__prev = obj->__next = NULL;
}

void __CLASS__LINKEDLIST__BIND(void) {
    LinkedList.alloc    = &__CLASS__LINKEDLIST__ALLOC;
    LinkedList.init     = &__CLASS__LINKEDLIST__INIT;
    LinkedList.copy     = &__CLASS__LINKEDLIST__COPY;
    LinkedList.dealloc  = &__CLASS__LINKEDLIST__DEALLOC;
    
    LinkedList.length   = &__CLASS__LINKEDLIST__LENGTH;
    LinkedList.get      = &__CLASS__LINKEDLIST_GET;
    LinkedList.set      = &__CLASS__LINKEDLIST_SET;
    LinkedList.insert   = &__CLASS__LINKEDLIST_INSERT;
    LinkedList.remove   = &__CLASS__LINKEDLIST_REMOVE;
    
    LinkedList._advance = &__CLASS__LINKEDLIST__ADVANCE;
}
