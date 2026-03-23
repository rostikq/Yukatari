//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_IDYNAMIC_H
#define YUKATARI_IDYNAMIC_H


class IDynamic {
public:
    virtual ~IDynamic() = default;

    virtual void update(float dt) = 0;
};


#endif //YUKATARI_IDYNAMIC_H