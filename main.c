#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "log.h"
#include "state.h"

#include "link_list.h"
#include <windows.h>
//===============================================

typedef struct thread_args
{
    long        *pIs_keep_thread;
    void        *pHandle;

} thread_args_t;


#define MSG_ID_CHANGE_STATE         5
typedef struct mevent
{
    llist_t             list;

    unsigned long       msg_id;
    void                *pExtra_data;

} mevent_t;
//===============================================
// global variables
LOG_FLAG_s          gLog_flags;
pthread_mutex_t     g_mutex_msg;

#define MAX_EVENT_NUM           10
static pthread_mutex_t  g_event_mutex;
static long             g_event_idx = 0;
static mevent_t         g_event[MAX_EVENT_NUM] = {{{0}}};
static llist_t          g_1st_event = {0};
//===============================================
//===============================================
// objects
static state_obj_msg_status_t
_obj_1_msg_handler(
    state_obj_base_msg_t *pMsg,
    void                 *pState_desc)
{
    dbg(" get msg\n");
    return STATE_OBJ_MSG_ROLL;
}

static state_obj_msg_status_t
_obj_2_msg_handler(
    state_obj_base_msg_t *pMsg,
    void                 *pState_desc)
{
    dbg(" get msg\n");
    return STATE_OBJ_MSG_ROLL;
}

static state_obj_msg_status_t
_obj_3_msg_handler(
    state_obj_base_msg_t *pMsg,
    void                 *pState_desc)
{
    dbg(" get msg\n");
    return STATE_OBJ_MSG_ROLL;
}

static state_obj_desc_t        g_obj_1 =
{
    .obj_tag        = 0x1,
    .cb_msg_handler = _obj_1_msg_handler,
};

static state_obj_desc_t        g_obj_2 =
{
    .obj_tag        = 0x2,
    .cb_msg_handler = _obj_2_msg_handler,
};

static state_obj_desc_t        g_obj_3 =
{
    .obj_tag        = 0x3,
    .cb_msg_handler = _obj_3_msg_handler,
};

//===============================================
// example of state description
#define STATE_ID_AAAA       0xaaaa
#define STATE_ID_BBBB       0xbbbb

static state_err_t
_state_aaaa_init(
    state_args_t        *pArgs,
    state_obj_handle_t  **ppHObj_priv)
{
    state_obj_handle_t      *pHObj = 0;

    dbg("enter\n");
    do {
        // create obj
        state_obj_create_handle(&pHObj, 0);
        if( !pHObj )
        {
            log_err("create pHObj fail \n");
            break;
        }

        // register obj
        state_obj_register(pHObj, &g_obj_1);
        state_obj_register(pHObj, &g_obj_2);
    } while(0);

    *ppHObj_priv = pHObj;
    return 0;
}

static state_err_t
_state_aaaa_deinit(
    state_args_t        *pArgs,
    state_obj_handle_t  **ppHObj_priv)
{
    state_obj_handle_t      *pHObj = *ppHObj_priv;
    dbg("enter\n");

    *ppHObj_priv = 0;
    // destroy obj
    state_obj_destroy_handle(&pHObj, 0);
    return 0;
}

static state_err_t
_state_bbbb_init(
    state_args_t        *pArgs,
    state_obj_handle_t  **ppHObj_priv)
{
    state_obj_handle_t      *pHObj = 0;

    dbg("enter\n");
    do {
        // create obj
        state_obj_create_handle(&pHObj, 0);
        if( !pHObj )
        {
            log_err("create pHObj fail \n");
            break;
        }

        // register obj
        state_obj_register(pHObj, &g_obj_3);
    } while(0);

    *ppHObj_priv = pHObj;
    return 0;
}

static state_err_t
_state_bbbb_deinit(
    state_args_t        *pArgs,
    state_obj_handle_t  **ppHObj_priv)
{
    state_obj_handle_t      *pHObj = *ppHObj_priv;
    dbg("enter\n");

    *ppHObj_priv = 0;
    // destroy obj
    state_obj_destroy_handle(&pHObj, 0);
    return 0;
}

static state_desc_t    g_state_aaaa_desc =
{
    .state_tag  = STATE_ID_AAAA,
    .init       = _state_aaaa_init,
    .deinit     = _state_aaaa_deinit,
};

static state_desc_t    g_state_bbbb_desc =
{
    .state_tag  = STATE_ID_BBBB,
    .init       = _state_bbbb_init,
    .deinit     = _state_bbbb_deinit,
};

//===============================================
// message routing
static void*
_thread_test(void *pArgs)
{
    long            *pIs_keep = ((thread_args_t*)pArgs)->pIs_keep_thread;
    state_mgr_t     *pHState = (state_mgr_t*)((thread_args_t*)pArgs)->pHandle;
    llist_t         *pList_root = &g_1st_event;

    state_mgr_set_active_state(pHState, STATE_ID_AAAA, 0);

    while( *pIs_keep )
    {
        llist_t     *pCur = 0;
        long        have_valid_event = 0l;

        // simulate message event
        pthread_mutex_lock(&g_event_mutex);
        {
            pCur = llist_fine_tail(pList_root);
            if( pCur && (pCur != pList_root) )
            {
                llist_node_remove(pCur);
                have_valid_event = 1l;
            }
        }
        pthread_mutex_unlock(&g_event_mutex);

        // state process
        if( have_valid_event )
        {
            mevent_t        *pEvent_act = (mevent_t*)((long)pCur - (long)&((mevent_t*)0)->list);

            switch( pEvent_act->msg_id )
            {
                case MSG_ID_CHANGE_STATE:
                    state_mgr_set_active_state(pHState, STATE_ID_BBBB, 0);
                    break;
                default:
                    state_mgr_state_proc(pHState, 0);
                    break;
            }
        }

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
    state_mgr_state_register(pHState, &g_state_bbbb_desc);

    // create routing
    thread_arg.pIs_keep_thread = &is_keep_thread;
    thread_arg.pHandle         = pHState;

    pthread_create(&tid, NULL, _thread_test, &thread_arg);

    // generate message event
    {
        int     i;
        for(i = 0; i < MAX_EVENT_NUM; ++i)
        {
            mevent_t    *pCur = &g_event[i];

            pthread_mutex_lock(&g_event_mutex);

            pCur->msg_id = i;
            llist_node_init(&pCur->list);
            llist_node_insert(&g_1st_event, &pCur->list);

            pthread_mutex_unlock(&g_event_mutex);
        }

        g_event_idx = i;
    }


    Sleep(5*1000);
    is_keep_thread = 0l;
    pthread_join(tid, 0);

    state_mgr_destroy(&pHState);

    return 0;
}
