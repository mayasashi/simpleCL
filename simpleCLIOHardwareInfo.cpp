#include "simpleCLIOHardwareInfo.h"

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

extern PDinfoHandler pd;


std::ifstream file_input;
std::ofstream file_output;

std::string str_platform_name     = "";
std::string str_platform_vendor   = "";
std::string str_device_name       = "";
std::string str_device_vendor     = "";
std::string str_device_uniqueID   = "";

cl_uint specified_device_uniqueID;

const char *str_platform_name_leftside   = "MAIN_PLATFORM_NAME=";
const char *str_platform_vendor_leftside = "MAIN_PLATFORM_VENDOR=";
const char *str_device_name_leftside     = "MAIN_DEVICE_NAME=";
const char *str_device_vendor_leftside   = "MAIN_DEVICE_VENDOR=";
const char *str_device_uniqueID_leftside = "MAIN_DEVICE_UNIQUEID=";

bool configureFileReadWriteCallFlg = 0;

simpleCLstandardEnum readInfoFromConfigure(simpleCLhandler &mainCLHandler){

	std::string str;
	bool configureCorrespondenceFlg = 0;
	cl_uint p_index, d_index;
	file_input.open("settings.configure");
	if (file_input.fail()) {
		printf("NOTE(%s) : settings.configure not found. The file will be created automatically during initial setup.\n", __func__);
		return SIMPLECL_LOAD_FAIL;
	}
	else{
        while(std::getline(file_input, str)){
            if(str.find(str_platform_name_leftside) == 0){
                str.erase(str.begin(), str.begin()+strlen(str_platform_name_leftside));
                str_platform_name = str;
            }
            else if(str.find(str_platform_vendor_leftside) == 0){
                str.erase(str.begin(), str.begin()+strlen(str_platform_vendor_leftside));
                str_platform_vendor = str;
            }
            else if(str.find(str_device_name_leftside) == 0){
                str.erase(str.begin(), str.begin()+strlen(str_device_name_leftside));
                str_device_name = str;
            }
            else if(str.find(str_device_vendor_leftside) == 0){
                str.erase(str.begin(), str.begin()+strlen(str_device_vendor_leftside));
                str_device_vendor = str;
            }
            else if(str.find(str_device_uniqueID_leftside) == 0){
                str.erase(str.begin(), str.begin()+strlen(str_device_uniqueID_leftside));
                str_device_uniqueID = str;
            }
        }
		specified_device_uniqueID = strtoul(str_device_uniqueID.c_str(), NULL, 10);
		
		for (int i = 0; i < (pd->num_platform) && !configureCorrespondenceFlg; i++) {
			for (int j = 0; j < (pd->num_device)[i] && !configureCorrespondenceFlg; j++) {
				if ((pd->device_uniqueID)[i][j] == specified_device_uniqueID) {
					configureCorrespondenceFlg = 1*((strcmp((pd->platform_vendor)[i],  str_platform_vendor.c_str()) == 0) &&
													(strcmp((pd->platform_name)[i],    str_platform_name.c_str())   == 0) &&
													(strcmp((pd->device_vendor)[i][j], str_device_vendor.c_str())   == 0) &&
													(strcmp((pd->device_name)[i][j],   str_device_name.c_str())     == 0));
					if (configureCorrespondenceFlg) {
						p_index = i;
						d_index = j;
					}
				}
			}
		}
		if (configureCorrespondenceFlg) {
			mainCLHandler->mainPlatform = (pd->platform)[p_index];
			mainCLHandler->mainDevice = (pd->device)[p_index][d_index];

			mainCLHandler->platform_index = p_index;
			mainCLHandler->platform_name = (pd->platform_name)[p_index];
			mainCLHandler->platform_vendor = (pd->platform_vendor)[p_index];

			mainCLHandler->device_index = d_index;
			mainCLHandler->device_name = (pd->device_name)[p_index][d_index];
			mainCLHandler->device_vendor = (pd->device_vendor)[p_index][d_index];
			mainCLHandler->device_uniqueID = (pd->device_uniqueID)[p_index][d_index];
		}
		else {
			printf("NOTE(%s) : settings.configure contains invalid parameters. The file will be created automatically during initial setup.\n", __func__);
			return SIMPLECL_LOAD_FAIL;
		}
    }
	return SIMPLECL_LOAD_SUCCESS;
}

simpleCLstandardEnum writeInfoToConfigure(simpleCLhandler &mainCLHandler){

	if (mainCLHandler->platform_name == NULL || mainCLHandler->platform_vendor == NULL || mainCLHandler->device_name == NULL || mainCLHandler->device_vendor == NULL) {
		printf("WARNING (%s) : mainCLHandler not fully initialized. Function called but not executed.\n", __func__);
		return SIMPLECL_WRITE_FAIL;
	}
	else {
		str_platform_name   = std::string(mainCLHandler->platform_name);
		str_platform_vendor = std::string(mainCLHandler->platform_vendor);
		str_device_name     = std::string(mainCLHandler->device_name);
		str_device_vendor   = std::string(mainCLHandler->device_vendor);
		str_device_uniqueID = std::to_string(mainCLHandler->device_uniqueID);

		file_output.open("settings.configure", std::ofstream::out | std::ofstream::trunc);
		std::string str = "";

		str += std::string(str_platform_name_leftside) + str_platform_name + "\n";
		str += std::string(str_platform_vendor_leftside) + str_platform_vendor + "\n";
		str += std::string(str_device_name_leftside) + str_device_name + "\n";
		str += std::string(str_device_vendor_leftside) + str_device_vendor + "\n";
		str += std::string(str_device_uniqueID_leftside) + str_device_uniqueID + "\n";
		file_output.write(str.c_str(), sizeof(char)*str.size());
		file_output.close();
	}
	return SIMPLECL_WRITE_SUCCESS;
}

/*void configureFileReadWrite() {
	if (configureFileReadWriteCallFlg) {
		printf("WARNING (%s) : This function already called once. Function not executed.\n", __func__);
	}
	else {
		readInfoFromFile();
		if (file_input.fail()) {
			writeInfoToFile();
		}
		configureFileReadWriteCallFlg = 1;
	}
}*/

