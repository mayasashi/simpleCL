#include "simpleCLqueryHardwareInfo.h"

typedef struct innerChainContainer{
	void                (*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

typedef struct PDinfoContainer {
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
}_PDinfoContainer;

extern void registerFuncChain(void (*f)());

PDinfoHandler pd;

bool openclzeroplatform;
bool openclzerodevice;

bool queryPlatformAndDeviceCallFlg;

void cleanup() {
	printf("simpleCLqueryHardwareInfo's cleanup called.\n");
	for (int i = 0; i < (pd->num_platform); i++)
	{
		for (int j = 0; j < (pd->num_device)[i]; j++)
		{
			FREE_SAFE((pd->device_name)[i][j]);
			FREE_SAFE((pd->device_vendor)[i][j]);
		}
		FREE_SAFE((pd->device_name)[i]);
		FREE_SAFE((pd->device_vendor)[i]);
		FREE_SAFE((pd->device_uniqueID)[i]);
		FREE_SAFE((pd->device_name_length)[i]);
		FREE_SAFE((pd->device_vendor_length)[i]);
		FREE_SAFE((pd->device)[i]);

		FREE_SAFE((pd->platform_name)[i]);
		FREE_SAFE((pd->platform_vendor)[i]);
	}
	
	FREE_SAFE((pd->device_name));
	FREE_SAFE((pd->device_vendor));
	FREE_SAFE((pd->device_uniqueID));
	FREE_SAFE((pd->device_name_length));
	FREE_SAFE((pd->device_vendor_length));
	FREE_SAFE(pd->num_device);
	
	FREE_SAFE((pd->platform_name));
	FREE_SAFE((pd->platform_vendor));
	FREE_SAFE((pd->platform_name_length));
	FREE_SAFE((pd->platform_vendor_length));

	FREE_SAFE(pd->device);
	FREE_SAFE(pd->platform);

	FREE_SAFE(pd);
}

void queryPlatformAndDevice()
{
	
	if (queryPlatformAndDeviceCallFlg == 0) {
		pd = (PDinfoHandler)malloc(sizeof(_PDinfoContainer));

		registerFuncChain(cleanup);
		queryPlatformAndDeviceCallFlg = 1;

		clGetPlatformIDs(0, NULL, &(pd->num_platform));
		if (pd->num_platform > 0) {
			/*get platform*/
			openclzeroplatform = 0;
			(pd->platform) = (cl_platform_id *)malloc(sizeof(cl_platform_id)*(pd->num_platform));
			clGetPlatformIDs((pd->num_platform), (pd->platform), NULL);

			/*get device*/
			(pd->num_device) = (cl_uint *)malloc(sizeof(cl_uint)*(pd->num_platform));
			for (int i = 0; i < (pd->num_platform); i++) {
				clGetDeviceIDs((pd->platform)[i], CL_DEVICE_TYPE_ALL, 0, NULL, &(pd->num_device)[i]);
			}

			int devicegreaterthanzero = 0;
			for (int i = 0; i < (pd->num_platform); i++)
			{
				devicegreaterthanzero += ((pd->num_device)[i] > 0);
			}
			if (devicegreaterthanzero > 0) {
				(pd->device) = (cl_device_id **)malloc(sizeof(cl_device_id *)*(pd->num_platform));
				for (int i = 0; i < (pd->num_platform); i++)
				{
					if ((pd->num_device)[i] > 0) {
						(pd->device)[i] = (cl_device_id *)malloc(sizeof(cl_device_id)*(pd->num_device)[i]);
						clGetDeviceIDs((pd->platform)[i], CL_DEVICE_TYPE_ALL, (pd->num_device)[i], (pd->device)[i], NULL);
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
		(pd->platform_name) = (char **)malloc(sizeof(char *)*(pd->num_platform));
		(pd->platform_vendor) = (char **)malloc(sizeof(char *)*(pd->num_platform));
		(pd->platform_name_length) = (size_t *)malloc(sizeof(size_t)*(pd->num_platform));
		(pd->platform_vendor_length) = (size_t *)malloc(sizeof(size_t)*(pd->num_platform));
		for (int i = 0; i < (pd->num_platform); i++) {
			clGetPlatformInfo((pd->platform)[i], CL_PLATFORM_NAME, 0, NULL, &(pd->platform_name_length)[i]);
			clGetPlatformInfo((pd->platform)[i], CL_PLATFORM_VENDOR, 0, NULL, &(pd->platform_vendor_length)[i]);
			if ((pd->platform_name_length)[i] > 0 && (pd->platform_vendor_length)[i] > 0) {
				(pd->platform_name)[i] = (char *)malloc((pd->platform_name_length)[i]);
				(pd->platform_vendor)[i] = (char *)malloc((pd->platform_vendor_length)[i]);
				clGetPlatformInfo((pd->platform)[i], CL_PLATFORM_NAME, (pd->platform_name_length)[i], (pd->platform_name)[i], NULL);
				clGetPlatformInfo((pd->platform)[i], CL_PLATFORM_VENDOR, (pd->platform_vendor_length)[i], (pd->platform_vendor)[i], NULL);
			}
		}

		/*Device info*/
		(pd->device_name) = (char ***)malloc(sizeof(char **)*(pd->num_platform));
		(pd->device_vendor) = (char ***)malloc(sizeof(char **)*(pd->num_platform));
		(pd->device_uniqueID) = (cl_uint **)malloc(sizeof(cl_uint *)*(pd->num_platform));

		(pd->device_name_length) = (size_t **)malloc(sizeof(size_t *)*(pd->num_platform));
		(pd->device_vendor_length) = (size_t **)malloc(sizeof(size_t *)*(pd->num_platform));

		for (int i = 0; i < (pd->num_platform); i++)
		{
			(pd->device_name)[i] = (char **)malloc(sizeof(char *)*(pd->num_device)[i]);
			(pd->device_vendor)[i] = (char **)malloc(sizeof(char *)*(pd->num_device)[i]);
			(pd->device_uniqueID)[i] = (cl_uint *)malloc(sizeof(cl_uint)*(pd->num_device)[i]);
			
			(pd->device_name_length)[i] = (size_t *)malloc(sizeof(size_t)*(pd->num_device)[i]);
			(pd->device_vendor_length)[i] = (size_t *)malloc(sizeof(size_t)*(pd->num_device)[i]);

			for (int j = 0; j < (pd->num_device)[i]; j++) {
				clGetDeviceInfo((pd->device)[i][j], CL_DEVICE_NAME, 0, NULL, &(pd->device_name_length)[i][j]);
				clGetDeviceInfo((pd->device)[i][j], CL_DEVICE_VENDOR, 0, NULL, &(pd->device_vendor_length)[i][j]);
				if ((pd->device_name_length)[i][j] > 0 && (pd->device_vendor_length)[i][j] > 0) {
					(pd->device_name)[i][j] = (char *)malloc((pd->device_name_length)[i][j]);
					(pd->device_vendor)[i][j] = (char *)malloc((pd->device_vendor_length)[i][j]);
					clGetDeviceInfo((pd->device)[i][j], CL_DEVICE_NAME, (pd->device_name_length)[i][j], (pd->device_name)[i][j], NULL);
					clGetDeviceInfo((pd->device)[i][j], CL_DEVICE_VENDOR, (pd->device_vendor_length)[i][j], (pd->device_vendor)[i][j], NULL);
					clGetDeviceInfo((pd->device)[i][j], CL_DEVICE_VENDOR_ID, sizeof(cl_uint), &(pd->device_uniqueID)[i][j], NULL);
				}
			}
		}
	}
	else {
		printf("WARNING (%s) : This function requires the queryPlatformAndDeviceInfo function to be called previously.\n", __func__);
	}
}

void selectMainPlatformAndDevice(simpleCLhandler &mainCLHandler) {
	char buffer[33], *e;
	unsigned long p_index, d_index;
	bool confirmation_flg = 0;

	while (!confirmation_flg) {
		/*platform*/
		while (1) {
			printf("Choose platform you want to use. (%d~%u) : ", 0, (pd->num_platform) - 1);
			fgets(buffer, 32, stdin);
			p_index = strtoul(buffer, &e, 10);
			if (p_index >= 0 && p_index <= (pd->num_platform) - 1) break;
			else printf("Invalid input. Please try again.\n");
		}
		printf("You've chosen the following platform : [%lu] %s\n", p_index, (pd->platform_name)[p_index]);

		/*device*/
		while (1) {
			printf("Choose device you want to use in the platform.  (%d~%u) : ", 0, (pd->num_device)[p_index] - 1);
			fgets(buffer, 32, stdin);
			d_index = strtoul(buffer, &e, 10);
			if (d_index >= 0 && d_index <= (pd->num_device)[p_index] - 1) break;
			else printf("Invalid input. Please try again.\n");
		}
		
		printf("You've chosen the following device : [%lu] %s\n", d_index, (pd->device_name)[p_index][d_index]);

		/*confirmation*/
		while (1) {
			printf("Would you confirm? [y/n] : ");
			fgets(buffer, 32, stdin);
			if (strcmp(buffer, "y\n") == 0 || strcmp(buffer, "Y\n") == 0) {
				printf("Y\n");

				mainCLHandler->mainPlatform    = (pd->platform)[p_index];
				mainCLHandler->mainDevice      = (pd->device)[p_index][d_index];

				mainCLHandler->platform_index  = p_index;
				mainCLHandler->platform_name  = (pd->platform_name)[p_index];
				mainCLHandler->platform_vendor = (pd->platform_vendor)[p_index];

				mainCLHandler->device_index    = d_index;
				mainCLHandler->device_name     = (pd->device_name)[p_index][d_index];
				mainCLHandler->device_vendor   = (pd->device_vendor)[p_index][d_index];
				mainCLHandler->device_uniqueID = (pd->device_uniqueID)[p_index][d_index];

				confirmation_flg = 1;
				break;
			}
			else if (strcmp(buffer, "n\n") == 0 || strcmp(buffer, "N\n") == 0) {
				printf("N\n");
				break;
			}
			else {
				printf("Invalid input. Please try again.\n");
			}
		}
		
	}
}
void setMainPlatformAndDevice(simpleCLhandler &mainCLHandler){
    mainCLHandler->mainPlatform = (pd->platform)[mainCLHandler->platform_index];
    mainCLHandler->mainDevice   = (pd->device)[mainCLHandler->platform_index][mainCLHandler->device_index];
    printf("Using Platform : %s\n",mainCLHandler->platform_name);
    printf("Using Device   : %s  [ID : %u]\n",mainCLHandler->device_name,mainCLHandler->device_uniqueID);
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

		for (int i = 0; i < (pd->num_platform); i++)
		{
			if (max_platform_name_length < (pd->platform_name_length)[i]) max_platform_name_length = (pd->platform_name_length)[i];
			if (max_platform_vendor_length < (pd->platform_vendor_length)[i]) max_platform_vendor_length = (pd->platform_vendor_length)[i];

			for (int j = 0; j < (pd->num_device)[i]; j++) {
				if (max_device_name_length < (pd->device_name_length)[i][j]) max_device_name_length = (pd->device_name_length)[i][j];
				if (max_device_vendor_length < (pd->device_vendor_length)[i][j]) max_device_vendor_length = (pd->device_vendor_length)[i][j];
				if (max_device_vendor_uniqueID_digit_length < (int)log10((pd->device_uniqueID)[i][j]) + 1) max_device_vendor_uniqueID_digit_length = (int)log10((pd->device_uniqueID)[i][j]) + 1;
			}

			if (max_device_index_digit_length < (int)log10((pd->num_device)[i]) + 1)max_device_index_digit_length = (int)log10((pd->num_device)[i]) + 1;
		}

		size_t Max_pNs = fmax(strlen(str_platformName), max_platform_name_length);
		size_t Max_pVs = fmax(strlen(str_platformVendor), max_platform_vendor_length);
		size_t Max_dNs = fmax(strlen(str_deviceName), max_device_name_length);
		size_t Max_dVs = fmax(strlen(str_deviceVendor), max_device_vendor_length);
		size_t Max_dUs = fmax(strlen(str_deviceUniqueID), max_device_vendor_uniqueID_digit_length);

		int li, lj;


		/*Platform*/
		printf("<PLATFORM_INFO>\n");
        printf("\n");
		for (int m = 0; m < 2 + (int)log10(pd->num_platform) + 1; m++)
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

		for (int m = 0; m < 2 + (int)log10(pd->num_platform) + 1 + Max_pNs + Max_pVs + 2; m++)
		{
			printf("-");
		}

		printf("\n");
		for (int i = 0; i < (pd->num_platform); i++)
		{
			printf("[%d]", i);
			li = (i == 0) ? 0 : (int)log10(i);
			for (int k = 0; k < (int)log10(pd->num_platform) - li; k++)
			{
				printf(" ");
			}
			printf("|");
			if ((pd->platform_name)[i] != NULL) {
				printf("%s", (pd->platform_name)[i]);
				for (int k = 0; k < Max_pNs - strlen((pd->platform_name)[i]); k++)
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
			if ((pd->platform_vendor)[i] != NULL) {
				printf("%s", (pd->platform_vendor)[i]);
				for (int k = 0; k < Max_pVs - strlen((pd->platform_vendor)[i]); k++)
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
        printf("\n");
		/*device*/
		printf("<DEVICE_INFO>\n");
        printf("\n");
		for (int m = 0; m < 4 + (int)log10(pd->num_platform) + 1 + max_device_index_digit_length; m++) {
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
		for (int m = 0; m < 4 + (int)log10(pd->num_platform) + 1 + max_device_index_digit_length + Max_dNs + Max_dVs + Max_dUs + 3; m++)
		{
			printf("-");
		}
		printf("\n");
		for (int i = 0; i < (pd->num_platform); i++)
		{
			for (int j = 0; j < (pd->num_device)[i]; j++)
			{
				printf("[%d][%d]", i, j);
				li = (i == 0) ? 0 : (int)log10(i);
				lj = (j == 0) ? 0 : (int)log10(j);
				for (int k = 0; k < (int)log10(pd->num_platform) + max_device_index_digit_length - li - lj - 1; k++)
				{
					printf(" ");
				}
				printf("|");
				if ((pd->device_name)[i][j] != NULL)
				{
					printf("%s", (pd->device_name)[i][j]);
					for (int k = 0; k < Max_dNs - strlen((pd->device_name)[i][j]); k++)
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
				if ((pd->device_vendor)[i][j] != NULL)
				{
					printf("%s", (pd->device_vendor)[i][j]);
					for (int k = 0; k < Max_dVs - strlen((pd->device_vendor)[i][j]); k++)
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
				if ((pd->device_uniqueID)[i][j] != 0)
				{
					printf("%u", (pd->device_uniqueID)[i][j]);
					for (int k = 0; k < Max_dUs - (int)log10((pd->device_uniqueID)[i][j]) - 1; k++)
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

			for (int m = 0; m < 4 + (int)log10(pd->num_platform) + 1 + max_device_index_digit_length; m++)
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
        printf("\n");
	}
	else {
		printf("WARNING (%s) : This function requires the queryPlatformAndDeviceInfo function to be called previously.\n", __func__);
	}
}
