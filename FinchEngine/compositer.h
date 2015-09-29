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

	void preRender();
	void postRender(GLint defbuf);
	void resize(int width, int height);
private:
	QOpenGLFunctions_4_3_Core* m_func;
	int m_swidth, m_sheight;

	GLuint m_vao;
	GLuint m_fbo;
	TexturePtr m_screen[COLORBUFCOUNT];

	SamplerPtr m_sampler;
	MaterialPtr m_planeMat;

	GLuint m_fboMSAA;
	GLuint m_colorMSAA[COLORBUFCOUNT];
	GLuint m_depthMSAA;

	QVector<QVector<GLfloat>> m_clearCols;
};

#endif