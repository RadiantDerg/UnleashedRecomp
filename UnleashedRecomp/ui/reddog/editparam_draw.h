namespace Reddog
{
    enum EParamType
    {
        TARGET_LIST = 0x8203C234,
        TYPE_LIST = 0x8203C31C,
        FLOAT = 0x8203C374,
        TARGET = 0x8203C184,
        SET_LIST = 0x8203C05C,
        POSITION = 0x8203C0E4,
        DUMMY = 0x8203BE54,
        BOOL = 0x8203BDAC,
        ULONG = 0x8203C40C,
        INT = 0x8203C4BC,
        LONG = 0x8203C464
        //switch (in_Vtable)
        //{
        //case 0x8203C31C:
        //    return "[TYPELIST]";
        //case 0x8203C374:
        //    return "[FLOAT]";
        //case 0x8203C234:
        //    return "[TARGET_LIST]";
        //case 0x8203C184:
        //    return "[TARGET]";
        //case 0x8203C05C:
        //    return "[LIST_SETID]";
        //case 0x8203C0E4:
        //    return "[POSITION]";
        //case 0x8203BE54:
        //    return "[DUMMY]";
        //case 0x8203BDAC:
        //    return "[BOOL]";
        //case 0x8203C40C:
        //    return "[ULONG]";
        //case 0x8203C4BC:
        //    return "[INT]";
        //case 0x8203C464:
        //    return "[LONG]";
        //}
    };
    class CEditParamDraw
    {
    public:
        static void DrawEditParam(const boost::shared_ptr<SWA::CEditParam>& in_EditParam);
        static std::string GetString(const char* value);
    };
}
