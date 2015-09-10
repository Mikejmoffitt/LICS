/*
  File: fm.h -- header file for software emulation for FM sound generator

*/

#pragma once

#ifndef __FM_H__
#define __FM_H__

/* select bit size of output : 8 or 16 */
#define FM_SAMPLE_BITS 16

/* compiler dependence */
typedef unsigned char	UINT8;   /* unsigned  8bit */
typedef unsigned short	UINT16;  /* unsigned 16bit */
typedef unsigned int	UINT32;  /* unsigned 32bit */
typedef signed char		INT8;    /* signed  8bit   */
typedef signed short	INT16;   /* signed 16bit   */
typedef signed int		INT32;   /* signed 32bit   */

typedef short int FMSAMPLE;

#endif /* __FM_H__ */
