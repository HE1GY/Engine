#include "Engine/Core/EntryPoint.h"
#include "Engine.h"

#include "Layers/Sandbox2D.h"
#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public Engine::Layer
{

public:
	ExampleLayer()
			:Layer("ExampleLayer"), m_camera_controller(1280.0f / 720.0f, true)
	{
		m_shader_lib.Load("texture", "../../../Sandbox/assets/shaders/default_2D_shader.glsl");
		m_shader_lib.Load("flat_color", "../../../Sandbox/assets/shaders/uniform_color_shader.glsl");

		m_texture_2 = Engine::Texture2D::Create("../../../Sandbox/assets/textures/ILTQq.png");
		m_texture = Engine::Texture2D::Create("../../../Sandbox/assets/textures/wallpaperflare.com_wallpaper.png");

		m_vao_square_texture2d = Engine::VertexArray::Create();
		m_vao_square_texture2d->Bind();
		{
			float data[] = {
					-0.5, -0.5, 0, 0, 0, 0.5, -0.5, 0, 1, 0, 0.5, 0.5, 0, 1, 1, -0.5, 0.5, 0, 0, 1,
			};
			Engine::Ref<Engine::VertexBuffer> vo_data;
			vo_data = Engine::VertexBuffer::Create(data, sizeof(data));
			vo_data->Bind();

			vo_data->set_layout({
					{ "a_position", Engine::ShaderDataType::Float3, false },
					{ "a_tex_coord", Engine::ShaderDataType::Float2, false },
			});

			uint32_t elements[] = {
					0, 1, 2, 2, 3, 0,
			};
			Engine::Ref<Engine::IndexBuffer> vo_index;
			vo_index = Engine::IndexBuffer::Create(elements, sizeof(elements) / sizeof(uint32_t));
			vo_index->Bind();

			m_vao_square_texture2d->AddVertexBuffer(vo_data);
			m_vao_square_texture2d->SetIndexBuffer(vo_index);
		}
		m_vao_square_texture2d->UnBind();

		m_vao_square = Engine::VertexArray::Create();
		m_vao_square->Bind();
		{
			float pos_box[] = {
					-0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0, -0.5, 0.5, 0,
			};
			Engine::Ref<Engine::VertexBuffer> vo_data_box;
			vo_data_box = Engine::VertexBuffer::Create(pos_box, sizeof(pos_box));
			vo_data_box->Bind();

			vo_data_box->set_layout({{ "a_position", Engine::ShaderDataType::Float3, false }});

			uint32_t elements_box[] = { 0, 1, 2, 2, 3, 0 };
			Engine::Ref<Engine::IndexBuffer> vo_index_box;
			vo_index_box = Engine::IndexBuffer::Create(elements_box, sizeof(elements_box) / sizeof(uint32_t));
			vo_index_box->Bind();

			m_vao_square->AddVertexBuffer(vo_data_box);
			m_vao_square->SetIndexBuffer(vo_index_box);
		}
		m_vao_square->UnBind();
	}

	void OnEvent(Engine::Event& event) override
	{
		m_camera_controller.OnEvent(event);
	}

	void OnUpdate(Engine::TimeStep ts) override
	{
		m_camera_controller.OnUpdate(ts);

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		Engine::Renderer::BeginScene(m_camera_controller.get_camera());

		float x{ 0 };
		float y{ 0 };
		glm::vec3 square_pos{ 0, 0, 0 };
		for (int i = 0; i < 20; ++i)
		{
			y = i * 0.11;
			for (int j = 0; j < 20; ++j)
			{
				x = j * 0.11;
				square_pos = { x, y, 0 };
				auto flat_shader = m_shader_lib.Get("flat_color");
				flat_shader->Bind();
				flat_shader->SetVec4("u_color", m_uniform_color);
				Engine::Renderer::Submit(flat_shader, m_vao_square,
						glm::translate(glm::mat4(1.0f), square_pos) *
								glm::scale(glm::mat4(1.0f), { 0.1f, 0.1f, 0.1f }));
			}
		}

		m_texture->Bind();
		auto texture = m_shader_lib.Get("texture");
		texture->Bind();
		texture->SetInt("u_texture2D", 0);
		Engine::Renderer::Submit(texture, m_vao_square_texture2d);

		m_texture_2->Bind();
		texture->Bind();
		texture->SetInt("u_texture2D", 0);
		Engine::Renderer::Submit(texture, m_vao_square_texture2d);

		Engine::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::ColorEdit4("color", &m_uniform_color.r);
		ImVec2 size = { 500, 100 };
		ImGui::SetWindowSize(size);
	}

private:
	Engine::ShaderLibrary m_shader_lib;

	Engine::Ref<Engine::VertexArray> m_vao_square_texture2d;

	Engine::Ref<Engine::VertexArray> m_vao_square;

	Engine::Ref<Engine::Texture2D> m_texture;
	Engine::Ref<Engine::Texture2D> m_texture_2;

	Engine::OrthographicCameraController m_camera_controller;

	glm::vec4 m_uniform_color{ 0.2, 0.8, 0.8, 1 };
};

class SandBox : public Engine::Application
{
public:
	SandBox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~SandBox() override = default;
};
Engine::Application* CreateApplication()
{
	return new SandBox();
}



