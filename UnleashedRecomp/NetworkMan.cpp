#include "NetworkMan.h"
//#include "ClientInfo.h"
//#include "Configuration.h"
//#include "DebugMenu/ChatMenu.h"

namespace net
{
    
	void NetManager::ParsePackets(enet_uint8* data, size_t length)
	{
		uint8_t packetType = PacketBase::DeserializePacket(data, 2).type;

		switch (packetType)
		{
		case MOVE:
		{
			GenericPacketVec4 msg = GenericPacketVec4::Deserialize(data, length);
			auto senderId = msg.senderId;
			//Another client moved
			if (senderId != CLIENT_ID)
			{
				auto user = GetClientFromId(senderId);
				if (user != nullptr)
				{
					//Debug::Toast(format("[%s] [%d] (%.2f, %.2f, %.2f)", GetClientFromId(senderId)->GetUsername().c_str(), senderId, msg.x, msg.y, msg.z), 0);
					auto newPosition = Hedgehog::Math::CVector(msg.x, msg.y, msg.z);
					GetClientFromId(senderId)->SetPosition(newPosition);
				}
			}
			else //Self got teleported by command
			{
				auto player = SWA::Player::CPlayerSpeedContext::GetInstance();
				if (player != nullptr)
				{
					//player->m_spMatrixNode->m_Transform.SetPosition(Hedgehog::Math::CVector(msg.x, msg.y, msg.z));
					//player->m_spMatrixNode->m_Transform.UpdateMatrix();
					//player->m_spMatrixNode->NotifyChanged();
				}
			}
			break;
		}
		case ROTATE:
		{
			GenericPacketVec4 msg = GenericPacketVec4::Deserialize(data, length);
			auto senderId = msg.senderId;
			if (senderId != CLIENT_ID)
			{
				auto user = GetClientFromId(senderId);
				if (user != nullptr)
				{
					//Debug::Toast(format("[%s] [%d] ROT (%.2f, %.2f, %.2f)", GetClientFromId(senderId)->GetUsername().c_str(), senderId, msg.x, msg.y, msg.z), 0);
					auto vec = Hedgehog::Math::CVector4(msg.x, msg.y, msg.z, msg.w);
					GetClientFromId(senderId)->SetRotation(vec);
				}
			}
			break;
		}
		case CHAT:
		{
			ChatPacket msg = ChatPacket::Deserialize(data, length);
			//Will result in a new chat message (Username: Message)
			//std::string username = "";
			//if (GetClientFromId(msg.senderId))
			//{
			//	username = GetClientFromId(msg.senderId)->GetUsername();
			//}
			//m_ChatLogs.push_back(SChatLog(msg.senderId, msg.chatMessage, msg.chatType));
			//net::imgui::ChatMenu::ShowNewChatMessage();
			break;
		}
		case ANIMATE:
		{
			GenericPacketString msg = GenericPacketString::Deserialize(data, length);
			auto senderId = msg.senderId;
			if (senderId != CLIENT_ID)
			{
				auto user = GetClientFromId(senderId);
				if (user != nullptr)
				{
					//If the user exists locally, set its animation to the one from the message.
					GetClientFromId(senderId)->SetAnimation(msg.payload.c_str());
				}
			}
			break;
		}
		case RACE_UPDATE:
		{
			GenericPacketFloat msg = GenericPacketFloat::Deserialize(data, length);
			auto senderId = msg.senderId;
			auto user = GetClientFromId(senderId);

			if (user != nullptr)
			{
				user->SetStageRatio(msg.payload);
			}

			break;
		}
		case ANIM_SPEED:
		{
			GenericPacketVec4Quantized msg = GenericPacketVec4Quantized::Deserialize(data, length);
			auto senderId = msg.senderId;
			if (senderId != CLIENT_ID)
			{
				auto user = GetClientFromId(senderId);
				if (user != nullptr)
				{
					//If the user exists locally, set its animation to the one from the message.
                    auto g = msg.GetAsFloat(-100, 100);
					GetClientFromId(senderId)->SetAnimationSpeed(Hedgehog::Math::CVector(g.x, g.y, g.z));
				}
			}
			break;
		}
		case ANIM_BLEND:
		{
			GenericPacketVec4Quantized msg = GenericPacketVec4Quantized::Deserialize(data, length);
			auto senderId = msg.senderId;
			if (senderId != CLIENT_ID)
			{
				auto user = GetClientFromId(senderId);
				if (user != nullptr)
				{
					//If the user exists locally, set its animation to the one from the message.
                    auto g = msg.GetAsFloat(-100, 100);
					GetClientFromId(senderId)->SetAnimationBlend(Hedgehog::Math::CVector(g.x, g.y, g.z));
				}
			}
			break;
		}
		case CONNECT:
		{
			auto msg = GenericPacketString::Deserialize(data, length);
			auto senderId = msg.senderId;

			if (GetClientFromId(senderId))
				GetClientFromId(senderId)->SetUsername(msg.payload);

			//This check is basically useless, it's only here since there is some time
			//between when the mod initializes and when the game fully sets itself up.
			//if (Sonic::CGameDocument::GetInstance())
			//{
			//	net::mod::MultiplayerHooks::m_qNotifications.push(format("%s has joined.", msg.payload.c_str()));
			//}
			break;
		}
		case DISCONNECT:
		{
			auto msg = GenericPacketInt::Deserialize(data, length);
			if (GetClientFromId(msg.payload))
			{
				//net::mod::MultiplayerHooks::m_qNotifications.push(format("%s has left.", GetClientFromId(msg.payload)->GetUsername().c_str()));
				//Debug::Toast(format("DELETED CLIENTINFO (%d)", msg.payload), 15, 100);

				//Remove client model and nametag if they exist
				auto client = GetClientFromId(msg.payload);
				client->Destroy();


				m_ClientMap.erase(std::remove(m_ClientMap.begin(), m_ClientMap.end(), GetClientFromId(msg.payload)), m_ClientMap.end());
			}
			break;
		}
		case ID:
		{
			//Retrieve ID for our client
			auto msg = GenericPacketInt::Deserialize(data, length);
			int prevId = CLIENT_ID;
			CLIENT_ID = msg.payload;
			if (GetClientFromId(prevId) == nullptr)
			{
				m_ClientMap.push_back(new ClientInfo(CLIENT_ID));
			}
			else
			{
				GetClientFromId(prevId)->SetID(CLIENT_ID);
			}
			printf("New player ID received. %d\n", msg.payload);
			//Debug::Toast(format("New player ID received. %d", msg.payload, 5));
			isConnectedFully = true;
			break;
		}
		case CHAT_STARTED:
		{
			auto msg = GenericPacketInt::Deserialize(data, length);
			if (GetClientFromId(msg.senderId) != nullptr)
			{
				GetClientFromId(msg.senderId)->SetChatVisibility(msg.payload);
			}
			break;
		}
		case SYNC_USERS:
		{
			auto msg = SyncClientsPacket::Deserialize(data, length);

			for (size_t i = 0; i < msg.clientIds.size(); ++i)
			{
				auto data = msg.clientIds[i];
				auto client = GetClientFromId(data.m_id);
				if (client == nullptr)
				{
					m_ClientMap.push_back(new ClientInfo(data.m_id));
					client = GetClientFromId(data.m_id);
					client->SetData(data);
					//client->m_sStage =(data.m_Stage);
					//client->SetPosition(data.position);
					//client->SetRotation(data.rotation);
					//client->SetPlayerType(data.m_iPlayerType);
				}
			}
			m_MaxPlayers = msg.maximumPeersCount;
			break;
		}
		case DAMAGE:
		{
			auto msg = GenericPacketInt::Deserialize(data, length);
			if (msg.senderId != CLIENT_ID)
			{
				//auto player = Sonic::Player::CPlayerSpeedContext::GetInstance();
				//if (player)
				//{
				//	//If the player isnt in the Goal state, as attacking them might
				//	//break the results screen
				//	if (player->m_pPlayer->m_StateMachine.GetCurrentState()->m_Name != "Goal")
				//	{
				//		//Stumble if moving, damage if standing
				//		if (player->m_Velocity.norm() > 0.05f)
				//			player->ChangeState("StumbleGround");
				//		else
				//			player->ChangeState("NormalDamage");
                //
				//		//Silver sound effect (archie request)
				//		GetSelf()->PlaySoundCue("5532_damage01");
				//	}
				//}
			}
			break;
		}
		case STAGE_END:
		{
			auto msg = StageEndPacket::Deserialize(data, length);
			if (msg.senderId != CLIENT_ID)
			{
				//If the player and the person who completed the stage are in the same map
				//show a message saying that the person has completed the stage.
				//Otherwise do nothing
				//std::string stage = Sonic::CApplicationDocument::GetInstance()->m_pMember->m_spGameParameter->m_pStageParameter->TerrainArchiveName.c_str();
				//if (msg.stage == stage)
				//{
				//	float time = msg.timeMilliseconds;
				//	int millisecond = (int)(time * 100.0f) % 100;
				//	int second = (int)time % 60;
				//	int minute = (int)time / 60;
				//	auto client = GetClientFromId(msg.senderId);
				//	//net::mod::MultiplayerHooks::m_qNotifications.push(format("%s has finished! %02d:%02d.%02d", client->GetUsername().c_str(), minute, second, (int)millisecond));
				//}
			}
			break;
		}
		case STAGE_START:
		{
			auto msg = StageStartPacket::Deserialize(data, length);
			auto senderId = msg.senderId;
			if (auto client = GetClientFromId(senderId))
			{
				client->SetStage(msg.stage);
				client->SetPlayerType(msg.playerType);
			}
			break;
		}
		case KICK_REASON:
		{
			auto msg = GenericPacketString::Deserialize(data, length);
			MessageBoxA(nullptr, msg.payload.c_str(), "", MB_ICONERROR);
			exit(-1);
			break;
		}
		case COLOR:
		{
			auto msg = GenericPacketVec4::Deserialize(data, length);
			auto senderId = msg.senderId;
			auto client = GetClientFromId(senderId);
			//if (client)
			//{
			//	client->SetColor(SimpleVec4(msg.x, msg.y, msg.z, 1));
			//	if (client->m_spNetPlayerModel != nullptr)
			//	{
			//		client->m_spNetPlayerModel->ApplyColoredMat();
			//		client->m_pNameTag->Recolor();
			//	}
            //
			//}

			break;
		}
		case STEAM_ID:
		{
			auto msg = GenericPacketInt64::Deserialize(data, length);
			auto senderId = msg.senderId;
			auto client = GetClientFromId(senderId);
			if (client)
			{
				client->SetSteam(msg.payload);
			}
			break;
		}
		case SYNC_CLIENT_INFO:
		{
			auto msg = ServerSettingsPacket::Deserialize(data, length);
			//Configuration::HomingAllow = msg.DoPlayerDamageHoming;
			//Configuration::StompAllow = msg.DoPlayerDamageStomp;
			//Configuration::PushAllow = msg.DoPlayerPushBoost;

			break;
		}
		default:
		{
			printf("Unknown packet ID %d\n", packetType);
			break;
		}
		}
	}

