#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include<glad/glad.h>

class SSBO
{
    public:
        GLuint ID;
        unsigned int num, size;
        SSBO(unsigned int numAgents, unsigned int sizeAgents);

        void AgentGen();//struct pos *points);
        void Bind(int layout);
        void Unbind(int layout);
        void Delete();
};

#endif
