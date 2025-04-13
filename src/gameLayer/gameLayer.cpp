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
#include <enemy.h>
//YTU: Keep gameplay info in one Struct
struct GameplayData
{
	glm::vec2 playerPos = {10, 10};
	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
};
GameplayData data;
constexpr int BACKGROUNDS = 3;


gl2d::Renderer2D renderer;
gl2d::Texture spaceShipTexture;
gl2d::Texture bulletTexture;
gl2d::TextureAtlasPadding spaceShipsAtlas;
gl2d::TextureAtlasPadding bulletAtlas;
gl2d::Texture backgroundTexture[BACKGROUNDS];
TiledRenderer tiledRenderer[BACKGROUNDS];


void restartGame() {

}




bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	//spaceShipTexture.loadFromFile(RESOURCES_PATH "spaceShip/ships/green.png", true);
	//Use atlas to pick ship textures (withpixelpadding is YTU custom function to add pixel padding to prevent leaking from neighbouring textures in atlas)
	spaceShipTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/spaceships.png", 128,true);
	spaceShipsAtlas = gl2d::TextureAtlasPadding(5,2,spaceShipTexture.GetSize().x, spaceShipTexture.GetSize().y);

	bulletTexture.loadFromFileWithPixelPadding(RESOURCES_PATH "spaceShip/stitchedFiles/projectiles.png", 500,true);
	bulletAtlas = gl2d::TextureAtlasPadding(3, 2, bulletTexture.GetSize().x, bulletTexture.GetSize().y);

	backgroundTexture[0].loadFromFile(RESOURCES_PATH "background1.png", true);
	backgroundTexture[1].loadFromFile(RESOURCES_PATH "background4.png", true);
	backgroundTexture[2].loadFromFile(RESOURCES_PATH "background3.png", true);
	tiledRenderer[0].texture = backgroundTexture[0];
	tiledRenderer[1].texture = backgroundTexture[1];
	tiledRenderer[2].texture = backgroundTexture[2];
	tiledRenderer[0].paralaxStrength = 0;
	tiledRenderer[1].paralaxStrength = 0.5;
	tiledRenderer[2].paralaxStrength = 0.7;
	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion


#pragma region movement
	glm::vec2 move = {};
	if (
		platform::isButtonHeld(platform::Button::W) ||
		platform::isButtonHeld(platform::Button::Up)
		)
	{
		move.y = -1;
	}
	if (
		platform::isButtonHeld(platform::Button::S) ||
		platform::isButtonHeld(platform::Button::Down)
		)
	{
		move.y = 1;
	}
	if (
		platform::isButtonHeld(platform::Button::A) ||
		platform::isButtonHeld(platform::Button::Left)
		)
	{
		move.x = -1;
	}
	if (
		platform::isButtonHeld(platform::Button::D) ||
		platform::isButtonHeld(platform::Button::Right)
		)
	{
		move.x = 1;
	}

	//Normalize movement vector
	if (move.x != 0 || move.y != 0)
	{
		move = glm::normalize(move);
		move *= deltaTime * 1700;	//200 pixels/second (deltaTime is the time since that last frame in seconds)
		data.playerPos += move;
	}

#pragma endregion

#pragma region follow
	renderer.currentCamera.follow(data.playerPos, deltaTime * 1700, 10, 200, w, h);
#pragma endregion

#pragma region render background

	renderer.currentCamera.zoom = 0.5f;

	for (int i= 0; i < BACKGROUNDS; i++) 
	{
		tiledRenderer[i].render(renderer);
	}
	


#pragma endregion



#pragma	region mouse pos
	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);

	glm::vec2 mouseDirection = mousePos - screenCenter;

	if(glm::length(mouseDirection) == 0.f)
	{
		mouseDirection = { 1,0 };
	}
	else {
		mouseDirection = normalize(mouseDirection);
	}
	float spaceShipAngle = atan2(mouseDirection.y, -mouseDirection.x);
#pragma endregion



#pragma region bullet logic
	if (platform::isLMousePressed())
	{
		
		Bullet b;
		b.position = data.playerPos;
		b.fireDirection = mouseDirection;
		b.speed = 2000.0f;
		data.bullets.push_back(b);
	}

	for (int i = 0; i < data.bullets.size(); i++) {
		if (glm::distance(data.bullets[i].position, data.playerPos) > 5'000) {
			data.bullets.erase(data.bullets.begin() + i);
			i--;
			continue;
		}
		data.bullets[i].update(deltaTime);

	}

	
#pragma endregion
	 


#pragma region renderbullets
	for (auto& b : data.bullets)
	{
		b.render(renderer, bulletTexture, bulletAtlas);
	}
#pragma endregion


#pragma region enemy logic
	for (int i = 0; i < data.enemies.size(); i++) {

		if (glm::distance(data.playerPos, data.enemies[i].position) > 4000.0f) {
			data.enemies.erase(data.enemies.begin() + i);
			i--;
			continue;
		}
		if (data.enemies[i].update(deltaTime, data.playerPos))
		{
			Bullet b;
			b.position = data.enemies[i].position;
			b.fireDirection = data.enemies[i].viewDirection;
			b.speed = 2000.0f;
			data.bullets.push_back(b);
		}
		data.enemies[i].update(deltaTime, data.playerPos);
	}


#pragma endregion


#pragma region render enemies
	for (auto& e : data.enemies) {
		e.render(renderer, spaceShipTexture, spaceShipsAtlas);
	}


#pragma endregion


#pragma region rendership
	constexpr float shipSize = 100.0f;
	
	renderSpaceShip(renderer, data.playerPos, shipSize, spaceShipTexture, spaceShipsAtlas.get(4, 0), mouseDirection);
#pragma endregion
	

	


	renderer.flush();


	//ImGui::ShowDemoWindow();
	ImGui::Begin("debug");

	ImGui::Text("Bullet Count: %d", (int)data.bullets.size());
	ImGui::Text("Bullet Enemies: %d", (int)data.enemies.size());
	if (ImGui::Button("Spawn Enemy")) {
		glm::uvec2 shipTypes[] = { {0,0},{0,1},{2,0} ,{3,1} };
		Enemy e;
		e.position = data.playerPos;
		e.speed = 100 + rand() % 1000;			//Change speed logic boooo
		e.turnSpeed = 2.0f + (rand() % 1000) / 500.0f;
		e.type = shipTypes[rand() % 4];
		e.fireRange = 1.5 + (rand() % 1000) / 2000.0f;
		e.fireTimeReset = 0.1 + (rand() % 1000) / 500;
		data.enemies.push_back(e);
	}
	ImGui::End();

	return true;


}

//This function might not be be called if the program is forced closed
void closeGame()
{



}
