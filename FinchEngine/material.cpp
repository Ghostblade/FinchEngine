#include "Material.h"

void Material::bind()
{
	m_shader->bind();
	for each (const GLuint unit in m_unitConfigs.keys())
	{
		const TextureUnitConfiguration& config = m_unitConfigs.value(unit);
		m_funcs->glActiveTexture(GL_TEXTURE0 + unit);
		config.texture()->bind();

		config.sampler()->bind(unit);
		if (m_samplerUniforms.contains(unit))
		{
			m_shader->setUniformValue(m_samplerUniforms.value(unit).constData(), unit);
		}
	}
}

void Material::release(){
	m_shader->release();
}


void Material::setShader(const ShaderProgramPtr& shader){
	m_shader = shader;
}

void Material::setShader(const QString& vertexShader,
						const QString& tessellationControlShader,
						const QString& tessellationEvaluationShader,
						const QString& geometryShader,
						const QString& fragmentShader){
	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader))
		qCritical() << QObject::tr("Could not compile vertex shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationControl, tessellationControlShader))
		qCritical() << QObject::tr("Could not compile tessellation control shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, tessellationEvaluationShader))
		qCritical() << QObject::tr("Could not compile tessellation evaluation shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryShader))
		qCritical() << QObject::tr("Could not compile geometry shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader))
		qCritical() << QObject::tr("Could not compile fragment shader. Log:") << m_shader->log();

	if (!m_shader->link())
		qCritical() << QObject::tr("Could not link shader program. Log:") << m_shader->log();
}

void Material::setShader(const QString& vertexShader,
						const QString& fragmentShader)
{
	// Create a shader program
	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader))
		qCritical() << QObject::tr("Could not compile vertex shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader))
		qCritical() << QObject::tr("Could not compile fragment shader. Log:") << m_shader->log();

	if (!m_shader->link())
		qCritical() << QObject::tr("Could not link shader program. Log:") << m_shader->log();
}

void Material::setShader(const QString& vertexShader,
						const QString& geometryShader,
						const QString& fragmentShader)
{
	// Create a shader program
	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader))
		qCritical() << QObject::tr("Could not compile vertex shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryShader))
		qCritical() << QObject::tr("Could not compile geometry shader. Log:") << m_shader->log();

	if (!m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader))
		qCritical() << QObject::tr("Could not compile fragment shader. Log:") << m_shader->log();

	if (!m_shader->link())
		qCritical() << QObject::tr("Could not link shader program. Log:") << m_shader->log();
}

void Material::setTextureUnitConfiguration(GLuint unit, 
	TexturePtr texture, 
	SamplerPtr sampler)
{
	TextureUnitConfiguration config(texture, sampler);
	m_unitConfigs.insert(unit, config);
}

void Material::setTextureUnitConfiguration(GLuint unit, 
	TexturePtr texture, 
	SamplerPtr sampler, 
	const QByteArray& uniformName)
{
	setTextureUnitConfiguration(unit, texture, sampler);
	m_samplerUniforms.insert(unit, uniformName);
}

TextureUnitConfiguration Material::getTexConfiguration(GLuint unit) const
{
	return m_unitConfigs.value(unit, TextureUnitConfiguration());
}