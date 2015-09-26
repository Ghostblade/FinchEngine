#include "compositer.h"


Compositer::Compositer(QOpenGLFunctions_4_3_Core* func, int swidth, int sheight) :m_func(func)
{
	m_func->glGenFramebuffers(1, &m_fboMSAA);
	m_func->glBindFramebuffer(GL_FRAMEBUFFER, m_fboMSAA);

	for (int i = 0; i < COLORBUFCOUNT; ++i){
		m_func->glGenRenderbuffers(1, &m_colorMSAA[i]);
		m_func->glBindRenderbuffer(GL_RENDERBUFFER, m_colorMSAA[i]);
		m_func->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, swidth, sheight);
		m_func->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, m_colorMSAA[i]);
	}


	m_func->glGenRenderbuffers(1, &m_depthMSAA);
	m_func->glBindRenderbuffer(GL_RENDERBUFFER, m_depthMSAA);
	m_func->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, swidth, sheight);
	m_func->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthMSAA);
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
	m_func->glGenRenderbuffers(1, &m_dbo);

	// 	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	// 	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferObject);
	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferObject);

	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
	// 	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); // Make sure your multisampled FBO is the read framebuffer
	// 	glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
	// 	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// 	GLuint colorTex, depthTex, fbo;
	// 	// create a RGBA color texture
	// 	glGenTextures(1, colorTex);
	// 	glBindTexture(GL_TEXTURE_2D, colorTex);
	// 	glTexImage2D(, GL_TEXTURE_2D, 0, GL_RGBA,
	// 		w, h,
	// 		0, GL_RGBA, GL_UNSIGNED_BYTE,
	// 		NULL);
	// 
	// 	// create a depth texture
	// 	glGenTextures(1, depthTex);
	// 	glBindTexture(GL_TEXTURE_2D, depthTex);
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	// 		w, h,
	// 		0, GL_DEPTH_COMPONENT, GL_FLOAT,
	// 		NULL);
	// 
	// 	// create the framebuffer object
	// 	glGenFramebuffers(1, &fbo);
	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	// 
	// 	// attach color
	// 	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex, 0);
	// 	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);

	// 	GLuint colorTex[n], depthTex, fbo;
	// 	// create n RGBA color textures
	// 	glGenTextures(n, colorTex);
	// 
	// 	for (int i = 0; i < n; ++i) {
	// 		glBindTexture(GL_TEXTURE_2D, colorTex[i]);
	// 		glTexImage2D(...);
	// 	}
	// 
	// 	// create a depth texture
	// 	glGenTextures(1, depthTex);
	// 	glBindTexture(GL_TEXTURE_2D, depthTex);
	// 	glTexImage2D(...);
	// 
	// 	// create the framebuffer object
	// 	glGenFramebuffers(1, &fbo);
	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	// 
	// 	// attach colors
	// 	for (int i = 0; i < n; ++i) {
	// 		glFramebufferTexture(GL_DRAW_FRAMEBUFFER,
	// 			GL_COLOR_ATTACHMENT0 + i, colorTex[i], 0);
	// 	}
	// 	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);

	// check if everything is OK
	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	// 	GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	// 	if (e != GL_FRAMEBUFFER_COMPLETE)
	// 		printf("There is a problem with the FBO\n");

	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	// 
	// 	// define the index array for the outputs
	// 	GLuint attachments[2] = { GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	// 	glDrawBuffers(2, attachments);

	// bind the source framebuffer and select a color attachment to copy from
	// 	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboS);
	// 	glReadBuffers(GL_COLOR_ATTACHMENT2);
	// 
	// 	// bind the destination framebuffer and select the color attachments to copy to
	// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboD);
	// 	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	// 	glDrawBuffers(2, attachments);
	// 
	// 	// copy
	// 	glBlitFramebuffer(0, 0, 1024, 1024, 0, 0, 512, 512, GL_COLOR_BUFFER_BIT, GL_LINEAR);


	// 	void printFramebufferLimits() {
	// 
	// 		int res;
	// 		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
	// 		printf("Max Color Attachments: %d\n", res);
	// 
	// 		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
	// 		printf("Max Framebuffer Width: %d\n", res);
	// 
	// 		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
	// 		printf("Max Framebuffer Height: %d\n", res);
	// 
	// 		glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
	// 		printf("Max Framebuffer Samples: %d\n", res);
	// 
	// 		glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
	// 		printf("Max Framebuffer Layers: %d\n", res);
	// 
	// 	}

	// int res, i = 0;
	// GLint buffer;
	// 
	// glBindFramebuffer(target, fbo);
	// 
	// do {
	// 	glGetIntegerv(GL_DRAW_BUFFER0 + i, &buffer);
	// 
	// 	if (buffer != GL_NONE) {
	// 
	// 		printf("Shader Output Location %d - color attachment %d\n",
	// 			i, buffer - GL_COLOR_ATTACHMENT0);
	// 
	// 		glGetFramebufferAttachmentParameteriv(target, buffer,
	// 			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
	// 		printf("\tAttachment Type: %s\n",
	// 			res == GL_TEXTURE ? "Texture" : "Render Buffer");
	// 		glGetFramebufferAttachmentParameteriv(target, buffer,
	// 			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
	// 		printf("\tAttachment object name: %d\n", res);
	// 	}
	// 	++i;
	// 
	// } while (buffer != GL_NONE);
	// }

	//http://manpages.ubuntu.com/manpages/saucy/man3/glClearBuffer.3G.html
}


Compositer::~Compositer()
{
	m_func->glDeleteFramebuffers(1, &m_fboMSAA);
	m_func->glDeleteRenderbuffers(1, &m_depthMSAA);
	for (int i = 0; i < COLORBUFCOUNT; ++i)
	{
		m_func->glDeleteRenderbuffers(1, &m_colorMSAA[i]);
	}
	m_sampler.clear();
	m_planeMat.clear();
	for (int i = 0; i < COLORBUFCOUNT; ++i)
	{
		m_screen[i].clear();
	}

	m_func->glDeleteFramebuffers(1, &m_fbo);
	m_func->glDeleteVertexArrays(1, &m_vao);
	m_func->glDeleteRenderbuffers(1, &m_dbo);
}