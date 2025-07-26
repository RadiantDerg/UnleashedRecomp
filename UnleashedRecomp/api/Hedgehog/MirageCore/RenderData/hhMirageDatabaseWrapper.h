#pragma once
namespace Hedgehog::Database
{
    class CDatabase;
}
namespace Hedgehog::Mirage
{
    class CModelData;
    class CMirageDatabaseWrapper : public Hedgehog::Base::CObject
    {
    public:
        SWA_INSERT_PADDING(0x8);
        CMirageDatabaseWrapper(Hedgehog::Database::CDatabase* in_pDatabase)
        {
            GuestToHostFunction<void*>(sub_82E5E6C8, this, in_pDatabase);
        }
        boost::shared_ptr<CModelData>* GetModelData(const Hedgehog::Base::CSharedString& in_rName)
        {
            guest_stack_var<boost::shared_ptr<CModelData>> model;
            guest_stack_var<Hedgehog::Base::CSharedString> modelName(in_rName);            
            GuestToHostFunction<void*>(sub_82E37318, model.get(), this, modelName.get(), 0);

            return model.get();
        }
    };
}
