
#include <Scandal.h>
namespace gossip {


    class region : public valueData<int, default_limit_tag> {
    public:
        typedef valueData<int, default_limit_tag> interest;
        RE::BGSLocation* tLoc;
        fameMap fameMap;
        region(SKSE::SerializationInterface* evt);
        void operator()(SKSE::SerializationInterface* evt);
        fameData* operator[](fameInfo* info) {
            auto entry = fameMap.find(info);
            return entry != fameMap.end() ? &entry->second : nullptr;
        }
    };
    using regionMap = std::map<RE::BGSLocation*, region>;
}  // namespace gossip