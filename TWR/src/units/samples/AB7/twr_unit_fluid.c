/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_fluid.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by AB7 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Max amount of particle in uniform grid cell */
#define TWR_MAX_PARTICLES_PER_CELL 16

/* Max amount of grid cells */
#define TWR_MAX_CELLS_PER_GRID 1000

/* Size of cell */
#define TWR_CELL_SIZE 32

/* Uniform grid cell structure type definition */
typedef struct tagtwrGRID_CELL
{
  INT Count;                                       /* Current number of particles */
  INT ParticleIndices[TWR_MAX_PARTICLES_PER_CELL]; /* Cell particles indices */
} twrGRID_CELL;

/* Sample unit description type */
typedef struct tagtwrUNIT_FLUID
{
  TWR_UNIT_BASE_FIELDS;
  twrMATERIAL_PATTERN *MtlPat; /* Sphere material pattern */
  twrMATERIAL *Mtl;            /* Particles material */

  twrPRIM *ParticlePrim; /* Particle primitive */
  twrVERTEX_STD4 *V;     /* Vertex array */

  INT NumOfParticles; /* Number of particles */
  VEC4 *PosDens;      /* Particle positions array */
  VEC4 *VelPres;      /* Particle velocities array */
  VEC4 *SavePosDens;  /* Previous positions array */

  DBL SmoothingRadius; /* Interaction smoothing radius */
  DBL TargetDensity;   /* Normal density for fluid */

  twrSHADER *ShdCompute[10]; /* Compute shaders */

  /* Buffers (VEC4) */
  twrBUFFER
    *BufSavePos, /* Save positions buffer */
    *BufSaveVel, /* Save velocities buffer */
    *BufPos,     /* Positions buffer */
    *BufVel;     /* Velocities buffer */

  VEC3 MinBB, MaxBB; /* Bound box for simulation */

  twrGRID_CELL Grid[TWR_MAX_CELLS_PER_GRID]; /* Uniform grid */
} twrUNIT_FLUID;

/***
 * SIMULATION HANDLE
 ***/

/* Fluid initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_FluidInit( twrUNIT_FLUID *Uni )
{
  INT i, x = 0, y = 0, z = 0;
  INT off;

  Twr->IsPause = TRUE;
  Uni->NumOfParticles = 512; /* 343 512 729 1000 */
  off = pow(Uni->NumOfParticles, 1 / 3.0);
  Uni->SmoothingRadius = 18.0;
  Uni->TargetDensity = 1.8;
  Uni->MinBB = Vec3Set1(-50), Uni->MaxBB = Vec3Set1(50);
  
  if (Uni->PosDens == NULL || Uni->VelPres == NULL)
  {
    Uni->PosDens = malloc(sizeof(VEC4) * Uni->NumOfParticles);
    Uni->VelPres = malloc(sizeof(VEC4) * Uni->NumOfParticles);
  }
  
  if (Uni->SavePosDens == NULL)
    Uni->SavePosDens = malloc(sizeof(VEC4) * Uni->NumOfParticles);

  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    DBL dx, dy, dz;

    if (z >= off)
      if (y >= off)
        if (x >= off)
          x = y = z = 0;
        else
          z = y = 0, x++;
      else
        z = 0, y++;
    else
      z++;
    dx = x / (FLT)off, dy = y / (FLT)off, dz = z / (FLT)off;
    Uni->PosDens[i] = Vec4MulNum(Vec4Set(2 * dx - 1, 2 * dy - 1, 2 * dz - 1, 1), 30);
    Uni->SavePosDens[i] = Uni->PosDens[i];
    Uni->VelPres[i] = Vec4Set1(0);
  }
} /* End of 'TWR_FluidInit' function */

