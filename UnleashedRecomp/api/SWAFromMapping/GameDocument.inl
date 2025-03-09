namespace SWA
{
    // TODO: Hedgehog::Base::TSynchronizedPtr<CGameDocument>
    inline CGameDocument2* CGameDocument2::GetInstance()
    {
        return *(xpointer<CGameDocument2>*)MmGetHostAddress(0x83367900);
    }
}
