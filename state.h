/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file state.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/10/27
 * @license
 * @description
 */

#ifndef __state_H_wuqHjroC_l50f_H818_sAUn_uSvLD1Eg8UdI__
#define __state_H_wuqHjroC_l50f_H818_sAUn_uSvLD1Eg8UdI__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
//=============================================================================
//                  Constant Definition
//=============================================================================
#define STATE_ARGS_MAX_NUM                  6


typedef enum state_err
{
    STATE_ERR_OK    = 0,
    STATE_ERR_ALLOCATE_FAIL,
    STATE_ERR_INVALID_PARAM,
    STATE_ERR_UNKNOWN,
} state_err_t;


//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
/**
 *  arguments description
 */
typedef struct state_args
{
    union {
        unsigned int        u32_value;
        unsigned long long  u64_value;
        void                *pAddr;
        float               float_value;
    } data[STATE_ARGS_MAX_NUM];

} state_args_t;

/**
 *  state descriptor
 */
typedef struct state_desc
{
    // identify
    unsigned int            state_tag;

    // state initialize
    state_err_t     (*init)(state_args_t *pArgs);

    // state terminate
    state_err_t     (*deinit)(state_args_t *pArgs);

    // state routing process
    state_err_t     (*proc)(state_args_t *pArgs);

    // private data of state
    void            *pPriv_data[4];

} state_desc_t;

/**
 *  setup info for state manager
 */
typedef struct state_setup_info
{
    int     reserved;
} state_setup_info_t;

/**
 *  state manager
 */
typedef struct state_mgr
{
    long        reserved;
} state_mgr_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
state_err_t
state_mgr_create(
    state_mgr_t         **ppHStateMgr,
    state_setup_info_t  *pSetup_info);


state_err_t
state_mgr_destroy(
    state_mgr_t    **ppHStateMgr);



#ifdef __cplusplus
}
#endif

#endif