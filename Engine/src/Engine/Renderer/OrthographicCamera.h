#pragma once

#include "glm/glm.hpp"

namespace Engine
{
class OrthographicCamera
{
  public:
    OrthographicCamera(float left, float right, float bottom, float top);

    inline const glm::vec3 &get_position() const
    {
        return m_position;
    }
    inline float get_rotation() const
    {
        return m_rotation;
    }

    inline void set_position(const glm::vec3 &position)
    {
        m_position = position;
        RecalculateViewMatrix();
    }
    inline void set_rotation(float rotation)
    {
        m_rotation = rotation;
        RecalculateViewMatrix();
    }

    inline const glm::mat4 &get_view_matrix() const
    {
        return m_view_matrix;
    }
    inline const glm::mat4 &get_projection_matrix() const
    {
        return m_projection_matrix;
    }
    inline const glm::mat4 &get_view_projection_matrix() const
    {
        return m_view_projection_matrix;
    }

  private:
    void RecalculateViewMatrix();

  private:
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;
    glm::mat4 m_view_projection_matrix;

    glm::vec3 m_position;
    float m_rotation;
};
} // namespace Engine
