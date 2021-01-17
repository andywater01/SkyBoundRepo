#include "TextureImage.h"

#include "stb_image.h"

//Referenced from Texture.cpp in nou
TextureImage::Image2D::Image2D(const std::string& filename, bool useNearest)
{
	int m_Channels;
	m_width = 0, m_height = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename.c_str(),
		&m_width, &m_height, &m_Channels, STBI_rgb_alpha);


	glGenTextures(1, &m_id);
	
	glBindTexture(GL_TEXTURE_2D, m_id);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	if (useNearest)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	
	stbi_image_free(data);
}

TextureImage::Image2D::~Image2D()
{
	glDeleteTextures(1, &m_id);
}

GLuint TextureImage::Image2D::GetID() const
{
	return m_id;
}

void TextureImage::Image2D::GetDimensions(int& Thewidth, int& Theheight) const
{
	Thewidth = m_width;
	Theheight = m_height;
}
