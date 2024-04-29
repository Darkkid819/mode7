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
float fFar = 50.0f; 
float fFoVHalf = PI / 4.0f;
float fSpeed = 100.0f; 
float skyOffset = 0.0f;

static void DrawMode7Line(int);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mode7 Demo");
    SetTargetFPS(60);

    texSky = LoadTexture("resources/sky1.png");
    texMap = LoadTexture("resources/map1.png");
    imgMap = LoadImageFromTexture(texMap);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_Q)) fNear += 0.1f * fSpeed * GetFrameTime();
        if (IsKeyDown(KEY_A)) fNear -= 0.1f * fSpeed * GetFrameTime();
        
        if (IsKeyDown(KEY_W)) fFar += 0.1f * fSpeed * GetFrameTime();
        if (IsKeyDown(KEY_S)) fFar -= 0.1f * fSpeed * GetFrameTime();        

        if (IsKeyDown(KEY_Z)) fFoVHalf += 0.1f * GetFrameTime();
        if (IsKeyDown(KEY_X)) fFoVHalf -= 0.1f * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT)){
            fWorldA += 1.0f * GetFrameTime();
            skyOffset += 1.0f * fSpeed * 1.5f * GetFrameTime();
        } 
        if (IsKeyDown(KEY_LEFT)) {
            fWorldA -= 1.0f * GetFrameTime();
            skyOffset -= 1.0f * fSpeed * 1.5f * GetFrameTime();   
        }

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

        skyOffset = fmod(skyOffset, texSky.width);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        Rectangle skySource = { skyOffset, 0, texSky.width, texSky.height };
        Rectangle skyDest = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
        DrawTexturePro(texSky, skySource, skyDest, (Vector2){0, 0}, 0.0f, WHITE);

        for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
            DrawMode7Line(y);
        }

        DrawLine(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(texSky);
    UnloadTexture(texMap);
    UnloadImage(imgMap);

    CloseWindow();

    return 0;
}

static void DrawMode7Line(int y) {
    float fSampleDepth = (float)y / ((float)SCREEN_HEIGHT / 2.0f );

    float fStartX = (frustum.Far1.x - frustum.Near1.x) / (fSampleDepth) + frustum.Near1.x;
    float fStartY = (frustum.Far1.y - frustum.Near1.y) / (fSampleDepth) + frustum.Near1.y;
    float fEndX = (frustum.Far2.x - frustum.Near2.x) / (fSampleDepth) + frustum.Near2.x;
    float fEndY = (frustum.Far2.y - frustum.Near2.y) / (fSampleDepth) + frustum.Near2.y;

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float fSampleWidth = (float)x / (float)SCREEN_WIDTH;
        float fSampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
        float fSampleY = (fEndY - fStartY) * fSampleWidth + fStartY;

        int texX = ((int)fSampleX % texMap.width + texMap.width) % texMap.width;
        int texY = ((int)fSampleY % texMap.height + texMap.height) % texMap.height;

        Color color = GetImageColor(imgMap, texX, texY);

        DrawPixel(x, (SCREEN_HEIGHT / 2) + y, color);
    }
}