#ifndef SIMPLECLIOHARDWAREINFO
#define SIMPLECLIOHARDWAREINFO

#include <iostream>
#include <string>
#include <fstream>
#ifndef __APPLE__
#include <CL/opencl.h>
#else
#include <OpenCL/opencl.h>
#endif

#ifdef SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif

#include "simpleCLtypes.h"

simpleCLstandardEnum readInfoFromConfigure(simpleCLhandler &mainCLHandler);
simpleCLstandardEnum writeInfoToConfigure(simpleCLhandler &mainCLHandler);

#endif  /*SIMPLECLIOHARDWAREINFO*/
