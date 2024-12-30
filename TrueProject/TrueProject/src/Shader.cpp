#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath): m_RendererID(0), m_FilePath(filepath)
{
    ShaderProgramSource src = parseShader(m_FilePath);

    m_RendererID = createShader(src.VertexSource, src.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); //See documentation for this
    glCompileShader(id);

    //TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Checks if shader is good
    if (result == GL_FALSE)
    {
        //Query message length
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << "source:\n" << src << std::endl;
        std::cout << "message:\n" << message << std::endl;
        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);//See documentation

    glValidateProgram(program);//See documentation

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int v0)
{
    GLCall(glUniform1i(getUniformLocation(name), v0));
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniform1f(const std::string& name, float v0)
{
    GLCall(glUniform1f(getUniformLocation(name), v0));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;

	return location;
}
