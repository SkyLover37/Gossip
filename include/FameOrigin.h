#pragma once
#include <valueData.h>
namespace gossip {
    struct default_limit_tag {};
    
    template <typename T>
    struct bound {
        T _min;
        T _max;
        bound() : _min(std::numeric_limits<T>::min()), _max(std::numeric_limits<T>::max()){};
        bound(T min, T max) : _min(min), _max(max) {}
        
        bound(SKSE::SerializationInterface* evt) {
            evt->ReadRecordData(_min);
            evt->ReadRecordData(_max);
        };
        void save(SKSE::SerializationInterface* evt) {
            evt->WriteRecordData(_min);
            evt->WriteRecordData(_max);
        }
        void setLimits(T min, T max) {
            _min = min;
            _max = max;
        }
        T min() { return _min; }
        T max() { return _max; }
        T min(T val) { return val < _min ? _min : val; }
        T max(T val) { return val > _max ? _max : val; }
        T clamp(T val) { return min(max(val)); }
    };

    typedef bound<std::uint16_t> fameLimit_t;
    struct fameLimit : fameLimit_t {
        enum limit_type { local = 'LOCL', regional = 'REGL' };
        limit_type type = limit_type::regional;
        //fameLimit(){};
        fameLimit(limit_type type, std::uint16_t min, std::uint16_t max) : type(type), fameLimit_t(min, max){};
        fameLimit(SKSE::SerializationInterface* evt) : fameLimit_t(evt) {
            char store;
            evt->ReadRecordData(store);
            
            switch (store) {
                case 'L': {
                    type = limit_type::local;
                    break;
                }
                case 'R': {
                    type = limit_type::regional;
                    break;
                }
            }
        };
        void setLimits(std::uint16_t min, std::uint16_t max) { 
            fameLimit_t::setLimits(min, max);
        }
        void save(SKSE::SerializationInterface* evt) {
            fameLimit_t::save(evt);
            type == local ? evt->WriteRecordData('L') : evt->WriteRecordData('R');
            
        }
        bool operator==(limit_type type) { return this->type == type; }
    };
    
    class fameInfo : fameLimit {
    public:
        
        std::vector<std::string> tags;
        std::string name = "";
        RE::TESGlobal* fameGlobal = nullptr;
        
        fameInfo(SKSE::SerializationInterface* evt) : fameLimit(evt) {
           
            readForm(evt, fameGlobal);
            std::size_t size;

            evt->ReadRecordData(size);
            tags.reserve(size);
            for (int i = 0; i < size; i++) {
                std::string temp = readString(evt); 
                tags.push_back(temp);
            }

            name = readString(evt);
        };
        fameInfo(RE::TESGlobal* newForm, std::string name, std::uint16_t min, std::uint16_t max, std::vector<std::string> tags);
        void save(SKSE::SerializationInterface* evt);
        fameLimit* getLimit() { return static_cast<fameLimit*>(this); };
        RE::TESGlobal* getGlobal() { return fameGlobal;}
        std::string& getName() { return name; }
        bool hasTag(std::string& tag) { 
            boost::algorithm::to_lower(tag);
            auto entry = std::find(tags.begin(), tags.end(), tag);
            return entry != tags.end();
        }
        bool operator!() { 
            return !fameGlobal; }
    };
    using infoMap = std::map<RE::TESGlobal*, fameInfo>;
    extern infoMap* infoRelay;

    
    class fameData {
    public:
        
        std::uint16_t raw = 0;
        std::uint16_t val = 0;
        fameLimit* limit = nullptr;
        std::uint16_t reqGossip = 0;
        std::uint16_t _gossip = 0;
        fameLimit localBound;
        fameInfo* info = nullptr;
        fameData(fameInfo* tmpInfo) : info(tmpInfo), localBound(fameLimit::limit_type::local, 0, 100), limit(tmpInfo->getLimit()) {};
        fameData(fameInfo* tmpInfo, std::uint16_t min, std::uint16_t max)
            : info(tmpInfo), localBound(fameLimit::limit_type::local, min, max), limit(tmpInfo->getLimit()) {}
        fameData(SKSE::SerializationInterface* evt);
        
        void operator+=(fameData& data) { 
            val = limit->clamp(raw += data.raw);

            _gossip += data._gossip;
        }
        void operator-=(fameData& data) { 
            val = limit->clamp(raw += data.val);
            _gossip -= data._gossip;
        }
        void operator=(fameData& data) {
            val = limit->clamp(raw += data.val);
            _gossip = data._gossip;
        }
        void set(int data) { 
            int old = val;
            val = limit->clamp(raw = data);
            logger::debug("Setting {} to {} and is now {} clamped to {}", info->getName(), data, raw, val);

            // update required gossip
            if (val != old) {
                reqGossip = (val / 2) * (1 + val);
            }
        }
        void mod(int data) { 
            int old = val;
            val = limit->clamp(raw += data);
            //update required gossip
            logger::debug("Modded {} by {} and is now {} clamped to {}", info->getName(), data, raw, val);
            if (val != old) {
                reqGossip = (val / 2) * (1 + val);
            }
        }
        operator int() { return val; }
        void save(SKSE::SerializationInterface* evt);
    };
    
    using fameMap = std::map<fameInfo*, fameData>;

}  // namespace gossip