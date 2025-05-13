#pragma once
#include <SWA/System/GameObject.h>

namespace SWA
{
    class CParamBase : public Hedgehog::Base::CObject
    {
    public:
        xpointer<void> m_vfTable;
        Hedgehog::Base::CSharedString m_Name;
        xpointer<void> m_pVariable;
    };
    SWA_ASSERT_OFFSETOF(CParamBase, m_Name, 0x4);


    class CEditParam
    {
    public:
        xpointer<void> m_vfTable;
        hh::vector<boost::shared_ptr<CParamBase>> m_ParamList;
        CEditParam()
        {
        }
    };
    //SWA_ASSERT_OFFSETOF(CEditParam, m_ParamList, 0x10);

    class CAbstractParameter
    {
    public:
        xpointer<void> m_vfTable;
        boost::shared_ptr<SWA::CEditParam> m_pEditParam;
        Hedgehog::Base::CSharedString m_Name;
        Hedgehog::Base::CSharedString m_DisplayName;
        hh::vector<boost::shared_ptr<CAbstractParameter>> m_Children;
        SWA_INSERT_PADDING(0x14);
    };
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_pEditParam, 0x4);
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_Name, 0xC);
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_Children, 0x14);


    class CAbstractParameterNode : public CAbstractParameter
    {
    public:
        SWA_INSERT_PADDING(0x8);
    };

    class CParameterFile : public CAbstractParameter
    {

    };

    class CParameterCategory2 : public CAbstractParameterNode
    {
    public: 
        struct Test
        {
            Hedgehog::Base::CSharedString test;
            bool test2;
        };
    public:
        hh::map <Hedgehog::Base::CSharedString, be<uint32_t >> m_ParamNameMap;
        int dword4c;
        int dword50;
        int dword54;
        int dword58;
        int dword5C;
    };
    SWA_ASSERT_OFFSETOF(CParameterCategory2, m_ParamNameMap, 0x40);
    SWA_ASSERT_OFFSETOF(CParameterCategory2, dword50, 0x50);
    class CGlobalParameterManager 
    {
    public:
        hh::vector<boost::shared_ptr<CParameterFile>> m_GlobalParameterFileList;
    };


    class CParameterEditorWindow;

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
