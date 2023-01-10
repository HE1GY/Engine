#include "pch.h"

#include "glad/glad.h"

#include "Application.h"
#include "Core.h"
#include "Renderer/Shader.h"

namespace Engine
{
	Application* Application::s_instance = nullptr;

Application::Application()
{
    ASSERT((s_instance == nullptr));
		s_instance = this;

		m_window = std::shared_ptr<Window>(Window::Create());
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    std::string vertex_src = R"(
    #version 460
    layout(location=0)in vec3 a_position;

    void main()
    {
        gl_Position=vec4(a_position,1);
    }
    )";

    std::string fragment_src = R"(
    #version 460
    layout(location=0)out vec4 color;

    void main()
    {
        color=vec4(1,1,1,1);
    }
    )";

    Shader shader(vertex_src, fragment_src);
    shader.Bind();

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float data[] = {
        -0.5, -0.5, 0, 0.5, -0.5, 0, 0, 0.5, 0,
    };

    m_vb.reset(VertexBuffer::Create(data, sizeof(data)));


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);

    uint32_t elements[] = {
        0,
        1,
        2,
    };
    m_ib.reset(IndexBuffer::Create(elements,sizeof(elements)/sizeof(uint32_t)));

    glClearColor(0, 0, 0, 0);
}

void Application::Run()
{
    while (m_is_running)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        for (auto layer : m_layer_stack)
        {
            layer->OnUpdate();
        }
        m_window->OnUpdate();
    }
}
void Application::OnEvent(Event &event)
{
    EventDispatcher ed(event);
    std::function<bool(WindowClosed &)> on_window_closed =
        std::bind(&Application::OnWindowsClosed, this, std::placeholders::_1);
    ed.Dispatch<WindowClosed>(on_window_closed);

    for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
    {
        (*--it)->OnEvent(event);
        if (event.Handled)
        {
            break;
        }
    }
}
bool Application::OnWindowsClosed(WindowClosed &event)
{
    m_is_running = false;
    return true;
}
void Application::PushLayer(Layer *layer)
{
    m_layer_stack.PushLayer(layer);
}
void Application::PushOverlay(Layer *layer)
{
    m_layer_stack.PushOverlay(layer);
}
void Application::EraseLayer(Layer *layer)
{
    m_layer_stack.EraseLayer(layer);
}
} // namespace Engine