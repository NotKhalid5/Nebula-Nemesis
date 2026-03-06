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
#include <bullet.h>
#include <vector>


struct GameplayData { // place gameplay inside a struct so easier to reset the game correctly
	glm::vec2 playerPos = {100, 100};

	std::vector<Bullet> bullets;  
};

GameplayData data;

gl2d::Renderer2D renderer;

constexpr int BACKGROUNDS = 4;

// declare space ship texture and atlas
gl2d::Texture spaceShipsTexture;
gl2d::TextureAtlasPadding spaceShipsAtlas;

// declare bullet texture and atlas
gl2d::Texture bulletsTexture;
gl2d::TextureAtlasPadding bulletsAtlas;
// gl2d::Texture spaceShipTexture;

gl2d::Texture backgroundTexture[BACKGROUNDS]; // subsequently make an arr of Texture obj 2, 2 correspond w/ TR objs
TiledRenderer tiledRenderer[BACKGROUNDS]; // 2 implement paralax, created mult TR insts n n array 

bool initGame() {
	// initializing renderer
	gl2d::init();
	renderer.create();

	// ships texture atlas
	spaceShipsTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png", 128, true);
	spaceShipsAtlas = gl2d::TextureAtlasPadding(5, 2, spaceShipsTexture.GetSize().x, spaceShipsTexture.GetSize().y);

	// bullets texture atlas
	bulletsTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/projectiles.png", 500, true);
	bulletsAtlas = gl2d::TextureAtlasPadding(3, 2, bulletsTexture.GetSize().x, bulletsTexture.GetSize().y);

	// spaceShipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true); // path specific; true param tells gpu u wanna keep the texture pixelated
	backgroundTexture[0].loadFromFile(RESOURCES_PATH "background1.png", true);
	backgroundTexture[1].loadFromFile(RESOURCES_PATH "background2.png", true);
	backgroundTexture[2].loadFromFile(RESOURCES_PATH "background3.png", true);
	backgroundTexture[3].loadFromFile(RESOURCES_PATH "background4.png", true);
	
	// set TR insts textures
	tiledRenderer[0].texture = backgroundTexture[0]; 
	tiledRenderer[1].texture = backgroundTexture[1];
	tiledRenderer[2].texture = backgroundTexture[2];
	tiledRenderer[3].texture = backgroundTexture[3];

	tiledRenderer[0].paralaxStrength = 0.0f;
    tiledRenderer[1].paralaxStrength = 0.5f;
    tiledRenderer[2].paralaxStrength = 0.7f;
	tiledRenderer[3].paralaxStrength = 0.9f;

	
	return true;
}

constexpr float shipSize = 250.f;

bool gameLogic(float deltaTime) {
#pragma region init
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


#pragma region camera follow
	renderer.currentCamera.follow(data.playerPos, deltaTime * 2000, 10, 200, w,h); // tracks plyr (I wanteed the camera speed to match the speed of the plyr's mvmnts)
#pragma endregion 


#pragma region render bg
	renderer.currentCamera.zoom = 1.1; // changes how much the cam can c

	for (int i = 0; i < BACKGROUNDS; i++) {
		tiledRenderer[i].render(renderer); 
	}
#pragma endregion


#pragma region mouse pos 
	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f); // vec pointing 2 mid of scr

	glm::vec2 mouseDirection = mousePos - screenCenter;

	if (glm::length(mouseDirection) == 0.f) {
		mouseDirection = {1, 0}; // if 0 set 2 arb dir
	}
	else {
		mouseDirection = normalize(mouseDirection);
	}

	float spaceShipAngle = atan2(mouseDirection.y, -mouseDirection.x);
#pragma endregion


#pragma region handle bullets
	if (platform::isLMousePressed()) {
		Bullet b;
		b.position = data.playerPos;
		b.fireDirection = mouseDirection;

		data.bullets.push_back(b);
	}

	for (auto &b : data.bullets) {b.update(deltaTime);}
#pragma endregion


#pragma region render ship
	renderer.renderRectangle({data.playerPos - glm::vec2(shipSize/2, shipSize/2), 200, 200}, spaceShipsTexture,
		Colors_White, {}, glm::degrees(spaceShipAngle) + 90.f, spaceShipsAtlas.get(3, 0)); // idx the right elm n the atlas
#pragma endregion
 

#pragma region render bullets
	for (auto &b : data.bullets) {b.render(renderer, bulletsTexture, bulletsAtlas);} // render all bullets using enhanced 4 loop
#pragma endregion


	renderer.flush(); // called once


	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

// ts f(x) might not b called if prgm is forced closed
void closeGame(){}
