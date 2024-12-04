#include "saveload.hpp"

// Function to save flashcards to JSON
void SaveFlashcardToJson(const vector<Flashcard>& flashcards, const string& saveDirectory, const string& folderName, const string& fileName) {
    json jsonArray;
    for (const auto& card : flashcards)
        jsonArray.emplace_back(card.toJson());
    ofstream file(saveDirectory + "/" + folderName + "/" + fileName);
    if (file.is_open()) {
        file << jsonArray.dump(4);
        file.close();
    }
    else cout << "File save failed\n";
}

vector<Flashcard> LoadFlashcardFromJson(const string& saveDirectory, const string& folderName, const string& fileName) {
    vector<Flashcard> flashcards;
    ifstream file(saveDirectory + "/" + folderName + "/" + fileName);
    if (file.is_open()) {
        json jsonArray;
        file >> jsonArray;
        for (const auto& jsonFlashcard : jsonArray)
            flashcards.emplace_back(Flashcard::fromJson(jsonFlashcard));
    }
    else cout << "File load failed\n";
    return flashcards;
}

// Fetch all folder in "saves"
vector<string> GetSavedFolder(const string& saveDirectory) {
    vector<string> savedFolders;
    for (const auto& folderName : directory_iterator(saveDirectory))
        savedFolders.emplace_back(folderName.path().filename().string());
    return savedFolders;
}

// Fetch all JSON files in folder
vector<string> GetSavedFiles(const string& saveDirectory, const string& folderName) {
    vector<string> savedFiles;
    for (const auto& fileName : directory_iterator(saveDirectory + "/" + folderName))
        if (fileName.path().extension() == ".json") // look for .json file only
            savedFiles.emplace_back(fileName.path().filename().string());
    return savedFiles;
}
