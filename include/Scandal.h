#pragma once

#include <RE/Skyrim.h>
#include <SKSE\Interfaces.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;

namespace gossip {
    class Gossip {
        
    public:
        std::vector<RE::BGSLocation*> trackedLocations;
        struct fameInfo {
            
            int max = 100;
            int min = 0;
            std::vector<std::string> tags;
            std::string name = "";
            RE::TESGlobal* fameGlobal = nullptr;
            fameInfo(){};
            fameInfo(RE::TESGlobal* newForm, std::string name, int min, int max, std::vector<std::string> tags)
                : fameGlobal(newForm), name(name), min(min), max(max), tags(tags) {}
            void save(SKSE::SerializationInterface* evt) { 
                evt->OpenRecord('MAX_', 1);
                evt->WriteRecordData(max);
                evt->OpenRecord('MIN_', 1);
                evt->WriteRecordData(min);
                evt->OpenRecord('TAGS', 1);
                std::size_t size = tags.size();
                evt->WriteRecordData(size);
                for (int i = 0; i < tags.size(); i++) {
                    evt->WriteRecordData(tags[i]);
                }
                evt->OpenRecord('NAME', 1);
                evt->WriteRecordData(name);
                evt->OpenRecord('GLOB', 1);
                evt->WriteRecordData(fameGlobal->formID);
                
            }
            void load(SKSE::SerializationInterface* evt) {
                std::uint32_t type;
                std::uint32_t version;
                std::uint32_t length;
                RE::FormID oldForm;
                bool error = false;

                while (!error && evt->GetNextRecordInfo(type, version, length)) {
                    if (type == 'MAX_') {
                        evt->ReadRecordData(max);
                    }
                    if (type == 'MIN_') {
                        evt->ReadRecordData(min);
                    }
                    if (type == 'TAGS') {
                        std::size_t size;
                        evt->ReadRecordData(size);
                        for (int i = 0; i < size; ++i) {
                            std::string elem;
                            evt->ReadRecordData(elem);
                            tags.push_back(elem);
                        }
                    }
                    if (type == 'NAME') {
                        evt->ReadRecordData(name);
                    }
                    if (type == 'GLOB') {
                        evt->ReadRecordData(&oldForm, length);
                        evt->ResolveFormID(oldForm, oldForm);
                        fameGlobal = RE::TESForm::LookupByID(oldForm)->As<RE::TESGlobal>();
                        if (fameGlobal) {
                            logger::info("{} :: {}", fameGlobal->GetName(), oldForm);
                        } else
                            logger::error("Failed to retrieve form :: {}", oldForm);
                    }
                }
                logger::info("{}, {}, {}, {}, {}", max, min, tags.size(), name, fameGlobal->GetName());
            }
        };

        std::map<RE::TESGlobal*, fameInfo> fame;


        struct fameData {
            fameInfo* info;
            int fameValue;
            int gossip;
            bool localLimit;
            int max = 100;
            int min = 0;
            fameData(fameInfo* newInfo) : info(newInfo) {  };
        };
        struct region {
            int interest;
            std::map<fameInfo, fameData> fame;
        };
        struct fameAlias {
            std::string name = "";
            RE::TESForm* form;
            region* currentRegion;
            std::map<RE::BGSLocation*, region> known;
            fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {
                for (auto& entry : Gossip::getSingleton()->trackedLocations) {
                    addLocation(entry);
                }
            }
            void addLocation(RE::BGSLocation* akLoc) {
                for (auto& fameEntry : Gossip::getSingleton()->fame) {
                    known[akLoc].fame[fameEntry.second] = fameData(&fameEntry.second);
                }
            }
            void save(SKSE::SerializationInterface* evt) { 
                evt->OpenRecord('ALAS', 1);
                evt->WriteRecordData(name);
                evt->WriteRecordData(form->formID);

            }
            void load(SKSE::SerializationInterface* evt) {
                std::uint32_t type;
                std::uint32_t version;
                std::uint32_t length;
                RE::FormID oldForm;
                bool error = false;

                while (!error && evt->GetNextRecordInfo(type, version, length)) {
                    if (type == 'ALAS') {
                        evt->ReadRecordData(&oldForm, length);
                        evt->ResolveFormID(oldForm, oldForm);
                        form = RE::TESForm::LookupByID(oldForm);
                        if (form) {
                            logger::info("{} :: {}", form->GetName(), oldForm);
                        } else
                            logger::error("Failed to retrieve form :: {}", oldForm);
                    }
                }
            }
        };
        fameAlias* currentFameAlias;
        region* currentRegion;
        std::map<RE::TESForm*, fameAlias> fameAlias;
        std::map<RE::BGSLocation*, int> regionTolerance;
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
        static bool newFameAlias(RE::StaticFunctionTag*, RE::TESForm* Alias, RE::BSString aliasName);
        static bool newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, RE::BSString fameName);
        static RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, RE::BSString globalName);
        static RE::BSString getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
        static std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
        static std::vector<RE::BSString> getAllFameNames(RE::StaticFunctionTag*);

        static void setFameMin(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);
        static void setFameMax(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);

        static int setFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        static int addFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        static int removeFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                       RE::TESGlobal* fameGlobal, int amt);
        static int getFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);


        static bool newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc, RE::BSString locName);
        static std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);
        static std::vector<RE::BSString> getAllLocationNames(RE::StaticFunctionTag*);

        
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


        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}