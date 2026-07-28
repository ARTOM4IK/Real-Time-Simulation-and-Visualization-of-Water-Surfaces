/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_compute.c
 * PURPOSE     : Tough Water Rendering project.
 *               Scenes inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 19.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "units/twr_units.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TWR_FORWARD_ALLOCATION_SCENES 8
#define TWR_FORWARD_ALLOCATION_UNITS  8

/* Get scene number in scenes array function
 * ARGUMENTS:
 *   - name of scene:
 *       CHAR *NameOfScene;
 * RETURNS:
 *   (INT) number of scene in scenes array (-1 if no scene).
 */
static INT TWR_GetSceneNumByName( CHAR *NameOfScene )
{
  INT i;

  for (i = 0; i < Twr->NumOfScenes; i++)
  {
    if (strcmp(Twr->Scenes[i].SceneName, NameOfScene) == 0)
    {
      return i;
    }
  }
  return -1;
} /* End of 'TWR_GetSceneNumByName' function */

/* Get scene name by its number in scenes array function
 * ARGUMENTS:
 *   - number of scene in scenes array:
 *       CHAR *NameOfScene;
 * RETURNS:
 *   (CHAR *) name of scene.
 */
static CHAR *TWR_GetSceneNameByNum( INT SceneNum )
{
  if (Twr->NumOfScenes >= SceneNum)
    return Twr->Scenes[SceneNum].SceneName;
  else
    return NULL;
} /* End of 'TWR_GetSceneNameByNum' function */

/* Scene setting by its name function
 * ARGUMENTS: 
 *   - name of scene:
 *       CHAR *NameOfScene; 
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE if it is no scene.
 */
static BOOL TWR_SceneSetByName( CHAR *NameOfScene )
{
  INT i, j;
  BOOL ret = FALSE;

  StockDeleteAllInstant(&Twr->StkUnits);
  
  for (i = 0; i < Twr->NumOfScenes; i++)
  {
    if (strcmp(Twr->Scenes[i].SceneName, NameOfScene) == 0)
    {
      for (j = 0; j < Twr->Scenes[i].NumOfUnits; j++)
        Twr->UnitAddByNum(Twr->Scenes[i].UnitsNum[j]);
      Twr->UsingSceneNum = i;
      if (Twr->Scenes[i].CamLoc.X == 0 && Twr->Scenes[i].CamLoc.Y == 0 && Twr->Scenes[i].CamLoc.Z == 0 && Twr->Scenes[i].CamAt.X == 0 && Twr->Scenes[i].CamAt.Y == 0 && Twr->Scenes[i].CamAt.Z == 0)
        CamSet(&Twr->Cam, Twr->Cam.Loc, Twr->Cam.At, Vec3Set(0, 1, 0));
      else
        CamSet(&Twr->Cam, Twr->Scenes[i].CamLoc, Twr->Scenes[i].CamAt, Vec3Set(0, 1, 0));
      ret = TRUE;
      break;
    }
  }
  
  if (strcmp("Common", NameOfScene) != 0)
  {
    for (i = 0; i < Twr->NumOfScenes; i++)
    {
      if (strcmp(Twr->Scenes[i].SceneName, "Common") == 0)
      {
        for (j = 0; j < Twr->Scenes[i].NumOfUnits; j++)
          Twr->UnitAddByNum(Twr->Scenes[i].UnitsNum[j]);
        break;
      }
    }
  }

  if (!ret)
    Twr->Error("No such scene - %s\n", NameOfScene);
  else
    Twr->Info("Scene setted - %s\n", NameOfScene);
  return ret;
} /* End of 'TWR_SceneSetByName' function */

/* Scene setting by its name function
 * ARGUMENTS: 
 *   - name of scene:
 *       CHAR *NameOfScene; 
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE if it is no scene.
 */
