#include "raylib.h"
#include "saveload.hpp"
#include "tech.hpp"

using namespace std;
using namespace filesystem;
using namespace nlohmann;


string saveDirectory = "saves";  // Save folder path

// Pages
enum AppPage {
    HOME,
    CREATE,
    LIBRARY,
    FLASHCARD,
};
AppPage currentPage = HOME;

// File Attribute
string currentFile = "untitled";
string currentFolder = "Default";

int currentFileIndex = 0;
int selectedFileIndex = 0;
char saveFileName[128] = "";

int currentFolderIndex = 0;
int selectedFolderIndex = 0;
char saveFolderName[128] = "";

// Card Attribute
vector<Flashcard>  flashcards;
int currentCardIndex = 0;
int selectedCardIndex = 0;
char frontInput[128] = "";
char backInput[128] = "";

// Saving
vector<Button> existingFile;
vector<Button> bFlashcardsFront;
vector<Button> bFlashcardsBack;
vector<FlipCard> bFlashcards;

// General object
Button bHome(1110, 20, 60, 60, "HOME");
Button bSetting(1180, 20, 60, 60, "SET");

// HOMEPAGE object
FlipCard WELCOME(140, 120, 800, 450, "WELCOME", 50, "", 30);
Button bNew(1000, 430, 240, 60, "New");
Button bLibrary(1000, 510, 240, 60, "Library");


/*Void DrawSettingPopUp() {

}*/


// Home Page
void DrawHomePage() {
    WELCOME.DrawFlipV(GetFrameTime());
    WELCOME.StartFlip();
    if (!WELCOME.IsFront() && !WELCOME.IsFlipping()) {
        DrawTextCentered("Click[New] to create a new deck", { 140, 290, 800, 50 }, 50, BLUE);
        DrawTextCentered("Click [Library] to view your storage", { 140, 350, 800, 50 }, 50, BLUE);
    }

    bHome.Draw();
    bSetting.Draw();
    bNew.Draw();
    bLibrary.Draw();
    
    if (bNew.IsClicked()) currentPage = CREATE;
    if (bLibrary.IsClicked()) currentPage = LIBRARY;
    //if (bSetting.IsClicked()) { DrawSettingPopUp(); }
}

// Creation Page
InputBox ibFileName(40, 50, 1000, 60, "Save as", 30);
InputBox ibInputFront(40,540,1000,60,"Enter Front Page", 30);
InputBox ibInputBack(40, 620, 1000, 60, "Enter Front Page", 30);
void DrawCreatePage() {
    bHome.Draw();
    bSetting.Draw();

    ibFileName.Draw();
    ibInputFront.Draw();
    ibInputBack.Draw();

    /*DrawRectangle(40, 50, 1000, 60, LIGHTGRAY);
    DrawRectangleLinesEx({ 40, 50, 1000, 60 }, 3, BLACK);

    if (strlen(saveFileName) == 0) DrawTextMiddle("Save as ...", { 40, 50, 1000, 60 }, 30, BLACK);
    else DrawTextMiddle(saveFileName, { 40, 50, 1000, 60 }, 30, BLACK);
    if (is_mouse_hovered({ 40, 50, 1000, 60 })) {
        int key = GetKeyPressed();
        if (key != 0 && key != KEY_ENTER && key != KEY_BACKSPACE && strlen(saveFileName) < 128) {
            saveFileName[strlen(saveFileName)] = (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(saveFileName) > 0) {
            saveFileName[strlen(saveFileName) - 1] = '\0';
        }
    }*/
    if (bHome.IsClicked()) currentPage = HOME;
}

// Library Page
void DrawLibraryPage() {
    bHome.Draw();
    bSetting.Draw();

    if (bHome.IsClicked()) currentPage = HOME;
}

