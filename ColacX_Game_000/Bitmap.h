//32bit RGBA bitmap

#pragma once

class Bitmap{

private:
    unsigned int width;
    unsigned int height;
    unsigned int* data;

    unsigned int textureID;
    unsigned int textureSize;

public:
    Bitmap( unsigned int* data, unsigned int width, unsigned int height );
    Bitmap( const char* filepath );
    ~Bitmap();
    unsigned int* getData() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getPixel( unsigned int x, unsigned int y ) const;
    void generateTexture();
    void deleteTexture();
	void bind();
    void draw();
    void getBitmapDataFromFile( char*& datapointer, unsigned int& width, unsigned int& height, const char* filepath );
    void convertBGRAtoRGBA( char* datapointer, unsigned int w, unsigned int h );
    void getBitmapDataFromClipboard( char*& datapointer, unsigned int& width, unsigned int& height );
};