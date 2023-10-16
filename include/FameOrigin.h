#pragma once
#include <valueData.h>
#include <serializationUtil.h>
namespace gossip {
    
    class fameInfo {
        regionLimit _limit{0, 100};
        std::vector<std::string> tags;
        std::string name = "";
        RE::TESGlobal* fameGlobal = nullptr;
        fameInfo(){};
        template <typename T>
        fameInfo(SKSE::SerializationInterface* evt, T* glob = nullptr);
        fameInfo(RE::TESGlobal* newForm, std::string name, int min, int max, std::vector<std::string> tags);

    public:
        void save(SKSE::SerializationInterface* evt);
        void setLimit(short min, short max);
        regionLimit getLimit();
        RE::TESGlobal* getGlobal() { return fameGlobal;}
    };
    class fameData {
        fameInfo* info;
        valueData<short> data;

        public:

    };
    using infoMap = std::map<RE::TESGlobal*, fameInfo>;
    using fameMap = std::map<fameInfo*, fameData>;
}  // namespace gossip