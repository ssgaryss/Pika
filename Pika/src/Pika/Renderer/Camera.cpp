#include "pkpch.h"
#include "Camera.h"


namespace Pika
{
    Pika::Camera::Camera(float vLeft, float vRight, float vBottom, float vTop)
    {
        m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    const glm::vec3 Camera::getPosition() const
    {
        return m_Position;
    }

    const glm::mat4 Pika::Camera::getViewMatrix() const
    {
        return m_ViewMatrix;
    }

    const glm::mat4 Pika::Camera::getProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }
    const glm::mat4 Camera::getViewProjectionMatrix() const
    {
        return m_ViewProjectionMatrix;
    }
}
