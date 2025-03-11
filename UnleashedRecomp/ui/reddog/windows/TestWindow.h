#pragma once

#include <ui/reddog/reddog_window.h>
#include <SWA.h>


class CSetObjectMakeEntry : public Hedgehog::Base::CObject
{
public:
    const void* m_pMake;
    Hedgehog::Base::CSharedString m_Name;
    bool m_Field08;

    CSetObjectMakeEntry(const void* in_rMake, const Hedgehog::Base::CSharedString& in_rName)
        : m_pMake(&in_rMake), m_Name(in_rName), m_Field08(false)
    {

    }
};


class CSetObjectFactory
{
public:
    hh::vector<CSetObjectMakeEntry> m_MakeList;
    uint8_t gap0[32];
    char UnknownChar;
    uint8_t gap501[39];
    int dword54;
    int test0;
    int test1;
    int test2;
    int test3;
    int test4;
     int test5;
    int test6;
    int test7;
    unsigned __int8 unsigned___int878;
    uint8_t gap79[7];
    int someInt;
    uint8_t ColorMap[124];
    uint8_t byte100;
};


class CSetObjectManager : public Hedgehog::Base::CSynchronizedObject, public SWA::CGameObject
{
public:
    SWA_INSERT_PADDING(0x4);
    boost::shared_ptr<CSetObjectFactory> m_spFactory;
};

SWA_ASSERT_OFFSETOF(CSetObjectManager, m_spFactory, 0xC4);


class TestWindow : public Reddog::Window
{
public:
    static inline CSetObjectManager* SetObjectManagerActor;
    static inline Hedgehog::Universe::CMessageActor* GamemodeActor;

    TestWindow() : Window()
    {
        Name = "Nextino window";
        Flags = Reddog::eWindowFlags_NoResize;
    }

    void Draw() override;
};
