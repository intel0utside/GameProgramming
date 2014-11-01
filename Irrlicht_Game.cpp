#include <irrlicht.h>
#include <iostream>

#include "Player.h"

#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "Kbhit.h"
#include <stdio.h>
#include <string.h>
#include "Gets.h"
#include <list>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

class MyEventReceiver : public IEventReceiver
{
public:
	
	virtual bool OnEvent(const SEvent& event)
	{
		
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
enum{
	PLAYER_MOVE = 1,
	PLAYER_CHAT = 2,

	ID_USER_REGISTER = ID_USER_PACKET_ENUM + 1,
	ID_USER_LOGIN = ID_USER_REGISTER + 1,
	ID_USER_POSITION_X = ID_USER_LOGIN + 1,
	ID_USER_POSITION_Y = ID_USER_POSITION_X + 1,
	ID_USER_POSITION_Z = ID_USER_POSITION_Y + 1,
	ID_PLAYER_COUNT = ID_USER_POSITION_Z + 1,

	WORLD_UPDATE = ID_PLAYER_COUNT + 1
};
int main(){
	RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet *packet;

	char portstring[30];
	printf("Enter The Client Port\n");
	Gets(portstring, sizeof(portstring));

	RakNet::SocketDescriptor socketDescriptorClient[2];
	socketDescriptorClient[0].port = atoi(portstring);
	socketDescriptorClient[0].socketFamily = AF_INET;

	client->Startup(8, socketDescriptorClient, 1);
	
	char serverIPAddress[30];
	char serverPortString[30];
	printf("Enter Server IP Address to be connected\n");
	Gets(serverIPAddress, sizeof(serverIPAddress));
	printf("Enter Server Port to be connected\n");
	Gets(serverPortString, sizeof(serverPortString));

	RakNet::ConnectionAttemptResult car = client->Connect(serverIPAddress, atoi(serverPortString), NULL, NULL);
	bool b = car == RakNet::CONNECTION_ATTEMPT_STARTED;

	char enter[2048];
	char username[2048];
	char username2[2048];

	if (b){
		printf("Client has connected to the server. please enter your username:\n");
		Gets(username, sizeof(username));
		username2[0] = ID_USER_REGISTER;
		username2[1] = '\0';
		strcat(username2, username);
		printf("Incoming Message %d\n", username2[0]);
		client->Send(username2, (const int)strlen(username2) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		printf("Username Creation Complete. press enter to continue");
	}
	else{
		printf("Client has failed to connect to the server. press enter to continue");
		Gets(enter, sizeof(enter));
		return 0;
	}
	Gets(enter, sizeof(enter));
		MyEventReceiver receiver;

		IrrlichtDevice *device =
			createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, &receiver);


		if (!device)
			return 1;

		device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

		IVideoDriver* driver = device->getVideoDriver();
		ISceneManager* smgr = device->getSceneManager();
		IGUIEnvironment* guienv = device->getGUIEnvironment();
		
		Player* players[8];
		IAnimatedMeshSceneNode* playersnode[8];

	/*	players[0] = new Player(username, 100, 0,0,0);
		playersnode[0] = smgr->addAnimatedMeshSceneNode(players[0]->getMesh());
		if (playersnode[0])
		{
			playersnode[0]->setPosition(vector3df(players[0]->getPosition().X, players[0]->getPosition().Y, players[0]->getPosition().Z));
			playersnode[0]->setMaterialFlag(EMF_LIGHTING, false);
			playersnode[0]->setMD2Animation(scene::EMAT_STAND);
			playersnode[0]->setMaterialTexture(0, players[0]->getTexture());
		}*/
	/*	for(int i = 0; i < 3; i++){
			players[i] = new Player(username, 100, 0, 0, 0);
			playersnode[i] = smgr->addAnimatedMeshSceneNode(players[i]->getMesh());
			if (playersnode[i])
			{
				playersnode[i]->setPosition(vector3df(players[i]->getPosX() + 10*i, players[i]->getPosY(), players[i]->getPosZ()));
				playersnode[i]->setMaterialFlag(EMF_LIGHTING, false);
				playersnode[i]->setMD2Animation(scene::EMAT_STAND);
				playersnode[i]->setMaterialTexture(0, players[i]->getTexture());
			}
		}*/
		/*node[0] = smgr->addAnimatedMeshSceneNode(player->getMesh());
		if (node[0])
		{
			node[0]->setPosition(vector3df(player->getPosX(), player->getPosY(), player->getPosZ()));
			node[0]->setMaterialFlag(EMF_LIGHTING, false);
			node[0]->setMD2Animation(scene::EMAT_STAND);
			node[0]->setMaterialTexture(0, player->getTexture());
		}
		node[1] = smgr->addAnimatedMeshSceneNode(players[0]->getMesh());
		if (node[1])
		{
			node[1]->setPosition(vector3df(players[0]->getPosX(), players[0]->getPosY(), players[0]->getPosZ()));
			node[1]->setMaterialFlag(EMF_LIGHTING, false);
			node[1]->setMD2Animation(scene::EMAT_STAND);
			node[1]->setMaterialTexture(0, players[0]->getTexture());
		}
		*/
		guienv->addEditBox(L"", rect<s32>(20, 300, 360, 320), true, 0, 1);

		ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
		
		
		int playerstate = PLAYER_MOVE;
		stringw greeting = L"WELCOME TO THE WORLD OF IRRLICHT 3D ";
		stringw playerLoginText = L" has connected";
		stringc playerChat;
		int otherPlayerNum = 0;
		core::vector3df nodePosition;
		int playerCount = 0;
		int x2 = 100;
		int y2 = 120;
		char command[3];
		command[0] = WORLD_UPDATE;
		command[1] = '\0';
		char positionXChar[10];
		char positionYChar[10];
		char positionZChar[10];
		char positionX[10];
		char positionY[10];
		char positionZ[10];
		positionX[0] = ID_USER_POSITION_X;
		positionX[1] = '\0';
		positionY[0] = ID_USER_POSITION_Y;
		positionY[1] = '\0';
		positionZ[0] = ID_USER_POSITION_Z;
		positionZ[1] = '\0';
		
		u32 then = device->getTimer()->getTime();
		const f32 MOVEMENT_SPEED = 100.f;

		//device->getCursorControl()->setVisible(false);
		while (device->run())
		{
			driver->beginScene(true, true, SColor(255, 100, 101, 140));
			
			const u32 now = device->getTimer()->getTime();
			const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
			then = now;
			
			packet = client->Receive();
			
			if (packet){
				stringw packetDataW(packet->data, packet->length);
				std::string packetDataS(reinterpret_cast<char*>(packet->data));
				switch (packet->data[0]){
				case ID_PLAYER_COUNT:
				{
					//std::string playerCountS(reinterpret_cast<char*>(packet->data[1]));
					playerCount = atoi(packetDataS.c_str());
					/*printf(reinterpret_cast<char*>(packet->data[0]));
					printf(reinterpret_cast<char*>(packet->data[1]));
					printf(reinterpret_cast<char*>(packet->data[2]));*/
					printf("THE PLAYER COUNT  %d", playerCount);
					guienv->addStaticText(packetDataW.c_str(),
						rect<s32>(20, x2, 360, y2), true, false, 0, 2);
					x2 += 20;
					y2 += 20;
				}
					break;
				case ID_USER_LOGIN:
					{
						 stringw playername = packetDataW;
						 stringw greetingConnected = L" has connected";
						 playername += greetingConnected;
						 guienv->addStaticText(playername.c_str(),
							 rect<s32>(20, x2, 360, y2), true, false, 0, 2);
						 x2 += 20;
						 y2 += 20;
						//for (int i = 0; i < 3; i++){
							//for (int j = 0; j < 3; j++){
								 /*if (players[i]->getName().equals_ignore_case(playername)){
									 guienv->addStaticText(L"You have connected",
										 rect<s32>(20, x2, 360, y2), true, false, 0, 2);
									 x2 += 20;
									 y2 += 20;
								 }*/
								 //else{
								//	 if (players[i] == NULL && playersnode[j] == NULL){
						 int i = playerCount;
										 players[i] = new Player(packetDataW, 100, 0,0,0);
										 playersnode[i] = smgr->addAnimatedMeshSceneNode(players[i]->getMesh());
										 if (playersnode[i])
										 {
											 playersnode[i]->setPosition(vector3df(players[i]->getPosition().X, players[i]->getPosition().Y, players[i]->getPosition().Z));
											 playersnode[i]->setMaterialFlag(EMF_LIGHTING, false);
											 playersnode[i]->setMD2Animation(scene::EMAT_STAND);
											 playersnode[i]->setMaterialTexture(0, players[i]->getTexture());
										 }
										 guienv->addStaticText(playername.c_str(),
											 rect<s32>(20, x2, 360, y2), true, false, 0, 2);
										 x2 += 20;
										 y2 += 20;
									// }
								 //}
							 //}
						 //}
					}
					break;
				case ID_USER_POSITION_X:
					break;
				case ID_USER_POSITION_Y:
					break;
				case ID_USER_POSITION_Z:
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					guienv->addStaticText(greeting.c_str(),
						rect<s32>(20, 20, 360, 40), true, false, 0, 2);
					break;
				default:
					playerCount = atoi(packetDataS.c_str());
					guienv->addStaticText(packetDataW.c_str(),
						rect<s32>(20, x2, 360, y2), true, false, 0, 2);
					x2 += 20;
					y2 += 20;
					break;
				}
				guienv->drawAll();
				client->DeallocatePacket(packet);
			}

			for (int i = 0; i < playerCount; i++){
				strcat(positionX, _itoa(players[i]->getPosX(), positionXChar, 10));
				strcat(positionY, _itoa(players[i]->getPosY(), positionYChar, 10));
				strcat(positionZ, _itoa(players[i]->getPosZ(), positionZChar, 10));
				client->Send(positionX, (const int)strlen(positionX) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				client->Send(positionY, (const int)strlen(positionX) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				client->Send(positionZ, (const int)strlen(positionX) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}

			/*nodePosition = playersnode[0]->getPosition();
			if (receiver.IsKeyDown(irr::KEY_KEY_W) || receiver.IsKeyDown(irr::KEY_KEY_A) || receiver.IsKeyDown(irr::KEY_KEY_S) || receiver.IsKeyDown(irr::KEY_KEY_D)){
				playersnode[0]->setMD2Animation(scene::EMAT_RUN);
				if (receiver.IsKeyDown(irr::KEY_KEY_W)){
					nodePosition.Z += MOVEMENT_SPEED * frameDeltaTime;
				}
				else if (receiver.IsKeyDown(irr::KEY_KEY_S)){
					nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
				}
				if (receiver.IsKeyDown(irr::KEY_KEY_A)){
					nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
				}
				else if (receiver.IsKeyDown(irr::KEY_KEY_D)){
					nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
				}
			}*/
			if (receiver.IsKeyDown(irr::KEY_RETURN)){
				playersnode[0]->setMD2Animation(scene::EMAT_STAND);
				playerChat = guienv->getRootGUIElement()->getElementFromId(1, true)->getText();
				guienv->getRootGUIElement()->getElementFromId(1, false)->setText(L"");
				client->Send(playerChat.c_str(), (const int)strlen(playerChat.c_str()) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}
			if (receiver.IsKeyDown(irr::KEY_SPACE)){
				client->Send(command, (const int)strlen(command) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}
			/*camera->setPosition(nodePosition+vector3df(0,50,-30));
			camera->setRotation(nodePosition);*/
			/*playersnode[0]->setPosition(nodePosition);
			players[0]->setPosX(nodePosition.X);
			players[0]->setPosY(nodePosition.Y);
			players[0]->setPosZ(nodePosition.Z);*/
			if (x2 >= 280 && y2 >= 300){
				x2 = 100;
				y2 = 120;
			}
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
		}

		device->drop();
		RakNet::RakPeerInterface::DestroyInstance(client);
		return 0;
}
