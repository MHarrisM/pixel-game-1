#pragma once
#include <gl2d/gl2d.h>

struct Enemy {

	glm::uvec2 type = {}; // for selecting textures, since we can change it
	glm::vec2 position = {};
	glm::vec2 viewDirection = { 1,0 };
	float enemyShipSize = 150.0f;
	float turnSpeed;
	float speed;
	float firedTime;
	float fireTimeReset;
	float fireRange;
	void render(
		gl2d::Renderer2D& renderer,
		gl2d::Texture &sprites, 
		gl2d::TextureAtlasPadding &atlas);

	bool update(float deltaTime, glm::vec2 playerPos);
};

