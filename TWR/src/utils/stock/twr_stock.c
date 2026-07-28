/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_stock.c
 * PURPOSE     : Tough Water Rendering project.
 *               Stock system implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 25.04.2025
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/*** Data structures type definitions ***/

/* Entry state flags */
typedef enum
{
  STATE_FREE,      /* Free */
  STATE_OCCUPIED,  /* Occupied */
  STATE_TO_DELETE, /* Marked to delete */
} STATE_FLAG;

/* Element entry structure type definition */
struct tagENTRY
{
  STOCK *Stk;         /* Parent stock pointer */
  ENTRY 
    *Prev,            /* Previous entry pointer */
    *Next;            /* Next entry pointer */
  STATE_FLAG State;   /* State flag: free, occupied or to delete */
  BYTE ElementData[]; /* Elements array */
};

/* Block structure type definition */
struct tagBLOCK
{
  BLOCK
    *Prev,        /* Previous block pointer */
    *Next;        /* Next block pointer */
  BYTE Entries[]; /* Entries array */
};

/*** Stock system handle ***/

STOCK
  StockStocks,  /* Stock of stocks */
  StockToStrip; /* Stock of stocks to strip */

/* Stock system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID StocksInit( VOID )
{
  Twr->Sys("Stock system is initialized");
  StockCreate(&StockStocks, "Stock of stocks", sizeof(STOCK *), 30, NULL);
  StockCreate(&StockToStrip, "Stock of stocks to strip", sizeof(STOCK *), 18, NULL);
} /* End of 'StocksInit' function */

/* Stock system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID StocksClose( VOID )
{
  StockFree(&StockToStrip);
  StockFree(&StockStocks);
  Twr->Sys("Stock system is deinitialized");
} /* End of 'StocksClose' function */

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
                  VOID (*DeleteStrategy)( VOID *Element ) )
{
  if (Stk == NULL || ElementSize < 1 || BlockSize < 2)
  {
    Twr->Error("Stock creation failed: %s", Name);
    return;
  }
  /* Fill default fields */
  strncpy(Stk->Name, Name, TWR_STR_MAX - 1);
  Stk->ElementSize = ElementSize;
  Stk->BlockSize = BlockSize;
  Stk->FirstBlock = NULL;
  Stk->FirstFree = Stk->FirstOccupied = Stk->FirstToDelete = NULL;
  Stk->NumOfFree = Stk->NumOfOccupied = Stk->NumOfToDelete = 0;
  Stk->DeleteStrategy = DeleteStrategy;
  /* Add to stock of stocks */
  if (Stk != &StockStocks && Stk != &StockToStrip)
  {
    STOCK **s = StockAlloc(&StockStocks);

    *s = Stk;
  }
  Twr->Info("Stock creation success: ");
  Twr->SetColor(0x88FFFF, 0);
  Twr->Log("%s", Stk->Name);
} /* End of 'StockCreate' function */

/* Free stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockFree( STOCK *Stk )
{
  BLOCK *blk;

  if (Stk == NULL)
    return;
  if ((blk = Stk->FirstBlock) != NULL)
  {
    /* Free every block memory */
    blk = Stk->FirstBlock;
    StockDeleteAllInstant(Stk);
    StockStrip(Stk);
    blk = Stk->FirstBlock;
    while (blk->Next != NULL)
    {
      blk = blk->Next;
      free(blk->Prev);
    }
    free(blk);
  }
  Stk->NumOfFree = Stk->NumOfOccupied = Stk->NumOfToDelete = 0;
  Stk->FirstFree = Stk->FirstOccupied = Stk->FirstToDelete = NULL;
  Twr->Info("Stock closing success: ");
  Twr->SetColor(0x88FFFF, 0);
  Twr->Log("%s", Stk->Name);
} /* End of 'StockFree' function */

/* Helpful entry offset pointer macros */
#define ENTRY_POINTER_OFFSET(N) ((ENTRY *)(blk->Entries + (N) * (sizeof(ENTRY) + Stk->ElementSize)))

/* Add new element to stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS:
 *   (VOID *) element pointer.
 */
