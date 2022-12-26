
#include "LayerStack.h"
namespace Engine
{
void LayerStack::PushLayer(Engine::Layer *layer)
{
    m_layers.emplace(m_layers.begin() + m_position++, layer);
}
void LayerStack::PushOverlay(Engine::Layer *layer)
{
    m_layers.push_back(layer);
}
void LayerStack::EraseLayer(Engine::Layer *layer)
{
    auto position_iterator = std::find(m_layers.begin(), m_layers.end(), layer);
    if (position_iterator != m_layers.end())
    {
        m_layers.erase(position_iterator);
    }
}

} // namespace Engine
