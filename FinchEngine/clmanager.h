#ifndef CL_MANAGER
#define CL_MANAGER

#include <CL/cl.h>
#include <CL/cl_gl.h>

class CLManager
{
public:
	CLManager();
	~CLManager();

private:
	const char* m_errors[68];
};


#endif
