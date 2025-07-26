#pragma once
#include <api/SWA.h>
//extern "C" {
#include <enet/enet.h>
//}
#include "ENetCommon.h"
#include "ClientInfo.h"
/// <summary>
/// Manages everything Network related. Based on https://www.youtube.com/watch?v=FxrKS_1zE9s.
/// </summary>

namespace net
{
    
	class NetManager
	{
	private:
		//Holds ip and port of server
		static inline ENetAddress m_Address;
		static inline ENetHost* m_pClient;
		static inline ENetEvent m_Event;
		static inline uint16_t m_MaxPlayers;
		//Server that we're connecting to
		//Peers is anybody that seems connected, the server has a list of these so that data can be exchanged
		static inline ENetPeer* m_pPeer;
		static inline int m_PacketAmountThisFrame;
		static inline int m_PacketTotalSizeThisFrame;
		static void ParsePackets(enet_uint8* data, size_t length);
	public:
		static int GetPing();
		static void SendPacketFloat(const float& in_rData, PacketType in_Type);
		//static inline boost::shared_ptr<NetManager> m_spInstance;
		static inline int CLIENT_ID = -1;

		//static inline std::vector<SChatLog> m_ChatLogs;
		static inline std::vector<ClientInfo*> m_ClientMap;
		static inline bool isConnectedFully;
		static inline bool isConnected;

		static int Initialize();
		static int Connect(const char* in_Username, const char* in_Address, unsigned int in_Port);
		static ClientInfo* GetSelf();
		static ClientInfo* GetClientFromId(int in_Id);
		static uint16_t GetMaxPlayers();
		static int GetPacketCount()
		{
			return m_PacketAmountThisFrame;
		}
		static void IncrementPacketSize(int in_Size)
		{
			m_PacketTotalSizeThisFrame+= in_Size;
		};
		static int GetTotalPacketsSize()
		{
			return m_PacketTotalSizeThisFrame;
		};
		static void IncrementPacketCount()
		{
			m_PacketAmountThisFrame++;
		};
		static void ResetPacketCount()
		{
			m_PacketAmountThisFrame = 0;
			m_PacketTotalSizeThisFrame = 0;
		};
		static void Disconnect();
		static void ProcessPackets();
		static void SendPacketPayload(const void* in_rPayload, int in_DataSize, enet_uint32 in_Flags);
		static void SendPacketString(const std::string& in_rData, PacketType in_Type = PacketType::CONNECT);
		static void SendPacketAnim(const std::string& in_rData, float in_Speed);
		static void SendPacketStage(const std::string& in_rData, int in_PlayerID);
		static void SendPacketChat(const std::string& in_rData, int in_Type);
		static void SendPacketInt(const int& in_rData, PacketType in_Type);
		//static void SendPacketInt64(const uint64& in_rData, PacketType in_Type);
		static void SendPacketStageEnd(const std::string& in_Data, const float& in_Milliseconds);
		//static void SendPacketVec4(Hedgehog::Math::CVector4& in_rVec4, PacketType in_Type = PacketType::MOVE);
		static void SendPacketVec4(SimpleVec4& in_rVec4, PacketType in_Type);
		//static void SendPacketVec3(Hedgehog::Math::CVector& vec, PacketType in_Type = PacketType::MOVE);
		//static void SendPacketVec3Quantized(Hedgehog::Math::CVector& in_rVec4, PacketType in_Type, float in_Min, float in_Max);
		static void SendPacketDamage(int in_TargetClient, std::string in_CurrentStatePlayer);
	};
}