/* Resolve particle collisions function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_FluidResolveCollisions( twrUNIT_FLUID *Uni )
{
  INT i;
  static DBL collision_dump = 0.47;

  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    if (Uni->PosDens[i].X < Uni->MinBB.X)
      Uni->PosDens[i].X = Uni->MinBB.X, Uni->VelPres[i].X = -Uni->VelPres[i].X * collision_dump;
    if (Uni->PosDens[i].Y < Uni->MinBB.Y)
      Uni->PosDens[i].Y = Uni->MinBB.Y, Uni->VelPres[i].Y = -Uni->VelPres[i].Y * collision_dump;
    if (Uni->PosDens[i].Z < Uni->MinBB.Z)
      Uni->PosDens[i].Z = Uni->MinBB.Z, Uni->VelPres[i].Z = -Uni->VelPres[i].Z * collision_dump;
    if (Uni->PosDens[i].X > Uni->MaxBB.X)
      Uni->PosDens[i].X = Uni->MaxBB.X, Uni->VelPres[i].X = -Uni->VelPres[i].X * collision_dump;
    if (Uni->PosDens[i].Y > Uni->MaxBB.Y)
      Uni->PosDens[i].Y = Uni->MaxBB.Y, Uni->VelPres[i].Y = -Uni->VelPres[i].Y * collision_dump;
    if (Uni->PosDens[i].Z > Uni->MaxBB.Z)
      Uni->PosDens[i].Z = Uni->MaxBB.Z, Uni->VelPres[i].Z = -Uni->VelPres[i].Z * collision_dump;
  }
} /* End of 'TWR_FluidResolveCollisions' function */

/* Handle particles viscosity interaction function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 *   - seconds since last step:
 *       DBL DeltaTime;
 * RETURNS: None.
 */
static VOID TWR_FluidViscosityHandle( twrUNIT_FLUID *Uni, DBL DeltaTime )
{
  INT i, j;
  static DBL sigma = 0, beta = 0.0001;

  for (i = 0; i < Uni->NumOfParticles; i++)
    for (j = i + 1; j < Uni->NumOfParticles; j++)
    {
      VEC4 r = Vec4SubVec4(Uni->PosDens[j], Uni->PosDens[i]);
      DBL q = Vec4Len(r) / Uni->SmoothingRadius;

      if (q < 1)
      {
        DBL u = Vec4DotVec4(Vec4SubVec4(Uni->VelPres[i], Uni->VelPres[j]), r);

        if (u > 0)
        {
          VEC4 I = Vec4MulNum(r, (1 - q) * (sigma * u + beta * u * u) * DeltaTime);

          Uni->VelPres[i] = Vec4SubVec4(Uni->VelPres[i], Vec4MulNum(I, 0.5));
          Uni->VelPres[j] = Vec4AddVec4(Uni->VelPres[j], Vec4MulNum(I, 0.5));
        }
      }
    }
} /* End of 'TWR_FluidViscosityHandle' function */

/* Handle particles density interaction function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 *   - seconds since last step:
 *       DBL DeltaTime;
 * RETURNS: None.
 */
static VOID TWR_FluidDensityHandle( twrUNIT_FLUID *Uni, DBL DeltaTime )
{
  INT i, j;
  static DBL k = 18;

  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    DBL dens = 0, dens_near = 0, pressure, pressure_near;
    VEC4 R = Vec4Set1(0);

    /* Compute density and near-density */
    for (j = 0; j < Uni->NumOfParticles; j++)
    {
      DBL q = Vec4Len(Vec4SubVec4(Uni->PosDens[j], Uni->PosDens[i])) / Uni->SmoothingRadius;

      if (i == j)
        continue;
      if (q < 1)
      {
        dens += (1 - q) * (1 - q);
        dens_near += (1 - q) * (1 - q) * (1 - q);
      }
    }

    /* Compute pressure and near-pressure */
    pressure = k * (dens - Uni->TargetDensity);
    pressure_near = k * dens_near;

    /* Apply displacement R */
    for (j = 0; j < Uni->NumOfParticles; j++)
    {
      VEC4 D;
      DBL q = Vec4Len(Vec4SubVec4(Uni->PosDens[j], Uni->PosDens[i])) / Uni->SmoothingRadius;

      if (i == j)
        continue;
      if (q < 1)
      {
        D = Vec4MulNum(Vec4SubVec4(Uni->PosDens[j], Uni->PosDens[i]), DeltaTime * DeltaTime * (pressure * (1 - q) + pressure_near * (1 - q) * (1 - q)));
        Uni->PosDens[j] = Vec4AddVec4(Uni->PosDens[j], Vec4MulNum(D, 0.5));
        R = Vec4SubVec4(R, Vec4MulNum(D, 0.5));
      }
    }

    Uni->PosDens[i] = Vec4AddVec4(Uni->PosDens[i], R);
  }
} /* End of 'TWR_FluidDensityHandle' function */

