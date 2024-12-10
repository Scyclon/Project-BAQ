#include "raylib.h"
#include "saveload.hpp"
#include "tech.hpp"
#include "utility.hpp"

using namespace std;
using namespace filesystem;
using namespace nlohmann;

#define bgColor {200,200,200,255}


// Saving
string saveDirectory = "saves";  // Save folder path
string currentFileName = "untitled";
string currentFolderName = "Default";

// Pages
enum AppPage {
    HOME,
    CREATE,
    LIBRARY,
    FLASHCARD,
};
AppPage currentPage = HOME;

// Input
int currentFileIndex = 0;
int selectedFileIndex = 0;

int currentFolderIndex = 0;
int selectedFolderIndex = 0;

string fullDirectory;
// Card Attribute
vector<Flashcard>  flashcards;
int currentCardIndex = 0;
int selectedCardIndex = 0;
char frontInput[40] = "";
char backInput[40] = "";
bool cardEditMode = false;
vector<string> existingFile;
vector<Button> bExistingFile;
//vector<Button> bFlashcardsFront;
//vector<Button> bFlashcardsBack;
vector<FlipCard> fFlashcards;

// General object
Button bHome(1110, 20, 60, 60, "HOME",WHITE,DARKBLUE);
Button bSetting(1180, 20, 60, 60, "SET",WHITE,DARKBLUE);

// HOMEPAGE object
FlipCard WELCOME(140, 120, 800, 450, "WELCOME", 50, "", 30);
Button bNew(1000, 430, 240, 60, "New",WHITE,BLUE);
Button bLibrary(1000, 510, 240, 60, "Library",WHITE,BLUE);


