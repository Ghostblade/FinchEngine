
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QFile>
#include <QImage>

#include "model.h"

#include <assert.h>

Model::Model(QOpenGLFunctions_4_3_Core* funcs)
: m_posBuf(QOpenGLBuffer::VertexBuffer), m_normBuf(QOpenGLBuffer::VertexBuffer), m_coordBuf(QOpenGLBuffer::VertexBuffer), m_idxBuf(QOpenGLBuffer::IndexBuffer), m_funcs(funcs)
{
}

Model::~Model()
{
	m_posBuf.destroy();
	m_normBuf.destroy();
	m_coordBuf.destroy();
	m_idxBuf.destroy();
	m_vao.destroy();
	
	m_cam.clear();
	m_mat.clear();
}

void Model::setMaterial(MaterialPtr& mat){
	m_mat = mat;
	m_vao.create();
	m_vao.bind();
	m_mat->shader()->bind();

	m_posBuf.bind();
	m_mat->shader()->enableAttributeArray("position");
	m_mat->shader()->setAttributeBuffer("position", GL_FLOAT, 0, 3);
	m_posBuf.release();

	m_normBuf.bind();
	m_mat->shader()->enableAttributeArray("normal");
	m_mat->shader()->setAttributeBuffer("normal", GL_FLOAT, 0, 3);
	m_normBuf.release();

	m_coordBuf.bind();
	m_mat->shader()->enableAttributeArray("texcoord");
	m_mat->shader()->setAttributeBuffer("texcoord", GL_FLOAT, 0, 2);
	m_normBuf.release();

	m_mat->shader()->release();

	m_idxBuf.bind();
	m_vao.release();
	m_idxBuf.release();
}

void Model::initWithName(const char* filename)
{
	TriShape tshape;
	tshape.request_vertex_texcoords2D();

	OpenMesh::IO::Options opt;
	opt += OpenMesh::IO::Options::VertexTexCoord;
	OpenMesh::IO::read_mesh(tshape, filename, opt);
	tshape.request_face_normals();
	tshape.update_face_normals();
	tshape.request_vertex_normals();
	tshape.update_vertex_normals();

	for (TriShape::FaceIter iter = tshape.faces_begin(); iter != tshape.faces_end(); ++iter)
	{
		TriShape::FaceVertexIter viter = tshape.fv_begin(*iter);
		m_indices << (viter++)->idx();
		m_indices << (viter++)->idx();
		m_indices << viter->idx();
	}

	m_idxBuf.create();
	m_idxBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_idxBuf.bind();
	m_idxBuf.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));

	m_posBuf.create();
	m_posBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_posBuf.bind();
	m_posBuf.allocate(tshape.points(), tshape.n_vertices()*3 * sizeof(float));

	m_normBuf.create();
	m_normBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_normBuf.bind();
	m_normBuf.allocate(tshape.vertex_normals(), tshape.n_vertices()*3 * sizeof(float));

	m_coordBuf.create();
	m_coordBuf.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_coordBuf.bind();
	m_coordBuf.allocate(tshape.texcoords2D(), tshape.n_vertices()*2 * sizeof(float));
}


void Model::render()
{
	m_mat->bind();
	m_vao.bind();
 
    QMatrix4x4 m;
    m.setToIdentity();
	m_mat->shader()->setUniformValue("mvp", m_cam->viewProjMat() * m);

    m_funcs->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    m_vao.release();
	m_mat->release();
}


