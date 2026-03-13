#include <iostream>
#include "src/core/Application.h"

int main() {
    Application application(640, 480, "Yukatari", 60);
    application.start();
    system("pause");
    return 0;
}
