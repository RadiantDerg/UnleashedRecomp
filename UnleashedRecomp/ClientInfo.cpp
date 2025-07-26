#include "ClientInfo.h"

#include "ObjNetworkSonic.h"
namespace net
{
    void ClientInfo::SetUsername(const std::string& in_Username)
    {
        data.m_username = in_Username;
        //m_pNameTag->SetText((in_Username).c_str());
    }
    int ClientInfo::GetID() const
    {
        return data.m_id;
    }
    std::string ClientInfo::GetUsername()
    {
        return data.m_username;
    }
    Hedgehog::Math::CVector ClientInfo::GetPosition() const
    {
        return Hedgehog::Math::CVector(data.position.x, data.position.y, data.position.z);
    }
    Hedgehog::Math::CVector4 ClientInfo::GetRotation() const
    {
        return Hedgehog::Math::CVector4(data.rotation.x, data.rotation.y, data.rotation.z);
    }
    SimpleVec4 ClientInfo::GetColor() const
    {
        return data.m_vColor;
    }
    std::string ClientInfo::GetAnimation()
    {
        return m_sAnimName;
    }

    int ClientInfo::GetPlayerType() const
    {
        return data.m_iPlayerType;
    }

    std::string ClientInfo::GetStage()
    {
        return data.m_Stage;
    }

    void ClientInfo::SetPosition(const Hedgehog::Math::CVector& in_Pos)
    {
        data.position = SimpleVec4(in_Pos.X, in_Pos.Y, in_Pos.Z, 0);
        if (m_spNetPlayerModel)
        {
            m_spNetPlayerModel->SetPosition(in_Pos);
        }
    }
    void ClientInfo::SetPosition(const SimpleVec4& in_Pos)
    {
        data.position = in_Pos;
        if (m_spNetPlayerModel)
        {
            m_spNetPlayerModel->SetPosition(data.position);
        }
    }
    void ClientInfo::SetRotation(const Hedgehog::Math::CVector4& in_Rot)
    {
        data.rotation = SimpleVec4(in_Rot.X, in_Rot.Y, in_Rot.Z, in_Rot.W);
        if (m_spNetPlayerModel)
        {
            m_spNetPlayerModel->SetRotation(in_Rot);
        }
    }
    void ClientInfo::SetRotation(const SimpleVec4& in_Rot)
    {
        data.rotation = in_Rot;
        if (m_spNetPlayerModel)
        {
            m_spNetPlayerModel->SetRotation(data.rotation);
        }

    }
    void ClientInfo::SetColor(const SimpleVec4& in_Color)
    {
        data.m_vColor = in_Color;
        //if (m_spNetPlayerModel != nullptr)
        //{
        //	m_spNetPlayerModel->SetRot(data.rotation);
        //}
    }
    void ClientInfo::Update()
    {
        if (NetManager::CLIENT_ID != GetID())
        {
            if (!m_spNetPlayerModel)
            {
                m_spNetPlayerModel = boost::make_shared<CObjNetworkSonic>();
                m_spNetPlayerModel->SetID(GetID());

                guest_stack_var<Hedgehog::Base::CSharedString> worldName("main");
                guest_stack_var<Hedgehog::Base::THolder<SWA::CWorld>, false> worldHolder;
                guest_stack_var<boost::shared_ptr<CObjNetworkSonic>> boost(m_spNetPlayerModel);
                GuestToHostFunction<void>(sub_8250F238, worldHolder.get(), SWA::CGameDocument::GetInstance(), worldName.get()); // get holder
                GuestToHostFunction<void>(sub_82510D20, SWA::CGameDocument::GetInstance(), worldHolder.get(), boost.get(), 0); // add game object

                m_spNetPlayerModel->SetPosition(GetPosition());
                //m_spNetPlayerModel->SetRot(GetRotation());
                //m_pNameTag->Initialize();
            }
            m_spNetPlayerModel->SetID(GetID());
            //m_pNameTag->Update(GetPosition());
        }
    }

    void ClientInfo::SetData(SClientCommonData in_Data)
    {
        data = in_Data;
    }

    void ClientInfo::SetAnimationSpeed(Hedgehog::Math::CVector speed)
    {
        m_fAnimSpeed = speed;
    }
    void ClientInfo::SetAnimationBlend(Hedgehog::Math::CVector speed)
    {
        m_fAnimBlend = speed;
    }
    void ClientInfo::SetStageRatio(float payload)
    {
        data.stageRatio = payload;
    }

    float ClientInfo::GetRatio() const
    {
        return data.stageRatio;
    }

    void ClientInfo::SetPlayerType(int in_PlayerType)
    {
        data.m_iPlayerType = in_PlayerType;
        //if (m_spNetPlayerModel != nullptr)
        //{
        //    m_spNetPlayerModel->Kill();
        //    m_spNetPlayerModel = nullptr;
        //}
    }

    void ClientInfo::SetStage(const std::string& in_Stage)
    {
        data.m_Stage = in_Stage;
    }

    void ClientInfo::SetChatVisibility(int in_Payload)
    {
        //if (m_spNetPlayerModel != nullptr)
        //{
        //    m_spNetPlayerModel->SetChatParticle(in_Payload);
        //}
    }
    void ClientInfo::SetSteam(int64_t  in_Id)
    {
        data.m_Steamid = in_Id;
    }
    int64_t  ClientInfo::GetSteam()
    {
        return data.m_Steamid;
    }
    void ClientInfo::SetAnimation(const std::string& in_AnimState) const
    {
        //if (m_spNetPlayerModel != nullptr)
        //{
        //    //If the current state isnt null (theres a brief period of time where it could be)
        //    if (m_spNetPlayerModel->m_StateMachine->GetCurrentState() != nullptr)
        //    {
        //        m_spNetPlayerModel->ChangeAnimation(in_AnimState.c_str());
        //    }
        //}
    }
    void ClientInfo::ResetPlayer()
    {
        if (m_spNetPlayerModel)
            m_spNetPlayerModel->Kill();
        m_spNetPlayerModel = nullptr;
    }
    void ClientInfo::SetID(int in_Id)
    {
        data.m_id = in_Id;
    }
    void ClientInfo::PlaySoundCue(const std::string& in_CueNodeName)
    {
        //m_spSound = Hedgehog::Sound::CSoundSystem::GetInstance()->PlaySound(in_CueNodeName.c_str());
    }
    void ClientInfo::Destroy() const
    {
        //if (m_spNetPlayerModel != nullptr)
        //    m_spNetPlayerModel->Kill();
        //
        //if (m_pNameTag != nullptr)
        //    m_pNameTag->Destroy();
    }
    ClientInfo::ClientInfo(int in_Id)
    {
        data.m_id = in_Id;
        data.position = SimpleVec4();
        data.rotation = SimpleVec4();
        //Debug::Toast(format("SPAWNED NEW CLIENTINFO (%d)", in_Id), 15, 100);
        ////if (m_pNameTag == nullptr)
        //m_pNameTag = new net::gui::CObjNameTagUI(this);
        //m_spNetPlayerModel = boost::make_shared<CObjNetworkSonic>();
        //m_spNetPlayerModel->SetID(in_Id);
    }
}
