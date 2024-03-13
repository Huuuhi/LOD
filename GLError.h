#pragma once

#include <GLFW/glfw3.h>

#define ASSERT(x) if(!(x)) __debugbreak()   // ����, ���Ϊ false, ����� msvc �Ķϵ�  ע������ (x)

//  \ �ǻ���ת���, ֮���ܼӿո�
// #x �� x ת��Ϊ�ַ���, __FILE__ �� __LINE__ ��ȡ�ô�����ļ������к�
// do-while �����ܹ��ı�������, ����ȷ���ں��еĴ������Ա�����һ�����ʹ��
#define GLCall(x) do{\
	    GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
	} while(0)

void GLClearError();   // ѭ����ȡ����, ����ȡ���д���, ����մ�����
bool GLLogCall(const char* function, const char* file, int line);   // ѭ����ȡ����, ÿ��ȡ������, �����������, ��ȡ���������ѭ�����ȳ�ԭ��, ���ȷ����Ĵ����Ȼ�ȡ
