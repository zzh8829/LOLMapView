#ifndef Z_VERTEXBUFFER_H_
#define Z_VERTEXBUFFER_H_

#include "Core.h"
#include "GL/glew.h"

class VertexBuffer
{
public:
	VertexBuffer(GLenum target,int numVertices, int);
	virtual ~VertexBuffer();

	void hehe(GLenum target,const void *data, GLsizei size)
	{
		glGenBuffers(1,&buffer_id_);
		glBindBuffer(target,buffer_id_);
		glBufferData(target,size,data,GL_STATIC_DRAW);
	}

private:
	GLuint buffer_id_;
};

#endif
