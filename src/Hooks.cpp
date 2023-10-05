#include "Hooks.h"
//Code taken from DDNG
SINGLETONBODY(gossip::UpdateHook)

void gossip::UpdateHook::Setup() {
    if (!_hooked) {
        _hooked = true;
        REL::Relocation<std::uintptr_t> vtbl{RE::PlayerCharacter::VTABLE[0]};
       // Update_old = vtbl.write_vfunc(0x0AD, Update);
        auto& trampoline = SKSE::GetTrampoline();
        trampoline.create(64);
        onFrame_old = trampoline.write_call<5>(REL::RelocationID(NULL, 36544).address() + 0x160, reinterpret_cast<std::uintptr_t>(UpdateFrame));
        //onFrame_old =
        //    trampoline.write_call<5>(0x005e4dd0 + REL::Module::get().base(),
        //                                       reinterpret_cast<std::uintptr_t>(UpdateFrame));
        
    }
}


// this function is only called if no menu is open. It also looks like that it is not called when player is in free cam
// mode
void gossip::UpdateHook::Update(RE::Actor* a_actor, float a_delta) {
    static RE::Actor* loc_player = RE::PlayerCharacter::GetSingleton();
    if (a_actor == loc_player) {
        {
            //static NodeHider* loc_nodehider = NodeHider::GetSingleton();
            //loc_nodehider->Update(a_delta);
        logger::debug("Player update");
        }
    }
    Update_old(a_actor, a_delta);
}

// this function is only called if no menu is open. It also looks like that it is not called when player is in free cam
// mode
void gossip::UpdateHook::UpdateFrame() {
    //static RE::Actor* loc_player = RE::PlayerCharacter::GetSingleton();
    
    logger::debug("Frame update");
    onFrame_old();
    
}