static BOOL TWR_SceneSetByNum( INT SceneNum )
{
  CHAR *SceneName = TWR_GetSceneNameByNum(SceneNum);
  
  if (SceneName != NULL)
    return TWR_SceneSetByName(SceneName);
  else
    return FALSE;
} /* End of 'TWR_SceneSetByNum' function */

/* Scene loading function without adding scene to
 * ARGUMENTS: 
 *   - name of file, from which scene will be loaded:
 *       CHAR *FileName; 
 *   - name of loading scene:
 *       CHAR *SceneName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE if it is no file or scene.
 */
static BOOL TWR_SceneLoad( CHAR *FileName, CHAR *SceneName )
{
  FILE *F;
  BOOL IsFound = FALSE, IsCommon = FALSE;
  INT NumOfUnits = 0, i;
  CHAR SceneUnits[TWR_STR_MAX][102] = {0};
  static CHAR Buf[3000], *S;
  
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  while (fgets(Buf, sizeof(Buf), F))
  {  
    S = strtok(Buf, "\n");
    while (S != NULL)
    {
      if (IsFound || IsCommon)
      {
        if (strstr(S, "<") != NULL)
        {
          if (IsCommon)
          {
            IsCommon = FALSE;
            continue;
          }
          else
          {
            fclose(F);
            Twr->SetColor(0xFF1818, 0);
            Twr->Info("Scene loading: %s", SceneName);
            StockDeleteAllInstant(&Twr->StkUnits);
            for (i = 0; i < NumOfUnits; i++)
            {
              Twr->UnitAddByName(SceneUnits[i]);
            }

            Twr->SetColor(0xFF8833, 0);
            Twr->Log("\nScene loaded: %s", SceneName);
            for (i = 0; i < NumOfUnits; i++)
            {
              Twr->Log("\n>>Unit: %s", SceneUnits[i]);
            }

            return TRUE;
          }
        }
        else
        {
          while (S != NULL)
          {
            if (NumOfUnits < 300)
            {
              strncpy(SceneUnits[NumOfUnits], S + 1, strlen(S) - 3);
              NumOfUnits++;
            }
            S = strtok(NULL, "\n");
          }
        }
      }
      else
      {
        if (strstr(S, SceneName) != NULL)
          IsFound = TRUE;
        if (strstr(S, "<Common>") != NULL)
          IsCommon = TRUE;
      }
      S = strtok(NULL, "\n");
    }
  }
  
  if (!IsFound)
  {
    fclose(F);
    return FALSE;
  }
  
  StockDeleteAllInstant(&Twr->StkUnits);
  fclose(F);
  for (i = 0; i < NumOfUnits; i++)
  {
    Twr->UnitAddByName(SceneUnits[i]);
  }

  Twr->SetColor(0xFF8833, 0);
  Twr->Log("\nScene loaded: %s", SceneName);
  for (i = 0; i < NumOfUnits; i++)
  {
    Twr->Log("\n>>Unit: %s", SceneUnits[i]);
  }

  return TRUE;
} /* End of 'TWR_SceneLoad' function */

/* Getting number from string function
 * ARGUMENTS:
 *   - string:
 *       CHAR *Buf;
 *   - start of number in string:
 *       INT Start;
 *   - character to stop reading number:
 *       CHAR Ch;
 *   - pointer of number in string:
 *       INT *End;
 * RETURNS:
 *   (FLT) number.
 */
static FLT TWR_GetNumFromStr( CHAR *Buf, INT Start, CHAR Ch, INT *End )
{
  FLT Num = 0;
  INT d = 1, k = Start;
  BOOL IsDouble = FALSE, IsNegative = FALSE;

  while (Buf[k] != Ch && Buf[k] != 0)
  {
    if (Buf[k] == '.')
    {
      IsDouble = TRUE;
      d = 1;
      k++;
      continue;
    }
    if (IsDouble)
      Num *= 10;
    else
      Num *= d;
    d *= 10;
    if (Buf[k] == '-')
    {
      IsNegative = TRUE;
      d /= 10;
      k++;
      continue;
    }
    Num += Buf[k++] - '0';
  }
  if (IsDouble)
    Num /= d, IsDouble = FALSE;
  if (IsNegative)
    Num *= -1, IsNegative  = FALSE;
  
  if (End != NULL)
    *End = k;
  
  return Num;
} /* End of 'TWR_GetNumFromStr' function */

