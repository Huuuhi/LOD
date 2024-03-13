#include "GLError.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << std::hex << error << std::dec << "): ";   // 由于错误码是 16 进制, 通过 hex 输出 16 进制数, 使用后切换为 10 进制
        std::cout << function << " " << file << ":" << line << std::endl;
        return false;     // 如果有错误, 则返回 false
    }
    return true;
}