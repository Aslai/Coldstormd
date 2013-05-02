#pragma once
#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include<process.h>
    #define socklen_t int
    typedef HANDLE mutex;
    typedef uintptr_t thread;
#else

    #include <pthread.h>
    typedef pthread_mutex_t mutex;
    typedef int thread;
#endif


thread call_thread( void(*func)(void*), void* args );
mutex mutex_create();
void mutex_lock(mutex& m);
void mutex_unlock(mutex& m);

