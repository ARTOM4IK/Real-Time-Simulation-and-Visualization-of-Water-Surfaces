/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_textures.c
 * PURPOSE     : Tough Water Rendering project.
 *               Textures processing module.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 23.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Textures stock */
static STOCK TWR_StkTextures;

/* default texture pointer */
static twrTEXTURE *DefTex;

/* Texture delete strategy function.
 * ARGUMENTS:
 *   - texture pointer:
 *       twrTEXTURE *Tex.
 * RETURNS: None.
 */
static VOID TWR_TextureDeleteStrategy( twrTEXTURE *Tex )
{
  Twr->Log("Delete texture:");
  Twr->SetColor(0xFFFF00, 0);
  Twr->Log(Tex->Name);
  Twr->SetColor(0xFFFFFF, 0);
  Twr->Log(" [%p Id: %d Ref: %d]\n", Tex, Tex->TexId, Tex->RefCounter);
  glDeleteTextures(1, &Tex->TexId);
  Tex->TexId = -102;
} /* End of 'TWR_TextureDeleteStrategy' function */

/* Tool texture find in stock function.
 * ARGUMENTS:
 *   - Texture name:
 *       CHAR *TextureName;
 * RETURNS:
 *    (BOOL).
 */
static BOOL TWR_TexFindFunc( twrTEXTURE *Tex, CHAR *TextureName )
{
  return strcmp(Tex->Name, TextureName) == 0;
} /* End of 'TWR_TexFindFunc' function */

/* Find texture in stock function.
 * ARGUMENTS:
 *   - Texture name:
 *       CHAR *TextureName;
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TextureFind( CHAR *TextureName )
{
  return StockFind(&TWR_StkTextures, TWR_TexFindFunc, TextureName);
} /* End of 'TWR_TextureFind' function */

/* Texture's usage counter increase function.
 * ARGUMENTS:
 *   - texture pointer:
 *       twrTEXTURE *Tex;
 * RETURNS: None.
 */
static VOID TWR_TexReferenceInc( twrTEXTURE *Tex )
{
  if (Tex == NULL)
    return;
  Tex->RefCounter++;
} /* End of 'TWR_TexReferenceInc' function */

/* Create 2D texture function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *TextureName;
 *   - texture size:
 *       INT W, H;
 *   - mipmap creation flag:
 *       BOOL IsMipmap;
 *   - OpenGL texture type:
 *       INT GLType;
 *   - texture pixel color data (may be NULL):
 *       VOID *Bits;
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TexCreate( CHAR *TextureName, INT W, INT H, BOOL IsMipmap, INT GLType, VOID *Bits )
{
  INT mips = 1, comps = 1, type = GL_UNSIGNED_BYTE;
  BOOL is_tex_transparent = FALSE;
  twrTEXTURE *Tex = NULL;

  Twr->Sys("Create texture: ");
  Twr->SetColor(0xFFFF00, 0);
  Twr->Log(TextureName);
  Twr->SetColor(0xFFFFFF, 0);
  Twr->Log(" %d x %d", W, H);
  /*
  if ((Tex = TWR_TextureFind(TextureName)) != NULL)
  {
    TWR_TexReferenceInc(Tex);
    return Tex;
  }
  */
  if ((Tex = StockAlloc(&TWR_StkTextures)) == NULL)
    return NULL;

  switch (GLType)
  {
  case GL_R8:
    type = GL_UNSIGNED_BYTE;
    comps = 1;
    break;
  case GL_R32F:
    type = GL_FLOAT;
    comps = 1;
    break;
  case GL_RGB8:
    type = GL_UNSIGNED_BYTE;
    comps = 3;
    break;
  case GL_RGB32F:
    type = GL_FLOAT;
    comps = 3;
    break;
  case GL_RGBA8:
    type = GL_UNSIGNED_BYTE;
    comps = 4;
    if (Bits != 0)
    {
      DWORD *ptr = Bits;
      INT n = W * H;

      while (n-- > 0)
        if ((*ptr >> 24) != 0xFF)
        {
          is_tex_transparent = TRUE;
          break;
        }
    }
    break;
  case GL_RGBA32F:
    type = GL_FLOAT;
    comps = 4;
    break;
  }

  /* Setup OpenGL texture */
  glGenTextures(1, &Tex->TexId);
  glBindTexture(GL_TEXTURE_2D, Tex->TexId);

  if (IsMipmap)
  {
    mips = log(W > H ? W : H) / log(2);
    mips = mips < 1 ? 1 : mips;
  }

  glTexStorage2D(GL_TEXTURE_2D, mips, GLType, W, H);

  if (Bits != NULL)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, 
                    comps == 4 ? GL_BGRA : comps == 3 ? GL_BGR : GL_LUMINANCE,
                    type, Bits);
  }
  /* Set filters */
  if (IsMipmap)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* Set addressing */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  strncpy(Tex->Name, TextureName, TWR_STR_MAX - 1);
  Tex->W = W;
  Tex->H = H;
  Tex->Type = TWR_TEXTURE_2D;
  Tex->RefCounter = 0;
  Tex->IsMipmap = IsMipmap;
  Tex->IsTransparent = is_tex_transparent;
  Tex->GLType = GLType;

  return Tex;
} /* End of 'TWR_TexCreate' function */

