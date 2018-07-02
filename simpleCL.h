#ifndef SIMPLECL
#define SIMPLECL
#include <iostream>
#include <CL/opencl.h>

#define SIMPLECL_NOTIFY_WIN

#ifdef  SIMPLECL_NOTIFY_WIN
#define SIMPLECL_WIN
#elif   SIMPLECL_NOTIFY_MAC
#define SIMPLECL_MAC
#else
#define SIMPLECL_PLATFORM_UNKNOWN
#endif

#ifdef  SIMPLECL_PLATFORM_UNKNOWN
#error  simpleclmessage : Failed to identify which platform you use. If you're working on windows or mac platform, it's necessary to define the macro SIMPLECL_NOTIFY_WIN or SIMPLECL_NOTIFY_MAC respectively.'
#endif

#ifndef FREESAFE_DEFINED
#define FREE_SAFE(arg)       if(arg != NULL)free(arg);
#define FREESAFE_DEFINED
#endif

#ifdef  SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif

#include "simpleCLqueryHardwareInfo.h"

typedef struct simpleCLcontainer{
	cl_context     *   mainContext;
	cl_platform_id *   mainPlatform;
	cl_device_id   *   mainDevice;
}_simpleCLcontainer;

typedef struct innerChainContainer *innerChainHandler;

typedef struct simpleCLcontainer *simpleCLhandler;

void            simpleCL_init ();
void            simpleCL_close ();

extern simpleCLhandler mainCLHandler;
extern innerChainHandler iCH;




#endif  /*SIMPLECL*/
