// raygui_example.c

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void)
{
    const int W = 1000, H = 700;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(W, H, "raygui quick UI");

    SetExitKey(0); // Disable ESC default window close

    SetTargetFPS(60);

    // UI state
    bool paused = false;
    int chkA = 1;
    int chkB = 0;
    int chkC = 1;
    
    bool cursorEnabled = true;

    float timeScale = 1.0f;

    // Optional style
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    while (!WindowShouldClose())
    {
        // --- Input ---
        if (IsKeyPressed(KEY_ESCAPE))
            paused = !paused;

        // Cursor policy: capture only while RMB is held and NOT paused
        if (!paused && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            if (cursorEnabled)
              DisableCursor();
              cursorEnabled = false;
        } else if (!paused && !cursorEnabled) {
            EnableCursor();
            cursorEnabled = true;
        }

        // Non-paused updates (your sim updates go here)
        if (!paused) {
            // update simulation using timeScale...
        }

        // --- Draw ---
        BeginDrawing();
        ClearBackground((Color){20, 20, 28, 255});

        // ======= In-game UI (bottom bar) =======
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        // Bottom panel strip
        //Rectangle panel = { 0, (float)sh - 120, (float)sw, 120 };
        //DrawRectangleRec(panel, (Color){0, 0, 0, 120});
        //GuiPanel(panel, "");

        // Layout helpers
        float x = 20;
        float y = sh - 100;

        // --- "Text on left, checkbox on right" triplet ---
        const char* txtA = "Show names";
        int twA = MeasureText(txtA, GuiGetStyle(DEFAULT, TEXT_SIZE));
        DrawText(txtA, (int)x, (int)y + 6, GuiGetStyle(DEFAULT, TEXT_SIZE), RAYWHITE);
        GuiCheckBox((Rectangle){ x + twA + 12, y, 24, 24 }, "", &chkA);

        const char* txtB = "Show distance";
        int twB = MeasureText(txtB, GuiGetStyle(DEFAULT, TEXT_SIZE));
        float yB = y + 34;
        DrawText(txtB, (int)x, (int)yB + 6, GuiGetStyle(DEFAULT, TEXT_SIZE), RAYWHITE);
        GuiCheckBox((Rectangle){ x + twB + 12, yB, 24, 24 }, "", &chkB);

        const char* txtC = "Show speed vectors";
        int twC = MeasureText(txtC, GuiGetStyle(DEFAULT, TEXT_SIZE));
        float yC = y + 68;
        DrawText(txtC, (int)x, (int)yC + 6, GuiGetStyle(DEFAULT, TEXT_SIZE), RAYWHITE);
        GuiCheckBox((Rectangle){ x + twC + 12, yC, 24, 24 }, "", &chkC);

        // --- Center strip: 3 buttons on each side around a label ---
        float centerY = y + 20;
        float bw = 64, bh = 34;
        float gap = 8;

        // Compute center label
        char label[64];
        snprintf(label, sizeof(label), "Time Scale: %.2fx", timeScale);
        int labelW = MeasureText(label, GuiGetStyle(DEFAULT, TEXT_SIZE));
        float centerX = sw * 0.5f;

        // Left buttons
        Rectangle bL3 = { centerX - labelW / 2.0f - gap - bw, centerY, bw, bh };
        Rectangle bL2 = { bL3.x - gap - bw, centerY, bw, bh };
        Rectangle bL1 = { bL2.x - gap - bw, centerY, bw, bh };

        // Right buttons
        Rectangle bR1 = { centerX + labelW / 2.0f + gap, centerY, bw, bh };
        Rectangle bR2 = { bR1.x + gap + bw, centerY, bw, bh };
        Rectangle bR3 = { bR2.x + gap + bw, centerY, bw, bh };

        if (GuiButton(bL1, "<<<")) timeScale *= 0.125f;   // /8
        if (GuiButton(bL2, "<<"))  timeScale *= 0.25f;    // /4
        if (GuiButton(bL3, "<"))   timeScale *= 0.5f;     // /2

        if (GuiButton(bR1, ">"))   timeScale *= 2.0f;
        if (GuiButton(bR2, ">>"))  timeScale *= 4.0f;
        if (GuiButton(bR3, ">>>")) timeScale *= 8.0f;

        // Clamp timeScale
        if (timeScale < 0.01f) timeScale = 0.01f;
        if (timeScale > 1.0e6f) timeScale = 1.0e6f;

        // Center label
        GuiLabel((Rectangle){ centerX - labelW / 2.0f, centerY + 4, (float)labelW, bh }, label);

        // ======= Pause overlay =======
        if (paused)
        {
            // Dim background
            DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 180});

            const char* title = "Paused";
            int titleW = MeasureText(title, 32);
            DrawText(title, (sw - titleW) / 2, sh / 2 - 140, 32, RAYWHITE);

            float mw = 320, mh = 220;
            Rectangle menu = { (sw - mw) / 2.0f, (sh - mh) / 2.0f, mw, mh };
            GuiPanel(menu, "");

            float bx = menu.x + 40;
            float by = menu.y + 56;
            float bwm = menu.width - 80;
            float bH = 42;

            if (GuiButton((Rectangle){ bx, by, bwm, bH }, "Resume [ESC]")) paused = false;
            by += bH + 12;
            if (GuiButton((Rectangle){ bx, by, bwm, bH }, "Settings")) {
                // open your settings screen…
            }
            by += bH + 12;
            if (GuiButton((Rectangle){ bx, by, bwm, bH }, "Quit")) break;

            if (!cursorEnabled)
              EnableCursor();
              cursorEnabled = true;
        }

        // Small hint
        DrawText("Hold RMB to look around (cursor captured). Press ESC for pause menu.",
                 10, 10, 18, (Color){200, 200, 200, 255});

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

