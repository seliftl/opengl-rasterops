#include "texture.h"
// CS. Added.
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "glutils.h"

/*static*/
GLuint Texture::loadTexture( const std::string & fName ) {
    int width, height;
    unsigned char * data = Texture::loadPixels(fName, width, height);
	GLuint tex = 0;
    if( data != nullptr ) {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
        Texture::deletePixels(data);
    }

    return tex;
}

void Texture::deletePixels(unsigned char *data) {
    stbi_image_free(data);
}

unsigned char *Texture::loadPixels(const std::string &fName, int & width, int & height, bool flip) {
    int bytesPerPix;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(fName.c_str(), &width, &height, &bytesPerPix, 4);
    return data;
}


