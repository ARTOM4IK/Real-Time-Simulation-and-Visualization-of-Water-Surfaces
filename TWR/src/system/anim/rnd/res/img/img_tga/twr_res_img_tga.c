/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_tga.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images in format TGA.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 04.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include <stdio.h>

#pragma pack(push, 1)

/***
 * TGA file structure:
 *   FileHeader.
 *   IDString.
 *   ColorMap (palette table).
 *   Image data.
 *  TGA v.2-00:
 *   Extensions data.
 *   FileFooter.
 ***/

/* Base image header, TGA v.1-00, placed at file begin */
typedef struct tagtgaFILEHEADER
{
  BYTE IDLength;     /* Length of ID string placed after header */
  BYTE ColorMapType; /* 1 - for color-mapped images */

  /* Image type:
   *   0 - no image
   *   1 - Uncompressed color-mapped image
   *   2 - Uncompressed RGB image
   *   3 - Uncompressed black-white image
   *   9 - Runlength encoded color-mapped image
   *  10 - Runlength encoded RGB image
   *  11 - Compressed black-white image
   *  32 - Compressed color-mapped image (Huffman, Delta, RLE)
   *  33 - Compressed color-mapped image (Huffman, Delta, RLE,
   *       4-pass quadtree-type process)
   */
  BYTE ImageType;

  /* Color-map (palette) data (ignored if 'ColorMapType' == 0) */
  WORD PaletteStart; /* First palette table (color-map) entry index */
  WORD PaletteSize;  /* Length of palette table in entries */
  BYTE PaletteEntrySize;  /* Length of palette table entry in bytes */

  /* Image data */
  WORD X, Y; /* Image origin (lower-left corner) coordinates */
  WORD Width, Height; /* Image size (in pixels) */
  BYTE BitsPerPixel; /* Pixel size in bits */

  /* Image description (by bits):
   *   bits 3-0   - amount of attributes bits for every pixel
   *   bits 4     - reserved (must be 0)
   *   bits 5     - screen origin: 0 - lower-left, 1 - upper-left
   *   bits 7-6   - data storage interleaving:
   *                  00 = non-interleaved
   *                  01 = two-way (even/odd) interleaving
   *                  10 = four way interleaving
   *                  11 = reserved
   */
  BYTE ImageDescr;

  /* After this header place ID string (length IDLength bytes),
   * than color-map table, than image data.
   * RLE compression format: <byte> <data>
   *    7 6 5 4 3 2 1 0 |
   *   '_'_'_'_'_'_'_'_'|d.a.t.a.
   *    ^ +-----------+ <- (repeat - 1) = [1..128]
   *    |
   *    +- if 1 than next data repeated color
   *       otherwise - next 'repeat' times color data is
   *       unpacked pixels
   */
} tgaFILEHEADER;

/* Footer TGA 2.00 identify string */
#define TGA_EXT_SIGNATURE "TRUEVISION-XFILE."

/* Image footer, TGA v.2-00, placed at end of file */
typedef struct tagtgaFILEFOOTER
{
  /* Extension area offset from file begin (0 - absent) */
  DWORD ExtensionOffset;
  /* Developer area offset from file begin (0 - absent) */
  DWORD DeveloperOffset;
  /* Signature of TGA - "TRUEVISION-XFILE.\x00",
   * see TGA_EXT_SIGNATURE */
  CHAR Signature[18];
} tgaFILEFOOTER;

/* Extension header */
typedef struct tagtgaEXTHEADER
{
  WORD Size;               /* Extension area size */
  CHAR AuthorName[41];     /* Author name */
  /* Author comments: 4 lines by 80 characters with NULLs */
  CHAR AuthorComment[324];
  /* Date/time of last file modification */
  WORD
    StampMonth, StampDay, StampYear,
    StampHour, StampMinute, StampSecond;
  CHAR JobName[41];        /* Name/ID of task, job */
  /* Job time */
  WORD
    JobHour, JobMinute, JobSecond;
  CHAR SoftwareID[41];     /* Software ID */
  WORD VersionNumber;      /* Software version number (* 100) */
  BYTE VersionLetter;      /* Software version letter */
  DWORD KeyColor;          /* Key, background color */
  /* Aspect ration Num./Denom. */
  WORD
    PixelNumerator, PixelDenominator;
  /* Gamma correction Num./Denom. */
  WORD
    GammaNumerator, GammaDenominator;
  /* Offset of color correction table:
   *   WORD Alpha, Red, Green, Blue;    x 256 (2048 bytes)
   *  black - 0, 0, 0, 0; white - 65535, 65535, 65535, 65535 */
  DWORD ColorOffset;
  /* Offset of stamp image: 2 bytes WxH, stamp data (the same
   * to image format, no compressed) */
  DWORD StampOffset;
  /* Offset of raster lines table: 4 bytes offset for every line */
  DWORD ScanOffset;
  /* Type of attributes: 0 - no alpha present */
  BYTE AttributeType;
} tgaEXTHEADER;

#pragma pack(pop)

/* The rendering image in format TGA saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveTGA( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;
  tgaFILEHEADER FH = {0};
  tgaFILEFOOTER FF = {0};
  INT i;

  if ((F = fopen(FileName, "wb")) != NULL)
  {
    /* File Header */
    FH.ImageType = 2;
    FH.Height = Img->H;
    FH.Width = Img->W;
    FH.BitsPerPixel = 32;
    fwrite(&FH, sizeof(tgaFILEHEADER), 1, F);
    /* Image Data */
    for (i = 0; i < Img->H * Img->W; i++)
      fwrite(&Img->Pixels[i], 4, 1, F);
    /* FileFooter */
    strcpy(FF.Signature, TGA_EXT_SIGNATURE);
    fwrite(&FF, sizeof(tgaFILEFOOTER), 1, F);
    fclose(F);

    return TRUE;
  }

  return FALSE;
} /* End of 'TWR_RndSaveTGA' function */

