#include "TestTexture2D.h"
#include <GL/glew.h>
#include "../Renderer.h"
#include "../vendor/imgui/imgui.h"


namespace test
{
	TestTexture2D::TestTexture2D()
        : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0), m_Proj(glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)), m_View(camera.GetViewMatrix())
	{
        float positions[] = {
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //0 (first half of values = positons, second half = texture coordinates)
           0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //1
           0.5f,  0.5f, 0.0f,1.0f, 1.0f, //2
           -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  //3
        };

        //Index array/buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        //Setting up blending function for textures (blending function is by default set to add)
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_VAO = std::make_unique<VertexArray>();
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);
        m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
        m_VB = std::make_unique<VertexBuffer>(positions, 5 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(3u);
        layout.push<float>(2u);
        m_VAO->AddBuffer(*m_VB, layout);

        m_Shader->Bind();
        m_Shader->setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        m_Shader->setUniform1i("u_Texture", 0);
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
        m_Proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        m_View = camera.GetViewMatrix();

	}
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->setUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->setUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
	}
	void TestTexture2D::OnImGuiRender()
	{
        //ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        //ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}