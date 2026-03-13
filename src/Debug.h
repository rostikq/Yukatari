//
// Created by user on 2/24/2026.
//

#ifndef YUKATARI_DEBUG_H
#define YUKATARI_DEBUG_H

#define DEBUG

#ifdef DEBUG

#define DEBUG_CLOG(ptr ,msg) std::cout << std::format("[{}] ", typeid(ptr).name()) << msg << std::endl;
#else
#define DEBUG_CLOG(ptr, msg)
#endif


#endif //YUKATARI_DEBUG_H