#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_4_3_Core>
#include <QSharedPointer>
#include <QWidget>


class Texture
{
public:
	enum TextureType{
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_2D_ARRAY,
		TextureCubeMap = GL_TEXTURE_CUBE_MAP
	};
	Texture(QOpenGLFunctions_4_3_Core* funcs, TextureType type = Texture2D) :m_type(type), m_funcs(funcs), m_textureId(0){}
	~Texture();

	TextureType type() const{ return m_type; }
	GLuint textureID() const { return m_textureId; }


	void create();
	void destory();

	void bind();
	void release();

	void initializeBySize(const QSize& size, bool isdepth = false, int layer = 0);
	void initializeWithImage(const QImage& img);
	void genMipMaps();

	void resize(int width, int height);

private:
	TextureType m_type;
	GLuint m_textureId;
	QOpenGLFunctions_4_3_Core* m_funcs;
};

typedef QSharedPointer<Texture> TexturePtr;

#endif

