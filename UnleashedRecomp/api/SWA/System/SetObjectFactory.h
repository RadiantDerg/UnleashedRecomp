namespace SWA
{
    class CSetObjectMakeEntry;
    class CSetObjectFactory
    {
    public:
        hh::map<Hedgehog::Base::CSharedString, be<uint32_t>> m_NameToMakeListIndexMap;
        SWA_INSERT_PADDING(0x4);
        hh::vector<CSetObjectMakeEntry> m_MakeList;
        /*uint8_t gap0[32];
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
        uint8_t byte100;*/
    };
}
