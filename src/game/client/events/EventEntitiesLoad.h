//
// Created by user on 3/18/2026.
//

#ifndef YUKATARI_EVENTENTITIESLOAD_H
#define YUKATARI_EVENTENTITIESLOAD_H
#include <vector>

#include "../../../core/IEvent.h"
#include "../../common/world/Entity.h"


struct EventEntitiesLoad : IEvent{
    size_t entitiesSize = 0;
    std::vector<EntitySnapshot> snapshots;
};


#endif //YUKATARI_EVENTENTITIESLOAD_H