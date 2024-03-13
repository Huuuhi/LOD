#pragma once

#include <GLFW/glfw3.h>

#define ASSERT(x) if(!(x)) __debugbreak()   // 断言, 如果为 false, 则调用 msvc 的断点  注意括号 (x)

//  \ 是换行转义符, 之后不能加空格
// #x 将 x 转换为字符串, __FILE__ 和 __LINE__ 获取该代码的文件名和行号
// do-while 不是能够改变作用域, 而是确保在宏中的代码块可以被当做一个语句使用
#define GLCall(x) do{\
	    GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));\
	} while(0)

void GLClearError();   // 循环获取错误, 即获取所有错误, 则清空错误了
bool GLLogCall(const char* function, const char* file, int line);   // 循环获取错误, 每获取到错误, 就输出错误码, 获取错误过程遵循先入先出原则, 即先发生的错误先获取
