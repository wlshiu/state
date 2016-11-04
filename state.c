/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file state.c
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/10/27
 * @license
 * @description
 */

#include "log.h"
#include "platform_def.h"
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
typedef struct state_box
{
    struct state_box    *next;

    state_desc_t    state_desc;

} state_box_t;

typedef struct state_dev
{
    state_mgr_t     state_mgr;

    sys_mutex_t     mtx_state;

    // state box list
    state_box_t     *pFirst_stat_box;
    state_box_t     *pLast_stat_box;

    // active state box
    state_box_t     *pState_box_act;
    state_box_t     *pState_box_next_act;
    void            *pAttach_data;

    state_obj_handle_t    *pHObj;
} state_dev_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================
#if 0
static state_err_t
_state_mgr_state_init(
    state_mgr_t     *pHStateMgr,
    state_args_t    *pArgs)
{
    state_err_t        result = STATE_ERR_OK;
    state_dev_t        *pDev = STRUCTURE_POINTER(state_dev_t, pHStateMgr, state_mgr);

    _verify_handle(pHStateMgr, STATE_ERR_INVALID_PARAM);

    _mutex_lock(&pDev->mtx_state);
    do {
        if( pDev->pState_box_act &&
            pDev->pState_box_act->state_desc.init )
        {
            if( (result = pDev->pState_box_act->state_desc.init(pArgs)) )
                    != STATE_ERR_OK )
            {
                log_err("init state fail (result= x%x)\n", result);
                break;
            }
        }
    } while(0);

    _mutex_unlock(&pDev->mtx_state);

    return result;
}

static state_err_t
_state_mgr_state_deinit(
    state_mgr_t     *pHStateMgr,
    state_args_t    *pArgs)
{
    state_err_t        result = STATE_ERR_OK;
    state_dev_t        *pDev = STRUCTURE_POINTER(state_dev_t, pHStateMgr, state_mgr);

    _verify_handle(pHStateMgr, STATE_ERR_INVALID_PARAM);

    _mutex_lock(&pDev->mtx_state);
    do {
        if( pDev->pState_box_act &&
            pDev->pState_box_act->state_desc.deinit )
        {
            if( (result = pDev->pState_box_act->state_desc.deinit(pArgs))
                    != STATE_ERR_OK )
            {
                log_err("deinit state fail (result= x%x)\n", result);
                break;
            }
        }
    } while(0);

    _mutex_unlock(&pDev->mtx_state);

    return result;
}
#endif
//=============================================================================
//                  Public Function Definition
//=============================================================================
state_err_t
state_mgr_create(
    state_mgr_t         **ppHStateMgr,
    state_setup_info_t  *pSetup_info)
{
    state_err_t        result = STATE_ERR_OK;

    do {
        state_dev_t         *pDev = 0;

        if( *ppHStateMgr )
        {
            log_err("error, Exist d handle !!\n");
            result = STATE_ERR_INVALID_PARAM;
            break;
        }

        if( !(pDev = sys_malloc(0, sizeof(state_dev_t))) )
        {
            log_err("error, malloc (%d) fail !!\n", sizeof(state_dev_t));
            result = STATE_ERR_ALLOCATE_FAIL;
            break;
        }

        memset(pDev, 0x0, sizeof(state_dev_t));

        _mutex_init(&pDev->mtx_state);

        //----------------------
        (*ppHStateMgr) = &pDev->state_mgr;

    } while(0);

    if( result != STATE_ERR_OK )
    {
        // TODO: destroy dev
        log_err("err 0x%x !\n", result);
    }

    return result;
}

state_err_t
state_mgr_destroy(
    state_mgr_t    **ppHStateMgr)
{
    state_err_t         result = STATE_ERR_OK;

    if( ppHStateMgr && *ppHStateMgr )
    {
        state_dev_t         *pDev = STRUCTURE_POINTER(state_dev_t, (*ppHStateMgr), state_mgr);
        sys_mutex_t         mutex;

        _mutex_lock(&pDev->mtx_state);

        *ppHStateMgr = 0;

        mutex = pDev->mtx_state;

        // TODO: free registered state list

        sys_free(pDev);

        _mutex_unlock(&mutex);
        _mutex_deinit(&mutex);
    }

    return result;
}

