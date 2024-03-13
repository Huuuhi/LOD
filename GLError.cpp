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
        std::cout << "[OpenGL Error] (" << std::hex << error << std::dec << "): ";   // ���ڴ������� 16 ����, ͨ�� hex ��� 16 ������, ʹ�ú��л�Ϊ 10 ����
        std::cout << function << " " << file << ":" << line << std::endl;
        return false;     // ����д���, �򷵻� false
    }
    return true;
}