/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_bmp.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images in format BMP.
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

/* File 2-bytes prefix ("BM") (see dibBITMAPFILEHEADER) */
#define DIB_FILEPREFIX ('B' | ('M' << 8))

/* The rendering image in format BMP saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveBMP( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;
  BITMAPFILEHEADER FH = {0};
  BITMAPINFOHEADER IH = {0};
  INT x, y;

  if ((F = fopen(FileName, "wb")) != NULL)
  {
    /* File Header */
    FH.bfType = DIB_FILEPREFIX;
    FH.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * Img->H * Img->W;
    FH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fwrite(&FH, sizeof(BITMAPFILEHEADER), 1, F);
    /* Information Header */
    IH.biSize = 40;
    IH.biWidth = Img->W;
    IH.biHeight = Img->H;
    IH.biPlanes = 1;
    IH.biBitCount = 32;
    IH.biSizeImage = sizeof(DWORD) * Img->H * Img->W;
    fwrite(&IH, sizeof(BITMAPINFOHEADER), 1, F);
    for (y = Img->H - 1; y >= 0; y--)
      for (x = 0; x < Img->W; x++)
        fwrite(&Img->Pixels[x + y * Img->W], 4, 1, F);
    fclose(F);
    return TRUE;
  }
  return FALSE;
} /* End of 'TWR_RndSaveBMP' function */

/* The rendering image loading at file format BMP program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadBMP( twrIMAGE *Img, CHAR *FileName )
{
  HBITMAP hBm;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    INT i;
    HBITMAP hDIB;
    BITMAP bm;
    BITMAPINFOHEADER bmih = {0};
    DWORD *Bits;
    HDC hMemDC1, hMemDC2;

    /* Get load image info */
    GetObject(hBm, sizeof(bm), &bm);

    /* Prepare target DIB image */
    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biPlanes = 1;
    bmih.biBitCount = 32;
    bmih.biCompression = BI_RGB;
    bmih.biWidth = bm.bmWidth;
    bmih.biHeight = -bm.bmHeight;
    bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;

    hDIB = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, &Bits, NULL, 0);
    hMemDC1 = CreateCompatibleDC(Twr->hRndDC);
    SelectObject(hMemDC1, hBm);
    hMemDC2 = CreateCompatibleDC(Twr->hRndDC);
    SelectObject(hMemDC2, hDIB);

    /* Draw load image */
    BitBlt(hMemDC2, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCCOPY);

    /* Store im TWR image */
    if (TWR_ImgCreate(Img, bm.bmWidth, bm.bmHeight))
    {
      memcpy(Img->Pixels, Bits, bm.bmWidth * bm.bmHeight * 4);
      /* Setup alpha-channel */
      if (bm.bmBitsPixel != 32)
        for (i = Img->W * Img->H - 1; i >= 0; i--)
          Img->Pixels[i] |= 0xFF000000;
    }
    DeleteDC(hMemDC1);
    DeleteDC(hMemDC2);
    DeleteObject(hBm);
    DeleteObject(hDIB);
    return TRUE;
  }
  return FALSE;
} /* End of 'TWR_RndLoadBMP' function */

/* END OF 'twr_res_img_bmp.c' FILE */
