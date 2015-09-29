#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "sampler.h"
#include <QOpenGLShaderProgram>
#include <QSharedPointer>
#include <QMap>
#include <QPair>

typedef QSharedPointer<QOpenGLShaderProgram> ShaderProgramPtr;

class TextureUnitConfiguration :public QPair<TexturePtr, SamplerPtr>
{
public:
	TextureUnitConfiguration() :QPair<TexturePtr, SamplerPtr>(TexturePtr(), SamplerPtr())
	{

	}
	explicit TextureUnitConfiguration(const TexturePtr& texture, const SamplerPtr& sampler)
		:QPair<TexturePtr, SamplerPtr>(texture, sampler)
	{
	}

	void setTexture(const TexturePtr& texture){ first = texture; }
	TexturePtr texture() const { return first; }

	void setSampler(const SamplerPtr& sampler){ second = sampler; }
	SamplerPtr sampler() const { return second; }

	~TextureUnitConfiguration(){ first.clear(); second.clear(); }
};

class Material
{
public:
	Material(QOpenGLFunctions_4_3_Core* funcs) :m_funcs(funcs), m_shader(new QOpenGLShaderProgram){}
	~Material(){ release(); m_shader.clear(); }

	void bind();
	void release();
	void setShader(const QString& v_src, const QString& f_src);
	void setShader(const QString& v_src, const QString& g_src, const QString& f_src);
	void setShader(const QString& v_src, const QString& t_c_src, const QString& t_e_src, const QString& g_src, const QString& f_src);

	void setShader(const ShaderProgramPtr& shader);
	ShaderProgramPtr& shader()  {
		return m_shader;
	}

	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	void setTextureUnitConfiguration(GLuint unit, TexturePtr texture, SamplerPtr sampler);
	void setTextureUnitConfiguration(GLuint unit, TexturePtr texture, SamplerPtr sampler, const QByteArray& uniformName);

	TextureUnitConfiguration getTexConfiguration(GLuint unit) const;
private:
	ShaderProgramPtr m_shader;
	QMap<GLuint, TextureUnitConfiguration> m_unitConfigs;
	QMap<GLuint, QByteArray> m_samplerUniforms;

	QOpenGLFunctions_4_3_Core* m_funcs;
};

typedef QSharedPointer<Material> MaterialPtr;

#endif


