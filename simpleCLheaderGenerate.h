#ifndef SIMPLECLHEADERGENERATE
#define SIMPLECLHEADERGENERATE

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//#include <varargs.h>

#ifndef __APPLE__
#include <CL/opencl.h>
#else
#include <OpenCL/opencl.h>
#endif

#ifndef  __APPLE__
#pragma warning(disable:4996)
#endif

#include "simpleCL.h"
#include "findString.h"

struct kernel_t;

struct kernelContainer;

typedef kernelContainer *kernelContainerPtr;

struct kernelHandler {
	kernelContainerPtr ptr;

	kernelHandler();
	~kernelHandler();

	void addKernelProgramFile(const char *path, const char *name, cl_uint ID);
    void loadProgramFile();
    void buildProgram(simpleCLhandler handler);
	void printProgramBuildInfo(simpleCLhandler handler);
	cl_program & getProgram(cl_uint ID);
	void generateHeaderString();
};

#endif  /*SIMPLECLHEADERGENERATE*/
