/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_VG4_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by VG4 implementation module.
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

#define TWR_NUM_OF_LIGHTS 1

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_VG4
{
  TWR_UNIT_BASE_FIELDS;
  twrMODEL *Mdls[5];                /* Test models */
  twrPRIM *Floor;                   /* Floor primitive */
  twrPRIM *Flame;                   /* Flame primitive */
  twrLIGHT *FlameLigs[4];           /* Flame light sources */
  VEC3 FlamePos;                    /* FlamePosition */
  twrPRIM *LightDebug;              /* Light primitive */
  twrLIGHT *Lig[TWR_NUM_OF_LIGHTS]; /* Test light sources */
  twrFONT *Fnt, *Fnt1, *Fnt2;       /* Test fonts */
} twrUNIT_SAMPLE_VG4;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_VG4 *Uni )
{
  INT i;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition",  3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, P)},
    {"InTexCoord",  2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, T)},
    {"InNormal",    3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, N)},
    {"InColor",     4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, C)},
    {"InTangent",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Tng)},
    {"InBitangent", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Btn)},
  };
  twrMATERIAL_PATTERN *MtlPat = Twr->MtlPatCreate("Phong_STD6", "normal_mapping", 4, 0, sizeof(twrVERTEX_STD6), attrs, 6);
  twrMATERIAL_PATTERN *MtlPatLightDebug = Twr->MtlPatCreate("LightDebug", "lights/debug", 0, 1, 0, NULL, 0);
  twrMATERIAL *MtlLightDebug = Twr->MtlCreate("LightDebug", MtlPatLightDebug, TWR_MTL_OPAQUE);
  VEC3 B;
  FLT Sc;
  MATR M;
  twrVERTEX_STD6 V[] = 
  {
    {{-300, 0, -300}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
    {{-300, 0, 300}, {0, 50}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
    {{300, 0, -300}, {50, 0}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
    {{300, 0, 300}, {50, 50}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
  };
  twrVERTEX_ATTRS attrs_plane[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
    {"InTangent",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Tng)},
    {"InBitangent", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Btn)},
  };
  twrMATERIAL_PATTERN *MtlPatPlane;
  twrMATERIAL *MtlPlane;
  twrVERTEX_ATTRS attrs_flame[] =
  {
    {"InPosition",  3, TRUE, 0},
  };
  twrMATERIAL_PATTERN *MtlPatFlame = Twr->MtlPatCreate("flame", "samples/vg4/flame", 1, 0, sizeof(VEC3), attrs_flame, 1);
  twrMATERIAL *MtlFlame = Twr->MtlCreate("flame", MtlPatFlame, TWR_MTL_FINAL);
  VEC3 FlamePos = {0, 0, 0};

  Twr->MtlPatSetBlendMode(MtlPatFlame, TWR_BLEND_ALPHA);
  Twr->MtlBindTex(MtlFlame, 0, Twr->TexLoad("bin/textures/samples/vg4/flame/particlefx_10.png"));
  Uni->Flame = Twr->PrimCreate(MtlFlame, TWR_RND_PRIM_POINTS, &FlamePos, 1, NULL, 0);

  MtlPatPlane = Twr->MtlPatCreate("plain", "normal_mapping", 4, 0, sizeof(twrVERTEX_STD6), attrs_plane, 6);
  MtlPlane = Twr->MtlCreate("MyMtl", MtlPatPlane, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(MtlPlane, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  /* Twr->MtlBindTex(MtlPlane, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1))); */
  Twr->MtlBindTex(MtlPlane, 1, Twr->TexLoad("bin/textures/NormalMapsTextures/156/156.jpg"));
  Twr->MtlBindTex(MtlPlane, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(1), 102)));
  /* Twr->MtlBindTex(MtlPlane, 3, Twr->TexCreateFromVec4("NMap", Vec4Set(0.5, 0.5, 1, 0))); */
  Twr->MtlBindTex(MtlPlane, 3, Twr->TexLoad("bin/textures/NormalMapsTextures/156/156_norm.jpg"));
  Uni->Floor = Twr->PrimCreate(MtlPlane, TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);

  Uni->Mdls[0] = Twr->ModelLoadG3DM(NULL, "bin/models/barrels/barrel_rust.g3dm");
  Uni->Mdls[1] = Twr->ModelLoadG3DM(NULL, "bin/models/barrels/barrel_steel.g3dm");
  Uni->Mdls[2] = Twr->ModelLoadG3DM(NULL, "bin/models/barrels/fuel_can.g3dm");
  Uni->Mdls[3] = Twr->ModelLoadG3DM(NULL, "bin/models/barrels/plastic_barrel.g3dm");

  Uni->LightDebug = Twr->PrimCreate(MtlLightDebug, TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  /*
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->Mdls[4] = Twr->ModelLoadG3DM(NULL, "bin/models/x6.g3dm");
  */
  Sc = 4.9;
  Uni->Mdls[4] = Twr->ModelLoadG3DM(NULL, "bin/models/sponza.g3dm"); /* sponza.g3dm san-miguel.g3dm */
  /*
  Uni->Mdls[4] = Twr->ModelLoadG3DM(NULL, "bin/models/hair.g3dm");
  */
  if (Uni->Mdls[4] != NULL)
  {
    B = Vec3SubVec3(Uni->Mdls[4]->MaxBB, Uni->Mdls[4]->MinBB);
    M = MatrTranslate(Vec3Set(-Uni->Mdls[4]->MinBB.X - B.X / 2, -Uni->Mdls[4]->MinBB.Y, -Uni->Mdls[4]->MinBB.Z - B.Z / 2));
    Sc = 30;
    M = MatrMulMatr(M, MatrScale(Vec3Set1(Sc / B.X)));
    Uni->Mdls[4]->Trans = M;
  }
  /*
  Uni->Mdls[4]->NumOfPrims = 1;
  {
    twrMODEL_PRIM tmp;

    COM_SWAP(Uni->Mdls[4]->Prims[1000], Uni->Mdls[4]->Prims[0], tmp);
  }
  */

  for (i = 0; i < 4; i++)
    Uni->FlameLigs[i] = Twr->LightAddPoint(Vec3Set(0, 0, 0), Vec3Set1(0.50), Vec3Set(1, 0.08, 0), Vec2Set(0.18, 3));


  for (i = 0; i < TWR_NUM_OF_LIGHTS; i++)
    /* Uni->Lig[i] = Twr->LightAddPoint(Vec3Set(0, 3, (i - TWR_NUM_OF_LIGHTS / 2) * 50), Vec3Set(1, 1, 1), Vec3Set(1, 0.08, 0), Vec2Set(3, 30)); */
    Uni->Lig[i] = Twr->LightAddDir(Vec3Normalize(Vec3Set(1, 3, 1)), Vec3Set(1, 1, 1));
  Twr->MtlBindBuf(MtlLightDebug, Uni->Lig[0]->Buffer);

  if (Uni->Lig[0] != NULL)
    Twr->LightEnableShadow(Uni->Lig[0]);

  Uni->Fnt = Twr->FntCreate("Book", Vec4Set(1, 1, 0, 0.8));
  /*
  Uni->Fnt1 = Twr->FntCreate("Consolas", Vec4Set(1, 1, 1, 0.5));
  */
  Uni->Fnt2 = Twr->FntCreate("ConsolasBold", Vec4Set(1, 1, 1, 1));
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_VG4 *Uni )
{
  INT i;

  for (i = 0; i < 4; i++)
    Twr->ModelFree(Uni->Mdls[i]);

  Twr->FntFree(Uni->Fnt);
  Twr->FntFree(Uni->Fnt1);
  Twr->FntFree(Uni->Fnt2);
  Twr->PrimFree(Uni->Floor);
  Twr->PrimFree(Uni->LightDebug);
  Twr->PrimFree(Uni->Flame);
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_VG4 *Uni )
{
  FLT
    rx = Twr->Mx * Twr->Cam.Wp / Twr->Cam.FrameW - Twr->Cam.Wp / 2,
    ry = (Twr->Cam.FrameH - Twr->My) * Twr->Cam.Hp / Twr->Cam.FrameH - Twr->Cam.Hp / 2;
  VEC3 Dir = Vec3Normalize(Vec3AddVec3(Vec3AddVec3(Vec3MulNum(Twr->Cam.Dir, Twr->Cam.ProjDist),
                                                   Vec3MulNum(Twr->Cam.Right, rx)),
                                                   Vec3MulNum(Twr->Cam.Up, ry)));

  if (fabs(Dir.Y) > 0.0001)
  {
    FLT t = -Twr->Cam.Loc.Y / Dir.Y;
    VEC3 Pos = Vec3AddVec3(Twr->Cam.Loc, Vec3MulNum(Dir, t));
    CHAR Buf[102];

    sprintf(Buf, "<%f,%f,%f>", Pos.X, Pos.Y, Pos.Z);
    /*
    Twr->FntDrawBB(Uni->Fnt, Buf, Vec3AddVec3(Pos, Vec3MulNum(Twr->Cam.Up, 2)), 1);
    Twr->FntDraw(Uni->Fnt, Buf, Vec3Set1(0), 0);
    Twr->MrkDrawSphere(Pos, Vec3Set1(0.18), Vec4Set(0, 1, 0, 1), MatrIdentity());
    Twr->MrkDrawCylinder(Pos, 0.08, Vec3Set(Pos.X, 18, Pos.Z), 0.08, Vec4Set(1, 0, 1, 1), MatrIdentity());
    */
    if (Twr->Keys[VK_LBUTTON])
      Uni->FlamePos = Pos;
  }

  if (Uni->Lig[0] != NULL)
  {
    if (Twr->KeysClick['H'])
      Uni->Lig[0]->Dir = Vec3Normalize(Vec3Set(1, 1, 1));
    Uni->Lig[0]->Dir = VectorTransform(Uni->Lig[0]->Dir, MatrRotateX(30 * Twr->GlobalDeltaTime * (Twr->Keys['K'] - Twr->Keys['J'])));
    Uni->Lig[0]->Dir = VectorTransform(Uni->Lig[0]->Dir, MatrRotateY(30 * Twr->GlobalDeltaTime * (Twr->Keys['I'] - Twr->Keys['U'])));
    Uni->Lig[0]->Dir = VectorTransform(Uni->Lig[0]->Dir, MatrRotateZ(30 * Twr->GlobalDeltaTime * (Twr->Keys['M'] - Twr->Keys['N'])));
    Twr->LightUpdate(Uni->Lig[0]);
  }
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_VG4 *Uni )
{
  INT i, j, s = 0;
  CHAR Buf[102];
  static CHAR *Barrels[] =
  {
    "barrel_rust",
    "barrel_steel",
    "fuel_can",
    "plastic_barrel",
  };

  for (i = -s; i <= s; i++)
    for (j = -s; j <= s; j++)
      Twr->ModelDraw(Uni->Mdls[4],
        MatrMulMatr(MatrScale(Vec3Set1(1)), MatrTranslate(Vec3Set(j * 47, 0, i * 47))));
  sprintf(Buf, "FPS: %.3f\n<%d,%d>", Twr->FPS, Twr->Mx, Twr->My);
  /* Twr->FntDrawBB(Uni->Fnt, Buf, Vec3MulNum(Twr->Cam.Up, 8), 1); */
  Twr->PrimDraw(Uni->Floor, MatrIdentity());
  Twr->PrimDraw(Uni->LightDebug, MatrIdentity());
  /* Twr->PrimDraw(Uni->Flame, MatrTranslate(Uni->FlamePos)); */

  Twr->PrimDraw(Uni->Flame, MatrMulMatr(MatrScale(Vec3Set1(0.030)), MatrTranslate(Uni->FlamePos)));

  for (i = -1; i <= 1; i += 2)
    for (j = -1; j <= 1; j += 2)
    {
      FLT dx = 4.438, dy = 1.48;
      INT ln = (i + 1) / 2 * 2 + (j + 1) / 2;

      Twr->PrimDraw(Uni->Flame, MatrMulMatr(MatrScale(Vec3Set1(0.022)), MatrTranslate(Vec3Set(dx * j, 2, dy * i))));
      Uni->FlameLigs[ln]->Pos = Vec3Set(dx * j, 2.4 + 0.102 * sin(Twr->Time * 18), dy * i);
      Twr->LightUpdate(Uni->FlameLigs[ln]);
      Twr->MrkDrawSphere(Vec3Set(dx * j, 2, dy * i), Vec3Set1(0.030), Vec4Set(1, 1, 1, 1), MatrIdentity());
    }
  /// Twr->MrkDrawSphere(Vec3Set(0, 0, 0), Vec3Set1(1.30), Vec4Set(1, 0, 0, 1), MatrIdentity());
#if 0
  Twr->FntDraw3D(Uni->Fnt, "CGSG forever!!! тлк ╧ 30", Vec3Set1(0), 1);

  for (i = 0; i < 4; i++)
    for (j = 0; j < 100 + (INT)(Twr->Time * 0.030); j++)
    {
      VEC3 p = Vec3SubVec3(Vec3MulNum(Noise3Turb3D(i, j, 0, 5), 300), Vec3Set(150, 0, 150));
      MATR m;

      p.Y = 0;
      if (p.X * p.X + p.Z * p.Z < 36)
      {
        p.X += 50;
        p.Z += 50;
      }
      p.X += sin(i * 10 + j + Twr->Time * 0.8) * 2;
      p.Z += sin(i * 10 + j + 2 + Twr->Time * 0.8) * 2;

      Twr->ModelDraw(Uni->Mdls[i],
        /* MatrMulMatr(MatrScale(Vec3Set1(0.030)), MatrTranslate(Vec3Set(i * 3, 0, 0)))); */
        m = MatrMulMatr3(MatrScale(Vec3Set1(0.030)), MatrRotateY((30 + i * 3) * (j % 2 * 2 - 1) * Twr->Time), MatrTranslate(p)));

      sprintf(Buf, "%s\n<%.2f,%.2f,%.2f>", Barrels[i], p.X, p.Y, p.Z);
      if (Uni->Fnt2 != NULL)
        ;//Twr->FntDrawBBS(Uni->Fnt2, Buf, PointTransform(Vec3AddVec3(Vec3MulNum(Twr->Cam.Right, -1), Vec3MulNum(Twr->Cam.Up, Uni->Mdls[i]->MaxBB.Y + 5)), m), Uni->Fnt2->LineH / 3);
    }

  sprintf(Buf, "FPS: %.3f\n<%d,%d>", Twr->FPS, Twr->Mx, Twr->My);
  if (Uni->Fnt2 != NULL)
    ;//Twr->FntDrawBBS(Uni->Fnt2, Buf, Vec3AddVec3(Vec3MulNum(Twr->Cam.Right, 5), Vec3MulNum(Twr->Cam.Up, 8)), Uni->Fnt2->LineH / 2);


  Twr->FntDraw(Uni->Fnt1,
    "0123456789012345678\n"
    "abc\tdef\trr\n"
    "q\trrree\td\n"
    "a\tb\tc\tee\td\n"
    "a\rb\rc\r\n"
    "Lorem ipsum dolor sit amet,\n"
    "consectetur adipiscing elit. Aliquam lectus risus,\n"
    "finibus ornare vestibulum et, feugiat quis dui. Vivamus\n"
    "sit amet dolor et magna facilisis rhoncus. Curabitur\n"
    "maximus est sed porta scelerisque. Sed suscipit, arcu\n"
    "volutpat feugiat posuere, eros nisi tristique nibh,\n"
    "mollis vehicula lectus tortor eu purus. Donec ut\n"
    "tortor blandit, sagittis diam eget, suscipit eros.\n"
    "Quisque at magna neque. Nulla faucibus mi at nunc\n"
    "mattis placerat. Pellentesque quis augue quis elit\n"
    "tristique auctor. Integer varius est orci, vel egestas\n"
    "felis dictum nec. Phasellus porta ex sit amet turpis\n"
    "finibus, sed vestibulum nisl efficitur. Praesent ultrices\n"
    "diam enim. In ut tellus sed sem placerat sollicitudin.\n"
    "Donec quis mollis dolor. Etiam viverra, arcu cursus\n"
    "porttitor porttitor, diam nunc auctor nisl, quis\n"
    "placerat magna erat et odio. Phasellus feugiat\n"
    "turpis quis mollis lacinia. Sed ac orci et nisi\n"
    "venenatis pharetra ac non arcu.", Vec3Set1(0), 0);
#endif /* 0 */
} /* End of 'TWR_UnitRender' function */

/* VG4 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleVG4( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_VG4))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleVG4' function */

/* END OF 'twr_unit_sample_VG4_0.c' FILE */
