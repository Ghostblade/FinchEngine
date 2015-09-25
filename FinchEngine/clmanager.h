#ifndef CL_MANAGER
#define CL_MANAGER

#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <vector>

#define VALIDATE_FUNC(func) \
if (cl_status != CL_SUCCESS)\
	{\
	printf("function %s failed, with %s\n", #func, m_errors[cl_status]); \
	return; \
	}\


class CLManager
{
public:
	~CLManager();
	static CLManager* getSingletonPtr(){
		static CLManager clm;
		return &clm;
	}
	void getErrorInfo(const char* funcname, int idx);
private:
	CLManager();
	CLManager(CLManager const&);
	void operator =(CLManager const&);

	const char* m_errors[69];
	cl_context m_context;
	cl_platform_id m_platform;
	std::vector<cl_device_id> m_devices;
	std::vector<cl_command_queue> m_queues;
};


#endif
