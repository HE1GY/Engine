#pragma once

#include "glm/glm.hpp"

namespace Engine
{
class Shader
{
  public:
    Shader(std::string vertex_src, std::string fragment_src);
    ~Shader();
    void Bind();
    void UploadUniform(const std::string &name, float value);
    void UploadUniform(const std::string &name, const glm::mat4 &value);

  private:
    uint32_t m_renderer_id;
};
} // namespace Engine