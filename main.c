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


typedef struct m_event
{
    unsigned long       msg_id;

} m_event_t;
//===============================================
// global variables
LOG_FLAG_s          gLog_flags;
pthread_mutex_t     g_mutex_msg;

#define MAX_EVENT_NUM           10
static pthread_mutex_t  g_event_mutex;
static long             g_event_idx = 0;
static m_event_t        g_event[MAX_EVENT_NUM] = {{0}};
//===============================================

//===============================================
// example of state description
static state_err_t
_state_aaaa_init(state_args_t *pArgs)
{
    dbg("enter\n");
    // create obj
    // register obj
    return 0;
}

static state_err_t
_state_aaaa_deinit(state_args_t *pArgs)
{
    dbg("enter\n");
    // destroy obj
    return 0;
}


static state_desc_t    g_state_aaaa_desc =
{
    .state_tag  = 0xaaaa,
    .init       = _state_aaaa_init,
    .deinit     = _state_aaaa_deinit,
//    .proc       = _state_aaaa_proc,
};

//===============================================
// message routing
static void*
_thread_test(void *pArgs)
{
    long            *pIs_keep = ((thread_args_t*)pArgs)->pIs_keep_thread;
    state_mgr_t     *pHState = (state_mgr_t*)((thread_args_t*)pArgs)->pHandle;

    while( *pIs_keep )
    {
        // simulate message event

        // state process

        Sleep(10);
    }

    pthread_exit(0);
    return NULL;
}

//===============================================
int main(void)
{
    pthread_t       tid;
    long            is_keep_thread = 1l;
    state_mgr_t     *pHState = 0;
    thread_args_t   thread_arg = {0};

    // environment  setup
    pthread_mutex_init(&g_mutex_msg, 0);
    log_on(LOG_STATE);


    pthread_mutex_init(&g_event_mutex, 0);
    //=====================================
    // action
    state_mgr_create(&pHState, 0);

    // register state
    state_mgr_state_register(pHState, &g_state_aaaa_desc);

    // generate message event
    pthread_mutex_lock(&g_event_mutex);

    pthread_mutex_unlock(&g_event_mutex);

    // create routing
    thread_arg.pIs_keep_thread = &is_keep_thread;
    thread_arg.pHandle         = pHState;

    pthread_create(&tid, NULL, _thread_test, &thread_arg);

    Sleep(30*1000);
    is_keep_thread = 0l;
    pthread_join(tid, 0);

    state_mgr_destroy(&pHState);

    return 0;
}
