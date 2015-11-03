#ifndef TOS_H_INCLUDED
#define TOS_H_INCLUDED

#if !defined(__CYGWIN__)
#if defined(__MSP430__)
#include <sys/inttypes.h>
#else
#include <inttypes.h>
#endif
#else //cygwin
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <ctype.h>

#include "../lib/safe/include/annots_stage1.h"

#ifndef __cplusplus
typedef uint8_t bool;
#endif


enum { FALSE = 0, TRUE = 1 };

typedef nx_int8_t nx_bool;
extern uint16_t TOS_NODE_ID;

/* This macro is used to mark pointers that represent ownership
   transfer in interfaces. See TEP 3 for more discussion. */
#define PASS

/* This platform_bootstrap macro exists in accordance with TEP
   107. A platform may override this through a platform.h file. */
#include <platform.h>
#ifndef platform_bootstrap
#define platform_bootstrap() {}
#endif

#define dbg(s, ...) debug(unique("TOSSIM.debug"), s, __VA_ARGS__)
#define dbgerror(s, ...) 
#define dbg_clear(s, ...) 
#define dbgerror_clear(s, ...) 

struct @atmostonce { };
struct @atleastonce { };
struct @exactlyonce { };

/* DEVS-SIM specific files.
   This header declares all the functions used to communicate with the Devs components */
#include <../../interface/Interface.h>

#endif
