#include <Region.h>
namespace gossip {
    region::region(SKSE::SerializationInterface* evt) : interest(evt) {
        readForm(evt, tLoc);
        std::size_t size;
        evt->ReadRecordData(size);
        
        for (int i = 0; i < size; i++) {
            fameData tmpFameObj = fameData(evt);
            if (!tmpFameObj.info) continue;
            static_cast<fameMap>(*this)[tmpFameObj.info] = tmpFameObj;
        }
    }
    void region::operator()(SKSE::SerializationInterface* evt) {
        interest(evt);
        evt->WriteRecordData(tLoc->formID);
        evt->WriteRecordData(size());
        for (auto& fame : *this) {
            evt->WriteRecordData(fame.first->getGlobal()->GetFormID());
            fame.second(evt);
            fame.second.localBound(evt);
            
        }
    }
}  // namespace gossip