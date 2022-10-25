#ifndef ORDINARYLOG_CHESS_H
#define ORDINARYLOG_CHESS_H

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <algorithm>
#include "cplus.h"

#define window_width 900
#define window_height window_width
#define none 0
#define king 1
#define queen 2
#define bishop 3
#define knight 4
#define rook 5
#define pawn 6
#define white 8
#define black 16

using namespace std;

class Chess {

public:

    sf::RenderWindow window;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::Texture piecesTexture;
    sf::Sprite piecesSprite;
    int mouseX = 0, mouseY = 0, mouseIndex = -1, selectedPieceIndex = -1;
    bool colorToMove = false;
    vector<int> board;
    vector<int> legalMoves;
    vector<vector<int>> numSquareToEdge;
    vector<vector<int>> knightPseudoMoves;
    vector<vector<bool>> pawnPseudoMoves;

    Chess() = default;

    ~Chess() = default;

    void init();

    void loop();

    void handleEvents();

    void display();

    void computePseudoMoves();

    void generateLegalMoves(int startIndex);

    void loadFEN(string fen);

    void resizeImage(const sf::Image &originalImage, sf::Image &resizedImage);

    sf::Image newImage(const string& path, int resizedWidth, int resizedHeight);

    bool isInRange(int value, int min, int max);

    int getType(int n);

    int getColour(int n);

};


#endif
