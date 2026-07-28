/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_fonts.c
 * PURPOSE     : Tough Water Rendering project.
 *               Fonts implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

/***
 * Fonts data
 ***/

/* Direct 3D draw font material pattern */
static twrMATERIAL_PATTERN *TWR_MtlPatFontDirect;

/* Screen draw font material pattern */
static twrMATERIAL_PATTERN *TWR_MtlPatFontScreen;

/* Fonts stock */
static STOCK TWR_StkFonts;

/***
 * Font handle functions
 ***/

/* Font delete strategy function.
 * ARGUMENTS:
 *   - pointer to font:
 *       twrFONT *Fnt;
 * RETURNS: None.
 */
static VOID TWR_FontDeleteStrategy( twrFONT *Fnt )
{
  INT i;

  for (i = 0; i < 256; i++)
  {
    Twr->PrimFree(Fnt->Glyphs[i]);
    Twr->PrimFree(Fnt->GlyphsScreen[i]);
  }
  Twr->MtlFree(Fnt->MtlDirect);
  Twr->MtlFree(Fnt->MtlScreen);

  Twr->Info("Free font: ");
  Twr->SetColor(0x00FFFF, 0);
  Twr->Log("%s\n", Fnt->Name);
  Twr->SetColor(0xFFFFFF, 0);
  memset(Fnt, 0, sizeof(twrFONT));
} /* End of 'TWR_FontDeleteStrategy' function */

/* Load font function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 *   - font letters draw color:
 *       VEC4 TextColor;
 * RETURNS:
 *   (twrFONT *) pointer to load font resourse or NULL if error is occured.
 */
