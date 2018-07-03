#include "simpleCLqueryHardwareInfo.h"

typedef struct innerChainContainer{
	void                (*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

extern void registerFuncChain(void (*f)());

cl_device_id **     device;
cl_platform_id *    platform;
cl_uint             num_platform, *num_device;

char **             platform_name;
char **             platform_vendor;
size_t *            platform_name_length;
size_t *            platform_vendor_length;

char ***            device_name;
char ***            device_vendor;
cl_uint **          device_uniqueID;
size_t **           device_name_length;
size_t **           device_vendor_length;

bool openclzeroplatform;
bool openclzerodevice;

bool queryPlatformAndDeviceCallFlg;

void cleanup() {
	printf("simpleCLqueryHardwareInfo's cleanup called.\n");
	for (int i = 0; i < num_platform; i++)
	{
		for (int j = 0; j < num_device[i]; j++)
		{
			FREE_SAFE(device_name[i][j]);
			FREE_SAFE(device_vendor[i][j]);
		}
		FREE_SAFE(device_name[i]);
		FREE_SAFE(device_vendor[i]);
		FREE_SAFE(device_uniqueID[i]);
		FREE_SAFE(device_name_length[i]);
		FREE_SAFE(device_vendor_length[i]);
		FREE_SAFE(device[i]);

		FREE_SAFE(platform_name[i]);
		FREE_SAFE(platform_vendor[i]);
	}
	
	FREE_SAFE(device_name);
	FREE_SAFE(device_vendor);
	FREE_SAFE(device_uniqueID);
	FREE_SAFE(device_name_length);
	FREE_SAFE(device_vendor_length);
	FREE_SAFE(num_device);
	
	FREE_SAFE(platform_name);
	FREE_SAFE(platform_vendor);
	FREE_SAFE(platform_name_length);
	FREE_SAFE(platform_vendor_length);

	FREE_SAFE(device);
	FREE_SAFE(platform);
}

void queryPlatformAndDevice()
{
	
	if (queryPlatformAndDeviceCallFlg == 0) {

		registerFuncChain(cleanup);
		queryPlatformAndDeviceCallFlg = 1;

		clGetPlatformIDs(0, NULL, &num_platform);
		if (num_platform > 0) {
			/*get platform*/
			openclzeroplatform = 0;
			platform = (cl_platform_id *)malloc(sizeof(cl_platform_id)*num_platform);
			clGetPlatformIDs(num_platform, platform, NULL);

			/*get device*/
			num_device = (cl_uint *)malloc(sizeof(cl_uint)*num_platform);
			for (int i = 0; i < num_platform; i++) {
				clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_device[i]);
			}

			int devicegreaterthanzero = 0;
			for (int i = 0; i < num_platform; i++)
			{
				devicegreaterthanzero += (num_device[i] > 0);
			}
			if (devicegreaterthanzero > 0) {
				device = (cl_device_id **)malloc(sizeof(cl_device_id *)*num_platform);
				for (int i = 0; i < num_platform; i++)
				{
					if (num_device[i] > 0) {
						device[i] = (cl_device_id *)malloc(sizeof(cl_device_id)*num_device[i]);
						clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, num_device[i], device[i], NULL);
					}
				}
			}
			else {
				openclzerodevice = 1;
				printf("WARNING (%s) : OpenCL device not found.\n", __func__);
			}
		}
		else {
			openclzeroplatform = 1;
			printf("WARNING (%s) : OpenCL platform not found.\n", __func__);
		}
	}
	else {
		printf("NOTE (%s) : The function is already called.\n", __func__);
	}
}

