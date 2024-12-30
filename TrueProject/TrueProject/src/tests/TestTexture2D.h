#pragma once

#include "Test.h"

#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../IndexBuffer.h"
#include "../Texture.h"
#include "../Camera.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern Camera camera;
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		;
	private:
		glm::vec3 m_TranslationA, m_TranslationB;
		glm::mat4 m_Proj, m_View;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VB;
		;
	};
}