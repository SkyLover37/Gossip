
#include <Scandal.h>
namespace gossip {

    struct interest : bound<std::uint16_t> {
        typedef bound<std::uint16_t> limit;
        std::uint16_t raw;
        std::uint16_t val;
        interest(std::uint16_t amt, std::uint16_t min, std::uint16_t max) : raw(amt), limit(min, max), val(clamp(raw)) {} 
        interest(SKSE::SerializationInterface* evt) : limit(evt) {
            evt->ReadRecordData(raw);
            evt->ReadRecordData(val);
        }
        void operator=(std::uint16_t amt) { val = clamp(raw = amt); }
        void operator+=(std::uint16_t amt) { val = clamp(raw += amt);}
        void operator-=(std::uint16_t amt) { val = clamp(raw -= amt); }
        operator int() { return val; }
        void operator()(SKSE::SerializationInterface* evt) {
            limit::operator()(evt);
            evt->WriteRecordData(raw);
            evt->WriteRecordData(val);

        }
    };
    class region : public interest{
    public:
        RE::BGSLocation* tLoc;
        fameMap fameMap;
        region(RE::BGSLocation* tLoc) : tLoc(tLoc), interest(0, 0, 100) {};
        region(SKSE::SerializationInterface* evt);
        void operator()(SKSE::SerializationInterface* evt);
        fameData* operator[](fameInfo* info) {
            auto entry = fameMap.find(info);
            return entry != fameMap.end() ? &entry->second : nullptr;
        }
        bool operator!() { return tLoc; }
    };
    using regionMap = std::map<RE::BGSLocation*, region>;
}  // namespace gossip