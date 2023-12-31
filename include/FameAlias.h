#pragma once
#include <Region.h>

namespace gossip {
    
    class fameAlias {
    public:
        std::string name = "";
        RE::TESFaction* faction;
        regionMap regionMap;
        fameAlias(std::string name, RE::TESFaction* faction) : name(name), faction(faction) {}
        fameAlias(SKSE::SerializationInterface* evt, std::uint32_t version);
        void operator()(SKSE::SerializationInterface* evt);
        region& operator[](RE::BGSLocation* loc) { 
            auto entry = regionMap.find(loc);
            bool found = entry != regionMap.end();
            if (!found)
                logger::error("Could not find region associated with {} {} {:x}", loc->GetName(),
                              loc->GetFormEditorID(), loc->GetFormID());
            return entry->second;
        }
    };
    using aliasMap = std::map<RE::TESFaction*, fameAlias>;

    class fameProfile {
    public:
        RE::TESObjectREFR* akActor = RE::PlayerCharacter::GetSingleton();
        fameAlias* activeAlias = nullptr;
        aliasMap aliasMap;
        regionMap regionMap;
        std::vector<RE::Actor*> sawPlayerSex;
        fameProfile(RE::TESObjectREFR* akActor) : akActor(akActor) {}
        fameProfile(SKSE::SerializationInterface* evt, std::uint32_t Version);
        fameAlias& operator[](RE::TESFaction* fac) { 
            auto entry = aliasMap.find(fac);
            if (entry == aliasMap.end())
                logger::error("Could not find fameAlias associated with {} {} {:x}", fac->GetName(),
                              fac->GetFormEditorID(), fac->GetFormID());
            return entry->second;
        }
        
        void operator=(fameAlias* val) { activeAlias = val; }
        void operator()(SKSE::SerializationInterface* evt);
        
    };
}  // namespace gossip