//
//  LinkedList.h
//  bits
//
//  Created by Sean Allred on 7/18/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#ifndef bits_LinkedList_h
#define bits_LinkedList_h

typedef struct __CLASS__LINKEDLIST__MODEL * LinkedListRef;

struct __CLASS__LINKEDLIST__MODEL {
    LinkedListRef __prev;
    LinkedListRef __next;
    void * __data;
};

struct __CLASS__LINKEDLIST__METHODS {
    LinkedListRef   (*alloc)    (void);
    void            (*init)     (LinkedListRef);
    LinkedListRef   (*copy)     (LinkedListRef);
    void            (*dealloc)  (LinkedListRef);
    
    size_t          (*length)   (LinkedListRef);
    const void     *(*get)      (LinkedListRef, int);
    void            (*set)      (LinkedListRef, int, const void *);
    void            (*insert)   (LinkedListRef, int, const void *);
    void            (*remove)   (LinkedListRef, int);

    LinkedListRef   (*_advance) (LinkedListRef, int);
} LinkedList;

void __CLASS__LINKEDLIST__BIND(void) __attribute__ ((constructor));

#endif
