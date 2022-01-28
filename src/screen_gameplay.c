/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "easings.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int line = 0;

static int framesDuration = 60 * 10;

typedef enum GameplayState { SPACE, ARROW, CIRCLE, WAIT}GameplayState;
GameplayState gameplayState = SPACE;


typedef enum Key {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3} Key;
Key keys[4];
bool keyCorrect[4];

static void SetWait(void);
static void WaitUpdate(void);
static void SetSpace(void);
static void UpdateSpace(void);
static void DrawSpace(void);
static void SetCircles(void);
static void UpdateCircles(void);
static void DrawCircles(void);
static void GetRandomKeys(void);
static void UpdateKeys(void);
static void DrawKeys(void);
static void UpdateTimeBar(void);

static int currentKey = 0;
static int nextState = 0;
static Rectangle timeBarFront;
static Rectangle timeBarBack;

static Rectangle arrowSourceRect;
static Rectangle arrowDestRect;
static Vector2 arrowOrigin;

static Rectangle backgroundBar;
static Rectangle frontgroundBar;

static int radius;
static int radiusClick;
static Vector2 circlePosition;
static int currentCircle;
static int maxCircle;
Color circleColor;



static Texture2D arrowTex;


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    GetRandomKeys();
    
    arrowTex = LoadTexture("resources/arrow.png");
    
    arrowSourceRect.x = 0;
    arrowSourceRect.y = 0;
    arrowSourceRect.width = arrowTex.width;
    arrowSourceRect.height = arrowTex.height;
    
    arrowDestRect.x = 0;
    arrowDestRect.y = 0;
    arrowDestRect.width = arrowTex.width;
    arrowDestRect.height = arrowTex.height;
    
    arrowOrigin = (Vector2) {arrowSourceRect.width/2, arrowSourceRect.height/2};
    
    timeBarBack.width = 100;
    timeBarBack.height = 20;
    timeBarBack.x = 800 - timeBarBack.width - 20;
    timeBarBack.y = 20;
    
    timeBarFront.width = 90;
    timeBarFront.height = 15;
    timeBarFront.x = 800 - timeBarFront.width - 30;
    timeBarFront.y = 25;
    
    backgroundBar.width = 320;
    backgroundBar.height = 120;
    backgroundBar.x = 800/2 - backgroundBar.width/2;
    backgroundBar.y = 450/2 - backgroundBar.height/2;
    
    frontgroundBar.width = 300;
    frontgroundBar.height = 100;
    frontgroundBar.x = 800/2 - frontgroundBar.width/2;
    frontgroundBar.y = 450/2 - frontgroundBar.height/2;
    
    radius = 20;
    radiusClick = 100;
    circlePosition = (Vector2) {0, 0};
    maxCircle = 5;
    circleColor = PURPLE;
    circleColor.a = 200;
    
    gameplayState = WAIT;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    
    switch(gameplayState)
    {
        case WAIT:
        {
            WaitUpdate();
        }break;
        case ARROW:
        {
            UpdateKeys();
        }break;
        case SPACE:
        {
            UpdateSpace();
        }break;
        case CIRCLE:
        {
            UpdateCircles();
        }break;
    }
    
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    DrawTexture(backgroundTexture, 0, 0, WHITE);
    DrawTextEx(font, TextFormat("SCORE: %i", score), (Vector2){ 20, 90 }, font.baseSize*2, 4, MAROON);
    
    switch(gameplayState)
    {
        case WAIT:
        {
            DrawTextEx(font, "CODING...", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case ARROW:
        {
            DrawKeys();
        }break;
        case SPACE:
        {
            DrawSpace();
        }break;
        case CIRCLE:
        {
            DrawCircles();
        }break;
    }

}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    UnloadTexture(arrowTex);
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

static void SetWait()
{
    score++;
    framesCounter = 0;
    gameplayState = WAIT;
}

static void WaitUpdate()
{
    framesCounter++;
    
    if (framesCounter >= 5*60)
    {
        nextState = GetRandomValue(0, 2);
        if (nextState == 0)
        {
            SetSpace();
        }
        else if (nextState == 1)
        {
            gameplayState = CIRCLE;
            SetCircles();
        }
        else
        {
            gameplayState = ARROW;
            GetRandomKeys();
        }
        
        line = GetRandomValue(0, 3);
        framesCounter = 0;
    }
}

static void SetSpace()
{
    gameplayState = SPACE;
    frontgroundBar.width = 300;
}

static void UpdateSpace()
{
    frontgroundBar.width -= 300/(900 - framesDuration);
    
    if (IsKeyPressed(KEY_SPACE))
    {
        frontgroundBar.width += 15;
    }
    
    if (frontgroundBar.width <= 0)
        finishScreen = 1;
    else if (frontgroundBar.width >= backgroundBar.width - 10)
    {
        SetWait();
    }
}

static void DrawSpace()
{
    switch(line)
    {
        case 0:
        {
            DrawTextEx(font, "TIME FOR A BREAK!", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 1:
        {
        DrawTextEx(font, "EAT TIME!", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 2:
        {
        DrawTextEx(font, "*STARES SILENTLY TO THE VOID*", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 3:
        {
        DrawTextEx(font, "*STARTS TO PET THE CAT*", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
    }
    
    DrawTextEx(font, "PRESS THE SPACEBAR TO CONTINUE WORKING", (Vector2){ 150, 400}, font.baseSize*2, 4, RED);
    
    DrawRectangleRec(backgroundBar, GRAY);
    DrawRectangleRec(frontgroundBar, GREEN);
    DrawRectangleLinesEx(backgroundBar, 10, BLACK);
}

static void SetCircles()
{
    radiusClick = 100;
    framesCounter = 0;
    circlePosition.x = GetRandomValue(100, 700);
    circlePosition.y = GetRandomValue(50, 400);
}

static void UpdateCircles()
{
    if (IsMouseButtonPressed(0) && CheckCollisionPointCircle(GetMousePosition(), circlePosition, radius))
    {
        currentCircle++;
         if (currentCircle > maxCircle)
         {
             SetWait();
             currentCircle = 0;
         }
         else
             SetCircles();
    }
    
    framesCounter++;
    radiusClick = EaseLinearIn(framesCounter, 50, -50, framesDuration/maxCircle);
    
    if (radiusClick <= 0)
    {
        finishScreen = 1;
        framesCounter = 0;
    }
}

static void DrawCircles()
{
    switch(line)
    {
        case 0:
        {
            DrawTextEx(font, "LET'S CHECK TWITTER", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 1:
        {
            DrawTextEx(font, "*STARTS LOOKING FOOD VIDEOS*", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 2:
        {
            DrawTextEx(font, "I HAVE TO CLEAN THE ROOM...", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 3:
        {
            DrawTextEx(font, "*CHEKS BANK ACCOUNT*", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
    }
    
    DrawTextEx(font, "CLICK ON THE CIRCLES TO CONTINUE WORKING", (Vector2){ 140, 400  }, font.baseSize*2, 4, RED);
    
    DrawCircleV(circlePosition, radius, GREEN);
    DrawCircleV(circlePosition, radiusClick, circleColor);
    DrawCircleLines(circlePosition.x, circlePosition.y, radiusClick, PURPLE);
}

static void GetRandomKeys()
{
    currentKey = 0;
    
    for(int i = 0; i < 4; i++)
    {
        int a = GetRandomValue(0, 3);
        keys[i] = (enum Key) a;
        keyCorrect[i] = false;
    }
}

static void UpdateTimeBar()
{
    framesCounter++;
    timeBarFront.width = EaseLinearIn(framesCounter, 90, -90, framesDuration);
    
    if (timeBarFront.width <= 0)
        finishScreen = 1;
}



static void UpdateKeys()
{
    if (currentKey > 3)
    {
        currentKey = 0;
        
        SetWait();
        framesCounter = 0;
        return;
    }
    
    UpdateTimeBar();
    
    switch(keys[currentKey])
    {
        case UP:
        {
            if (IsKeyPressed(KEY_UP))
            {
                keyCorrect[currentKey] = true;
                currentKey++;
            }
        }       
        break;
        case DOWN:
        {
            if (IsKeyPressed(KEY_DOWN))
            {
                keyCorrect[currentKey] = true;
                currentKey++;
            }
        }
        break;
        case LEFT:
        {
            if (IsKeyPressed(KEY_LEFT))
            {
                keyCorrect[currentKey] = true;
                currentKey++;
            }
        }
        break;
        case RIGHT:
        {
            if (IsKeyPressed(KEY_RIGHT))
            {
                keyCorrect[currentKey] = true;
                currentKey++;
            }
        }
        break;
    }
    


}

static void DrawKeys()
{
    switch(line)
    {
        case 0:
        {
            DrawTextEx(font, "FIXING BUGS...", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 1:
        {
        DrawTextEx(font, "TESTING", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 2:
        {
        DrawTextEx(font, "SEARCHING IN STACKOVERFLOW", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
        case 3:
        {
        DrawTextEx(font, "CHECKING OLD CODE", (Vector2){ 20, 30 }, font.baseSize*3, 4, MAROON);
        }break;
    }
    
    DrawTextEx(font, "PRESS THE CORRECT KEYS TO CONTINUE WORKING", (Vector2){ 120, 400 }, font.baseSize*2, 4, RED);
    
    
    DrawRectangleRec(timeBarBack, GRAY);
    DrawRectangleRec(timeBarFront, GREEN);
    DrawRectangleLinesEx(timeBarBack, 5, BLACK);
    
    for(int i = 0; i < 4; i++)
    {
        arrowDestRect.x = 600/4 + 600/4 * i + arrowDestRect.width/2;
        arrowDestRect.y = 450/2 + arrowDestRect.height/2;
        
        switch(keys[i])
        {
            case UP:
            {
                DrawRectangle(arrowDestRect.x - arrowDestRect.width/2, arrowDestRect.y - arrowDestRect.height/2,
                arrowDestRect.width, arrowDestRect.height, BLUE);
                //DrawTextureEx(arrowTex, (Vector2) {600/4 + 600/4 * i, 450/2}, 0, 1, WHITE);
                
                if(!keyCorrect[i])
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 0, WHITE);
                else
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 0, GREEN);
            }       
            break;
            case DOWN:
            {
                DrawRectangle(arrowDestRect.x - arrowDestRect.width/2, arrowDestRect.y - arrowDestRect.height/2,
                arrowDestRect.width, arrowDestRect.height, RED);
                
                if(!keyCorrect[i])
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 180, WHITE);
                else
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 180, GREEN);
            }
            break;
            case LEFT:
            {
                DrawRectangle(arrowDestRect.x - arrowDestRect.width/2, arrowDestRect.y - arrowDestRect.height/2, 
                arrowDestRect.width, arrowDestRect.height, PINK);

                if(!keyCorrect[i])
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 270, WHITE);
                else
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 270, GREEN);
            }
            break;
            case RIGHT:
            {
                DrawRectangle(arrowDestRect.x - arrowDestRect.width/2, arrowDestRect.y - arrowDestRect.height/2, 
                arrowDestRect.width, arrowDestRect.height, YELLOW);
                
                if(!keyCorrect[i])
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 90, WHITE);
                else
                    DrawTexturePro(arrowTex, arrowSourceRect, arrowDestRect, arrowOrigin, 90, GREEN);
            }
            break;
        }
    }
}