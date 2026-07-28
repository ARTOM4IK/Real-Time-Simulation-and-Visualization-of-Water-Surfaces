/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_profiler.h
 * PURPOSE     : Tough Water Rendering project.
 *               Profiler handle declaration module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_profiler_h_
#define __twr_profiler_h_

#include "twr_def.h"

/* Create profiler window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerInit( VOID );

/* Deinitialize profiler window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerClose( VOID );

/* Show/hide profiler window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerShowHide( VOID );

/* Start/stop profiler collection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerStartStop( VOID );

/* Profiling restart (should be called before frame snap start) function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerReset( VOID );

/* Collect profiling information (should be called before mesure event) function.
 * ARGUMENTS:
 *   - profile sample name:
 *       CHAR *ProfName;
 * RETURNS: None.
 */
VOID TWR_ProfilerCollect( CHAR *ProfName );

/* Add text to profiler function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - print parameters:
 *       . . .
 * RETURNS: None.
 */
VOID TWR_ProfilerAddText( CHAR *Fmt, ... );

#endif /* __twr_profiler_h_ */

/* END OF 'twr_profiler.h' FILE */
