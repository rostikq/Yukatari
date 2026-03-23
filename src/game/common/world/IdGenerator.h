//
// Created by user on 3/17/2026.
//

#ifndef YUKATARI_IDGENERATOR_H
#define YUKATARI_IDGENERATOR_H
#include <queue>
#include <vector>


class IdGenerator {
    unsigned int m_currentId = 0;
    std::queue<unsigned int> m_availableIds;
    std::vector<unsigned int> m_usedIds;
public:
    IdGenerator() = default;
    ~IdGenerator() = default;

    unsigned int allocateId() {
        if (m_availableIds.empty()) {
            bool alreadyAllocated = false;
            do {
                for (auto it = m_usedIds.begin(); it != m_usedIds.end(); ++it) {
                    if (*it == m_currentId) {
                        alreadyAllocated = true;
                        m_currentId++;
                    }
                }
            }
            while (alreadyAllocated);

            m_usedIds.push_back(m_currentId);
            return m_currentId++;
        }
        else {
            unsigned int id = m_availableIds.front();
            m_availableIds.pop();
            m_usedIds.push_back(id);
            return id;
        }
    }

    void forceAllocateId(unsigned int id) {
        for (auto it = m_usedIds.begin(); it != m_usedIds.end(); ++it) {
            if (*it == id) {
                m_usedIds.erase(it);
            }
        }
        m_usedIds.push_back(id);
    }

    void returnId(unsigned int id) {
        m_availableIds.push(id);
        for (auto it = m_usedIds.begin(); it != m_usedIds.end(); ++it) {
            if (*it == id) {
                m_usedIds.erase(it);
            }
        }
    }

};


#endif //YUKATARI_IDGENERATOR_H