#include "raylib.h"
#include "saveload.hpp"
#include "button.hpp"

using namespace std;
using namespace filesystem;
using namespace nlohmann;


string saveDirectory = "saves";  // Save folder path

// Pages
enum AppPage {
    HOME,
    LIBRARY,
    FLASHCARD,
    CREATE_DECK,
};
AppPage currentPage = HOME;


// File Attribute
string fileName = "untitled";
int currentFileIndex = 0;
int selectedFileIndex = 0;
char saveAsName[128] = "";

// Card Attribute
vector<Flashcard>  flashcards;
int currentCardIndex = 0;
int selectedCardIndex = 0;
char frontInput[128] = "";
char backInput[128] = "";

// Libray Component
vector<Button> existingFile;
vector<Button> bFlashcardsFront;
vector<Button> bFlashcardsBack;

// Button objects
Button createButton(280, 230, 240, 60, "Create New File");
Button libraryButton(280, 320, 240, 60, "Library");
Button backButton(250, 500, 300, 50, "Back to Home");
Button saveCardButton(250, 400, 300, 50, "Save Card");
Button previousCardButton(57, 350, 80, 50, "<<");
Button finishDeckButton(250, 460, 300, 50, "Finish Deck");
Button loadButton(250, 500, 300, 50, "Load Selected Deck");

//=====================================================================================================================================
// SAVING AND LOADING
//=====================================================================================================================================



// Home Page
void DrawHomePage() {
    DrawTextCentered("Welcome", { 350,100,100,40 }, 50, BLACK);
    createButton.Draw();
    libraryButton.Draw();

    if (createButton.IsClicked()) {
        flashcards.clear();
        currentPage = CREATE_DECK;
    }
    if (libraryButton.IsClicked()) {
        currentPage = LIBRARY;
    }
}


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

}

// Main Loop
int main() {
    InitWindow(800, 600, "Flashcard App");
    SetTargetFPS(60);

    // Ensure save folder exists
    if (!exists(saveDirectory))
        create_directories(saveDirectory);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currentPage) {
        case HOME:
            DrawHomePage();
            break;
        case CREATE_DECK:
            DrawCreateDeckPage();
            break;
        case LIBRARY:
            DrawLibraryPage();
            break;
        case FLASHCARD:
            DrawFlashcardPage();
            break;
        /*case FINAL:
            DrawFinalPage();
            break;*/
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

