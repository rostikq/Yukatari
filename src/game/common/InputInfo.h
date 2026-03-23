//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_INPUTINFO_H
#define YUKATARI_INPUTINFO_H


struct InputInfo {
    int8_t x;
    int8_t y;

    bool operator==(const InputInfo& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const InputInfo& other) const {
        return !(*this == other);
    }
};



#endif //YUKATARI_INPUTINFO_H