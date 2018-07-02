#ifndef SIMPLECL
#define SIMPLECL
#include <iostream>
#include <CL/opencl.h>

#define SIMPLECL_NOTIFY_WIN

#ifdef SIMPLECL_NOTIFY_WIN
#define SIMPLECL_WIN
#elif SIMPLECL_NOTIFY_MAC
#define SIMPLECL_MAC
#else
#define SIMPLECL_PLATFORM_UNKNOWN
#endif

#ifdef SIMPLECL_PLATFORM_UNKNOWN
#error simpleclmessage : Failed to identify which platform you use. If you're working on windows or mac platform, it's necessary to define the macro SIMPLECL_NOTIFY_WIN or SIMPLECL_NOTIFY_MAC respectively.'
#endif

#define FREE_SAFE(arg) if(arg != NULL)free(arg);

#ifdef SIMPLECL_WIN
#pragma warning(disable:4996)  /*fopen*/
#endif

class simpleCL {
public:
	simpleCL();
	~simpleCL();
	void cleanup();
private:

	void queryPlatformAndDevice();
	void queryPlatformAndDeviceInfo();
	void printPlatformAndDeviceInfo();

	cl_context *mainContext;
	cl_device_id *mainDevice;
	cl_platform_id *mainPlatform;

	cl_device_id **device;
	cl_platform_id *platform;
	cl_uint num_platform, *num_device;

	char **platform_name;
	char **platform_vendor;
	size_t *platform_name_length;
	size_t *platform_vendor_length;

	char ***device_name;
	char ***device_vendor;
	cl_uint **device_uniqueID;
	size_t **device_name_length;
	size_t **device_vendor_length;


	bool openclzeroplatform;
	bool openclzerodevice;

	bool queryPlatformAndDeviceCallFlg;
};


#endif  /*SIMPLECL*/
