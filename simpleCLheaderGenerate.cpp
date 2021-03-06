#include "simpleCLheaderGenerate.h"

char *clTypeStr_REF[][2] = {
	{ "bool","bool" },
	{ "char","cl_char" },
	{ "uchar","cl_uchar" },
	{ "short","cl_short" },
	{ "int","cl_int" },
	{ "uint","cl_uint" },
	{ "long","cl_long" },
	{ "ulong","cl_ulong" },
	{ "float","cl_float" },
	{ "double","cl_double" },
	{ "half","cl_half" },
	{ "size_t","size_t" },
	{ "ptrdiff_t","ptrdiff_t" },
	{ "intptr_t","intptr_t" },
	{ "uintptr_t","uintptr_t" },
	{ "void","void" }
};

struct kernel_t;

typedef struct IDChainContainer {
	cl_uint ID;
	kernel_t *k;
	IDChainContainer *nextAddress;
	IDChainContainer *lastAddress;
}_IDChainContainer;

typedef _IDChainContainer *IDChainHandler;



IDChainHandler mainICH = NULL;

kernel_t * getKernelUsingID(cl_uint ID)
{
	bool foundFlg = false;
	if (mainICH == NULL) return NULL;
	else {
		IDChainHandler searchICH = mainICH;
		while (!foundFlg && searchICH->nextAddress != NULL)
		{
			searchICH = searchICH->nextAddress;
			foundFlg = (searchICH->ID == ID);
		}
		if (foundFlg) {
			return searchICH->k;
		}
	}
	return NULL;
}
struct fStr{
    char *              funcName;
    size_t              funcName_Length;
    std::vector<char *> argument_name;
    std::vector<void *> argument_value_ptr;
	std::vector<size_t> argument_value_size;
};

struct kernel_t {
    
    /*standard variables*/
	FILE *file;				/*File pointer*/
	char *path;				/*File path of the kernel file*/
	char *name;				/*Name used to specify the kernel file*/
    char *data;				/*Char pointer to actual data in memory*/
    long  data_length;		/*Length of actual data in memory*/
	bool notFound;          /*When a file object fails to load the kernel file, this flag is set to true.*/

	cl_build_status buildStatus;			/*Program build status*/
	char         *  buildInfoLog;			/*Program build info log returned when a program compilation fails*/
	size_t          buildInfoLogLength;		/*Length of build info log*/
    
    std::vector<fStr *> funcVector;
	IDChainHandler ICH;
    
    /*opencl variables*/
    cl_program program;
	bool *quotation_or_comment;

	/*constructor and destructor*/
	kernel_t(const char *arg_path, const char *arg_name, cl_uint arg_ID);
	~kernel_t();
};

kernel_t::kernel_t(const char *arg_path, const char *arg_name, cl_uint arg_ID) {
	size_t arg_path_length = strlen(arg_path);
	size_t arg_name_length = strlen(arg_name);
    file = NULL;
	path = (char *)malloc(sizeof(char)*(arg_path_length + 1));
	name = (char *)malloc(sizeof(char)*(arg_name_length + 1));
	sprintf(path, "%s", arg_path);
	sprintf(name, "%s", arg_name);
    data = NULL;
	notFound = false;
    buildStatus = 0;
	buildInfoLog = NULL;
	buildInfoLogLength = 0;
	quotation_or_comment = NULL;
	
	ICH = (IDChainHandler)malloc(sizeof(_IDChainContainer));
	ICH->ID = arg_ID;
	ICH->k = this;
	ICH->nextAddress = NULL;
	ICH->lastAddress = NULL;

	if (mainICH->lastAddress != NULL)
	{
		(mainICH->lastAddress)->nextAddress = ICH;
	}
	else {
		mainICH->nextAddress = ICH;
	}
	mainICH->lastAddress = ICH;
}

kernel_t::~kernel_t() {
	FREE_SAFE(path);
	FREE_SAFE(name);
    FREE_SAFE(data);
	FREE_SAFE(buildInfoLog);
	FREE_SAFE(quotation_or_comment);
	FREE_SAFE(ICH);
}

typedef std::vector<kernel_t*> kernel_vec;

bool quotationFlg = false;
bool comment1Flg = false;
bool comment2Flg = false;
bool comment2restrict = false;
int comment2restrictCount = 0;

kernel_vec *mainKernelVec = NULL;   /*Main kernel vector object*/

struct kernelContainer {
	kernel_vec *kernelVec;
	IDChainHandler ICH;
	kernelContainer() {
		printf("constructor called.\n");
		if (mainKernelVec == NULL)
		{
			mainKernelVec = new kernel_vec();
		}
		if (mainICH == NULL)
		{
			mainICH = (IDChainHandler)malloc(sizeof(_IDChainContainer));
			mainICH->ID = NULL;
			mainICH->k = NULL;
			mainICH->lastAddress = NULL;
			mainICH->nextAddress = NULL;
		}
		kernelVec = mainKernelVec;
		ICH = mainICH;
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
			FREE_SAFE(mainICH);
		}
		delete kernelVec;
	}
};

void quotationCommentQuery(char t, char u,char v);
bool quotationCommentQueryWithFlg(char t, char u, char v);
inline bool variableCharFlg(char c);

kernelHandler::kernelHandler(){
	ptr = new kernelContainer;
}

kernelHandler::~kernelHandler() {
	delete ptr;
}


void kernelHandler::addKernelProgramFile(const char *path, const char *name, cl_uint ID) {
	mainKernelVec->push_back(new kernel_t(path, name, ID));
}

