/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file state_id.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/10/27
 * @license
 * @description
 */

#ifndef __state_id_H_w79dSiHP_lIYM_HJwA_s4ED_uC4xgcomwhas__
#define __state_id_H_w79dSiHP_lIYM_HJwA_s4ED_uC4xgcomwhas__

#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
//                  Constant Definition
//=============================================================================
#define STATE_ID_AAAA       0xaaaa
#define STATE_ID_BBBB       0xbbbb

/**
 *  set of state id (maybe don't need these)
 */
typedef enum state_id
{
    state_id_main,
} state_id_t;
//=============================================================================
//                  Macro Definition
//=============================================================================
#define FOURCC(a, b, c, d)              ((a & 0xFF) << 24 | (b & 0xFF) << 16 | (c & 0xFF) << 8 | (d & 0xFF))
//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