/*
// Create Deck Page
void DrawCreateDeckPage() {
    DrawTextCentered("Create New Deck", { 200, 60, 400, 60 }, 30, DARKBLUE);

    if (strlen(frontInput) == 0) DrawTextMiddle("Enter Front:", { 57, 177, 686, 46 }, 100, BLACK);
    DrawRectangle(50, 170, 700, 60, LIGHTGRAY);
    DrawRectangle(57, 177, 686, 46, WHITE);
    DrawTextMiddle(frontInput, { 57, 177, 686, 46 }, 100, BLACK);
    if (is_mouse_hovered({ 57, 177, 686, 46 })) {
        int key = GetKeyPressed();
        if (key != 0 && key != KEY_ENTER && key != KEY_BACKSPACE && strlen(frontInput) < 128) {
            frontInput[strlen(frontInput)] = (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(frontInput) > 0) {
            frontInput[strlen(frontInput) - 1] = '\0';
        }
    }
    
    if (strlen(backInput) == 0) DrawTextMiddle("Enter Back:", { 57, 277, 686, 46 }, 100, BLACK);
    DrawRectangle(50, 270, 700, 60, LIGHTGRAY);
    DrawRectangle(57, 277, 686, 46, WHITE);
    DrawTextMiddle(backInput, { 57, 277, 686, 46 }, 100, BLACK);
    if (is_mouse_hovered({ 57, 277, 686, 46 })) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        int key = GetKeyPressed();
        if (key != 0 && key != KEY_ENTER && key != KEY_BACKSPACE && strlen(backInput) < 128) {
            backInput[strlen(backInput)] = (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(backInput) > 0) {
            backInput[strlen(backInput) - 1] = '\0';
        }
    }

    if (strlen(saveAsName) == 0) DrawTextCentered("File Name", { 57, 377, 500, 40 }, 100, BLACK);
    else DrawTextCentered(saveAsName, { 57, 377, 500, 40 }, 100, BLACK);
    if (is_mouse_hovered({ 57, 377, 500, 40 })) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        int key = GetKeyPressed();
        if (key != 0 && key != KEY_ENTER && key != KEY_BACKSPACE && strlen(saveAsName) < 128) {
            saveAsName[strlen(saveAsName)] = (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(saveAsName) > 0) {
            saveAsName[strlen(saveAsName) - 1] = '\0';
        }
    }

    previousCardButton.Draw();
    saveCardButton.Draw();
    finishDeckButton.Draw();

    // go back to prervious card
    if (previousCardButton.IsClicked() && flashcards.size()>0 && currentPage == CREATE_DECK) {
        memset(frontInput, 0, sizeof(frontInput)); // empty the string
        memset(backInput, 0, sizeof(backInput));
        strcpy_s(frontInput, flashcards[flashcards.size() - 1].getFront().c_str());
        strcpy_s(backInput, flashcards[flashcards.size() - 1].getBack().c_str());
        flashcards.pop_back();
    }

    // save the current card
    if (saveCardButton.IsClicked() && currentPage == CREATE_DECK) {
        flashcards.emplace_back(frontInput, backInput); // function the same with put_back() but more stable
        memset(frontInput, 0, sizeof(frontInput)); // empty the string
        memset(backInput, 0, sizeof(backInput));
    }

    // save the current card deck
    if (finishDeckButton.IsClicked() && currentPage == CREATE_DECK) {
        if (strlen(saveAsName) > 0) fileName = saveAsName;
        if (!fileName.ends_with(".json")) fileName += ".json";
        SaveFlashcardToJson(flashcards, saveDirectory, fileName);
        currentPage = LIBRARY;
    }

    // back to HOME
    backButton.Draw();
    if (backButton.IsClicked() && currentPage == CREATE_DECK) {
        currentPage = HOME;
    }
}

void DrawLibraryPage() {
    DrawText("Library - Select a Deck", 200, 50, 30, DARKBLUE);
    vector<string> savedFiles = GetSavedFiles(saveDirectory);
    existingFile.clear();
    int yOffset = 60;
    // List the existing files
    for (size_t i = 0; i < savedFiles.size(); ++i) {
        existingFile.emplace_back(10, yOffset, 120, 30, savedFiles[i].erase(savedFiles[i].find_last_of("."), string::npos));
        existingFile[i].Draw();
        yOffset += 40;
    }

    // Card Deck select
    for (int i = 0; i < existingFile.size(); ++i)
        if (existingFile[i].IsClicked() && currentPage == LIBRARY) {
            flashcards = LoadFlashcardFromJson(saveDirectory, existingFile[i].getLabel() + ".json");
            currentPage = FLASHCARD; 
        }

    // Draw Back button
    backButton.Draw();
    if (backButton.IsClicked() && currentPage == LIBRARY) {
        currentPage = HOME;
    }
}

void DrawFlashcardPage() {
    bFlashcardsFront.clear();
    bFlashcardsBack.clear();
    bool showBack = false;
    int xOffset = 10;
    int yOffset = 10;

    // Get Data from Flashcard-object to Button-object
    for (size_t i = 0; i < flashcards.size(); ++i) {
        bFlashcardsFront.emplace_back(100,yOffset,200,50,flashcards[i].getFront()); // x , y, width, height, text/content
        bFlashcardsBack.emplace_back(100 + xOffset,yOffset,200,50,flashcards[i].getBack());
    }

    // Draw the Flashcards as Button-object
    for (size_t i = 0; i < flashcards.size(); ++i) {
        bFlashcardsFront[i].Draw();
        if (showBack) bFlashcardsBack[i].Draw();
    }

    // Draw Next and Back button


    int yOffset = 70;
    for (size_t i = 0; i < flashcards.size(); ++i) {
        bFlashcards.emplace_back(100, yOffset, 200, 50, flashcards[i].getFront(), flashcards[i].getBack()); // x , y, width, height, text/content
    }
    for (size_t i = 0; i < flashcards.size(); ++i) {
        bFlashcards[i].StartFlip();
        bFlashcards[i].Draw(GetFrameTime());
    }

}*/

// Main Loop
int main() {
    InitWindow(1280, 720, "Flashcard App");
    SetTargetFPS(60);

    // Ensure save folder exists
    if (!exists(saveDirectory))
        create_directories(saveDirectory);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawRectangleLinesEx({ 3,3,1274,714 }, 3, BLACK);
        switch (currentPage) {
        case HOME:
            DrawHomePage();
            break;
        case CREATE:
            DrawCreatePage();
            break;
        case LIBRARY:
            DrawLibraryPage();
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

