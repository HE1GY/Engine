#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_shader_lib.Load("flat_color", "../../Sandbox/assets/shaders/uniform_color_shader.glsl");

	m_vao_square = Engine::VertexArray::Create();
	m_vao_square->Bind();
	{
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

		m_vao_square->AddVertexBuffer(vo_data_box);
		m_vao_square->set_indexBuffer(vo_index_box);
	}
	m_vao_square->UnBind();
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::ColorEdit4("color", &m_uniform_color.r);
	ImVec2 size = { 500, 100 };
	ImGui::SetWindowSize(size);
}
void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_camera_controller.OnEvent(event);
}
void Sandbox2D::OnUpdate(Engine::TimeStep ts)
{
	m_camera_controller.OnUpdate(ts);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Engine::RendererCommand::Clear();

	Engine::Renderer::BeginScene(m_camera_controller.get_camera());

	auto flat_color_shader = m_shader_lib.Get("flat_color");
	std::dynamic_pointer_cast<Engine::OpenGLShader>(flat_color_shader)->UploadUniform("u_color", m_uniform_color);
	Engine::Renderer::Submit(flat_color_shader, m_vao_square);

	Engine::Renderer::EndScene();

}


