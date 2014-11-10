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
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
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
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
enum{
	ID_USER_REGISTER = ID_USER_PACKET_ENUM + 1,
	ID_USER_LOGIN = ID_USER_REGISTER + 1,
	ID_PLAYER_COUNT = ID_USER_LOGIN + 1,
	ID_PLAYER_POSITION = ID_PLAYER_COUNT + 1,

	WORLD_UPDATE = ID_PLAYER_POSITION + 1,
	PLAYER_MOVELEFT = WORLD_UPDATE + 1,
	PLAYER_MOVERIGHT = PLAYER_MOVELEFT +1,
	PLAYER_MOVEFWD = PLAYER_MOVERIGHT + 1,
	PLAYER_MOVEBWD = PLAYER_MOVEFWD + 1,
	PLAYER_JUMP = PLAYER_MOVEBWD + 1
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

		guienv->addEditBox(L"", rect<s32>(20, 300, 360, 320), true, 0, 1);

		ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
		
		
		std::string playerstate = "idle";
		stringw greeting = L"WELCOME TO THE WORLD OF IRRLICHT 3D ";
		stringc playerChat;
		int otherPlayerNum = 0;
		core::vector3df nodePosition;
		int playerCount = 0;
		int x2 = 100;
		int y2 = 120;
		char command[3];
		command[0] = WORLD_UPDATE;
		command[1] = '\0';
		char playerPos[2048];
		// In order to do framerate independent movement, we have to know
		// how long it was since the last frame
		u32 then = device->getTimer()->getTime();
		const f32 MOVEMENT_SPEED = 100.f;

		//device->getCursorControl()->setVisible(false);
		while (device->run())
		{
			driver->beginScene(true, true, SColor(255, 100, 101, 140));
			// Work out a frame delta time.
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
					char playercount[2048];
					strcpy(playercount, packetDataS.c_str());
					char *pch;
					std::string playerCountS;
					pch = strtok(playercount, " ");
					playerCountS = pch;
					playerCount = atoi(packetDataS.c_str());
					printf("THE PLAYER COUNT  %d\n", playerCount);
					printf("THE PLAYER COUNT  %d\n", atoi(playerCountS.c_str()));
					guienv->addStaticText(packetDataW.c_str(),
						rect<s32>(20, x2, 360, y2), true, false, 0, 2);
					x2 += 20;
					y2 += 20;
				}
					break;
				case ID_USER_LOGIN:
					{
						char userinfo[2048];
						strcpy(userinfo, packetDataS.c_str());
						char *pch;
						pch = strtok(userinfo, ",");
						stringw playername = pch;
						pch = strtok(NULL, ".");
						std::string posX = pch;
						pch = strtok(NULL, ".");
						std::string posY = pch;
						pch = strtok(NULL, " ");
						std::string posZ = pch;
						 guienv->addStaticText(playername.c_str(),
							 rect<s32>(20, x2, 360, y2), true, false, 0, 2);
						 x2 += 20;
						 y2 += 20;
						 int i = playerCount;
						 players[i] = new Player(packetDataW, 100, atoi(posX.c_str()),atoi(posY.c_str()),atoi(posZ.c_str()));
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
					}
					break;
				case ID_PLAYER_POSITION:
				{
					
				}
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					guienv->addStaticText(greeting.c_str(),
						rect<s32>(20, 20, 360, 40), true, false, 0, 2);
					break;
				default:
					guienv->addStaticText(packetDataW.c_str(),
						rect<s32>(20, x2, 360, y2), true, false, 0, 2);
					x2 += 20;
					y2 += 20;
					break;
				}
				guienv->drawAll();
				client->DeallocatePacket(packet);
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
