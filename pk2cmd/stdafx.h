// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

static const int	VERSION_MAJOR =	1;
static const int	VERSION_MINOR =	21;
static const int	VERSION_DOT =	0;


#include	<stdio.h>
#include	<stdlib.h>
#include <unistd.h>
#include	<ctype.h>
#include	<stdarg.h>
#include <string.h>
#include <errno.h>
#include	<time.h>

static const int	MAX_PATH =	256;

typedef char _TCHAR;
typedef int errno_t;
#define	_tcslen		strlen
#define	_totupper	 toupper
//#define	_tcsncpy_s	strncpy
#define	_tcscat_s	strcat
#define	_tcsncat_s	strncat
#define	_tcschr		strchr
#define	_tstof		atof
#define	_tzset		tzset
#define	_fputts		fputs
#define	_fgetts		fgets
#define	_tcsncmp		strncmp

#define	Sleep(x)		usleep(x*1000)
#define	_tfopen_s	fopen_s

extern char * _tcsncpy_s(char *dst, const char *src, int len);
extern void _tsearchenv_s(const char *fname, const char *path, char *bfr);
extern void	_localtime64_s(struct tm *x, time_t *y);
extern void	_tcsftime(char *bfr, int len, const char *fmt, struct tm *time);
extern void	_stprintf_s(char *bfr, int size, const char *fmt, ...);
extern int	fopen_s(FILE **fp, char *path, const char *spec);


// Not all compilers define 'bool' to have the same size, so we force it here.
//#ifdef bool
//#undef bool
//#endif
//#define	bool	unsigned char

// TODO: reference additional headers your program requires here
