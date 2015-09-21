#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Core>
#include "material.h"
#include "camera.h"

#define  _USE_MATH_DEFINES

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/Property.hh>

struct CurTraits : public OpenMesh::DefaultTraits
{
	VertexAttributes(OpenMesh::Attributes::Status);
	FaceAttributes(OpenMesh::Attributes::Status);
	EdgeAttributes(OpenMesh::Attributes::Status);
};

typedef OpenMesh::TriMesh_ArrayKernelT<CurTraits> TriShape;


class Model
{
public:
    Model(QOpenGLFunctions_4_3_Core* funcs);
    ~Model();
    void initWithName(const char* filename);
    void render();
	void setCamera(CameraPtr& cam){ m_cam = cam; }
	void setMaterial(MaterialPtr& mat);

private:
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_posBuf;
	QOpenGLBuffer m_normBuf;
	QOpenGLBuffer m_coordBuf;

	QOpenGLBuffer m_idxBuf;
	QVector<unsigned int> m_indices;
	QOpenGLFunctions_4_3_Core* m_funcs;
	MaterialPtr m_mat;
	CameraPtr m_cam;


};

#endif // CUBE_H
