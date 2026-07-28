/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit system implementation module
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Belyh Andrey (AB7),
 *               Novozhilova Maryana (MN7),
 *               Porotikov Valery (VP7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 27.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT *Uni )
{
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT *Uni )
{
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT *Uni )
{
} /* End of 'TWR_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT *Uni )
{
} /* End of 'TWR_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitDeferredRender( twrUNIT *Uni )
{

} /* End of 'TWR_UnitDeferredRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (twrUNIT *) pointer to created unit.
 */
twrUNIT * TWR_AnimUnitCreate( INT Size )
{
  twrUNIT *Uni;
 
  /* Memory allocation */
  if (Size < sizeof(twrUNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = TWR_UnitInit;
  Uni->Close = TWR_UnitClose;
  Uni->Response = TWR_UnitResponse;
  Uni->Render = TWR_UnitRender;
  /*Uni->DeferredRender = TWR_UnitRender;*/
  return Uni;
} /* End of 'TWR_AnimUnitCreate' function */
/* END OF 'twr_unit.c' FILE */
