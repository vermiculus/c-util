//
//  Stack.c
//  bits
//
//  Created by Sean Allred on 7/18/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"

size_t ____normalize_index(int given, size_t max) {
    assert(abs(given) <= max);
    return given < 0 ? max + given : given;
}

StackRef ___CLASS__STACK_ALLOC() {
    return malloc(sizeof(struct __CLASS__STACK__MODEL));
}

void ___CLASS__STACK_INIT(StackRef obj) {
    obj->__length = 0;
    obj->__head = NULL;
}

void ___CLASS__STACK_DEALLOC(StackRef obj) {
    free(obj->__head);
    free(obj);
}

const void *___CLASS__STACK_PEEK(StackRef obj) {
    assert(obj->__length > 0);
    return obj->__head[obj->__length - 1];
}

void ___CLASS__STACK_POP(StackRef obj) {
    assert(obj->__length > 0);
    obj->__length -= 1;
    realloc(obj->__head, obj->__length);
}

void ___CLASS__STACK_PUSH(StackRef obj, void *value) {
    obj->__length += 1;
    obj->__head = realloc(obj->__head, obj->__length);
    obj->__head[obj->__length - 1] = value;
}

size_t ___CLASS__STACK_LENGTH(StackRef obj) {
    return obj->__length;
}


void __CLASS__STACK__BIND (void) {
    Stack.alloc     = &___CLASS__STACK_ALLOC;
    Stack.init      = &___CLASS__STACK_INIT;
    Stack.dealloc   = &___CLASS__STACK_DEALLOC;
    Stack.peek      = &___CLASS__STACK_PEEK;
    Stack.pop       = &___CLASS__STACK_POP;
    Stack.push      = &___CLASS__STACK_PUSH;
    Stack.length    = &___CLASS__STACK_LENGTH;
}