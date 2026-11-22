#include <raylib.h>

int main() {

    //WINDOW
    const int SCR_WIDTH = 640;
    const int SCR_HEIGHT = 480;
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "A* Pathfinding algorithm");
    SetTargetFPS(60);

    //MAIN LOOP
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}