/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_anim.c
 * PURPOSE     : Tough Water Rendering project.
 *               Animation system inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "units/twr_units.h"

/* Animation resize function.
 * ARGUMENTS:
 *   - window width and height:
 *       INT W, H;
 * RETURNS: None.
 */
static VOID TWR_AnimResize( INT W, INT H )
{
  Twr->RndResize(W, H);
} /* End of 'TWR_AnimResize' function */

/* Animation one frame render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_AnimRender( VOID )
{
  twrUNIT **UniPtr;
  STOCK **stk;

  Twr->CurrentFrameNo++;
  TWR_ProfilerReset();
  TWR_ProfilerCollect("Frame start (total)");

  if (Twr->IsActive)
    Twr->InputResponse();
  else
    Twr->PrevInputIsActive = 0;
  Twr->TimerResponse();
  TWR_ProfilerCollect("After input/timer response");

  if (Twr->IsNextSceneToSet)
  {
    Twr->UsingSceneNum = (Twr->UsingSceneNum + 1) % Twr->NumOfScenes;
    if (strcmp(Twr->Scenes[Twr->UsingSceneNum].SceneName, "Common") == 0)
      Twr->UsingSceneNum = (Twr->UsingSceneNum + 1) % Twr->NumOfScenes;
    Twr->SceneSetByName(Twr->Scenes[Twr->UsingSceneNum].SceneName);
    Twr->IsNextSceneToSet = FALSE;
  }

  /* Call interframe response for every unit */
  if ((UniPtr = StockFirst(&Twr->StkUnits)) != NULL)
    do
    {
      (*UniPtr)->Response(*UniPtr);
    } while ((UniPtr = StockNext(UniPtr)) != NULL);
  TWR_ProfilerCollect("Response all units");

  Twr->RndStart();
  /* TWR_ProfilerCollect("Start frame draw"); */

  /* Call render for every unit */
  if ((UniPtr = StockFirst(&Twr->StkUnits)) != NULL)
    do
    {
      (*UniPtr)->Render(*UniPtr);
    } while ((UniPtr = StockNext(UniPtr)) != NULL);
  TWR_ProfilerCollect("Render all units");

  Twr->RndEnd();
  TWR_ProfilerCollect("End frame draw");
  TWR_ProfilerAddText("Global matrix count: %u", Twr->BuffersSize);

  /* Strip every stock in stock of strips */
  stk = StockFirst(&StockToStrip);
  while (stk != NULL)
  {
    StockStrip(*stk);
    stk = StockNext(stk);
  }
  /* Show total primitives statistics */
  TWR_ProfilerAddText("Total count of triangles: %u", Twr->TrianglesCount);
  stk = StockFirst(&StockStocks);
  /* Show every stock statistics */
  TWR_ProfilerAddText("+--------------------+--------+--------+--------+");
  TWR_ProfilerAddText("| Stocks (%2d total):|Occupied|  Free  |ToDelete|", StockStocks.NumOfOccupied);
  TWR_ProfilerAddText("+--------------------+--------+--------+--------+");
  stk = StockFirst(&StockStocks);
  while (stk != NULL)
  {
    TWR_ProfilerAddText("| %-18s | %6d | %6d | %6d |", (*stk)->Name, (*stk)->NumOfOccupied, (*stk)->NumOfFree, (*stk)->NumOfToDelete);
    stk = StockNext(stk);
  }
  TWR_ProfilerAddText("+--------------------+--------+--------+--------+");
} /* End of 'TWR_AnimRender' function */

/* Default unit handler function.
 * ARGUMENTS:
 *   - unit self pointer:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitHandler( twrUNIT *Uni )
{
} /* End of 'TWR_UnitHandler' function */

/* Add unit to animation function.
 * ARGUMENTS:
 *   - Size of unit to be added:
 *       UINT Size;
 * RETURNS:
 *   (twrUNIT *) Pointer to created unit.
 */
static twrUNIT * TWR_AnimUnitCreate( UINT Size )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(twrUNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  Uni->Init = TWR_UnitHandler;
  Uni->Close = TWR_UnitHandler;
  Uni->Response = TWR_UnitHandler;
  Uni->Render = TWR_UnitHandler;
  return Uni;
} /* End of 'TWR_AnimUnitCreate' function */

/* Units stock delete strategy function.
 * ARGUMENTS:
 *   - unit reference pointer:
 *       twrUNIT **UniPtr;
 * RETURNS: None.
 */
static VOID TWR_UniDeleteStrategy( twrUNIT **UniPtr )
{
  (*UniPtr)->Close(*UniPtr);
  free(*UniPtr);
} /* End of 'TWR_ShdDeleteStrategy' function */

/* Add unit to unit's array function.
 * ARGUMENTS:
 *   - Unit to be added:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_AnimUnitAdd( twrUNIT *Uni )
{
  twrUNIT **UniPtr = StockAlloc(&Twr->StkUnits);

  *UniPtr = Uni;
  Uni->Init(Uni);
} /* End of 'TWR_AnimUnitAdd' function */

/* End animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_AnimDoExit( VOID )
{
} /* End of 'TWR_DoExit' function */

/* Flip full screen function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */
static VOID TWR_AnimFlipFullScreen( VOID )
{
} /* End of 'TWR_FlipFullScreen' function */

/* Animation system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemAnimationInit( VOID )
{
  TWR_SystemWindowInit();
  TWR_SystemImageInit();
  TWR_SystemRenderInit();
  TWR_SystemInputInit();
  TWR_SystemTimerInit();
  TWR_SystemStillWaterInit();
  TWR_SystemScenesInit();

  /* Set pointers to anim functions */
  Twr->AnimResize = TWR_AnimResize;
  Twr->AnimRender = TWR_AnimRender;
  Twr->UnitCreate = TWR_AnimUnitCreate;
  Twr->UnitAdd = TWR_AnimUnitAdd;
  Twr->UnitAddByName = TWR_UnitAddByName;
  Twr->UnitAddByNum = TWR_UnitAddByNum;
  Twr->GetUnitNum = TWR_GetUnitNum;

  StockCreate(&Twr->StkUnits, "Units", sizeof(twrUNIT *), 30, TWR_UniDeleteStrategy);
  TWR_StockRegisterToStrip(&Twr->StkUnits);

  Twr->Sys("Animation system is initialized");
} /* End of 'TWR_SystemAnimationInit' function */

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemAnimationClose( VOID )
{
  TWR_SystemScenesClose();
  TWR_SystemStillWaterClose();
  TWR_SystemTimerClose();
  TWR_SystemInputClose();
  TWR_SystemRenderClose();
  TWR_SystemWindowClose();
  TWR_SystemImageClose();
  Twr->Sys("Animation system is deinitialized");
} /* End of 'TWR_SystemAnimationClose' function */

/* END OF 'twr_anim.c' FILE */
