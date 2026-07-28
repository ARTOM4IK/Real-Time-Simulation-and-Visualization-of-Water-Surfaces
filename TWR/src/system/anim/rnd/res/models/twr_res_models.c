/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_models.c
 * PURPOSE     : Tough Water Rendering project.
 *               Models implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 10.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

/* Default normal map texture for model loading */
static twrTEXTURE *TWR_DefNMTex;

/* G3DM file material representation type */
typedef struct tagtwrG3DM_MATERIAL
{
  CHAR Name[300]; /* Material name */
  /* Illumination coefficients */
  VEC3 Ka, Kd, Ks;         /* Ambient, diffuse, specular coefficients */
  FLT Ph;                  /* Phong power coefficient – shininess */
  FLT Trans;               /* Transparency factor */
  DWORD Tex[8];            /* Texture references
                            * (8 time: texture number in G3DM file, -1 if no texture) */
  /* Shader information */
  CHAR ShaderString[300];  /* Additional shader information */
  DWORD Shader;            /* Shader number (used after load into memory) */
} twrG3DM_MATERIAL;

/* Stock of models */
static STOCK TWR_StkModels;

/* Load G3DM transform matrix */
static MATR TWR_ModelLoadG3DMTransformMatrix;

/* Pointer to model material pattern */
static twrMATERIAL_PATTERN *TWR_ModelMtlPat;

/* Create model function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (twrMODEL *) pointer to model, NULL if model is not created.
 */
static twrMODEL *TWR_ModelCreate( VOID )
{
  twrMODEL *Mdl;

  Mdl = StockAlloc(&TWR_StkModels);
  memset(Mdl, 0, sizeof(twrMODEL));
  if ((Mdl->Prims = malloc(sizeof(twrMODEL_PRIM))) == NULL)
    return NULL;

  memset(&Mdl->Prims[0], 0, sizeof(twrMODEL_PRIM));
  if ((Mdl->Prims[0].Prim = malloc(sizeof(twrPRIM))) == NULL)
    return NULL;

  Mdl->NumOfPrims = 0;
  Mdl->MaxNumOfPrims = 300;
  Mdl->Trans = MatrIdentity();

  return Mdl;
} /* End of 'TWR_ModelCreate' function */

/* Create model by number of primitives function.
 * ARGUMENTS:
 *   - number of primitives in model:
 *       INT NumOfPrims;
 * RETURNS:
 *   (twrMODEL *) pointer to model, NULL if model is not created.
 */
static twrMODEL *TWR_ModelCreateByNumOfPrims( INT NumOfPrims )
{
  INT i;
  twrMODEL *Mdl;

  Mdl = StockAlloc(&TWR_StkModels);
  memset(Mdl, 0, sizeof(twrMODEL));
  if ((Mdl->Prims = malloc(sizeof(twrMODEL_PRIM) * NumOfPrims)) == NULL)
    return NULL;
  memset(Mdl->Prims, 0, sizeof(twrMODEL_PRIM) * NumOfPrims);

  for (i = 0; i < NumOfPrims; i++)
    Mdl->Prims[i].Trans = MatrIdentity();

  Mdl->NumOfPrims = 0;
  Mdl->MaxNumOfPrims = 300;
  Mdl->Trans = MatrIdentity();
  return Mdl;
} /* End of 'TWR_ModelCreateByNumOfPrims' function */

/* Model free function.
 * ARGUMENTS:
 *   - model pointer:
 *       twrMODEL * Mdl;
 * RETURNS: None.
 */
static VOID TWR_ModelFree( twrMODEL *Mdl )
{
  if (Mdl == NULL)
    return;
  StockDeleteInstant(Mdl);
} /* End of 'TWR_ModelFree' function */

