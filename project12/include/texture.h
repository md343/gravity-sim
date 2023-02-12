#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>

class Texture
{
    public:
        GLuint ID;
        Texture(int width, int height);

        void Bind(int layout);
        void Delete();
        
};
#endif