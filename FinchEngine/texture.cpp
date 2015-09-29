#include "Texture.h"
#include <QGLWidget>

Texture::~Texture()
{
	destory();
}

void Texture::create()
{
	m_funcs->glGenTextures(1, &m_textureId);
}

void Texture::destory()
{
	if (m_textureId)
	{
		m_funcs->glDeleteTextures(1, &m_textureId);
		m_textureId = 0;
	}
}

void Texture::bind()
{
	m_funcs->glBindTexture(m_type, m_textureId);
}

void Texture::release()
{
	m_funcs->glBindTexture(m_type, 0);
}

void Texture::initializeBySize(const QSize& size, bool isdepth, int layer){
	if (isdepth)
	{
		m_funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.width(), size.height(), 0, GL_RGBA, GL_FLOAT, 0);
	}
	else if (m_type == Texture3D){
		m_funcs->glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, size.width(), size.height(), layer, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	else{
		m_funcs->glTexImage2D(m_type, 0, GL_RGBA, size.width(), size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
}

void Texture::resize(int width, int height){
	m_funcs->glTexImage2D(m_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

void Texture::initializeWithImage(const QImage& img){
	QImage glImg = QGLWidget::convertToGLFormat(img);
	m_funcs->glTexImage2D(m_type, 0, GL_RGBA, glImg.width(), glImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImg.bits());
}

void Texture::genMipMaps(){
	m_funcs->glGenerateMipmap(m_type);
}