/*
    MAINLY GPT GENERATED
*/

#include <game.hpp>

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <iostream>

// Tetromino shapes
static const std::array<std::array<Vec2i,4>,7> SHAPES = {{
    {{ {0,1}, {1,1}, {2,1}, {3,1} }}, // I
    {{ {1,0}, {2,0}, {1,1}, {2,1} }}, // O
    {{ {1,0}, {0,1}, {1,1}, {2,1} }}, // T
    {{ {1,0}, {2,0}, {0,1}, {1,1} }}, // S
    {{ {0,0}, {1,0}, {1,1}, {2,1} }}, // Z
    {{ {0,0}, {0,1}, {1,1}, {2,1} }}, // J
    {{ {2,0}, {0,1}, {1,1}, {2,1} }}  // L
}};

Game::Game(int width, int height)
: screenWidth(width), screenHeight(height)
{
    InitWindow(screenWidth, screenHeight, "Tetris - Raylib");
    SetTargetFPS(60);

    m_board.fill(-1);
    m_boardOrigin = {50,50};
    m_previewOrigin = {m_boardOrigin.x + BOARD_WIDTH*BLOCK_SIZE + 40, m_boardOrigin.y + 40};
    m_holdOrigin = {m_boardOrigin.x + BOARD_WIDTH*BLOCK_SIZE + 40, m_boardOrigin.y + 220};

    std::srand(std::time(nullptr));
    m_next = MakePiece(static_cast<TetrominoType>(std::rand()%7));
    SpawnPiece();
}

Game::~Game() {
    CloseWindow();
}

Piece Game::MakePiece(TetrominoType t) {
    Piece p;
    p.type = t;
    for (int i = 0; i < 4; ++i)
        p.blocks[i] = SHAPES[(int)t][i];
    p.position = {BOARD_WIDTH/2 - 2,0};
    return p;
}

void Game::SpawnPiece() {
    m_current = m_next;
    m_current.position = {BOARD_WIDTH/2 - 2,0};
    m_current.rotation = 0;
    m_next = MakePiece(static_cast<TetrominoType>(std::rand()%7));
    m_holdUsed = false;

    if (!CanPlace(m_current)) {
        // Game over
        ResetBoard();
    }
}

bool Game::CanPlace(const Piece& p) const {
    for (auto b : p.blocks) {
        int rx = b.x, ry = b.y;
        int r = (p.rotation % 4 + 4) % 4;
        for(int i=0;i<r;++i){ int nx=3-ry; int ny=rx; rx=nx; ry=ny; }
        int x = p.position.x + rx;
        int y = p.position.y + ry;
        if (x<0 || x>=BOARD_WIDTH || y<0 || y>=BOARD_HEIGHT) return false;
        if (m_board[y*BOARD_WIDTH + x] != -1) return false;
    }
    return true;
}

void Game::PlacePiece(const Piece& p) {
    for (auto b : p.blocks) {
        int rx = b.x, ry = b.y;
        int r = (p.rotation % 4 + 4) % 4;
        for(int i=0;i<r;++i){ int nx=3-ry; int ny=rx; rx=nx; ry=ny; }
        int x = p.position.x + rx;
        int y = p.position.y + ry;
        if(x>=0 && x<BOARD_WIDTH && y>=0 && y<BOARD_HEIGHT)
            m_board[y*BOARD_WIDTH + x] = (int)p.type;
    }
}

void Game::ClearLines() {
    int lines = 0;
    for(int y=0;y<BOARD_HEIGHT;++y){
        bool full = true;
        for(int x=0;x<BOARD_WIDTH;++x)
            if(m_board[y*BOARD_WIDTH+x]==-1){ full=false; break; }
        if(full){
            ++lines;
            for(int yy=y; yy>0; --yy)
                for(int x=0;x<BOARD_WIDTH;++x)
                    m_board[yy*BOARD_WIDTH + x] = m_board[(yy-1)*BOARD_WIDTH + x];
            for(int x=0;x<BOARD_WIDTH;++x) m_board[x]=-1;
        }
    }
    if(lines>0){
        m_score += 100*((1<<(lines-1)))*m_level;
        m_linesCleared += lines;
        m_level = 1 + m_linesCleared/10;
    }
}

void Game::RotatePiece(Piece& p, int dir) {
    int oldRot = p.rotation;
    p.rotation = (p.rotation + dir)&3;
    std::array<Vec2i,5> kicks = {{{0,0},{-1,0},{1,0},{0,-1},{0,1}}};
    for(auto k:kicks){
        Piece test = p;
        test.position.x += k.x;
        test.position.y += k.y;
        if(CanPlace(test)){ p=test; return; }
    }
    p.rotation = oldRot;
}

