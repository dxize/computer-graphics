#pragma once

#include "Model.h"
#include "ObjLoader.h"
#include "MathTypes.h"

#include <string>
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

class ChessScene
{
public:
    bool Load();
    void Restart();
    void Update(float deltaTime);
    void Draw() const;

private:
    bool LoadModels();
    bool LoadModel(const std::string& fileName, Model& model);

    int AddPiece(const Model& model, int file, int rank, bool isWhite);

    void CreatePieces();
    void CreateWhitePieces(const Model* backRank[8]);
    void CreateBlackPieces(const Model* backRank[8]);
    void UpdateAnimation(float deltaTime);

    void AnimatePiece(
        int pieceIndex,
        int fromFile,
        int fromRank,
        int toFile,
        int toRank,
        float startTime,
        float duration
    );

    Vec3 GetSquarePosition(int file, int rank) const;

    void DrawBoard() const;
    void DrawPieces() const;
    void DrawPiece(int index) const;

    void DrawModel(
        const Model& model,
        float x,
        float y,
        float z,
        float scale,
        float rotationY
    ) const;

    float GetJumpHeight(int pieceIndex) const;
    bool GetMoveProgress(float startTime, float duration, float& progress) const;

    void ApplyPieceMaterial(const ChessPiece& piece) const;
    void ApplyMaterial(
        float red,
        float green,
        float blue,
        float specular,
        float shininess
    ) const;

private:
    ObjLoader m_loader;

    Model m_king;
    Model m_queen;
    Model m_rook;
    Model m_bishop;
    Model m_knight;
    Model m_pawn;

    std::vector<ChessPiece> m_pieces;

    float m_pieceScale = 1.0f;
    float m_animationTime = 0.0f;

    int m_whiteF2Pawn = -1;
    int m_whiteG2Pawn = -1;
    int m_blackE7Pawn = -1;
    int m_blackQueen = -1;
};