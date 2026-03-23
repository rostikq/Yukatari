//
// Created by user on 2/24/2026.
//

#ifndef YUKATARI_DEBUG_H
#define YUKATARI_DEBUG_H

#define DEBUG
#include <iostream>
#include <format>
#include <cxxabi.h>

#ifdef DEBUG

#define DEBUG_CLOG(ptr ,msg) debugCLOG(this, msg);
#else
#define DEBUG_CLOG(ptr, msg)
#endif

template<typename T1, typename T2>
void debugCLOG(T1* obj, T2&& msg) {
    char* demangledName = abi::__cxa_demangle(typeid(obj).name(), nullptr, nullptr, nullptr);
    std::cout << std::format("[{}] {}\n", demangledName, msg);
}

#endif //YUKATARI_DEBUG_H