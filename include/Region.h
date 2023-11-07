
#include <Scandal.h>
namespace gossip {
    class region{
    public:
        RE::BGSLocation* tLoc = nullptr;
        float timeSinceVisit = 0;
        fameMap fameMap{};
        region(RE::BGSLocation* tLoc) : tLoc(tLoc) {};
        region(SKSE::SerializationInterface* evt, std::uint32_t Version);
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
        void syncLocation() {
            for (auto entry : fameMap) {
                logger::debug("Setting {}({}) GlobalVariable to {} in {}", entry.second.info->getGlobal()->GetName(),
                              entry.second.info->getGlobal()->value, int(entry.second), tLoc->GetFormEditorID());
                
                timeSinceVisit = 0;
                entry.second.info->getGlobal()->value = entry.second;
            }
        }
    };
    using regionMap = std::map<RE::BGSLocation*, region>;
}  // namespace gossip