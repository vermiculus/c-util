//
//  Progress.c
//  bits
//
//  Created by Sean Allred on 8/13/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include "Progress.h"

ProgressRef __CLASS__PROGRESS_ALLOC() {
    return malloc(sizeof(struct __CLASS__PROGRESS__MODEL));
}

void __CLASS__PROGRESS_INIT(ProgressRef obj, const size_t steps) {
    obj->on_step = 0;
    obj->total_steps = steps;
    obj->times = malloc(steps * sizeof(clock_t));
    obj->timer = Timer.alloc();
    Timer.init(obj->timer);
}

ProgressRef __CLASS__PROGRESS_COPY(ProgressRef obj) {
    ProgressRef r = Progress.alloc();
    r->on_step = obj->on_step;
    r->total_steps = obj->total_steps;
    r->times = obj->times;
    r->timer = Timer.copy(obj->timer);
    return r;
}

void __CLASS__PROGRESS_DEALLOC(ProgressRef obj) {
    free(obj->times);
    Timer.dealloc(obj->timer);
    free(obj);
}

void __CLASS__PROGRESS_START(ProgressRef obj) {
    Timer.start(obj->timer);
}

void __CLASS__PROGRESS_STEP(ProgressRef obj) {
    assert(obj->on_step < obj->total_steps);
    obj->times[obj->on_step++] = Timer.getTime(obj->timer);
}

void __CLASS__PROGRESS_STOP(ProgressRef obj) {
    Timer.stop(obj->timer);
}
void __CLASS__PROGRESS_FPRINT(ProgressRef obj, Stream s, const bool gui, const bool percent, const bool elapsed, const bool remaining, const bool estdone) {
    if (obj == NULL) {
            //[-------**PROGRESS**------] xxx.x%      ELAPSED        LEFT       EST. DONE
        fprintf(s, "%27s %5s%% %10s %10s %10s",
                gui?"[-------**PROGRESS**------]":"",
                percent?"###.#":"",
                elapsed?"ELAPSED":"",
                remaining?"LEFT":"",
                estdone?"EST. DONE":"");
    } else {
        if (gui) {
            String pbar = new_String(27);
            for (int i = 1; i < obj->on_step; i++) {
                pbar[i] = '*';
            }
            fprintf(s, " %27s", pbar);
            free(pbar);
        }
        if (percent) {
            fprintf(s, " %3.1f%%", (float)obj->on_step / (float)obj->total_steps);
        }
        if (elapsed) {
            Timer.fprint(obj->timer, s);
        }
        if (remaining) {
            fprintf(s, "fuck if I know"); //TODO: be nicer
        }
        if (estdone) {
            fprintf(s, "NEVER");
        }
    }
}

void __CLASS__PROGRESS__BIND(void) {
    Progress.alloc = __CLASS__PROGRESS_ALLOC;
    Progress.copy = __CLASS__PROGRESS_COPY;
    Progress.init = __CLASS__PROGRESS_INIT;
    Progress.dealloc = __CLASS__PROGRESS_DEALLOC;
    
    Progress.start = __CLASS__PROGRESS_START;
    Progress.step = __CLASS__PROGRESS_STEP;
    Progress.stop = __CLASS__PROGRESS_STOP;
    Progress.fprint = __CLASS__PROGRESS_FPRINT;
}
