#pragma once
#include <Region.h>

namespace gossip {
    
    struct fameAlias {
        std::string name = "";
        RE::TESForm* form = nullptr;
        
        typedef std::map<RE::BGSLocation*, region> regionList;

        
        regionList known;
        fameAlias(){};
        fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {}
        fameAlias(SKSE::SerializationInterface* evt) {
            name = readString(evt);
            form = readForm(evt);
            
            //std::string aliasName = readString(evt);
            std::size_t size;
            evt->ReadRecordData(size);
            for (int i = 0; i < size; i++) {
                RE::BGSLocation* loc = readForm(evt)->As<RE::BGSLocation>();
                known[loc] = region(evt);
            }
        };
        
        void save(SKSE::SerializationInterface* evt);
        region* getRegion(RE::BGSLocation* loc);
        valueData* getValueObject(valueType val, RE::BGSLocation* loc, RE::TESGlobal* global);
    };
    struct fameProfile {
        RE::TESObjectREFR* akActor;
        float viewingTime = 0;
        int depravity = 0;
        int disGood = 0;
        int disBad = 0;
        std::map<fameAlias*, valueData> interest;
        fameProfile(){};
        fameProfile(RE::TESObjectREFR* akActor) : akActor(akActor) {}
        fameProfile(SKSE::SerializationInterface* evt);
        void save(SKSE::SerializationInterface* evt);
        int setInterest(RE::BGSLocation* fameLoc, int amt);
        int addInterest(RE::BGSLocation* fameLoc, int amt);
        int removeInterest(RE::BGSLocation* fameLoc, int amt);
        int getInterest(RE::BGSLocation* fameLoc);
        void clearValues(valueType type, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
    };
}  // namespace gossip