/***
 * CELL SYSTEM
 ***/

/* Generate grid cell hash by position function.
 * ARGUMENTS:
 *   - position vector:
 *       VEC3 Pos;
 * RETURNS:
 *   (UINT) cell hash.
 */
static UINT TWR_GetUniformGridHash( VEC3 Pos )
{
  UINT a = (UINT)(Pos.X / TWR_CELL_SIZE) * 15823;
  UINT b = (UINT)(Pos.Y / TWR_CELL_SIZE) * 9737333;
  UINT c = (UINT)(Pos.Z / TWR_CELL_SIZE) * 14386733;

  return (a + b + c) % TWR_MAX_CELLS_PER_GRID;
} /* End of 'TWR_GetUniformGridHash' function */

/* Build uniform grid data function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_BuildUniformGrid( twrUNIT_FLUID *Uni )
{
  INT i;

  /* Clear grid */
  memset(Uni->Grid, 0.0, sizeof(TWR_CELL_SIZE) * TWR_MAX_CELLS_PER_GRID);

  /* Bind particles into grid cells */
  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    UINT key = TWR_GetUniformGridHash(Vec3Set(Uni->PosDens[i].X, Uni->PosDens[i].Y, Uni->PosDens[i].Z));

    /* Add particle to cell if there's space */
    if (Uni->Grid[key].Count < TWR_MAX_PARTICLES_PER_CELL)
      Uni->Grid[key].ParticleIndices[Uni->Grid[key].Count++] = i;
  }
} /* End of 'TWR_BuildUniformGrid' function */

/* Fluid each step function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 *   - seconds since last step:
 *       DBL DeltaTime;
 * RETURNS: None.
 */
static VOID TWR_FluidStep( twrUNIT_FLUID *Uni, DBL DeltaTime )
{
  INT i;

  if (DeltaTime == 0 || Twr->IsPause)
    return;

  /* Build uniform grid */
  TWR_BuildUniformGrid(Uni);

  /* Apply viscosity */
  TWR_FluidViscosityHandle(Uni, DeltaTime);

  /* Apply gravity */
  for (i = 0; i < Uni->NumOfParticles; i++)
    Uni->VelPres[i].Y += -9.81 * DeltaTime;

  /* Advance to predicted position */
  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    Uni->SavePosDens[i] = Uni->PosDens[i];
    Uni->PosDens[i] = Vec4AddVec4(Uni->PosDens[i], Vec4MulNum(Uni->VelPres[i], DeltaTime));
  }

  /* Handle density */ 
  TWR_FluidDensityHandle(Uni, DeltaTime);

  /* Resolve collisions */
  TWR_FluidResolveCollisions(Uni);

  /* Use previous position to compute next velocity */
  for (i = 0; i < Uni->NumOfParticles; i++)
    Uni->VelPres[i] = Vec4DivNum(Vec4SubVec4(Uni->PosDens[i], Uni->SavePosDens[i]), DeltaTime);
} /* End of 'TWR_FluidStep' function */

/* Fluid each step for GPU function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 *   - seconds since last step:
 *       DBL DeltaTime;
 * RETURNS: None.
 */
