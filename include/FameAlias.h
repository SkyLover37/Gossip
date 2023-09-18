
namespace gossip {
    
    struct fameAlias {
        std::string name = "";
        RE::TESForm* form = nullptr;
        
        typedef std::map<RE::BGSLocation*, region> regionList;

        
        regionList known;
        fameAlias(){};
        fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {}
        fameAlias(SKSE::SerializationInterface* evt) {
            name = readString(evt);
            form = readForm(evt);
            
            //std::string aliasName = readString(evt);
            std::size_t size;
            evt->ReadRecordData(size);
            for (int i = 0; i < size; i++) {
                RE::BGSLocation* loc = readForm(evt)->As<RE::BGSLocation>();
                known[loc] = region(evt);
            }
        };
        fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {
           
        }
        void save(SKSE::SerializationInterface* evt);
        int setInterest(RE::BGSLocation* fameLoc, int amt);
        int addInterest(RE::BGSLocation* fameLoc, int amt);
        int removeInterest(RE::BGSLocation* fameLoc, int amt);
        int getInterest(RE::BGSLocation* fameLoc);
    };
    struct fameProfile {
        RE::TESObjectREFR* akActor;
        float viewingTime;
        int depravity;
        int disGood;
        int disBad;
        std::map<fameAlias*, valueData> interest;
        fameProfile(RE::TESObjectREFR* akActor) : akActor(akActor) {}
        fameProfile(SKSE::SerializationInterface* evt) {
            akActor = readForm(evt)->As<RE::TESObjectREFR>();
            evt->ReadRecordData(viewingTime);
            evt->ReadRecordData(depravity);
            evt->ReadRecordData(disGood);
            evt->ReadRecordData(disBad);
            size_t size;
            evt->ReadRecordData(size);
            for (int i = 0; i < size; ++i) {
                
                fameAlias* alias = &Gossip::getSingleton()->Alias[readForm(evt)->As<RE::TESFaction>()];
                
                
                interest[alias] = valueData(evt);
            }
        }
        void save(SKSE::SerializationInterface* evt);
        int setInterest(RE::BGSLocation* fameLoc, int amt);
        int addInterest(RE::BGSLocation* fameLoc, int amt);
        int removeInterest(RE::BGSLocation* fameLoc, int amt);
        int getInterest(RE::BGSLocation* fameLoc);
    };
}  // namespace gossip