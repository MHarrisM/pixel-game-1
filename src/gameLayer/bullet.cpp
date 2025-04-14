#include <bullet.h>

void Bullet::render(gl2d::Renderer2D& renderer, gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding bulletsAtlas)
{

	float bulletAngle = atan2(fireDirection.y, -fireDirection.x);
	bulletAngle = glm::degrees(bulletAngle) + 90.0f;
	glm::vec4 textureCoords = bulletsAtlas.get(1, 0);
	glm::vec4 color;
	if (isEnemy) {
		textureCoords = bulletsAtlas.get(0, 0);
	}

	if (type == 0) {

		for (int i = 0; i < 5; i++) {
			color = glm::vec4(
				1 * (i + 4) / 5.0f,
				1 * (i + 4) / 5.0f,
				1 * (i + 4) / 5.0f,
				(i + 1) / 5.0f);
			renderer.renderRectangle({ position + glm::vec2(50,50) + (float)i * 25.0f * fireDirection, 100,100 }, bulletsTexture, color, {}, bulletAngle, textureCoords);
		}
	}
	
	else if (type == 1) {
		textureCoords = bulletsAtlas.get(2, 1);
		for (int i = 0; i < 60; i++) {
			color = glm::vec4(
				1 * (i + 4) / 5.0f,
				1 * (i + 4) / 5.0f,
				1 * (i + 4) / 5.0f,
				(i + 1) / 5.0f);
			renderer.renderRectangle({ position + glm::vec2(50,50) + (float)i * 25.0f * fireDirection, 100,100 }, bulletsTexture, color, {}, bulletAngle, textureCoords);
		}
	}
	else if (type == 2) {

	}
	else if (type == 3) {

	}


	
	
}

void Bullet::update(float deltaTime)
{
	position += fireDirection * deltaTime * speed;
}