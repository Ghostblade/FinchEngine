#include "compositer.h"


Compositer::Compositer(QOpenGLFunctions_4_3_Core* func, int swidth, int sheight) :m_func(func), m_swidth(swidth), m_sheight(sheight)
{
	m_func->glGenFramebuffers(1, &m_fboMSAA);
	m_func->glBindFramebuffer(GL_FRAMEBUFFER, m_fboMSAA);

	m_func->glGenTextures(COLORBUFCOUNT, m_colorMSAA);
	for (int i = 0; i < COLORBUFCOUNT;++i)
	{
		m_func->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorMSAA[i]);
		m_func->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, swidth, sheight, false);
		m_func->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, m_colorMSAA[i], 0);
	}

	m_func->glGenTextures(1, &m_depthMSAA);
	m_func->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_depthMSAA);
	m_func->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT24, swidth, sheight, false);
	m_func->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_depthMSAA, 0);

	GLuint test = m_func->glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (test != GL_FRAMEBUFFER_COMPLETE)
	{
		perror("--OpenGL--Failed to create multi-sampled render target\n");
		return;
	}

	m_sampler = SamplerPtr(new Sampler(m_func));
	m_sampler->create();
	m_sampler->setMinFilter(GL_NEAREST);
	m_sampler->setMaxFilter(GL_NEAREST);
	m_sampler->setWrapMode(Sampler::S, GL_CLAMP_TO_EDGE);
	m_sampler->setWrapMode(Sampler::T, GL_CLAMP_TO_EDGE);

	m_planeMat = MaterialPtr(new Material(m_func));
	m_planeMat->setShader("res/shaders/plane.vert", "res/shaders/plane.frag");

	std::string s[2] = { "rtt0", "rtt1" };
	for (int i = 0; i < COLORBUFCOUNT; ++i)
	{
		m_screen[i] = TexturePtr(new Texture(m_func));
		m_screen[i]->create();
		m_screen[i]->bind();
		m_screen[i]->initializeBySize(QSize(swidth, sheight));
		m_planeMat->setTextureUnitConfiguration(i, m_screen[i], m_sampler, s[i].c_str());
	}

	m_func->glGenFramebuffers(1, &m_fbo);
	m_func->glGenVertexArrays(1, &m_vao);

	m_func->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	for (int i = 0; i < COLORBUFCOUNT;++i)
	{
		m_func->glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_screen[i]->textureID(), 0);
	}

	for (int i = 0; i < COLORBUFCOUNT;++i)
	{
		QVector<float> clearcol;
		clearcol << 0 << 0 << 0 << 0;
		m_clearCols << clearcol; 
	}
}

void Compositer::preRender(){
	m_func->glBindFramebuffer(GL_FRAMEBUFFER, m_fboMSAA);
	QVector<GLenum> color_bufs;
	for (int i = 0; i < COLORBUFCOUNT;++i)
	{
		color_bufs << GL_COLOR_ATTACHMENT0 + i;
	}

	m_func->glDrawBuffers(2, color_bufs.constData());
	m_func->glClearColor(0, 0, 0, 0);
	m_func->glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < COLORBUFCOUNT; ++i)
	{
		m_func->glClearBufferfv(GL_COLOR, i, m_clearCols[i].constData());
	}

	m_func->glViewport(0, 0, m_swidth, m_sheight);

}

void Compositer::postRender(GLint defbuf){
	for (int i = 0; i < COLORBUFCOUNT;++i){
		m_func->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboMSAA);
		m_func->glReadBuffer(GL_COLOR_ATTACHMENT0+i);
		m_func->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
		m_func->glDrawBuffer(GL_COLOR_ATTACHMENT0+i);
		m_func->glBlitFramebuffer(0, 0, m_swidth, m_sheight, 0, 0, m_swidth, m_sheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	m_func->glBindFramebuffer(GL_FRAMEBUFFER, defbuf);
	m_func->glClearColor(0, 0, 0, 0);
	m_func->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_func->glViewport(0, 0, m_swidth, m_sheight);

	m_planeMat->bind();
	m_func->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_planeMat->release();
}

void Compositer::resize(int width, int height){
	m_swidth = width;
	m_sheight = height;

	for (int i = 0; i < COLORBUFCOUNT;++i)
	{
		m_func->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorMSAA[i]);
		m_func->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, m_swidth, m_sheight, false);

		m_screen[i]->bind();
		m_screen[i]->resize(width, height);
	}
}

Compositer::~Compositer()
{
	m_func->glDeleteFramebuffers(1, &m_fboMSAA);

	m_func->glDeleteTextures(COLORBUFCOUNT, m_colorMSAA);
	m_func->glDeleteTextures(1, &m_depthMSAA);

	m_sampler.clear();
	m_planeMat.clear();
	for (int i = 0; i < COLORBUFCOUNT; ++i)
	{
		m_screen[i].clear();
	}

	m_func->glDeleteFramebuffers(1, &m_fbo);
	m_func->glDeleteVertexArrays(1, &m_vao);
}