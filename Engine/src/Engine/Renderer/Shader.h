#pragma once

namespace Engine
{
class Shader
{
  public:
    static Shader *Create(const std::string &vertex_src, const std::string &fragment_src);
    static Shader *Create(const std::string &src_path);
    virtual ~Shader() = default;
    virtual void Bind() const = 0;
};
} // namespace Engine