#ifndef COMPUTE_CLASS_H
#define COMPUTE_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Compute
{
    public:
        GLuint ID;
        Compute(const char* computeFile);

        void Activate();
        void Dispatch(int workGroupX, int workGroupY, int workGroupZ);
        void Delete();
    private:
        void compileErrors(unsigned int shader, const char* type);
};

#endif