#pragma once

#include <ui/reddog/reddog_window.h>
#include <SWA.h>
//class CSetObjectListener
//{
//public:
//    struct Vftable
//    {
//        be<uint32_t> m_fpDtor;
//        be<uint32_t> m_fpInitializeEditParam;
//    };
//    xpointer<Vftable> m_pVftable;
//
//    void InitializeEditParam(const guest_stack_var<SWA::CEditParam>& in_rEditParam)
//    {
//        bool isvtablenull = m_pVftable == nullptr;
//        GuestToHostFunction<void>(m_pVftable->m_fpInitializeEditParam.get(), this, in_rEditParam.get());
//    }
//};
//
//struct SSetObjectInfo
//{
//    boost::shared_ptr<SWA::CGameObject> spGameObject;
//    boost::shared_ptr<CSetObjectListener> spSetObjectListener;
//    float Field10;
//    bool Field14;
//    alignas(0x10) Hedgehog::Math::CQuaternion Field20;
//    size_t Field30;
//    size_t Field34;
//    bool Field38;
//    float Field3C;
//
//};
//class CSetObjectEntry
//{
//public:
//    boost::shared_ptr<SSetObjectInfo> m_spSetObjectInfo;
//    Hedgehog::Base::CSharedString m_Name;
//};
//class CSetObjectMakeEntry : public Hedgehog::Base::CObject
//{
//public:
//    xpointer<std::pair<Hedgehog::Base::CSharedString, be<uint32_t>>> m_pMapIt;
//    be<uint32_t> m_pFunc;
//    bool m_Field08;
//
//    void SpawnObject(const guest_stack_var<boost::shared_ptr<CSetObjectEntry>>& in_Var)
//    {
//        GuestToHostFunction<void>(m_pFunc, in_Var.get());
//    }
//};
//
//
//class CSetObjectFactory
//{
//public:
//
//    hh::map<Hedgehog::Base::CSharedString, be<uint32_t>> m_NameToMakeListIndexMap;
//    SWA_INSERT_PADDING(0x4);
//    hh::vector<CSetObjectMakeEntry> m_MakeList;
//    uint8_t gap0[32];
//    char UnknownChar;
//    uint8_t gap501[39];
//    int dword54;
//    int test0;
//    int test1;
//    int test2;
//    int test3;
//    int test4;
//     int test5;
//    int test6;
//    int test7;
//    unsigned __int8 unsigned___int878;
//    uint8_t gap79[7];
//    int someInt;
//    uint8_t ColorMap[124];
//    uint8_t byte100;
//};
//
////SWA_ASSERT_OFFSETOF(CSetObjectFactory, m_MakeList, 0x14);
//
//class CSetObjectManager : public Hedgehog::Base::CSynchronizedObject, public SWA::CGameObject
//{
//public:
//    struct CMember
//    {
//    boost::shared_ptr <CSetObjectFactory> m_spFactory;
//    };
//    SWA_INSERT_PADDING(0x4);
//    xpointer<CMember> m_pMember;
//};
//
//SWA_ASSERT_OFFSETOF(CSetObjectManager, m_pMember, 0xC4);


class TestWindow : public Reddog::Window
{
public:
    static inline SWA::CSetObjectManager* SetObjectManagerActor;
    static inline Hedgehog::Universe::CMessageActor* GamemodeActor;

    TestWindow() : Window()
    {
        Name = "Nextino window";
        Flags = Reddog::eWindowFlags_NoResize;
    }

    void Draw() override;
};
