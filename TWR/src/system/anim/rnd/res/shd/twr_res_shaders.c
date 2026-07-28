/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_shaders.c
 * PURPOSE     : Tough Water Rendering project.
 *               Shaders control module.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>
#include <time.h>

/* Maximum include shader files */
#define TWR_MAX_INCLUDE_SHADER_FILES 102

/* Helpful shader existance struct */
typedef struct tagSHD_EXIST
{
  CHAR *Name; /* Name of shader */
  BOOL Exist; /* Existance flag */
} SHD_EXIST;

static SHD_EXIST ShdExist[] = {{"opaq", FALSE}, {"tran", FALSE}, {"shad", FALSE}, {"last", FALSE}, {"comp", FALSE}};

/* Directive '#line' support flag */
static BOOL TWR_ShdIsLineSupport = FALSE;

/* Shaders stock */
static STOCK TWR_StkShaders;

/* Shaders directories watcher struction value */
static twrDIR_WATCHER TWR_ShaderWatcher;

/* Load text from file function.
 * ARGUMENTS:
 * - file name to be load from:
 *     CHAR *FileName;
 * RETURNS:
 *     (CHAR *) load text (allocate throgh malloc).
 */
static CHAR * TWR_ShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);
  fclose(F);
  return txt;
} /* End of 'TWR_ShdLoadTextFromFile' function */

/* Load shader from file function.
 * ARGUMENTS:
 * - file name prefix to be load from:
 *     CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *     (INT) program shader identificator.
 */
INT TWR_ShdLoad( CHAR *ShaderFileNamePrefix )
{
  INT txt_size = 0, prg = 0, txt_common_size = 0, bufline_size = 0;
  CHAR *chr_pos, *txt[TWR_MAX_INCLUDE_SHADER_FILES];
  BOOL is_ok = TRUE;
  struct
  {
    CHAR *Name;      /* Shader file suffix name */
    INT Type;        /* Shader OpenGL type */
    CHAR *VerDefine; /* SHader version and type define */
    INT Id;          /* Shader Id created by OpenGL */
  } shd[] = 
  {
    {"vert", GL_VERTEX_SHADER, "#version 460\n#define VERTEX_SHADER 1\n"},
    {"frag", GL_FRAGMENT_SHADER, "#version 460\n#define FRAGMENT_SHADER 1\n"},
    {"geom", GL_GEOMETRY_SHADER, "#version 460\n#define GEOMETRY_SHADER 1\n"},
    {"ctrl", GL_TESS_CONTROL_SHADER, "#version 460\n#define TESS_CONTROL_SHADER 1\n"},
    {"eval", GL_TESS_EVALUATION_SHADER, "#version 460\n#define TESS_EVALUATION_SHADER 1\n"},
    {"comp", GL_COMPUTE_SHADER, "#version 460\n#define COMPUTE_SHADER 1\n"},
  };
  INT i, load_counter = 0, res, Ns = sizeof(shd) / sizeof(*shd);
  static CHAR Buf[30000], Buf1[300], BufLine[TWR_MAX_INCLUDE_SHADER_FILES][102];

  txt_size++;

  if ((txt[txt_size + 1] =
    TWR_ShdLoadTextFromFile("bin/shaders/includes/commondf.glsl")) != NULL)
  {
    txt[txt_size++] = !TWR_ShdIsLineSupport ? "\n" : "#line 1 \"commondf.glsl\"\n";
    txt_size++;
  }
  else
    Twr->Error("Error load commondf.glsl to %s", ShaderFileNamePrefix);

  chr_pos = ShaderFileNamePrefix;
  do
  {
    chr_pos = strchr(chr_pos, '/');
    strcpy(Buf1, ShaderFileNamePrefix);
    if (chr_pos != NULL)
      Buf1[chr_pos++ - ShaderFileNamePrefix] = 0;
    sprintf(Buf, "bin/shaders/%s/incl.glsl", Buf1);
    if ((txt[txt_size + 1] = TWR_ShdLoadTextFromFile(Buf)) != NULL)
    {
      sprintf(BufLine[bufline_size], "\n#line 1 \"%s/incl.glsl\"\n", Buf1);
      txt[txt_size++] = !TWR_ShdIsLineSupport ? "\n" : BufLine[bufline_size++];
      txt_size++;
    }
  } while (chr_pos != NULL);

  for (i = 0; is_ok && i < Ns; i++)
  {
    txt[0] = shd[i].VerDefine;
    /* txt[1] = "#extension GL_GOOGLE_cpp_style_line_directive : require\n"; */
    /* Load shader text from file */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", ShaderFileNamePrefix, shd[i].Name);

    txt[txt_size + 1] = TWR_ShdLoadTextFromFile(Buf);
    sprintf(Buf, "\n#line 1 \"%s/%s.glsl\"\n", ShaderFileNamePrefix, shd[i].Name);
    txt[txt_size] = !TWR_ShdIsLineSupport ? "\n" : Buf;
    if (txt[txt_size + 1] == NULL)
    {
      if (i >= 2 || ShaderFileNamePrefix + strlen(ShaderFileNamePrefix) - 4)
        continue;
      Twr->Error("%s: %s\n%s", ShaderFileNamePrefix, shd[i].Name, "Error file loading");
      is_ok = FALSE;
      break;
    }

    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      Twr->Error("%s: %s\n%s", ShaderFileNamePrefix, shd[i].Name, "Error create shader");
      free(txt[txt_size + 1]);
      is_ok = FALSE;
      break;
    }
    /* Attach shader source code to shader */
    glShaderSource(shd[i].Id, txt_size + 2, txt, NULL);

    /*
    Test debug text array output:
    for (j = 0; j < txt_size + 2; j++)
      Twr->Error("%d\n$%s$", j, txt[j]);
    */

    free(txt[txt_size + 1]);
    load_counter++;

    /* Compile shader */
    glCompileShader(shd[i].Id);
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      Twr->Error("%s: %s\n", ShaderFileNamePrefix, shd[i].Name);
      Twr->SetColor(0xFF8888, 0);
      Twr->Log("%s", Buf);
      is_ok = FALSE;
      break;
    }
  }
  while (txt_size-- > 1)
    free(txt[txt_size--]);

  if (load_counter == 0)
    is_ok = FALSE;
  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      is_ok = FALSE;
      Twr->Error("%s: %s\n%s", ShaderFileNamePrefix, "PROG", "Error create program");
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < Ns; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        Twr->Error("%s: %s\n%s", ShaderFileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
  }

  /* Error handle */
  if (!is_ok)
  {
    /* Remove all shaders */
    for (i = 0; i < Ns; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Remove program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = -1;
  }

  return prg;
} /* End of 'TWR_ShdLoad' function */

