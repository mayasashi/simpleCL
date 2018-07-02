#include "simpleCLIOHardwareInfo.h"

typedef struct innerChainContainer{
	void                (*p)();
	innerChainContainer *nextAddress;
	innerChainContainer *lastAddress;
}_innerChainContainer;

void cleanup();

void registerCleanUpChain() {

	innerChainHandler iCH_sub = (innerChainHandler)malloc(sizeof(_innerChainContainer));

	iCH_sub->p = cleanup;
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

void checkInfoFile() {

}
