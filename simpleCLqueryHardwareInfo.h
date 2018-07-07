#ifndef SIMPLECLQUERYHARDWAREINFO
#define SIMPLECLQUERYHARDWAREINFO

#include <iostream>

#ifndef __APPLE__
#include <CL/opencl.h>
#else
#include <OpenCL/opencl.h>
#endif
#include <cmath>

#include "simpleCLtypes.h"

#ifdef SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif


void queryPlatformAndDevice();
void queryPlatformAndDeviceInfo();
void printPlatformAndDeviceInfo();

void selectMainPlatformAndDevice(simpleCLhandler &mainCLHandler);

void setMainPlatformAndDevice(simpleCLhandler &mainCLHandler);

extern PDinfoHandler pd;

extern bool openclzeroplatform;
extern bool openclzerodevice;

extern bool queryPlatformAndDeviceCallFlg;


#endif  /*SIMPLECLQUERYHARDWAREINFO*/