/* The rendering image loading at file format TGA program function.
* ARGUMENTS:
*   - name of file:
*       CHAR *FileName; 
*   - pointer on structure of image:
*       twrIMAGE *Img;
* RETURNS:
*   (BOOL) TRUE if successful, FALSE otherwise.
*/
BOOL TWR_RndLoadTGA( twrIMAGE *Img, CHAR *FileName )
{
  FILE *F;
  INT i;
  tgaFILEHEADER FH;
  DWORD Pal[256];
  static CHAR Buf[sizeof(TGA_EXT_SIGNATURE) + 1];

  if ((F = fopen(FileName, "rb")) != NULL)
  {
    fread(&FH, sizeof(tgaFILEHEADER), 1, F);
    /* Check header and supported image type */
    if (!((FH.ImageType == 2 || FH.ImageType == 10) && (FH.BitsPerPixel == 24 || FH.BitsPerPixel == 32) ||
          FH.ColorMapType == 1 && (FH.ImageType == 1 || FH.ImageType == 9) && FH.BitsPerPixel == 8))
    {
      fclose(F);
      return FALSE;
    }
    fseek(F, -(INT)sizeof(TGA_EXT_SIGNATURE), SEEK_END);
    if (fread(Buf, sizeof(TGA_EXT_SIGNATURE), 1, F) != 1 ||
        strcmp(TGA_EXT_SIGNATURE, Buf) != 0)
    {
      fclose(F);
      return FALSE;
    }
    if (!TWR_ImgCreate(Img, FH.Width, FH.Height))
    {
      fclose(F);
      return FALSE;
    }
    Twr->Log("\n");

    /* Skip file comments */
    fseek(F, sizeof(FH) + FH.IDLength, SEEK_SET);

    /* Load palette if needed */
    if (FH.ColorMapType == 1)
    {
      INT bpp = FH.PaletteEntrySize >> 3;
      INT pstart = FH.PaletteStart, psize = FH.PaletteSize;

      if (bpp > 4)
        bpp = 4;
      if (pstart + psize > 256)
        psize = 256 - pstart;

      for (i = 0; i < psize; i++)
      {
        DWORD col = 0xFF000000;

        fread(&col, bpp, 1, F);
        Pal[pstart + i] = col;
      }
      /* Skip palette */
      fseek(F, sizeof(FH) + FH.IDLength + (FH.PaletteEntrySize >> 3) * FH.PaletteSize, SEEK_SET);
    }

    /* Black and white picture */
    /*
    if (FH.ImageType == 3)
    {                   
      INT i;

      for(i = 0; i < Img->H * Img->W; i += 8)
      {
        DWORD j = 0, c;
        fread(&j, 4, 1, F);
        for (c = 0; c < 8; c++)
          Img->Pixels[i + c] = (j << c) >> 7;
      }
    }
    */

    /* Not Compresed */
    if (FH.ImageType == 1 || FH.ImageType == 2)
    {
      INT i;

      for(i = 0; i < Img->H * Img->W; i++)
      {
        DWORD col = 0xFF000000;

        /* Color-mapped */
        if (FH.ImageType == 1)
        {
          BYTE c;

          fread(&c, 1, 1, F);
          col |= Pal[c];
        }
        /* RGB */
        else
          fread(&col, FH.BitsPerPixel >> 3, 1, F);
        Img->Pixels[i] = col;
      }
    }
    /* Cmpresed */
    else if (FH.ImageType == 9 || FH.ImageType == 10)
    {
      INT i = 0;

      while (i < Img->H * Img->W)
      {
        BYTE counter = 0;

        fread(&counter, 1, 1, F);
        /* First bite RLE == 1 */
        if (counter & 0x80)
        {
          DWORD col = 0xFF000000;

          /* Color-mapped */
          if (FH.ImageType == 9)
          {
            BYTE c;

            fread(&c, 1, 1, F);
            col |= Pal[c];
          }
          /* RGB */
          else
            fread(&col, FH.BitsPerPixel >> 3, 1, F);

          counter &= 0x7F;
          counter++;
          while (counter-- > 0)
            Img->Pixels[i++] = col;
        }
        /* First bite RLE == 0 */
        else
        {
          counter &= 0x7F;
          counter++;
          while (counter-- > 0)
          {
            DWORD col = 0xFF000000;

            /* Color-mapped */
            if (FH.ImageType == 9)
            {
              BYTE c;

              fread(&c, 1, 1, F);
              col |= Pal[c];
            }
            /* RGB */
            else
              fread(&col, FH.BitsPerPixel >> 3, 1, F);

            Img->Pixels[i++] = col;
          }
        }
      }
    }
    /* Revers picture if it need */
    if ((FH.ImageDescr & 0x04) == 1)
    {
      INT x, y;

      for (y = 0; y < Img->H / 2; y++)
        for (x = 0; x < Img->W; x++)
        {      
          DWORD col = 0;

          col = Img->Pixels[x + y * Img->W];
          Img->Pixels[x + y * Img->W] = Img->Pixels[x + (Img->H - y - 1) * Img->W];
          Img->Pixels[x + (Img->H - y - 1) * Img->W] = col;
        }
    }
    fclose(F);
    return TRUE;
  }
  return FALSE;
} /* End of 'TWR_RndLoadTGA' function */

/* END OF 'twr_res_img_tga.c' FILE */
