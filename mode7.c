#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef struct Frustum {
    Vector2 Far1;
    Vector2 Far2;
    Vector2 Near1;
    Vector2 Near2;
} Frustum;

Texture2D texSky;
Texture2D texMap;
Image imgMap;
Frustum frustum;

float fWorldX = 0.0f;
float fWorldY = 0.0f;
float fWorldA = 0.1f;
float fNear = 0.005f;
float fFar = 20.0f; 
float fFoVHalf = PI / 4.0f;
float fSpeed = 50.0f; 

static void DrawMode7Line(int);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - Mode7 Demo");
    SetTargetFPS(60);

    texSky = LoadTexture("resources/sky1.png");
    texMap = LoadTexture("resources/map1.png");
    imgMap = LoadImageFromTexture(texMap);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_Q)) fNear += 0.1f * fSpeed * GetFrameTime();
        if (IsKeyDown(KEY_A)) fNear -= 0.1f * fSpeed * GetFrameTime();
        
        if (IsKeyDown(KEY_W)) fFar += 0.1f * fSpeed * GetFrameTime();
        if (IsKeyDown(KEY_S)) fFar -= 0.1f * fSpeed * GetFrameTime();        

        if (IsKeyDown(KEY_Z)) fFoVHalf += 0.1f * fSpeed * GetFrameTime();
        if (IsKeyDown(KEY_X)) fFoVHalf -= 0.1f * fSpeed * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)) fWorldA += 1.0f * GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) fWorldA -= 1.0f * GetFrameTime();

        if (IsKeyDown(KEY_UP)) {
            fWorldX += cosf(fWorldA) * fSpeed * GetFrameTime();
            fWorldY += sinf(fWorldA) * fSpeed * GetFrameTime();
        }

        if (IsKeyDown(KEY_DOWN)) {
            fWorldX -= cosf(fWorldA) * fSpeed * GetFrameTime();
            fWorldY -= sinf(fWorldA) * fSpeed * GetFrameTime();
        }

        frustum.Far1.x = fWorldX + cosf(fWorldA - fFoVHalf) * fFar;
        frustum.Far1.y = fWorldY + sinf(fWorldA - fFoVHalf) * fFar;
        
        frustum.Near1.x = fWorldX + cosf(fWorldA - fFoVHalf) * fNear;
        frustum.Near1.y = fWorldY + sinf(fWorldA - fFoVHalf) * fNear;
        
        frustum.Far2.x = fWorldX + cosf(fWorldA + fFoVHalf) * fFar;
        frustum.Far2.y = fWorldY + sinf(fWorldA + fFoVHalf) * fFar;
        
        frustum.Near2.x = fWorldX + cosf(fWorldA + fFoVHalf) * fNear;
        frustum.Near2.y = fWorldY + sinf(fWorldA + fFoVHalf) * fNear;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
            DrawMode7Line(y);
        }

        EndDrawing();
    }

    UnloadTexture(texSky);
    UnloadTexture(texMap);
    UnloadImage(imgMap);

    CloseWindow();

    return 0;
}

static void DrawMode7Line(int y) {
    float fSampleDepth = (float)y / ((float)GetScreenHeight() / 2.0f);

    float fStartX = (frustum.Far1.x - frustum.Near1.x) / (fSampleDepth) + frustum.Near1.x;
    float fStartY = (frustum.Far1.y - frustum.Near1.y) / (fSampleDepth) + frustum.Near1.y;
    float fEndX = (frustum.Far2.x - frustum.Near2.x) / (fSampleDepth) + frustum.Near2.x;
    float fEndY = (frustum.Far2.y - frustum.Near2.y) / (fSampleDepth) + frustum.Near2.y;

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float fSampleWidth = (float)x / (float)GetScreenWidth();
        float fSampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
        float fSampleY = (fEndY - fStartY) * fSampleWidth + fStartY;

        // fSampleX = fmod(fSampleX, (texMap.width + texMap.width));
        // fSampleX = fmod(fSampleX, texMap.width);
        // fSampleY = fmod(fSampleY, (texMap.height + texMap.height));
        // fSampleY = fmod(fSampleY, texMap.height);

        int texX = ((int)fSampleX % texMap.width + texMap.width) % texMap.width;
        int texY = ((int)fSampleY % texMap.height + texMap.height) % texMap.height;

        Color color = GetImageColor(imgMap, texX, texY);

        DrawPixel(x, (GetScreenHeight() / 2) + y, color);
    }
}