/* Primitive add to model function.
 * ARGUMENTS:
 *   - model pointer:
 *       twrMODEL *Mdl;
 *   - primitive pointer:
 *       twrPRIM *Pr;
 *   - transform matrix:
 *       MATR Trans;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TWR_ModelPrimAdd( twrMODEL *Mdl, twrPRIM *Pr, MATR Trans )
{
  if (Mdl->NumOfPrims >= Mdl->MaxNumOfPrims)
  {
    /* Allocate memory for a new primitive block */
    twrMODEL_PRIM *block = malloc(sizeof(twrMODEL_PRIM) * (Mdl->MaxNumOfPrims + 30));

    if (block == NULL)
      return FALSE;
    if (Mdl->Prims != NULL)
      memcpy(block, Mdl->Prims, sizeof(twrMODEL_PRIM) * Mdl->MaxNumOfPrims);
    Mdl->MaxNumOfPrims += 30;
    free(Mdl->Prims);
    Mdl->Prims = block;
  }
  Mdl->Prims[Mdl->NumOfPrims].Prim = Pr;
  Mdl->Prims[Mdl->NumOfPrims].Trans = Trans;
  Mdl->NumOfPrims++;
  Twr->ModelEvalBB(Mdl);
  return TRUE;
} /* End of 'TWR_ModelPrimAdd' function */

/* Load g3dm model transformation matrix set function.
 * ARGUMENTS:
 *   - transform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
static VOID TWR_ModelLoadG3DMSetTransform( MATR Transform )
{
  TWR_ModelLoadG3DMTransformMatrix = Transform;
} /* End of 'TWR_ModelLoadG3DMSetTransform' function */

/* Load g3dm model function.
 * ARGUMENTS:
 *   - material pattern:
 *       twrMATERIAL_PATTERN *MtlPat;
 *   - name of model file:
 *       CHAR *FileName;
 * RETURNS:
 *   (twrMODEL *) pointer to model, NULL if model is not loaded.
 */