/* Create 3D Cube map texture function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *TextureName;
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TexCreateCubeMap( CHAR *TextureName )
{
  twrTEXTURE *Tex = NULL;
  twrIMAGE Img = {0};
  INT mips = 1, W = 1, H = 1, i;
  CHAR *Names[] =
  {
    "PosX", "NegX",
    "PosY", "NegY",
    "PosZ", "NegZ",
  };
  CHAR *TypeImg[] =
  {
    "bmp", "png", "g24", "g32", "tga", "dds", "jpg"
  };
  CHAR CurrentType[5] = {0};
  CHAR Buf[TWR_STR_MAX];

  if ((Tex = StockAlloc(&TWR_StkTextures)) == NULL)
    return NULL;

  glGenTextures(1, &Tex->TexId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, Tex->TexId);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (i = 0; i < sizeof(TypeImg) / sizeof(*TypeImg); i++)
  {
    wsprintf(Buf, "bin\\textures\\SkyBoxes\\%s\\%s.%s", TextureName, Names[0], TypeImg[i]);
    if (Twr->ImgLoad(&Img, Buf))
    {
      strncpy(CurrentType, TypeImg[i], 4);
      Twr->ImgFree(&Img);
      break;
    }
  }
  if (CurrentType[0] == 0)
  {
    glDeleteTextures(1, &Tex->TexId);
    return NULL;
  }
  for (i = 0; i < 6; i++)
  {
    wsprintf(Buf, "bin\\textures\\SkyBoxes\\%s\\%s.%s", TextureName, Names[i], CurrentType);
    if (!Twr->ImgLoad(&Img, Buf))
    {
      glDeleteTextures(1, &Tex->TexId);
      return NULL;
    }
    if (i == 0)
    {
      W = Img.W;
      mips = log(W) / log(2);
      mips = mips < 1 ? 1 : mips;
      glTexStorage2D(GL_TEXTURE_CUBE_MAP, mips, GL_RGBA8, W, W);
    }
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, Img.W, Img.W,
                GL_BGRA, GL_UNSIGNED_BYTE, Img.Pixels);
    Twr->ImgFree(&Img);
  }
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
 
  Tex->W = W;
  Tex->H = W;
  strncpy(Tex->Name, TextureName, TWR_STR_MAX - 1);
  Tex->GLType = GL_UNSIGNED_BYTE;
  Tex->Type = TWR_TEXTURE_CUBE_MAP;
  Tex->RefCounter = 0;
  Tex->IsMipmap = FALSE;
  /* Activate texture */
  glActiveTexture(GL_TEXTURE0 + 10);
  glBindTexture(GL_TEXTURE_CUBE_MAP, Tex->TexId);
 
  return Tex;
} /* End of 'TWR_TexCreateCubeMap' function */

/* Resize 2D texture function.
 * ARGUMENTS:
 *   - texture pointer:
 *       twrTEXTURE *Tex;
 *   - new texture size:
 *       INT W, H;
 * RETURNS: None.
 */
static VOID TWR_TexResize( twrTEXTURE *Tex, INT W, INT H )
{
  if (Tex == NULL)
    return;
  if (W == 0)
    W = 1;
  if (H == 0)
    H = 1;
  glDeleteTextures(1, &Tex->TexId);
  glGenTextures(1, &Tex->TexId);
  glBindTexture(GL_TEXTURE_2D, Tex->TexId);
  glTexStorage2D(GL_TEXTURE_2D, 1, Tex->GLType, Tex->W = W, Tex->H = H);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
} /* End of 'TWR_TexResize' function */

/* Free texture function.
 * ARGUMENTS:
 *   - texture pointer:
 *       twrTEXTURE *Tex;
 * RETURNS: None.
 */
