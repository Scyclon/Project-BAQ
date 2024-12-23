#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "json.hpp"

using namespace std;
using namespace filesystem;
using namespace nlohmann;

// Flashcard Class
class Flashcard {
private:
    string front; // front text
    string back; // back text

public:
    Flashcard(const string& fr, const string& ba) : front(fr), back(ba) {}
    Flashcard() = default;

    string getFront() { return front; }
    string getBack() { return back; }

    void setFront(string setFront) { front = setFront; }
    void setBack(string setBack) { back = setBack; }

    json toJson() const { return { {"front", front}, {"back", back} }; }
    static Flashcard fromJson(const json& jsonFlashcard) {
        return Flashcard(jsonFlashcard.at("front"), jsonFlashcard.at("back"));
    }
};

void SaveFlashcardToJson(const vector<Flashcard>& flashcards, const string& fullDirectory, const string& fileName);
vector<Flashcard> LoadFlashcardFromJson(const string& fullDirectory,const string& fileName);
vector<string> GetSavedFolder(const string& saveDirectory);
vector<string> GetSavedFiles(const string& fullDirectory);