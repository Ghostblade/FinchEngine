#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

GLWidget::GLWidget(QWidget *parent)
: QOpenGLWidget(parent), m_yaw(0), m_pitch(0), m_defaultFB(-1), m_swidth(1024), m_sheight(768)
{
}

GLWidget::~GLWidget()
{
	cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(400, 400);
}



void GLWidget::cleanup()
{
	makeCurrent();

	m_phong.clear();
	m_mainCam.clear();
	delete cube;
	delete m_comp;
	doneCurrent();
}

void GLWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
	m_func = context()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	m_func->initializeOpenGLFunctions();

	CLManager::getSingletonPtr()->setSource("bsort8.cl");

	m_mainCam = CameraPtr(new Camera(m_swidth, m_sheight));
	m_comp = new Compositer(m_func, m_swidth, m_sheight);

	m_func->glClearColor(0, 0, 0, 1);

	m_phong = MaterialPtr(new Material(m_func));
	m_phong->setShader("res/shaders/shader.vert", "res/shaders/shader.frag");
	m_phong->shader()->setUniformValue("lightPos", QVector3D(0, 0, 70));

	cube = new Model(m_func);
	cube->initWithName("res/models/toonface.obj");
	cube->setMaterial(m_phong);

	cube->setCamera(m_mainCam);
	QVector3D cam_trans(0,1,0);
	m_mainCam->translate(cam_trans);

	SamplerPtr sampler(new Sampler(m_func));
	sampler->create();
	sampler->setMinFilter(GL_LINEAR);
	sampler->setMaxFilter(GL_LINEAR);
	sampler->setWrapMode(Sampler::S, GL_CLAMP_TO_EDGE);
	sampler->setWrapMode(Sampler::T, GL_CLAMP_TO_EDGE);

	QImage diffuseImg("res/textures/face.dds");
	m_func->glActiveTexture(GL_TEXTURE0);

	TexturePtr diffuseMap(new Texture(m_func));
	diffuseMap->create();
	diffuseMap->bind();
	diffuseMap->initializeWithImage(diffuseImg);
	m_phong->setTextureUnitConfiguration(0, diffuseMap, sampler, QByteArrayLiteral("textureSampler"));

}

void GLWidget::paintGL()
{
	if (m_defaultFB<0)
	{
		m_func->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_defaultFB);
		if (m_defaultFB!=0)
		{
			printf("--#attention! default fbo is %d\n", m_defaultFB);
		}
	}

	m_func->glEnable(GL_DEPTH_TEST);
	m_func->glEnable(GL_CULL_FACE);

	m_comp->preRender();
	cube->render();

	m_comp->postRender(m_defaultFB);
}

void GLWidget::resizeGL(int w, int h)
{
	m_mainCam->updateAspect(w, h);
	m_swidth = w;
	m_sheight = h;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons()&Qt::MidButton)
	{
		m_mainCam->setPanning(true);
	}
	m_lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_mainCam->setPanning(false);
}

void GLWidget::wheelEvent(QWheelEvent *event){
	int delta = event->delta();
	if (event->orientation()==Qt::Vertical)
	{
		if (m_mainCam->projType() == CameraProperty::PERSP)
		{
			if (delta<0)
			{
				m_mainCam->translate(QVector3D(0,0,-1)*0.02);
			}
			else{
				m_mainCam->translate(QVector3D(0, 0, 1)*0.02);
			}
		}
		else{
			m_mainCam->deltaScale(-delta*0.001);
		}
	}
	update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		m_yaw -= dx*0.2;
		m_pitch -= dy*0.2;
		if (m_yaw<0)
		{
			m_yaw += 360;
		}else if (m_yaw>=360)
		{
			m_yaw -= 360;
		}

		if (m_pitch<-90)
		{
			m_pitch = -90;
		}else if (m_pitch>90)
		{
			m_pitch = 90;
		}

		m_mainCam->rotate(m_yaw, m_pitch);
		update();
	}
	else if (event->buttons() & Qt::MidButton)
	{
		QVector3D localtrans = dx*QVector3D(-1, 0, 0) + dy*QVector3D(0,1,0);
		localtrans *= 0.005;
		m_mainCam->translate(localtrans);
		update();
	}
	m_lastPos = event->pos();
}