/* Loading all scenes from file to scene array function
 * ARGUMENTS: 
 *   - name of file, from which scene will be loaded:
 *       CHAR *FileName; 
 * RETURNS: None.
 */
static VOID TWR_FileScenesLoad( CHAR *FileName )
{
  FILE *F;
  INT i = 0, j = 0, k;
  BOOL IsExist = FALSE;
  INT NameStart = 0;
  CHAR Buf[300], *S;
  VEC3 Loc, At;
  
  if ((F = fopen(FileName, "rb")) == NULL)
    return;

  while (fgets(Buf, sizeof(Buf), F))
  {
    i = 0;
    while (Buf[i] != 0)
    {
      if (Buf[i] == '<')
      {
        /* Scene name founded */
        NameStart = i;

        if (Twr->NumOfScenes >= Twr->MaxNumOfScenes)
        {
          twrSCENE *block;
          
          block = malloc(sizeof(twrSCENE) * (Twr->MaxNumOfScenes + TWR_FORWARD_ALLOCATION_SCENES));
          if (block == NULL)
          {
            Twr->Error("Not enought memory");
            fclose(F);
            return;
          }
          if (Twr->Scenes != NULL)
            memcpy(block, Twr->Scenes, sizeof(twrSCENE) * Twr->NumOfScenes);
          free(Twr->Scenes);
          Twr->Scenes = block;
          Twr->MaxNumOfScenes = Twr->MaxNumOfScenes + TWR_FORWARD_ALLOCATION_SCENES;
        }
        
        memset(&Twr->Scenes[Twr->NumOfScenes], 0, sizeof(Twr->Scenes[Twr->NumOfScenes]) - 1);
        i++;

        /* Write scene name to flex array */
        while (Buf[i] != '>')
        {
          if (Buf[i] == 0)
          {
            Twr->SetColor(0xFF3030, 0);
            Twr->Info("%i loaded scenes at all:\n", Twr->NumOfScenes);
            Twr->SetColor(0x47AA47, 0);
            for (i = 0; i < Twr->NumOfScenes; i++)
              Twr->Log("%s\n", Twr->Scenes[i].SceneName);
            fclose(F);
            return;
          }
          i++;
        }
        strncpy(Twr->Scenes[Twr->NumOfScenes].SceneName, Buf + 1, (i - NameStart - 1));
        
        for (k = 0; k < Twr->NumOfScenes; k++)
        {
          if (strcmp(Twr->Scenes[k].SceneName, Twr->Scenes[Twr->NumOfScenes].SceneName) == 0)
          {
            IsExist = TRUE;
          }
        }
        if (IsExist)
        {
          IsExist = FALSE;
          continue;
        }
        j += i;
        Twr->Scenes[Twr->NumOfScenes].UnitsNum = NULL;
        Twr->Scenes[Twr->NumOfScenes].NumOfUnits = Twr->Scenes[Twr->NumOfScenes].MaxNumOfUnits = 0;
        if (!fgets(Buf, sizeof(Buf), F))
        {
          Twr->SetColor(0xFF3030, 0);
          Twr->Info("%i loaded scenes at all:\n", Twr->NumOfScenes);
          Twr->SetColor(0x47AA47, 0);
          for (i = 0; i < Twr->NumOfScenes; i++)
            Twr->Log("%s\n", Twr->Scenes[i].SceneName);
          fclose(F);
          return;
        }
        Twr->SetColor(0x4747FF, 0);
        Twr->Log("\nScene loading: %s\n", Twr->Scenes[Twr->NumOfScenes].SceneName);

        Loc = Vec3Set1(0);
        At = Vec3Set1(0);

        /* Loc */
        if (Buf[0] == 'l' && Buf[1] == 'o' && Buf[2] == 'c' && Buf[3] == ':')
        {
          k = 5;
          Loc.X = TWR_GetNumFromStr(Buf, k, ',', &k);
          k += 2;
          Loc.Y = TWR_GetNumFromStr(Buf, k, ',', &k);
          k += 2;
          Loc.Z = TWR_GetNumFromStr(Buf, k, ';', NULL);
          
          j += strlen(Buf);

          if (!fgets(Buf, sizeof(Buf), F))
          {
            Twr->SetColor(0xFF3030, 0);
            Twr->Info("%i loaded scenes at all:\n", Twr->NumOfScenes);
            Twr->SetColor(0x47AA47, 0);
            for (i = 0; i < Twr->NumOfScenes; i++)
              Twr->Log("%s\n", Twr->Scenes[i].SceneName);
            fclose(F);
            return;
          }
        }

        /* At */
        if (Buf[0] == 'a' && Buf[1] == 't' && Buf[2] == ':')
        {
          k = 4;
          At.X = TWR_GetNumFromStr(Buf, k, ',', &k);
          k += 2;
          At.Y = TWR_GetNumFromStr(Buf, k, ',', &k);
          k += 2;
          At.Z = TWR_GetNumFromStr(Buf, k, ';', NULL);
          
          j += strlen(Buf);

          if (!fgets(Buf, sizeof(Buf), F))
          {
            Twr->SetColor(0xFF3030, 0);
            Twr->Info("%i loaded scenes at all:\n", Twr->NumOfScenes);
            Twr->SetColor(0x47AA47, 0);
            for (i = 0; i < Twr->NumOfScenes; i++)
              Twr->Log("%s\n", Twr->Scenes[i].SceneName);
            fclose(F);
            return;
          }
        }

        Twr->Scenes[Twr->NumOfScenes].CamLoc = Loc;
        Twr->Scenes[Twr->NumOfScenes].CamAt = At;

        Twr->Log("Loc: %f, %f, %f;\n", Twr->Scenes[Twr->NumOfScenes].CamLoc.X, Twr->Scenes[Twr->NumOfScenes].CamLoc.Y, Twr->Scenes[Twr->NumOfScenes].CamLoc.Z);
        Twr->Log("At: %f, %f, %f;\n", Twr->Scenes[Twr->NumOfScenes].CamAt.X, Twr->Scenes[Twr->NumOfScenes].CamAt.Y, Twr->Scenes[Twr->NumOfScenes].CamAt.Z); 

        /* Reading units */
        while (Buf[0] != '<')
        {
          if (Twr->Scenes[Twr->NumOfScenes].NumOfUnits >= Twr->Scenes[Twr->NumOfScenes].MaxNumOfUnits)
          {
            INT *block;
            
            block = malloc(sizeof(INT) * (Twr->Scenes[Twr->NumOfScenes].MaxNumOfUnits + TWR_FORWARD_ALLOCATION_UNITS));
            if (block == NULL)
            {
              Twr->Error("Not enought memory");
              fclose(F);
              return;
            }
            if (Twr->Scenes[Twr->NumOfScenes].UnitsNum != NULL)
              memcpy(block, Twr->Scenes[Twr->NumOfScenes].UnitsNum, sizeof(INT) * Twr->Scenes[Twr->NumOfScenes].NumOfUnits);
            free(Twr->Scenes[Twr->NumOfScenes].UnitsNum);
            Twr->Scenes[Twr->NumOfScenes].UnitsNum = block;
            Twr->Scenes[Twr->NumOfScenes].MaxNumOfUnits = Twr->Scenes[Twr->NumOfScenes].MaxNumOfUnits + TWR_FORWARD_ALLOCATION_UNITS;
          }
          k = 1;
          while (Buf[k] != '"' && Buf[k] != 0)
          {
            Buf[k++];
          }
          
          if ((S = malloc(sizeof(CHAR) * k)) == NULL)
            return;
          strncpy(S, Buf + 1, k - 1);
          strncpy(S + k - 1, "\0", 1);
          if ((Twr->Scenes[Twr->NumOfScenes].UnitsNum[Twr->Scenes[Twr->NumOfScenes].NumOfUnits] = Twr->GetUnitNum(S)) != -1)
          {
            Twr->Scenes[Twr->NumOfScenes].NumOfUnits++;
            Twr->SetColor(0x33CCFF, 0);
            Twr->Log("Unit loaded - %s\n", S);
          }
          free(S);
          S = NULL;

          j += strlen(Buf);
          if (!fgets(Buf, sizeof(Buf), F))
          {
            Twr->SetColor(0xFF3030, 0);
            Twr->Info("%i loaded scenes at all:\n", Twr->NumOfScenes);
            Twr->SetColor(0x47AA47, 0);
            Twr->NumOfScenes++;
            if (Twr->Scenes[Twr->NumOfScenes - 1].NumOfUnits == 0)
            {
              free(Twr->Scenes[Twr->NumOfScenes - 1].UnitsNum);
              Twr->Scenes[Twr->NumOfScenes - 1].UnitsNum = NULL;
              Twr->NumOfScenes--;
            }
            for (i = 0; i < Twr->NumOfScenes; i++)
              Twr->Log("%s\n", Twr->Scenes[i].SceneName);
            fclose(F);
            return;
          }
          i = strlen(Buf);
        }
        rewind(F);
        fseek(F, j + 1, SEEK_SET);
        Twr->NumOfScenes++;
        if (Twr->Scenes[Twr->NumOfScenes - 1].NumOfUnits == 0)
        {
          free(Twr->Scenes[Twr->NumOfScenes - 1].UnitsNum);
          Twr->Scenes[Twr->NumOfScenes - 1].UnitsNum = NULL;
          Twr->NumOfScenes--;
        }
      }
      /* No scene */
      else
        i++;
    }
  }
  
  Twr->SetColor(0xFF3030, 0);
  Twr->Log("\n%i loaded scenes at all:\n", Twr->NumOfScenes);
  for (i = 0; i < Twr->NumOfScenes; i++)
    Twr->Log("%s\n", Twr->Scenes[i].SceneName);

  fclose(F);
} /* End of 'TWR_FileScenesLoad' function */

