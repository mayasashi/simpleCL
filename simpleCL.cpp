#include "simpleCL.h"

typedef struct innerChainContainer {
	void(*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

simpleCLhandler mainCLHandler;
innerChainHandler iCH;

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



bool simpleCLalreadySetFlg = 0;

simpleCLhandler & simpleCL_init() {

	if (simpleCLalreadySetFlg) {
		printf("SIMPLECL_WARNING (%s) : This function was used once for initialization.");
	}
	else {
		iCH = (innerChainHandler)malloc(sizeof(_innerChainContainer));
		iCH->nextAddress = NULL;
		iCH->lastAddress = NULL;

		mainCLHandler = (simpleCLhandler)malloc(sizeof(_simpleCLcontainer));

		queryPlatformAndDevice();
		queryPlatformAndDeviceInfo();
		printPlatformAndDeviceInfo();
		if (readInfoFromConfigure(mainCLHandler) == SIMPLECL_LOAD_FAIL) {
			selectMainPlatformAndDevice(mainCLHandler);
			writeInfoToConfigure(mainCLHandler);
		}
		setMainPlatformAndDevice(mainCLHandler);
		cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM,(cl_context_properties)(mainCLHandler->mainPlatform),0 };
		mainCLHandler->mainContext = clCreateContext(properties, 1, &(mainCLHandler->mainDevice), NULL, NULL, NULL);

		mainCLHandler->mainQueue = clCreateCommandQueue(mainCLHandler->mainContext, mainCLHandler->mainDevice, 0, NULL);
	}

	return mainCLHandler;
}

void simpleCL_close(simpleCLhandler &handler) {
	if (handler != mainCLHandler)
	{
		printf("SIMPLECL_WARNING (%s) : Invalid argument. It seems that specified handler references incorrect data.");
	}
	else {
		if (iCH->lastAddress != NULL && iCH->nextAddress != NULL) {
			innerChainHandler currentChainAddress = iCH->nextAddress;
			(*(currentChainAddress->p))();
			while (currentChainAddress != iCH->lastAddress)
			{
				currentChainAddress = currentChainAddress->nextAddress;
				(*(currentChainAddress->p))();
			}
		}
		
		clReleaseCommandQueue(handler->mainQueue);
		clReleaseContext(handler->mainContext);
		
		FREE_SAFE(iCH);
		FREE_SAFE(handler);
	}
}
