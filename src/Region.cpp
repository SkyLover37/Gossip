#include <Region.h>
namespace gossip {
    region::region(SKSE::SerializationInterface* evt){
        
        readForm(evt, tLoc);
        std::size_t size;
        evt->ReadRecordData(size);
        
        for (int i = 0; i < size; i++) {
            fameData tmpFameObj = fameData(evt);
            if (!tmpFameObj.info) continue;
            fameMap.insert({tmpFameObj.info, tmpFameObj});
        }
    }
    void region::operator()(SKSE::SerializationInterface* evt) {
        //interest::operator()(evt);
        evt->WriteRecordData(tLoc->formID);
        evt->WriteRecordData(fameMap.size());
        for (auto& fame : fameMap) {
            evt->WriteRecordData(fame.first->getGlobal()->GetFormID());
            fame.second(evt);
            //fame.second.localBound(evt);
            
        }
    }
}  // namespace gossip