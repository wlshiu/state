#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "log.h"
#include "state.h"

#include <windows.h>
//===============================================

typedef struct thread_args
{
    long        *pIs_keep_thread;
    void        *pHandle;

} thread_args_t;
//===============================================
LOG_FLAG_s          gLog_flags;
pthread_mutex_t     g_mutex_msg;
//===============================================

//===============================================
static void*
_thread_test(void *pArgs)
{
//    thread_args_t   *pThread_arg = (thread_args_t*)pArgs;
    long            *pIs_keep = ((thread_args_t*)pArgs)->pIs_keep_thread;
    state_mgr_t     *pHState = (state_mgr_t*)((thread_args_t*)pArgs)->pHandle;

    while( *pIs_keep )
    {

        Sleep(10);
    }

    pthread_exit(0);
    return NULL;
}
//===============================================
int main()
{
    pthread_t       tid;
    long            is_keep_thread = 1l;
    state_mgr_t     *pHState = 0;
    thread_args_t   thread_arg = {0};

    pthread_mutex_init(&g_mutex_msg, 0);
    log_on(LOG_STATE);


    thread_arg.pIs_keep_thread = &is_keep_thread;
    thread_arg.pHandle         = pHState;

    pthread_create(&tid, NULL, _thread_test, &thread_arg);

    printf("Hello world!\n");
    return 0;
}
