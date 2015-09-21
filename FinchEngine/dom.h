#ifndef DOM_H
#define DOM_H

#include <QGL>
#include <QOpenGLFunctions_4_3_Core>
#include "material.h"

class DOM
{
public:
	DOM();
	~DOM();
private:
	GLuint m_vao;
};


#endif

