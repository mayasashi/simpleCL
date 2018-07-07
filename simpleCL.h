#ifndef SIMPLECL
#define SIMPLECL

#include <iostream>

#ifndef __APPLE__
#include <CL/opencl.h>
#else
#include <OpenCL/opencl.h>
#endif

#ifdef  SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif

#include "simpleCLqueryHardwareInfo.h"
#include "simpleCLIOHardwareInfo.h"
#include "simpleCLtypes.h"

simpleCLhandler & simpleCL_init ();
void              simpleCL_close (simpleCLhandler &handler);

extern simpleCLhandler mainCLHandler;
extern innerChainHandler iCH;

#endif  /*SIMPLECL*/
