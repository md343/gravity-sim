#ifndef VAO_CLASS_H
#define VAO_CLASS_H


#include<glad/glad.h>
#include"VBO.h"
#include"EBO.h"


class VAO
{
    public:
        GLuint ID;
        VAO();

        void LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, unsigned long offset);
        void LinkVertBuffer(VBO& VBO, unsigned long offset);
        void LinkElemBuffer(EBO& EBO);
        void Bind();
        void Unbind();
        void Delete();
};

#endif
