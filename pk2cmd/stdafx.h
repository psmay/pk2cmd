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

#include <string>
#include <vector>
using namespace std;



static const int	MAX_PATH =	256;

typedef char _TCHAR;
typedef vector<_TCHAR*> TextVec;
typedef int errno_t;
#define	TXT_LENGTH		(int)strlen
#define	TCH_UPPER	 toupper
//#define	_tcsncpy_s	strncpy
#define	TXT_PUSH_UNSAFE	strcat
#define	TXT_PUSH	strncat
#define	TXT_SEEK_TCHAR		strchr
#define	TXT_TO_DOUBLE		atof
#define	_tzset		tzset
#define	TXT_WRITE_TO		fputs
#define	TXT_PUSH_FROM		fgets
#define	TXT_COMPARE		strncmp

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

#define XRIGHT(str,skip) &str[skip]

// TODO: reference additional headers your program requires here
