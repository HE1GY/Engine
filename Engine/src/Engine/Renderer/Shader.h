#pragma once

namespace Engine
{
class Shader
{
  public:
    static Shader *Create(std::string vertex_src, std::string fragment_src);
    virtual ~Shader() = default;
    virtual void Bind() const = 0;
};
} // namespace Engine