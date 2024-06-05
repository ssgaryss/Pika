#include "pkpch.h"
#include "Camera2D.h"


namespace Pika
{
    Pika::Camera2D::Camera2D(float vLeft, float vRight, float vBottom, float vTop)
    {
        m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    const glm::vec3 Camera2D::getPosition() const
    {
        return m_Position;
    }

    const glm::mat4 Pika::Camera2D::getViewMatrix() const
    {
        return m_ViewMatrix;
    }

    const glm::mat4 Pika::Camera2D::getProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }
    const glm::mat4 Camera2D::getViewProjectionMatrix() const
    {
        return m_ViewProjectionMatrix;
    }
}
