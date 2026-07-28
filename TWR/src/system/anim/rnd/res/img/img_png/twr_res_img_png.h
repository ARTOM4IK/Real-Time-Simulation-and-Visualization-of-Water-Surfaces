/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img_png.h
 * PURPOSE     : Tough Water Rendering project.
 *               ... declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 31.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_img_png_h_
#define __twr_res_img_png_h_

#include "twr_def.h"

/* The rendering image in format PNG saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSavePNG( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format PNG program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadPNG( twrIMAGE *Img, CHAR *FileName );

#endif /* __twr_res_img_png_h_ */

/* END OF 'twr_res_img_png.h' FILE */
