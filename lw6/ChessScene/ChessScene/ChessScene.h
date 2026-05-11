#pragma once

#include "Model.h"
#include "ObjLoader.h"
#include "MathTypes.h"

#include <vector>

struct ChessPiece
{
    const Model* ModelPtr = nullptr;

    int File = 0;
    int Rank = 0;

    float X = 0.0f;
    float Z = 0.0f;

    bool IsWhite = true;
};

struct ChessMove
{
    int PieceIndex = -1;

    int ToFile = 0;
    int ToRank = 0;

    float Duration = 1.0f;

    float StartX = 0.0f;
    float StartZ = 0.0f;

    float EndX = 0.0f;
    float EndZ = 0.0f;
};

class ChessScene
{
public:
    bool Load();

    void Restart();
    void Update(float deltaTime);
    void Draw() const;

private:
    bool LoadModels();

    void ArrangePieces();

    int AddPiece(
        const Model& model,
        int file,
        int rank,
        bool isWhite,
        float rotationY
    );

    void AddMove(
        int pieceIndex,
        int toFile,
        int toRank,
        float duration
    );

    void ResetAnimation();
    void UpdateAnimation(float deltaTime);

    Vec3 CalculateSquarePosition(int file, int rank) const;

    void DrawBoard() const;
    void DrawPieces() const;

    void DrawModel(
        const Model& model,
        float x,
        float y,
        float z,
        float scale,
        float rotationY
    ) const;

    void ApplyMaterial(
        float red,
        float green,
        float blue,
        float specular,
        float shininess
    ) const;

    float CalculateLerp(float from, float to, float t) const;
    float CalculateSmoothStep(float value) const;

private:
    ObjLoader m_loader;

    Model m_king;
    Model m_queen;
    Model m_rook;
    Model m_bishop;
    Model m_knight;
    Model m_pawn;

    std::vector<ChessPiece> m_pieces;
    std::vector<float> m_pieceRotations;
    std::vector<ChessMove> m_moves;

    float m_pieceScale = 1.0f;

    int m_currentMoveIndex = 0;
    int m_matedKingIndex = -1;

    float m_moveTime = 0.0f;
    float m_pauseTime = 0.4f;
    float m_victoryTime = 0.0f;

    bool m_isAnimationFinished = false;
};