//
//  Progress.h
//  bits
//
//  Created by Sean Allred on 8/13/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#ifndef bits_Progress_h
#define bits_Progress_h
#include "Timer.h"

/*
 ### Progress Mode
 In progress mode, the only textual output is an overall progress bar and related data. This output will either dynamically update itself (like `top`) or just display line-by-line. Below is a fabricated example of its use.
 
 $ bits --test 2048 32 11 9 --progress
 Execution begun 17:14:27.6  [PERC]      [ELPS]        [LEFT]        [ESTD]
 [-------**PROGRESS**------] xxx.x%      ELAPSED        LEFT       EST. DONE
 [-------------------------]   0.0%    00:00:00.0    00:00:42.4    17:15:00.0
 [***----------------------]  12.5%    00:00:05.3    00:00:37.1    17:15:00.0
 [*************------------]  50.0%    00:00:21.2    00:00:31.2    17:15:00.0
 [*******************------]  75.0%    00:00:31.8    00:00:10.6    17:15:00.0
 [**********************---]  87.5%    00:00:37.1    00:00:05.3    17:15:00.0
 [*************************] 100.0%    00:00:42.4    00:00:00.0    17:15:00.0
 
 While the default is to display all of them, the output columns may be chosen:
 
 $ bits --test 2048 32 11 9 --progress PBAR,ELPS,ESTD
 Execution begun 17:14:27.6      [ELPS]        [ESTD]
 [-------**PROGRESS**------]     ELAPSED     EST. DONE
 [-------------------------]   00:00:00.0    17:15:00.0
 [***----------------------]   00:00:05.3    17:15:00.0
 [*************------------]   00:00:21.2    17:15:00.0
 [*******************------]   00:00:31.8    17:15:00.0
 [**********************---]   00:00:37.1    17:15:00.0
 [*************************]   00:00:42.4    17:15:00.0
*/

typedef struct __CLASS__PROGRESS__MODEL * ProgressRef;

struct __CLASS__PROGRESS__MODEL {
    size_t on_step;
    size_t total_steps;
    clock_t *times;
    TimerRef timer;
};

struct __CLASS__PROGRESS__METHODS {
    ProgressRef (*alloc)();
    void (*init)(ProgressRef, size_t);
    ProgressRef (*copy)(ProgressRef);
    void (*dealloc)(ProgressRef);
    
    void (*start)(ProgressRef);
    void (*step)(ProgressRef);
    void (*stop)(ProgressRef);
    void (*fprint)(ProgressRef, Stream, bool, bool, bool, bool, bool);
} Progress;

void __CLASS__PROGRESS__BIND(void) __attribute__ ((constructor));

#endif
