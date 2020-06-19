#include <glew/glew.h>

#include "Gizmo.h"
#include "Scene.h"

#include "../Geometry/Geometry.h"
#include "../Geometry/GeometryHelper.h"
#include "../Material/Material.h"
#include "../Material/ShaderHelper.h"

Gizmo::Gizmo()
{
	glCreateVertexArrays(3, m_vao);

	m_axeX = GeometryHelper::CreateCylinder(Metre(0.1f), Metre(0.1f), Metre(1), 10, 2);
	m_axeX->transform(Transform::MakeTranslation(Vector3<Metre>(Metre(), Metre(0.5f), Metre())));
	m_axeX->transform(Transform::MakeRotationZ(-Degree(90)));
	m_axeX->setColor(Color::Red());

	m_axeY = GeometryHelper::CreateCylinder(Metre(0.1f), Metre(0.1f), Metre(1), 10, 2);
	m_axeY->transform(Transform::MakeTranslation(Vector3<Metre>(Metre(), Metre(0.5f), Metre())));
	m_axeY->setColor(Color::Green());

	m_axeZ = GeometryHelper::CreateCylinder(Metre(0.1f), Metre(0.1f), Metre(1), 10, 2);
	m_axeZ->transform(Transform::MakeTranslation(Vector3<Metre>(Metre(), Metre(0.5f), Metre())));
	m_axeZ->transform(Transform::MakeRotationX(Degree(90)));
	m_axeZ->setColor(Color::Blue());

	m_material = new Material(ShaderHelper::LoadBaseVertexShader(), ShaderHelper::LoadBaseNoLitFragmentShader());

	uint32 posAttribute = m_material->attribute("aPosition");
	uint32 normalAttribute = m_material->attribute("aNormal");
	uint32 tangentAttribute = m_material->attribute("aTangent");
	uint32 uvAttribute = m_material->attribute("aTexCoord");
	uint32 vertexSize = sizeof(Vertex);

	glBindVertexArray(m_vao[0]);
	glEnableVertexAttribArray(posAttribute);
	glEnableVertexAttribArray(normalAttribute);
	glEnableVertexAttribArray(tangentAttribute);
	glEnableVertexAttribArray(uvAttribute);

	m_axeX->bindBuffersVAO();	
	glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)12);
	glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)24);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)36);
	glBindVertexArray(0);

	glBindVertexArray(m_vao[1]);
	glEnableVertexAttribArray(posAttribute);
	glEnableVertexAttribArray(normalAttribute);
	glEnableVertexAttribArray(tangentAttribute);
	glEnableVertexAttribArray(uvAttribute);

	m_axeY->bindBuffersVAO();	
	glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)12);
	glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)24);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)36);
	glBindVertexArray(0);

	glBindVertexArray(m_vao[2]);
	glEnableVertexAttribArray(posAttribute);
	glEnableVertexAttribArray(normalAttribute);
	glEnableVertexAttribArray(tangentAttribute);
	glEnableVertexAttribArray(uvAttribute);

	m_axeZ->bindBuffersVAO();
	glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)12);
	glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)24);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)36);
	glBindVertexArray(0);
}

Gizmo::~Gizmo()
{
	delete m_axeX;
	delete m_axeY;
	delete m_axeZ;

	delete m_material;

	glDeleteVertexArrays(3, m_vao);
}

void Gizmo::setTransform(const Transform& t)
{
	m_gizmoTransform = t;
}

const Transform& Gizmo::getTransform() const
{
	return m_gizmoTransform;
}

void Gizmo::render(const Scene& s)
{
	glBindVertexArray(m_vao[0]);
	m_material->bind();
	m_material->setMat4("gModelMatrix", getTransform());
	s.bind(*m_material);
	m_axeX->render(*m_material);
	m_material->unbind();
	glBindVertexArray(0);

	glBindVertexArray(m_vao[1]);
	m_material->bind();
	m_material->setMat4("gModelMatrix", getTransform());
	s.bind(*m_material);
	m_axeY->render(*m_material);
	m_material->unbind();
	glBindVertexArray(0);
	
	glBindVertexArray(m_vao[2]);
	m_material->bind();
	m_material->setMat4("gModelMatrix", getTransform());
	s.bind(*m_material);
	m_axeZ->render(*m_material);
	m_material->unbind();
	glBindVertexArray(0);	
}