/* Scenes system initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemScenesInit( VOID )
{
  Twr->SceneLoad = TWR_SceneLoad;
  Twr->FileScenesLoad = TWR_FileScenesLoad;
  Twr->GetSceneNumByName = TWR_GetSceneNumByName;
  Twr->GetSceneNameByNum = TWR_GetSceneNameByNum;
  Twr->SceneSetByName = TWR_SceneSetByName;
  Twr->SceneSetByNum = TWR_SceneSetByNum;

  Twr->Scenes = NULL;
  Twr->NumOfScenes = 0;
  Twr->MaxNumOfScenes = 0;
  Twr->IsNextSceneToSet = FALSE;

  Twr->Sys("Scenes system is initialized");
} /* End of 'TWR_ScenesSystemInit' function */

/* Scenes system deinitialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemScenesClose( VOID )
{
  INT i;

  StockFree(&Twr->StkUnits);

  for (i = 0; i < Twr->NumOfScenes; i++)
  {
    free(Twr->Scenes[i].UnitsNum);
    Twr->Scenes[i].UnitsNum = NULL;
    Twr->Scenes[i].NumOfUnits = 0;
    Twr->Scenes[i].MaxNumOfUnits = 0;
  }
  Twr->NumOfScenes = 0;
  Twr->MaxNumOfScenes = 0;
  free(Twr->Scenes);
  Twr->Scenes = NULL;
  
  Twr->Sys("Scenes system is deinitialized");
} /* End of 'TWR_ScenesSystemClose' function */

/* END OF 'twr_scenes.c' FILE */
