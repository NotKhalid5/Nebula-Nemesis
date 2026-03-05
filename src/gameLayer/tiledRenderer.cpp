#include <tiledRenderer.h>

void TiledRenderer::render(gl2d::Renderer2D &renderer) {

    auto viewRect = renderer.getViewRect(); // luk what we can c

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            // using these, each time the plyr shift past a background all of them shift by one
            int xPos = x + int(viewRect.x / backgroundSize);
            int yPos = y + int(viewRect.y / backgroundSize); 

            renderer.renderRectangle(
                glm::vec4{xPos, yPos, 1, 1} * backgroundSize, texture);
        }
    }
	
	renderer.renderRectangle({0,0, 10000, 10000}, texture);


	
};
