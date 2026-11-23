#include <raylib.h>
#include <cmath>
#include <climits>
#include <algorithm>

struct From {
    int row;
    int col;
};
struct Node {
    int id = 0;
    int fScore = -1;
    int gScore = -1;
    int hScore = -1;
    From from = {-1, -1};
};

void RenderMap(Node map[64][64], int nRows, int nCols, int tileSize);
void FindPath(Node map[64][64], int nRows, int nCols);
void ChangePlacement(Node map[64][64], int nRows, int nCols, int tileSize);
void ClearMap(Node map[64][64], int nRows, int nCols, char mouseBtn);

int main() {

    //NODE
    Node node;

    //MAP
    Node map[64][64] = {node};
    //map[32][22] = {1};
    //map[63][63] = {2};

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

        ChangePlacement(map, nRows, nCols, tileSize);
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
            Node & node = map[i][j];
            switch (node.id) {
                case 0:
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, WHITE);
                    break;
                case 1:
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, RED);
                    break;
                case 2:
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, GREEN);
                    break;
                case 3:
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, BLACK);
                    break;
                case 4:
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, ORANGE);
                    break;
                default:
                    break;
            }
        }
    }
}
void FindPath(Node map[64][64], int nRows, int nCols) {
    if (IsKeyPressed(KEY_SPACE)) {

        //START AND END NODE COORDS
        int startRow = -1;
        int startCol = -1;
        int endRow = -1;
        int endCol = -1;

        //FIND START AND END NODE
        for (int i = 0; i < nRows; i++) {
            for (int j = 0; j < nCols; j++) {
                if (map[i][j].id == 1) {
                    startRow = i;
                    startCol = j;
                } else if (map[i][j].id == 2) {
                    endRow = i;
                    endCol = j;
                }
            }
        }

        //CURRENT NODE
        int currRow = startRow;
        int currCol = startCol;
        Node & currNode = map[currRow][currCol];
        //CALCULATE G SCORE
        currNode.gScore = 0;
        //CALCULATE H SCORE
        int dx = abs(currCol - endCol);
        int dy = abs(currRow - endRow);
        currNode.hScore = 14 * std::min(dx, dy) + 10 * abs(dx - dy);
        //CALCULATE F SCORE
        currNode.fScore = currNode.gScore + currNode.hScore;

        //DO UNTIL CURRENT NODE IS AT END NODE
        while (currRow != endRow || currCol != endCol) {

            int fBest = INT_MAX;
            int nextRow = -1;
            int nextCol = -1;
            bool foundEnd = false;

            //CHECK NEIGHBORING NODES
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {

                    //SKIP CENTER NDOE
                    if (i == 0 && j == 0) {
                        continue;
                    } else {

                        //SCAN NODE
                        int scnRow = currRow + i;
                        int scnCol = currCol + j;
                        Node & scnNode = map[scnRow][scnCol];

                        //CHECK IF SCAN NODE IS END NODE
                        if (scnRow == endRow && scnCol == endCol) {
                            foundEnd = true;
                            break;
                        }

                        //SET COST
                        int cost = -1;
                        if (i == 0 || j == 0) {
                            cost = 10;
                        } else {
                            cost = 14;
                        }

                        //CALCULATE G SCORE
                        scnNode.gScore = map[currRow][currCol].gScore + cost;
                        //CALCULATE H SCORE
                        int dx = abs(scnCol - endCol);
                        int dy = abs(scnRow - endRow);
                        scnNode.hScore = 14 * std::min(dx, dy) + 10 * abs(dx - dy);
                        //CALCULATE F SCORE
                        scnNode.fScore = scnNode.gScore + scnNode.hScore;

                        //CHECK IF BEST F SCORE
                        if (scnNode.fScore < fBest) {
                            fBest = scnNode.fScore;
                            nextRow = scnRow;
                            nextCol = scnCol;
                            scnNode.id = 4;
                        }
                    }
                }
                if (foundEnd) {
                    break;
                }
            }
            if (foundEnd) {
                break;
            }

            //REGISTER WHICH NODE IS BEING JUMPED FROM
            map[nextRow][nextCol].from = {currRow, currCol};

            //SET NEXT COORDINATES
            currRow = nextRow;
            currCol = nextCol;
        }
    }
}
void ChangePlacement(Node map[64][64], int nRows, int nCols, int tileSize) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ClearMap(map, nRows, nCols, 'L');
        int clickCol = GetMouseX() / tileSize;
        int clickRow = GetMouseY() / tileSize;
        map[clickRow][clickCol].id = 1;
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        ClearMap(map, nRows, nCols, 'R');
        int clickCol = GetMouseX() / tileSize;
        int clickRow = GetMouseY() / tileSize;
        map[clickRow][clickCol].id = 2;
    } else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        int clickCol = GetMouseX() / tileSize;
        int clickRow = GetMouseY() / tileSize;
        map[clickRow][clickCol].id = 3;
    }
}
void ClearMap(Node map[64][64], int nRows, int nCols, char mouseBtn) {
    
    //CHECK NODE TYPE TO ELIMINATE
    int toEliminate = -1;
    switch (mouseBtn) {
        case 'L':
            toEliminate = 1;
            break;
        case 'R':
            toEliminate = 2;
            break;
    }

    //ELIMINATE NODES OF TYPE AND ALL WALLS
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            Node & node = map[i][j];
            if (node.id == toEliminate) {
                node.id = 0;
            } else if (node.id == 4) {
                node.id = 0;
            }
        }
    }
}