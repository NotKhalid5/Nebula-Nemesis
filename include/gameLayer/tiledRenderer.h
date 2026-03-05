#pragma once
#include <gl2d/gl2d.h>

struct TiledRenderer {
    float backgroundSize = 1000;
    gl2d::Texture texture; // texture stored as a member to used isn other game files
    
    float paralaxStrength = 1;

    void render(gl2d::Renderer2D &renderer); // ts method takes renderer as a reference
};