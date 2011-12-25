#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class tgaImageFile
{
public:

    tgaImageFile(): m_texFormat(-1),  m_nImageWidth(0), m_nImageHeight(0), m_nImageBits(0),  m_nImageData(NULL) {}
    ~tgaImageFile();

    enum TGALoadError
    {
        TGA_NO_ERROR = 1,   // No error
        TGA_FILE_NOT_FOUND, // File was not found 
        TGA_BAD_IMAGE_TYPE, // Color mapped image or image is not uncompressed
        TGA_BAD_DIMENSION,  // Dimension is not a power of 2 
        TGA_BAD_BITS,       // Image bits is not 8, 24 or 32 
        TGA_BAD_DATA        // Image data could not be loaded 
    };

    tgaImageFile::TGALoadError load( char *name );

    GLenum m_texFormat;
    int    m_nImageWidth;
    int    m_nImageHeight;
    int    m_nImageBits;
    unsigned char * m_nImageData;
    
private:

    bool checkSize(int x);
    int returnError(FILE *s, int error);
    unsigned char *getRGBA(FILE *s, int size);
    unsigned char *getRGB(FILE *s, int size);
    unsigned char *getGray(FILE *s, int size);
};

class Texture{
    protected:
        unsigned int textureID;
        void loadTGA(const char* textureFilepath);
    
	public:
        Texture(const char* textureFilepath); //must be called by the thread that owns the rendering context or it will be ignored
        ~Texture(); //must be called by the thread that owns the rendering context or it will be ignored
        void bind();
};