#include "camera.h"

Camera::Camera(int s_width, int s_height, CameraProperty::ProjType type):m_odir(0,0,-1), m_oright(1,0,0), m_oup(0,1,0), m_panning(false){
	cp = new CameraProperty(type, s_width, s_height);
}

Camera::~Camera(){
	delete cp;
}

void Camera::translate(const QVector3D& mv_local, CameraProperty::TransType mv_type){
	QVector3D mv_world(0, 0, 0);
	if (!qFuzzyIsNull(mv_local.x()))
	{
		QVector3D cam_right = QVector3D::crossProduct(cp->m_dir, cp->m_up).normalized();
		mv_world += mv_local.x()*cam_right;
	}
	if (!qFuzzyIsNull(mv_local.y()))
	{
		mv_world += mv_local.y()*cp->m_up;
	}
	if (!qFuzzyIsNull(mv_local.z()))
	{
		mv_world += mv_local.z()*cp->m_dir;
	}

	cp->m_position += mv_world;

	if (mv_type==CameraProperty::TransType::PAN)
	{
		cp->m_target += mv_world;
	}
	else{
		cp->m_dir = (cp->m_target - cp->m_position).normalized();
		QVector3D cam_right = QVector3D::crossProduct(cp->m_dir, cp->m_up).normalized();
		cp->m_up = QVector3D::crossProduct(cam_right, cp->m_dir).normalized();
	}

	cp->updateView();
	cp->updateVProj();
}

QQuaternion Camera::rotateAngleAxis(const float angle, const QVector3D& axis){
	return QQuaternion::fromAxisAndAngle(axis, angle);
}

void Camera::rotate(float yrot, float xrot){
	QMatrix4x4 cameraTransformation;
	cameraTransformation.rotate(yrot, 0, 1, 0);
	cameraTransformation.rotate(xrot, 1, 0, 0);

	cp->m_dir = cameraTransformation*QVector3D(0, 0, -1);
	cp->m_up = cameraTransformation*QVector3D(0, 1, 0);

	float dis = (cp->m_target - cp->m_position).length();
	cp->m_position = cp->m_target - dis*cp->m_dir;

	cp->updateView();
	cp->updateVProj();
}

void Camera::pitch(const float angle){
// 	QVector3D cam_right = QVector3D::crossProduct(cp->m_dir, cp->m_up);
	QQuaternion q = rotateAngleAxis(angle, m_oright);
	QQuaternion qi = rotateAngleAxis(-angle, m_oright);
	
	m_odir = qi.rotatedVector(m_odir);
	cp->m_dir = q.rotatedVector(cp->m_dir);
	m_oup = qi.rotatedVector(m_oup);
	cp->m_up = q.rotatedVector(cp->m_up);

	float dis = (cp->m_target - cp->m_position).length();
	cp->m_position = cp->m_target - dis*cp->m_dir;

	cp->updateView();
	cp->updateVProj();

}

void Camera::yaw(const float angle){
	QQuaternion q = rotateAngleAxis(angle, m_oup);
	QQuaternion qi = rotateAngleAxis(-angle, m_oup);

	m_odir = qi.rotatedVector(m_odir);
	cp->m_dir = q.rotatedVector(cp->m_dir);
	
	float dis = (cp->m_target - cp->m_position).length();
	cp->m_position = cp->m_target - dis*cp->m_dir;

	cp->updateView();
	cp->updateVProj();
}

void Camera::roll(const float angle){
	QQuaternion q = rotateAngleAxis(angle, QVector3D(0,0,-1));
	cp->m_up = q.rotatedVector(cp->m_dir);

	float dis = (cp->m_target - cp->m_position).length();
	cp->m_position = cp->m_target - dis*cp->m_dir;

	cp->updateView();
	cp->updateVProj();
}

