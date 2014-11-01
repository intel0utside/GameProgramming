#include <irrlicht.h>
#include <string>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Player{
	IrrlichtDevice* device =
	createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
	false, false, false, 0);

	IVideoDriver* driver = device->getVideoDriver();

	ISceneManager* smgr = device->getSceneManager();

	IAnimatedMesh* mesh;
	ITexture* texture;

	core::vector3df nodePosition;

	stringw playername;
	int hp;
	float posX;
	float posY;
	float posZ;
	vector3df position;

public:
	Player(stringw playername, int hp, float posX, float posY, float posZ);
	void moveTo(vector3df position);
	void resetPosition();
	void increaseHP(int hp);
	void decreaseHP(int hp);
	stringw getName();
	int getHP();
	vector3df getPosition();
	IAnimatedMesh* getMesh();
	ITexture* getTexture();
	void setPosX(float posX);
	void setPosY(float posY);
	void setPosZ(float posZ);
	float getPosX();
	float getPosY();
	float getPosZ();
};

Player::Player(stringw playername, int hp, float posX, float posY, float posZ){
	Player::playername = playername;
	Player::hp = hp;
	Player::posX = posX;
	Player::posY = posY;
	Player::posZ = posZ;
	Player::position = vector3df(posX,posY,posZ);
}

void Player::moveTo(vector3df position){
	Player::getPosition() += position;
}

vector3df Player::getPosition(){
	return Player::position;
}

void Player::increaseHP(int hp){
	Player::hp += hp;
}
void Player::decreaseHP(int hp){
	Player::hp -= hp;
}
stringw Player::getName(){
	return Player::playername;
}

int Player::getHP(){
	return Player::hp;
}

IAnimatedMesh* Player::getMesh(){
	mesh = smgr->getMesh("D:/irrlicht-1.8.1/media/sydney.md2");
	return mesh;
}

ITexture* Player::getTexture(){
	texture = driver->getTexture("D:/irrlicht-1.8.1/media/sydney.bmp");
	return texture;
}
void Player::setPosX(float posX){
	Player::posX = posX;
	Player::position.set(Player::posX, Player::posY, Player::posZ);
}
void Player::setPosY(float posY){
	Player::posY = posY;
	Player::position.set(Player::posX, Player::posY, Player::posZ);
}
void Player::setPosZ(float posZ){
	Player::posZ = posZ;
	Player::position.set(Player::posX, Player::posY, Player::posZ);
}
float Player::getPosX(){
	return Player::posX;
}
float Player::getPosY(){
	return Player::posY;
}
float Player::getPosZ(){
	return Player::posZ;
}
