#include <tiledRenderer.h>

void TiledRenderer::render(gl2d::Renderer2D &renderer) {

    auto viewRect = renderer.getViewRect(); // luk what we can c

    glm::vec2 paralaxDistance = {viewRect.x, viewRect.y}; // takes curr plyr pos
    paralaxDistance *= -paralaxStrength;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            // using these, each time the plyr shift past a background all of them shift by one
            int xPos = x + int((viewRect.x - paralaxDistance.x) / backgroundSize);
            int yPos = y + int((viewRect.y - paralaxDistance.y) / backgroundSize); 

            renderer.renderRectangle(
                glm::vec4{xPos, yPos, 1, 1} * backgroundSize
                + glm::vec4(paralaxDistance, 0, 0), texture); // add val 2 rendering call so bg moves slower
        }
    }
	
	// renderer.renderRectangle({0, 0, 10000, 10000}, texture);
};

void renderSpaceShip (
        gl2d::Renderer2D &renderer,
        glm::vec2 position, 
        float size,
        gl2d::Texture texture, 
        glm::vec4 uvs, 
        glm::vec2 viewDirection) {
            
            float spaceShipAngle = atan2(viewDirection.y, -viewDirection.x);

            renderer.renderRectangle(
                {position - glm::vec2(size / 2, size / 2), size, size}, 
                texture, 
                Colors_White, 
                {}, 
                glm::degrees(spaceShipAngle) + 90.f,
                uvs
            );
}