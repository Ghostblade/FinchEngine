#ifndef SAMPLER_H
#define SAMPLER_H

#include <qopengl.h>
#include <QSharedPointer>
#include <QOpenGLFunctions_4_3_Core>


class Sampler
{
public:
	Sampler(QOpenGLFunctions_4_3_Core* funcs) :m_funcs(funcs){};
	~Sampler();
	GLuint samplerId() const{ return m_samplerId; }


	void create();
	void destory();
	void bind(GLuint unit);
	void release(GLuint unit);

	enum CoordDir{
		S = GL_TEXTURE_WRAP_S,
		T = GL_TEXTURE_WRAP_T,
		R = GL_TEXTURE_WRAP_R
	};

	void setWrapMode(CoordDir dir, GLenum mode);
	void setMinFilter(GLenum filter);
	void setMaxFilter(GLenum filter);
private:
	GLuint m_samplerId;
	QOpenGLFunctions_4_3_Core* m_funcs;
};

typedef QSharedPointer<Sampler> SamplerPtr;


#endif