VOID * StockAlloc( STOCK *Stk )
{
  INT i;
  ENTRY *ptr, *blk_entry;

  if (Stk == NULL)
    return NULL;
  if (Stk->FirstFree == NULL)
  {
    /* Creating new data block */
    INT size = sizeof(BLOCK) + Stk->BlockSize * (sizeof(ENTRY) + Stk->ElementSize);
    BLOCK *blk = malloc(size);

    if (blk == NULL)
    {
      Twr->Error("Stock not enough memory: %s", Stk->Name);
      return NULL;
    }
    memset(blk, 0, size);
    blk->Prev = NULL;
    blk->Next = Stk->FirstBlock;
    if (Stk->FirstBlock != NULL)
      Stk->FirstBlock->Prev = blk;
    Stk->FirstBlock = blk;
    /* Filling entries */
    for (i = 0; i < Stk->BlockSize; i++)
    {
      blk_entry = ENTRY_POINTER_OFFSET(i);
      blk_entry->Stk = Stk;
      blk_entry->State = STATE_FREE;
      if (i == 0)
        blk_entry->Prev = NULL;
      else
        blk_entry->Prev = ENTRY_POINTER_OFFSET(i - 1);
      if (i == Stk->BlockSize - 1)
        blk_entry->Next = NULL;
      else
        blk_entry->Next = ENTRY_POINTER_OFFSET(i + 1);
    }
    Stk->FirstFree = ENTRY_POINTER_OFFSET(0);
    Stk->NumOfFree += Stk->BlockSize;
  }
  /* Moving first free entry to occupied */
  ptr = Stk->FirstFree;
  ptr->State = STATE_OCCUPIED;
  Stk->FirstFree = Stk->FirstFree->Next;
  Stk->NumOfFree--;
  if (Stk->FirstFree != NULL)
    Stk->FirstFree->Prev = NULL;
  ptr->Next = Stk->FirstOccupied;
  if (Stk->FirstOccupied != NULL)
    Stk->FirstOccupied->Prev = ptr;
  Stk->FirstOccupied = ptr;
  Stk->NumOfOccupied++;
  return (VOID *)((INT)ptr + sizeof(ENTRY));
} /* End of 'StockAlloc' function */

/* Mark element to delete from stock function.
 * ARGUMENTS:
 *   - element pointer:
 *       VOID *Element;
 * RETURNS: None.
 */
VOID StockDelete( VOID *Element )
{
  if (Element != NULL)
  {
    ENTRY
      *ptr = (ENTRY *)((INT)Element - sizeof(ENTRY)),
      *prev = ptr->Prev,
      *next = ptr->Next;
    STOCK *stk = ptr->Stk;

    if (ptr->State != STATE_OCCUPIED)
    {
      Twr->Error("Element deleted repeatedly: %s", stk->Name);
      return;
    }
    stk->NumOfOccupied--;
    /* Move to marked list */
    if (prev != NULL)
      prev->Next = next;
    else
      stk->FirstOccupied = stk->FirstOccupied->Next;
    if (next != NULL)
      next->Prev = prev;
    ptr->Prev = NULL;
    ptr->Next = stk->FirstToDelete;
    if (stk->FirstToDelete != NULL)
      stk->FirstToDelete->Prev = ptr;
    stk->FirstToDelete = ptr;
    ptr->State = STATE_TO_DELETE;
    stk->NumOfToDelete++;
  }
} /* End of 'StockDelete' function */

/* Delete element from stock function.
 * ARGUMENTS:
 *   - element pointer:
 *       VOID *Element;
 * RETURNS: None.
 */
VOID StockDeleteInstant( VOID *Element )
{
  if (Element != NULL)
  {
    ENTRY
      *ptr = (ENTRY *)((INT)Element - sizeof(ENTRY)),
      *prev = ptr->Prev,
      *next = ptr->Next;
    STOCK *stk = ptr->Stk;

    if (ptr->State != STATE_OCCUPIED)
    {
      Twr->Error("Element deleted repeatedly: %s [%.50s]", stk->Name, ptr->ElementData);
      return;
    }
    stk->NumOfOccupied--;
    /* Move to free list */
    if (prev != NULL)
      prev->Next = next;
    else
      stk->FirstOccupied = stk->FirstOccupied->Next;
    if (next != NULL)
      next->Prev = prev;
    ptr->Prev = NULL;
    ptr->Next = stk->FirstFree;
    /* Delete function call */
    if (stk->DeleteStrategy != NULL)
      stk->DeleteStrategy((VOID *)ptr->ElementData);
    if (stk->FirstFree != NULL)
      stk->FirstFree->Prev = ptr;
    stk->FirstFree = ptr;
    ptr->State = STATE_FREE;
    stk->NumOfFree++;
  }
} /* End of 'StockDeleteInstant' function */

/* Delete all marked elements from stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk; 
 * RETURNS: None.
 */
VOID StockStrip( STOCK *Stk )
{
  ENTRY *entry;

  if (Stk == NULL || (entry = Stk->FirstToDelete) == NULL)
    return;
  /* Change state of marked entries to free */
  if (Stk->DeleteStrategy == NULL)
    while (entry->Next != NULL)
    {
      entry->State = STATE_FREE;
      entry = entry->Next;
    }
  else
  {
    while (entry->Next != NULL)
    {
      /* Call delete element function */
      Stk->DeleteStrategy((VOID *)entry->ElementData);
      entry->State = STATE_FREE;
      entry = entry->Next;
    }
    Stk->DeleteStrategy((VOID *)entry->ElementData);
  }
  entry->State = STATE_FREE;
  entry->Next = Stk->FirstFree;
  /* Add as first free */
  if (Stk->FirstFree != NULL)
    Stk->FirstFree->Prev = entry;
  Stk->FirstFree = entry;
  Stk->FirstToDelete = NULL;
  /* Recalculate counters */
  Stk->NumOfFree += Stk->NumOfToDelete;
  Stk->NumOfToDelete = 0;
} /* End of 'StockStrip' function */

