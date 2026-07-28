/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_jpeg.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images in format JPEG.
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
#include <setjmp.h>
#include "jpeglib/jpeglib.h"

typedef struct twrIMGS_ERROR_MGR *twrIMGS_ERROR_MGR;

/* Error manager structure */
struct twrIMGS_ERROR_MGR
{
  struct jpeg_error_mgr pub; /* "Public" fields */

  jmp_buf setjmp_buffer;     /* For return to Caller */
};

/* JPEG format error exit function.
 * ARGUMENTS:
 *   - image info:
 *       j_common_ptr CInfo;
 * RETURNS: None.
 */
static VOID TWR_ImgsErrorExit( j_common_ptr CInfo )
{
  /* cinfo->err really points to a twrIMGS_ERROR_MGR struct, so coerce pointer */
  twrIMGS_ERROR_MGR myerr = (twrIMGS_ERROR_MGR)CInfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*CInfo->err->output_message)(CInfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
} /* End of 'TWR_ImgsErrorExit' function */

/* The rendering image in format JPG saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveJPG( twrIMAGE *Img, CHAR *FileName )
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *F;
  DWORD *buffer;
  JSAMPARRAY row_pointer;
  INT row_stride;
  INT i, k, l;

  /* Allocating memory for image bits buffer */
  if ((buffer = malloc(sizeof(DWORD) * Img->W * Img->H)) == NULL)
    return FALSE;

  for (k = 0; k < Img->H; k++)
    for (l = 0; l < Img->W; l++)
      buffer[k * Img->W + l] = Img->Pixels[(Img->H - k - 1) * Img->W + l];

  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Specify data destination (eg, a file) */

  /* Open file */
  if ((F = fopen(FileName, "wb")) == NULL) 
  {
    jpeg_destroy_compress(&cinfo);
    return FALSE;
  }
  jpeg_stdio_dest(&cinfo, F);

  /* Set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */

  /* image width and height, in pixels */
  cinfo.image_width = Img->W;
  cinfo.image_height = Img->H;
  /* # of color components per pixel */
  cinfo.input_components = 3;
  /* colorspace of input image */
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, 100, TRUE);

  /* Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* while (scan lines remain to be written) */
  /*   jpeg_write_scanlines(...); */

  /* JSAMPLEs per row in image_buffer */
  row_stride = Img->W * 3;
  row_pointer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  while (cinfo.next_scanline < cinfo.image_height)
  { 
    for (i = 0; i < Img->W; i++)
    {
      INT
        R = Img->Pixels[Img->W * cinfo.next_scanline + i] & 0xFF,
        G = (Img->Pixels[Img->W * cinfo.next_scanline + i] >> 8) & 0xFF,
        B = (Img->Pixels[Img->W * cinfo.next_scanline + i] >> 16) & 0xFF;

      row_pointer[0][i * 3] = B;
      row_pointer[0][i * 3 + 1] = G;
      row_pointer[0][i * 3 + 2] = R;
    }
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Finish compression */
  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(F);
  free(buffer);

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);
  return TRUE;
} /* End of 'TWR_RndSaveJPG' function */

/* The rendering image loading at file format JPG program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadJPG( twrIMAGE *Img, CHAR *FileName )
{
  INT rotate, row_stride, i;
  FILE *F;
  struct jpeg_decompress_struct cinfo;
  struct twrIMGS_ERROR_MGR jerr;
  JSAMPARRAY buffer;

  memset(Img, 0, sizeof(twrIMAGE));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = TWR_ImgsErrorExit;

  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) 
  {
    /* If we have error */
    jpeg_destroy_decompress(&cinfo);
    fclose(F);
    return FALSE;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Specify data source (eg, a file) */
  jpeg_stdio_src(&cinfo, F);

  /* Read file parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);

  /* Start decompressor */
  jpeg_start_decompress(&cinfo);

  /* Handle image orientation */
  rotate = cinfo.rec_outbuf_height;
  if (rotate == 6 || rotate == 8)
  {
    Img->W = cinfo.output_height;
    Img->H = cinfo.output_width;
  }
  else
  {
    Img->W = cinfo.output_width;
    Img->H = cinfo.output_height;
  }

  /* Create image */
  Twr->ImgCreate(Img, Img->W, Img->H);

  row_stride = cinfo.output_width * cinfo.output_components;

  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  switch (cinfo.output_components)
  {
  case 1:
    while (cinfo.output_scanline < cinfo.output_height) 
    {
      jpeg_read_scanlines(&cinfo, buffer, 1);

      for (i = 0; i < (INT)cinfo.output_width; i++)
      {
        INT
          R = buffer[0][i];

        if (rotate == 6)
          Img->Pixels[i * Img->W + (cinfo.output_scanline - 1)] = COM_MAKELONG0123(R, R, R, 0xFF);
        else if (rotate == 8)
          Img->Pixels[(Img->H - 1 - i) * Img->W + (cinfo.output_scanline - 1)] = COM_MAKELONG0123(R, R, R, 0xFF);
        else if (rotate == 3)
          Img->Pixels[(Img->H - 1 - (cinfo.output_scanline - 1)) * Img->W + (Img->W - 1 - i)] = COM_MAKELONG0123(R, R, R, 0xFF);
        else
          Img->Pixels[(cinfo.output_scanline - 1) * Img->W + i] = COM_MAKELONG0123(R, R, R, 0xFF);
      }
    }
    break;
  case 3:
    while (cinfo.output_scanline < cinfo.output_height) 
    {
      jpeg_read_scanlines(&cinfo, buffer, 1);

      for (i = 0; i < (INT)cinfo.output_width; i++)
      {
        INT
          R = buffer[0][i * 3],
          G = buffer[0][i * 3 + 1],
          B = buffer[0][i * 3 + 2];

        if (rotate == 6)
          Img->Pixels[i * Img->W + (cinfo.output_scanline - 1)] = COM_MAKELONG0123(B, G, R, 0xFF);
        else if (rotate == 8)
          Img->Pixels[(Img->H - 1 - i) * Img->W + (cinfo.output_scanline - 1)] = COM_MAKELONG0123(B, G, R, 0xFF);
        else if (rotate == 3)
          Img->Pixels[(Img->H - 1 - (cinfo.output_scanline - 1)) * Img->W + (Img->W - 1 - i)] = COM_MAKELONG0123(B, G, R, 0xFF);
        else
          Img->Pixels[(cinfo.output_scanline - 1) * Img->W + i] = COM_MAKELONG0123(B, G, R, 0xFF);
      }
    }
    break;
  }

  /* Finish decompression */
  jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);
  fclose(F);
  return TRUE;
} /* End of 'TWR_RndLoadJPG' function */

/* END OF 'twr_res_img_jpeg.c' FILE */
