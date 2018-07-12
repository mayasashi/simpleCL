#include "simpleCLheaderGenerate.h"

struct kernel_t {
    
    /*standard variables*/
	FILE *file;
	char *path;
	char *name;
    char *data;
    long  data_length;

	cl_build_status buildStatus;
	char         *  buildInfoLog;
	size_t          buildInfoLogLength;
    
    
    /*opencl variables*/
    cl_program program;

	/*constructor and destructor*/
	kernel_t(const char *arg_path, const char *arg_name);
	~kernel_t();
};

kernel_t::kernel_t(const char *arg_path, const char *arg_name) {
	size_t arg_path_length = strlen(arg_path);
	size_t arg_name_length = strlen(arg_name);
    file = NULL;
	path = (char *)malloc(sizeof(char)*(arg_path_length + 1));
	name = (char *)malloc(sizeof(char)*(arg_name_length + 1));
    data = NULL;
    buildStatus = 0;
}

kernel_t::~kernel_t() {
	FREE_SAFE(path);
	FREE_SAFE(name);
    FREE_SAFE(data);
}

typedef std::vector<kernel_t*> kernel_vec;

kernel_vec *mainKernelVec = NULL;   /*Main kernel vector object*/

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
    kernel_vec::iterator itr;
    
    for(itr = mainKernelVec->begin(); itr < mainKernelVec->end(); itr++){
        
      /*1:opening a file*/
        
        (*itr)->file = fopen((*itr)->path, "r");
        
      /*2:obtain the file size*/
        
        fseek((*itr)->file, 0L, SEEK_END);
        (*itr)->data_length = ftell((*itr)->file);
        rewind((*itr)->file);
        
      /*3:allocate memory for data storage*/
        
        (*itr)->data = (char *)malloc((*itr)->data_length);
        
      /*4:load data*/
        
        for(int i = 0; i < (*itr)->data_length; i++)
        {
            ((*itr)->data)[i] = (char)fgetc((*itr)->file);
        }
    }
}

void kernelHandler::buildProgram(simpleCLhandler handler){
    for(kernel_vec::iterator itr = mainKernelVec->begin();
        itr < mainKernelVec->end();
        itr++)
    {
        (*itr)->program = clCreateProgramWithSource(
			handler->mainContext,                    /*main context*/                    
			1,                                       /*count*/
			(const char **)&((*itr)->data),          /*data pointer*/
			(const size_t *)&((*itr)->data_length),  /*data length pointer*/
            NULL									 /*error code*/
		);                                   
        
        clBuildProgram(
			(*itr)->program,        /*program*/
			1,                      /*num_devices*/
			&handler->mainDevice,   /*device_list*/
			"",                     /*options*/
			NULL,                   /*pfn_notify*/
			NULL                    /*user_data*/
		);

		clGetProgramBuildInfo(
			(*itr)->program,          /*program*/
			handler->mainDevice,      /*device*/
			CL_PROGRAM_BUILD_STATUS,  /*build_info*/
			sizeof(cl_build_status),  /*param_value_size*/
			&((*itr)->buildStatus),   /*param_value*/
			NULL                      /*param_value_size_ret*/
		);

		if ((*itr)->buildStatus != CL_BUILD_SUCCESS && (*itr)-> buildStatus != CL_BUILD_NONE)
		{
			clGetProgramBuildInfo(
				(*itr)->program,					/*program*/
				handler->mainDevice,				/*device*/
				CL_PROGRAM_BUILD_LOG,				/*build_info*/
				0,									/*param_value_size*/
				NULL,								/*param_value*/
				&((*itr)->buildInfoLogLength)		/*param_value_size_ret*/
			);
			if ((*itr)->buildInfoLogLength > 1)
			{
				(*itr)->buildInfoLog = (char *)malloc((*itr)->buildInfoLogLength);
				clGetProgramBuildInfo(
					(*itr)->program,				/*program*/
					handler->mainDevice,			/*device*/
					CL_PROGRAM_BUILD_LOG,			/*build_info*/
					(*itr)->buildInfoLogLength,		/*param_value_size*/
					(*itr)->buildInfoLog,			/*param_value*/
					NULL							/*param_value_size_ret*/
				);
			}
		}
    }
    
    
}
