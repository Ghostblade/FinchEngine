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
#include <string>
#include <fstream>
#include <algorithm>

GLuint Material::LoadShaders(const char * vertex_file_path, const char * fragment_file_path){
	// Create the shaders
	GLuint VertexShaderID = m_funcs->glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = m_funcs->glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	m_funcs->glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	m_funcs->glCompileShader(VertexShaderID);

	// Check Vertex Shader
	m_funcs->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	m_funcs->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		m_funcs->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	m_funcs->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	m_funcs->glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	m_funcs->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	m_funcs->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		m_funcs->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = m_funcs->glCreateProgram();
	m_funcs->glAttachShader(ProgramID, VertexShaderID);
	m_funcs->glAttachShader(ProgramID, FragmentShaderID);
	m_funcs->glLinkProgram(ProgramID);

	// Check the program
	m_funcs->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	m_funcs->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		m_funcs->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	m_funcs->glDeleteShader(VertexShaderID);
	m_funcs->glDeleteShader(FragmentShaderID);

	return ProgramID;
}