static VOID TWR_TexFree( twrTEXTURE *Tex )
{
  if (Tex == NULL)
    return;
  Tex->RefCounter--;
  if (Tex->RefCounter <= 0)
    StockDelete(Tex);
} /* End of 'TWR_TexFree' function */

/* Create 2D texture 1x1 RGBA32F from vector4 function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *TextureName;
 *   - pixel color:
 *       VEC4 Value;
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TexCreateFromVec4( CHAR *TextureName, VEC4 Value )
{
  Value = Vec4Set(Value.Z, Value.Y, Value.X, Value.W);

  return TWR_TexCreate(TextureName, 1, 1, FALSE, GL_RGBA32F, &Value);
} /* End of 'TWR_TexCreateFromVec4' function */

/* Create 2D texture 1x1 RGB32F from vector4 function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *TextureName;
 *   - pixel color:
 *       VEC3 Value;
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TexCreateFromVec3( CHAR *TextureName, VEC3 Value )
{
  Value = Vec3Set(Value.Z, Value.Y, Value.X);

  return TWR_TexCreate(TextureName, 1, 1, FALSE, GL_RGB32F, &Value);
} /* End of 'TWR_TexCreateFromVec3' function */

/* Create 2D texture 1x1 RGBA32F white color function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_TexCreateDefault( VOID )
{
  VEC4 Data = Vec4Set(1, 1, 1, 1);

  DefTex = TWR_TexCreate("Default white texture 1x1", 1, 1, FALSE, GL_RGBA32F, &Data);
  Twr->TexReferenceInc(DefTex);
} /* End of 'TWR_TexCreateDefault' function */

/* Get default texture function.
 * ARGUMENTS: None.
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TexGetDefault( VOID )
{
  return DefTex;
} /* End of 'TWR_TexGetDefault' function */

/* Delete 2D texture 1x1 RGBA32F white color function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_TexDeleteDefault( VOID )
{
  TWR_TexFree(DefTex);
} /* End of 'TWR_TexCreateDefault' function */

/* Load 2D texture function.
 * ARGUMENTS: 
 *   - texture name:
 *       CHAR *TextureName.
 * RETURNS:
 *    twrTEXTURE *Tex.
 */
static twrTEXTURE * TWR_TexLoad( CHAR *TextureName )
{
  twrTEXTURE *Tex = NULL;
  twrIMAGE Img;

  Twr->ImgLoad(&Img, TextureName);
  Tex = TWR_TexCreate(TextureName, Img.W, Img.H, TRUE, GL_RGBA8, Img.Pixels);
  Twr->ImgFree(&Img);
  return Tex;
} /* End of 'TWR_TexLoad' function */

/* Save 2D texture function.
 * ARGUMENTS: 
 *   - texture name:
 *       CHAR *TextureName.
 * RETURNS: None.
 */
static VOID TWR_TexSave( CHAR *TextureName )
{
  CHAR Str[TWR_STR_MAX];
  twrIMAGE *Img = NULL;
  memset(Str, 0, TWR_STR_MAX - 1);
  strcat(Str, ".BMP");
  Twr->ImgSaveBMP(Img, Str);
} /* End of 'TWR_TexSave' function */

/* Textures system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTexturesInit( VOID )
{
  StockCreate(&TWR_StkTextures, "Textures", sizeof(twrTEXTURE), 100, TWR_TextureDeleteStrategy);
  /* Register stock of textures to strip */
  TWR_StockRegisterToStrip(&TWR_StkTextures);

  Twr->TexCreate = TWR_TexCreate;
  Twr->TexResize = TWR_TexResize;
  Twr->TexFree = TWR_TexFree;
  Twr->TextureFind = TWR_TextureFind;
  Twr->TexReferenceInc = TWR_TexReferenceInc;
  Twr->TexCreateFromVec4 = TWR_TexCreateFromVec4;
  Twr->TexCreateFromVec3 = TWR_TexCreateFromVec3;
  Twr->TexGetDefault = TWR_TexGetDefault;
  Twr->TexLoad = TWR_TexLoad;
  Twr->TexSave = TWR_TexSave;
  Twr->TexCreateCubeMap = TWR_TexCreateCubeMap;
  
  TWR_TexCreateDefault();

  Twr->Sys("Textures system is initialized");
} /* End of 'TWR_SystemTexturesInit' function */

/* Textures system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTexturesClose( VOID )
{
  TWR_TexDeleteDefault();

  StockFree(&TWR_StkTextures);
  Twr->Sys("Textures system is deinitialized");
} /* End of 'TWR_SystemTexturesClose' function */

/* END OF 'twr_res_textures.c' FILE */
