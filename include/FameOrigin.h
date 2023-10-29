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
        bound(const bound& data) {
            _min = data._min;
            _max = data._max;
        }
        bound(SKSE::SerializationInterface* evt) {
            evt->ReadRecordData(_min);
            evt->ReadRecordData(_max);
        };
        void operator()(SKSE::SerializationInterface* evt) {
            evt->WriteRecordData(_min);
            evt->WriteRecordData(_max);
        }
        void operator()(T min, T max) {
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
        limit_type type = limit_type::local;
        fameLimit(){};
        fameLimit(limit_type type, std::uint16_t min, std::uint16_t max) : type(type), fameLimit_t(min, max){};
        fameLimit(SKSE::SerializationInterface* evt) : fameLimit_t(evt) { evt->ReadRecordData(type); };
        void operator()(std::uint16_t min, std::uint16_t max) { 
            static_cast<fameLimit_t>(*this)(min, max);
        }
        void operator()(SKSE::SerializationInterface* evt) {
            static_cast<fameLimit_t>(*this)(evt);
            evt->WriteRecordData(type);
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
        void operator()(SKSE::SerializationInterface* evt);
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
        fameLimit* limit;
        
        std::uint16_t _gossip = 0;
        fameLimit localBound;
        fameInfo* info;
        fameData(fameInfo* tmpInfo) : info(tmpInfo), localBound(fameLimit::limit_type::local, 0, 100), limit(tmpInfo->getLimit()) {};
        fameData(fameInfo* tmpInfo, std::uint16_t min, std::uint16_t max) : info(tmpInfo), localBound(fameLimit::limit_type::local, min, max) {}
        fameData(SKSE::SerializationInterface* evt);
        fameData(const fameData& data) {
            limit = data.limit;
            localBound = data.localBound;
            info = data.info;
            val = limit->clamp(raw += data.val);
            _gossip = data._gossip;
        }
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
        void operator=(int data) { 
            val = limit->clamp(raw = data);
        }
        void operator+=(int data) {
            val = limit->clamp(raw += data);
        }
        void operator-=(int data) {
            val = limit->clamp(raw -= data);
        }
        operator int() { return val; }
        void operator()(SKSE::SerializationInterface* evt);
    };
    
    using fameMap = std::map<fameInfo*, fameData>;

}  // namespace gossip