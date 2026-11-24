#include <raylib.h>
#include <cmath>
#include <climits>
#include <algorithm>
#include <vector>

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
        ClearBackground(LIGHTGRAY);
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
                case 0:     //FLOOR
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, WHITE);
                    break;
                case 1:     //START NODE
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, WHITE);
                    DrawCircle(j * tileSize + (tileSize / 2), i * tileSize + (tileSize / 2), (tileSize / 2), RED);
                    break;
                case 2:     //END NODE
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, WHITE);
                    DrawCircle(j * tileSize + (tileSize / 2), i * tileSize + (tileSize / 2), (tileSize / 2), GREEN);
                    break;
                case 3:     //WALL
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, BLACK);
                    break;
                case 4:     //PATH
                    DrawRectangle(j * tileSize, i * tileSize, tileSize - 1, tileSize - 1, WHITE);
                    DrawCircle(j * tileSize + (tileSize / 2), i * tileSize + (tileSize / 2), (tileSize / 4), GRAY);
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

        //STORE MOVES (COORDINATES)
        std::vector<From> path;

        //DO UNTIL CURRENT NODE IS AT END NODE
        while (currRow != endRow || currCol != endCol) {

            //DECLARE VARIABLES
            int fBest = INT_MAX;
            int nxtRow = -1;
            int nxtCol = -1;
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
                            scnNode.from = {currRow, currCol};
                            path.push_back(scnNode.from);
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
                            nxtRow = scnRow;
                            nxtCol = scnCol;
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

            //REGISTER WHICH NODE IS BEING JUMPED FROM AND SAVES COORDS IN STACK
            Node & nxtNode = map[nxtRow][nxtCol];
            nxtNode.from = {currRow, currCol};
            path.push_back(nxtNode.from);

            //SET NEXT COORDINATES
            currRow = nxtRow;
            currCol = nxtCol;
        }

        //FOLLOW FROM COORDS STARTING FROM END NODE
        while (path.size() > 1) {
            int lastRow = path.back().row;
            int lastCol = path.back().col;
            Node & pathNode = map[lastRow][lastCol];
            pathNode.id = 4;
            path.pop_back();
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
        default:
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