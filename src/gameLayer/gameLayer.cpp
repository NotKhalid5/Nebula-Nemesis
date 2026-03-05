#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>


struct GameplayData { // place gameplay inside a struct so easier to reset the game correctly
	glm::vec2 playerPos = {100, 100};
};

GameplayData data;

gl2d::Renderer2D renderer;

gl2d::Texture spaceShipTexture;
gl2d::Texture backgroundTexture;

TiledRenderer tiledRenderer; // created TitledRenderer instance

bool initGame() {
	// initializing renderer
	gl2d::init();
	renderer.create();

	spaceShipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true); // path specific; true param tells gpu u wanna keep the texture pixelated
	backgroundTexture.loadFromFile(RESOURCES_PATH "background1.png", true);
	
	tiledRenderer.texture = backgroundTexture; // set TR inst texture
	
	return true;
}



bool gameLogic(float deltaTime) {
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); // window w
	h = platform::getFrameBufferSizeY(); // window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); // clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion


#pragma region movement

	glm::vec2 move = {};

	// 4 each single gameplay input map controls
	if (
		platform::isButtonHeld(platform::Button::W) ||
		platform::isButtonHeld(platform::Button::Up)
		)
	{
		move.y = -1;
	}

	if (
		platform::isButtonHeld(platform::Button::A) ||
		platform::isButtonHeld(platform::Button::Left)
		)
	{
		move.x = -1;
	}

	if (
		platform::isButtonHeld(platform::Button::S) ||
		platform::isButtonHeld(platform::Button::Down)
		)
	{
		move.y = 1;
	}

	if (
		platform::isButtonHeld(platform::Button::D) ||
		platform::isButtonHeld(platform::Button::Right)
		)
	{
		move.x = 1;
	}
	
	if (move.x != 0 || move.y != 0) {
		move =glm::normalize(move); // normalize vector so diagonal movement doesn't exceed intention

		move *= deltaTime * 2000; // 200 px/sec (mult 2 make mvmnt constant); deltaTime is the time of the last frame n sec
		data.playerPos += move;
		
	}

#pragma endregion

#pragma region render bg
	renderer.currentCamera.zoom = 1.1; // changes how much the cam can c

	tiledRenderer.render(renderer); 

#pragma endregion

	renderer.currentCamera.follow(data.playerPos, deltaTime * 2000, 10, 200, w,h); // tracks plyr (I wanteed the camera speed to match the speed of the plyr's mvmnts)

	renderer.renderRectangle({data.playerPos, 200, 200}, spaceShipTexture);


	renderer.flush(); // called once


	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

// ts f(x) might not b called if prgm is forced closed
void closeGame(){}