void kernelHandler::loadProgramFile(){
    kernel_vec::iterator itr;
    
    for(itr = mainKernelVec->begin(); itr < mainKernelVec->end(); itr++){
        
      /*1:Open a kernel file*/
        
        (*itr)->file = fopen((*itr)->path, "r");
		if ((*itr)->file == NULL)
		{
			printf("SIMPLECLHEADERGENERATE (%s) : kernel file not found.\n",__func__);
			printf("Source path : %s\n", (*itr)->path);
			(*itr)->notFound = true;
		}
		else {
			/*2:Obtain the file size*/

			fseek((*itr)->file, 0L, SEEK_END);
			(*itr)->data_length = ftell((*itr)->file);
			rewind((*itr)->file);

			/*3:Allocate memory to store the kernel data*/

			(*itr)->data = (char *)malloc((*itr)->data_length);
			(*itr)->quotation_or_comment = (bool *)malloc(sizeof(bool)*(*itr)->data_length);

			/*4:Load data*/

			quotationFlg = false;
			comment1Flg = false;
			comment2Flg = false;
			for (int i = 0; i < (*itr)->data_length; i++)
			{
				((*itr)->data)[i] = (char)fgetc((*itr)->file);
				if (i == 0)
				{
					((*itr)->quotation_or_comment)[i] = quotationCommentQueryWithFlg(0, ((*itr)->data)[i], ((*itr)->data)[i + 1]);
				}
				else if (i < (*itr)->data_length)
				{
					((*itr)->quotation_or_comment)[i] = quotationCommentQueryWithFlg(((*itr)->data)[i - 1], ((*itr)->data)[i], 0);
				}
				else
				{
					((*itr)->quotation_or_comment)[i] = quotationCommentQueryWithFlg(((*itr)->data)[i - 1], ((*itr)->data)[i], ((*itr)->data)[i + 1]);
				}
			}
		}
    }
}

void kernelHandler::buildProgram(simpleCLhandler handler){
    for(
		kernel_vec::iterator itr = mainKernelVec->begin();
        itr < mainKernelVec->end() &&
		!(*itr)->notFound;
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
		itr < mainKernelVec->end() &&
		!(*itr)->notFound;
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

cl_program & kernelHandler::getProgram(cl_uint ID)
{
	return (*getKernelUsingID(ID)).program;
}

void kernelHandler::generateHeaderString(){

	bool buildSuccessFlg = true;

	quotationFlg = false;
	comment1Flg = false;
	comment2Flg = false;

	bool reachEndFlg = false;

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
		unsigned int searchIndex_sub = 0;
		
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
				                          NULL,
				                          0,
                                          searchIndex,
										  &searchIndex_sub,
                                          &searchIndex,
                                          NULL
                                          ) != 0
                  )
            {
                /*if we find the characters "kernel void" */
                
                if(!((*itr)->quotation_or_comment)[searchIndex_sub]){
                    while(
						((*itr)->data)[searchIndex] == ' ' ||
						((*itr)->data)[searchIndex] == '\t' ||
						((*itr)->quotation_or_comment)[searchIndex]
						)
					{
                        searchIndex++;
                    }
                    
                    /*Now we can obtain the name of the function*/
                    
                    ((*itr)->funcVector).push_back(new fStr);
                    
                    fStr *st = ((*itr)->funcVector).back(); 
                    
                    st->funcName = &((*itr)->data)[searchIndex];
                    st->funcName_Length = 1;
                    
                    while(
						((*itr)->data)[searchIndex] != '(' &&
						((*itr)->data)[searchIndex] != ' ' &&
						((*itr)->data)[searchIndex] != '\t' &&
						((*itr)->data)[searchIndex] != '\n'

						)
					{
                        searchIndex++;
                        (st->funcName_Length)++;
                    }
                    
					
					while (!reachEndFlg)
					{
						while (
							(((*itr)->data)[searchIndex] != ',' && 
							 ((*itr)->data)[searchIndex] != ')')
							||
							quotationCommentQueryWithFlg(((*itr)->data)[searchIndex],((*itr)->data)[searchIndex - 1],0)
							)
						{
							searchIndex++;
						}
						if (((*itr)->data)[searchIndex] == ')') reachEndFlg = true;
						while (
							((*itr)->data)[searchIndex] != ' ' && ((*itr)->data)[searchIndex] != '\t' && ((*itr)->data)[searchIndex] != '\n'
							)
						{
							searchIndex--;
						}
					}
                    
                    
                }
            }
		}
	}
}

void quotationCommentQuery(char t,char u, char v)
{
	if (comment2restrict) comment2restrictCount++;
	if (comment2restrictCount == 3) { comment2restrictCount = 0; comment2restrict = false; }
	//  ' " '
	if (u == '"' && !quotationFlg && !comment1Flg && !comment2Flg) quotationFlg = true;
	else if (u == '"' && t != '\\' && quotationFlg) quotationFlg = false;

	//  ' // '
	if (u == '/' && v == '/' && !quotationFlg && !comment2Flg) comment1Flg = true;
	else if (u == '\n' && comment1Flg) comment1Flg = false;

	//  ' /* */ '
	if (u == '/' && v == '*' && !quotationFlg && !comment1Flg && !comment2restrict) { comment2Flg = true; comment2restrict = true; }
	else if (u == '/' && t == '*' && comment2Flg && !comment2restrict) comment2Flg = false;

}
bool quotationCommentQueryWithFlg(char t, char u, char v)
{
	quotationCommentQuery(t, u, v);
	return (quotationFlg || comment1Flg || comment2Flg);
}

inline bool variableCharFlg(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}
