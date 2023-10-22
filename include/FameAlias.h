#pragma once
#include <Region.h>

namespace gossip {
    
    class fameAlias : public regionMap {
    public:
        std::string name = "";
        RE::TESFaction* faction;
 
        fameAlias(std::string name, RE::TESFaction* faction) : name(name), faction(faction) {}
        fameAlias(SKSE::SerializationInterface* evt);
        void operator()(SKSE::SerializationInterface* evt);
        region& operator[](RE::BGSLocation* loc) { return static_cast<regionMap>(*this)[loc]; }
    };
    using aliasMap = std::map<RE::TESFaction*, fameAlias>;


    using recognition = valueData<long long>;
    class fameProfile : public recognition, public aliasMap {
    public:
        RE::TESObjectREFR* akActor;
        fameAlias* activeAlias;

        fameProfile(RE::TESObjectREFR* akActor) : akActor(akActor) {}
        fameProfile(SKSE::SerializationInterface* evt);
        fameAlias& operator[](RE::TESFaction* fac) { return static_cast<aliasMap>(*this)[fac];
        }
        void operator()(SKSE::SerializationInterface* evt);
    };
}  // namespace gossip