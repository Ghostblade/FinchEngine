#ifndef COMPOSITER_H
#define COMPOSITER_H

#include <QGL>
#include <QOpenGLFunctions_4_3_Core>
#include "material.h"

const int COLORBUFCOUNT = 2;

class Compositer
{
public:
	Compositer(QOpenGLFunctions_4_3_Core* func, int swidth, int sheight);
	~Compositer();

private:
	QOpenGLFunctions_4_3_Core* m_func;

	GLuint m_vao;
	GLuint m_fbo, m_dbo;
	TexturePtr m_screen[COLORBUFCOUNT];

	SamplerPtr m_sampler;
	MaterialPtr m_planeMat;

	GLuint m_fboMSAA;
	GLuint m_colorMSAA[COLORBUFCOUNT];
	GLuint m_depthMSAA;
};

#endif