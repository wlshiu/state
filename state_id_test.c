/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file state_id_test.c
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/11/04
 * @license
 * @description
 */

#include <pthread.h>
#include "log.h"
#include "state_id.h"
#include "state.h"

//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================
// objects
static state_obj_msg_status_t
_obj_1_msg_handler(
    state_obj_base_msg_t *pMsg,
    void                 *pState_desc)
{
    dbg(" get msg (tag= %ld, x%p)\n", pMsg->msg_tag, ((state_args_t*)pMsg)->data[0].pAddr);
    return STATE_OBJ_MSG_ROLL;
}

static state_obj_msg_status_t
_obj_2_msg_handler(
    state_obj_base_msg_t *pMsg,
    void                 *pState_desc)
{
    dbg(" get msg (tag= %ld, x%p)\n", pMsg->msg_tag, ((state_args_t*)pMsg)->data[0].pAddr);
    return STATE_OBJ_MSG_ROLL;
}

static state_obj_msg_status_t
_obj_3_msg_handler(
    state_obj_base_msg_t *pMsg,
    void                 *pState_desc)
{
    dbg(" get msg (tag= %ld, x%p)\n", pMsg->msg_tag, ((state_args_t*)pMsg)->data[0].pAddr);
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

//=================================================
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
//=============================================================================
//                  Public Function Definition
//=============================================================================
state_desc_t    g_state_aaaa_desc =
{
    .state_tag  = STATE_ID_AAAA,
    .init       = _state_aaaa_init,
    .deinit     = _state_aaaa_deinit,
};

state_desc_t    g_state_bbbb_desc =
{
    .state_tag  = STATE_ID_BBBB,
    .init       = _state_bbbb_init,
    .deinit     = _state_bbbb_deinit,
};
