#ifndef GAME_HPP
#define GAME_HPP

/*
    GPT GENERATED
*/

#include <raylib.h>
#include <array>
#include <vector>
#include <string>

struct Vec2i {
    int x;
    int y;
};

// Tetromino types
enum class TetrominoType {
    I, O, T, S, Z, J, L, None
};

// A falling piece
struct Piece {
    TetrominoType type = TetrominoType::None;
    std::array<Vec2i,4> blocks;
    Vec2i position = {0,0};
    int rotation = 0;
};

class Game {
public:
    Game(int width = 900, int height = 720);
    ~Game();

    void Run();

private:
    void Update();
    void Draw();
    void HandleInput();
    void ResetBoard();

    bool CanPlace(const Piece& p) const;
    void PlacePiece(const Piece& p);
    void ClearLines();
    void SpawnPiece();
    void RotatePiece(Piece& p, int dir);
    void HardDrop();

    Piece MakePiece(TetrominoType t);

    // Drawing helpers
    void DrawBoard();
    void DrawPiece(const Piece& p, Vec2i origin);
    void DrawNextPiece();
    void DrawHoldPiece();
    void DrawStats();

    static const int BOARD_WIDTH  = 10;
    static const int BOARD_HEIGHT = 20;
    static const int BLOCK_SIZE   = 30;

    int screenWidth;
    int screenHeight;

    Piece m_current;
    Piece m_next;
    Piece m_hold;
    bool m_holdUsed = false;

    std::array<int, BOARD_WIDTH*BOARD_HEIGHT> m_board;

    float m_dropAccumulator = 0.f;
    float m_dropInterval = 0.5f; // seconds

    int m_score = 0;
    int m_linesCleared = 0;
    int m_level = 1;

    Vec2i m_boardOrigin;
    Vec2i m_previewOrigin;
    Vec2i m_holdOrigin;

    // Font and text settings
    int m_fontSize = 20;
    Color m_textColor = WHITE;
};

#endif