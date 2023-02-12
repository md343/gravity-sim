#include"computeClass.h"
#include"ShaderClass.h"
#include<iostream>

using namespace std;

Compute::Compute(const char* computeFile)
{
    //cout << computeFile << endl;
    std::string computeCode = get_file_contents(computeFile);
    const char* computeSource = computeCode.c_str();
    
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeSource, NULL);
	glCompileShader(computeShader);
    compileErrors(computeShader, "COMPUTE");

    ID = glCreateProgram();
    glAttachShader(ID, computeShader);
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");
    glDeleteShader(computeShader);
    
}

void Compute::Activate()
{
    glUseProgram(ID);
}

void Compute::Dispatch(int workGroupX, int workGroupY, int workGroupZ)
{
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
    glDispatchCompute(workGroupX, workGroupY, workGroupZ);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
}

void Compute::Delete()
{
    glDeleteProgram(ID);
}

void Compute::compileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;

        }
    }

}
