/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_compute.c
 * PURPOSE     : Tough Water Rendering project.
 *               Compute inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Memery allocation for buffer constant */
#define TWR_FORWARD_ALLOCATION_MATRS 300

/* Filling global matrixes array for buffer for compute.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_GlobalMatrixesArrayFill( VOID )
{
  twrMATERIAL_PATTERN *MtlPat;
  twrMATERIAL*Mtl;
  twrPRIM *Pr;

  MtlPat = Twr->FirstDraw;
  while (MtlPat != NULL)
  {
    Mtl = MtlPat->FirstDraw;
    while (Mtl != NULL)
    {
      Pr = Mtl->FirstDraw;
      while (Pr != NULL)
      {
        if (Pr->NumOfInstances + Twr->NumOfGlobalMatrixes >= Twr->MaxNumOfGlobalMatrixes)
        {
          MATR *block;
          /* For better memory allocation without not correct allocation memory for array */
          INT NeedMemSize = Pr->NumOfInstances + Twr->NumOfGlobalMatrixes - Twr->MaxNumOfGlobalMatrixes;
          
          /* Allocate a block with new size */
          block = malloc(sizeof(MATR) * (Twr->MaxNumOfGlobalMatrixes + (NeedMemSize / TWR_FORWARD_ALLOCATION_MATRS + 1) * TWR_FORWARD_ALLOCATION_MATRS));
          if (block == NULL)
            return;
          if (Twr->GlobalMatrixes != NULL)
            memcpy(block, Twr->GlobalMatrixes, sizeof(MATR) * Twr->NumOfGlobalMatrixes);
          free(Twr->GlobalMatrixes);
          Twr->GlobalMatrixes = block;
          Twr->MaxNumOfGlobalMatrixes = Twr->MaxNumOfGlobalMatrixes + (NeedMemSize / TWR_FORWARD_ALLOCATION_MATRS + 1) * TWR_FORWARD_ALLOCATION_MATRS;
        }
        memcpy(&Twr->GlobalMatrixes[Twr->NumOfGlobalMatrixes], Pr->WorldMatrixes, sizeof(MATR) * Pr->NumOfInstances);
        Pr->FirstMatrixIndex = Twr->NumOfGlobalMatrixes;
        Twr->NumOfGlobalMatrixes += Pr->NumOfInstances;
        Pr = Pr->NextDraw;
      }
      Mtl = Mtl->NextDraw;
    }
    MtlPat = MtlPat->NextDraw;
  }
} /* End of 'TWR_GlobalMatrixesArrayFill' function */

/* Matrixes buffers for compute shader update.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_GlobalMatrBufsUpdate( VOID )
{
  /* Don't resize buffer if it's size is bigger then needed */
  if (Twr->BuffersSize < Twr->NumOfGlobalMatrixes)
  {
    Twr->BufResize(Twr->WorldMatrixesBuffer, (sizeof(MATR) * Twr->NumOfGlobalMatrixes));
    Twr->BufResize(Twr->ComputedMatrixesBuffer, (2 * sizeof(MATR) * Twr->NumOfGlobalMatrixes));

    /* Remember new biggest size of buffer */
    Twr->BuffersSize = Twr->NumOfGlobalMatrixes;
  }

  Twr->BufUpdate(Twr->WorldMatrixesBuffer, 0, 0, Twr->GlobalMatrixes);
} /* End of 'TWR_GlobalMatrBufsUpdate' function */

/* Matrix evaluation by compute shader.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_GlobalMatrBufsCompute( VOID )
{
  if (Twr->ShdApply(Twr->ComputeShd, TWR_MTL_COMPUTE))
  {
    glDispatchCompute((Twr->NumOfGlobalMatrixes / 1024 + (Twr->NumOfGlobalMatrixes % 1024 == 0 ? 0 : 1)), 1, 1);
    /* glDispatchCompute(1020, 1020, 1020); */
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
} /* End of 'TWR_GlobalMatrBufsCompute' function */

/* Compute system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemComputeInit( VOID )
{
  Twr->GlobalMatrBufsUpdate = TWR_GlobalMatrBufsUpdate;
  Twr->GlobalMatrBufsCompute = TWR_GlobalMatrBufsCompute;
  Twr->GlobalMatrixesArrayFill = TWR_GlobalMatrixesArrayFill;

  /* Creating buffers */
  Twr->WorldMatrixesBuffer = Twr->BufCreate("Matrix Buffer for compute", TWR_BUF_BIND_MATRIX_IN, 0, NULL);
  Twr->BufReferenceInc(Twr->WorldMatrixesBuffer);
  Twr->ComputedMatrixesBuffer = Twr->BufCreate("Matrix Buffer of WVP and WInv", TWR_BUF_BIND_MATRIX_OUT, 0, NULL);
  Twr->BufReferenceInc(Twr->ComputedMatrixesBuffer);

  Twr->BuffersSize = 0;

  /* Applying buffers */
  Twr->BufApply(Twr->WorldMatrixesBuffer);
  Twr->BufApply(Twr->ComputedMatrixesBuffer);
  
  /* Initialization of matrixes flex array */
  Twr->GlobalMatrixes = NULL;
  Twr->NumOfGlobalMatrixes = 0;
  Twr->MaxNumOfGlobalMatrixes = 0;

  /* Creating compute shader */
  Twr->ComputeShd = Twr->ShdCreate("matrix");

  Twr->Sys("Compute system is initialized");
} /* End of 'TWR_SystemComputeInit' function */

/* Compute system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemComputeClose( VOID )
{
  /* Free buffers */
  Twr->BufFree(Twr->WorldMatrixesBuffer);
  Twr->BufFree(Twr->ComputedMatrixesBuffer);

  Twr->BuffersSize = 0;

  /* Free matrixes array */
  free(Twr->GlobalMatrixes);
  Twr->GlobalMatrixes = NULL;
  Twr->NumOfGlobalMatrixes = 0;
  Twr->MaxNumOfGlobalMatrixes = 0;
  
  /* Free compute shader */
  Twr->ShdFree(Twr->ComputeShd);

  Twr->Sys("Compute system is deinitialized");
} /* End of 'TWR_SystemComputeClose' function */

/* END OF 'twr_rnd_compute.c' FILE */
