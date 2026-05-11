#include "ChessScene.h"

#include <GLFW/glfw3.h>

#include <cmath>
#include <string>

bool ChessScene::Load()
{
    if (!LoadModels())
    {
        return false;
    }

    if (m_king.GetHeight() > 0.0001f)
    {
        m_pieceScale = 1.25f / m_king.GetHeight();
    }

    ArrangePieces();

    return true;
}

void ChessScene::Restart()
{
    ArrangePieces();
}

void ChessScene::Update(float deltaTime)
{
    UpdateAnimation(deltaTime);
}

void ChessScene::Draw() const
{
    DrawBoard();
    DrawPieces();
}

bool ChessScene::LoadModels()
{
    const std::string basePath = "assets/models/";

    bool result = true;

    result = result && m_loader.Load(basePath + "king.obj", m_king);
    result = result && m_loader.Load(basePath + "queen.obj", m_queen);
    result = result && m_loader.Load(basePath + "rook.obj", m_rook);
    result = result && m_loader.Load(basePath + "bishop.obj", m_bishop);
    result = result && m_loader.Load(basePath + "knight.obj", m_knight);
    result = result && m_loader.Load(basePath + "pawn.obj", m_pawn);

    return result;
}

void ChessScene::ArrangePieces()
{
    m_pieces.clear();
    m_pieceRotations.clear();
    m_moves.clear();

    ResetAnimation();

    /*
        Поворот фигур.

        Если модели повернуты на 90 градусов не туда,
        эти две константы исправляют направление.

        Белые и чёрные должны смотреть друг на друга,
        поэтому разница между ними = 180 градусов.
    */
    const float whiteRotation = 270.0f;
    const float blackRotation = 90.0f;

    int whiteF2Pawn = -1;
    int whiteG2Pawn = -1;
    int whiteKing = -1;

    int blackE7Pawn = -1;
    int blackQueen = -1;

    // Белые основные фигуры
    AddPiece(m_rook, 0, 0, true, whiteRotation);
    AddPiece(m_knight, 1, 0, true, whiteRotation);
    AddPiece(m_bishop, 2, 0, true, whiteRotation);
    AddPiece(m_queen, 3, 0, true, whiteRotation);

    whiteKing = AddPiece(m_king, 4, 0, true, whiteRotation);
    m_matedKingIndex = whiteKing;

    AddPiece(m_bishop, 5, 0, true, whiteRotation);
    AddPiece(m_knight, 6, 0, true, whiteRotation);
    AddPiece(m_rook, 7, 0, true, whiteRotation);

    // Белые пешки
    for (int file = 0; file < 8; ++file)
    {
        int index = AddPiece(m_pawn, file, 1, true, whiteRotation);

        if (file == 5)
        {
            whiteF2Pawn = index;
        }

        if (file == 6)
        {
            whiteG2Pawn = index;
        }
    }

    // Чёрные основные фигуры
    AddPiece(m_rook, 0, 7, false, blackRotation);
    AddPiece(m_knight, 1, 7, false, blackRotation);
    AddPiece(m_bishop, 2, 7, false, blackRotation);

    blackQueen = AddPiece(m_queen, 3, 7, false, blackRotation);

    AddPiece(m_king, 4, 7, false, blackRotation);
    AddPiece(m_bishop, 5, 7, false, blackRotation);
    AddPiece(m_knight, 6, 7, false, blackRotation);
    AddPiece(m_rook, 7, 7, false, blackRotation);

    // Чёрные пешки
    for (int file = 0; file < 8; ++file)
    {
        int index = AddPiece(m_pawn, file, 6, false, blackRotation);

        if (file == 4)
        {
            blackE7Pawn = index;
        }
    }

    /*
        Анимация партии: мат дурака.

        1. f2 -> f3
        2. e7 -> e5
        3. g2 -> g4
        4. Qd8 -> h4#
    */

    AddMove(whiteF2Pawn, 5, 2, 1.0f);
    AddMove(blackE7Pawn, 4, 4, 1.0f);
    AddMove(whiteG2Pawn, 6, 3, 1.0f);
    AddMove(blackQueen, 7, 3, 1.4f);
}

