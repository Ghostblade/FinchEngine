#include "Sampler.h"
#include <QOpenGLContext>
Sampler::~Sampler()
{
	destory();
}

void Sampler::create()
{
	m_funcs->glGenSamplers(1, &m_samplerId);
}

void Sampler::destory()
{
	if (m_samplerId)
	{
		m_funcs->glDeleteSamplers(1, &m_samplerId);
		m_samplerId = 0;
	}
}

void Sampler::bind(GLuint uint)
{
	m_funcs->glBindSampler(uint, m_samplerId);
}

void Sampler::release(GLuint uint)
{
	m_funcs->glBindSampler(uint, 0);
}

void Sampler::setWrapMode(CoordDir dir, GLenum mode){
	m_funcs->glSamplerParameteri(m_samplerId, dir, mode);
}

void Sampler::setMinFilter(GLenum filter){
	m_funcs->glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, filter);
}

void Sampler::setMaxFilter(GLenum filter){
	m_funcs->glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, filter);
}

void Sampler::setCompareFunc(GLenum func){
	m_funcs->glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_FUNC, func);
}

void Sampler::setCompareMode(GLenum mode){
	m_funcs->glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_MODE, mode);
}
