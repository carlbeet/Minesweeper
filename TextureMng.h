#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
using std::unordered_map;
using namespace std;

class TextureMng {
    // static = only one in memory, exist in memory for duration of memory.
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string name);
public:

    static sf::Texture& GetTexture(string textureName);
    static void Clear(); // clear all textures, call once at the end of main
};


