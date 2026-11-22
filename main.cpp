#include <raylib.h>

void RenderMap(int map[32][32], int nRows, int nCols, int tileSize);

int main() {

    //MAP
    int map[32][32] = {0};
    map[0][0] = 1;
    map[31][31] = 2;

    int nRows = sizeof(map) / sizeof(map[0]);
    int nCols = sizeof(map[0]) / sizeof(map[0][0]);
    int tileSize = 24;

    //WINDOW
    int scrWidth = nCols * tileSize;
    int scrHeight = nRows * tileSize;
    InitWindow(scrWidth, scrHeight, "A* Pathfinding algorithm");
    SetTargetFPS(60);

    //MAIN LOOP
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        RenderMap(map, nRows, nCols, tileSize);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void RenderMap(int map[32][32], int nRows, int nCols, int tileSize) {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            switch (map[i][j]) {
                case 0:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, WHITE);
                    break;
                case 1:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, RED);
                    break;
                case 2:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, GREEN);
                    break;
                default:
                    break;
            }
        }
    }
}