/* Delete strategy function.
 * ARGUMENTS:
 *   - shader pointer:
 *       twrSHADER *Shd;
 * RETURNS: None.
 */
static VOID TWR_ShdDeleteStrategy( twrSHADER *Shd )
{
  INT shdrs[5], n, i, k;

  if (Shd == NULL)
    return;
  for (k = 0; k < 5; k++)
  {
    if (Shd->ShdId[k] == -1 || !glIsProgram(Shd->ShdId[k]))
      continue;
    glGetAttachedShaders(Shd->ShdId[k], 5, &n, shdrs);
    for (i = 0; i < n; i++)
      if (glIsShader(shdrs[i]))
      {
        glDetachShader(Shd->ShdId[k], shdrs[i]);
        glDeleteShader(shdrs[i]);
      }
    glDeleteProgram(Shd->ShdId[k]);
    Shd->ShdId[k] = -1;
  }
} /* End of 'TWR_ShdDeleteStrategy' function */

/* Find shader in stock function.
 * ARGUMENTS:
 *   - shader name:
 *       CHAR *ShaderName;
 * RETURNS:
 *    (BOOL) TRUE if shader is found, FALSE otherwise.
 */
static BOOL TWR_ShdFindFunc( twrSHADER *Shd, CHAR *ShaderName )
{
  return strcmp(Shd->Name, ShaderName) == 0;
} /* End of 'TWR_ShdFindFunc' function */

/* Find shader in stock function.
 * ARGUMENTS:
 *   - shader name:
 *       CHAR *ShaderName;
 * RETURNS:
 *   (twrSHADER *) pointer to found shader.
 */
static twrSHADER * TWR_ShdFind( CHAR *ShaderName )
{
  return StockFind(&TWR_StkShaders, TWR_ShdFindFunc, ShaderName);
} /* End of 'TWR_ShdFind' function */

/* Check if shader exists in files function.
 * ARGUMENTS:
 *   - shader name:
 *       CHAR *ShaderName;
 * RETURNS: None.
 */
static VOID TWR_ShdExist( CHAR *ShaderName )
{
  INT i, Count = 0;
  CHAR Buf[TWR_STR_MAX];

  for (i = 0; i < 5; i++)
  {
    sprintf(Buf, "bin/shaders/%s/%s", ShaderName, ShdExist[i].Name);
    if (TWR_IsDirExists(Buf))
      ShdExist[i].Exist = TRUE, Count++;
    else
      ShdExist[i].Exist = FALSE;
  }
  if (Count == 0)
    Twr->SetColor(0xFF8888, 0), Twr->Log(" No shaders found.");
} /* End of 'TWR_ShdExist' function */

/* Load 5 shaders from files function.
 * ARGUMENTS:
 *   - shader name:
 *       CHAR *ShaderName;
 *   - shader pointer:
 *       twrSHADER *Shd;
 * RETURNS: None.
 */
static VOID TWR_ShadersLoad( CHAR *ShaderName, twrSHADER *Shd )
{
  INT i;
  CHAR Buf[TWR_STR_MAX];

  for (i = 0; i < 5; i++)
  {
    sprintf(Buf, "%s/%s", ShaderName, ShdExist[i].Name);
    if (ShdExist[i].Exist)
      Shd->ShdId[i] = TWR_ShdLoad(Buf), Shd->ShdId[i] < 1 ? (VOID)0: Twr->Info("     %s loaded. ", Buf);
    else
      Shd->ShdId[i] = -1;
  }
} /* End of 'TWR_ShadersLoad' function */