int ChessScene::AddPiece(
    const Model& model,
    int file,
    int rank,
    bool isWhite,
    float rotationY
)
{
    Vec3 position = CalculateSquarePosition(file, rank);

    ChessPiece piece;

    piece.ModelPtr = &model;
    piece.File = file;
    piece.Rank = rank;
    piece.X = position.X;
    piece.Z = position.Z;
    piece.IsWhite = isWhite;

    m_pieces.push_back(piece);
    m_pieceRotations.push_back(rotationY);

    return static_cast<int>(m_pieces.size()) - 1;
}

void ChessScene::AddMove(
    int pieceIndex,
    int toFile,
    int toRank,
    float duration
)
{
    if (pieceIndex < 0)
    {
        return;
    }

    ChessMove move;

    move.PieceIndex = pieceIndex;
    move.ToFile = toFile;
    move.ToRank = toRank;
    move.Duration = duration;

    m_moves.push_back(move);
}

void ChessScene::ResetAnimation()
{
    m_currentMoveIndex = 0;

    m_moveTime = 0.0f;
    m_pauseTime = 0.4f;
    m_victoryTime = 0.0f;

    m_isAnimationFinished = false;
}

void ChessScene::UpdateAnimation(float deltaTime)
{
    if (m_isAnimationFinished)
    {
        m_victoryTime += deltaTime;
        return;
    }

    if (m_currentMoveIndex >= static_cast<int>(m_moves.size()))
    {
        m_isAnimationFinished = true;
        return;
    }

    if (m_pauseTime > 0.0f)
    {
        m_pauseTime -= deltaTime;
        return;
    }

    ChessMove& move = m_moves[m_currentMoveIndex];
    ChessPiece& piece = m_pieces[move.PieceIndex];

    if (m_moveTime == 0.0f)
    {
        Vec3 endPosition = CalculateSquarePosition(move.ToFile, move.ToRank);

        move.StartX = piece.X;
        move.StartZ = piece.Z;

        move.EndX = endPosition.X;
        move.EndZ = endPosition.Z;
    }

    m_moveTime += deltaTime;

    float progress = m_moveTime / move.Duration;

    if (progress > 1.0f)
    {
        progress = 1.0f;
    }

    float smoothProgress = CalculateSmoothStep(progress);

    piece.X = CalculateLerp(move.StartX, move.EndX, smoothProgress);
    piece.Z = CalculateLerp(move.StartZ, move.EndZ, smoothProgress);

    if (progress >= 1.0f)
    {
        piece.File = move.ToFile;
        piece.Rank = move.ToRank;

        piece.X = move.EndX;
        piece.Z = move.EndZ;

        ++m_currentMoveIndex;

        m_moveTime = 0.0f;
        m_pauseTime = 0.45f;
    }
}

Vec3 ChessScene::CalculateSquarePosition(int file, int rank) const
{
    return {
        -3.5f + static_cast<float>(file),
        0.12f,
        -3.5f + static_cast<float>(rank)
    };
}

