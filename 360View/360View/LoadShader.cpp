//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader Progame 编程
// Shader：类似于C编译器
// Program：类似于C链接器
//
// 1. 创建Shader
//1）编写Vertex Shader和 Fragment Shader源码
//2）创建两个shader实例 glCreateShader(GLenum type)
//3）给Shader实例指定源码 glShaderSource
//4）在线编译shaer源码  glCompileShader(GLuint shader)
//
//创建Program
//1）创建program glCreateProgram(void)
//2）绑定shader到program glAttachShader(GLuint program, GLuint shader) 每个program必须绑定一个Vertex Shader和一个Fragment Shader
//3）链接program glLinkProgram(GLuint program)
//4）使用porgram glUseProgram(GLuint program)
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LoadShader.h"
#include <iostream>


#ifdef __cplusplus
extern "C"{
#endif     //__cplusplus

	static const GLchar* ReadShader(const char* fileName)
	{
#ifdef WIN32
		FILE* infile;
		fopen_s(&infile, fileName, "rb");
#else
		FILE* infile = fopen(fileName, "rb");
#endif //WIN32

		if (!infile)
		{
#ifdef _DEBUG
			std::cerr << "Unable to open file..." << std::endl;
#endif //_DEBUG
			return NULL;
		}

		fseek(infile, 0, SEEK_END); //SEEK_END = 2
		int lenFile = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		GLchar* source = new GLchar[lenFile + 1];
		fread(source, 1, lenFile, infile);
		fclose(infile);

		source[lenFile] = 0;

		return const_cast<const GLchar*>(source);
	}

	GLuint LoadShader(ShaderInfo* shaders)
	{
		if (shaders == NULL)
		{
			return 0;
		}

		GLuint program = glCreateProgram(); //program 类是C链接器
		ShaderInfo* entry = shaders;

		while (entry->type != GL_NONE)
		{
			GLuint shader = glCreateShader(entry->type); //shader类似C编译器
			entry->shader = shader;

			const GLchar* soruce = ReadShader(entry->fileName); //获取源码
			if (soruce == NULL)
			{
				for (entry = shaders; entry->type != GL_NONE; ++entry)
				{
					glDeleteShader(entry->shader);
					entry->shader = 0;
				}
				return 0;
			}

			glShaderSource(shader, 1, &soruce, NULL); //给shader指定源码
			delete[]soruce;
			glCompileShader(shader);                  //编译

			GLint complied; //获取shader的编译情况
			glGetShaderiv(shader, GL_COMPILE_STATUS, &complied);
			if (!complied)
			{
#ifdef _DEBUG
				GLsizei len;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				GLchar* log = new GLchar[len + 1];
				glGetShaderInfoLog(shader, len, &len, log);
				std::cerr << "Shader compilation failed: " << log << std::endl;
				delete[] log;
#endif // _DEBUG
				return 0;
			}

			glAttachShader(program, shader); //shader绑定到program
			++entry;
		}

		glLinkProgram(program);  //链接program
		GLint linked;			 //获取链接情况
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (!linked)
		{
#ifdef _DEBUG
			GLsizei len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(program, len, &len, log);
			std::cerr << "Shader linking failed: " << log << std::endl;
			delete[] log;
#endif // _DEBUG

			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			return 0;
		}

		return program;
	}


#ifdef __cplusplus
};
#endif // __cplusplus