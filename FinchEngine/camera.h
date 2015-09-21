#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <QVector3D>
#include <QSharedPointer>

class Camera;

class CameraProperty
{
public:
	enum ProjType{
		ORTHO,
		PERSP
	};

	enum TransType{
		PAN,
		MOVE
	};

	CameraProperty(ProjType type, int s_width, int s_height):m_projType(type),
		m_position(0.0f, 0.0f, 6.0f), m_up(0.0f, 1.0f, 0.0f),
		m_target(0.0f, 0.0f, 0.0f), m_dir(0.0f, 0.0f, -1.0f), m_scale(2),
		m_near(0.1f), m_far(1000.0f), m_fov(60),
		m_bot(-m_scale), m_top(m_scale){
		m_aspect = (float)s_width / s_height;
		m_left = -m_aspect*m_scale;
		m_right = m_aspect*m_scale;
		updateMetric();
	}

	void updateMetric(){
		updateProj();
		updateView();
		updateVProj();
	}

	void updatePerspProj(){
		m_projMat.setToIdentity();
		m_projMat.perspective(m_fov, m_aspect, m_near, m_far);
	}

	void updateOrthoProj(){
		m_projMat.setToIdentity();
		m_projMat.ortho(m_left, m_right, m_bot, m_top, m_near, m_far);
	}

	void updateView(){
		m_viewMat.setToIdentity();
		m_viewMat.lookAt(m_position, m_target, m_up);
	}
	 
	void updateVProj(){
		m_view_projMat = m_projMat*m_viewMat;
	}

	void updateProj(){
		if (m_projType == ProjType::PERSP)
		{
			updatePerspProj();
		}
		else{
			updateOrthoProj();
		}
	}

	void updateAspect(int s_width, int s_height){
		m_aspect = (float)s_width / s_height;
		m_left = -m_aspect*m_scale;
		m_right = m_aspect*m_scale;

		updateProj();
		updateVProj();
	}

	float m_scale;

	QVector3D m_position;
	QVector3D m_up;
	QVector3D m_target;
	QVector3D m_dir;

	ProjType m_projType;

	float m_near;
	float m_far;

	float m_fov;
	float m_aspect;

	float m_left;
	float m_right;
	float m_bot;
	float m_top;

	QMatrix4x4 m_viewMat;
	QMatrix4x4 m_projMat;
	QMatrix4x4 m_view_projMat;
};


class Camera
{
public:
	Camera(int s_width, int s_height, CameraProperty::ProjType type=CameraProperty::ProjType::ORTHO);
	~Camera();

	QVector3D& position(){
		return cp->m_position;
	}

	QVector3D& target(){
		return cp->m_target;
	}

	QVector3D& viewdir(){
		return cp->m_dir;
	}

	QMatrix4x4& projMat(){
		return cp->m_projMat;
	}

	QMatrix4x4& viewMat(){
		return cp->m_viewMat;
	}

	QMatrix4x4& viewProjMat(){
		return cp->m_view_projMat;
	}

	void updateAspect(int s_width, int s_height){
		cp->updateAspect(s_width, s_height);
	}

	void translate(const QVector3D& mv_local, CameraProperty::TransType mv_type=CameraProperty::PAN);
	void setPanning(bool ispanning){ m_panning = ispanning; }
	bool isPanning(){ return m_panning; }

	CameraProperty::ProjType& projType(){ return cp->m_projType; }

	void deltaScale(float delta){ cp->m_scale += delta; cp->m_bot = -cp->m_scale; 
	cp->m_top = cp->m_scale; cp->m_right = cp->m_scale*cp->m_aspect; cp->m_left = -cp->m_right;
	cp->updateProj(); cp->updateVProj();
	}

	void rotate(float yrot, float xrot);

	void pitch(const float angle);

	void yaw(const float angle);

	void roll(const float angle);
private:
	QQuaternion rotateAngleAxis(const float angle, const QVector3D& axis);

	QVector3D m_oright;
	QVector3D m_odir;
	QVector3D m_oup;

	CameraProperty* cp;

	bool m_panning;
};

typedef QSharedPointer<Camera> CameraPtr;

#endif