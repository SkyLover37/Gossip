#include <Region.h>
namespace gossip {
    region::region(SKSE::SerializationInterface* evt){
        
        readForm(evt, tLoc);
        if (!tLoc) logger::debug("location failed");
        std::size_t size;
        evt->ReadRecordData(size);
        logger::debug("Loading {} fame in {}", size, tLoc->GetName());
        for (int i = 0; i < size; ++i) {
            fameData tmpFameObj = fameData(evt);
            if (!tmpFameObj.info) continue;
            logger::debug("{} {} {}", tmpFameObj.raw, tmpFameObj.val, tmpFameObj._gossip);
            logger::debug("Limits: {} {} : {} {}", tmpFameObj.localBound.min(), tmpFameObj.localBound.max(),
                          tmpFameObj.info->getLimit()->min(), tmpFameObj.info->getLimit()->max());
            logger::debug("Loaded {} fameData for {} ", tLoc->GetName(),
                          tmpFameObj.info->getGlobal()->GetFormEditorID());
            fameMap.insert(std::make_pair(tmpFameObj.info, tmpFameObj));
        }
        logger::debug("Finished loading {} region", tLoc->GetName());
    }
    void region::operator()(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(tLoc->GetFormID());
        evt->WriteRecordData(fameMap.size());
        for (auto& fame : fameMap) {
            logger::debug("Saving {} Fame", fame.second.info->getGlobal()->GetFormEditorID());
            fame.second.save(evt);
            //fame.second.localBound(evt);
            
        }
    }
}  // namespace gossip