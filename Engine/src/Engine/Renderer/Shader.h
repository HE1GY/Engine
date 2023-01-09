#pragma once
#include <string>

namespace Engine{
class Shader{
  public:
    Shader(std::string vertex_src,std::string fragment_src);
    ~Shader();
    void Bind();

  private:
    uint32_t m_renderer_id;
};
}