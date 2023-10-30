
#include <FameAlias.h>
namespace gossip {
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
        void set(int data) { 
            val = clamp(raw = data);
        }
        void mod(int data) { 
            val = clamp(raw += data);
        }
        operator int() { return val; }
        void save(SKSE::SerializationInterface* evt) {
            limit::save(evt);
            evt->WriteRecordData(raw);
            evt->WriteRecordData(val);
        }
        void operator()(std::uint16_t min, std::uint16_t max) { limit(min, max); }
        std::uint16_t getRaw() { return raw; }
    };
    class Gossip {
    public:
       


        std::string version = "0.0.0";
        
        bool isActive = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        infoMap fame;
        std::map<RE::BGSLocation*, std::map<RE::TESGlobal*, tolerance>> regionTolerance{};
        RE::TESGlobal* interest = nullptr;
        RE::TESGlobal* recognition = nullptr;
        //Originally planned to allow for NPCs to have fame.
        //std::map<RE::TESObjectREFR*, fameProfile> profiles;
        fameProfile profile;
        static Gossip& getSingleton() {
            
            return o_gossip;
        }
        Gossip();
        bool setup(RE::TESGlobal* inte, RE::TESGlobal* recog) { 
            if (!inte || !recog) return false;
            
            interest = inte;
            auto entry = fame.find(inte);
            if (entry != fame.end()) fame.erase(interest);
            fame.insert(std::make_pair(interest, fameInfo(interest, "Interest", 0, 100, std::vector<std::string>())));
            
            recognition = recog;
            entry = fame.find(recognition);
            if (entry != fame.end()) fame.erase(recognition);
            fame.insert(std::make_pair(recognition, fameInfo(recognition, "Recognition", 0, 100, std::vector<std::string>())));
            return true;
        }

        RE::TESFaction* currentFaction();
        RE::BGSLocation* currentLocation();
        RE::BGSLocation* checkLocation(RE::BGSLocation* checkLoc, bool CheckLocation = true);
        fameInfo* operator[](RE::TESGlobal* global);
        
        fameData* getFameObj(RE::TESGlobal* global, RE::TESFaction * faction, RE::BGSLocation * loc);

        region* getRegionObj(RE::TESFaction * fac, RE::BGSLocation * loc);

        fameAlias* getAliasObj(RE::TESFaction * fac);

        tolerance* getToleranceObj(RE::BGSLocation* loc, RE::TESGlobal* glob);
        bool isReady() {
            
            return interest || recognition;
        }
        bool setActive(bool set) {
            return isReady() ? isActive = set : isActive = false;
        };



    }extern o_gossip;
    
    void onGameSaved(SKSE::SerializationInterface* evt);
    void onGameLoad(SKSE::SerializationInterface* evt);
    void onRevert(SKSE::SerializationInterface* evt);
}