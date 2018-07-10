#include "simpleCLheaderGenerate.h"

struct kernel_t {
	FILE *file;
	char *path;
	char *name;
    char *data;
	int   buildflg;

	/*constructor and destructor*/
	kernel_t(const char *arg_path, const char *arg_name);
	~kernel_t();
};

kernel_t::kernel_t(const char *arg_path, const char *arg_name) {
	size_t arg_path_length = strlen(arg_path);
	size_t arg_name_length = strlen(arg_name);
	path = (char *)malloc(sizeof(char)*(arg_path_length + 1));
	name = (char *)malloc(sizeof(char)*(arg_name_length + 1));
}

kernel_t::~kernel_t() {
	FREE_SAFE(path);
	FREE_SAFE(name);
}

typedef std::vector<kernel_t*> kernel_vec;

kernel_vec *mainKernelVec = NULL;

struct kernelContainer {
	kernel_vec *kernelVec;
	kernelContainer() {
		printf("constructor called.\n");
		if (mainKernelVec == NULL)
		{
			mainKernelVec = (std::vector<kernel_t*> *)malloc(sizeof(std::vector<kernel_t*>));
		}
		kernelVec = mainKernelVec;
	}
	~kernelContainer() {
		printf("destructor called.\n");
		FREE_SAFE(mainKernelVec);
	}
};

kernelHandler::kernelHandler(){
	ptr = new kernelContainer;
}

kernelHandler::~kernelHandler() {
	delete ptr;
}


void kernelHandler::addKernelProgram(const char *path, const char *name) {
    mainKernelVec->push_back(new kernel_t(path,name));
}

void kernelHandler::loadProgramFile(){
    long size;
    kernel_vec::iterator itr;
    for(itr = mainKernelVec->begin(); itr < mainKernelVec->end(); itr++){
        (*itr)->file = fopen((*itr)->path, "r");
        fseek((*itr)->file, 0L, SEEK_END);
        size = ftell((*itr)->file);
        rewind((*itr)->file);
        (*itr)->data = (char *)malloc(size);
        
    }
    
}
