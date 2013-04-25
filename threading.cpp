#include "threading.h"
#ifdef _WIN32
thread call_thread( void(*func)(void*), void* args )
{
    return _beginthread( func, 0, args );
}
mutex mutex_create(){
    return CreateMutex(
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);             // unnamed mutex
}
void mutex_lock(mutex& m){
    WaitForSingleObject(
            m,    // handle to mutex
            INFINITE);  // no time-out interval
}
void mutex_unlock(mutex& m){
    ReleaseMutex(m);
}

#else
thread call_thread( void(*func)(void*), void* args )
{

    pthread_t iThreadId;
    return pthread_create(&iThreadId, NULL, (void*(*)(void*))func, args);
}
static mutex initmutex = PTHREAD_MUTEX_INITIALIZER;
mutex mutex_create(){
    return initmutex;
}
void mutex_lock(mutex& m){
    pthread_mutex_lock( &m );
}
void mutex_unlock(mutex& m){
    pthread_mutex_unlock( &m );
}
#endif