static twrMODEL * TWR_ModelLoadG3DM( twrMATERIAL_PATTERN *MtlPat, CHAR *FileName )
{
  FILE *F;
  twrMODEL *Mdl;
  INT flen, p, m, t;
  BYTE *mem, *ptr, *prim_ptr, *mtl_ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  DWORD TotalNumOfFacetIndexes = 0;
  MATR tm = TWR_ModelLoadG3DMTransformMatrix,
       tminv = MatrTranspose(MatrInverse(tm));
  VOID *ResTable;
  twrMATERIAL **Mtls;
  twrTEXTURE **Texs;
  static CHAR
    TexName[TWR_STR_MAX],
    Buf[10000],
    Drive[_MAX_DRIVE],
    Dir[_MAX_DIR],
    FName[_MAX_FNAME],
    Ext[_MAX_EXT];

  /* Set model transformation matrix */
  TWR_ModelLoadG3DMTransformMatrix = MatrIdentity();

  /* Set default model material pattern */
  if (MtlPat == NULL)
    MtlPat = TWR_ModelMtlPat;

  Twr->Info("Loading model: ");
  Twr->SetColor(0x88FFFF, 0);
  Twr->Log("%s", FileName);
  Twr->SetColor(0xFFFFFF, 0);

  if ((F = fopen(FileName, "rb")) == NULL)
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, file not found");
    return NULL;
  }
 
  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory for whole file */
  if ((mem = malloc(flen)) == NULL)
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, not enough memory for load file bulk");
    fclose(F);
    return NULL;
  }
  _splitpath(FileName, Drive, Dir, FName, Ext);

  /* Load whole file to memory */
  rewind(F);
  fread(mem, 1, flen, F);
  fclose(F);

  ptr = mem;
  /* Signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, file is not .G3DM");
    free(mem);
    return NULL;
  }

  /* Quantities */
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;

  /* Allocate memory for resourses pointers */
  if ((ResTable = malloc(sizeof(VOID *) * (NumOfMaterials + NumOfTextures))) == NULL)
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, not enough memory for resourse table");
    free(mem);
    return NULL;
  }
  Texs = ResTable;
  Mtls = (twrMATERIAL **)(Texs + NumOfTextures);

  if ((Mdl = Twr->ModelCreateByNumOfPrims(NumOfPrims)) == NULL)
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, not enough memory for model primitives");
    free(ResTable);
    free(mem);
    return NULL;
  }

  /* Look for materials - skip primitives */
  prim_ptr = ptr;
  for (p = 0 ; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes, NumOfFacetIndexes, MtlNo;

    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    ptr += sizeof(twrVERTEX_STD4) * NumOfVertexes;
    ptr += sizeof(INT) * NumOfFacetIndexes;
  }

  /* Look for textures - skip materials */
  mtl_ptr = ptr;
  ptr += sizeof(twrG3DM_MATERIAL) * NumOfMaterials;
  /* Textures */
  for (t = 0 ; t < NumOfTextures; t++)
  {
    CHAR *Name;
    DWORD W, H, C;
    INT GLType;

    Name = (CHAR *)ptr;
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    if (C == 1)
      GLType = GL_R8;
    else if (C == 3)
      GLType = GL_RGB8;
    else
      GLType = GL_RGBA8;
    sprintf(Buf, "%s %s", FName, Name);
    Texs[t] = Twr->TexCreate(Buf, W, H, TRUE, GLType, ptr);
    ptr += C * W * H;
  }

  /* Materials */
  ptr = mtl_ptr;
  for (m = 0 ; m < NumOfMaterials; m++)
  {
    twrG3DM_MATERIAL *fmat;
    twrTRANSPARENCY_FLAG trans_flag;

    fmat = (VOID *)ptr;
    ptr += sizeof(*fmat);

    trans_flag = fmat->Trans == 1 ? TWR_MTL_OPAQUE : TWR_MTL_TRANSPARENT;

    if (fmat->Tex[0] != -1 && MtlPat->NumOfTextures > 1 && Texs[fmat->Tex[0]]->IsTransparent)
      trans_flag = TWR_MTL_TRANSPARENT;

    Mtls[m] = Twr->MtlCreate(fmat->Name, MtlPat, trans_flag);
    sprintf(Buf, "%s.g3dm %s Ka texture", FName, fmat->Name);
    Twr->MtlBindTex(Mtls[m], 0, Twr->TexCreateFromVec4(Buf, Vec4SetVec3(fmat->Ka, 0)));
    Mtls[m]->Trans = fmat->Trans;
    if (fmat->Tex[0] == -1 && Mtls[m]->NumOfTextures > 1)
    {
      sprintf(Buf, "%s.g3dm %s Kd texture", FName, fmat->Name);
      Twr->MtlBindTex(Mtls[m], 1, Twr->TexCreateFromVec4(Buf, Vec4SetVec3(fmat->Kd, 0)));
    }
    else if (Mtls[m]->NumOfTextures > 1)
      Twr->MtlBindTex(Mtls[m], 1, Texs[fmat->Tex[0]]);
    if (fmat->Tex[1] == -1 && Mtls[m]->NumOfTextures > 3)
      Twr->MtlBindTex(Mtls[m], 3, TWR_DefNMTex);
    else if (Mtls[m]->NumOfTextures > 3)
      Twr->MtlBindTex(Mtls[m], 3, Texs[fmat->Tex[1]]);
    if (fmat->Tex[2] == -1 && Mtls[m]->NumOfTextures > 2)
    {
      sprintf(Buf, "%s.g3dm %s KsPh texture", FName, fmat->Name);
      Twr->MtlBindTex(Mtls[m], 2, Twr->TexCreateFromVec4(Buf, Vec4SetVec3(fmat->Ks, fmat->Ph)));
    }
    else if (Mtls[m]->NumOfTextures > 2)
      Twr->MtlBindTex(Mtls[m], 2, Texs[fmat->Tex[2]]);
  }

  /* Primitives */
  ptr = prim_ptr;
  for (p = 0 ; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;
    DWORD MtlNo;              /* Material number in table below (in material section) */
    twrVERTEX_STD4 *V;
    INT *Ind;
    INT i;

    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    V = (twrVERTEX_STD4 *)ptr;
    ptr += sizeof(twrVERTEX_STD4) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;
    TotalNumOfFacetIndexes += NumOfFacetIndexes;

    for (i = 0; i < NumOfVertexes; i++)
    {
      V[i].P = PointTransform(V[i].P, tm);
      V[i].N = VectorTransform(V[i].N, tminv);
    }
    if (MtlNo >= 0 && MtlNo < NumOfMaterials)
    {
      /* Create twrVERTEX_STD6 vertex and evaluate every triangle's tangent space */
      twrVERTEX_STD6 *V6;

      /* Copy 4 vertex parameters in the twrVERTEX_STD6 vertex */
      if ((V6 = malloc(sizeof(twrVERTEX_STD6) * NumOfVertexes)) != NULL)
      {
        for (i = 0; i < NumOfVertexes; i++)
        {
          V6[i].P = V[i].P;
          V6[i].T = V[i].T;
          V6[i].N = V[i].N;
          V6[i].C = V[i].C;
          V6[i].Tng = Vec3AddVec3(Vec3CrossVec3(V6[i].N, Vec3Set(1, 0, 0)),
                                  Vec3CrossVec3(V6[i].N, Vec3Set(0, 1, 0))),
          V6[i].Btn = Vec3Normalize(Vec3CrossVec3(V6[i].Tng, V6[i].N));
          V6[i].Tng = Vec3CrossVec3(V6[i].N, V6[i].Btn);
          V6[i].Btn = Vec3Normalize(V6[i].Btn);
        }
        /* Evaluate every triangle's tangent space */
        if (((twrG3DM_MATERIAL *)mtl_ptr)[MtlNo].Tex[1] != -1)
        {
          for (i = 0; i < NumOfFacetIndexes; i += 3)
            Twr->EvalTriangleTS(&V6[Ind[i]], &V6[Ind[i + 1]], &V6[Ind[i + 2]]);
          /* Normalize every vertex's tangent and bitangent */
          for (i = 0; i < NumOfVertexes; i++)
          {
            V6[i].Tng = Vec3Normalize(V6[i].Tng);
            V6[i].Btn = Vec3Normalize(V6[i].Btn);
          }
        }
        Mtls[MtlNo]->MtlPat->VertexStride = sizeof(twrVERTEX_STD6);
        Twr->ModelPrimAdd(Mdl, Twr->PrimCreate(Mtls[MtlNo], TWR_RND_PRIM_TRIMESH,
                                               V6, NumOfVertexes, Ind, NumOfFacetIndexes), MatrIdentity());
        free(V6);
      }
    }
  }
  free(ResTable);
  free(mem);
  Twr->Log(" - ok, %d primitives, %d materials, %d textures, %d triangles",
           NumOfPrims, NumOfMaterials, NumOfTextures, TotalNumOfFacetIndexes / 3);
  return Mdl;
} /* End of 'TWR_ModelLoadG3DM' function */

