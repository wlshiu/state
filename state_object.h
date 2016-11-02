/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file state_object.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/11/01
 * @license
 * @description
 */

#ifndef __state_object_H_wLXR0bLB_lTpP_HL3g_s4jM_uCm26cEa430D__
#define __state_object_H_wLXR0bLB_lTpP_HL3g_s4jM_uCm26cEa430D__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
//=============================================================================
//                  Constant Definition
//=============================================================================
typedef enum state_obj_err
{
    STATE_OBJ_ERR_OK    = 0,
    STATE_OBJ_ERR_ALLOCATE_FAIL,
    STATE_OBJ_ERR_INVAILD_PARAM,
    STATE_OBJ_ERR_UNKNOWN,

} state_obj_err_t;


typedef enum state_obj_msg_status
{
    STATE_OBJ_MSG_ROLL  = 0,
    STATE_OBJ_MSG_BREAK,
} state_obj_msg_status_t;


//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct state_obj_base_msg
{
    unsigned long       msg_tag;

    unsigned long       from;
    unsigned long       to;

} state_obj_base_msg_t;

typedef state_obj_msg_status_t (*CB_OBJ_MSG_HANDLE)(state_obj_base_msg_t *pMsg, void *pExtra_msg);


/**
 *  obj descriptor
 */
typedef struct state_obj_desc
{
    unsigned long           obj_tag;

    CB_OBJ_MSG_HANDLE       cb_msg_handler;

} state_obj_desc_t;

/**
 *  object handle
 */
typedef struct state_obj_handle
{
    long            reserved;
} state_obj_handle_t;
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
    void                    *extraData);


state_obj_err_t
state_obj_destroy_handle(
    state_obj_handle_t      **ppHObj,
    void                    *extraData);


state_obj_err_t
state_obj_register(
    state_obj_handle_t      *pHObj,
    state_obj_desc_t        *pObj_desc);


state_obj_err_t
state_obj_handle_msg(
    state_obj_handle_t      *pHObj,
    state_obj_base_msg_t    *pMsg,
    void                    *pExtra_data);


#ifdef __cplusplus
}
#endif

#endif
