//
// Created by user on 3/13/2026.
//

#ifndef YUKATARI_IOBSERVER_H
#define YUKATARI_IOBSERVER_H
#include "IEvent.h"


class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void onEvent(IEvent& event) = 0;
};


#endif //YUKATARI_IOBSERVER_H