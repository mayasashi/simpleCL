#include "simpleCLheaderGenerate.h"


struct fStr{
    char *              funcName;
    size_t              funcName_Length;
    std::vector<char *> argument_name;
    std::vector<char *> argument_data;
};

struct kernel_t {
    
    /*standard variables*/
	FILE *file;				/*File pointer*/
	char *path;				/*File path of the kernel file*/
	char *name;				/*Name used to specify the kernel file*/
    char *data;				/*Char pointer to actual data in memory*/
    long  data_length;		/*Length of actual data in memory*/

	cl_build_status buildStatus;			/*Program build status*/
	char         *  buildInfoLog;			/*Program build info log returned when a program compilation fails*/
	size_t          buildInfoLogLength;		/*Length of build info log*/
    
    std::vector<fStr *> funcVector;
    
    
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
	FREE_SAFE(buildInfoLog);
}

typedef std::vector<kernel_t*> kernel_vec;

bool quotationFlg = false;
bool comment1Flg = false;
bool comment2Flg = false;

kernel_vec *mainKernelVec = NULL;   /*Main kernel vector object*/

struct kernelContainer {
	kernel_vec *kernelVec;
	kernelContainer() {
		printf("constructor called.\n");
		if (mainKernelVec == NULL)
		{
			mainKernelVec = (kernel_vec *)malloc(sizeof(kernel_vec));
		}
		kernelVec = mainKernelVec;
	}
	~kernelContainer() {
		printf("destructor called.\n");
		if (mainKernelVec->empty() == false) {
			for (
				kernel_vec::iterator itr = mainKernelVec->begin();
				itr < mainKernelVec->end();
				itr++
				)
			{
				if (*itr != NULL) delete *itr;
			}
			mainKernelVec->clear();
		}
		FREE_SAFE(mainKernelVec);
	}
};

void quotationCommentQuery(char u, char w,unsigned int i);
inline bool variableCharFlg(char c);

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
        
      /*1:Open a kernel file*/
        
        (*itr)->file = fopen((*itr)->path, "r");
        
      /*2:Obtain the file size*/
        
        fseek((*itr)->file, 0L, SEEK_END);
        (*itr)->data_length = ftell((*itr)->file);
        rewind((*itr)->file);
        
      /*3:Allocate memory to store the kernel data*/
        
        (*itr)->data = (char *)malloc((*itr)->data_length);
        
      /*4:Load data*/
        
        for(int i = 0; i < (*itr)->data_length; i++)
        {
            ((*itr)->data)[i] = (char)fgetc((*itr)->file);
        }
    }
}

void kernelHandler::buildProgram(simpleCLhandler handler){
    for(
		kernel_vec::iterator itr = mainKernelVec->begin();
        itr < mainKernelVec->end();
        itr++
		)
    {
		/*1:Create OpenCL program objects.*/
        (*itr)->program = clCreateProgramWithSource(
			handler->mainContext,                    /*main context*/                    
			1,                                       /*count*/
			(const char **)&((*itr)->data),          /*data pointer*/
			(const size_t *)&((*itr)->data_length),  /*data length pointer*/
            NULL									 /*error code*/
		);                                   
        
		/*2 : Build OpenCL programs.*/
        clBuildProgram(
			(*itr)->program,        /*program*/
			1,                      /*num_devices*/
			&handler->mainDevice,   /*device_list*/
			"",                     /*options*/
			NULL,                   /*pfn_notify*/
			NULL                    /*user_data*/
		);

		/*3 : Check whether the OpenCL programs were built successfully.*/
		clGetProgramBuildInfo(
			(*itr)->program,          /*program*/
			handler->mainDevice,      /*device*/
			CL_PROGRAM_BUILD_STATUS,  /*build_info*/
			sizeof(cl_build_status),  /*param_value_size*/
			&((*itr)->buildStatus),   /*param_value*/
			NULL                      /*param_value_size_ret*/
		);

		/*4 : If an error occurred during the OpenCL program compilation, obtain a build info log from the specified device.*/
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

void kernelHandler::printProgramBuildInfo(simpleCLhandler handler) {
	printf("<PROGRAM BUILD_INFO>\n\n");
	printf("Compile Device Used : %s\n\n", handler->device_name);
	for (
		kernel_vec::iterator itr = mainKernelVec->begin();
		itr < mainKernelVec->end();
		itr++
		)
	{
			printf("	[NAME : %s]\n", (*itr)->name);
			printf("BuildStatus : %d\n", (*itr)->buildStatus);
		if ((*itr)->buildStatus != CL_BUILD_SUCCESS && (*itr)->buildStatus != CL_BUILD_NONE) {
			printf("(BuildLog)\n%s\n", (*itr)->buildInfoLog);
		}
	}
}

void kernelHandler::generateHeaderString(){

	bool buildSuccessFlg = true;

	quotationFlg = false;
	comment1Flg = false;
	comment2Flg = false;

    /*1 : Check whether all programs are built successfully. Otherwise abort the following process.*/
    for
        (
         kernel_vec::iterator itr = mainKernelVec->begin();
         itr < mainKernelVec->end() && buildSuccessFlg;
         itr++
        )
    {
		buildSuccessFlg *= ((*itr)->buildStatus == CL_BUILD_SUCCESS);
    }

	if (!buildSuccessFlg) {
		printf("SIMPLECLHEADERGENERATE (%s) : There's at least one program that was not built successfully. Aborting the process.\n");
	}
	else {
		/*2: Search kernel functions in the kernel file.*/
		
		unsigned int searchIndex = 0;
		
		for (
			kernel_vec::iterator itr = mainKernelVec->begin();
			itr < mainKernelVec->end();
			itr++
			)
		{
			/* Search in the order of "kernel(__kernel)", "void" and "function name". */
            while(findTwoStringsWithSpace(
                                          (*itr)->data,
                                          "kernel",
                                          "void",
                                          searchIndex,
                                          &searchIndex,
                                          quotationCommentQuery
                                          ) != 0
                  )
            {
                /*if we find the characters "kernel void" */
                
                if(!quotationFlg && !comment1Flg && !comment2Flg){
                    while(((*itr)->data)[searchIndex] == ' ' || ((*itr)->data)[searchIndex] == '\t'){
                        searchIndex++;
                    }
                    
                    ((*itr)->funcVector).push_back(new fStr);
                    fStr *st = ((*itr)->funcVector).back();
                    
                    while(((*itr)->data)[searchIndex] == '('){
                        searchIndex++;
                    }
                }
            }
		}
	}
}

void quotationCommentQuery(char u, char w,unsigned int i)
{
	if (u == '"' && !quotationFlg && !comment1Flg && !comment2Flg) quotationFlg = true;
	if (u == '"' && w != '\\' && quotationFlg) quotationFlg = false;

	if (u == '/' && w == '/' && !quotationFlg && !comment2Flg) comment1Flg = true;
	if (u == '\n' && comment1Flg) comment1Flg = false;

	if (u == '*' && w == '/' && !quotationFlg && !comment1Flg) comment2Flg = true;
	if (u == '/' && w == '*' && comment2Flg) comment2Flg = false;
}

inline bool variableCharFlg(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}
