cmake_minimum_required(VERSION 4.1)
project(Yukatari)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(Yukatari main.cpp
        src/core/Application.cpp
        src/core/Application.h
        src/Debug.h
        src/core/Timer.h
        src/core/State.h
        src/core/IEvent.h
        src/core/IObserver.h
        src/core/EventManager.cpp
        src/core/EventManager.h
        src/core/NetworkManager.cpp
        src/core/NetworkManager.h
        src/game/common/networking/PacketHeader.h
        src/game/common/networking/PacketPing.h
        src/game/server/ServerState.cpp
        src/game/server/ServerState.h
        src/game/server/Client.h
        src/game/client/ClientState.cpp
        src/game/client/ClientState.h
        src/game/common/World.cpp
        src/game/common/World.h
        src/game/common/world/Tile.h
        src/game/common/Camera.cpp
        src/game/common/Camera.h
        src/game/common/world/MapInfo.h
        src/game/server/networking/PacketWorldLoad.h
        src/game/common/WorldRenderer.cpp
        src/game/common/WorldRenderer.h
        src/game/client/events/EventWorldLoad.h
        src/game/common/events/EventApplicationTerminate.h
        src/game/common/world/Entity.cpp
        src/game/common/world/Entity.h
        src/game/common/world/IdGenerator.cpp
        src/game/common/world/IdGenerator.h
        src/game/server/networking/PacketEntitiesLoad.h
        src/game/client/events/EventEntitiesLoad.h
        src/game/common/world/EntityType.h
        src/game/common/resources/ResourceManager.cpp
        src/game/common/resources/ResourceManager.h
        src/game/common/resources/TextureName.h
        src/game/client/networking/PacketInputUpdate.h
        src/game/common/world/Player.cpp
        src/game/common/world/Player.h
        src/game/common/world/IDynamic.h
        src/game/common/InputInfo.h
        src/game/server/networking/PacketEntityTransformUpdate.h
        src/game/server/events/EventEntityTransformUpdate.h
        src/game/server/networking/PacketAttachPlayer.h
        src/game/client/events/EventAttachPlayer.h)

target_link_libraries(Yukatari sfml-graphics sfml-window sfml-system sfml-network)