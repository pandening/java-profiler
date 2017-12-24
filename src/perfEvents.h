/*
 * Copyright 2017 Andrei Pangin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _PERFEVENTS_H
#define _PERFEVENTS_H

#include <jvmti.h>
#include <signal.h>
#include "engine.h"


class PerfEvent;
class PerfEventType;

class PerfEvents : public Engine {
  private:
    static int _max_events;
    static PerfEvent* _events;
    static PerfEventType* _event_type;
    static long _interval;

    static void createForThread(int tid);
    static void createForAllThreads();
    static void destroyForThread(int tid);
    static void destroyForAllThreads();
    static void installSignalHandler();
    static void signalHandler(int signo, siginfo_t* siginfo, void* ucontext);

  public:
    const char* name() {
        return "perf";
    }

    Error start(const char* event, long interval);
    void stop();

    static void init();
    static int tid();
    static const char** getAvailableEvents();
    static int getCallChain(int tid, const void** callchain, int max_depth);

    static void JNICALL ThreadStart(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread) {
        createForThread(tid());
    }

    static void JNICALL ThreadEnd(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread) {
        destroyForThread(tid());
    }
};

#endif // _PERFEVENTS_H
