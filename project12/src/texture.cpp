#include"texture.h"

Texture::Texture(int width, int height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
	glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(ID, 1, GL_RGBA32F, width, height);
	glBindImageTexture(0, ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Texture::Bind(int layout)
{
    glBindTextureUnit(layout, ID);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}