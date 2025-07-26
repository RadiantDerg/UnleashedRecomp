#pragma once
#include <api/SWA.h>
#include "ENetCommon.h"
class CObjNetworkSonic;
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
    boost::shared_ptr<CObjNetworkSonic> m_spNetPlayerModel;

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
};
