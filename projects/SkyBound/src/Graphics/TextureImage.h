#pragma once
#include "glad/glad.h"

#include <string>

class TextureImage
{

	class Image2D
	{
	public:

		Image2D(const std::string& filename, bool useNearest = false);
		~Image2D();

		GLuint GetID() const;
		void GetDimensions(int& width, int& height) const;

	private:

		GLuint m_id;
		int m_width;
		int m_height;
	};
	


};

