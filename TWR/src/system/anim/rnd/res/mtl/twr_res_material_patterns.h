/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_material_patterns.h
 * PURPOSE     : Tough Water Rendering project.
 *               Material patterns declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_material_patterns_h_
#define __twr_res_material_patterns_h_

/* Maximum amount of vertex attributes */
#define TWR_MAX_ATTRIBUTES 16

/* Macrodefinitions of bits to compare depth flag with */
#define TWR_READ_BIT 1
#define TWR_WRITE_BIT 2

/* Vertex attributes types structure */
typedef struct tagtwrVERTEX_ATTRS
{
  CHAR *TypeName;                /* Vertex type name */
  INT NumOfComponents;           /* Number of components */
  BOOL IsFloat;                  /* Flag that shows if attribute is float or not */
  INT Offset;                    /* Offset of different attributes */
} twrVERTEX_ATTRS;

/* Material patterns depth flag */
typedef enum twrDEPTH_FLAG
{
  TWR_DEPTH_NOT_READ_NOT_WRTIE = 0,   /* Material pattern should not read depth buffer
                                       * and it's depth buffer should not be written */
  TWR_DEPTH_READ_ONLY = 1,            /* Material pattern should read depth buffer */
  TWR_DEPTH_WRITE_ONLY = 2,           /* Material pattern's depth buffer should be written */
  TWR_DEPTH_READ_AND_WRITE = 3,       /* Material pattern should read depth buffer
                                       * and it's depth buffer should be written */
} twrDEPTH_FLAG;

/* Material patterns blend mode */
typedef enum twrBLEND_MODE
{
  TWR_BLEND_ALPHA = 0,               /* Colors are being mixed together */
  TWR_BLEND_ADD = 1,                 /* Colors are being added to each other */
  TWR_BLEND_REPLACE = 2,             /* Colors are being replaced with the last one */
} twrBLEND_MODE;

/* Material pattern representation type */
typedef struct tagtwrMATERIAL_PATTERN
{
  CHAR Name[TWR_STR_MAX];                     /* Material pattern name */
  twrSHADER *Shd;                             /* Pointer to shader */
  INT RefCounter;                             /* Reference counter */
  INT NumOfTextures;                          /* Number of textures */
  INT NumOfAttributes;                        /* Number of attributes */
  twrVERTEX_ATTRS Attrs[TWR_MAX_ATTRIBUTES];  /* Vertex attributes array */
  INT NumOfBuffers;                           /* Number of buffers */
  twrBUFFER *Bufs[TWR_MAX_BUFFERS];           /* Buffers array */
  INT VertexStride;                           /* Vertex buffer inter-vertex stride in bytes */
  twrDEPTH_FLAG DepthFlag;                    /* Material pattern depth flag (read only / write only /
                                               * read and write / neither read nor write) */
  twrBLEND_MODE BlendMode;                    /* Material pattern blend mode (colors are being mixed /
                                               * added / replaced) */
  UINT64 StoreFrameNo;                        /* Index of frame when material pattern
                                               * was first stored */
  twrMATERIAL *FirstDraw;                     /* Pointer to the first material
                                               * of this material pattern to draw */
  twrMATERIAL_PATTERN *NextDraw;              /* Pointer to the next material pattern
                                               * to draw */
} twrMATERIAL_PATTERN;

/* Vertex structure with default amount of attributes */
typedef struct tagtwrVERTEX_STD4
{
  VEC3 P;          /* Vertex position */
  VEC2 T;          /* Vertex texture coordinates */
  VEC3 N;          /* Vertex normal */
  VEC4 C;          /* Vertex color */
} twrVERTEX_STD4;

/* Vertex structure with maximum amount of attributes (tangent and bitangent vectors added) */
typedef struct tagtwrVERTEX_STD6
{
  VEC3 P;          /* Vertex position */
  VEC2 T;          /* Vertex texture coordinates */
  VEC3 N;          /* Vertex normal */
  VEC4 C;          /* Vertex color */
  VEC3 Tng;        /* Tangent basis vector */
  VEC3 Btn;        /* Bitangent basis vector */
} twrVERTEX_STD6;

/* Structure field offset determination */
#define TWR_GetFieldOffset(StructureType, FieldName) \
    ((INT)(&(((StructureType *)0L)->FieldName)))

