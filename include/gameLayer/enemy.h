#pragma once
#include <gl2d/gl2d.h>

struct Enemy {
    glm::uvec2 type = {}; // idx n2 txtr atlas
    glm::vec2 position = {};
    glm::vec2 viewDirection = {1, 0};

    void render(gl2d::Renderer2D &renderer, gl2d::Texture &texture, gl2d::TextureAtlasPadding &atlas);
};  