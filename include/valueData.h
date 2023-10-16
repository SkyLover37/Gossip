#pragma once
#include <FameOrigin.h>
namespace gossip {
    template <typename T>
    class valueData {
        //valueType type
        
        ts::integer<t> _raw = 0;
        
        ts::constraints::closed_interval<T> _limit;
        clamped<T, ts::constraints::closed_interval<T>> _value;
    public:
        valueData(T val = 0, T min = 0, T max = 100) : _limit(min, max), _value(val, _limit){};
        valueData(SKSE::SerializationInterface* evt);
        valueData operator=(int amt);
        valueData operator+=(int amt);
        valueData operator-=(int amt);
        valueData operator=(valueData& data);
        operator int() const;
        operator raw() const;
        operator clamped<T, ts::constraints::closed_interval<T>>() const;
        void save(SKSE::SerializationInterface* evt);
        int getRawValue();
        int getValue();

        

    };
}  // namespace goszsip