/* Add shader to stock function.
 * ARGUMENTS:
 *   - shader name:
 *       CHAR *ShaderName;
 * RETURNS:
 *   (twrSHADER *) pointer to created shader.
 */
static twrSHADER * TWR_ShdCreate( CHAR *ShaderName )
{
  INT i, Count = 0;
  twrSHADER *Shd = NULL;

  if ((Shd = TWR_ShdFind(ShaderName)) != NULL)
  {
    BOOL Id = FALSE;

    for (i = 0; i < 5; i++)
    {
      if (Shd->ShdId[i] != -1)
      {
        Id = TRUE;
        break;
      }
    }
    if (Id)
      return Shd;
  }
  if ((Shd = StockAlloc(&TWR_StkShaders)) == NULL)
    return NULL;

  Twr->Info("Loading shader: ");
  Twr->SetColor(0x88FFFF, 0);
  Twr->Log("%s", ShaderName);
  Twr->SetColor(0xFFFFFF, 0);

  TWR_ShdExist(ShaderName);
  TWR_ShadersLoad(ShaderName, Shd);

  strncpy(Shd->Name, ShaderName, TWR_STR_MAX - 1);

  return Shd;
} /* End of 'TWR_ShdCreate' function */

/* Delete shader function.
 * ARGUMENTS:
 *   - shader pointer:
 *       twrSHADER *Shd;
 * RETURNS: None.
 */
static VOID TWR_ShdFree( twrSHADER *Shd )
{
  StockDeleteInstant(Shd);
} /* End of 'TWR_ShdFree' function */

/* Apply shader function.
 * ARGUMENTS:
 *   - shader pointer:
 *       twrSHADER *Shd;
 *   - shader apply pass flag:
 *       twrTRANSPARENCY_FLAG PassFlag;
 * RETURNS:
 *   (BOOL) TRUE if shader apply successfuly, FALSE otherwise.
 */
static BOOL TWR_ShdApply( twrSHADER *Shd, twrTRANSPARENCY_FLAG PassFlag )
{
  if (Shd == NULL || Shd->ShdId[PassFlag] < 1)
    return FALSE;
  glUseProgram(Shd->ShdId[PassFlag]);
  return TRUE;
} /* End of 'TWR_ShdApply' function */

/* Update all shaders function.
 * ARGUMENTS:
 *   - shader pointer:
 *       twrSHADER *Shd;
 * RETURNS: None.
 */
static VOID TWR_ShadersUpdate( VOID )
{
  LONG t;
  twrSHADER *Shd;
  static LONG save_time;

  t = clock();

  if (TWR_DirWatcher_IsChanged(&TWR_ShaderWatcher) && t - save_time > CLOCKS_PER_SEC * 1)
  {
    Shd = StockFirst(&TWR_StkShaders);
    while (Shd != NULL)
    {
      TWR_ShdDeleteStrategy(Shd);
      Twr->Info("Reloading shader: ");
      Twr->SetColor(0x88FFFF, 0);
      Twr->Log("%s", Shd->Name);
      Twr->SetColor(0xFFFFFF, 0);
      TWR_ShdExist(Shd->Name);
      TWR_ShadersLoad(Shd->Name, Shd);
      Shd = StockNext(Shd);
    }
    save_time = t;
  }
} /* End of 'TWR_ShadersUpdate' function */

/* Shaders system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemShadersInit( VOID )
{
  INT res, tst_id;
  CHAR *TestShdTxt = "#version 450\n#line 1 \"aaa\"\nvoid main( void ){}";

  /* Check '#line' directive support */
  tst_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(tst_id, 1, &TestShdTxt, NULL);
  glCompileShader(tst_id);
  glGetShaderiv(tst_id, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    Twr->Warning("Shaders '#line' directive is not supported");
    TWR_ShdIsLineSupport = FALSE;
  }
  else
    TWR_ShdIsLineSupport = TRUE;
  glDeleteShader(tst_id);

  TWR_DirWatcher_StartWatch(&TWR_ShaderWatcher, "bin/shaders");

  StockCreate(&TWR_StkShaders, "Shaders", sizeof(twrSHADER), 100, TWR_ShdDeleteStrategy);
  /* Register stock of shaders to strip */
  TWR_StockRegisterToStrip(&TWR_StkShaders);

  Twr->ShdCreate = TWR_ShdCreate;
  Twr->ShdFree = TWR_ShdFree;
  Twr->ShdFind = TWR_ShdFind;
  Twr->ShdApply = TWR_ShdApply;
  Twr->ShadersUpdate = TWR_ShadersUpdate;
  Twr->Sys("Shaders system is initialized");
} /* End of 'TWR_SystemShadersInit' function */

/* Shaders system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemShadersClose( VOID )
{
  StockFree(&TWR_StkShaders);
  TWR_DirWatcher_StopWatch(&TWR_ShaderWatcher);
  Twr->Sys("Shaders system is deinitialized");
} /* End of 'TWR_SystemShadersClose' function */

/* END OF 'twr_res_shaders.c' FILE */
