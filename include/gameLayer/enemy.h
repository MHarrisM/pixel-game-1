#pragma once
#include <gl2d/gl2d.h>

struct Enemy {

	glm::uvec2 type = {}; // for selecting textures, since we can change it
	glm::vec2 position = {};
	glm::vec2 viewDirection = { 1,0 };
	float enemyShipSize = 150.0f;
	float turnSpeed = 3.0f;
	float speed = 1300.0f;
	float firedTime = 1.0f;
	float fireTimeReset = 1.0f;
	float fireRange = 1.5;
	float enemyHealth = 1.0f;
	void render(
		gl2d::Renderer2D& renderer,
		gl2d::Texture &sprites, 
		gl2d::TextureAtlasPadding &atlas);

	bool update(float deltaTime, glm::vec2 playerPos);
};

