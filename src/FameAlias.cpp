#include "FameAlias.h"
//#include <Scandal.h>
namespace gossip {
    fameProfile::fameProfile(SKSE::SerializationInterface* evt) : recognition(0){
        readForm(evt, akActor);
    }
    void fameProfile::operator()(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(akActor->formID);
    }
    fameAlias::fameAlias(SKSE::SerializationInterface* evt) {
        name = readString(evt);
        readForm(evt, faction);

        // std::string aliasName = readString(evt);

        std::size_t size;
        evt->ReadRecordData(size);
        for (int i = 0; i < size; i++) {
            RE::BGSLocation* loc;
            readForm(evt, loc);
            if (!loc) continue;
            regionMap.insert(std::make_pair(loc, region(evt)));
        }
    }
    void fameAlias::operator()(SKSE::SerializationInterface* evt) {
        
        writeString(evt, name);
        evt->WriteRecordData(faction->formID);
            
        for (auto& knownEntry : regionMap) {
            evt->WriteRecordData(regionMap.size());
            evt->WriteRecordData(knownEntry.first->formID);
            knownEntry.second(evt);
        }
        
    }
   
}  // namespace gossip