/* Model set primitive transformation function.
 * ARGUMENTS:
 *   - model pointer:
 *       twrMODEL *Mdl;
 *   - primitive index:
 *       INT PrimNo;
 *   - transform matrix:
 *       MATR Trans;
 * RETURNS: None.
 */
static VOID TWR_ModelPrimSetTrans( twrMODEL *Mdl, INT PrimNo, MATR Trans )
{
  if (Mdl == NULL || PrimNo >= Mdl->NumOfPrims)
    return;
  Mdl->Prims[PrimNo].Trans = Trans;
} /* End of 'TWR_ModelPrimSetTrans' function */

/* Create model material pattern function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (twrMATERIAL_PATTERN *) pointer to material pattern.
 */
static twrMATERIAL_PATTERN *TWR_ModelMtlPatCreate( VOID )
{
  twrMATERIAL_PATTERN *MtlPat;

  /* Material pattern attributes */
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, P)},
    {"InTexCoord",   2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, T)},
    {"InNormal",     3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, N)},
    {"InColor",      4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, C)},
    {"InTangent",    3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Tng)},
    {"InBitangent",  3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Btn)},
  };

  MtlPat = Twr->MtlPatCreate("Phong_STD6", "normal_mapping", 4, 0, sizeof(twrVERTEX_STD6), attrs, 6);
  Twr->MtlPatReferenceInc(MtlPat);

  return MtlPat;
} /* End of 'TWR_ModelMtlPatCreate' function */

/* Model draw function.
 * ARGUMENTS:
 *   - model pointer:
 *       twrMODEL *Mdl;
 *   - world matrix:
 *       MATR World;
 * RETURNS: None.
 */
static VOID TWR_ModelDraw( twrMODEL *Mdl, MATR World )
{
  INT i;
  MATR m;

  if (Mdl == NULL)
    return;

  m = MatrMulMatr(Mdl->Trans, World);

  /* Draw all primitives */
  for (i = 0; i < Mdl->NumOfPrims; i++)
    Twr->PrimDraw(Mdl->Prims[i].Prim, MatrMulMatr(Mdl->Prims[i].Trans, m));
} /* End of 'TWR_ModelDraw' function */

