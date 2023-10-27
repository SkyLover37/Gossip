
#include <FameAlias.h>
namespace gossip {
   
    class Gossip {
    public:
        struct tolerance : bound<std::uint16_t> {
            typedef bound<std::uint16_t> limit;
            std::uint16_t raw = 0;
            std::uint16_t val = 0;
            tolerance(){};
            tolerance(std::uint16_t amt, std::uint16_t min, std::uint16_t max)
                : raw(amt), limit(min, max), val(clamp(raw)) {}
            tolerance(SKSE::SerializationInterface* evt) : limit(evt) {
                evt->ReadRecordData(raw);
                evt->ReadRecordData(val);
            }
            tolerance(std::uint16_t amt){};
            tolerance(const tolerance& data) : limit(static_cast<limit>(data)) {
                raw = data.raw;
                val = data.val;
            }
            void operator=(std::uint16_t amt) { val = clamp(raw = amt); }
            void operator+=(std::uint16_t amt) { val = clamp(raw += amt); }
            void operator-=(std::uint16_t amt) { val = clamp(raw -= amt); }
            operator int() { return val; }
            void operator()(SKSE::SerializationInterface* evt) {
                limit::operator()(evt);
                evt->WriteRecordData(raw);
                evt->WriteRecordData(val);
            }
            void operator()(std::uint16_t min, std::uint16_t max) { limit(min, max);
            }
            std::uint16_t getRaw() { return raw; }
        };
        std::string version = "0.0.0";
        bool active = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        infoMap fame;
        std::map<RE::BGSLocation*, std::map<RE::TESGlobal*, tolerance>> regionTolerance{};
        //std::map<RE::TESObjectREFR*, fameProfile> profiles;
        fameProfile profile;
        static Gossip& getSingleton() {
            // static Gossip* container;
            // if (!container) container = new Gossip;
            return o_gossip;
        }
        Gossip();
        
        RE::TESFaction* currentFaction();
        RE::BGSLocation* currentLocation();
        RE::BGSLocation* checkLocation(RE::BGSLocation* checkLoc, bool CheckLocation = true);
        fameInfo* operator[](RE::TESGlobal* global);
        
        fameData* getFameObj(RE::TESGlobal* global, RE::TESFaction * faction, RE::BGSLocation * loc);

        region* getRegionObj(RE::TESFaction * fac, RE::BGSLocation * loc);

        fameAlias* getAliasObj(RE::TESFaction * fac);
        bool setState(bool active);

        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    } static o_gossip;
    
}