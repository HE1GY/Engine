#include "pch.h"

#include "glad/glad.h"

#include "Application.h"
#include "Core.h"

namespace Engine
{
	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:
			CORE_ERROR("BufferLayout None type");
			ASSERT(false);
			return 0;
		case ShaderDataType::Float:
			return GL_FLOAT;
			break;
		case ShaderDataType::Float2:
			return GL_FLOAT;
			break;
		case ShaderDataType::Float3:
			return GL_FLOAT;
			break;
		case ShaderDataType::Float4:
			return GL_FLOAT;
			break;
		case ShaderDataType::Int:
			return GL_INT;
			break;
		case ShaderDataType::Int2:
			return GL_INT;
			break;
		case ShaderDataType::Int3:
			return GL_INT;
			break;
		case ShaderDataType::Int4:
			return GL_INT;
			break;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
			break;
		case ShaderDataType::Mat4:
			return GL_FLOAT;
			break;
		case ShaderDataType::Bool:
			return GL_BOOL;
			break;
		default:
			CORE_ERROR("BufferLayout None type");
			ASSERT(false);
			return 0;
		}
	}

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		ASSERT((s_instance == nullptr));
		s_instance = this;

		m_window = std::shared_ptr<Window>(Window::Create());
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		std::string vertex_src = R"(
    #version 460 core
    layout(location=0)in vec3 a_position;
    layout(location=1)in vec3 a_color;

	out vec3 v_color;
    void main()
    {
		v_color=a_color;
        gl_Position=vec4(a_position,1);
    }
    )";

		std::string fragment_src = R"(
    #version 460 core
    layout(location=0)out vec4 color;
	in vec3 v_color;
    void main()
    {
        color=vec4(v_color,1);
    }
    )";
		m_shader.reset(new Shader(vertex_src, fragment_src));
		m_shader->Bind();

		GLuint vao;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		float data[] = {
				-0.5, -0.5, 0, 1, 0, 0,
				0.5, -0.5, 0, 0, 1, 0,
				0, 0.5, 0, 0, 0, 1,
		};

		m_vb.reset(VertexBuffer::Create(data, sizeof(data)));

		BufferLayout layout = {
				{ "a_position", ShaderDataType::Float3, false },
				{ "a_color", ShaderDataType::Float3, false },
		};

		uint32_t index{ 0 };
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type),
					element.normalized, layout.get_stride(), (const void*)element.offset);
			index++;
		}

		uint32_t elements[] = {
				0,
				1,
				2,
		};
		m_ib.reset(IndexBuffer::Create(elements, sizeof(elements) / sizeof(uint32_t)));

	}

	void Application::Run()
	{
		while (m_is_running)
		{
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (auto layer : m_layer_stack)
			{
				layer->OnUpdate();
			}
			m_window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		std::function<bool(WindowClosed&)> on_window_closed =
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
	bool Application::OnWindowsClosed(WindowClosed& event)
	{
		m_is_running = false;
		return true;
	}
	void Application::PushLayer(Layer* layer)
	{
		m_layer_stack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_layer_stack.PushOverlay(layer);
	}
	void Application::EraseLayer(Layer* layer)
	{
		m_layer_stack.EraseLayer(layer);
	}
} // namespace Engine