	int NetManager::GetPing()
	{
		return m_pPeer->roundTripTime;
	}

	ClientInfo* NetManager::GetClientFromId(int in_Id)
	{
		for (size_t i = 0; i < m_ClientMap.size(); ++i)
		{
			if (m_ClientMap[i]->GetID() == in_Id)
				return m_ClientMap[i];
		}
		return nullptr;
	}

	ClientInfo* NetManager::GetSelf()
	{
		return GetClientFromId(CLIENT_ID);
	}

	uint16_t NetManager::GetMaxPlayers()
	{
		return m_MaxPlayers;
	}

	int NetManager::Initialize()
	{
		if (enet_initialize() != 0)
		{
            MessageBox(nullptr, TEXT("An error occured whilst initializing ENet."), TEXT("MOD_NAME"), MB_ICONERROR);
			return EXIT_FAILURE;
		}
		atexit(Disconnect);
		atexit(enet_deinitialize);

		//EnetHost is used for both client and server
		//client uses null for enet_host_create, while
		//the server does not.
		m_pClient = enet_host_create(NULL, 1, 1, 0, 0);

		if (m_pClient == NULL)
		{
            MessageBox(nullptr, TEXT("Unable to create ENet client."), TEXT("MOD_NAME"), MB_ICONERROR);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

	int NetManager::Connect(const char* in_Username, const char* in_Address, unsigned int in_Port)
	{
		if (isConnected)
		{
			Disconnect();
		}
		ENetAddress address;

		enet_address_set_host(&address, in_Address);
		address.port = in_Port;

		m_pPeer = enet_host_connect(m_pClient, &address, 1, 0);
		if (m_pPeer == NULL)
		{
            MessageBox(nullptr, TEXT("No available peers to initiate a connection.\n"), TEXT("MOD_NAME"), MB_ICONERROR);
			return EXIT_FAILURE;
		}

		//enet_host_service allows us to see any events that are being received by the server
		//If this returns more than 1 and its type of CONNECT, then we connected successfully
		if (enet_host_service(m_pClient, &m_Event, 5000) > 0 && m_Event.type == ENET_EVENT_TYPE_CONNECT)
		{
			printf("Connection to %s:%d success\n", in_Address, in_Port);
		}
		else
		{
			enet_peer_reset(m_pPeer);
			printf("Connection to %s:%d failed.\n", in_Address, in_Port);
			return EXIT_FAILURE;
		}

		// Send the Server our username
		SendPacketString(std::string(in_Username));
		//SendPacketVec4(Configuration::PlayerColor, COLOR);
		//CSteamID mySteamID = SteamUser()->GetSteamID();
		//SendPacketInt64(mySteamID.ConvertToUint64(), STEAM_ID);
		isConnected = true;

		AuthenticationPacket* requestPacket = new AuthenticationPacket();
		//requestPacket->modList = Configuration::ModList;
		requestPacket->version = 1;
		void* packetData = requestPacket->Serialize();
		SendPacketPayload(packetData, requestPacket->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);

		//Wait until an ID is received to avoid potential problems
		while (!isConnectedFully && isConnected)
		{
			ProcessPackets();
		}
		return 0;
	}

	void NetManager::Disconnect()
	{
		enet_peer_disconnect(m_pPeer, 0);

		while (enet_host_service(m_pClient, &m_Event, 3000) > 0)
		{
			switch (m_Event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(m_Event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnection success");
				break;
			}
		}
		isConnected = false;
		isConnectedFully = false;
		m_MaxPlayers = 0;
		return;
	}

	void NetManager::ProcessPackets()
	{
		ENetEvent event;
		while (enet_host_service(m_pClient, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				ParsePackets(event.packet->data, event.packet->dataLength); // Parse the receiving data.
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("DISCONNECTED");
				isConnected = false;
				isConnectedFully = false;
				break;
			}
		}
	}

	void NetManager::SendPacketPayload(const void* in_rPayload, int in_DataSize, enet_uint32 in_Flags)
	{
		// Create the ENet packet with the binary data
		ENetPacket* packet = enet_packet_create(in_rPayload, in_DataSize, in_Flags);

		// Send the packet
		enet_peer_send(m_pPeer, 0, packet);
		IncrementPacketCount();
		IncrementPacketSize(in_DataSize);
	}

	void NetManager::SendPacketString(const std::string& in_rData, PacketType in_Type)
	{
		//Create a string packet with a string and its type
		GenericPacketString* packet = new GenericPacketString();
		packet->type = in_Type;
		packet->SetString(in_rData);

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	void NetManager::SendPacketAnim(const std::string& in_rData, float in_Speed)
	{
		//Create a string packet with a string and its type
		AnimPacket* packet = new AnimPacket();
		packet->speed = in_Speed;
		packet->animName = (in_rData);

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	void NetManager::SendPacketStage(const std::string& in_rData, int in_PlayerID)
	{
		//Create a string packet with a string and its type
		StageStartPacket* packet = new StageStartPacket();
		packet->type = PacketType::STAGE_START;
		packet->stage = (in_rData);
		packet->playerType = in_PlayerID;

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	void NetManager::SendPacketChat(const std::string& in_rData, int in_Type)
	{
		//Create a string packet with a string and its type
		ChatPacket* packet = new ChatPacket();
		packet->type = PacketType::CHAT;
		packet->chatMessage = (in_rData);
		packet->chatType = in_Type;

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	void NetManager::SendPacketInt(const int& in_rData, PacketType in_Type)
	{
		//Create a string packet with a string and its type
		GenericPacketInt* packet = new GenericPacketInt();
		packet->type = in_Type;
		packet->payload = (in_rData);

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	//void NetManager::SendPacketInt64(const uint64& in_rData, PacketType in_Type)
	//{
	//	//Create a string packet with a string and its type
	//	GenericPacketInt64* packet = new GenericPacketInt64();
	//	packet->type = in_Type;
	//	packet->payload = (in_rData);
    //
	//	void* packetData = packet->Serialize();
	//	SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
	//	delete[] static_cast<uint8_t*>(packetData);
	//	IncrementPacketCount();
	//	IncrementPacketSize(packet->GetSize());
	//}
	void NetManager::SendPacketFloat(const float& in_rData, PacketType in_Type)
	{
		//Create a string packet with a string and its type
		GenericPacketFloat* packet = new GenericPacketFloat();
		packet->type = in_Type;
		packet->payload = (in_rData);

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	void NetManager::SendPacketStageEnd(const std::string& in_Data, const float& in_Milliseconds)
	{
		StageEndPacket* packet = new StageEndPacket();
		packet->type = STAGE_END;
		packet->stage = in_Data;
		packet->timeMilliseconds = in_Milliseconds;

		void* packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	//void NetManager::SendPacketVec4(Hedgehog::Math::CVector4& in_rVec4, PacketType in_Type)
	//{
	//	GenericPacketVec4* packet = new GenericPacketVec4();
	//	packet->type = in_Type;
	//	packet->Set(in_rVec4);
    //
	//	auto packetData = packet->Serialize();
	//	SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
	//	delete[] static_cast<uint8_t*>(packetData);
	//	IncrementPacketCount();
	//	IncrementPacketSize(packet->GetSize());
	//}
	void NetManager::SendPacketVec4(SimpleVec4& in_rVec4, PacketType in_Type)
	{
		GenericPacketVec4* packet = new GenericPacketVec4();
		packet->type = in_Type;
		packet->x = (in_rVec4.x);
		packet->y = (in_rVec4.y);
		packet->z = (in_rVec4.z);
		packet->w= (in_rVec4.w);

		auto packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}
	//oid NetManager::SendPacketVec3(Hedgehog::Math::CVector& in_rVec4, PacketType in_Type)
	//
	//	GenericPacketVec4* packet = new GenericPacketVec4();
	//	packet->type = in_Type;
	//	packet->Set(in_rVec4);
    //
	//	auto packetData = packet->Serialize();
	//	SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
	//	delete[] static_cast<uint8_t*>(packetData);
	//	IncrementPacketCount();
	//	IncrementPacketSize(packet->GetSize());
	//
	//void NetManager::SendPacketVec3Quantized(Hedgehog::Math::CVector& in_rVec4, PacketType in_Type, float in_Min, float in_Max)
	//{
	//	GenericPacketVec4Quantized* packet = new GenericPacketVec4Quantized();
	//	packet->type = in_Type;
	//	packet->Set(in_rVec4, in_Min, in_Max);
    //
	//	auto packetData = packet->Serialize();
	//	SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
	//	delete[] static_cast<uint8_t*>(packetData);
	//	IncrementPacketCount();
	//	IncrementPacketSize(packet->GetSize());
	//}
	void NetManager::SendPacketDamage(int in_TargetClient, std::string in_CurrentStatePlayer)
	{
		DamagePacket* packet = new DamagePacket();
		packet->type = DAMAGE;
		packet->targetClient = in_TargetClient;
		packet->senderState = in_CurrentStatePlayer;

		auto packetData = packet->Serialize();
		SendPacketPayload(packetData, packet->GetSize(), ENET_PACKET_FLAG_RELIABLE);
		delete[] static_cast<uint8_t*>(packetData);
		IncrementPacketCount();
		IncrementPacketSize(packet->GetSize());
	}

}
