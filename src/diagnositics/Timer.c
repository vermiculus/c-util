//
//  Timer.c
//  bits
//
//  Created by Sean Allred on 7/17/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Timer.h"

TimerRef __CLASS__TIMER_ALLOC() {
    return malloc(sizeof(struct __CLASS__TIMER__MODEL));
}

void __CLASS__TIMER_DEALLOC(TimerRef obj) {
    free(obj);
}

TimerRef __CLASS__TIMER_COPY(TimerRef obj) {
    TimerRef ret = Timer.alloc();
    ret->on    = obj->on;
    ret->begin = obj->begin;
    ret->end   = obj->end;
    ret->total = obj->total;
    return ret;
}

void __CLASS__TIMER_INIT(TimerRef obj) {
    obj->on    = false;
    obj->begin = 0;
    obj->end   = 0;
    obj->total = 0;
}

void __CLASS__TIMER_START(TimerRef obj) {
    if (!obj->on) {
        obj->on = true;
        obj->begin = clock();
    }
}

void __CLASS__TIMER_STOP(TimerRef obj) {
    if (obj->on) {
        obj->end = clock();
        obj->total += (obj->end - obj->begin);
        obj->on = false;
    }
}

clock_t __CLASS__TIMER_GET_TIME(TimerRef obj) {
    return obj->total;
}

void __CLASS__TIMER_RESET(TimerRef obj) {
    obj->total = 0;
    obj->begin = 0;
    obj->end = 0;
    obj->on = false;
}

void __CLASS__TIMER_FPRINT(TimerRef obj, Stream s) {
    fprintf(s, "%tu ticks (%f seconds)\n", obj->total, (float)obj->total/CLOCKS_PER_SEC);
}

void __CLASS__TIMER__BIND(void) {
    Timer.alloc   =  &__CLASS__TIMER_ALLOC;
    Timer.init    =  &__CLASS__TIMER_INIT;
    Timer.copy    =  &__CLASS__TIMER_COPY;
    Timer.dealloc =  &__CLASS__TIMER_DEALLOC;
    Timer.start   =  &__CLASS__TIMER_START;
    Timer.stop    =  &__CLASS__TIMER_STOP;
    Timer.reset   =  &__CLASS__TIMER_RESET;
    Timer.getTime =  &__CLASS__TIMER_GET_TIME;
    Timer.fprint  =  &__CLASS__TIMER_FPRINT;
}