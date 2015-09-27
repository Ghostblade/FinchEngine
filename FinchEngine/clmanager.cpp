#include "clmanager.h"
#include <windows.h>
#include <CL/cl_ext.h>
#include <stdio.h>

CLManager::CLManager() :m_context(NULL)
{
	m_errors[-CL_SUCCESS] = "CL_SUCCESS";
	m_errors[-CL_DEVICE_NOT_FOUND] = "CL_DEVICE_NOT_FOUND";
	m_errors[-CL_DEVICE_NOT_AVAILABLE] = "CL_DEVICE_NOT_AVAILABLE";
	m_errors[-CL_COMPILER_NOT_AVAILABLE] = "CL_COMPILER_NOT_AVAILABLE";
	m_errors[-CL_MEM_OBJECT_ALLOCATION_FAILURE] = "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	m_errors[-CL_OUT_OF_RESOURCES] = "CL_OUT_OF_RESOURCES";
	m_errors[-CL_OUT_OF_HOST_MEMORY] = "CL_OUT_OF_HOST_MEMORY";
	m_errors[-CL_PROFILING_INFO_NOT_AVAILABLE] = "CL_PROFILING_INFO_NOT_AVAILABLE";
	m_errors[-CL_MEM_COPY_OVERLAP] = "CL_MEM_COPY_OVERLAP";
	m_errors[-CL_IMAGE_FORMAT_MISMATCH] = "CL_IMAGE_FORMAT_MISMATCH";
	m_errors[-CL_IMAGE_FORMAT_NOT_SUPPORTED] = "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	m_errors[-CL_BUILD_PROGRAM_FAILURE] = "CL_BUILD_PROGRAM_FAILURE";
	m_errors[-CL_MAP_FAILURE] = "CL_MAP_FAILURE";
	m_errors[-CL_INVALID_VALUE] = "CL_INVALID_VALUE";
	m_errors[-CL_INVALID_DEVICE_TYPE] = "CL_INVALID_DEVICE_TYPE";
	m_errors[-CL_INVALID_PLATFORM] = "CL_INVALID_PLATFORM";
	m_errors[-CL_INVALID_DEVICE] = "CL_INVALID_DEVICE";
	m_errors[-CL_INVALID_CONTEXT] = "CL_INVALID_CONTEXT";
	m_errors[-CL_INVALID_QUEUE_PROPERTIES] = "CL_INVALID_QUEUE_PROPERTIES";
	m_errors[-CL_INVALID_COMMAND_QUEUE] = "CL_INVALID_COMMAND_QUEUE";
	m_errors[-CL_INVALID_HOST_PTR] = "CL_INVALID_HOST_PTR";
	m_errors[-CL_INVALID_MEM_OBJECT] = "CL_INVALID_MEM_OBJECT";
	m_errors[-CL_INVALID_IMAGE_FORMAT_DESCRIPTOR] = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	m_errors[-CL_INVALID_IMAGE_SIZE] = "CL_INVALID_IMAGE_SIZE";
	m_errors[-CL_INVALID_SAMPLER] = "CL_INVALID_SAMPLER";
	m_errors[-CL_INVALID_BINARY] = "CL_INVALID_BINARY";
	m_errors[-CL_INVALID_BUILD_OPTIONS] = "CL_INVALID_BUILD_OPTIONS";
	m_errors[-CL_INVALID_PROGRAM] = "CL_INVALID_PROGRAM";
	m_errors[-CL_INVALID_PROGRAM_EXECUTABLE] = "CL_INVALID_PROGRAM_EXECUTABLE";
	m_errors[-CL_INVALID_KERNEL_NAME] = "CL_INVALID_KERNEL_NAME";
	m_errors[-CL_INVALID_KERNEL_DEFINITION] = "CL_INVALID_KERNEL_DEFINITION";
	m_errors[-CL_INVALID_KERNEL] = "CL_INVALID_KERNEL";
	m_errors[-CL_INVALID_ARG_INDEX] = "CL_INVALID_ARG_INDEX";
	m_errors[-CL_INVALID_ARG_VALUE] = "CL_INVALID_ARG_VALUE";
	m_errors[-CL_INVALID_ARG_SIZE] = "CL_INVALID_ARG_SIZE";
	m_errors[-CL_INVALID_KERNEL_ARGS] = "CL_INVALID_KERNEL_ARGS";
	m_errors[-CL_INVALID_WORK_DIMENSION] = "CL_INVALID_WORK_DIMENSION";
	m_errors[-CL_INVALID_WORK_GROUP_SIZE] = "CL_INVALID_WORK_GROUP_SIZE";
	m_errors[-CL_INVALID_WORK_ITEM_SIZE] = "CL_INVALID_WORK_ITEM_SIZE";
	m_errors[-CL_INVALID_GLOBAL_OFFSET] = "CL_INVALID_GLOBAL_OFFSET";
	m_errors[-CL_INVALID_EVENT_WAIT_LIST] = "CL_INVALID_EVENT_WAIT_LIST";
	m_errors[-CL_INVALID_EVENT] = "CL_INVALID_EVENT";
	m_errors[-CL_INVALID_OPERATION] = "CL_INVALID_OPERATION";
	m_errors[-CL_INVALID_GL_OBJECT] = "CL_INVALID_GL_OBJECT";
	m_errors[-CL_INVALID_BUFFER_SIZE] = "CL_INVALID_BUFFER_SIZE";
	m_errors[-CL_INVALID_MIP_LEVEL] = "CL_INVALID_MIP_LEVEL";
	m_errors[-CL_INVALID_GLOBAL_WORK_SIZE] = "CL_INVALID_GLOBAL_WORK_SIZE";
	m_errors[-CL_INVALID_IMAGE_DESCRIPTOR] = "CL_INVALID_IMAGE_DESCRIPTOR";
	m_errors[-CL_INVALID_COMPILER_OPTIONS] = "CL_INVALID_COMPILER_OPTIONS";
	m_errors[-CL_INVALID_LINKER_OPTIONS] = "CL_INVALID_LINKER_OPTIONS";
	m_errors[-CL_INVALID_DEVICE_PARTITION_COUNT] = "CL_INVALID_DEVICE_PARTITION_COUNT";
	m_errors[-CL_INVALID_PROPERTY] = "CL_INVALID_PROPERTY";
	cl_int cl_status;

	cl_uint platform_cnt = 0;
	cl_status  = clGetPlatformIDs(0, NULL, &platform_cnt);
	VALIDATE_FUNC(clGetPlatformIDs);

	clGetGLContextInfoKHR_fn _clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddress("clGetGLContextInfoKHR");

	cl_platform_id platform[100];
	cl_status = clGetPlatformIDs(platform_cnt, platform, NULL);
	VALIDATE_FUNC(clGetPlatformIDs);

	for (int i = 0; i < platform_cnt;++i)
	{
		cl_context_properties GL_PROPS[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform[i],
			0
		};

		cl_device_id devices[100];
		cl_uint device_cnt;
		cl_status = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_GPU, 100, devices, &device_cnt);
		if (cl_status!=CL_SUCCESS)
		{
			continue;
		}
		if (device_cnt)
		{
			size_t gl_device_cnt = 0;
			cl_device_id gl_device;
			cl_status = _clGetGLContextInfoKHR(GL_PROPS, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), &gl_device, &gl_device_cnt);
			VALIDATE_FUNC(_clGetGLContextInfoKHR);

			if (!gl_device_cnt)
			{
				//you need a better machine, you need a decent n-card
				return;
			}
			else{
				m_context = clCreateContext(GL_PROPS, device_cnt, devices, NULL, NULL, &cl_status);
				VALIDATE_FUNC(clCreateContext);
				m_platform = platform[i];

				char namebuf[200];
				cl_status = clGetPlatformInfo(m_platform, CL_PLATFORM_VENDOR, sizeof(namebuf), namebuf, NULL);
				printf("OpenCL platform %s is created!\n", namebuf);

				for (int i = 0; i < device_cnt;++i)
				{
					m_devices.push_back(devices[i]);
				}

				for (int i = 0; i < device_cnt;++i)
				{
					cl_command_queue cur_queue = clCreateCommandQueue(m_context, m_devices[i], CL_QUEUE_PROFILING_ENABLE, &cl_status);
					VALIDATE_FUNC(clCreateCommandQueue);
					
					m_queues.push_back(cur_queue);
				}
			}
		}
	}
}