void queryPlatformAndDeviceInfo() {
	if (queryPlatformAndDeviceCallFlg) {
		/*Platform info*/
		platform_name = (char **)malloc(sizeof(char *)*num_platform);
		platform_vendor = (char **)malloc(sizeof(char *)*num_platform);
		platform_name_length = (size_t *)malloc(sizeof(size_t)*num_platform);
		platform_vendor_length = (size_t *)malloc(sizeof(size_t)*num_platform);
		for (int i = 0; i < num_platform; i++) {
			clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, 0, NULL, &platform_name_length[i]);
			clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, 0, NULL, &platform_vendor_length[i]);
			if (platform_name_length[i] > 0 && platform_vendor_length[i] > 0) {
				platform_name[i] = (char *)malloc(platform_name_length[i]);
				platform_vendor[i] = (char *)malloc(platform_vendor_length[i]);
				clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, platform_name_length[i], platform_name[i], NULL);
				clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, platform_vendor_length[i], platform_vendor[i], NULL);
			}
		}

		/*Device info*/
		device_name = (char ***)malloc(sizeof(char **)*num_platform);
		device_vendor = (char ***)malloc(sizeof(char **)*num_platform);
		device_uniqueID = (cl_uint **)malloc(sizeof(cl_uint *)*num_platform);

		device_name_length = (size_t **)malloc(sizeof(size_t *)*num_platform);
		device_vendor_length = (size_t **)malloc(sizeof(size_t *)*num_platform);

		for (int i = 0; i < num_platform; i++)
		{
			device_name[i] = (char **)malloc(sizeof(char *)*num_device[i]);
			device_vendor[i] = (char **)malloc(sizeof(char *)*num_device[i]);
			device_uniqueID[i] = (cl_uint *)malloc(sizeof(cl_uint)*num_device[i]);
			
			device_name_length[i] = (size_t *)malloc(sizeof(size_t)*num_device[i]);
			device_vendor_length[i] = (size_t *)malloc(sizeof(size_t)*num_device[i]);

			for (int j = 0; j < num_device[i]; j++) {
				clGetDeviceInfo(device[i][j], CL_DEVICE_NAME, 0, NULL, &device_name_length[i][j]);
				clGetDeviceInfo(device[i][j], CL_DEVICE_VENDOR, 0, NULL, &device_vendor_length[i][j]);
				if (device_name_length[i][j] > 0 && device_vendor_length[i][j] > 0) {
					device_name[i][j] = (char *)malloc(device_name_length[i][j]);
					device_vendor[i][j] = (char *)malloc(device_vendor_length[i][j]);
					clGetDeviceInfo(device[i][j], CL_DEVICE_NAME, device_name_length[i][j], device_name[i][j], NULL);
					clGetDeviceInfo(device[i][j], CL_DEVICE_VENDOR, device_vendor_length[i][j], device_vendor[i][j], NULL);
					clGetDeviceInfo(device[i][j], CL_DEVICE_VENDOR_ID, sizeof(cl_uint), &device_uniqueID[i][j], NULL);
				}
			}
		}
	}
	else {
		printf("WARNING (%s) : This function requires the queryPlatformAndDeviceInfo function to be called previously.\n", __func__);
	}
}

