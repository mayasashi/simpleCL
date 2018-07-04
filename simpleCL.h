#ifndef SIMPLECL
#define SIMPLECL

#include <iostream>

#ifndef __APPLE__
#include <CL/opencl.h>
#else
#include <OpenCL/opencl.h>
#endif

#ifndef FREESAFE_DEFINED
#define FREE_SAFE(arg)       if(arg != NULL)free(arg);
#define FREESAFE_DEFINED
#endif

#ifdef  SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif

#include "simpleCLqueryHardwareInfo.h"
#include "simpleCLIOHardwareInfo.h"
#include "simpleCLenum.h"

typedef struct simpleCLcontainer {
	cl_context     *   mainContext;
	cl_platform_id *   mainPlatform;
	cl_device_id   *   mainDevice;

	/*platform_information*/
	cl_uint            platform_index;
	char           *   platform_name;
	char           *   platform_vendor;

	/*device_information*/
	cl_uint            device_index;
	char           *   device_name;
	char           *   device_vendor;
	cl_uint            device_uniqueID;

}_simpleCLcontainer;

typedef struct innerChainContainer *innerChainHandler;

typedef struct simpleCLcontainer *simpleCLhandler;

void            simpleCL_init ();
void            simpleCL_close ();

extern simpleCLhandler mainCLHandler;
extern innerChainHandler iCH;




#endif  /*SIMPLECL*/
