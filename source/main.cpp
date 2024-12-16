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
    SETTING,
    CREATE,
    LIBRARY,
    FLASHCARD,
};
AppPage currentPage = HOME;
AppPage previousPage = currentPage;

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
vector<FlipCard> fFlashcards;

// General object
Button bHome(1110, 20, 60, 60, "HOME",WHITE,DARKBLUE);
Button bSetting(1180, 20, 60, 60, "SET",WHITE,DARKBLUE);

// Setting
Music bgMusic;
Button bPauseMusic(495, 445, 280, 40, "Pause/Play", WHITE, BLUE);
Button bChooseMusic(495, 355, 280, 40, "Choose", WHITE, BLUE);
Button bVolUp(780, 600, 50, 40, ">>>", WHITE, BLUE);
Button bVolDown(440, 600, 50, 40, "<<<", WHITE, BLUE);
Button bNextMusic(780, 400, 50, 40, ">>>", WHITE, BLUE);
Button bPreviousMusic(440, 400, 50, 40, "<<<", WHITE, BLUE);
vector<string> existingMusic;
int currentMusicIndex = 0;
float musicVolume;
bool isMusicPlaying = true;
void SettingPopUp() {
    bSetting.Draw();
    if (bSetting.IsClicked()) {
        if (currentPage != SETTING) {
            previousPage = currentPage;
            currentPage = SETTING;
        }
        else currentPage = previousPage;
    }
}
void DrawSettingPage() {
    DrawRecWithLines(300, 40, 680, 640, WHITE, 3);

    //Get Music File
    for (const auto& fileName : directory_iterator("soundtracks"))
        if (fileName.path().extension() == ".mp3" || fileName.path().extension() == ".wav" || fileName.path().extension() == ".ogg") // look for music file
            existingMusic.emplace_back(fileName.path().filename().string());


    //Draw Music List
    DrawRecWithLines(495, 400, 280, 40, WHITE, 3);
    bNextMusic.Draw();
    bPreviousMusic.Draw();
    bChooseMusic.Draw();
    bVolDown.Draw();
    bVolUp.Draw();
    bPauseMusic.Draw();

    if (!existingMusic.empty()) {
        DrawTextCentered(existingMusic[currentMusicIndex].c_str(), { 495, 400, 280, 40 }, 15, BLACK);

        if (bNextMusic.IsClicked() && currentMusicIndex < existingMusic.size() - 1) ++currentMusicIndex;
        if (bPreviousMusic.IsClicked() && currentMusicIndex > 0) --currentMusicIndex;
        if (bChooseMusic.IsClicked()) {
            bgMusic = LoadMusicStream(("soundtracks/" + existingMusic[currentMusicIndex]).c_str());
            PlayMusicStream(bgMusic);
            SetMusicVolume(bgMusic, musicVolume);
        }

        if (bPauseMusic.IsClicked()) {
            if (isMusicPlaying) PauseMusicStream(bgMusic);
            else ResumeMusicStream(bgMusic);
            isMusicPlaying = !isMusicPlaying;
        }
    }
    else DrawTextCentered("No music file found", { 495, 400, 280, 40 }, 15, BLACK);
    if (bVolDown.IsClicked() && musicVolume > 0) musicVolume -= 0.05;
    if (bVolUp.IsClicked() && musicVolume < 1.0) musicVolume += 0.05;
    SetMusicVolume(bgMusic, musicVolume);
    DrawTextCentered("Music Volume", { 495,555,280,40 }, 20, BLUE);
    DrawTextCentered((to_string((int)(musicVolume * 100)) + "%").c_str(), { 495,600,280,40 }, 20, BLUE);

    bHome.Draw();
    if (bHome.IsClicked()) currentPage = HOME;
    existingMusic.clear();

    SettingPopUp();
}

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
    bNew.Draw();
    bLibrary.Draw();
    if (bNew.IsClicked()) currentPage = CREATE;
    if (bLibrary.IsClicked()) currentPage = LIBRARY;
    SettingPopUp();
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

InputBox ibNewFolder(40, 630, 1000, 50, "Create New Folder", 30);



