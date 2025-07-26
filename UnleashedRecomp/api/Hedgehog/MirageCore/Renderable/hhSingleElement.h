#pragma once
namespace SWA
{
    class CMatrixNodeTransform;
};
namespace Hedgehog::Mirage
{
    class CSingleElement : public CRenderable
    {
    public:
        SWA_INSERT_PADDING(0x90);


        CSingleElement(boost::shared_ptr<Hedgehog::Mirage::CModelData>* spModelData)
        {
            GuestToHostFunction<void*>(sub_82E1F820, this, spModelData);
        }
        void BindMatrixNode(const boost::shared_ptr < SWA::CMatrixNodeTransform>& in_spMatrixNode)
        {
            GuestToHostFunction<void*>(sub_82E1E100, this, &in_spMatrixNode);
        }
    };
    SWA_ASSERT_SIZEOF(CSingleElement, 0x98u);
};
