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

void simpleCL_close() {
	if (iCH->lastAddress != NULL && iCH->nextAddress != NULL) {
		innerChainHandler currentChainAddress = iCH->nextAddress;
		(*(currentChainAddress->p))();
		while (currentChainAddress != iCH->lastAddress)
		{
			currentChainAddress = currentChainAddress->nextAddress;
			(*(currentChainAddress->p))();
		}
	}
}