void ChessScene::DrawBoard() const
{
    const float boardY = 0.05f;
    const float squareY = 0.08f;

    // Рамка доски.
    ApplyMaterial(0.34f, 0.33f, 0.30f, 0.03f, 8.0f);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-4.25f, boardY, -4.25f);
    glVertex3f(4.25f, boardY, -4.25f);
    glVertex3f(4.25f, boardY, 4.25f);
    glVertex3f(-4.25f, boardY, 4.25f);

    glEnd();

    for (int rank = 0; rank < 8; ++rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            bool isLight = (file + rank) % 2 == 0;

            if (isLight)
            {
                // Светлая клетка под цвет белых фигур.
                ApplyMaterial(1.00f, 1.00f, 0.94f, 0.08f, 8.0f);
            }
            else
            {
                // Тёмная клетка под цвет чёрных фигур.
                ApplyMaterial(0.28f, 0.28f, 0.28f, 0.20f, 25.0f);
            }

            float x0 = -4.0f + static_cast<float>(file);
            float x1 = x0 + 1.0f;

            float z0 = -4.0f + static_cast<float>(rank);
            float z1 = z0 + 1.0f;

            glBegin(GL_QUADS);

            glNormal3f(0.0f, 1.0f, 0.0f);

            glVertex3f(x0, squareY, z0);
            glVertex3f(x1, squareY, z0);
            glVertex3f(x1, squareY, z1);
            glVertex3f(x0, squareY, z1);

            glEnd();
        }
    }
}
void ChessScene::DrawPieces() const
{
    for (int i = 0; i < static_cast<int>(m_pieces.size()); ++i)
    {
        const ChessPiece& piece = m_pieces[i];

        if (piece.ModelPtr == nullptr)
        {
            continue;
        }

        bool isMatedKing =
            m_isAnimationFinished &&
            i == m_matedKingIndex;

        if (isMatedKing)
        {
            // Белый король после мата слегка выделяется тёплым цветом.
            ApplyMaterial(0.95f, 0.62f, 0.45f, 0.08f, 12.0f);
        }
        else if (piece.IsWhite)
        {
            ApplyMaterial(0.82f, 0.80f, 0.72f, 0.05f, 10.0f);
        }
        else
        {
            ApplyMaterial(0.18f, 0.18f, 0.18f, 0.20f, 25.0f);
        }

        float y = 0.14f;
        float scale = m_pieceScale;
        float rotationY = m_pieceRotations[i];

        bool isCurrentMovingPiece =
            !m_isAnimationFinished &&
            m_currentMoveIndex < static_cast<int>(m_moves.size()) &&
            m_moves[m_currentMoveIndex].PieceIndex == i;

        if (isCurrentMovingPiece)
        {
            float progress = m_moveTime / m_moves[m_currentMoveIndex].Duration;

            if (progress > 1.0f)
            {
                progress = 1.0f;
            }

            y += std::sin(progress * 3.1415926535f) * 0.25f;
        }

        if (isMatedKing)
        {
            // При мате трясётся именно белый король.
            y += std::sin(m_victoryTime * 18.0f) * 0.05f;
            rotationY += std::sin(m_victoryTime * 22.0f) * 8.0f;
            scale *= 1.0f + std::sin(m_victoryTime * 10.0f) * 0.03f;
        }

        DrawModel(
            *piece.ModelPtr,
            piece.X,
            y,
            piece.Z,
            scale,
            rotationY
        );
    }
}

void ChessScene::DrawModel(
    const Model& model,
    float x,
    float y,
    float z,
    float scale,
    float rotationY
) const
{
    Vec3 center = model.GetCenter();
    Vec3 min = model.GetMin();

    glPushMatrix();

    glTranslatef(x, y, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);

    /*
        Исправление локальных координат модели:
        - центр модели ставим в центр клетки;
        - нижнюю точку модели ставим на доску.
    */
    glTranslatef(-center.X, -min.Y, -center.Z);

    model.Draw(false);

    glPopMatrix();
}

void ChessScene::ApplyMaterial(
    float red,
    float green,
    float blue,
    float specular,
    float shininess
) const
{
    float ambient[] =
    {
        red * 0.60f,
        green * 0.60f,
        blue * 0.60f,
        1.0f
    };

    float diffuse[] =
    {
        red,
        green,
        blue,
        1.0f
    };

    float specularColor[] =
    {
        specular,
        specular,
        specular,
        1.0f
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

float ChessScene::CalculateLerp(float from, float to, float t) const
{
    return from + (to - from) * t;
}

float ChessScene::CalculateSmoothStep(float value) const
{
    return value * value * (3.0f - 2.0f * value);
}