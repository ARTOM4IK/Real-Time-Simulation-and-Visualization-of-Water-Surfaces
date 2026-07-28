/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_dict.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit dictionary inplementation module.
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
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "units/twr_units.h"

/*** Unit creation function references ***/

/* Sample units */
twrUNIT * TWR_UnitCreateSampleMK3( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleAA4( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleVN4( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleIK4( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleAB7( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleMN7( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleVP7( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleAT7( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleVG4( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleVG4Frustum( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSampleUI( twrPARAMS *Params );
/** Test units */
twrUNIT * TWR_UnitCreateTestAB7( twrPARAMS *Params );
/* System units */
twrUNIT * TWR_UnitCreateControl( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateNormalMapping( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateStillWater( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateFluid( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateSkybox( twrPARAMS *Params );
twrUNIT * TWR_UnitCreateProjectedGrid( twrPARAMS *Params );

/* Unit dictionary */
static struct
{
  CHAR *Name;                                       /* Unit identification name */
  twrUNIT * (*UnitCreateFunc)( twrPARAMS *Params ); /* Unit create function pointer */
} TWR_UnitDictionary[] =
{
  {"Control", TWR_UnitCreateControl},
  {"MK3 unit", TWR_UnitCreateSampleMK3},
  {"AA4 unit", TWR_UnitCreateSampleAA4},
  {"VN4 unit", TWR_UnitCreateSampleVN4},
  {"IK4 unit", TWR_UnitCreateSampleIK4},
  {"AB7 unit", TWR_UnitCreateSampleAB7},
  {"MN7 unit", TWR_UnitCreateSampleMN7},
  {"VP7 unit", TWR_UnitCreateSampleVP7},
  {"AT7 unit", TWR_UnitCreateSampleAT7},
  {"VG4 unit", TWR_UnitCreateSampleVG4},
  {"VG4 unit frustum", TWR_UnitCreateSampleVG4Frustum},
  {"UI unit", TWR_UnitCreateSampleUI},
  {"AB7 test", TWR_UnitCreateTestAB7},
  {"Normal mapping test unit", TWR_UnitCreateNormalMapping},
  {"Still water unit", TWR_UnitCreateStillWater},
  {"Phys water unit", TWR_UnitCreateFluid},
  {"Projected grid water unit", TWR_UnitCreateProjectedGrid},
};

/* Unit dictionary length */
#define TWR_UNIT_DICTIONARY_LENGTH (sizeof(TWR_UnitDictionary) / sizeof(TWR_UnitDictionary[0]))

/* Get unit number in dictionary function.
 * ARGUMENTS:
 *   - unit identification name:
 *       CHAR *Name;
 * RETURNS: 
     (INT) number of unit in units dictionary (-1 if no unit).
 */
INT TWR_GetUnitNum( CHAR *Name )
{
  INT k;

  for (k = 0; k < TWR_UNIT_DICTIONARY_LENGTH; k++)
  {
    if (strstr(TWR_UnitDictionary[k].Name, Name) != NULL && Name[0] != '\0')
    {
      return k;
    }
  }
  return -1;
} /* End of 'TWR_GetUnitNum' function */

/* Create unit by its number in dictionary function.
 * ARGUMENTS:
 *   - unit number in dictionary:
 *       INT Num;
 * RETURNS: None.
 */
VOID TWR_UnitAddByNum( INT Num )
{
  if (Num >= 0 && Num < TWR_UNIT_DICTIONARY_LENGTH)
    Twr->UnitAdd(TWR_UnitDictionary[Num].UnitCreateFunc(NULL));
} /* End of 'TWR_UnitAddByNum' function */

/* Create unit by dictionary name function.
 * ARGUMENTS:
 *   - unit identification name:
 *       CHAR *Name;
 * RETURNS: None.
 */
VOID TWR_UnitAddByName( CHAR *Name )
{
  INT i;

  for (i = 0; i < TWR_UNIT_DICTIONARY_LENGTH; i++)
    if (strcmp(TWR_UnitDictionary[i].Name, Name) == 0)
      Twr->UnitAdd(TWR_UnitDictionary[i].UnitCreateFunc(NULL));
} /* End of 'TWR_UnitAddByName' function */

/* END OF 'twr_dict.c' FILE */
