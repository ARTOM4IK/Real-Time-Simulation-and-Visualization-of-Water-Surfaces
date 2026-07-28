/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with universal images functions.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_res_img.h"

#include <string.h>

/* The rendering image create program function.
 * ARGUMENTS:
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 *   - height and weight of picture:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_ImgCreate( twrIMAGE *Img, INT W, INT H )
{
  memset(Img, 0, sizeof(twrIMAGE));
  if ((Img->Pixels = malloc(sizeof(DWORD) * W * H)) == NULL)
    return FALSE;
  memset(Img->Pixels, 0, sizeof(DWORD) * W * H);
  Img->W = W;
  Img->H = H;
  return TRUE;
} /* End of 'TWR_ImgCreate' function */

/* The rendering image loading at file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_ImgLoad( twrIMAGE *Img, CHAR *FileName )
{
  memset(Img, 0, sizeof(twrIMAGE));
  if (TWR_RndLoadG24(Img, FileName))
    return TRUE;
  else if (TWR_RndLoadG32(Img, FileName))
    return TRUE;
  else if (TWR_RndLoadBMP(Img, FileName))
    return TRUE;
  else if (TWR_RndLoadTGA(Img, FileName))
    return TRUE;
  else if (TWR_RndLoadPNG(Img, FileName))
    return TRUE;
  else if (TWR_RndLoadJPG(Img, FileName))
    return TRUE;
  else if (TWR_RndLoadDDS(Img, FileName))
    return TRUE;
  return FALSE;
} /* End of 'TWR_ImgLoad' function */

/* The rendering image free program function.
 * ARGUMENTS:
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: None.
 */
VOID TWR_ImgFree( twrIMAGE *Img )
{
  free(Img->Pixels);
  memset(Img, 0, sizeof(twrIMAGE));
} /* End of 'TWR_ImgFree' function */

/* Image system initialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemImageInit( VOID )
{
  Twr->ImgCreate = TWR_ImgCreate;
  Twr->ImgFree = TWR_ImgFree;
  Twr->ImgLoad = TWR_ImgLoad;
  Twr->ImgSaveG24 = TWR_RndSaveG24;
  Twr->ImgSaveG32 = TWR_RndSaveG32;
  Twr->ImgSaveTGA = TWR_RndSaveTGA;
  Twr->ImgSaveDDS = TWR_RndSaveDDS;
  Twr->ImgSaveBMP = TWR_RndSaveBMP;
  Twr->ImgSavePNG = TWR_RndSavePNG;
  Twr->ImgSaveJPG = TWR_RndSaveJPG;
} /* End of 'TWR_SystemImageInit' function */

/* Image system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemImageClose( VOID )
{
} /* End of 'TWR_SystemImageClose' function */

/* END OF 'twr_res_img.c' FILE */
