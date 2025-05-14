
namespace SWA
{
    class CParamBase;
    class CEditParam
    {
    public:
        xpointer<void> m_vfTable;
        hh::vector<boost::shared_ptr<CParamBase>> m_ParamList;
        CEditParam()
        {
        }
    };
}
