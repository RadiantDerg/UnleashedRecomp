#pragma once

#include <SWA.inl>

namespace Hedgehog::Math
{
    class CMatrix // Eigen::Affine3f
    {
    public:
        be<float> m_M00;
        be<float> m_M01;
        be<float> m_M02;
        be<float> m_M03;
        be<float> m_M10;
        be<float> m_M11;
        be<float> m_M12;
        be<float> m_M13;
        be<float> m_M20;
        be<float> m_M21;
        be<float> m_M22;
        be<float> m_M23;
        be<float> m_M30;
        be<float> m_M31;
        be<float> m_M32;
        be<float> m_M33;
    };

    class CMatrix44
    {
    public:
        be<float> data[16]; // Eigen::Matrix4f
    };

    //SWA_ASSERT_SIZEOF(CMatrix, 0x40);
    //SWA_ASSERT_SIZEOF(CMatrix44, 0x40);
}
