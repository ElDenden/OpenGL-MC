#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);

}


bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ": " << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    //Binding calls
    shader.Bind();
    va.Bind();
    ib.Bind();

    //Drawing calls  
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const
{
    //Binding calls
    shader.Bind();
    va.Bind();
    vb.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, vb.getCount()));
}

void Renderer::Clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
