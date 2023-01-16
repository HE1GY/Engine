#include "Engine.h"

#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public Engine::Layer
{

public:
	ExampleLayer()
			:Layer("ExampleLayer"), m_camera(-1.6, 1.6, -0.9, 0.9), m_square_pos(1.0f)
	{
		m_vao_square_texture2d.reset(Engine::VertexArray::Create());
		m_vao_square_texture2d->Bind();

		m_shader_texture2D.reset(Engine::Shader::Create("../../Sandbox/assets/shaders/texture2D_shader.glsl"));
		m_shader_texture2D->Bind();

		float data[] = {
				-0.5, -0.5, 0, 0, 0, 0.5, -0.5, 0, 1, 0, 0.5, 0.5, 0, 1, 1, -0.5, 0.5, 0, 0, 1,
		};
		Engine::Ref<Engine::VertexBuffer> vo_data;
		vo_data.reset(Engine::VertexBuffer::Create(data, sizeof(data)));
		vo_data->Bind();

		vo_data->set_layout({
				{ "a_position", Engine::ShaderDataType::Float3, false },
				{ "a_tex_coord", Engine::ShaderDataType::Float2, false },
		});

		uint32_t elements[] = {
				0, 1, 2, 2, 3, 0,
		};
		Engine::Ref<Engine::IndexBuffer> vo_index;
		vo_index.reset(Engine::IndexBuffer::Create(elements, sizeof(elements) / sizeof(uint32_t)));
		vo_index->Bind();

		m_vao_square_texture2d->AddVertexBuffer(vo_data);
		m_vao_square_texture2d->set_indexBuffer(vo_index);

		m_vao_square_texture2d->UnBind();

		{
			m_vao_box.reset(Engine::VertexArray::Create());
			m_vao_box->Bind();

			m_shader_box.reset(Engine::Shader::Create("../../Sandbox/assets/shaders/uniform_color_shader.glsl"));
			m_shader_box->Bind();

			float pos_box[] = {
					-0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0, -0.5, 0.5, 0,
			};
			Engine::Ref<Engine::VertexBuffer> vo_data_box;
			vo_data_box.reset(Engine::VertexBuffer::Create(pos_box, sizeof(pos_box)));
			vo_data_box->Bind();

			vo_data_box->set_layout({{ "a_position", Engine::ShaderDataType::Float3, false }});

			uint32_t elements_box[] = { 0, 1, 2, 2, 3, 0 };
			Engine::Ref<Engine::IndexBuffer> vo_index_box;
			vo_index_box.reset(Engine::IndexBuffer::Create(elements_box, sizeof(elements_box) / sizeof(uint32_t)));
			vo_index_box->Bind();

			m_vao_box->AddVertexBuffer(vo_data_box);
			m_vao_box->set_indexBuffer(vo_index_box);
		}

		m_texture_2 = Engine::Texture2D::Create("../../Sandbox/assets/textures/ILTQq.png");
		m_texture = Engine::Texture2D::Create("../../Sandbox/assets/textures/wallpaperflare.com_wallpaper.png");
	}

	void OnEvent(Engine::Event& event) override
	{
	}

	void OnUpdate(Engine::TimeStep ts) override
	{
		{
			glm::vec3 cam_pos = m_camera.get_position();
			float cam_rot = m_camera.get_rotation();
			float speed = 5.0f;
			float speed_rot = 30.0f;

			if (Engine::Input::IsKeyPress(KEY_A))
			{
				cam_pos.x -= speed * ts;
			}
			if (Engine::Input::IsKeyPress(KEY_D))
			{
				cam_pos.x += speed * ts;
			}
			if (Engine::Input::IsKeyPress(KEY_W))
			{
				cam_pos.y += speed * ts;
			}
			if (Engine::Input::IsKeyPress(KEY_S))
			{
				cam_pos.y -= speed * ts;
			}
			if (Engine::Input::IsKeyPress(KEY_Q))
			{
				cam_rot += speed_rot * ts;
			}
			if (Engine::Input::IsKeyPress(KEY_E))
			{
				cam_rot -= speed_rot * ts;
			}

			m_camera.set_rotation(cam_rot);
			m_camera.set_position(cam_pos);
		}

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		Engine::Renderer::BeginScene(m_camera);

		float x = 0;
		float y = 0;
		for (int i = 0; i < 20; ++i)
		{
			y = i * 0.11;
			for (int j = 0; j < 20; ++j)
			{
				std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader_box)
						->UploadUniform("u_color", m_uniform_color);
				Engine::Renderer::Submit(m_shader_box, m_vao_box,
						glm::translate(glm::mat4(1.0f), m_square_pos) *
								glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.1f }));
				x = j * 0.11;

				m_square_pos = { x, y, 0 };
			}
		}

		m_texture->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader_texture2D)->UploadUniform("u_texture2D", 0);
		Engine::Renderer::Submit(m_shader_texture2D, m_vao_square_texture2d);

		m_texture_2->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader_texture2D)->UploadUniform("u_texture2D", 0);
		Engine::Renderer::Submit(m_shader_texture2D, m_vao_square_texture2d);

		Engine::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::ColorEdit4("color", &m_uniform_color.r);
		ImVec2 size = { 500, 100 };
		ImGui::SetWindowSize(size);
	}

private:
	Engine::Ref<Engine::Shader> m_shader_texture2D;
	Engine::Ref<Engine::VertexArray> m_vao_square_texture2d;

	Engine::Ref<Engine::Shader> m_shader_box;
	Engine::Ref<Engine::VertexArray> m_vao_box;

	Engine::Ref<Engine::Texture2D> m_texture;
	Engine::Ref<Engine::Texture2D> m_texture_2;

	Engine::OrthographicCamera m_camera;

	glm::vec3 m_square_pos;
	glm::vec4 m_uniform_color = { 1, 0, 0, 0 };
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
