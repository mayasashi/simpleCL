#include "simpleCL.h"

typedef struct innerChainContainer {
	void(*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

void registerFuncChain(void (*f)()) {
    
    innerChainHandler iCH_sub = (innerChainHandler)malloc(sizeof(_innerChainContainer));
    
    iCH_sub->p = f;
    iCH_sub->nextAddress = NULL;
    iCH_sub->lastAddress = NULL;
    
    if (iCH->lastAddress != NULL) {
        (iCH->lastAddress)->nextAddress = iCH_sub;
    }
    else {
        iCH->nextAddress = iCH_sub;
    }
    iCH->lastAddress = iCH_sub;
}

simpleCLhandler mainCLHandler;
innerChainHandler iCH;

void simpleCL_init() {

	iCH = (innerChainHandler)malloc(sizeof(_innerChainContainer));
	iCH->nextAddress = NULL;
	iCH->lastAddress = NULL;

	mainCLHandler = (simpleCLhandler)malloc(sizeof(_simpleCLcontainer));

	queryPlatformAndDevice();
	queryPlatformAndDeviceInfo();
	printPlatformAndDeviceInfo();
	if (readInfoFromConfigure() == SIMPLECL_LOAD_FAIL) {
		selectMainPlatformAndDevice();
		writeInfoToConfigure();
	}
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

	FREE_SAFE(iCH);
	FREE_SAFE(mainCLHandler);
}
