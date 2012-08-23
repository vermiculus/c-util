//
//  Timer.h
//  bits
//
//  Created by Sean Allred on 7/17/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#ifndef bits_Timer_h
#define bits_Timer_h

#include <time.h>
#include <stdbool.h>
#include "mem.h"

typedef struct __CLASS__TIMER__MODEL * TimerRef;

struct __CLASS__TIMER__MODEL {
    clock_t begin;
    clock_t end;
    clock_t total;
    bool on;
};

struct __CLASS__TIMER__METHODS {
    TimerRef (*alloc)();
    void (*init)(TimerRef);
    TimerRef (*copy)(TimerRef);
    void (*dealloc)(TimerRef);
    
    void (*start)(TimerRef);
    void (*stop)(TimerRef);
    clock_t (*getTime)(TimerRef);
    void (*reset)(TimerRef);
    void (*fprint)(TimerRef, Stream);
} Timer;

void __CLASS__TIMER__BIND(void) __attribute__ ((constructor));

#endif
