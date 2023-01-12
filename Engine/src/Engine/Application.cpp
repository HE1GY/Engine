#include "pch.h"

#include "Application.h"
#include "Core.h"

#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

#include <glm/vec4.hpp>

#include "KeyCode.h"

namespace Engine
{

Application *Application::s_instance = nullptr;

Application::Application() : m_camera(-1.6, 1.6, -0.9, 0.9)
{
    CORE_ASSERT((s_instance == nullptr), "App exists");
    s_instance = this;

    m_window = std::shared_ptr<Window>(Window::Create());
    m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    m_vao_triangle.reset(VertexArray::Create());
    m_vao_triangle->Bind();

    std::string vertex_src = R"(
    #version 460 core
    layout(location=0)in vec3 a_position;
    layout(location=1)in vec3 a_color;

    uniform mat4 u_vp;

	out vec3 v_color;
    void main()
    {
		v_color=a_color;
        gl_Position=u_vp* vec4(a_position,1);
    }
    )";

    std::string fragment_src = R"(
    #version 460 core
    in vec3 v_color;

    layout(location=0)out vec4 color;
    void main()
    {
        color=vec4(v_color,1);
    }
    )";

    m_shader_triangle.reset(new Shader(vertex_src, fragment_src));
    m_shader_triangle->Bind();

    float pos[] = {
        -0.5, -0.5, 0, 1, 0, 0, 0.5, -0.5, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 1,
    };
    std::shared_ptr<VertexBuffer> vo_data;
    vo_data.reset(VertexBuffer::Create(pos, sizeof(pos)));
    vo_data->Bind();

    BufferLayout layout = {
        {"a_position", ShaderDataType::Float3, false},
        {"a_color", ShaderDataType::Float3, false},
    };

    vo_data->set_layout(layout);

    uint32_t elements[] = {
        0,
        1,
        2,
    };
    std::shared_ptr<IndexBuffer> vo_index;
    vo_index.reset(IndexBuffer::Create(elements, sizeof(elements) / sizeof(uint32_t)));
    vo_index->Bind();

    m_vao_triangle->AddVertexBuffer(vo_data);
    m_vao_triangle->set_indexBuffer(vo_index);

    m_vao_triangle->UnBind();

    {
        std::string vertex_src_box = R"(
    #version 460 core
    layout(location=0)in vec3 a_position;
    uniform mat4 u_vp;

	out vec3 v_color;
    void main()
    {
        gl_Position=u_vp*vec4(a_position,1);
    }
    )";

        std::string fragment_src_box = R"(
    #version 460 core
    layout(location=0)out vec4 color;
    void main()
    {
        color=vec4(0.6,0.2,0.4,1);
    }
    )";

        m_vao_box.reset(VertexArray::Create());
        m_vao_box->Bind();

        m_shader_box.reset(new Shader(vertex_src_box, fragment_src_box));
        m_shader_box->Bind();

        float pos_box[] = {
            -0.8, -0.8, 0, 0.8, -0.8, 0, 0.8, 0.8, 0, -0.8, 0.8, 0,
        };
        std::shared_ptr<VertexBuffer> vo_data_box;
        vo_data_box.reset(VertexBuffer::Create(pos_box, sizeof(pos_box)));
        vo_data_box->Bind();

        BufferLayout layout_box = {{"a_position", ShaderDataType::Float3, false}};

        vo_data_box->set_layout(layout_box);

        uint32_t elements_box[] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> vo_index_box;
        vo_index_box.reset(IndexBuffer::Create(elements_box, sizeof(elements_box) / sizeof(uint32_t)));
        vo_index_box->Bind();

        m_vao_box->AddVertexBuffer(vo_data_box);
        m_vao_box->set_indexBuffer(vo_index_box);
    }
}

void Application::Run()
{

    while (m_is_running)
    {
        RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 0.1f});
        RendererCommand::Clear();

        Renderer::BeginScene(m_camera);

        Renderer::Submit(m_shader_box, m_vao_box);

        Renderer::Submit(m_shader_triangle, m_vao_triangle);

        Renderer::EndScene();

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

    std::function<bool(KeyPress &)> on_camera_control =
        std::bind(&Application::CameraControl, this, std::placeholders::_1);
    ed.Dispatch<KeyPress>(on_camera_control);

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

bool Application::CameraControl(KeyPress &event)
{
    glm::vec3 cam_pos = m_camera.get_position();
    float cam_rot = m_camera.get_rotation();
    float speed = 0.2f;

    int key_code = event.get_key();
    switch (key_code)
    {
    case KEY_A:
        cam_pos.x += speed;
        break;
    case KEY_D:
        cam_pos.x -= speed;
        break;
    case KEY_W:
        cam_pos.y -= speed;
        break;
    case KEY_S:
        cam_pos.y += speed;
        break;
    case KEY_Q:
        cam_rot -= speed;
        break;
    case KEY_E:
        cam_rot += speed;
        break;
    }
    m_camera.set_rotation(cam_rot);
    m_camera.set_position(cam_pos);

    return false;
}
} // namespace Engine