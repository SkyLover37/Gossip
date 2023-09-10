namespace gossip {
    static int setInterest(RE::BGSLocation* fameLoc, int amt);
    static int addInterest(RE::BGSLocation* fameLoc, int amt);
    static int removeInterest(RE::BGSLocation* fameLoc, int amt);
    static int getInterest(RE::BGSLocation* fameLoc);

    static int setTolerance(RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
    static int getTolerance(RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);
    static int addTolerance(RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
    static int removeTolerance(RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);

    static bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry) {
        Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
        return true;
    }
}  // namespace gossip