static twrFONT * TWR_FntCreate( CHAR *FileName, VEC4 TextColor )
{
  INT i, j;
  CHAR FontName[TWR_STR_MAX], FontFullFileName[TWR_STR_MAX];
  FILE *F;
  twrFONT *Fnt = NULL;
  DWORD Sign, LineH, BaseH, W, H, *FontTextureColors = NULL;
  FLT AdvanceX[256];
  /* Font file vertex representation type */
  struct tagFONT_FILE_VERTEX
  {
    VEC3 P; /* Vertex position */
    VEC2 T; /* Vertex texture coordinates */
    VEC3 N; /* Normal at vertex */
    VEC4 C; /* Vertex color */
  } Chars[256][4]; /* Characters glyphs */
  twrTEXTURE *Tex;

  sprintf(FontName, "%-.30s (%.3f,%.3f,%.3f,%.3f)", FileName, TextColor.X, TextColor.Y, TextColor.Z, TextColor.W);

  if ((Fnt = StockFirst(&TWR_StkFonts)) != NULL)
    do
    {
      if (strcmp(Fnt->Name, FontName) == 0)
        return Fnt;
    } while ((Fnt = StockNext(Fnt)) != NULL);

  sprintf(FontFullFileName, "bin/fonts/%s.g3df", FileName);
  if ((F = fopen(FontFullFileName, "rb")) == NULL)
    return NULL;;

  /* Read font file header + glyps + texture */
  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF" ||
      fread(&LineH, 4, 1, F) != 1 || !(LineH > 0 && LineH < 2048) ||
      fread(&BaseH, 4, 1, F) != 1 || !(BaseH > 0 && BaseH < 2048) ||
      fread(AdvanceX, 4, 256, F) != 256 ||
      fread(Chars, sizeof(struct tagFONT_FILE_VERTEX), 4 * 256, F) != 4 * 256 ||
      fread(&W, 4, 1, F) != 1 || !(W > 0 && LineH < 4096) ||
      fread(&H, 4, 1, F) != 1 || !(H > 0 && BaseH < 4096) ||
      (FontTextureColors = malloc(4 * W * H)) == NULL ||
      fread(FontTextureColors, 4, W * H, F) != W * H)
  {
    free(FontTextureColors);
    fclose(F);
    return NULL;
  }
  fclose(F);

  if ((Fnt = StockAlloc(&TWR_StkFonts)) == NULL)
    return NULL;
  memset(Fnt, 0, sizeof(twrFONT));
  strncpy(Fnt->Name, FontName, TWR_STR_MAX - 1);
  Fnt->BaseH = BaseH;
  Fnt->LineH = LineH;
  memcpy(Fnt->AdvanceX, AdvanceX, sizeof(AdvanceX));

  sprintf(FontFullFileName, "%-.50s direct material", FontName);
  Fnt->MtlDirect = Twr->MtlCreate(FontFullFileName, TWR_MtlPatFontDirect, TWR_MTL_FINAL);
  Twr->MtlReferenceInc(Fnt->MtlDirect);

  sprintf(FontFullFileName, "%-.50s screen material", FontName);
  Fnt->MtlScreen = Twr->MtlCreate(FontFullFileName, TWR_MtlPatFontScreen, TWR_MTL_FINAL);
  Twr->MtlReferenceInc(Fnt->MtlScreen);

  sprintf(FontFullFileName, "%-.50s texture", FontName);
  Tex = Twr->TexCreate(FontFullFileName, W, H, TRUE, GL_RGBA8, FontTextureColors);
  free(FontTextureColors);

  Twr->MtlBindTex(Fnt->MtlDirect, 0, Tex);
  Twr->MtlBindTex(Fnt->MtlScreen, 0, Tex);
      
  for (i = 0; i < 256; i++)
  {
    for (j = 0; j < 4; j++)
      Chars[i][j].C = TextColor;
    if (Fnt->AdvanceX[i] != 0)
    {
      Fnt->Glyphs[i] =
        Twr->PrimCreate(Fnt->MtlDirect, TWR_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
      Fnt->GlyphsScreen[i] =
        Twr->PrimCreate(Fnt->MtlScreen, TWR_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    }
  }
  Twr->Info("Create font: ");
  Twr->SetColor(0x00FFFF, 0);
  Twr->Log("%s\n", FontName);
  Twr->SetColor(0xFFFFFF, 0);
  return Fnt;
} /* End of 'TWR_FntCreate' function */

/* Font free function.
 * ARGUMENTS:
 *   - font pointer:
 *       twrFONT *Fnt;
 * RETURNS: None.
 */
static VOID TWR_FntFree( twrFONT *Fnt )
{
  if (Fnt == NULL)
    return;
  StockDelete(Fnt);
} /* End of 'TWR_FntFree' function */

/* Draw string in screen space function.
 * ARGUMENTS:
 *   - font pointer:
 *       twrFONT *Fnt;
 *   - text to draw:
 *       CHAR *Text;
 *   - draw position:
 *       VEC3 Pos;
 *   - font size (in world space units):
 *       FLT Size;
 * RETURNS: None.
 */
static VOID TWR_FntDraw( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size )
{
  INT cpos = 0;
  VEC3 p = Pos;
  FLT startx = Pos.X;

  if (Fnt == NULL)
    return;
  if (Size == 0)
    Size = Fnt->LineH;

  while (*Text != 0)
  {
    if (*Text == '\n')
      cpos = 0, p.X = startx, p.Y -= Size;
    else if (*Text == '\r')
      cpos = 0, p.X = startx;
    else if (*Text == '\t')
      p.X += (8 - (cpos & 7)) * Fnt->AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
      if (Fnt->AdvanceX[(UCHAR)*Text] != 0)
      {
        Twr->PrimDraw(Fnt->GlyphsScreen[(UCHAR)*Text], MatrMulMatr(MatrScale(Vec3Set(Size, Size, 1)), MatrTranslate(p)));
        p.X += Fnt->AdvanceX[(UCHAR)*Text] * Size;
        cpos++;
      }
    Text++;
  }
} /* End of 'TWR_FntDraw' function */

/* Draw string in 3D space function.
 * ARGUMENTS:
 *   - font pointer:
 *       twrFONT *Fnt;
 *   - text to draw:
 *       CHAR *Text;
 *   - draw position:
 *       VEC3 Pos;
 *   - font size (in world space units):
 *       FLT Size;
 * RETURNS: None.
 */
static VOID TWR_FntDraw3D( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size )
{
  INT cpos = 0;
  VEC3 p = Pos;
  FLT startx = Pos.X;

  if (Fnt == NULL)
    return;
  if (Size == 0)
    Size = 1;

  while (*Text != 0)
  {
    if (*Text == '\n')
      cpos = 0, p.X = startx, p.Y -= Size;
    else if (*Text == '\r')
      cpos = 0, p.X = startx;
    else if (*Text == '\t')
      p.X += (8 - (cpos & 7)) * Fnt->AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
      if (Fnt->AdvanceX[(UCHAR)*Text] != 0)
      {
        Twr->PrimDraw(Fnt->Glyphs[(UCHAR)*Text], MatrMulMatr(MatrScale(Vec3Set(Size, Size, 1)), MatrTranslate(p)));
        p.X += Fnt->AdvanceX[(UCHAR)*Text] * Size;
        cpos++;
      }
    Text++;
  }
} /* End of 'TWR_FntDraw3D' function */

/* Draw string in billboard mode function.
 * ARGUMENTS:
 *   - font pointer:
 *       twrFONT *Fnt;
 *   - text to draw:
 *       CHAR *Text;
 *   - draw position:
 *       VEC3 Pos;
 *   - font size (0 - for original size, otherwise - size in pixels):
 *       FLT Size;
 * RETURNS: None.
 */
static VOID TWR_FntDrawBB( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size )
{
  INT cpos = 0;
  VEC3 p = Pos;
  VEC3 start = Pos;
  MATR m =
    MatrSet(Twr->Cam.Right.X, Twr->Cam.Right.Y, Twr->Cam.Right.Z, 0,
            Twr->Cam.Up.X, Twr->Cam.Up.Y, Twr->Cam.Up.Z, 0,
            Twr->Cam.Dir.X, Twr->Cam.Dir.Y, Twr->Cam.Dir.Z, 0,
            0, 0, 0, 1);

  if (Fnt == NULL)
    return;
  if (Size == 0)
    Size = 1;

  while (*Text != 0)
  {
    if (*Text == '\n')
      cpos = 0, start = Vec3SubVec3(start, Vec3MulNum(Twr->Cam.Up, Size)), p = start;
    else if (*Text == '\r')
      cpos = 0, p = start;
    else if (*Text == '\t')
      p = Vec3AddVec3(p, Vec3MulNum(Twr->Cam.Right, (8 - (cpos & 7)) * Fnt->AdvanceX[' '] * Size)), cpos += (8 - (cpos & 7));
    else
      if (Fnt->AdvanceX[(UCHAR)*Text] != 0)
      {
        Twr->PrimDraw(Fnt->Glyphs[(UCHAR)*Text], MatrMulMatr3(MatrScale(Vec3Set(Size, Size, 1)), m, MatrTranslate(p)));
        p = Vec3AddVec3(p, Vec3MulNum(Twr->Cam.Right, Fnt->AdvanceX[(UCHAR)*Text] * Size));
        cpos++;
      }
    Text++;
  }
} /* End of 'TWR_FntDrawBB' function */

/* Draw string in billboard mode function.
 * ARGUMENTS:
 *   - font pointer:
 *       twrFONT *Fnt;
 *   - text to draw:
 *       CHAR *Text;
 *   - draw position:
 *       VEC3 Pos;
 *   - font size in pixels (0 - for original size, otherwise - size in pixels):
 *       FLT Size;
 * RETURNS: None.
 */
static VOID TWR_FntDrawBBS( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size )
{
  INT cpos = 0;
  VEC3 p = Pos;
  VEC3 start = Pos;
  MATR m =
    MatrSet(Twr->Cam.Right.X, Twr->Cam.Right.Y, Twr->Cam.Right.Z, 0,
            Twr->Cam.Up.X, Twr->Cam.Up.Y, Twr->Cam.Up.Z, 0,
            Twr->Cam.Dir.X, Twr->Cam.Dir.Y, Twr->Cam.Dir.Z, 0,
            0, 0, 0, 1);
  FLT
    Dist = Vec3Len(Vec3SubVec3(Pos, Twr->Cam.Loc)),
    sy = Twr->Cam.Hp / Twr->Cam.FrameH * Dist / Twr->Cam.ProjDist;

  if (Fnt == NULL)
    return;
  if (Size == 0)
    Size = Fnt->LineH;

  Size *= sy; // PointTransform(Pos, Twr->Cam.MatrView).Z / 10;

  while (*Text != 0)
  {
    if (*Text == '\n')
      cpos = 0, start = Vec3SubVec3(start, Vec3MulNum(Twr->Cam.Up, Size)), p = start;
    else if (*Text == '\r')
      cpos = 0, p = start;
    else if (*Text == '\t')
      p = Vec3AddVec3(p, Vec3MulNum(Twr->Cam.Right, (8 - (cpos & 7)) * Fnt->AdvanceX[' '] * Size)), cpos += (8 - (cpos & 7));
    else
      if (Fnt->AdvanceX[(UCHAR)*Text] != 0)
      {
        Twr->PrimDraw(Fnt->Glyphs[(UCHAR)*Text], MatrMulMatr3(MatrScale(Vec3Set(Size, Size, 1)), m, MatrTranslate(p)));
        p = Vec3AddVec3(p, Vec3MulNum(Twr->Cam.Right, Fnt->AdvanceX[(UCHAR)*Text] * Size));
        cpos++;
      }
    Text++;
  }
} /* End of 'TWR_FntDrawBBS' function */

/* Make one logo texture .G3DF font function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FontFileName;
 *   - logo texture file name:
 *       CHAR *TexFileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
static BOOL TWR_FontMakeLogoFont( CHAR *FontFileName, CHAR *TexFileName )
{
  FILE *F;
  DWORD h, WH, x, y;
  twrIMAGE Img1, Img;
  FLT AdvanceX[256] = {0};
  /* Font file vertex representation type */
  struct tagFONT_FILE_VERTEX
  {
    VEC3 P; /* Vertex position */
    VEC2 T; /* Vertex texture coordinates */
    VEC3 N; /* Normal at vertex */
    VEC4 C; /* Vertex color */
  } V[4] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 1, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
  }, Chars[256][4] = {{{{{0}}}}}; /* Characters glyphs */

  if (!Twr->ImgLoad(&Img1, TexFileName))
    return FALSE;
  if ((F = fopen(FontFileName, "wb")) == NULL)
  {
    Twr->ImgFree(&Img);
    return FALSE;
  }
  WH = Img1.W > Img1.H ? Img1.W : Img1.H;
  WH = ceil(log(WH) / log(2));
  WH = WH < 1 ? 1 : WH;
  WH = 1 << WH;
  if (!Twr->ImgCreate(&Img, WH, WH))
  {
    Twr->ImgFree(&Img1);
    fclose(F);
    return FALSE;
  }
  /* Copy image */
  memset(Img.Pixels, 0, 4 * Img.W * Img.H);
  /* Test fill */
  /* memset(Img.Pixels, 0xFF, 4 * Img.W * Img.H); */
  for (y = 0; y < Img1.H; y++)
  {
    for (x = 0; x < Img1.W; x++)
      Img.Pixels[y * Img.W + x] = Img1.Pixels[y * Img1.W + x];
    /*
    Img.Pixels[y * Img.W + 0] = 0xFF00FF00;
    Img.Pixels[y * Img.W + Img1.W - 1] = 0xFF00FF00;
    */
  }
  /*
  for (x = 0; x < Img1.W; x++)
  {
    Img.Pixels[0 * Img.W + x] = 0xFF00FF00;
    Img.Pixels[(Img1.H - 1) * Img.W + x] = 0xFF00FF00;
  }
  */

  fwrite("G3DF", 4, 1, F);
  h = Img1.H;
  fwrite(&h, 4, 1, F);
  fwrite(&h, 4, 1, F);

  AdvanceX['G'] = (FLT)Img1.W / Img1.H;

  V[2].P.X = (FLT)Img1.W / Img1.H;
  V[3].P.X = (FLT)Img1.W / Img1.H;
  V[2].T.X = (FLT)Img1.W / WH;
  V[3].T.X = (FLT)Img1.W / WH;
  V[1].P.Y = -1;
  V[3].P.Y = -1;
  V[1].T.Y = (FLT)Img1.H / WH;
  V[3].T.Y = (FLT)Img1.H / WH;

  Chars['G'][0] = V[0];
  Chars['G'][1] = V[1];
  Chars['G'][2] = V[2];
  Chars['G'][3] = V[3];

  fwrite(AdvanceX, 4, 256, F);
  fwrite(Chars, sizeof(struct tagFONT_FILE_VERTEX), 4 * 256, F);
  fwrite(&Img.W, 4, 1, F);
  fwrite(&Img.H, 4, 1, F);
  fwrite(Img.Pixels, 4, Img.W * Img.H, F);
  Twr->ImgFree(&Img);
  Twr->ImgFree(&Img1);
  fclose(F);
  return FALSE;
} /* End of 'TWR_FontMakeLogoFont' function */

