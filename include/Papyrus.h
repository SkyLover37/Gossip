namespace gossip {
    
    bool newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName);
    void newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc);
    void newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, int min, int max,
                        std::vector<std::string> tags, bool force);
    RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);

    void addFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);
    bool removeFameTag(RE::StaticFunctionTag*, RE::TESGlobal, std::string valueTag);
    std::vector<std::string> getValueTags(RE::StaticFunctionTag*, RE::TESGlobal* valueGlobal);
    std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
    std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
    std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);

    std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);

    int getGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt, RE::TESFaction* fameAlias);
    int setGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                       RE::TESFaction* fameAlias);  
    int removeGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                          RE::TESFaction* fameAlias);  
    int addGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                       RE::TESFaction* fameAlias);

    int getFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                     RE::TESFaction* fameAlias);  
    int setFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                     RE::TESFaction* fameAlias);
    int removeFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                        RE::TESFaction* fameAlias);   
    int addFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                     RE::TESFaction* fameAlias);

    //std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);
    int setInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc, int amt);
    int addInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc, int amt);
    int removeInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc, int amt);
    int getInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc);

    int setTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
    int getTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);
    int addTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
    int removeTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);

    bool setCurrentRegion(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc);
    bool setCurrentAlias(RE::StaticFunctionTag*, RE::TESFaction* actorAlias);

    bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry) {
        Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
        Registry->RegisterFunction("newFameAlias", "GIP_SKSE", newFameAlias);
        Registry->RegisterFunction("newLocation", "GIP_SKSE", newLocation);
        Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        Registry->RegisterFunction("getValueTags", "GIP_SKSE", getValueTags);
        Registry->RegisterFunction("newFame", "GIP_SKSE", getFameName);
        Registry->RegisterFunction("getAllFameGlobals", "GIP_SKSE", getAllFameGlobals);
        Registry->RegisterFunction("getAllFameNames", "GIP_SKSE", getAllFameNames);
        Registry->RegisterFunction("getAllLocations", "GIP_SKSE", getAllLocations);
        Registry->RegisterFunction("getGossipValue", "GIP_SKSE", getGossipValue);
        Registry->RegisterFunction("setGossipValue", "GIP_SKSE", setGossipValue);
        Registry->RegisterFunction("removeGossipValue", "GIP_SKSE", removeGossipValue);
        Registry->RegisterFunction("addGossipValue", "GIP_SKSE", addGossipValue);
        Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        Registry->RegisterFunction("setFameValue", "GIP_SKSE", setFameValue);
        Registry->RegisterFunction("removeFameValue", "GIP_SKSE", removeFameValue);
        Registry->RegisterFunction("addFameValue", "GIP_SKSE", addFameValue);
        Registry->RegisterFunction("setInterest", "GIP_SKSE", setInterest);
        Registry->RegisterFunction("addInterest", "GIP_SKSE", addInterest);
        Registry->RegisterFunction("removeInterest", "GIP_SKSE", removeInterest);
        Registry->RegisterFunction("getInterest", "GIP_SKSE", getInterest);
        Registry->RegisterFunction("setTolerance", "GIP_SKSE", setTolerance);
        Registry->RegisterFunction("getTolerance", "GIP_SKSE", getTolerance);
        Registry->RegisterFunction("removeTolerance", "GIP_SKSE", removeTolerance);
        Registry->RegisterFunction("addTolerance", "GIP_SKSE", addTolerance);
        Registry->RegisterFunction("setCurrentRegion", "GIP_SKSE", setCurrentRegion);
        Registry->RegisterFunction("setCurrentAlias", "GIP_SKSE", setCurrentAlias);
        Registry->RegisterFunction("addFameTag", "GIP_SKSE", addFameTag);
        Registry->RegisterFunction("removeFameTag", "GIP_SKSE", removeFameTag);
        return true;
    }
}  // namespace gossip