#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
    unsigned int ID;
    
    Texture(const char* path);
    ~Texture();
    
    void bind(unsigned int slot = 0) const;
    
    void unbind() const;
    
    int width, height;
};

#endif