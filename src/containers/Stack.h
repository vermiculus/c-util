//
//  Stack.h
//  bits
//
//  Created by Sean Allred on 7/18/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#ifndef bits_Stack_h
#define bits_Stack_h

typedef struct __CLASS__STACK__MODEL {
    size_t __length;
    void  **__head;
} * StackRef;

struct __CLASS__STACK__METHODS {
    StackRef    (*alloc)    ();
    void        (*init)     (StackRef);
    void        (*dealloc)  (StackRef);
    
    const void *(*peek)     (StackRef);
    void        (*pop)      (StackRef);
    void        (*push)     (StackRef, void *);
    size_t      (*length)   (StackRef);
} Stack;

void __CLASS__STACK__BIND() __attribute__ ((constructor)); // binds functions and whatnot

#endif
