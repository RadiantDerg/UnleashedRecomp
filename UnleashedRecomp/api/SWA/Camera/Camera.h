#pragma once

#include <SWA.inl>
#include <SWA/System/GameObject.h>


namespace Hedgehog::Mirage
{
    class CCamera : public Base::CObject
    {
    public:
        SWA_INSERT_PADDING(0x10);
        Math::CMatrix m_View;
        Math::CMatrix44 m_Projection;
        Math::CVector m_Position;
        Math::CVector m_Direction;
        be<float> m_AspectRatio;
        be<float> m_Near;
        be<float> m_Far;
    };

    SWA_ASSERT_OFFSETOF(Hedgehog::Mirage::CCamera, m_View, 0x10);
    SWA_ASSERT_OFFSETOF(Hedgehog::Mirage::CCamera, m_Far, 0xB8);
}


namespace SWA
{
    class CCamera2 : public CGameObject // , public Hedgehog::Universe::TStateMachine<CCamera>
    {
    public:
        class CMyCamera : public Hedgehog::Mirage::CCamera
        {

        };
        xpointer<void> m_pVftable;
        SWA_INSERT_PADDING(0x60);
        CMyCamera m_MyCamera;
        SWA_INSERT_PADDING(0x178);
        be<float> m_FieldOfView;
        be<float> m_VertFieldOfView;
        be<float> m_HorzFieldOfView;
        SWA_INSERT_PADDING(0x18);
        bool m_InvertY;
        bool m_InvertX;
    };

    SWA_ASSERT_OFFSETOF(CCamera2, m_MyCamera, 0x120);


    class CCamera : public CGameObject // , public Hedgehog::Universe::TStateMachine<CCamera>
    {
    public:
        class CMyCamera : public Hedgehog::Mirage::CCamera
        {

        };
        xpointer<void> m_pVftable;
        SWA_INSERT_PADDING(0x60);
        xpointer<CMyCamera> test;
        SWA_INSERT_PADDING(0x60);
        // SWA::CCamera::MyCamera
        be<float> m_VertAspectRatio;
        SWA_INSERT_PADDING(0x48);
        be<float> m_HorzAspectRatio;
        SWA_INSERT_PADDING(0x178);
        be<float> m_FieldOfView;
        be<float> m_VertFieldOfView;
        be<float> m_HorzFieldOfView;
        SWA_INSERT_PADDING(0x18);
        bool m_InvertY;
        bool m_InvertX;
    };

    SWA_ASSERT_OFFSETOF(CCamera, test, 0x120);
    SWA_ASSERT_OFFSETOF(CCamera, m_VertAspectRatio, 0x184);
}
