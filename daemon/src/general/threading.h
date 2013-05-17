#pragma once
#ifdef _WIN32
    #ifndef WINVER
    #define WINVER 0xFFFF
    #endif
    #include<windows.h>
    #include<process.h>
    #define socklen_t int
    typedef HANDLE mutex;
    typedef uintptr_t thread;
#else

    #include <pthread.h>
    //typedef pthread_mutex_t mutex;
    typedef int thread;
    struct mutex{
        pthread_mutex_t m;
    };
#endif


thread call_thread( void(*func)(void*), void* args );

mutex
#ifndef _WIN32
&
#endif
mutex_create();
void mutex_lock(mutex& m);
void mutex_unlock(mutex& m);

