#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <vector>
#include <string>

class Texture
{
	public:
		unsigned int ID;

		Texture(const char* filepath, bool flip_vertically);

		static GLuint loadCubemap(std::vector<std::string>& faces);

	private:

		void load_texture(const char* filepath, bool flip_vertically);

		int width;
		int height;
		int channels;
};

#endif