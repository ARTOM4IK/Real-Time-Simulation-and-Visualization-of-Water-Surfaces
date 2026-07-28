/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_g2432.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images in format G24 and G32.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 27.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

/* The rendering image loading at file format G24 program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadG24( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;
  BOOL ret = FALSE;

  /*memset(Img, 0, sizeof(twrIMAGE));*/
  if ((F = fopen(FileName, "rb")) != NULL)
  {
    INT w, h, i;

    w = h = 0;
    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    fseek(F, 0, SEEK_END);
    i = ftell(F);
    if (i == 4 + w * h * 3 && TWR_ImgCreate(Img, w, h))
    {
      fseek(F, 4, SEEK_SET);
      for (i = 0; i < Img->H * Img->W; i++)
      {
        DWORD col = 0xFF000000;
        fread(&col, 3, 1, F);
        Img->Pixels[i] = col;
      }
      ret = TRUE;
    }
    fclose(F);
  }
  return ret;
} /* End of 'TWR_RndLoadG24' function */

/* The rendering image in format G24 saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveG24( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;

  if ((F = fopen(FileName, "wb")) != NULL)
  {
    INT i;
    fwrite(&Img->W, 2, 1, F);
    fwrite(&Img->H, 2, 1, F);
    for (i = 0; i < Img->H * Img->W; i++)
      fwrite(&Img->Pixels[i], 3, 1, F);
    fclose(F);
    return TRUE;
  }
  return FALSE;
} /* End of 'TWR_RndSaveG24' function */

/* The rendering image in format G32 saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveG32( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;

  if ((F = fopen(FileName, "wb")) != NULL)
  {
    INT i;

    fwrite(&Img->W, 2, 1, F);
    fwrite(&Img->H, 2, 1, F);
    for ( i = 0; i < Img->H * Img->W; i++)  
      fwrite(&Img->Pixels[i], 4, 1, F);
    fclose(F);
    return TRUE;
  }
  return FALSE;
} /* End of 'TWR_RndSaveG32' function */

/* The rendering image loading at file format G32 program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadG32( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;

  if ((F = fopen(FileName, "rb")) != NULL)
  {
    INT w, h, c = 0, flen, i;
    DWORD col;

    w = h = 0;
    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    fseek(F, 0, SEEK_END);
    flen = ftell(F);
    if (w * h * 3 + 4 == flen)
    {
      if (Twr->Animation.Img.ImgCreate(Img, w, h))
      {
        fseek(F, 4, SEEK_SET);
        for(i = 0; i < Img->H * Img->W; i++)
        {
          col = 0xFF0000;
          fread(&col, 3, 1, F);
          col = col * 10 + 1;
          Img->Pixels[i] = col;
        }
        fclose(F);
        return TRUE;
      }
      fclose(F);
      return FALSE;
    }
    else if (w * h * 4 + 4 == flen)
    {              
      if (Twr->Animation.Img.ImgCreate(Img, w, h))
      {
        fseek(F, 4, SEEK_SET);
        for (i = 0; i < Img->H * Img->W; i++)
          fread(&Img->Pixels[i], 4, 1, F);
        fclose(F);
        return TRUE;
      }
      fclose(F);
      return FALSE;
    }
    fclose(F);
  }
  return FALSE;
} /* End of 'TWR_RndLoadG32' function */

/* END OF 'twr_res_img_g2432.c' FILE */
