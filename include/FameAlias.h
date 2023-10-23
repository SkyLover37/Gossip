#pragma once
#include <Region.h>

namespace gossip {
    
    class fameAlias {
    public:
        std::string name = "";
        RE::TESFaction* faction;
        regionMap regionMap;
        fameAlias(){};
        fameAlias(std::string name, RE::TESFaction* faction) : name(name), faction(faction) {}
        fameAlias(SKSE::SerializationInterface* evt);
        void operator()(SKSE::SerializationInterface* evt);
        std::optional<region> operator[](RE::BGSLocation* loc) { 
            auto entry = regionMap.find(loc);
            bool found = entry != regionMap.end();
            return found ? std::optional<region>(entry->second) : std::nullopt;
        }
    };
    using aliasMap = std::map<RE::TESFaction*, fameAlias>;


    using recognition = valueData<long long, default_limit_tag>;
    class fameProfile : recognition {
    public:
        RE::TESObjectREFR* akActor;
        fameAlias* activeAlias;
        aliasMap aliasMap;
        regionMap regionMap;
        fameProfile(RE::TESObjectREFR* akActor) : akActor(akActor), recognition(0) {}
        fameProfile(SKSE::SerializationInterface* evt);
        fameAlias& operator[](RE::TESFaction* fac) { return aliasMap[fac];
        }
        void operator=(fameAlias* val) { activeAlias = val; }
        void operator()(SKSE::SerializationInterface* evt);
        void operator()(fameAlias* val){ 
            for (auto& entry : val->regionMap) {
                auto regionEntry = regionMap.find(entry.first);
                if (regionEntry != regionMap.end())
                    regionEntry->second += entry.second;
            }
        };
    };
}  // namespace gossip