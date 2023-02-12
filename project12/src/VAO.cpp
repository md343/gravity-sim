#include"include/VAO.h"
#include"include/EBO.h"

VAO::VAO()
{
    glCreateVertexArrays(1, &ID);
}

void VAO::LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, unsigned long offset)
{
    glEnableVertexArrayAttrib(ID, layout);
	glVertexArrayAttribBinding(ID, layout, 0);
	glVertexArrayAttribFormat(ID, layout, numComponents, type, GL_FALSE, offset);
    // VBO.Bind();
    // glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);

    // glEnableVertexAttribArray(layout);
    // VBO.Unbind();

}

void VAO::LinkVertBuffer(VBO& VBO, unsigned long offset)
{
    glVertexArrayVertexBuffer(ID, 0, VBO.ID, 0, offset);
}

void VAO::LinkElemBuffer(EBO& EBO)
{
    glVertexArrayElementBuffer(ID, EBO.ID);
}

void VAO::Bind()
{
    glBindVertexArray(ID);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete()
{
   glDeleteVertexArrays(1, &ID);
}