/* Material patterns structure */
typedef struct tagtwrSYSTEM_MATERIAL_PATTERNS
{
  /* Create new material pattern function.
   * ARGUMENTS:
   *   - new material pattern name:
   *       CHAR *MtlPatName;
   *   - shader name:
   *       CHAR *ShaderName;
   *   - number of textures:
   *       INT NumOfTextures;
   *   - number of buffers:
   *       INT NumOfBuffers;
   *   - stride (size of one vertex):
   *       INT Stride;
   *   - vertices array:
   *       twrVERTEX_ATTRS *Attrs;
   *   - amount of attributes:
   *       INT NumOfAttrs;
   * RETURNS:
   *   (twrMATERIAL_PATTERN *) pointer to new material pattern.
   */
  twrMATERIAL_PATTERN * (*MtlPatCreate)( CHAR *MtlPatName, CHAR *ShaderName, INT NumOfTextures,
           INT NumOfBuffers, INT Stride, twrVERTEX_ATTRS *Attrs, INT NumOfAttrs );

  /* Delete material pattern by its pointer function.
   * ARGUMENTS:
   *   - pointer to material pattern:
   *       twrMATERIAL_PATTERN *MtlPat; 
   * RETURNS: None.
   */
  VOID (*MtlPatFree)( twrMATERIAL_PATTERN *MtlPat );

  /* Create default material pattern function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*MtlPatCreateDefault)( VOID );

  /* Get default material pattern function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (twrMATERIAL_PATTERN *) pointer to default material pattern.
   */
  twrMATERIAL_PATTERN * (*MtlPatGetDefault)( VOID );

  /* Apply material pattern (buffers) to shader function.
   * ARGUMENTS:
   *   - material pattern to be applied:
   *       twrMATERIAL_PATTERN *MtlPat;
   * RETURNS: None.
   */
  VOID (*MtlPatApply)( twrMATERIAL_PATTERN *MtlPat );

  /* Bind buffer to the material pattern function.
   * ARGUMENTS:
   *   - material pattern to bind buffer to:
   *       twrMATERIAL_PATTERN *MtlPat;
   *   - buffer to be binded:
   *       twrBUFFER *Buf;
   * RETURNS: None.
   */
  VOID (*MtlPatBindBuf)( twrMATERIAL_PATTERN *MtlPat, twrBUFFER *Buf );

  /* Set depth flag to the material pattern function.
   * ARGUMENTS:
   *   - material pattern to set depth flag to:
   *       twrMATERIAL_PATTERN *MtlPat;
   *   - depth flag:
   *       twrDEPTH_FLAG DepthFlag;
   * RETURNS: None.
   */
  VOID (*MtlPatSetDepthFlag)( twrMATERIAL_PATTERN *MtlPat, twrDEPTH_FLAG DepthFlag );

  /* Set blending mode to the material pattern function.
   * ARGUMENTS:
   *   - material pattern to set blending mode to:
   *       twrMATERIAL_PATTERN *MtlPat;
   *   - blending mode:
   *       twrBLEND_MODE BlendMode;
   * RETURNS: None.
   */
  VOID (*MtlPatSetBlendMode)( twrMATERIAL_PATTERN *MtlPat, twrBLEND_MODE BlendMode );

  /* Find material by name in material pattern stock function.
   * ARGUMENTS:
   *   - material pattern name:
   *       CHAR *MtlPatName;
   *   - material pattern file name:
   *       CHAR *FindName;
   * RETURNS:
   *   (BOOL) TRUE if material pattern is found, FALSE otherwise.
   */
  BOOL (*MtlPatFind)( twrMATERIAL_PATTERN *MtlPat, CHAR *FindName );

  /* Increase counter of usage of material pattern function.
   * ARGUMENTS:
   *   - material pattern to increase reference counter of:
   *       twrMATERIAL_PATTERN *MtlPat;
   * RETURNS: None.
   */
  VOID (*MtlPatReferenceInc)( twrMATERIAL_PATTERN *MtlPat );
} twrSYSTEM_MATERIAL_PATTERNS;

/* Initialize material patterns system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialPatternsInit( VOID );

/* Delete material patterns system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialPatternsClose( VOID );

#endif /* __twr_res_material_patterns_h_ */

/* END OF 'twr_res_material_patterns.h' FILE */
