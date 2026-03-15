#include <iostream>
#include "src/core/Application.h"
#include "src/game/client/ClientState.h"
#include "src/game/server/Client.h"
#include "src/game/server/ServerState.h"

int main() {
    Application application(640, 480, "Yukatari", 60);
    State* state = nullptr;
    char decision;
    std::cout <<
        "What do you want to run:\n" <<
        "1. Client\n"
        "2. Server\n";
    std::cin >> decision;
    switch (decision) {
        case '1':
            state = new ClientState(application);
            break;
        case '2':
            state = new ServerState(application);
            break;
    }

    application.setState(state);
    application.start();
    delete state;
    system("pause");
    return 0;
}
