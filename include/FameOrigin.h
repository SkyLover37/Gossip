#pragma once
#include <valueData.h>
namespace gossip {
    namespace fame {
        enum limit_type { local = 'LOCL', regional = 'REGL' };
    }
    class fameInfo : bound<short, fame::limit_type> {
        
        
        std::vector<std::string> tags;
        std::string name = "";
        RE::TESGlobal* fameGlobal = nullptr;
        

    public:
        using limit = bound<short, fame::limit_type>;
        template <typename T>
        fameInfo(SKSE::SerializationInterface* evt, T* glob = nullptr) {
            if (glob) {
                fameGlobal = glob->As<RE::TESGlobal>();
            }
            std::size_t size;

            evt->ReadRecordData(size);
            tags.reserve(size);
            for (int i = 0; i < size; ++i) {
                tags.push_back(readString(evt));
            }
            name = readString(evt);
        };
        fameInfo(RE::TESGlobal* newForm, std::string name, int min, int max, std::vector<std::string> tags);
        void operator()(SKSE::SerializationInterface* evt);
        void operator()(short min, short max) { limit::operator()(min, max);
        };
        limit* getLimit() { return &static_cast<limit>(*this); };
        RE::TESGlobal* getGlobal() { return fameGlobal;}
    };
    using infoMap = std::map<RE::TESGlobal*, fameInfo>;
    infoMap* infoRelay = nullptr;

    
    class fameData : valueData<short, fame::limit_type>  {
    public:
        typedef valueData<short, fame::limit_type> value;
        using limit = bound<short, fame::limit_type>;
        value _value;
        limit localBound;
        fameInfo* info;
        fameData(){};
        fameData(fameInfo* tmpInfo) : info(tmpInfo), localBound(fame::limit_type::local, 0, 100), _value(0, *info->getLimit()) {};
        fameData(SKSE::SerializationInterface* evt);
        void operator()(SKSE::SerializationInterface* evt);
    };
    
    using fameMap = std::map<fameInfo*, fameData>;

}  // namespace gossip