state_err_t
state_mgr_state_register(
    state_mgr_t     *pHStateMgr,
    state_desc_t    *pState_desc)
{
    state_err_t        result = STATE_ERR_OK;
    state_dev_t        *pDev = STRUCTURE_POINTER(state_dev_t, pHStateMgr, state_mgr);

    _verify_handle(pHStateMgr, STATE_ERR_INVALID_PARAM);
    _verify_handle(pState_desc, STATE_ERR_INVALID_PARAM);

    _mutex_lock(&pDev->mtx_state);
    do {
        state_box_t     *pState_box_tmp = 0, *pState_box_new = 0;

        // verify state exist or not
        pState_box_tmp = pDev->pFirst_stat_box;
        while( pState_box_tmp )
        {
            if( pState_box_tmp->state_desc.state_tag == pState_desc->state_tag )
            {
                log_err("state (tag= x%lx) is already registered\n", pState_desc->state_tag);
                result = STATE_ERR_INVALID_PARAM;
                break;
            }
            pState_box_tmp = pState_box_tmp->next;
        }

        if( pState_box_tmp )
            break;

        // create instance
        if( !(pState_box_new = sys_malloc(0, sizeof(state_box_t))) )
        {
            log_err("error, malloc (%d) fail !!\n", sizeof(state_box_t));
            result = STATE_ERR_ALLOCATE_FAIL;
            break;
        }
        memset(pState_box_new, 0x0, sizeof(state_box_t));

        // assign info
        pState_box_new->state_desc = (*pState_desc);

        // insert
        if( pDev->pFirst_stat_box )
        {
            pDev->pLast_stat_box->next = pState_box_new;
            pDev->pLast_stat_box       = pState_box_new;
        }
        else
        {
            pDev->pFirst_stat_box = pState_box_new;
            pDev->pLast_stat_box  = pState_box_new;
        }

    } while(0);

    _mutex_unlock(&pDev->mtx_state);

    return result;
}


state_err_t
state_mgr_state_proc(
    state_mgr_t     *pHStateMgr,
    state_args_t    *pArgs)
{
    state_err_t        result = STATE_ERR_OK;
    state_dev_t        *pDev = STRUCTURE_POINTER(state_dev_t, pHStateMgr, state_mgr);

    _verify_handle(pHStateMgr, STATE_ERR_INVALID_PARAM);

    _mutex_lock(&pDev->mtx_state);
    do {
        // send msg to all objects in this state
        state_obj_handle_msg(pDev->pHObj, (state_obj_base_msg_t*)&pArgs->base_msg_info,
                             (pDev->pState_box_act) ? &pDev->pState_box_act->state_desc : 0);

        // state change
        if( pDev->pState_box_act != pDev->pState_box_next_act )
        {
            state_desc_t    *pAct_desc = &pDev->pState_box_act->state_desc;
            state_desc_t    *pNext_act_desc = &pDev->pState_box_next_act->state_desc;

            // terminate current state
            if( pDev->pState_box_act && pAct_desc->deinit )
            {
                state_obj_handle_t    *pHObj_act = pDev->pHObj;
                pAct_desc->deinit(pDev->pAttach_data, &pHObj_act);
                pDev->pHObj = 0;
            }

            // initialize new state
            if( pDev->pState_box_next_act && pNext_act_desc->init )
            {
                state_obj_handle_t    *pHObj_act = 0;
                pNext_act_desc->init(pDev->pAttach_data, &pHObj_act);

                pDev->pHObj = pHObj_act;
            }

            pDev->pAttach_data   = 0;
            pDev->pState_box_act = pDev->pState_box_next_act;
        }

    } while(0);

    _mutex_unlock(&pDev->mtx_state);

    return result;
}

state_err_t
state_mgr_set_active_state(
    state_mgr_t     *pHStateMgr,
    unsigned long   act_state_tag,
    state_args_t    *pAttach_data)
{
    state_err_t        result = STATE_ERR_OK;
    state_dev_t        *pDev = STRUCTURE_POINTER(state_dev_t, pHStateMgr, state_mgr);

    _verify_handle(pHStateMgr, STATE_ERR_INVALID_PARAM);

    _mutex_lock(&pDev->mtx_state);
    do {
        state_box_t     *pTmp_state_box = 0, *pTarget_state_box = 0;

        // find the target state
        pTmp_state_box = pDev->pFirst_stat_box;
        while( pTmp_state_box )
        {
            if( pTmp_state_box->state_desc.state_tag == act_state_tag )
            {
                pTarget_state_box = pTmp_state_box;
                break;
            }

            pTmp_state_box = pTmp_state_box->next;
        }

        if( !pTarget_state_box )
        {
            log_err("can't find this state (x%lx)\n", act_state_tag);
            result = STATE_ERR_INVALID_PARAM;
            break;
        }

        pDev->pState_box_next_act = pTarget_state_box;
        pDev->pAttach_data        = pAttach_data;

        if( pDev->pState_box_act == pTarget_state_box )
        {
            log_info("current state (x%lx) was target, no change... \n", act_state_tag);
            break;
        }

    } while(0);

    _mutex_unlock(&pDev->mtx_state);

    return result;
}

state_err_t
state_mgr_object_register(
    state_mgr_t     *pHStateMgr,
    void            *extraData)
{
    state_err_t        result = STATE_ERR_OK;


    if( result != STATE_ERR_OK )
    {
        log_err("err 0x%x !\n", result);
    }

    return result;
}

/*
state_err_t
state_mgr_tamplete(
    state_mgr_t     *pHStateMgr,
    void            *extraData)
{
    state_err_t        result = STATE_ERR_OK;


    if( result != STATE_ERR_OK )
    {
        log_err("err 0x%x !\n", result);
    }

    return result;
}
*/
