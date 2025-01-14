#include "Test.h"
#include "../vendor/imgui/imgui.h"
namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer): m_CurrentTest(currentTestPointer)
	{

	}
	/// <summary>
	/// Updates the current test if the corresponding button is clicked
	/// </summary>
	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				function_name = std::string(test.first);
				m_CurrentTest = test.second();
			}
		}
		if (ImGui::Button(std::string("Exit").c_str()))
		{
			m_CurrentTest = nullptr;
		}
	}
}
