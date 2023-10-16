#include <Region.h>
namespace gossip {
    region::region(SKSE::SerializationInterface* evt, infoMap& info) {
        super::valueData<int>(evt);

        readForm(evt, tLoc);
        std::size_t size;

         
        evt->ReadRecordData(size);
        for (int i = 0; i < size; i++) {
           

            fame tmpFameObj = fame(info);
            (*this)[tmpInfo] = tmpFameObj;
        }
    }
    void region::save(SKSE::SerializationInterface* evt) {
        super::save(evt);
        evt->WriteRecordData(tLoc->formID);
        evt->WriteRecordData(size());
        for (auto& fame : *this) {
            evt->WriteRecordData(fame.first->getGlobal()->GetFormID());
            fame.second.save(evt);
            fame.second.local.save(evt);
            
        }
    }
    fame::fame(SKSE::SerializationInterface* evt) {
        RE::TESGlobal* glob = nullptr;
        super(evt);
        local.load(evt);
        readForm(evt, glob);
        if (!glob) return;

        fameInfo* tmpInfo = &info[glob];
    }
}  // namespace gossip