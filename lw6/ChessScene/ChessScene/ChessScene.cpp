#include "ChessScene.h"

#include <GLFW/glfw3.h>
#include <cmath>

const float BoardMin = -4.0f;
const float SquareSize = 1.0f;

const float WhiteRotationY = 270.0f;
const float BlackRotationY = 90.0f;

const float DesiredKingHeight = 1.25f;
const float Pi = 3.1415f;

bool ChessScene::Load()
{
    if (!LoadModels())
    {
        return false;
    }

    if (m_king.GetHeight() > 0.0001f)
    {
        m_pieceScale = DesiredKingHeight / m_king.GetHeight();
    }

    Restart();

    return true;
}

void ChessScene::Restart()
{
    m_pieces.clear();

    m_animationTime = 0.0f;

    m_whiteF2Pawn = -1;
    m_whiteG2Pawn = -1;
    m_blackE7Pawn = -1;
    m_blackQueen = -1;

    CreatePieces();
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
    return
        LoadModel("king.obj", m_king) &&
        LoadModel("queen.obj", m_queen) &&
        LoadModel("rook.obj", m_rook) &&
        LoadModel("bishop.obj", m_bishop) &&
        LoadModel("knight.obj", m_knight) &&
        LoadModel("pawn.obj", m_pawn);
}

bool ChessScene::LoadModel(const std::string& fileName, Model& model)
{
    return m_loader.Load("assets/models/" + fileName, model);
}

void ChessScene::CreatePieces()
{
    const Model* backRank[8] =
    {
        &m_rook,
        &m_knight,
        &m_bishop,
        &m_queen,
        &m_king,
        &m_bishop,
        &m_knight,
        &m_rook
    };

    CreateWhitePieces(backRank);
    CreateBlackPieces(backRank);
}

void ChessScene::CreateWhitePieces(const Model* backRank[8])
{
    for (int file = 0; file < 8; ++file)
    {
        AddPiece(*backRank[file], file, 0, true);

        int pawnIndex = AddPiece(m_pawn, file, 1, true);

        if (file == 5)
        {
            m_whiteF2Pawn = pawnIndex;
        }

        if (file == 6)
        {
            m_whiteG2Pawn = pawnIndex;
        }
    }
}

void ChessScene::CreateBlackPieces(const Model* backRank[8])
{
    for (int file = 0; file < 8; ++file)
    {
        int pawnIndex = AddPiece(m_pawn, file, 6, false);

        if (file == 4)
        {
            m_blackE7Pawn = pawnIndex;
        }

        int pieceIndex = AddPiece(*backRank[file], file, 7, false);

        if (file == 3)
        {
            m_blackQueen = pieceIndex;
        }
    }
}

int ChessScene::AddPiece(const Model& model, int file, int rank, bool isWhite)
{
    Vec3 position = GetSquarePosition(file, rank);

    ChessPiece piece;
    piece.ModelPtr = &model;
    piece.File = file;
    piece.Rank = rank;
    piece.X = position.X;
    piece.Z = position.Z;
    piece.IsWhite = isWhite;

    m_pieces.push_back(piece);

    return static_cast<int>(m_pieces.size()) - 1;
}

void ChessScene::UpdateAnimation(float deltaTime)
{
    m_animationTime += deltaTime;

    AnimatePiece(m_whiteF2Pawn, 5, 1, 5, 2, 0.5f, 1.0f);
    AnimatePiece(m_blackE7Pawn, 4, 6, 4, 4, 2.0f, 1.0f);
    AnimatePiece(m_whiteG2Pawn, 6, 1, 6, 3, 3.5f, 1.0f);
    AnimatePiece(m_blackQueen, 3, 7, 7, 3, 5.0f, 1.4f);
}

void ChessScene::AnimatePiece(
    int pieceIndex,
    int fromFile,
    int fromRank,
    int toFile,
    int toRank,
    float startTime,
    float duration)
{
    if (pieceIndex < 0)
    {
        return;
    }

    ChessPiece& piece = m_pieces[pieceIndex];

    Vec3 start = GetSquarePosition(fromFile, fromRank);
    Vec3 end = GetSquarePosition(toFile, toRank);

    if (m_animationTime < startTime)
    {
        piece.X = start.X;
        piece.Z = start.Z;
        piece.File = fromFile;
        piece.Rank = fromRank;
        return;
    }

    if (m_animationTime > startTime + duration)
    {
        piece.X = end.X;
        piece.Z = end.Z;
        piece.File = toFile;
        piece.Rank = toRank;
        return;
    }

    float t = (m_animationTime - startTime) / duration;

    piece.X = start.X + (end.X - start.X) * t;
    piece.Z = start.Z + (end.Z - start.Z) * t;
}

Vec3 ChessScene::GetSquarePosition(int file, int rank) const
{
    return
    {
        3.5f - static_cast<float>(file),
        0.0f,
        -3.5f + static_cast<float>(rank)
    };
}

