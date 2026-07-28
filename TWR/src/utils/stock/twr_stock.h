/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_stock.h
 * PURPOSE     : Tough Water Rendering project.
 *               Stock system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 11.04.2025
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_stock_h_
#define __twr_stock_h_

#include "twr_def.h"

/* Stock structure type definition */
typedef struct tagSTOCK STOCK;
typedef struct tagBLOCK BLOCK;
typedef struct tagENTRY ENTRY;

/* Stock structure type definition */
struct tagSTOCK
{
  CHAR Name[TWR_STR_MAX]; /* Stock name */
  BLOCK *FirstBlock;      /* Block array */
  INT
    NumOfFree,            /* Free entry count  */
    NumOfOccupied,        /* Occupied entry count */
    NumOfToDelete;        /* To delete entry count */
  ENTRY
    *FirstFree,           /* First free entry */
    *FirstOccupied,       /* First occupied entry */
    *FirstToDelete;       /* First to delete entry */
  INT
    ElementSize,          /* Element size in bytes */
    BlockSize;            /* Number of entries in block */
  /* Pre-delete element call function pointer */
  VOID (*DeleteStrategy)( VOID *Element );
};

/*** Stock system handle ***/

/* Stock of stocks */
extern STOCK StockStocks;
/* Stock of stocks to strip */
extern STOCK StockToStrip;

/* Stock system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID StocksInit( VOID );

/* Stock system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID StocksClose( VOID );

/*** Stock handle functions ***/

/* Create stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 *   - stock name:
 *       CHAR *Name;
 *   - element size in bytes:
 *       INT ElementSize;
 *   - block size (number of elements):
 *       INT BlockSize;
 *   - pre-delete element call function pointer:
 *       VOID (*DeleteStrategy)( VOID *Element );
 * RETURNS: None.
 */
VOID StockCreate( STOCK *Stk, CHAR *Name, INT ElementSize, INT BlockSize,
                  VOID (*DeleteStrategy)( VOID *Element ) );

/* Free stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockFree( STOCK *Stk );

/* Add new element to stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS:
 *   (VOID *) element pointer.
 */
VOID * StockAlloc( STOCK *Stk );

/* Mark element to delete from stock function.
 * ARGUMENTS:
 *   - element pointer:
 *       VOID *Element;
 * RETURNS: None.
 */
VOID StockDelete( VOID *Element );

/* Delete element from stock function.
 * ARGUMENTS:
 *   - element pointer:
 *       VOID *Element;
 * RETURNS: None.
 */
VOID StockDeleteInstant( VOID *Element );

/* Erase all elements from stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockDeleteAll( STOCK *Stk );

/* Erase all elements instantly from stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockDeleteAllInstant( STOCK *Stk );

/* Delete all marked elements from stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk; 
 * RETURNS: None.
 */
VOID StockStrip( STOCK *Stk );

/* Check stock contains element function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 *   - element pointer:
 *       VOID *Element;
 * RETURNS: 
 *   (BOOL) TRUE - if stock contains the element, otherwise - FALSE.
 */
BOOL StockContains( STOCK *Stk, VOID *Element );

/* Get first stock element function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS:
 *   (VOID *) first element pointer.
 */
VOID * StockFirst( STOCK *Stk );

/* Get next stock element function.
 * ARGUMENTS:
 *   - current element pointer:
 *       VOID *Element;
 * RETURNS:
 *   (VOID *) next element pointer.
 */
VOID * StockNext( VOID *Element );

/* Find element in stock by data function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 *   - for-each call function pointer:
 *       VOID *FindFunc;
 *   - find data pointer:
 *       VOID *FindData;
 * RETURNS:
 *   (VOID *) element pointer.
 */
VOID * StockFind( STOCK *Stk, VOID *FindFunc, VOID *FindData );

/* Call function for every element in stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 *   - for-each call function pointer:
 *       VOID *WalkFunc;
 *   - walk data pointer:
 *       VOID *WalkData;
 * RETURNS: None.
 */
VOID StockWalk( STOCK *Stk, VOID *WalkFunc, VOID *WalkData );

#endif /* __twr_stock_h_ */

/* END OF 'twr_stock.h' FILE */
