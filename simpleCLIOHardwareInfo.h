#ifndef SIMPLECLIOHARDWAREINFO
#define SIMPLECLIOHARDWAREINFO

#include "simpleCL.h"
#include <string>
#include <fstream>

#ifdef SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif

#include "simpleCLenum.h"

simpleCLstandardEnum readInfoFromConfigure();
simpleCLstandardEnum writeInfoToConfigure();

#endif  /*SIMPLECLIOHARDWAREINFO*/
