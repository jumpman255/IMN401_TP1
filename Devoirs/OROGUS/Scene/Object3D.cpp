#include "Object3D.h"

#include "Scene.h"
#include "../Geometry/Geometry.h"
#include "../Geometry/GeometryManager.h"
#include "../Material/Material.h"
#include "../Material/ShaderHelper.h"

Object3D::Object3D(const std::string& name, Material* material, Geometry* geometry)
    : m_material(material)
    , m_geometry(geometry)
    , m_parent(nullptr)
    , m_scene(nullptr)
    , m_name(name)
{
    glCreateVertexArrays(1, &m_vao);
	glCreateVertexArrays(1, &m_vaoNormals);
	m_normalMaterial = new Material(ShaderHelper::LoadEngineNormalVertexShader(), ShaderHelper::LoadEngineNormalFragmentShader());
    updateVAO();
}

Object3D::~Object3D()
{
	delete m_normalMaterial;
	m_normalMaterial = nullptr;

    for (Object3D* child : m_children)
    {
        delete child;
    }
    m_children.clear();

    if (m_geometry != nullptr)
    {
        GeometryManager::GetInstance()->unloadGeometry(m_geometry);
    }

    if (m_material != nullptr)
    {
        delete m_material;
    }

    glDeleteVertexArrays(1, &m_vao);
	glDeleteVertexArrays(1, &m_vaoNormals);
}

const std::string& Object3D::getName() const
{
	return m_name;
}

void Object3D::setScene(const Scene* scene)
{
    m_scene = scene;
    for (Object3D* child : m_children)
    {
        child->setScene(scene);
    }
}

void Object3D::setParent(Object3D* parent)
{
    m_parent = parent;
    updateVAO();
}

void Object3D::updateVAO() const
{
    const Material* mat = getMaterial();
    if (mat != nullptr)
    {
        glBindVertexArray(m_vao);
        uint32 posAttribute = mat->attribute("aPosition");
        uint32 normalAttribute = mat->attribute("aNormal");
        uint32 tangentAttribute = mat->attribute("aTangent");
        uint32 uvAttribute = mat->attribute("aTexCoord");
        glEnableVertexAttribArray(posAttribute);
        glEnableVertexAttribArray(normalAttribute);
        glEnableVertexAttribArray(tangentAttribute);
        glEnableVertexAttribArray(uvAttribute);

        if (m_geometry != nullptr)
        {
            m_geometry->bindBuffersVAO();
        }

		uint32 vertexSize = sizeof(Vertex);
		glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)12);
		glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)24);
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, vertexSize, (const GLvoid*)36);
        glBindVertexArray(0);
    }

	if (m_normalMaterial->isInitialized())
	{
		glBindVertexArray(m_vaoNormals);
		uint32 posAttribute = m_normalMaterial->attribute("aPosition");
		glEnableVertexAttribArray(posAttribute);
		if (m_geometry != nullptr)
		{
			m_geometry->bindBuffersNormalVAO();
		}
		glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
	}
}

const Material* Object3D::getMaterial() const
{
    if (m_material != nullptr)
    {
        return m_material;
    }
    else if (m_parent != nullptr)
    {
        return m_parent->getMaterial();
    }
    return m_scene->getSceneMaterial();
}

void Object3D::setTransform(const Transform& t)
{
    m_transformation = t;
}

void Object3D::addChildren(Object3D* child)
{
    if (child != nullptr)
    {
        m_children.push_back(child);
        child->setParent(this);
    }
}

void Object3D::transformObject(const Transform& t)
{
    m_transformation = t * m_transformation;
}

Transform Object3D::getObjectTransform() const
{
    return m_transformation;
}

Transform Object3D::getTransform() const
{
    if (m_parent != nullptr)
    {
        return m_parent->getTransform() * getObjectTransform();
    }
    else
    {
        return m_scene->getSceneTransform() * getObjectTransform();
    }
}

void Object3D::render() const
{
    glBindVertexArray(m_vao);
    const Material* material = getMaterial();
    if (material != nullptr)
    {
        material->bind();
        material->setMat4("gModelMatrix", getTransform());
        m_scene->bind(*material);
    
        if (m_geometry != nullptr)
        {
            m_geometry->render(*material);
        }

        for (Object3D* child : m_children)
        {
            child->render();
        }
    
        material->unbind();
    }
    glBindVertexArray(0);
}

void Object3D::renderNormals() const
{
	glBindVertexArray(m_vaoNormals);
	m_normalMaterial->bind();
	m_normalMaterial->setMat4("gModelMatrix", getTransform());
	m_scene->bindNormals(*m_normalMaterial);
	if (m_geometry != nullptr)
	{
		m_geometry->renderNormal();
	}

	for (Object3D* child : m_children)
	{
		child->renderNormals();
	}

	m_normalMaterial->unbind();
	glBindVertexArray(0);
}