static VOID TWR_FluidStepCompute( twrUNIT_FLUID *Uni, DBL DeltaTime )
{
  INT i;
  static BOOL init_flag = TRUE;

  /* Initialization for GPU */
  if (init_flag)
  {
    init_flag = FALSE;
    /* Shaders initialization */
    Uni->ShdCompute[0] = Twr->ShdCreate("samples/ab7/fluid/compute_0"); /* Viscosity */
    Uni->ShdCompute[1] = Twr->ShdCreate("samples/ab7/fluid/compute_1"); /* Gravity & advance */
    Uni->ShdCompute[2] = Twr->ShdCreate("samples/ab7/fluid/compute_2"); /* Density 1 */
    Uni->ShdCompute[3] = Twr->ShdCreate("samples/ab7/fluid/compute_3"); /* Density 2 */
    Uni->ShdCompute[4] = Twr->ShdCreate("samples/ab7/fluid/compute_4"); /* Collisions & velocity */
    /* Buffers initialization */
    Uni->BufSavePos = Twr->BufCreate("BufSavePos", 7, sizeof(VEC4) * Uni->NumOfParticles, Uni->PosDens);
    Uni->BufSaveVel = Twr->BufCreate("BufSaveVel", 8, sizeof(VEC4) * Uni->NumOfParticles, Uni->VelPres);
    Uni->BufSavePos->RefCounter++, Uni->BufSaveVel->RefCounter++;
    Twr->BufApply(Uni->BufSavePos), Twr->BufApply(Uni->BufSaveVel);
    Uni->BufPos = Twr->BufCreate("BufPos", 9, sizeof(VEC4) * Uni->NumOfParticles, Uni->PosDens);
    Uni->BufVel = Twr->BufCreate("BufVel", 10, sizeof(VEC4) * Uni->NumOfParticles, Uni->VelPres);
    Uni->BufPos->RefCounter++, Uni->BufVel->RefCounter++;
    Twr->BufApply(Uni->BufPos), Twr->BufApply(Uni->BufVel);
  }

  /* Viscosity */
  if (Twr->ShdApply(Uni->ShdCompute[0], TWR_MTL_COMPUTE))
    for (i = 0; i < Uni->NumOfParticles; i++)
    {
      glUniform1i(7, i);            
      glDispatchCompute((Uni->NumOfParticles / 1024 + (Uni->NumOfParticles % 1024 == 0 ? 0 : 1)), 1, 1);
    }
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  /* Gravity & advance */
  if (Twr->ShdApply(Uni->ShdCompute[1], TWR_MTL_COMPUTE))
  {
    glDispatchCompute((Twr->NumOfGlobalMatrixes / 1024 + (Twr->NumOfGlobalMatrixes % 1024 == 0 ? 0 : 1)), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }

  /* Density 1 */
  if (Twr->ShdApply(Uni->ShdCompute[2], TWR_MTL_COMPUTE))
    for (i = 0; i < Uni->NumOfParticles; i++)
    {
      glUniform1i(7, i);            
      glDispatchCompute((Uni->NumOfParticles / 1024 + (Uni->NumOfParticles % 1024 == 0 ? 0 : 1)), 1, 1);
    }
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  /* Density 2 */
  if (Twr->ShdApply(Uni->ShdCompute[3], TWR_MTL_COMPUTE))
    for (i = 0; i < Uni->NumOfParticles; i++)
    {
      glUniform1i(7, i);            
      glDispatchCompute((Uni->NumOfParticles / 1024 + (Uni->NumOfParticles % 1024 == 0 ? 0 : 1)), 1, 1);
    }
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  /* Collision & velocity */
  if (Twr->ShdApply(Uni->ShdCompute[4], TWR_MTL_COMPUTE))
  {
    glDispatchCompute((Twr->NumOfGlobalMatrixes / 1024 + (Twr->NumOfGlobalMatrixes % 1024 == 0 ? 0 : 1)), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
} /* End of 'TWR_FluidStepCompute' function */

/***
 * UNIT HANDLE
 ***/

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_FLUID *Uni )
{
  INT i;

  /* Utility initialization */
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  Uni->MtlPat = Twr->MtlPatCreate("Particle material pattern", "samples/ab7/fluid", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MtlPat->RefCounter++;

  TWR_FluidInit(Uni);

  /* Particle primitive initialization */
  Uni->Mtl = Twr->MtlCreate("Particle material", Uni->MtlPat, FALSE);
  Uni->Mtl->RefCounter++;
  Uni->V = malloc(sizeof(twrVERTEX_STD4) * Uni->NumOfParticles);
  for (i = 0; i < Uni->NumOfParticles; i++)
    Uni->V[i].P = Vec3Set(Uni->PosDens[i].X, Uni->PosDens[i].Y, Uni->PosDens[i].Z);
  Uni->ParticlePrim = Twr->PrimCreate(Uni->Mtl, TWR_RND_PRIM_POINTS, Uni->V, Uni->NumOfParticles, NULL, 0);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_FLUID *Uni )
{
  /* Free shaders */
  Twr->ShdFree(Uni->ShdCompute[0]);
  Twr->ShdFree(Uni->ShdCompute[1]);
  Twr->ShdFree(Uni->ShdCompute[2]);
  Twr->ShdFree(Uni->ShdCompute[3]);
  Twr->ShdFree(Uni->ShdCompute[4]);

  /* Free buffers */
  Twr->BufFree(Uni->BufSavePos);
  Twr->BufFree(Uni->BufSaveVel);
  Twr->BufFree(Uni->BufPos);
  Twr->BufFree(Uni->BufVel);
  Twr->MtlFree(Uni->Mtl);
  Twr->MtlPatFree(Uni->MtlPat);

  free(Uni->V);
  free(Uni->PosDens);
  free(Uni->VelPres);
  free(Uni->SavePosDens);
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_FLUID *Uni )
{
  static BOOL compute_flag = FALSE;
  static DBL time = 0;
  INT i;
  DBL delta = Twr->Time - time;

  if (Twr->KeysClick['I'] || delta >= 18.0)
  {
    TWR_FluidInit(Uni);
    time = Twr->Time;
  }
  
  if (Twr->KeysClick['C'])
    compute_flag = !compute_flag;

  if (!compute_flag)
    TWR_FluidStep(Uni, 0.1 /*Twr->DeltaTime*/ );
  else
    TWR_FluidStepCompute(Uni, 0.1 /*Twr->DeltaTime*/ );

  /* Update vertex buffer */
  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    FLT velocity = Vec4Len(Uni->VelPres[i]);

    velocity = velocity * velocity * velocity / 3000;
    Uni->V[i].P = Vec3Set(Uni->PosDens[i].X, Uni->PosDens[i].Y, Uni->PosDens[i].Z);
    Uni->V[i].C = Vec4Set(velocity, 1 - 0.5 * velocity, 1 - velocity, 1.0);
  }

  glBindBuffer(GL_ARRAY_BUFFER, Uni->ParticlePrim->VBuf);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(twrVERTEX_STD4) * Uni->NumOfParticles, Uni->V);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
} /* End of 'TWR_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 *   - particle id:
 *       INT ParticleId;
 *   - sphere radius:
 *       VEC4 Radius;
 * RETURNS: None.
 */
static VOID TWR_UnitDrawSphere( twrUNIT_FLUID *Uni, INT ParticleId, VEC4 Radius )
{
  FLT velocity = Vec2Len(Vec2Set(Uni->VelPres[ParticleId].X, Uni->VelPres[ParticleId].Z));

  velocity = velocity * velocity * velocity * velocity / 5000;
  Twr->MrkDrawSphere(
    Vec3Set(Uni->PosDens[ParticleId].X, Uni->PosDens[ParticleId].Y, Uni->PosDens[ParticleId].Z),
    Vec3Set(Radius.X, Radius.Y, Radius.Z), Vec4Set(velocity, 1 - 0.5 * velocity, 1 - velocity, 1.0), MatrIdentity());
} /* End of 'TWR_UnitDrawSphere' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_FLUID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_FLUID *Uni )
{
  INT i;
  BYTE *bits;
  
  bits = Twr->BufRead(Uni->BufPos);
  if (bits != NULL)
  {
    memcpy(Uni->PosDens, bits, sizeof(VEC4) * Uni->NumOfParticles);
    free(bits);
  }
  bits = Twr->BufRead(Uni->BufVel);
  if (bits != NULL)
  {
    memcpy(Uni->VelPres, bits, sizeof(VEC4) * Uni->NumOfParticles);
    free(bits);
  }

  for (i = 0; i < Uni->NumOfParticles; i++)
  {
    UINT key = TWR_GetUniformGridHash(Vec3Set(Uni->PosDens[i].X, Uni->PosDens[i].Y, Uni->PosDens[i].Z));

    //TWR_UnitDrawSphere(Uni, i, Vec4Set1(3));
    Twr->MrkDrawSphere(
      Vec3Set(Uni->PosDens[i].X, Uni->PosDens[i].Y, Uni->PosDens[i].Z),
      Vec3Set1(3), Vec4Set((FLT)key / TWR_MAX_CELLS_PER_GRID, 1 - (FLT)key / TWR_MAX_CELLS_PER_GRID, 1.0, 1.0), MatrIdentity());
  }
  //Twr->PrimDraw(Uni->ParticlePrim, MatrIdentity());
  /* 
  Bound box
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr(MatrTranslate(Vec4Set(Uni->MinBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MinBB.Z)), MatrScale(Vec4Set(1, (Uni->MaxBB.Y - Uni->MinBB.Y) * 0.25, 1))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr(MatrTranslate(Vec4Set(Uni->MaxBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MinBB.Z)), MatrScale(Vec4Set(1, (Uni->MaxBB.Y - Uni->MinBB.Y) * 0.25, 1))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr(MatrTranslate(Vec4Set(Uni->MinBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MaxBB.Z)), MatrScale(Vec4Set(1, (Uni->MaxBB.Y - Uni->MinBB.Y) * 0.25, 1))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr(MatrTranslate(Vec4Set(Uni->MaxBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MaxBB.Z)), MatrScale(Vec4Set(1, (Uni->MaxBB.Y - Uni->MinBB.Y) * 0.25, 1))));

  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MinBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MinBB.Z)), MatrRotateZ(90), MatrScale(Vec4Set((Uni->MaxBB.X - Uni->MinBB.X) * 0.25, 1, 1))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MinBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MaxBB.Z)), MatrRotateZ(90), MatrScale(Vec4Set((Uni->MaxBB.X - Uni->MinBB.X) * 0.25, 1, 1))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MaxBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MinBB.Z)), MatrRotateZ(90), MatrScale(Vec4Set((Uni->MaxBB.X - Uni->MinBB.X) * 0.25, 1, 1))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MaxBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MaxBB.Z)), MatrRotateZ(90), MatrScale(Vec4Set((Uni->MaxBB.X - Uni->MinBB.X) * 0.25, 1, 1))));

  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MinBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MinBB.Z)), MatrRotateX(90), MatrScale(Vec4Set(1, 1, (Uni->MaxBB.X - Uni->MinBB.X) * 0.25))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MinBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MaxBB.Z)), MatrRotateX(90), MatrScale(Vec4Set(1, 1, (Uni->MaxBB.X - Uni->MinBB.X) * 0.25))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MaxBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MinBB.Z)), MatrRotateX(90), MatrScale(Vec4Set(1, 1, (Uni->MaxBB.X - Uni->MinBB.X) * 0.25))));
  Twr->ModelDraw(Uni->CylinderMrk,
    MatrMulMatr3(MatrTranslate(Vec4Set(Uni->MaxBB.X, Uni->MinBB.Y + Uni->MaxBB.Y, Uni->MaxBB.Z)), MatrRotateX(90), MatrScale(Vec4Set(1, 1, (Uni->MaxBB.X - Uni->MinBB.X) * 0.25))));
  */
} /* End of 'TWR_UnitRender' function */

/* AB7 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateFluid( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_FLUID))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateFluid' function */

/* END OF 'twr_unit_fluid.c' FILE */
