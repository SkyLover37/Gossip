
#include <Scandal.h>
namespace gossip {
    class region{
    public:
        RE::BGSLocation* tLoc = nullptr;
        fameMap fameMap{};
        region(RE::BGSLocation* tLoc) : tLoc(tLoc) {};
        region(SKSE::SerializationInterface* evt);
        region(const region& data)  { 
            tLoc = data.tLoc;
            fameMap = data.fameMap;
        }
        void operator()(SKSE::SerializationInterface* evt);
        
        fameData* operator[](fameInfo* info) {
            auto entry = fameMap.find(info);
            if (entry == fameMap.end()) {
                
                entry = fameMap.insert(std::make_pair(info, fameData(info))).first;
            }
            return &entry->second;
        }
        bool operator!() { return !tLoc; }
    };
    using regionMap = std::map<RE::BGSLocation*, region>;
}  // namespace gossip