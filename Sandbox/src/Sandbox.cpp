#include "Engine.h"

class ExampleLayer : public Engine::Layer
{

public:
	ExampleLayer()
			:Layer("ExampleLayer"), m_camera(-1.6, 1.6, -0.9, 0.9)
	{
		m_vao_triangle.reset(Engine::VertexArray::Create());
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

		m_shader_triangle.reset(new Engine::Shader(vertex_src, fragment_src));
		m_shader_triangle->Bind();

		float pos[] = {
				-0.5, -0.5, 0, 1, 0, 0, 0.5, -0.5, 0, 0, 1, 0, 0, 0.5, 0, 0, 0, 1,
		};
		std::shared_ptr<Engine::VertexBuffer> vo_data;
		vo_data.reset(Engine::VertexBuffer::Create(pos, sizeof(pos)));
		vo_data->Bind();

		Engine::BufferLayout layout = {
				{ "a_position", Engine::ShaderDataType::Float3, false },
				{ "a_color", Engine::ShaderDataType::Float3, false },
		};

		vo_data->set_layout(layout);

		uint32_t elements[] = {
				0,
				1,
				2,
		};
		std::shared_ptr<Engine::IndexBuffer> vo_index;
		vo_index.reset(Engine::IndexBuffer::Create(elements, sizeof(elements) / sizeof(uint32_t)));
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

			m_vao_box.reset(Engine::VertexArray::Create());
			m_vao_box->Bind();

			m_shader_box.reset(new Engine::Shader(vertex_src_box, fragment_src_box));
			m_shader_box->Bind();

			float pos_box[] = {
					-0.8, -0.8, 0, 0.8, -0.8, 0, 0.8, 0.8, 0, -0.8, 0.8, 0,
			};
			std::shared_ptr<Engine::VertexBuffer> vo_data_box;
			vo_data_box.reset(Engine::VertexBuffer::Create(pos_box, sizeof(pos_box)));
			vo_data_box->Bind();

			Engine::BufferLayout layout_box = {{ "a_position", Engine::ShaderDataType::Float3, false }};

			vo_data_box->set_layout(layout_box);

			uint32_t elements_box[] = { 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<Engine::IndexBuffer> vo_index_box;
			vo_index_box.reset(Engine::IndexBuffer::Create(elements_box, sizeof(elements_box) / sizeof(uint32_t)));
			vo_index_box->Bind();

			m_vao_box->AddVertexBuffer(vo_data_box);
			m_vao_box->set_indexBuffer(vo_index_box);
		}
	}

	void OnEvent(Engine::Event& event) override
	{
	}

	void OnUpdate() override
	{
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		Engine::Renderer::BeginScene(m_camera);

		Engine::Renderer::Submit(m_shader_box, m_vao_box);

		Engine::Renderer::Submit(m_shader_triangle, m_vao_triangle);

		Engine::Renderer::EndScene();

		glm::vec3 cam_pos = m_camera.get_position();
		float cam_rot = m_camera.get_rotation();
		float speed = 0.01f;
		float speed_rot = 0.1f;

		if (Engine::Input::IsKeyPress(KEY_A))
		{ cam_pos.x += speed; }
		if (Engine::Input::IsKeyPress(KEY_D))
		{ cam_pos.x -= speed; }
		if (Engine::Input::IsKeyPress(KEY_W))
		{ cam_pos.y -= speed; }
		if (Engine::Input::IsKeyPress(KEY_S))
		{ cam_pos.y += speed; }
		if (Engine::Input::IsKeyPress(KEY_Q))
		{ cam_rot -= speed_rot; }
		if (Engine::Input::IsKeyPress(KEY_E))
		{ cam_rot += speed_rot; }

		m_camera.set_rotation(cam_rot);
		m_camera.set_position(cam_pos);

	}
private:
	std::shared_ptr<Engine::Shader> m_shader_triangle;
	std::shared_ptr<Engine::VertexArray> m_vao_triangle;

	std::shared_ptr<Engine::Shader> m_shader_box;
	std::shared_ptr<Engine::VertexArray> m_vao_box;

	Engine::OrthographicCamera m_camera;
};

class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	~SandBox() override = default;
};

Engine::Application* CreateApplication()
{
	return new SandBox();
}
