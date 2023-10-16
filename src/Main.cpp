#pragma once
#include <stddef.h>
#include <Papyrus.h>
#include <versionlibdb.h>
#include <Hooks.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {
   
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }
        
        log->set_level(spdlog::level::level_enum::debug);
        log->flush_on(spdlog::level::level_enum::trace);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }

    
    void InitializeSerialization() {
        log::trace("Initializing cosave serialization...");
        auto* serde = GetSerializationInterface();
        serde->SetUniqueID(_byteswap_ulong('GSIP'));
        serde->SetSaveCallback(gossip::Gossip::onGameSaved);
        
        serde->SetRevertCallback(gossip::Gossip::onRevert);
        serde->SetLoadCallback(gossip::Gossip::onGameLoad);
        log::trace("Cosave serialization initialized.");
    }
    
    
    void InitializePapyrus() {
        log::trace("Initializing Papyrus binding...");
        if (GetPapyrusInterface()->Register(gossip::papyrusRegister)) {
            log::debug("Papyrus functions bound.");
        } else {
            stl::report_and_fail("Failure to register Papyrus bindings.");
        }
    }
    


    
    void InitializeMessaging() {
        if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
            switch (message->type) {
                // Skyrim lifecycle events.
                case MessagingInterface::kPostLoad: // Called after all plugins have finished running SKSEPlugin_Load.
                    // It is now safe to do multithreaded operations, or operations against other plugins.
                case MessagingInterface::kPostPostLoad: // Called after all kPostLoad message handlers have run.
                case MessagingInterface::kInputLoaded: // Called when all game data has been found.
                    break;
                case MessagingInterface::kDataLoaded: // All ESM/ESL/ESP plugins have loaded, main menu is now active.
                    // It is now safe to access form data.
               
                    break;

                // Skyrim game events.
                case MessagingInterface::kNewGame: // Player starts a new game from main menu.
                case MessagingInterface::kPreLoadGame: {  // Player selected a game to load, but it hasn't loaded yet.
                    // Data will be the name of the loaded save.
                    auto handler = RE::TESDataHandler::GetSingleton();
                    auto tempform = handler->LookupForm<RE::TESGlobal>(0x833, "Gossip.esp");
                    
                }
                case MessagingInterface::kPostLoadGame: // Player's selected save game has finished loading.
                    // Data will be a boolean indicating whether the load was successful.
                case MessagingInterface::kSaveGame: // The player has saved a game.
                    // Data will be the save name.
                case MessagingInterface::kDeleteGame: // The player deleted a saved game from within the load menu.
                    break;
            }
        })) {
            stl::report_and_fail("Unable to register message listener.");
        }
    }
}  // namespace

bool DumpSpecificVersion() {
    VersionDb db;

    // Try to load database of version 1.5.62.0 regardless of running executable version.
    if (!db.Load(1, 6, 640, 0)) {
        //_FATALERROR("Failed to load database for 1.5.62.0!");
        return false;
    }

    // Write out a file called offsets-1.5.62.0.txt where each line is the ID and offset.
    db.Dump("offsets-1.6.640.0.txt");
    //_MESSAGE("Dumped offsets for 1.5.62.0");
    return true;
}
/*
class UpdatePCHook {
public:
    static void Install() {
        REL::Relocation<std::uintptr_t> playerCharacterVTable{RE::VTABLE_PlayerCharacter[0]};
        UpdatePC = playerCharacterVTable.write_vfunc(0xAD, UpdatePCMod);
    }
private:
    static void UpdatePCMod(RE::PlayerCharacter* pc, float delta) {
        // do my stuff
        // call original function
        UpdatePC(pc, delta);
    }
    static inline REL::Relocation<decltype(UpdatePCMod)> UpdatePC;
};*/
/**
 * This if the main callback for initializing your SKSE plugin, called just before Skyrim runs its main function.
 *
 * <p>
 * This is your main entry point to your plugin, where you should initialize everything you need. Many things can't be
 * done yet here, since Skyrim has not initialized and the Windows loader lock is not released (so don't do any
 * multithreading). But you can register to listen for messages for later stages of Skyrim startup to perform such
 * tasks.
 * </p>
 */
SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);
    gossip::Gossip::getSingleton();

    Init(skse);
    InitializeMessaging();
    InitializeSerialization();
    InitializePapyrus();
    //gossip::UpdateHook::GetSingleton()->Setup();
    //DumpSpecificVersion();
    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
