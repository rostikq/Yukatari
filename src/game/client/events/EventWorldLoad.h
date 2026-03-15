//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_EVENTWORLDLOAD_H
#define YUKATARI_EVENTWORLDLOAD_H
#include "../../../core/IEvent.h"
#include "../../common/world/MapInfo.h"


struct EventWorldLoad : IEvent {
        MapInfo mapInfo;
};


#endif //YUKATARI_EVENTWORLDLOAD_H