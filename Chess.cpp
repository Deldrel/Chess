#include "Chess.h"

void Chess::init() {
    sf::Image icon;
    icon.loadFromFile("../src/icon.png");
    window.create(sf::VideoMode(window_width, window_height), "OrdinaryLog", sf::Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);
    boardTexture.loadFromImage(newImage("../src/board.png", window_width, window_height));
    boardSprite.setTexture(boardTexture);
    piecesTexture.loadFromImage(newImage("../src/pieces.png", window_width / 8 * 6, window_height / 8 * 2));
    piecesSprite.setTexture(piecesTexture);

    computePseudoMoves();

    loadFEN(""); ///default
    //loadFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R "); ///test position
}

void Chess::loop() {
    sf::Clock clock;
    sf::Time elapsedTime = sf::Time::Zero;

    while (window.isOpen()) {
        elapsedTime += clock.restart();
        handleEvents();

        if (elapsedTime > sf::seconds(1.0 / 60)) {
            elapsedTime = sf::Time::Zero;
            display();
        }
    }
}

void Chess::handleEvents() {

    sf::Event event{};

    while (window.pollEvent(event)) {

        switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) window.close();
                //if (event.key.code == sf::Keyboard::Space) reverse(board.begin(), board.end());
                break;

            case sf::Event::MouseMoved:
                mouseX = sf::Mouse::getPosition(window).x;
                mouseY = sf::Mouse::getPosition(window).y;
                mouseIndex = mouseY / (window_height / 8) * 8 + mouseX / (window_width / 8);
                break;

            case sf::Event::MouseButtonPressed:
                selectedPieceIndex = mouseIndex;
                if (selectedPieceIndex != -1) generateLegalMoves(selectedPieceIndex);
                break;

            case sf::Event::MouseButtonReleased:
                for (auto i: legalMoves) {
                    if (mouseIndex == i && mouseIndex != selectedPieceIndex) {


                        board[mouseIndex] = board[selectedPieceIndex];
                        board[selectedPieceIndex] = none;
                    }
                }
                selectedPieceIndex = -1;
                break;
        }
    }
}

void Chess::computePseudoMoves() {
    numSquareToEdge.resize(64);
    knightPseudoMoves.resize(64);
    pawnPseudoMoves.resize(64);

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {

            int index = y * 8 + x;

            numSquareToEdge[index] = {y, 7 - x, 7 - y, x, min(y, 7 - x), min(7 - x, 7 - y), min(7 - y, x), min(x, y)};

            for (auto offset: {-17, -15, -6, 10, 17, 15, 6, -10}) {
                if (max(abs(x - (index + offset - (index + offset) / 8 * 8)), abs(y - (index + offset) / 8)) == 2)
                    knightPseudoMoves[index].push_back(index + offset);
            }

            pawnPseudoMoves[index] = {y > 0, y > 0 && x < 7, y > 0 && x > 0, y == 6, y < 7, y < 7 && x > 0,
                                      y < 7 && x < 7, y == 1};
        }
    }
}

void Chess::generateLegalMoves(int startIndex) {
    legalMoves.clear();

    int type = getType(board[startIndex]);
    int color = getColour(board[startIndex]);

    switch (type) {
        case none:
            break;

        case king:
            //roques
        case bishop:
        case rook:
        case queen:
            for (int dirIndex = (type == bishop) ? 4 : 0; dirIndex < ((type == rook) ? 4 : 8); ++dirIndex) {
                for (int num = 0; num < ((type == king) ? 1 : numSquareToEdge[startIndex][dirIndex]); ++num) {

                    int targetIndex = startIndex + vector<int>{-8, 1, 8, -1, -7, 9, 7, -9}[dirIndex] * (num + 1);

                    if (getType(board[targetIndex]) != none && color == getColour(board[targetIndex])) break;
                    legalMoves.push_back(targetIndex);
                    if (getType(board[targetIndex]) != none && color != getColour(board[targetIndex])) break;
                }
            }
            break;

        case knight:
            for (auto endIndex: knightPseudoMoves[startIndex]) {
                if (color != getColour(board[endIndex])) legalMoves.push_back(endIndex);
            }
            break;

        case pawn:
            for (int i = (color == white ? 0 : 4); i < (color == white ? 4 : 8); ++i) {
                if (!pawnPseudoMoves[startIndex][i]) continue;

                int offset = vector<int>{8, 7, 9, 16}[color == white ? i : i - 4];
                int targetIndex = startIndex + offset * (color == white ? -1 : 1);

                if (((offset == 8 || offset == 16) && getType(board[targetIndex]) == none) ||
                    ((offset == 7 || offset == 9) && getType(board[targetIndex]) != none &&
                     color != getColour(board[targetIndex])))
                    legalMoves.push_back(targetIndex);
            }
            break;

        default:
            cerr << "\nInvalid piece type" << endl;
            break;
    }

    //en-passant
    //promotion
    //pout tous les coups legaux verifier echec
}

