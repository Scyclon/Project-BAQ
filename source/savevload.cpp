#include "saveload.hpp"

// Function to save flashcards to JSON
void SaveFlashcardToJson(const vector<Flashcard>& flashcards,const string& saveDirectory, const string& fileName) {
    json jsonArray;
    for (const auto& card : flashcards)
        jsonArray.emplace_back(card.toJson());
    ofstream file(saveDirectory + "/" + fileName);
    if (file.is_open()) {
        file << jsonArray.dump(4);
        file.close();
    }
    else cout << "File save failed\n";
}

vector<Flashcard> LoadFlashcardFromJson(const string& saveDirectory, const string& fileName) {
    vector<Flashcard> flashcards;
    ifstream file(saveDirectory + "/" + fileName);
    if (file.is_open()) {
        json jsonArray;
        file >> jsonArray;
        for (const auto& jsonFlashcard : jsonArray)
            flashcards.emplace_back(Flashcard::fromJson(jsonFlashcard));
    }
    else cout << "File load failed\n";
    return flashcards;
}

// Fetch all JSON files in the save folder
vector<string> GetSavedFiles(const string& saveDirectory) {
    vector<string> savedFiles;
    for (const auto& fileName : directory_iterator(saveDirectory))
        if (fileName.path().extension() == ".json") // look for .json file only
            savedFiles.emplace_back(fileName.path().filename().string());
    return savedFiles;
}
