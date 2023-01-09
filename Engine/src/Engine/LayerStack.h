#pragma once

#include "Layer.h"

namespace Engine
{
class LayerStack
{
  public:
    LayerStack();
    ~LayerStack();


    void PushLayer(Layer *layer);
    void PushOverlay(Layer *layer);

    void EraseLayer(Layer *layer);

    inline std::vector<Layer *>::iterator begin()
    {
        return m_layers.begin();
    }
    inline std::vector<Layer *>::iterator end()
    {
        return m_layers.end();
    }

  private:
    std::vector<Layer *> m_layers;
    int m_position;
};

} // namespace Engine