void Chess::display() {
    if (!window.hasFocus()) return;

    window.clear();

    window.draw(boardSprite);

    sf::RectangleShape rect;
    rect.setSize({window_width / 8, window_height / 8});
    int textureWidth = piecesTexture.getSize().x;
    int textureHeight = piecesTexture.getSize().y;

    for (int i = 0; i < 64; ++i) {
        if (board[i] == none) continue;
        piecesSprite.setTextureRect({getType(board[i] - 1) * textureWidth / 6,
                                     getColour(board[i]) / 16 * textureHeight / 2,
                                     textureWidth / 6,
                                     textureHeight / 2});
        if (i != selectedPieceIndex) piecesSprite.setPosition(i % 8 * textureWidth / 6, i / 8 * textureHeight / 2);
        else piecesSprite.setPosition(mouseX - textureWidth / 6 / 2, mouseY - textureHeight / 2 / 2);
        window.draw(piecesSprite);
    }

    if (selectedPieceIndex != -1)
        for (auto i: legalMoves) {
            rect.setPosition(i % 8 * window_width / 8,
                             i / 8 * window_height / 8);
            rect.setFillColor({255, 0, 0, 100});
            window.draw(rect);
        }

    window.display();
}

void Chess::loadFEN(string fen) {
    board.resize(64);
    fill(begin(board), end(board), none);

    if (fen.empty()) fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w ";
    bool boardEnd = false;
    int i = 0;

    for (auto c: fen) {

        switch (c) {
            default:
                cerr << "\nInvalid character in FEN string, loading default position instead..." << endl;
                loadFEN("");
                return;
            case ' ':
                boardEnd = true;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                break;
            case '/':
                i--;
                break;
            case 'r':
                board[i] = rook + black;
                break;
            case 'n':
                board[i] = knight + black;
                break;
            case 'b':
                if (boardEnd) colorToMove = true;
                else board[i] = bishop + black;
                break;
            case 'q':
                board[i] = queen + black;
                break;
            case 'k':
                board[i] = king + black;
                break;
            case 'p':
                board[i] = pawn + black;
                break;
            case 'R':
                board[i] = rook + white;
                break;
            case 'N':
                board[i] = knight + white;
                break;
            case 'B':
                board[i] = bishop + white;
                break;
            case 'Q':
                board[i] = queen + white;
                break;
            case 'K':
                board[i] = king + white;
                break;
            case 'P':
                board[i] = pawn + white;
                break;
            case 'w':
                colorToMove = false;
                break;
        }

        i += isInRange(c, 48, 56) ? c - 48 : 1;
    }
}

void Chess::resizeImage(const sf::Image &originalImage, sf::Image &resizedImage) {
    const sf::Vector2u originalImageSize{originalImage.getSize()};
    const sf::Vector2u resizedImageSize{resizedImage.getSize()};
    for (unsigned int y{0u}; y < resizedImageSize.y; ++y) {
        for (unsigned int x{0u}; x < resizedImageSize.x; ++x) {
            unsigned int origX{
                    static_cast<unsigned int>(static_cast<double>(x) / resizedImageSize.x * originalImageSize.x)};
            unsigned int origY{
                    static_cast<unsigned int>(static_cast<double>(y) / resizedImageSize.y * originalImageSize.y)};
            resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
        }
    }
}

sf::Image Chess::newImage(const string &path, int resizedWidth, int resizedHeight) {
    sf::Image originalImage;
    sf::Image resizedImage;
    resizedImage.create(resizedWidth, resizedHeight);
    originalImage.loadFromFile(path);
    resizeImage(originalImage, resizedImage);
    return resizedImage;
}

bool Chess::isInRange(int value, int min, int max) {
    return value <= max && value >= min;
}

int Chess::getType(int n) {
    return ((n & 4) + (n & 2) + (n & 1));
}

int Chess::getColour(int n) {
    return ((n & 16) + (n & 8));
}