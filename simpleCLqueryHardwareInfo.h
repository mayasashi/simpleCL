#ifndef SIMPLECLQUERYHARDWAREINFO
#define SIMPLECLQUERYHARDWAREINFO

#include "simpleCL.h"
#include <cmath>

#ifdef SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif


void queryPlatformAndDevice();
void queryPlatformAndDeviceInfo();
void printPlatformAndDeviceInfo();

void selectMainPlatformAndDevice();

extern cl_device_id **     device;
extern cl_platform_id *    platform;
extern cl_uint             num_platform, *num_device;

extern char **             platform_name;
extern char **             platform_vendor;
extern size_t *            platform_name_length;
extern size_t *            platform_vendor_length;

extern char ***            device_name;
extern char ***            device_vendor;
extern cl_uint **          device_uniqueID;
extern size_t **           device_name_length;
extern size_t **           device_vendor_length;

extern bool openclzeroplatform;
extern bool openclzerodevice;

extern bool queryPlatformAndDeviceCallFlg;


#endif  /*SIMPLECLQUERYHARDWAREINFO*/
