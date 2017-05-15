#ifndef __LOAD_SHADER_H__
#define __LOAD_SHADER_H__

//////////////////////////////////////////////////////////////////////////
// C style
//////////////////////////////////////////////////////////////////////////

#include <GL\glew.h>

#define BUFFER_OFFSET(a) ((void*)(a))

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

	typedef struct
	{
		GLenum type;
		const char* fileName;
		GLuint shader;
	}ShaderInfo;

	GLuint LoadShader(ShaderInfo*);

#ifdef __cplusplus
};
#endif // __cplusplus


#endif