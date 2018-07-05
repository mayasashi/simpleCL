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
    setMainPlatformAndDevice();
    cl_context_properties properties[3] = {CL_CONTEXT_PLATFORM,(cl_context_properties)*(mainCLHandler->mainPlatform),0};
    mainCLHandler->mainContext = (cl_context *)malloc(sizeof(cl_context));
    *(mainCLHandler->mainContext) = clCreateContext(properties, 1, mainCLHandler->mainDevice, NULL, NULL, NULL);
    
    
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
    clReleaseContext(*(mainCLHandler->mainContext));
    FREE_SAFE(mainCLHandler->mainContext);
	FREE_SAFE(iCH);
	FREE_SAFE(mainCLHandler);
}
