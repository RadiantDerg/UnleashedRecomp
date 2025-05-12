#pragma once
#include <SWA/System/GameObject.h>

namespace SWA
{
    class CParamBase : public Hedgehog::Base::CObject
    {
    public:
        xpointer<void> m_vfTable;
        Hedgehog::Base::CSharedString m_Name;
        bool t;
        bool t2;
    };


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


    class CAbstractParameter : public Hedgehog::Base::CObject
    {
    public:
        xpointer<CEditParam> m_pEditParam;
        Hedgehog::Base::CSharedString m_Name;
        Hedgehog::Base::CSharedString m_Description;
        hh::vector<boost::shared_ptr<CAbstractParameter>> m_Children;
        SWA_INSERT_PADDING(0x14);

        virtual ~CAbstractParameter() {};
    };
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_Name, 0xC);
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_Children, 0x14);


    class CParameterFile : public CAbstractParameter
    {

    };


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
