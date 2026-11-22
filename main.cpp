#include <raylib.h>
#include <cmath>

struct Node {
    int id = 0;
    int g = INT_MAX;
    int h = 0;
};

void RenderMap(Node map[64][64], int nRows, int nCols, int tileSize);
void MakeWalls(Node map[64][64], int tileSize);
void FindPath(Node map[64][64], int nRows, int nCols);

int main() {

    //NODE
    Node node;

    //MAP
    Node map[64][64] = {node};
    map[0][0] = {1};
    map[63][63] = {2};

    int nRows = sizeof(map) / sizeof(map[0]);
    int nCols = sizeof(map[0]) / sizeof(map[0][0]);
    int tileSize = 12;

    //WINDOW
    int scrWidth = nCols * tileSize;
    int scrHeight = nRows * tileSize;
    InitWindow(scrWidth, scrHeight, "A* Pathfinding algorithm");
    SetTargetFPS(60);

    //MAIN LOOP
    while (!WindowShouldClose()) {

        MakeWalls(map, tileSize);
        FindPath(map, nRows, nCols);

        BeginDrawing();
        ClearBackground(BLACK);
        RenderMap(map, nRows, nCols, tileSize);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void RenderMap(Node map[64][64], int nRows, int nCols, int tileSize) {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            switch (map[i][j].id) {
                case 0:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, WHITE);
                    break;
                case 1:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, RED);
                    break;
                case 2:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, GREEN);
                    break;
                case 3:
                    DrawRectangle(j * tileSize, i * tileSize, 100, 100, BLACK);
                    break;
                default:
                    break;
            }
        }
    }
}
void MakeWalls(Node map[64][64], int tileSize) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        int clickCol = GetMouseY() / tileSize;
        int clickRow = GetMouseX() / tileSize;
        map[clickCol][clickRow].id = 3;
    }
}
void FindPath(Node map[64][64], int nRows, int nCols) {
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nRows; j++) {
            //DO SOMETHING
        }
    }
}