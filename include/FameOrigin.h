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
            logger::debug("Loading fame limit");
            evt->ReadRecordData(_min);
            evt->ReadRecordData(_max);
        };
        void save(SKSE::SerializationInterface* evt) {
            logger::debug("Saving fame limit");
            evt->WriteRecordData(_min);
            evt->WriteRecordData(_max);
        }
        void setLimits(T min, T max) {
            logger::debug("Setting limits 2 {} {}", min, max);
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
            logger::debug("loading fameLimit object");
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
            logger::debug("setting limits {} {} ", min, max);
            fameLimit_t::setLimits(min, max);
        }
        void save(SKSE::SerializationInterface* evt) {
            logger::debug("saving fameLimit object");
            fameLimit_t::save(evt);
            evt->WriteRecordData(static_cast<char>(type));
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
                tags.push_back(readString(evt));
            }
            name = readString(evt);
        };
        fameInfo(RE::TESGlobal* newForm, std::string name, std::uint16_t min, std::uint16_t max, std::vector<std::string> tags);
        void save(SKSE::SerializationInterface* evt);
        fameLimit* getLimit() { return static_cast<fameLimit*>(this); };
        RE::TESGlobal* getGlobal() { return fameGlobal;}
        std::string& getName() { return name; }
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
        
        std::uint16_t _gossip = 0;
        fameLimit localBound;
        fameInfo* info = nullptr;
        fameData(fameInfo* tmpInfo) : info(tmpInfo), localBound(fameLimit::limit_type::local, 0, 100), limit(tmpInfo->getLimit()) {};
        fameData(fameInfo* tmpInfo, std::uint16_t min, std::uint16_t max) : info(tmpInfo), localBound(fameLimit::limit_type::local, min, max) {}
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
            val = limit->clamp(raw = data);
        }
        void mod(int data) { 
            val = limit->clamp(raw += data);
        }
        operator int() { return val; }
        void save(SKSE::SerializationInterface* evt);
    };
    
    using fameMap = std::map<fameInfo*, fameData>;

}  // namespace gossip