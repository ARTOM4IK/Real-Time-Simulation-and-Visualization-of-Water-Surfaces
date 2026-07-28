/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_png.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images in format PNG.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 05.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include "pnglib/png.h"

/* The rendering image in format PNG saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSavePNG( twrIMAGE *Img, CHAR *FileName )
{
  INT i;
  FILE *F;
  png_bytep *Rows = NULL;
  png_structp png_ptr;
  png_infop pif;

  if ((F = fopen(FileName, "wb")) == NULL)
    return FALSE;

  if ((png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  if ((pif = png_create_info_struct(png_ptr)) == NULL)
  {
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(F);
    return FALSE;
  }

  if ((Rows = malloc(sizeof(png_bytep) * Img->H)) == NULL)
  { 
    png_destroy_write_struct(&png_ptr, &pif);
    fclose(F);
    return FALSE;
  }
  for (i = 0; i < Img->H; i++)
    Rows[i] = (png_bytep)(Img->Pixels + i * Img->W);

  png_set_IHDR(png_ptr, pif,
               Img->W, Img->H,           /* width and height */
               8,                        /* bit depth */
               PNG_COLOR_TYPE_RGB_ALPHA, /* color type */
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_set_bgr(png_ptr);
  png_init_io(png_ptr, F);
  png_set_rows(png_ptr, pif, (png_bytep *)Rows);

  png_write_png(png_ptr, pif, PNG_TRANSFORM_IDENTITY, NULL);
  png_write_end(png_ptr, pif);
  png_destroy_write_struct(&png_ptr, &pif);
  free(Rows);
  fclose(F);
  return TRUE;
} /* End of 'TWR_RndSavePNG' function */

/* The rendering image loading at file format PNG program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadPNG( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;
  CHAR Help[4];
  png_image I = {0};
  UINT components;
  BYTE *B3 = (BYTE *)Img->Pixels;
  INT x, y;
  static DWORD Pal[1024];

  memset(Img, 0, sizeof(twrIMAGE));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  fread(Help, 1, 4, F);
  fclose(F);

  I.version = PNG_IMAGE_VERSION;
  if (!png_image_begin_read_from_file(&I, FileName))
    return FALSE;

  components = PNG_IMAGE_PIXEL_CHANNELS(I.format);

  Twr->ImgCreate(Img, I.width, I.height);

  if (!png_image_finish_read(&I, NULL, Img->Pixels, 0, Pal))
    return FALSE;

  B3 = (VOID *)Img->Pixels;

  if (components == 3)
    for (y = Img->H - 1; y >= 0; y--)
      for (x = Img->W - 1; x >= 0; x--)
      {
        BYTE
          r = B3[(x + y * Img->W) * 3 + 0],
          g = B3[(x + y * Img->W) * 3 + 1],
          b = B3[(x + y * Img->W) * 3 + 2];

        Img->Pixels[x + y * Img->W] = COM_MAKELONG0123(b, g, r, 0xFF);
      }
  else if (components == 4)
    for (y = Img->H - 1; y >= 0; y--)
      for (x = Img->W - 1; x >= 0; x--)
      {
        DWORD c = Img->Pixels[x + y * Img->W];
        BYTE
          r = COM_LONGBYTE0(c),
          g = COM_LONGBYTE1(c),
          b = COM_LONGBYTE2(c),
          a = COM_LONGBYTE3(c);

        Img->Pixels[x + y * Img->W] = COM_MAKELONG0123(b, g, r, a);
      }
  else if (components == 1)
    for (y = Img->H - 1; y >= 0; y--)
      for (x = Img->W - 1; x >= 0; x--)
      {
        DWORD
          m = B3[x + y * Img->W];
        BYTE
          r = COM_LONGBYTE0(Pal[m]),
          g = COM_LONGBYTE1(Pal[m]),
          b = COM_LONGBYTE2(Pal[m]),
          a = COM_LONGBYTE3(Pal[m]);

        Img->Pixels[x + y * Img->W] = COM_MAKELONG0123(b, g, r, a);
      }
  return TRUE;
} /* End of 'TWR_RndLoadPNG' function */

/* END OF 'twr_res_img_png.c' FILE */