/* Fonts system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemFontsInit( VOID )
{
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition",  3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord",  2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",    3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",     4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  Twr->FntCreate = TWR_FntCreate;
  Twr->FntFree = TWR_FntFree;
  Twr->FntDraw = TWR_FntDraw;
  Twr->FntDraw3D = TWR_FntDraw3D;
  Twr->FntDrawBB = TWR_FntDrawBB;
  Twr->FntDrawBBS = TWR_FntDrawBBS;

  /* Create material patterns */
  TWR_MtlPatFontDirect = Twr->MtlPatCreate("Fonts_Direct", "fonts/direct/", 1, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(TWR_MtlPatFontDirect);
  TWR_MtlPatFontScreen = Twr->MtlPatCreate("Fonts_Screen", "fonts/screen/", 1, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(TWR_MtlPatFontScreen);

  /* Create fonts stock */
  StockCreate(&TWR_StkFonts, "Fonts", sizeof(twrFONT), 30, TWR_FontDeleteStrategy);
  /* Register fonts stock to strip stock */
  TWR_StockRegisterToStrip(&TWR_StkFonts);

  /* TWR_FontMakeLogoFont("CGSG-Logo.g3df", "CGSG-Logo.png"); */
  Twr->Sys("Fonts system is initialized");
} /* End of 'TWR_SystemFontsInit' function */

/* Fonts system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemFontsClose( VOID )
{
  StockFree(&TWR_StkFonts);
  Twr->MtlPatFree(TWR_MtlPatFontDirect);
  Twr->MtlPatFree(TWR_MtlPatFontScreen);
  Twr->Sys("Fonts system is deinitialized");
} /* End of 'TWR_SystemFontsClose' function */

/* END OF 'twr_res_fonts.c' FILE */
