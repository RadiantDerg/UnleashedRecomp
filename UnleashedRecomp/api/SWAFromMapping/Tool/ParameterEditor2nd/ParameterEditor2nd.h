#pragma once
#include <SWA/System/GameObject.h>

namespace SWA
{    
    class CParameterEditorWindow;
    class CGlobalParameterManager;

    class CParameterEditor2nd : public SWA::CGameObject //Hedgehog::Mirage::CRenderable, Hedgehog::Universe::TStateMachine<SWA::CParameterEditor2nd>
    {
    public:
        SWA_INSERT_PADDING(0x70);
        char m_Field12C;
        SWA_INSERT_PADDING(0x3);
        boost::shared_ptr<CGlobalParameterManager> m_spGlobalParameterManager;
        boost::shared_ptr<CParameterEditorWindow> m_spWindow;
    };
    SWA_ASSERT_OFFSETOF(CParameterEditor2nd, m_Field12C, 0x12C);
    SWA_ASSERT_OFFSETOF(CParameterEditor2nd, m_spGlobalParameterManager, 0x130);
}
