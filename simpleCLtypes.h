#ifndef SIMPLECLTYPE
#define SIMPLECLTYPE

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

enum simpleCLstandardEnum {
	SIMPLECL_LOAD_FAIL,
	SIMPLECL_LOAD_SUCCESS,
	SIMPLECL_WRITE_FAIL,
	SIMPLECL_WRITE_SUCCESS
};

typedef struct simpleCLcontainer {
	cl_context        mainContext;
	cl_platform_id    mainPlatform;
	cl_device_id      mainDevice;
	cl_command_queue  mainQueue;


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

typedef struct PDinfoContainer *PDinfoHandler;

typedef struct innerChainContainer *innerChainHandler;

typedef struct simpleCLcontainer *simpleCLhandler;



#endif  /*SIMPLECLTYPE*/
