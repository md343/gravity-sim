#include"SSBO.h"



SSBO::SSBO(unsigned int numAgents, unsigned int sizeAgents)
{
    num = numAgents;
    size = sizeAgents;
    glGenBuffers(1, &ID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, num * size, NULL, GL_STATIC_DRAW);
}


void SSBO::Bind(int layout)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layout, ID);
}

void SSBO::Unbind(int layout)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layout, 0);
}

void SSBO::Delete()
{
   glDeleteBuffers(1, &ID);
}