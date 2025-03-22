#include "engine/application.h"

int main(int argc, char* argv[]) {
    Application app("FPS Game", 800, 600);
    app.run();
    return 0;
}