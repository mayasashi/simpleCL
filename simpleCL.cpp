#include "simpleCL.h"

typedef struct innerChainContainer {
	void(*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

simpleCLhandler mainCLHandler;
innerChainHandler iCH;

void simpleCL_init() {
	iCH = (innerChainHandler)malloc(sizeof(_innerChainContainer));
	iCH->nextAddress = NULL;
	iCH->lastAddress = NULL;
	queryPlatformAndDevice();
	queryPlatformAndDeviceInfo();
	printPlatformAndDeviceInfo();
}