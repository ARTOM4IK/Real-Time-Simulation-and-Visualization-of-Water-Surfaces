/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res.h
 * PURPOSE     : Tough Water Rendering project.
 *               resources declaration file.
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
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_h_
#define __twr_res_h_

#include "mtl/twr_res_materials.h"
#include "mtl/twr_res_material_patterns.h"
#include "shd/twr_res_shaders.h"
#include "tex/twr_res_textures.h"
#include "buf/twr_res_buffers.h"
#include "font/twr_res_fonts.h"
#include "prims/twr_res_prim.h"
#include "models/twr_res_models.h"
#include "topo/twr_res_topology.h"
#include "topo/twr_res_topology_grid.h"
#include "img/twr_res_img.h"

/* Resource system representation type */
typedef struct tagtwrSYSTEM_RENDER_RESOURCES
{
  /* Materials system */
  union
  {
    twrSYSTEM_MATERIALS;
    twrSYSTEM_MATERIALS Materials;
  };

  /* Material patterns system */
  union
  {
    twrSYSTEM_MATERIAL_PATTERNS;
    twrSYSTEM_MATERIAL_PATTERNS MtlPatterns;
  };

  /* Textures system render type */
  union
  {
    twrSYSTEM_TEXTURES;
    twrSYSTEM_TEXTURES Textures;
  };

  /* Buffers system render type */
  union
  {
    twrSYSTEM_BUFFERS;
    twrSYSTEM_BUFFERS Buffers;
  };

  /* Shaders system render type */
  union
  {
    twrSYSTEM_SHADERS;
    twrSYSTEM_SHADERS Shaders;
  };
  /* Topology system */
  union
  {
    twrSYSTEM_TOPOLOGY;
    twrSYSTEM_TOPOLOGY Topology;
  };
  /* Topology grid system */
  union
  {
    twrSYSTEM_TOPOLOGY_GRID;
    twrSYSTEM_TOPOLOGY Grid;
  };
  /* Primitives system */
  union
  {
    twrSYSTEM_PRIMS;
    twrSYSTEM_PRIMS Primitives;
  };
  /* Models system */
  union
  {
    twrSYSTEM_MODELS;
    twrSYSTEM_MODELS Models;
  };
  /* Fonts system */
  union
  {
    twrSYSTEM_FONTS;
    twrSYSTEM_FONTS Fonts;
  };
} twrSYSTEM_RENDER_RESOURCES;

/* Resources system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemResourcesInit( VOID );

/* Resources system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemResourcesClose( VOID );

#endif /* __twr_res_h_ */

/* END OF 'twr_res.h' FILE */
