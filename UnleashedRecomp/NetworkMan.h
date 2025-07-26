#pragma once
#include <api/SWA.h>
//extern "C" {
#include <enet/enet.h>
//}
#include "ENetCommon.h"
/// <summary>
/// Manages everything Network related. Based on https://www.youtube.com/watch?v=FxrKS_1zE9s.
/// </summary>

namespace net
{
    class ClientInfo
    {
    private:
        //TODO: replace the stuff in here with commondata
        SClientCommonData data;
        //int m_iID;
        //std::string m_sUsername;
        //Hedgehog::Math::CVector m_Position;
        //Hedgehog::Math::CVector4 m_Rotation;
        //boost::shared_ptr<Hedgehog::Sound::CSoundHandle> m_spSound;

    public:

        //SSteamAvatarCsd* avatar = new SSteamAvatarCsd();
        //uint8_t m_iPlayerType;
        //std::string m_sStage;
        std::string m_sAnimName;
        Hedgehog::Math::CVector m_fAnimSpeed;
        Hedgehog::Math::CVector m_fAnimBlend;
        //gui::CObjNameTagUI* m_pNameTag;
        //boost::shared_ptr<mod::CObjNetworkSonic> m_spNetPlayerModel;

        ClientInfo(int in_Id);

        std::string GetUsername();
        int GetID() const;
        Hedgehog::Math::CVector GetPosition() const;
        Hedgehog::Math::CVector4 GetRotation() const;
        SimpleVec4 GetColor() const;
        std::string GetAnimation();
        int GetPlayerType() const;
        std::string GetStage();


        void SetUsername(const std::string& in_Username);
        void SetID(int in_Id);
        void SetPosition(const SimpleVec4& in_Pos);
        void SetPosition(const Hedgehog::Math::CVector& in_Pos);
        void SetRotation(const Hedgehog::Math::CVector4& in_Rot);
        void SetRotation(const SimpleVec4& in_Rot);
        void SetColor(const SimpleVec4& in_Color);
        void SetAnimation(const std::string& in_AnimState) const;
        void SetPlayerType(int in_PlayerType);
        void SetStage(const std::string& in_Stage);
        void SetChatVisibility(int in_Payload);
        void SetSteam(int64_t  in_Payload);

        int64_t  GetSteam();

        void ResetPlayer();
        void PlaySoundCue(const std::string& in_CueNodeName);
        void Destroy() const;
        void Update();
        void SetData(SClientCommonData in_Data);
        void SetAnimationSpeed(Hedgehog::Math::CVector speed);
        void SetAnimationBlend(Hedgehog::Math::CVector speed);
        void SetStageRatio(float payload);
        float GetRatio() const;
    };
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