/* Model eval bound box function.
 * ARGUMENTS:
 *   - model pointer:
 *       twrMODEL *Mdl;
 * RETURNS: None.
 */
static VOID TWR_ModelEvalBB( twrMODEL *Mdl )
{
  INT i;

  if (Mdl == NULL)
    return;
  if (Mdl->Prims[0].Prim != NULL && Mdl->NumOfPrims > 0)
  {
    Mdl->MinBB = Mdl->Prims[0].Prim->MinBB;
    Mdl->MaxBB = Mdl->Prims[0].Prim->MaxBB;
  }
  for (i = 0; i < Mdl->NumOfPrims; i++)
  {
    if (Mdl->MinBB.X > Mdl->Prims[i].Prim->MinBB.X)
      Mdl->MinBB.X = Mdl->Prims[i].Prim->MinBB.X;
    if (Mdl->MaxBB.X < Mdl->Prims[i].Prim->MaxBB.X)
      Mdl->MaxBB.X = Mdl->Prims[i].Prim->MaxBB.X;

    if (Mdl->MinBB.Y > Mdl->Prims[i].Prim->MinBB.Y)
      Mdl->MinBB.Y = Mdl->Prims[i].Prim->MinBB.Y;
    if (Mdl->MaxBB.Y < Mdl->Prims[i].Prim->MaxBB.Y)
      Mdl->MaxBB.Y = Mdl->Prims[i].Prim->MaxBB.Y;

    if (Mdl->MinBB.Z > Mdl->Prims[i].Prim->MinBB.Z)
      Mdl->MinBB.Z = Mdl->Prims[i].Prim->MinBB.Z;
    if (Mdl->MaxBB.Z < Mdl->Prims[i].Prim->MaxBB.Z)
      Mdl->MaxBB.Z = Mdl->Prims[i].Prim->MaxBB.Z;
  }
} /* End of 'TWR_ModelEvalBB' function */

/* Set models deletion strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ModelDeleteStrategy( twrMODEL *Mdl )
{
  INT i;

  for (i = 0; i < Mdl->NumOfPrims; i++)
    Twr->PrimFree(Mdl->Prims[i].Prim);
  free(Mdl->Prims);
  memset(Mdl, 0, sizeof(twrMODEL));
} /* End of 'TWR_ModelDeleteStrategy' function */

/* Models system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemModelsInit( VOID )
{
  TWR_ModelLoadG3DMTransformMatrix = MatrIdentity();
  Twr->ModelCreate = TWR_ModelCreate;
  Twr->ModelFree = TWR_ModelFree;
  Twr->ModelPrimAdd = TWR_ModelPrimAdd;
  Twr->ModelLoadG3DM = TWR_ModelLoadG3DM;
  Twr->ModelLoadG3DMSetTransform = TWR_ModelLoadG3DMSetTransform;
  Twr->ModelPrimSetTrans = TWR_ModelPrimSetTrans;
  Twr->ModelMtlPatCreate = TWR_ModelMtlPatCreate;
  Twr->ModelDraw = TWR_ModelDraw;
  Twr->ModelEvalBB = TWR_ModelEvalBB;
  Twr->ModelCreateByNumOfPrims = TWR_ModelCreateByNumOfPrims;

  /* Create model material pattern */
  TWR_ModelMtlPat = Twr->ModelMtlPatCreate();

  /* Create default normal map texture */
  TWR_DefNMTex = Twr->TexCreateFromVec4("NM texture", Vec4SetVec3(Vec3Set(0.5, 0.5, 1), 1));
  Twr->TexReferenceInc(TWR_DefNMTex);

  /* Create stock of models */
  StockCreate(&TWR_StkModels, "Models", sizeof(twrMODEL),
    300, TWR_ModelDeleteStrategy);
  /* Register stock of models to strip */
  TWR_StockRegisterToStrip(&TWR_StkModels);

  Twr->Sys("Model system is initialized");
} /* End of 'TWR_SystemModelsInit' function */

/* Models system close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemModelsClose( VOID )
{
  StockFree(&TWR_StkModels);
  Twr->Sys("Model system is deinitialized");
} /* End of 'TWR_SystemModelsClose' function */

/* END OF 'twr_res_models.c' FILE */
