#include "simpleCLIOHardwareInfo.h"

typedef struct innerChainContainer{
	void                (*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

extern void registerFuncChain(void (*f)());

std::ifstream file_input;
std::ofstream file_output;

std::string str_platform_name;
std::string str_platform_vendor;
std::string str_device_name;
std::string str_device_vendor;
std::string str_device_uniqueID;

const char *str_platform_name_leftside   = "MAIN_PLATFORM_NAME=";
const char *str_platform_vendor_leftside = "MAIN_PLATFORM_VENDOR=";
const char *str_device_name_leftside     = "MAIN_DEVICE_NAME=";
const char *str_device_vendor_leftside   = "MAIN_DEVICE_VENDOR=";
const char *str_device_uniqueID_leftside = "MAIN_DEVICE_UNIQUEID=";

bool configureFileReadWriteCallFlg = 0;

void readInfoFromConfigure(){

	std::string str;
	file_input.open("settings.configure");
	if (file_input.fail()) {
		printf("NOTE(%s) : settings.configure not found. The file will be created automatically during initial setup.\n", __func__);
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
    }
}

void writeInfoToConfigure(){

	if (mainCLHandler->platform_name == NULL || mainCLHandler->platform_vendor == NULL || mainCLHandler->device_name == NULL || mainCLHandler->device_vendor == NULL) {
		printf("WARNING (%s) : mainCLHandler not fully initialized. Function called but not executed.\n", __func__);
	}
	else {
		str_platform_name = std::string(mainCLHandler->platform_name);
		str_platform_vendor = std::string(mainCLHandler->platform_vendor);
		str_device_name = std::string(mainCLHandler->device_name);
		str_device_vendor = std::string(mainCLHandler->device_vendor);
		str_device_vendor = std::to_string(mainCLHandler->device_uniqueID);

		file_output.open("settings.configure", std::ofstream::out | std::ofstream::trunc);
		std::string str = "";

		str += std::string(str_platform_name_leftside) + str_platform_name + "\n";
		str += std::string(str_platform_vendor_leftside) + str_platform_vendor + "\n";
		str += std::string(str_device_name_leftside) + str_device_name + "\n";
		str += std::string(str_device_vendor_leftside) + str_device_vendor + "\n";
		str += std::string(str_device_uniqueID_leftside) + str_device_uniqueID_leftside + "\n";
		file_output.write(str.c_str(), sizeof(char)*str.size());
		file_output.close();
	}
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