char* CLManager::readFile(const char* filename, size_t* size){
	FILE* handle = fopen(filename, "r");

	if (!handle)
	{
		perror("file not found!!!!!\n");
		exit(1);
	}
	fseek(handle, 0, SEEK_END);

	*size = (size_t)ftell(handle);
	rewind(handle);
	char* buffer = (char*)malloc(*size + 1);
	buffer[*size] = '\0';

	fread(buffer, sizeof(char), *size, handle);
	fclose(handle);

	return buffer;
}

void CLManager::setSource(const char* srcname){
	char *programBuf, *programLog;
	size_t programSize, logSize;
	int err;

	programBuf = readFile(srcname, &programSize);
	m_program = clCreateProgramWithSource(m_context, 1, (const char**)&programBuf, &programSize, &err);

	if (err < 0){
		perror("program cannot be created!\n");
		exit(1);
	}
	free(programBuf);

	err = clBuildProgram(m_program, 0, NULL, NULL, NULL, NULL);
	if (err < 0){
		clGetProgramBuildInfo(m_program, m_devices[0], CL_PROGRAM_BUILD_LOG,
			0, NULL, &logSize);
		programLog = (char*)malloc(logSize + 1);
		programLog[logSize] = '\0';
		clGetProgramBuildInfo(m_program, m_devices[0], CL_PROGRAM_BUILD_LOG,
			logSize + 1, programLog, NULL);
		printf("%s\n", programLog);
		free(programLog);
		exit(1);
	}
}

void CLManager::getErrorInfo(const char* funcname, int idx){
	printf("%s %s\n", funcname, m_errors[idx]);
}

CLManager::~CLManager()
{
	if (!m_queues.empty())
	{
		for (int i = 0; i < m_queues.size();++i)
		{
			clReleaseCommandQueue(m_queues[i]);
		}
		clReleaseContext(m_context);
		clReleaseProgram(m_program);
	}
}