// Home Page
void DrawHomePage() {
    WELCOME.DrawFlipV(GetFrameTime());
    WELCOME.StartFlip();
    if (!WELCOME.IsFront() && !WELCOME.IsFlipping()) {
        DrawTextCentered("Click [New] to create a new deck", { 140, 290, 800, 50 }, 50, BLUE);
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

// Create-File
InputBox ibFileName(40, 50, 1000, 50, "Save as", 30);
InputBox ibInputFront(40, 560, 1000, 50, "Enter Front Page", 30);
InputBox ibInputBack(40, 630, 1000, 50, "Enter Back Page", 30);
Button bSaveCard(1080, 560, 160, 35, "SAVE", WHITE, BLUE);
Button bFinishDeck(1080, 602.5, 160, 35, "FINISH", WHITE, BLUE);
Button bFinishFile(1080, 602.5, 160, 35, "FINISH", WHITE, BLUE);
vector<string> cardFront;
vector<string> cardBack;
vector<string> existingFolder;
vector<Button> bExistingFolder;

bool isCreatingFile = true;
float initFileY = 130;
float boxFileHeight = 40;
float initFolderY = 130;
float boxFolderHeight = 40;
float offsetY = 0;
Roller rCreateFileRoller(1220, 120, 20, 400, (float)((boxFileHeight + 100)* flashcards.size() + 400), true);
Roller rFolderRoller(1220, 120, 20, 400, (float)((boxFolderHeight + 100)* existingFolder.size() + 400), true);

InputBox ibNewFolder(40, 560, 1000, 50, "New Folder", 30);



void DrawCreatePage() {
    for (const auto& saveFolderName : GetSavedFolder(saveDirectory))
        existingFolder.emplace_back(saveFolderName);
    for (size_t i = 0; i < existingFolder.size(); ++i)
        bExistingFolder.emplace_back(50, (float)(initFolderY + (i * boxFolderHeight * 1.5)), 1000, boxFolderHeight, existingFolder[i], WHITE, BLACK);

    // Flashcard display
    DrawRectangle(40, 120, 1200, 400, GRAY); // Draw frame

    if (isCreatingFile) {
        //offsetY = rCreateFileRoller.getOffset();
        if (!flashcards.empty()) {
            for (size_t i = 0; i < flashcards.size(); ++i) {
                if (((float)(initFileY + (i * 100)) + boxFileHeight - offsetY <= 560) && ((float)(initFileY + (i * 100)) - offsetY >= 80)) {
                    DrawRectangle(50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight, WHITE);
                    DrawTextMiddle(flashcards[i].getFront().c_str(), { 50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight }, 30, BLUE);
                }
                if (((float)(initFileY + (i * 100)) + boxFileHeight - offsetY <= 560) && ((float)(initFileY + (i * 100)) - offsetY >= 80)) {
                    DrawRectangle(50, (float)(initFileY + boxFileHeight + (i * 100)) - offsetY, 1000, boxFileHeight, WHITE);
                    DrawTextMiddle(flashcards[i].getBack().c_str(), { 50, (float)(initFileY + boxFileHeight + (i * 100)) - offsetY, 1000, boxFileHeight }, 30, BLUE);
                }
                if (((float)(initFileY + (i * 100)) + boxFileHeight * 2 - offsetY <= 600) && ((float)(initFileY + (i * 100)) - offsetY >= 80)) {
                    DrawRectangleLinesEx({ 50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight * 2 }, 3, BLACK);
                    DrawLineEx({ 50,(float)(initFileY + (i * 100)) + boxFileHeight - offsetY }, { 1050,(float)(initFileY + (i * 100)) + boxFileHeight - offsetY }, 3, BLACK);

                    // Edit-Mode
                    if (is_chosen({ 50, (float)(initFileY + (i * 100)) - offsetY, 950, boxFileHeight * 2 })) {
                        ibInputFront.SetText(flashcards[i].getFront());
                        ibInputBack.SetText(flashcards[i].getBack());
                        currentCardIndex = i;
                        cardEditMode = true;
                    }

                    // Delete chosen Card
                    DrawRecWithLines(1070, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight * 2, RED, 3);
                    if (is_chosen({ 1070, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight * 2 })) {
                        flashcards.erase(flashcards.begin() + i);
                        cardEditMode = false;
                    }
                }
            }
        }
        rCreateFileRoller.Draw();
        offsetY = rCreateFileRoller.getOffset();

        DrawRectangle(40, 40, 1200, 80, bgColor);
        DrawRectangle(40, 520, 1200, 80, bgColor);
        DrawRectangleLinesEx({ 40, 120, 1200, 400 }, 3, BLACK);

        // Typing Box
        ibFileName.Draw();
        ibInputFront.Draw();
        ibInputBack.Draw();

        // Function Button
        bSaveCard.Draw();
        bFinishDeck.Draw();
        if (bSaveCard.IsClicked() && !ibInputFront.GetText().empty() && !ibInputBack.GetText().empty()) {
            if (!cardEditMode) {
                flashcards.emplace_back(ibInputFront.GetText(), ibInputBack.GetText());
            }
            else {
                flashcards[currentCardIndex].setFront(ibInputFront.GetText());
                flashcards[currentCardIndex].setBack(ibInputBack.GetText());
                cardEditMode = false;
            }
            ibInputFront.Clear();
            ibInputBack.Clear();
        }
        if (bFinishDeck.IsClicked() && !flashcards.empty()) {
            currentFileName = ibFileName.GetText();
            ibInputFront.Clear();
            ibInputBack.Clear();
            isCreatingFile = false;
        }

        // Display total number of cards
        DrawRecWithLines(1080, 645, 160, 35, WHITE, 3); //Total Card number
        DrawTextCentered("Total:", { 1080, 645, 160, 35 }, 25, BLUE);
    }
    else {
        for (size_t i = 0; i < existingFolder.size(); ++i) {
            if (((float)(initFolderY + (i * 100)) + boxFolderHeight - offsetY <= 560) && ((float)(initFolderY + (i * 100)) - offsetY >= 80)) {
                bExistingFolder[i].SetY((float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY);
                bExistingFolder[i].Draw();
                if (bExistingFolder[i].IsClicked()) currentFolderName = bExistingFolder[i].getLabel();

                // Delete Folder
                DrawRecWithLines(1070, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight, RED, 3);
                if (is_chosen({ 1070,(float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY ,50,boxFolderHeight })) {
                    remove_all(saveDirectory + "/" + bExistingFolder[i].getLabel());
                }
            }
        }
        bFinishFile.Draw();
        if (bFinishFile.IsClicked()) {
            string fulldirectory = saveDirectory + "/" + currentFolderName;
            if (!exists(fulldirectory))
                create_directories(fulldirectory);
            if (!currentFileName.ends_with(".json")) currentFileName += ".json";
            SaveFlashcardToJson(flashcards, fulldirectory, currentFileName);
            //flashcards.clear();
            //existingFile.clear();
            //bExistingFolder.clear();
            isCreatingFile = true;
            currentFolderName = "Default";
            currentFileName = "untiled";
            currentPage = LIBRARY;
        }
            
        rFolderRoller.Draw();
        offsetY = rFolderRoller.getOffset();

        DrawRectangle(40, 40, 1200, 80, bgColor);
        DrawRectangle(40, 520, 1200, 80, bgColor);
        DrawRectangleLinesEx({ 40, 120, 1200, 400 }, 3, BLACK);
        
        ibNewFolder.Draw();
        if (!ibNewFolder.GetText().empty() && GetKeyPressed() == KEY_ENTER) {
            fullDirectory = saveDirectory + "/" + ibNewFolder.GetText();
            if (!exists(fullDirectory))
                create_directories(fullDirectory);
        }
    }
    
    bHome.Draw();
    bSetting.Draw();
    if (bHome.IsClicked()) {
        ibFileName.Clear();
        ibInputFront.Clear();
        ibInputBack.Clear();
        existingFolder.clear();
        bExistingFolder.clear();
        flashcards.clear();
        currentPage = HOME;
    }
    existingFolder.clear();
    bExistingFolder.clear();
}



/*void DrawCreateFolderPage() {
    bHome.Draw();
    bSetting.Draw();
    if (bHome.IsClicked()) currentPage = HOME;

    

    // Flashcard display
    DrawRectangle(40, 120, 1200, 400, GRAY); // Draw frame
    offsetY = rCreateFileRoller.getOffset();
    if (!flashcards.empty()) {

        for (size_t i = 0; i < existingFolder.size(); ++i) {
            if (((float)(initFileY + (i * 100)) + boxFileHeight - offsetY <= 560) && ((float)(initFileY + (i * 100)) - offsetY >= 80)) {
                DrawRectangle(50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight, WHITE);
                DrawTextMiddle(existingFolder[i].c_str(), {50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight}, 30, BLUE);
            }
        }
    }
    DrawRectangle(40, 40, 1200, 80, bgColor);
    DrawRectangle(40, 520, 1200, 80, bgColor);
    DrawRectangleLinesEx({ 40, 120, 1200, 400 }, 3, BLACK);
    rCreateFileRoller.Draw();
    offsetY = rCreateFileRoller.getOffset();




    string fulldirectory = saveDirectory + "/" + currentFolderName;
    if (!exists(fulldirectory))
        create_directories(fulldirectory);
    SaveFlashcardToJson(flashcards, fulldirectory, currentFileName);
    flashcards.clear();
}
*/
bool choosingFolder = true;
Button bNext(920,605,100,50,">>",GREEN,BLUE);
Button bBack(260, 605, 100, 50, "<<", GREEN, BLUE);
enum LibraryStage {
    CHOOSEFOLDER,
    CHOOSEFILE,
    EDITCARD,
    FLIPCARD
};
LibraryStage currentStage = CHOOSEFOLDER;
// Library Page
void DrawLibraryPage() {
    for (const auto& saveFolderName : GetSavedFolder(saveDirectory))
        existingFolder.emplace_back(saveFolderName);
    for (size_t i = 0; i < existingFolder.size(); ++i)
        bExistingFolder.emplace_back(50, (float)(initFolderY + (i * boxFolderHeight * 1.5)), 1000, boxFolderHeight, existingFolder[i], WHITE, BLACK);

    

    switch (currentStage) {
    case CHOOSEFOLDER:
        DrawRectangle(40, 120, 1200, 400, GRAY); // Draw frame
        for (size_t i = 0; i < existingFolder.size(); ++i) {
            if (((float)(initFolderY + (i * 100)) + boxFolderHeight - offsetY <= 560) && ((float)(initFolderY + (i * 100)) - offsetY >= 80)) {
                bExistingFolder[i].SetY((float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY);
                bExistingFolder[i].Draw();
                if (bExistingFolder[i].IsClicked()) {
                    currentFolderName = bExistingFolder[i].getLabel();
                    currentStage = CHOOSEFILE;
                }
                // Delete Folder
                DrawRecWithLines(1070, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight, RED, 3);
                if (is_chosen({ 1070,(float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY ,50,boxFolderHeight })) {
                    remove_all(saveDirectory + "/" + bExistingFolder[i].getLabel());
                }
            }
        }
        rFolderRoller.Draw();
        offsetY = rFolderRoller.getOffset();

        DrawRectangle(40, 40, 1200, 80, bgColor);
        DrawRectangle(40, 520, 1200, 80, bgColor);
        DrawRectangleLinesEx({ 40, 120, 1200, 400 }, 3, BLACK);
        break;
    case CHOOSEFILE:
        DrawRectangle(40, 120, 1200, 400, GRAY); // Draw frame
        fullDirectory = saveDirectory + "/" + currentFolderName;
        for (const auto& fileName : GetSavedFiles(fullDirectory))
            existingFile.emplace_back(fileName);
        for (size_t i = 0; i < existingFile.size(); ++i)
            bExistingFile.emplace_back(50, (float)(initFileY + (i * boxFileHeight * 1.5)), 1000, boxFileHeight, existingFile[i], WHITE, BLACK);

        for (size_t i = 0; i < bExistingFile.size(); ++i) {
            if (((float)(initFolderY + (i * 100)) + boxFolderHeight - offsetY <= 560) && ((float)(initFolderY + (i * 100)) - offsetY >= 80)) {
                bExistingFile[i].SetY((float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY);
                bExistingFile[i].Draw();
                if (bExistingFile[i].IsClicked()) {
                    currentFileName = bExistingFile[i].getLabel();
                    flashcards = LoadFlashcardFromJson(fullDirectory,currentFileName);
                    currentStage = FLIPCARD;
                }
                // Delete Folder
                DrawRecWithLines(1070, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight, RED, 3);
                if (is_chosen({ 1070,(float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY ,50,boxFolderHeight })) {
                    remove_all(saveDirectory + "/" + currentFolderName + "/" + currentFileName);
                }
            }
        }
        break;
    case EDITCARD:

        break;
    case FLIPCARD:
        for (size_t i = 0; i < flashcards.size(); ++i)
            fFlashcards.emplace_back(240,135,800,450,flashcards[i].getFront(),30,flashcards[i].getBack(),30);
        fFlashcards[currentCardIndex].DrawFlipH(GetFrameTime());
        fFlashcards[currentCardIndex].StartFlip();

        bNext.Draw();
        bBack.Draw();
        if (bNext.IsClicked() && currentCardIndex < fFlashcards.size()-1) ++currentCardIndex;
        if (bBack.IsClicked() && currentCardIndex > 0) --currentCardIndex;
        break;
    }
    
    bHome.Draw();
    bSetting.Draw();
    if (bHome.IsClicked()) {
        existingFolder.clear();
        bExistingFolder.clear();
        existingFile.clear();
        bExistingFile.clear();
        fFlashcards.clear();
        flashcards.clear();
        currentCardIndex = 0;
        currentStage = CHOOSEFOLDER;
        currentPage = HOME;
    }
    
    existingFolder.clear();
    bExistingFolder.clear();
    existingFile.clear();
    bExistingFile.clear();
    fFlashcards.clear();
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
    if (!exists(saveDirectory + "/Default"))
        create_directories(saveDirectory + "/Default");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bgColor);
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

