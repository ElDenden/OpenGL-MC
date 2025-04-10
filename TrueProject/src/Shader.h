#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	//caching uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;

	;
public:
	Shader(const std::string& filepath);
	~Shader();


	void Bind() const;
	void Unbind() const;

	//Set uniforms

	void setUniform1i(const std::string& name, int v0);

	void setUniform3f(const std::string& name, float v0, float v1, float v2);

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void setUniform1f(const std::string& name, float v0);

	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
	;
private:
	int getUniformLocation(const std::string& name);

	unsigned int compileShader(unsigned int type, const std::string& source);

	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	ShaderProgramSource parseShader(const std::string& filepath);
	;
};