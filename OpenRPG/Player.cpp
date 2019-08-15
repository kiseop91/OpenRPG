#include "Player.h"


//Initializer functions
void Player::initVariables()
{

}

void Player::initComponents()
{

}

Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();
	this->setPositions(x, y);

	this->createMovementComponent(200.f, 50.f, 10.f);
	this->createAnimationComponent(texture_sheet);

	this->animationComponent->addAnimation("IDLE_DOWN", 10.f, 0, 3, 0, 3, 128, 128);
	this->animationComponent->addAnimation("WALK_UP",    10.f, 0, 0, 3, 0, 128, 128);
	this->animationComponent->addAnimation("WALK_LEFT",  10.f, 0, 1, 3, 1, 128, 128);
	this->animationComponent->addAnimation("WALK_RIGHT", 10.f, 0, 2, 3, 2, 128, 128);
	this->animationComponent->addAnimation("WALK_DOWN",  10.f, 0, 3, 3, 3, 128, 128);
}


Player::~Player()
{

}

void Player::update(const float & dt)
{
	this->movementComponent->update(dt);


	if (this->movementComponent ->getState(IDLE))
		this->animationComponent->play("IDLE_DOWN", dt);
	else
	{
		if(this->movementComponent->getState(MOVING_RIGHT))
			this->animationComponent->play("WALK_RIGHT", dt);
		if (this->movementComponent->getState(MOVING_LEFT))
			this->animationComponent->play("WALK_LEFT", dt);
		if (this->movementComponent->getState(MOVING_UP))
			this->animationComponent->play("WALK_UP", dt);
		if (this->movementComponent->getState(MOVING_DOWN))
			this->animationComponent->play("WALK_DOWN", dt);
	}
	//this->animationComponent->play("IDLE_LEFT", dt);
}
