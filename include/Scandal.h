#pragma once

#include <RE/Skyrim.h>
#include <SKSE\Interfaces.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;


void writeString(SKSE::SerializationInterface* evt, std::string string) {
    std::size_t size = string.length() + 1;
    evt->WriteRecordData(size);
    evt->WriteRecordData(string.data(), static_cast<std::uint32_t>(size));
}
std::string readString(SKSE::SerializationInterface* evt) {
    std::string name;
    std::size_t strings;
    evt->ReadRecordData(strings);
    name.reserve(strings);
    evt->ReadRecordData(name.data(), static_cast<std::uint32_t>(strings));
    return name;
}
RE::TESForm* readForm(SKSE::SerializationInterface* evt){
    RE::FormID oldForm;
    RE::FormID newForm = 0;
    evt->ReadRecordData(oldForm);
    evt->ResolveFormID(oldForm, newForm);
    logger::info("{:x},{:x}", oldForm, newForm);
    return RE::TESForm::LookupByID(newForm);
};
namespace gossip {
    class Gossip {
        
    public:
        bool busy = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        struct fameInfo {
            
            int max = 100;
            int min = 0;
            std::vector<std::string> tags;
            std::string name = "";
            RE::TESGlobal* fameGlobal = nullptr;
            fameInfo(){};
            fameInfo(SKSE::SerializationInterface* evt, RE::TESGlobal* glob) : fameGlobal(glob) {
 
                std::string name;
                std::vector<std::string> tags;
                
                
                evt->ReadRecordData(max);

                evt->ReadRecordData(min);

                std::size_t size;
                evt->ReadRecordData(size);
                for (int i = 0; i < size; ++i) {
                    
                    tags.push_back(readString(evt));
                }
                name = readString(evt);
            }
            fameInfo(RE::TESGlobal* newForm, std::string name, int min, int max, std::vector<std::string> tags)
                : fameGlobal(newForm), name(name), min(min), max(max), tags(tags) {
                logger::info("New fame {} ", name);
            }

            void save(SKSE::SerializationInterface* evt) { 
                
                evt->WriteRecordData(fameGlobal->GetLocalFormID());
                evt->WriteRecordData(max);
                evt->WriteRecordData(min);
                std::size_t size = tags.size();
                evt->WriteRecordData(size);
                for (int i = 0; i < tags.size(); i++) {
                    writeString(evt, tags[i]);
                }
                size = name.length() + 1;
                evt->WriteRecordData(size); 
                evt->WriteRecordData(name.data(), static_cast<std::uint32_t>(size));
                
                
            }
        };

        std::map<RE::TESGlobal*, fameInfo> fame;


        struct fameData {
            int fameValue{};
            int gossip{};
            bool localLimit{};
            int max = 100;
            int min = 0;
            fameData(){};
            fameData(SKSE::SerializationInterface* evt) { 
                evt->ReadRecordData(fameValue);
                evt->ReadRecordData(gossip);
                evt->ReadRecordData(localLimit);
                evt->ReadRecordData(max);
                evt->ReadRecordData(min);
            };
            void save(SKSE::SerializationInterface* evt) { 
                evt->WriteRecordData(fameValue);
                evt->WriteRecordData(gossip);
                evt->WriteRecordData(localLimit);
                evt->WriteRecordData(max);
                evt->WriteRecordData(min);
            };
        };
        struct region {
            int interest;
            std::map<RE::TESGlobal*, fameData> fame;
            region(){};
            region(SKSE::SerializationInterface* evt) { evt->ReadRecordData(interest);
                std::size_t size;
                evt->ReadRecordData(size);
                for (int i = 0; i < size; i++) {
                    RE::TESGlobal* glob = readForm(evt)->As<RE::TESGlobal>();
                    fame[glob] = fameData(evt);
                }
            }
            void save(SKSE::SerializationInterface* evt) {
                
                evt->WriteRecordData(interest);
                evt->WriteRecordData(fame.size());
                for (auto& fame : fame) {
                    evt->WriteRecordData(fame.first->formID);
                    fame.second.save(evt);
                }
            }
        };
        struct fameAlias {
            std::string name = "";
            RE::TESForm* form = nullptr;
            std::map<RE::BGSLocation*, region> known;
            fameAlias(){};
            fameAlias(SKSE::SerializationInterface* evt){
                name = readString(evt);
                form = readForm(evt);
                std::size_t size;
                evt->ReadRecordData(size);
                for (int i = 0; i < size; i++) {
                    RE::BGSLocation* loc = readForm(evt)->As<RE::BGSLocation>();
                    known[loc] = region(evt);
                }
            };
            fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {
                for (auto& entry : Gossip::getSingleton()->trackedLocations) {
                    addLocation(entry);
                }
            }
            void addLocation(RE::BGSLocation* akLoc) {
                for (auto& fameEntry : Gossip::getSingleton()->fame) {
                    known[akLoc].fame[fameEntry.first] = fameData();
                }
            }
            void save(SKSE::SerializationInterface* evt) { 
                if(!evt->OpenRecord('ALAS', 1)){
                
                }else{
                        writeString(evt, name);
                        evt->WriteRecordData(form->formID);
                        evt->WriteRecordData(known.size());
                        for (auto& knownEntry : known) {
                            evt->WriteRecordData(knownEntry.first->formID);
                            knownEntry.second.save(evt);
                        }
                    
                    }

            }
            
            void load(SKSE::SerializationInterface* evt) {
                std::uint32_t type;
                std::uint32_t version;
                std::uint32_t length;
                RE::FormID oldForm;
                bool error = false;
                evt->ReadRecordData(&oldForm, length);
                evt->ResolveFormID(oldForm, oldForm);
                form = RE::TESForm::LookupByID(oldForm);
                if (form) {
                    logger::info("{} :: {}", form->GetName(), oldForm);
                } else
                    logger::error("Failed to retrieve form :: {}", oldForm);
            }
            
        };
        fameAlias* currentFameAlias = nullptr;
        region* currentRegion = nullptr;
        std::map<RE::TESForm*, fameAlias> Alias{};
        std::map<RE::BGSLocation*, int> regionTolerance{};
        struct fameProfile {
            RE::TESObjectREFR* akActor;
            float viewingTime;
            int interest;
            int relation;
        };
        
        
        static Gossip* getSingleton() {
            static Gossip* container;
            if (!container) container = new Gossip;
            return container;
        }
        static bool newFameAlias(RE::StaticFunctionTag*, RE::TESForm* Alias, std::string aliasName);
        static bool newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, int min, int max,
                            std::vector<std::string> tags);
        static RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);
        static std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
        static std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
        static std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);

        static void setFameMin(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);
        static void setFameMax(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);

        static int setFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        static int addFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        static int removeFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                       RE::TESGlobal* fameGlobal, int amt);
        static int getFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);


        static bool newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc, std::string locName);
        static std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);
        static std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);

        
        static int setGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                             RE::TESGlobal* fameGlobal, int amt);
        static int addGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                             RE::TESGlobal* fameGlobal, int amt);
        static int removeGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal, int amt);
        static int getGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                             RE::TESGlobal* fameGlobal);

        static int setInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        static int addInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        static int removeInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        static int getInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc);
        
        static int setTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal, int amt);
        static int getTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal);
        static int addTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal, int amt);
        static int removeTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                   RE::TESGlobal* fameGlobal, int amt);

        static bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry) {
            Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
            return true;
        }
        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}