void printPlatformAndDeviceInfo() {
	if (queryPlatformAndDeviceCallFlg) {

		size_t max_platform_name_length = 0;
		size_t max_platform_vendor_length = 0;

		size_t max_device_name_length = 0;
		size_t max_device_vendor_length = 0;
		int max_device_vendor_uniqueID_digit_length = 0;
		int max_device_index_digit_length = 0;

		const char *str_platformName = "NAME";
		const char *str_platformVendor = "VENDOR";

		const char *str_deviceName = "NAME";
		const char *str_deviceVendor = "VENDOR";
		const char *str_deviceUniqueID = "UNIQUE ID";

		for (int i = 0; i < num_platform; i++)
		{
			if (max_platform_name_length < platform_name_length[i]) max_platform_name_length = platform_name_length[i];
			if (max_platform_vendor_length < platform_vendor_length[i]) max_platform_vendor_length = platform_vendor_length[i];

			for (int j = 0; j < num_device[i]; j++) {
				if (max_device_name_length < device_name_length[i][j]) max_device_name_length = device_name_length[i][j];
				if (max_device_vendor_length < device_vendor_length[i][j]) max_device_vendor_length = device_vendor_length[i][j];
				if (max_device_vendor_uniqueID_digit_length < (int)log10(device_uniqueID[i][j]) + 1) max_device_vendor_uniqueID_digit_length = (int)log10(device_uniqueID[i][j]) + 1;
			}

			if (max_device_index_digit_length < (int)log10(num_device[i]) + 1)max_device_index_digit_length = (int)log10(num_device[i]) + 1;
		}

		size_t Max_pNs = fmax(strlen(str_platformName), max_platform_name_length);
		size_t Max_pVs = fmax(strlen(str_platformVendor), max_platform_vendor_length);
		size_t Max_dNs = fmax(strlen(str_deviceName), max_device_name_length);
		size_t Max_dVs = fmax(strlen(str_deviceVendor), max_device_vendor_length);
		size_t Max_dUs = fmax(strlen(str_deviceUniqueID), max_device_vendor_uniqueID_digit_length);

		int li, lj;


		/*Platform*/
		printf("<PLATFORM_INFO>\n");
		for (int m = 0; m < 2 + (int)log10(num_platform) + 1; m++)
		{
			printf(" ");
		}
		printf("|");
		printf("%s", str_platformName);
		for (int m = 0; m < max_platform_name_length - strlen(str_platformName); m++)
		{
			printf(" ");
		}
		printf("|");
		printf("%s", str_platformVendor);
		for (int m = 0; m < max_platform_vendor_length - strlen(str_platformVendor); m++)
		{
			printf(" ");
		}
		printf("\n");

		for (int m = 0; m < 2 + (int)log10(num_platform) + 1 + Max_pNs + Max_pVs + 2; m++)
		{
			printf("-");
		}

		printf("\n");
		for (int i = 0; i < num_platform; i++)
		{
			printf("[%lu]", i);
			li = (i == 0) ? 0 : (int)log10(i);
			for (int k = 0; k < (int)log10(num_platform) - li; k++)
			{
				printf(" ");
			}
			printf("|");
			if (platform_name[i] != NULL) {
				printf("%s", platform_name[i]);
				for (int k = 0; k < Max_pNs - strlen(platform_name[i]); k++)
				{
					printf(" ");
				}
			}
			else {
				for (int k = 0; k < Max_pNs; k++)
				{
					printf(" ");
				}
			}
			printf("|");
			if (platform_vendor[i] != NULL) {
				printf("%s", platform_vendor[i]);
				for (int k = 0; k < Max_pVs - strlen(platform_vendor[i]); k++)
				{
					printf(" ");
				}
			}
			else {
				for (int k = 0; k < Max_pVs; k++)
				{
					printf(" ");
				}
			}

			printf("\n");
			/*for (int k = 0; k < 2 + (int)log10(num_platform) + 1 + Max_pNs + Max_pVs + 2; k++)
			{
				printf("-");
			}
			printf("\n");*/
		}
		/*device*/
		printf("<DEVICE_INFO>\n");
		for (int m = 0; m < 4 + (int)log10(num_platform) + 1 + max_device_index_digit_length; m++) {
			printf(" ");
		}
		printf("|");
		printf("%s", str_deviceName);
		for (int m = 0; m < Max_dNs - strlen(str_deviceName); m++)
		{
			printf(" ");
		}
		printf("|");
		printf("%s", str_deviceVendor);
		for (int m = 0; m < Max_dVs - strlen(str_deviceVendor); m++)
		{
			printf(" ");
		}
		printf("|");
		printf("%s", str_deviceUniqueID);
		for (int m = 0; m < Max_dUs - strlen(str_deviceUniqueID); m++)
		{
			printf(" ");
		}
		printf("\n");
		for (int m = 0; m < 4 + (int)log10(num_platform) + 1 + max_device_index_digit_length + Max_dNs + Max_dVs + Max_dUs + 3; m++)
		{
			printf("-");
		}
		printf("\n");
		for (int i = 0; i < num_platform; i++)
		{
			for (int j = 0; j < num_device[i]; j++)
			{
				printf("[%lu][%lu]", i, j);
				li = (i == 0) ? 0 : (int)log10(i);
				lj = (j == 0) ? 0 : (int)log10(j);
				for (int k = 0; k < (int)log10(num_platform) + max_device_index_digit_length - li - lj - 1; k++)
				{
					printf(" ");
				}
				printf("|");
				if (device_name[i][j] != NULL)
				{
					printf("%s", device_name[i][j]);
					for (int k = 0; k < Max_dNs - strlen(device_name[i][j]); k++)
					{
						printf(" ");
					}
				}
				else {
					for (int k = 0; k < Max_dNs; k++)
					{
						printf(" ");
					}
				}
				printf("|");
				if (device_vendor[i][j] != NULL)
				{
					printf("%s", device_vendor[i][j]);
					for (int k = 0; k < Max_dVs - strlen(device_vendor[i][j]); k++)
					{
						printf(" ");
					}
				}
				else {
					for (int k = 0; k < Max_dVs; k++)
					{
						printf(" ");
					}
				}
				printf("|");
				if (device_uniqueID[i][j] != 0)
				{
					printf("%lu", device_uniqueID[i][j]);
					for (int k = 0; k < Max_dUs - (int)log10(device_uniqueID[i][j]) - 1; k++)
					{
						printf(" ");
					}
				}
				else {
					for (int k = 0; k < Max_dUs; k++)
					{
						printf(" ");
					}
				}
				printf("\n");
			}

			for (int m = 0; m < 4 + (int)log10(num_platform) + 1 + max_device_index_digit_length; m++)
			{
				printf("-");
			}
			printf("|");
			for (int m = 0; m < Max_dNs; m++)
			{
				printf("-");
			}
			printf("|");
			for (int m = 0; m < Max_dVs; m++)
			{
				printf("-");
			}
			printf("|");
			for (int m = 0; m < Max_dUs; m++)
			{
				printf("-");
			}
			printf("\n");
		}
	}
	else {
		printf("WARNING (%s) : This function requires the queryPlatformAndDeviceInfo function to be called previously.\n", __func__);
	}
}
