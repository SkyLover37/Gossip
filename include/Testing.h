
void MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
                                         RE::TESObjectREFR* a_ref) {
    if (!a_ref) {
        a_vm->TraceStack("Object reference is None", a_stackID);
        return;
    }

    const auto find_nearest_vertex = [&]() {
        std::optional<RE::NiPoint3> pos = std::nullopt;

        const auto cell = a_ref->GetParentCell();
        if (!cell || !cell->navMeshes) {
            return pos;
        }

        auto& [navMeshes] = *cell->navMeshes;
        auto shortestDistance = std::numeric_limits<float>::max();

        for (const auto& navMesh : navMeshes) {
            for (auto& [location] : navMesh->vertices) {
                const auto linearDistance = a_ref->GetPosition().GetDistance(location);
                if (linearDistance < shortestDistance) {
                    shortestDistance = linearDistance;
                    pos.emplace(location);
                }
            }
        }

        return pos;
    };

    const auto nearestVertex = find_nearest_vertex();
    if (!nearestVertex) {
        a_vm->TraceForm(a_ref, "does not have a valid navmesh position", a_stackID);
        return;
    }

    const auto handle = a_ref->CreateRefHandle();
    SKSE::GetTaskInterface()->AddTask([handle, nearestVertex]() {
        if (const auto ref = handle.get(); ref) {
            ref->SetPosition(*nearestVertex);
        }
    });
}