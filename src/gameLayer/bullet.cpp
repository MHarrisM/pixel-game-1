#include <bullet.h>

void Bullet::render(gl2d::Renderer2D& renderer, gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding bulletsAtlas)
{

	float bulletAngle = atan2(fireDirection.y, -fireDirection.x);
	bulletAngle = glm::degrees(bulletAngle) + 90.0f;
	renderer.renderRectangle({ position + glm::vec2(0,0), 50,50 }, bulletsTexture, Colors_White, {}, bulletAngle,bulletsAtlas.get(1, 0));
	
}

void Bullet::update(float deltaTime)
{
	position += fireDirection * deltaTime * 1500.0f;
}