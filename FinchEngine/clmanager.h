#ifndef CL_MANAGER
#define CL_MANAGER

#include <CL/cl.h>
#include <CL/cl_gl.h>

#define VALIDATE_KENEL(kernel) \
if (sStatusCL != CL_SUCCESS)\
	{\
	Log(ERR, this) << "Failure in call to clEnqueueNDRangeKernel for kernel " << #kernel << " : " << oclError(); \
	return false; \
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
	
};


#endif