void ChessScene::DrawBoard() const
{
    DrawBoardBase();
    DrawBoardSquares();
}

void ChessScene::DrawBoardBase() const
{
    ApplyMaterial(0.32f, 0.30f, 0.26f, 0.05f, 8.0f);

    glBegin(GL_QUADS);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(-4.3f, 0.02f, -4.3f);
        glVertex3f(4.3f, 0.02f, -4.3f);
        glVertex3f(4.3f, 0.02f, 4.3f);
        glVertex3f(-4.3f, 0.02f, 4.3f);

    glEnd();
}

void ChessScene::DrawBoardSquares() const
{
    for (int rank = 0; rank < 8; ++rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            DrawBoardSquare(file, rank);
        }
    }
}

void ChessScene::DrawBoardSquare(int file, int rank) const
{
    bool isLight = IsLightSquare(file, rank);

    ApplyBoardSquareMaterial(isLight);

    Vec3 center = GetSquarePosition(file, rank);

    float halfSize = SquareSize / 2.0f;

    float x0 = center.X - halfSize;
    float x1 = center.X + halfSize;

    float z0 = center.Z - halfSize;
    float z1 = center.Z + halfSize;

    DrawFlatSquare(x0, x1, z0, z1);
}

bool ChessScene::IsLightSquare(int file, int rank) const
{
    return ((file + rank) % 2) != 0;
}

void ChessScene::ApplyBoardSquareMaterial(bool isLight) const
{
    if (isLight)
    {
        ApplyMaterial(0.95f, 0.90f, 0.78f, 0.08f, 10.0f);
    }
    else
    {
        ApplyMaterial(0.20f, 0.20f, 0.20f, 0.25f, 25.0f);
    }
}

void ChessScene::DrawFlatSquare(float x0, float x1, float z0, float z1) const
{
    glBegin(GL_QUADS);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(x0, 0.06f, z0);
        glVertex3f(x1, 0.06f, z0);
        glVertex3f(x1, 0.06f, z1);
        glVertex3f(x0, 0.06f, z1);

    glEnd();
}

void ChessScene::DrawPieces() const
{
    for (int i = 0; i < static_cast<int>(m_pieces.size()); ++i)
    {
        DrawPiece(i);
    }
}

void ChessScene::DrawPiece(int index) const
{
    const ChessPiece& piece = m_pieces[index];

    if (piece.ModelPtr == nullptr)
    {
        return;
    }

    ApplyPieceMaterial(piece);

    float y = 0.10f + GetJumpHeight(index);
    float rotationY = piece.IsWhite ? WhiteRotationY : BlackRotationY;

    DrawModel(
        *piece.ModelPtr,
        piece.X,
        y,
        piece.Z,
        m_pieceScale,
        rotationY
    );
}

void ChessScene::DrawModel(
    const Model& model,
    float x,
    float y,
    float z,
    float scale,
    float rotationY) const
{
    Vec3 center = model.GetCenter();
    Vec3 min = model.GetMin();

    glPushMatrix();

    glTranslatef(x, y, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);

    glTranslatef(-center.X, -min.Y, -center.Z);

    model.Draw();

    glPopMatrix();
}

float ChessScene::GetJumpHeight(int pieceIndex) const
{
    float t = 0.0f;

    if (pieceIndex == m_whiteF2Pawn && GetMoveProgress(0.5f, 1.0f, t))
    {
        return std::sin(t * Pi) * 0.22f;
    }

    if (pieceIndex == m_blackE7Pawn && GetMoveProgress(2.0f, 1.0f, t))
    {
        return std::sin(t * Pi) * 0.22f;
    }

    if (pieceIndex == m_whiteG2Pawn && GetMoveProgress(3.5f, 1.0f, t))
    {
        return std::sin(t * Pi) * 0.22f;
    }

    if (pieceIndex == m_blackQueen && GetMoveProgress(5.0f, 1.4f, t))
    {
        return std::sin(t * Pi) * 0.22f;
    }

    return 0.0f;
}

bool ChessScene::GetMoveProgress(float startTime, float duration, float& progress) const
{
    if (m_animationTime < startTime)
    {
        return false;
    }

    if (m_animationTime > startTime + duration)
    {
        return false;
    }

    progress = (m_animationTime - startTime) / duration;

    if (progress < 0.0f)
    {
        progress = 0.0f;
    }

    if (progress > 1.0f)
    {
        progress = 1.0f;
    }

    return true;
}

void ChessScene::ApplyPieceMaterial(const ChessPiece& piece) const
{
    if (piece.IsWhite)
    {
        ApplyMaterial(0.85f, 0.82f, 0.70f, 0.08f, 12.0f);
    }
    else
    {
        ApplyMaterial(0.12f, 0.12f, 0.12f, 0.30f, 35.0f);
    }
}

void ChessScene::ApplyMaterial(
    float red,
    float green,
    float blue,
    float specular,
    float shininess) const
{
    float ambient[] =
    {
        red * 0.9f,
        green * 0.9f,
        blue * 0.9f,
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