void DrawCreatePage() {
    for (const auto& saveFolderName : GetSavedFolder(saveDirectory))
        existingFolder.emplace_back(saveFolderName);
    for (size_t i = 0; i < existingFolder.size(); ++i)
        bExistingFolder.emplace_back(50, (float)(initFolderY + (i * boxFolderHeight * 1.5)), 1000, boxFolderHeight, existingFolder[i], WHITE, BLACK);

    // Flashcard display
    DrawRectangle(40, 120, 1200, 400, GRAY); // Draw frame

    if (isCreatingFile) {
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
                    if (is_chosen({ 50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight * 2 }) && GetMousePosition().y > 120 && GetMousePosition().y < 520) {
                        ibInputFront.SetText(flashcards[i].getFront());
                        ibInputBack.SetText(flashcards[i].getBack());
                        currentCardIndex = i;
                        cardEditMode = true;
                    }

                    // Delete chosen Card
                    DrawRecWithLines(1070, (float)(initFileY + (i * 100)) - offsetY, 50, boxFolderHeight * 2, RED, 3);
                    if (is_chosen({ 1070, (float)(initFileY + (i * 100)) - offsetY, 50, boxFolderHeight * 2 }) && GetMousePosition().y > 120 && GetMousePosition().y < 520) {
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
        string total = "Total: " + to_string(flashcards.size());
        DrawRecWithLines(1080, 645, 160, 35, WHITE, 3); //Total Card number
        DrawTextCentered(total.c_str(), {1080, 645, 160, 35}, 25, BLUE);
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
            fullDirectory = saveDirectory + "/" + currentFolderName;
            if (!exists(fullDirectory))
                create_directories(fullDirectory);
            if (!currentFileName.ends_with(".json")) currentFileName += ".json";
            SaveFlashcardToJson(flashcards, fullDirectory, currentFileName);
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
        DrawRecWithLines(40, 560, 1000, 50, WHITE, 3);
        DrawTextCentered(currentFolderName.c_str(), {40, 560, 1000, 50}, 30, BLACK);
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

    SettingPopUp();
}



// Library
bool choosingFolder = true;
Button bNext(920,605,100,50,">>",GREEN,BLUE);
Button bBack(260, 605, 100, 50, "<<", GREEN, BLUE);
enum LibraryStage {
    VIEWFOLDER,
    VIEWFILE,
    EDITCARD,
    FLIPCARD
};
LibraryStage currentStage = VIEWFOLDER;
// Library Page
void DrawLibraryPage() {
    for (const auto& saveFolderName : GetSavedFolder(saveDirectory))
        existingFolder.emplace_back(saveFolderName);
    for (size_t i = 0; i < existingFolder.size(); ++i)
        bExistingFolder.emplace_back(50, (float)(initFolderY + (i * boxFolderHeight * 1.5)), 1000, boxFolderHeight, existingFolder[i], WHITE, BLACK);

    // switch between stages in library
    switch (currentStage) {
    case VIEWFOLDER: {
        DrawRectangle(40, 120, 1200, 400, GRAY); // Draw frame
        for (size_t i = 0; i < existingFolder.size(); ++i) {
            if (((float)(initFolderY + (i * 100)) + boxFolderHeight - offsetY <= 560) && ((float)(initFolderY + (i * 100)) - offsetY >= 80)) {
                bExistingFolder[i].SetY((float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY);
                bExistingFolder[i].Draw();
                if (bExistingFolder[i].IsClicked()) {
                    currentFolderName = bExistingFolder[i].getLabel();
                    currentStage = VIEWFILE;
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
    }
    case VIEWFILE:
    {
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
                    if (!currentFileName.ends_with(".json")) currentFileName += ".json";
                    flashcards = LoadFlashcardFromJson(fullDirectory, currentFileName);
                    currentStage = FLIPCARD;
                }

                DrawRecWithLines(1140, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight, DARKGRAY, 3);
                if (is_chosen({ 1140,(float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY ,50,boxFolderHeight })) {
                    currentFileName = bExistingFile[i].getLabel();
                    if (!currentFileName.ends_with(".json")) currentFileName += ".json";
                    flashcards = LoadFlashcardFromJson(fullDirectory, currentFileName);
                    currentStage = EDITCARD;
                }

                // Delete Folder
                DrawRecWithLines(1070, (float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY, 50, boxFolderHeight, RED, 3);
                if (is_chosen({ 1070,(float)(initFolderY + (i * boxFolderHeight * 1.5)) - offsetY ,50,boxFolderHeight })) {
                    currentFileName = bExistingFile[i].getLabel();
                    if (!currentFileName.ends_with(".json")) currentFileName += ".json";
                    remove_all(saveDirectory + "/" + currentFolderName + "/" + currentFileName);
                }
            }
        }
        rCreateFileRoller.Draw();
        offsetY = rCreateFileRoller.getOffset();

        DrawRectangle(40, 40, 1200, 80, bgColor);
        DrawRectangle(40, 520, 1200, 80, bgColor);
        DrawRectangleLinesEx({ 40, 120, 1200, 400 }, 3, BLACK);
        break;
    }
    case EDITCARD: {
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
                    if (is_chosen({ 50, (float)(initFileY + (i * 100)) - offsetY, 1000, boxFileHeight * 2 }) && GetMousePosition().y > 120 && GetMousePosition().y < 520) {
                        ibInputFront.SetText(flashcards[i].getFront());
                        ibInputBack.SetText(flashcards[i].getBack());
                        currentCardIndex = i;
                        cardEditMode = true;
                    }

                    // Delete chosen Card
                    DrawRecWithLines(1070, (float)(initFileY + (i * 100)) - offsetY, 50, boxFolderHeight * 2, RED, 3);
                    if (is_chosen({ 1070, (float)(initFileY + (i * 100)) - offsetY, 50, boxFolderHeight * 2 }) && GetMousePosition().y > 120 && GetMousePosition().y < 520) {
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
            if (!ibFileName.GetText().empty()) {
                if (!currentFileName.ends_with(".json")) currentFileName += ".json";
                remove_all(fullDirectory + "/" + currentFileName);
                currentFileName = ibFileName.GetText();
            }
            if (!exists(fullDirectory))
                create_directories(fullDirectory);
            if (!currentFileName.ends_with(".json")) currentFileName += ".json";
            SaveFlashcardToJson(flashcards, fullDirectory, currentFileName);
                       
            currentFolderName = "Default";
            currentFileName = "untiled";
            flashcards.clear();
            ibInputFront.Clear();
            ibInputBack.Clear();
            currentStage = VIEWFOLDER;
        }
        
        // Display total number of cards
        string total = "Total: " + to_string(flashcards.size());
        DrawRecWithLines(1080, 645, 160, 35, WHITE, 3); //Total Card number
        DrawTextCentered(total.c_str(), {1080, 645, 160, 35}, 25, BLUE);
        break;
    }
    case FLIPCARD: {
        if(fFlashcards.empty())
        for (size_t i = 0; i < flashcards.size(); ++i)
            fFlashcards.emplace_back(240, 135, 800, 450, flashcards[i].getFront(), 30, flashcards[i].getBack(), 30);
        fFlashcards[currentCardIndex].DrawFlipH(GetFrameTime());
        fFlashcards[currentCardIndex].StartFlip();

        bNext.Draw();
        bBack.Draw();
        if (bNext.IsClicked() && currentCardIndex < fFlashcards.size() - 1) ++currentCardIndex;
        if (bBack.IsClicked() && currentCardIndex > 0) --currentCardIndex;
        break;
    }

    }

    bHome.Draw();
    if (bHome.IsClicked()) {
        existingFolder.clear();
        bExistingFolder.clear();
        existingFile.clear();
        bExistingFile.clear();
        fFlashcards.clear();
        flashcards.clear();
        currentCardIndex = 0;
        currentStage = VIEWFOLDER;
        currentPage = HOME;
    }

    existingFolder.clear();
    bExistingFolder.clear();
    existingFile.clear();
    bExistingFile.clear();
    //fFlashcards.clear();

    SettingPopUp();
}



// Main Loop
int main() {
    InitWindow(1280, 720, "Flashcard App");
    InitAudioDevice();
    SetTargetFPS(60);

    // Ensure saves and soundtracks folder exists
    if (!exists(saveDirectory))
        create_directories(saveDirectory);
    if (!exists(saveDirectory + "/Default"))
        create_directories(saveDirectory + "/Default");
    if (!exists("soundtracks"))
        create_directories("soundtracks");

    // Load Music
    for (const auto& fileName : directory_iterator("soundtracks"))
        if (fileName.path().extension() == ".mp3" || fileName.path().extension() == ".wav" || fileName.path().extension() == ".ogg") // look for music file
            existingMusic.emplace_back(fileName.path().filename().string());
    if (!existingMusic.empty())
        bgMusic = LoadMusicStream(("soundtracks/" + existingMusic[0]).c_str()); //music file directory

    musicVolume = 0.5;
    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, musicVolume);
    existingMusic.clear();

    while (!WindowShouldClose()) {
        UpdateMusicStream(bgMusic);

        BeginDrawing();
        ClearBackground(bgColor);
        DrawRectangleLinesEx({ 3,3,1274,714 }, 3, BLACK);
        switch (currentPage) {
        case HOME:
            DrawHomePage();
            break;
        case SETTING:
            DrawSettingPage();
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

    StopMusicStream(bgMusic);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

