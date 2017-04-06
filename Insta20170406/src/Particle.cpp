#include "Particle.h"

Particle::Particle()
{
	this->location = ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1));
	//this->location = ofVec2f(0, 0);
	this->velocity = ofVec2f(0, 0);
	this->acceleration = ofVec2f(0, 0);

	this->max_speed = 50;
	this->max_force = 15;
	this->radius = 30;
	this->angle = ofRandom(360);

	float body_value = ofRandom(255);
	this->body_color.setHsb(body_value, 255, 255, 255);
	this->body_size = 20;
}

Particle::~Particle()
{

}

void Particle::applyForce(ofVec2f force)
{
	this->acceleration += force;
}

void Particle::seek(ofVec2f target)
{
	ofVec2f desired = this->location - target;
	float distance = desired.length();
	desired.normalize();
	if (distance < this->radius)
	{
		float m = ofMap(distance, 0, this->radius, 0, this->max_speed);
		desired *= m;
	}
	else
	{
		desired *= this->max_speed;
	}

	ofVec2f steer = this->velocity - desired;
	steer.limit(this->max_force);
	applyForce(steer);
}

void Particle::update()
{
	this->future.set(this->velocity);
	this->future.normalize();
	this->future *= 60;
	this->future += this->location;

	this->angle = ofRandom(0, 360);
	float x = 60 * cos(this->angle * DEG_TO_RAD) + future.x;
	float y = 60 * sin(this->angle * DEG_TO_RAD) + future.y;
	this->target = ofVec2f(x, y);

	this->seek(this->target);

	this->velocity += this->acceleration;
	this->velocity.limit(max_speed);
	this->location += this->velocity;
	
	this->acceleration *= 0;
	this->velocity *= 0.98;

	this->log.push_back(this->location);
	if (this->log.size() > 20)
	{
		this->log.erase(this->log.begin());
	}
}

void Particle::draw()
{
	ofPath line = ofPath();
	line.setStrokeColor(this->body_color);
	line.setStrokeWidth(1);
	line.setFilled(false);

	for (auto itr = log.begin(); itr != log.end(); ++itr)
	{
		if (itr == log.begin())
		{
			line.moveTo(itr->x, itr->y);
		}
		else
		{
			line.lineTo(itr->x, itr->y);
		}
	}

	line.draw();
	line.close();
}