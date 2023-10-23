
#include <serializationUtil.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;

namespace gossip {
    enum default_tag { defaulttag };
    template <typename T, typename B>
    struct bound {
        T _min;
        T _max;
        B _clamp;
        bound(B _clamp, T min, T max) : _clamp(_clamp), _min(min), _max(max) {}
        bound(SKSE::SerializationInterface* evt) {
            evt->ReadRecordData(_min);
            evt->ReadRecordData(_max);
            evt->ReadRecordData(_clamp);
        };
        virtual void operator()(SKSE::SerializationInterface* evt) {
            evt->WriteRecordData(_min);
            evt->WriteRecordData(_max);
            evt->WriteRecordData(_clamp);
        }
        void operator()(T min, T max) {
            _min = min;
            _max = max;
        }
        
        T min(T val) { return val < _min ? _min : val; }
        T max(T val) { return val > _max ? _max : val; }
        T clamp(T val) { return min(max(val)); }
    };

    template <typename T, typename B = default_tag>
    class valueData : public bound<T, B> {
        typedef bound<T, B> limit;

        // valueType type
        T raw;
        T val;

    public:
        valueData() : raw(0), val(0), limit(0, 100){};
        valueData(B limitType) : raw(0), val(0), limit(0, 100){};
        valueData(T val, limit _limit) : raw(0), val(0), limit(_limit){};
        valueData(B limitType, limit _limit) : raw(0), val(0), limit(_limit){};
        valueData(B limitType, T val, T min, T max) : raw(val), limit(val, min, max) {};
        valueData(SKSE::SerializationInterface* evt) : limit(evt) {
            evt->ReadRecordData(raw);
            evt->ReadRecordData(val);
        };
        void operator=(T amt) { val = limit::clamp(raw = amt); };

        void operator+=(T amt){ 
            raw += amt;
            val = limit::clamp(raw);
        };
        void operator-=(T amt) { 
            raw -= amt;
            val = limit::clamp(raw);
        };
        void operator=(limit data) { limit = data;};
        void operator()(T min, T max){limit = new limit(min, max)};
        operator T() const { return val; };
        T getRawValue() { return raw; };
        void operator()(SKSE::SerializationInterface* evt) {
            evt->WriteRecordData(raw);
            evt->WriteRecordData(val);
            limit::operator()(evt);
        };
        limit getLocalBound(){ return limit; };
    };
}  // namespace goszsip