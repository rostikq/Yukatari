//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_EVENTENTITYTRANSFORMUPDATE_H
#define YUKATARI_EVENTENTITYTRANSFORMUPDATE_H


struct EventEntityTransformUpdate : IEvent {
    unsigned int id;
    float posX, posY, posZ;
    float rotation;
};


#endif //YUKATARI_EVENTENTITYTRANSFORMUPDATE_H