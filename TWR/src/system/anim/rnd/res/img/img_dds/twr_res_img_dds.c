/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_dds.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images in format DDS.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 31.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>
#include <ddraw.h>

/* The rendering image in format DDS saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveDDS( twrIMAGE *Img, CHAR *FileName )
{
  DWORD Sign = *(DWORD *)"DDS ";
  DDSURFACEDESC2 DDS = {0};
  FILE *F;
  INT w, h;

  if ((F = fopen(FileName, "wb")) != NULL)
    return FALSE;
  /* Fill DDS info */
  memset(&DDS, 0, sizeof(DDS));
  DDS.dwMipMapCount = 1;
  DDS.dwFlags |= DDSD_MIPMAPCOUNT;
  DDS.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;
  DDS.ddpfPixelFormat.dwBBitMask = 0x00FF0000;
  DDS.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
  DDS.ddpfPixelFormat.dwRBitMask = 0x000000FF;
  DDS.dwSize = sizeof(DDS);
  DDS.dwWidth = Img->W;
  DDS.dwHeight = Img->H;
  DDS.ddpfPixelFormat.dwSize = 32;
  DDS.ddpfPixelFormat.dwRGBBitCount = 32;
  DDS.dwFlags = DDSD_PIXELFORMAT | DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS | DDSD_PITCH;
  DDS.ddpfPixelFormat.dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;

  /* Write header into file */
  fwrite(&Sign, 4, 1, F);
  fwrite(&DDS, sizeof(DDS), 1, F);

  /* Evaluationg DDS image size */
  w = DDS.dwWidth;
  h = DDS.dwHeight;

  return TRUE;
} /* End of 'TWR_RndSaveDDS' function */

/* The rendering image loading at file format DDS program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadDDS( twrIMAGE *Img, CHAR *FileName )
{
  INT i, size = 0, w, h;
  FILE *F;
  DWORD Sign;
  DDSURFACEDESC2 DDS = {0};
  INT awd = strlen(FileName);
  UINT16 *MipLevel;
  twrIMAGE *Img1;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == FALSE)
    return FALSE;

  /* Read header of ".dds" file */
  fread(&Sign, 4, 1, F);
  fread(&DDS, 124, 1, F);

  /* Check for valid file dds info */
  if (Sign != *(DWORD *)"DDS " ||
      DDS.dwSize != sizeof(DDS) ||
      !(DDS.dwFlags & DDSD_HEIGHT) ||
      !(DDS.dwFlags & DDSD_WIDTH) ||
      !(DDS.dwFlags & DDSD_PIXELFORMAT) ||
      DDS.ddpfPixelFormat.dwSize != 32 ||
      !(DDS.ddpfPixelFormat.dwFlags & DDPF_RGB) ||
      DDS.ddpfPixelFormat.dwRGBBitCount != 32 ||
      (DDS.ddpfPixelFormat.dwBBitMask != 0x00FF0000 &&
       DDS.ddpfPixelFormat.dwBBitMask != 0x000000FF) ||
      DDS.ddpfPixelFormat.dwGBitMask != 0x0000FF00 ||
      (DDS.ddpfPixelFormat.dwRBitMask != 0x000000FF &&
       DDS.ddpfPixelFormat.dwRBitMask != 0x00FF0000) ||
      DDS.ddpfPixelFormat.dwRGBAlphaBitMask != 0xFF000000)
  {
    fclose(F);
    return FALSE;
  }

  /* Introduct mipmap level */
  MipLevel = (UINT16 *)DDS.dwMipMapCount;

  /* Create main picture and fill it pixel color info */
  size += DDS.dwWidth * DDS.dwHeight;
  Twr->ImgCreate(Img, DDS.dwWidth, DDS.dwHeight);
  fread(Img->Pixels, 1, 4 * size, F);

  /* Correct color info (ABGR8 -> RGBA8) */
  if (DDS.ddpfPixelFormat.dwBBitMask == 0x00FF0000)
    for (i = 0; i < size; i++)
      Img->Pixels[i] = ((Img->Pixels[i] >> 16) & 0xFF) |
                     (((Img->Pixels[i] >> 8) & 0xFF) << 8) |
                     ((Img->Pixels[i] & 0xFF) << 16) |
                     (((Img->Pixels[i] >> 24) & 0xFF) << 24);

  /* For mip-maps place */
  fseek(F, 128 + DDS.dwWidth * DDS.dwHeight * 4, SEEK_SET);

  /* Evaluation of file size */
  Img1 = malloc(sizeof(twrIMAGE) * (INT)DDS.dwMipMapCount);
  w = DDS.dwWidth, h = DDS.dwHeight;
  for (i = 0; i < (INT)DDS.dwMipMapCount; i++)
  {
    w = COM_MAX(1, (w / 2));
    h = COM_MAX(1, (h / 2));

    /* Read pixel info from file to mipmap array */
    Twr->ImgCreate(&Img1[i], w, h);
    fread(Img1[i].Pixels, 1, 4 * w * h, F);

    /* Correct color info (ABGR8 -> RGBA8) */
    if (DDS.ddpfPixelFormat.dwBBitMask == 0x00FF0000)
    {
      INT j;

      for (j = 0; j < w * h; j++)
        Img->Pixels[j] = ((Img->Pixels[j] >> 16) & 0xFF) |
                     (((Img->Pixels[j] >> 8) & 0xFF) << 8) |
                     ((Img->Pixels[j] & 0xFF) << 16) |
                     (((Img->Pixels[j] >> 24) & 0xFF) << 24);
    }
  }
  return TRUE;
} /* End of 'TWR_RndLoadDDS' function */

/* END OF 'twr_res_img_dds.c' FILE */
