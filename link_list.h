/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file link_list.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/11/02
 * @license
 * @description
 */

#ifndef __link_list_H_wFt1TcqZ_lDai_HuyP_sGor_uLZS62JZ3n1F__
#define __link_list_H_wFt1TcqZ_lDai_HuyP_sGor_uLZS62JZ3n1F__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct llist
{
    struct llist        *next, *prev;

} llist_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================
static inline int
llist_node_init(struct llist *pllist)
{
    int         rval = -1;
    if( pllist )
    {
        memset(pllist, 0x0, sizeof(struct llist));
        rval = 0;
    }
    return rval;
}

static inline int
llist_node_insert(struct llist *pAct_llist, struct llist *pNew_llist)
{
    int     rval = -1;
    do {
        if( !pAct_llist || !pNew_llist )
            break;

        log_err("insert: x%p\n", pNew_llist);
        if( pAct_llist->next )
            pAct_llist->next->prev = pNew_llist;
        pNew_llist->next = pAct_llist->next;
        pAct_llist->next = pNew_llist;
        pNew_llist->prev = pAct_llist;

        rval = 0;
    } while(0);

    return rval;
}

static inline int
llist_node_remove(struct llist *pRemoved_llist)
{
    int     rval = -1;

    do {
        if( !pRemoved_llist )   break;

        if( pRemoved_llist->prev )
            pRemoved_llist->prev->next = pRemoved_llist->next;

        if( pRemoved_llist->next )
            pRemoved_llist->next->prev = pRemoved_llist->prev;

        pRemoved_llist->next = pRemoved_llist->prev = 0;
        rval = 0;
    } while(0);

    return rval;
}

static inline llist_t*
llist_fine_head(struct llist *pllist)
{
    struct llist   *pCur = 0;

    pCur = pllist;
    while( pCur )
    {
        if( !pCur->prev )
            break;

        pCur = pCur->prev;
    }

    return pCur;
}

static inline llist_t*
llist_fine_tail(struct llist *pllist)
{
    struct llist   *pCur = 0;

    pCur = pllist;
    while( pCur )
    {
        if( !pCur->next )
            break;

        pCur = pCur->next;
    }

    return pCur;
}
//=============================================================================
//                  Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
