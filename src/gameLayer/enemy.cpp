#include <enemy.h>
#include <tiledRenderer.h>

constexpr float ENEMY_SIZE = 200.0f;

void Enemy::render(
    gl2d::Renderer2D &renderer,
    gl2d::Texture &sprites, 
    gl2d::TextureAtlasPadding &atlas) {
        glm::vec4 uvs = atlas.get(type.x, type.y);

        renderSpaceShip(renderer, position, ENEMY_SIZE, sprites, uvs, viewDirection);
}