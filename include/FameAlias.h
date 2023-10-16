#pragma once
#include <Region.h>

namespace gossip {
    
    struct fameAlias {
        
        std::string name = "";
        RE::TESForm* form = nullptr;
        
        regionMap known;
        fameAlias(){};
        fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {}
        fameAlias(SKSE::SerializationInterface* evt) {
            name = readString(evt);
            readForm(evt, form);
            
            //std::string aliasName = readString(evt);
            std::size_t size;
            evt->ReadRecordData(size);
            for (int i = 0; i < size; i++) {
                RE::BGSLocation* loc;
                readForm(evt, loc);
                known[loc] = region(evt);
            }
        };
        
        void save(SKSE::SerializationInterface* evt);
        region* getRegion(RE::BGSLocation* loc);
        valueData* getValueObject(valueType val, RE::BGSLocation* loc, RE::TESGlobal* global);
    };
    struct fameProfile {
        valueData recognition;
        RE::TESObjectREFR* akActor;
        std::vector<fameAlias*> activeAlias;
        std::map<fameAlias*, valueData> interest;
        fameProfile(){};
        fameProfile(RE::TESObjectREFR* akActor) : akActor(akActor) {}
        fameProfile(SKSE::SerializationInterface* evt);
        void save(SKSE::SerializationInterface* evt);
    };
}  // namespace gossip