void Game::HardDrop() {
    while(true){
        Piece test = m_current;
        test.position.y++;
        if(CanPlace(test)) m_current.position.y++;
        else { PlacePiece(m_current); ClearLines(); SpawnPiece(); break; }
    }
}

void Game::ResetBoard() {
    m_board.fill(-1);
    m_score=0; m_linesCleared=0; m_level=1;
}

void Game::HandleInput() {
    if(IsKeyPressed(KEY_LEFT)){
        Piece test = m_current; test.position.x--;
        if(CanPlace(test)) m_current.position.x--;
    }
    if(IsKeyPressed(KEY_RIGHT)){
        Piece test = m_current; test.position.x++;
        if(CanPlace(test)) m_current.position.x++;
    }
    if(IsKeyPressed(KEY_UP)) RotatePiece(m_current, +1);
    if(IsKeyPressed(KEY_Z)) RotatePiece(m_current, -1);
    if(IsKeyPressed(KEY_SPACE)) HardDrop();
}

void Game::Update() {
    static float timer=0.f;
    timer += GetFrameTime();
    HandleInput();

    if(timer>=m_dropInterval){
        Piece test = m_current;
        test.position.y++;
        if(CanPlace(test)) m_current.position.y++;
        else { PlacePiece(m_current); ClearLines(); SpawnPiece(); }
        timer=0.f;
    }
}

Color ColorForType(TetrominoType t){
    switch(t){
        case TetrominoType::I: return BLUE;
        case TetrominoType::O: return YELLOW;
        case TetrominoType::T: return PURPLE;
        case TetrominoType::S: return GREEN;
        case TetrominoType::Z: return RED;
        case TetrominoType::J: return SKYBLUE;
        case TetrominoType::L: return ORANGE;
        default: return WHITE;
    }
}

void Game::DrawPiece(const Piece& p, Vec2i origin){
    for(auto b:p.blocks){
        int rx=b.x, ry=b.y;
        int r=(p.rotation%4+4)%4;
        for(int i=0;i<r;++i){ int nx=3-ry; int ny=rx; rx=nx; ry=ny; }
        DrawRectangle(origin.x + (p.position.x + rx)*BLOCK_SIZE,
                      origin.y + (p.position.y + ry)*BLOCK_SIZE,
                      BLOCK_SIZE-1, BLOCK_SIZE-1, ColorForType(p.type));
    }
}

void Game::DrawBoard(){
    for(int y=0;y<BOARD_HEIGHT;++y)
        for(int x=0;x<BOARD_WIDTH;++x){
            int v = m_board[y*BOARD_WIDTH+x];
            Color c = (v==-1)? DARKGRAY : ColorForType((TetrominoType)v);
            DrawRectangle(m_boardOrigin.x + x*BLOCK_SIZE,
                          m_boardOrigin.y + y*BLOCK_SIZE,
                          BLOCK_SIZE-1, BLOCK_SIZE-1, c);
        }
}

void Game::DrawNextPiece(){
    Piece preview = m_next;
    preview.position = {0,0}; // draw at origin
    DrawPiece(preview, m_previewOrigin);
    DrawText("Next:", m_previewOrigin.x, m_previewOrigin.y-30, m_fontSize, m_textColor);
}

void Game::DrawHoldPiece(){
    if(m_hold.type != TetrominoType::None){
        Piece hold = m_hold;
        hold.position = {0,0};
        DrawPiece(hold, m_holdOrigin);
        DrawText("Hold:", m_holdOrigin.x, m_holdOrigin.y-30, m_fontSize, m_textColor);
    }
}

void Game::DrawStats(){
    std::string scoreText = "Score: " + std::to_string(m_score);
    std::string linesText = "Lines: " + std::to_string(m_linesCleared);
    std::string levelText = "Level: " + std::to_string(m_level);

    DrawText(scoreText.c_str(), m_previewOrigin.x, m_previewOrigin.y + 120, m_fontSize, m_textColor);
    DrawText(linesText.c_str(), m_previewOrigin.x, m_previewOrigin.y + 150, m_fontSize, m_textColor);
    DrawText(levelText.c_str(), m_previewOrigin.x, m_previewOrigin.y + 180, m_fontSize, m_textColor);
}

// -------------------------------------------------------------------

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawBoard();
    DrawPiece(m_current, m_boardOrigin);
    DrawNextPiece();
    DrawHoldPiece();
    DrawStats();

    EndDrawing();
}

// takes raw binary data and writes it to a file on disk
void Game::InstallBlob() {

}

void Game::Run() {
    InstallBlob();
    
    while(!WindowShouldClose()){
        Update();
        Draw();
    }
}