/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file state_object.c
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/11/01
 * @license
 * @description
 */

#include "log.h"
#include "platform_def.h"
#include "state_object.h"

//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
/**
 *  obj descriptor
 */
typedef struct state_obj_desc_box
{
    struct state_obj_desc   *next;

    state_obj_desc_t        obj_desc;

} state_obj_desc_box_t;


/**
 *  device of state object
 */
typedef struct state_obj_dev
{
    state_obj_handle_t          state_obj_handle;

    sys_mutex_t                 mtx_obj;

    state_obj_desc_box_t        *pFirst_obj;
    state_obj_desc_box_t        *pLast_obj;

} state_obj_dev_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
state_obj_err_t
state_obj_create_handle(
    state_obj_handle_t      **ppHObj,
    void                    *extraData)
{
    state_obj_err_t        rval = STATE_OBJ_ERR_OK;

    do {
        state_obj_dev_t     *pDev = 0;

        if( *ppHObj != 0 )
        {
            log_err("error, Exist object handle !!");
            break;
        }

        if( !(pDev = sys_malloc(0, sizeof(state_obj_dev_t))) )
        {
            log_err("malloc (%d) fail \n", sizeof(state_obj_dev_t));
            rval = STATE_OBJ_ERR_ALLOCATE_FAIL;
            break;
        }
        memset(pDev, 0x0, sizeof(state_obj_dev_t));

        _mutex_init(&pDev->mtx_obj);

        //----------------------
        (*ppHObj) = &pDev->state_obj_handle;

    } while(0);

    if( rval != STATE_OBJ_ERR_OK )
    {
        // TODO: destroy dev
        log_err("err 0x%x !", rval);
    }

    return rval;
}

state_obj_err_t
state_obj_destroy_handle(
    state_obj_handle_t      **ppHObj,
    void                    *extraData)
{
    state_obj_err_t        rval = STATE_OBJ_ERR_OK;
    if( ppHObj && *ppHObj )
    {
        state_obj_dev_t     *pDev = STRUCTURE_POINTER(state_obj_dev_t, (*ppHObj), state_obj_handle);
        sys_mutex_t         mutex;

        _mutex_lock(&pDev->mtx_obj);

        *ppHObj = 0;

        mutex = pDev->mtx_obj;

        // TODO: free registered state list

        sys_free(pDev);

        _mutex_unlock(&mutex);
        _mutex_deinit(&mutex);
    }

    return rval;
}

state_obj_err_t
state_obj_register(
    state_obj_handle_t      *pHObj,
    state_obj_desc_t        *pObj_desc)
{
    state_obj_err_t     rval = STATE_OBJ_ERR_OK;
    state_obj_dev_t     *pDev = STRUCTURE_POINTER(state_obj_dev_t, pHObj, state_obj_handle);

    _verify_handle(pHObj, STATE_OBJ_ERR_INVAILD_PARAM);
    _verify_handle(pObj_desc, STATE_OBJ_ERR_INVAILD_PARAM);

    _mutex_lock(&pDev->mtx_obj);
    do {

    } while(0);

    _mutex_unlock(&pDev->mtx_obj);

    return rval;
}

/*
state_obj_err_t
d_tamplete(
    state_obj_handle_t      *pHObj,
    void                    *extraData)
{
    state_obj_err_t     rval = STATE_OBJ_ERR_OK;
    state_obj_dev_t     *pDev = STRUCTURE_POINTER(state_obj_dev_t, pHObj, state_obj_handle);

    _verify_handle(pHObj, STATE_OBJ_ERR_INVAILD_PARAM);

    _mutex_lock(&pDev->mtx_obj);
    do {

    } while(0);

    _mutex_unlock(&pDev->mtx_obj);
    return rval;
}
*/