/* Erase all elements from stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockDeleteAll( STOCK *Stk )
{
  ENTRY *ptr;

  if (Stk == NULL)
    return;
  while (Stk->FirstOccupied != NULL)
  {
    ptr = Stk->FirstOccupied;
    Stk->FirstOccupied = Stk->FirstOccupied->Next;
    Stk->NumOfOccupied--;
    if (Stk->FirstOccupied != NULL)
      Stk->FirstOccupied->Prev = NULL;
    ptr->Next = Stk->FirstToDelete;
    if (Stk->FirstToDelete != NULL)
      Stk->FirstToDelete->Prev = ptr;
    Stk->FirstToDelete = ptr;
    ptr->State = STATE_TO_DELETE;
    Stk->NumOfToDelete++;
  }
} /* End of 'StockDeleteAll' function */

/* Erase all elements instantly from stock function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockDeleteAllInstant( STOCK *Stk )
{
  ENTRY *entry;

  if (Stk == NULL || (entry = Stk->FirstOccupied) == NULL)
    return;
  /* Change state of occupied entries to free */
  if (Stk->DeleteStrategy == NULL)
    while (entry->Next != NULL)
    {
      entry->State = STATE_FREE;
      entry = entry->Next;
    }
  else
  {
    while (entry->Next != NULL)
    {
      /* Call delete element function */
      Stk->DeleteStrategy((VOID *)entry->ElementData);
      entry->State = STATE_FREE;
      entry = entry->Next;
    }
    Stk->DeleteStrategy((VOID *)entry->ElementData);
  }
  entry->State = STATE_FREE;
  entry->Next = Stk->FirstFree;
  /* Add as first free */
  if (Stk->FirstFree != NULL)
    Stk->FirstFree->Prev = entry;
  Stk->FirstFree = entry;
  Stk->FirstOccupied = NULL;
  /* Recalculate counters */
  Stk->NumOfFree += Stk->NumOfOccupied;
  Stk->NumOfOccupied = 0;
} /* End of 'StockDeleteAllInstant' function */

/* Check stock contains element function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 *   - element pointer:
 *       VOID *Element;
 * RETURNS: 
 *   (BOOL) TRUE - if stock contains the element, otherwise - FALSE.
 */
BOOL StockContains( STOCK *Stk, VOID *Element )
{
  ENTRY *ptr;

  if (Stk == NULL || Element == NULL)
    return FALSE;
  /* Check every occupied entry */
  ptr = Stk->FirstOccupied;
  while (ptr != NULL)
  {
    if ((VOID *)ptr->ElementData == Element && ptr->State != STATE_FREE)
      return TRUE;
    ptr = ptr->Next;
  }
  return FALSE;
} /* End of 'StockContains' function */

/* Get first stock element function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS:
 *   (VOID *) first element pointer.
 */
VOID * StockFirst( STOCK *Stk )
{
  ENTRY *entry;

  if (Stk == NULL || (entry = Stk->FirstOccupied) == NULL)
    return NULL;
  while (entry->Next != NULL)
    entry = entry->Next;
  return (VOID *)((INT)entry + sizeof(ENTRY));
} /* End of 'StockFirst' function */

/* Get next stock element function.
 * ARGUMENTS:
 *   - current element pointer:
 *       VOID *Element;
 * RETURNS:
 *   (VOID *) next element pointer.
 */
VOID * StockNext( VOID *Element )
{
  ENTRY *entry;

  if (Element == NULL)
    return NULL;
  entry = (ENTRY *)((INT)Element - sizeof(ENTRY));
  if (entry->Prev == NULL)
    return NULL;
  return (VOID *)((INT)entry->Prev + sizeof(ENTRY));
} /* End of 'StockNext' function */

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
VOID * StockFind( STOCK *Stk, VOID *FindFunc, VOID *FindData )
{
  ENTRY *ptr;

  if (Stk == NULL || FindFunc == NULL)
    return NULL;
  ptr = Stk->FirstOccupied;
  while (ptr != NULL)
  {
    if (((BOOL (*)(VOID *, VOID *))FindFunc)((VOID *)ptr->ElementData, FindData))
      return (VOID *)ptr->ElementData;
    ptr = ptr->Next;
  }
  return NULL;
} /* End of 'StockFind' function */

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
VOID StockWalk( STOCK *Stk, VOID *WalkFunc, VOID *WalkData )
{
  ENTRY *ptr;

  if (Stk == NULL || WalkFunc == NULL)
    return;
  ptr = Stk->FirstOccupied;
  while (ptr != NULL)
  {
    ((VOID (*)(VOID *, VOID *))WalkFunc)((VOID *)ptr->ElementData, WalkData);
    ptr = ptr->Next;
  }
  return;
} /* End of 'StockWalk' function */

/* END OF 'twr_stock.c' FILE */
