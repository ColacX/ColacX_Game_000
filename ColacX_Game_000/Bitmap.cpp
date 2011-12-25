#include "Bitmap.h"

#include <string>
#include <iostream>
#include <sstream>
#include <afxwin.h>
#include <atlimage.h>
#include <Gdiplusimaging.h>
#include <gdiplus.h>
#include <gl\gl.h>

Bitmap::Bitmap( unsigned int* data, unsigned int width, unsigned int height ){
    this->data = data;
    this->width = width;
    this->height = height;
}

Bitmap::Bitmap( const char* filepath ){
    char* datapointer = 0;
    getBitmapDataFromFile( datapointer, width, height, filepath );
    data = (unsigned int*)datapointer;
}

Bitmap::~Bitmap(){
    if( data != 0 ){
        delete[] data;
    }
    if( textureID != 0 ){
        deleteTexture();
    }
}

unsigned int* Bitmap::getData() const {
    return data;
}

unsigned int Bitmap::getWidth() const {
    return width;
}

unsigned int Bitmap::getHeight() const {
        return height;
    }

unsigned int Bitmap::getPixel( unsigned int x, unsigned int y ) const {
    if( x > width || y > height ){
        throw "Bitmap: out of range";
    }

    return data[ y * width + x ];
}

void Bitmap::generateTexture(){
    //create a square texture that can hold entire picture
    textureSize = 2;
    while( textureSize<width ){
        textureSize *= 2;
    }
    unsigned int* textureData = new unsigned int[ textureSize*textureSize ];
    ZeroMemory( textureData, textureSize*textureSize );

    //copy pixel data into texture data
    for( unsigned int ib=0; ib<height; ib++ ){
        for( unsigned int ia=0; ia<width; ia++ ){
            textureData[ ib*textureSize + ia ] = ((unsigned int*)data)[ ib*width + ia ];
        }
    }

    //generate opengl texture from texture data
    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureSize, textureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData );

    //delete texture data
    delete[] textureData;
}

void Bitmap::deleteTexture(){
    glDeleteTextures( 1, &textureID );
}

void Bitmap::bind(){
	glBindTexture( GL_TEXTURE_2D, textureID );
}

void Bitmap::draw(){
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, textureID );

    int w = width;
    int h = height;
    float tw = float( w )/float( textureSize );
    float th = float( h )/float( textureSize );
    
    glBegin( GL_TRIANGLES );

	glNormal3f( 0, 0, 1 );

    glTexCoord2f(  0,  0 ); glVertex2i( 0, 0 );
    glTexCoord2f( tw,  0 ); glVertex2i( w, 0 );
    glTexCoord2f( tw, th ); glVertex2i( w, h );

    glTexCoord2f( tw, th ); glVertex2i( w, h );
    glTexCoord2f(  0, th ); glVertex2i( 0, h );
    glTexCoord2f(  0, 0  ); glVertex2i( 0, 0 );
                
    glEnd();
}

void Bitmap::getBitmapDataFromFile( char*& datapointer, unsigned int& width, unsigned int& height, const char* filepath ){
    //open file
    CImage cimage;
    cimage.Load( filepath );            
    HBITMAP hbitmap = (HBITMAP)cimage;
	if( hbitmap == 0 )
	{
		throw "Bitmap: HBITMAP failed";
	}

    unsigned int w = cimage.GetWidth();
    unsigned int h = cimage.GetHeight();

    //copy bitmap bits
    unsigned int size = w * h * 4;
    char* buffer = new char[ size ];
    GetBitmapBits( hbitmap, size, buffer );

    //set return values
    datapointer = buffer;
    width = w;
    height = h;

    //auto convert to rgba
    convertBGRAtoRGBA( datapointer, w, h );
}

void Bitmap::convertBGRAtoRGBA( char* datapointer, unsigned int w, unsigned int h ){
    //convert bitmap formats from BGRA to RGBA
    for( unsigned int y=0; y<h; y++){
        for ( unsigned int x=0; x<w; x++){
                
            unsigned int offset = (y*w + x)*4;
                    
            // data in BGRA format
            char b = datapointer[offset + 0];
            //char g = datapointer[offset + 1];
            char r = datapointer[offset + 2];
            //char a = datapointer[offset + 3];

            // data in RGBA format
            datapointer[offset + 0] = r;
            //datapointer[offset + 1] = g;
            datapointer[offset + 2] = b;
            //datapointer[offset + 3] = a;
        }
    }
}

void Bitmap::getBitmapDataFromClipboard( char*& datapointer, unsigned int& width, unsigned int& height ){
    if ( !OpenClipboard( 0 ) ){
        throw "OpenClipboard failed";
    }
                    
    //Get the clipboard data
    HBITMAP hbitmap = (HBITMAP)GetClipboardData( CF_BITMAP );
    CBitmap* cbitmap = CBitmap::FromHandle( hbitmap );
    BITMAP bitmap;
    cbitmap->GetObject(sizeof(BITMAP),&bitmap);
    unsigned int w = bitmap.bmWidth;
    unsigned int h = bitmap.bmHeight;

    //copy bitmap bits
    unsigned int size = w * h * 4;
    char* buffer = new char[ size ];
    GetBitmapBits( hbitmap, size, buffer );

    CloseClipboard();

    //set return values
    datapointer = buffer;
    width = w;
    height = h;

    //auto convert to rgba
    convertBGRAtoRGBA( datapointer, w, h );
}
