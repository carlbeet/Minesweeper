

#include "TextureMng.h"
unordered_map<string, sf::Texture>  TextureMng::textures;
//redefine static vars in global space


void TextureMng::LoadTexture(string name) {
    string path = "images/";
    path += name + ".png" ;
    // ex string  "images/mine.png"

    textures[name].loadFromFile(path);
}
sf::Texture& TextureMng::GetTexture(string textureName) {
    // if textures[name] DOESNT EXIST
    if (textures.find(textureName) == textures.end()) // find() returns end iter WHEN NOT FOUND
    { LoadTexture(textureName); }

    return textures[textureName];


}
void TextureMng::Clear() {// clear all textures, call once
    textures.clear();
}

