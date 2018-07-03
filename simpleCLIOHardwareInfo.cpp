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

void readInfoFromFile(){
    std::string str;
    
    if(!file_input.fail()){
        while(std::getline(file_input, str)){
            if(str.find("MAIN_PLATFORM_NAME=") == 0){
                str.erase(str.begin(), str.begin()+strlen("MAIN_PLATFORM_NAME="));
                str_platform_name = str;
            }
            else if(str.find("MAIN_PLATFORM_VENDOR=") == 0){
                str.erase(str.begin(), str.begin()+strlen("MAIN_PLATFORM_VENDOR="));
                str_platform_vendor = str;
            }
            else if(str.find("MAIN_DEVICE_NAME=") == 0){
                str.erase(str.begin(), str.begin()+strlen("MAIN_DEVICE_NAME="));
                str_device_name = str;
            }
            else if(str.find("MAIN_DEVICE_VENDOR=") == 0){
                str.erase(str.begin(), str.begin()+strlen("MAIN_DEVICE_VENDOR="));
                str_device_vendor = str;
            }
            else if(str.find("MAIN_DEVICE_UNIQUEID=") == 0){
                str.erase(str.begin(), str.begin()+strlen("MAIN_DEVICE_UNIQUEID="));
                str_device_uniqueID = str;
            }
        }
    }
}

void writeInfoToFile(){
    
}

void openFile() {
    file_input.open("settings.configure");
    if(file_input.fail()){
        printf("NOTE(%s) : initialization file not found. Setting the configure file for the first start.\n");
    }
    
}
