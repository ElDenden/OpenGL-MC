#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//For error checking
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__, __LINE__))
//Error Checking

/// <summary>
/// Clears all OpenGL Errors
/// </summary>
void GLClearError();

/// <summary>
/// Prints all OpenGL errors
/// </summary>
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    void Draw(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const;

    void Clear();
    ;
};