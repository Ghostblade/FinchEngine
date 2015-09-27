
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLFunctions_4_3_Core>
#include <QWheelEvent>

#include "model.h"
#include "clmanager.h"
#include "compositer.h"

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	QSize sizeHint() const Q_DECL_OVERRIDE;


	void cleanup();


protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
private:
	float m_yaw;
	float m_pitch;

	GLint m_defaultFB;

	QPoint m_lastPos;

	QOpenGLFunctions_4_3_Core* m_func;

	Model* cube;
	MaterialPtr m_phong;
	CameraPtr m_mainCam;

	Compositer* m_comp;

	int m_swidth, m_sheight;
};

#endif
