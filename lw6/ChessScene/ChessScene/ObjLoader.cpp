#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool ObjLoader::Load(const std::string& objPath, Model& model)
{
    model.Clear();

    std::ifstream file(objPath);
    if (!file.is_open())
    {
        std::cout << "Не удалось открыть OBJ-файл: " << objPath << "\n";
        return false;
    }

    std::filesystem::path fullObjPath(objPath);
    std::filesystem::path modelDirectory = fullObjPath.parent_path();

    std::string line;
    while (std::getline(file, line))
    {
        if (!ProcessObjLine(line, model, modelDirectory))
        {
            return false;
        }
    }

    return true;
}

bool ObjLoader::ProcessObjLine(
    const std::string& line,
    Model& model,
    const std::filesystem::path& modelDirectory)
{
    std::istringstream stream(line);

    std::string command;
    if (!(stream >> command))
    {
        return true;
    }

    if (command == "v")
    {
        Vec3 vertex;
        stream >> vertex.X >> vertex.Y >> vertex.Z;
        model.AddVertex(vertex);
    }
    else if (command == "vn")
    {
        Vec3 normal;
        stream >> normal.X >> normal.Y >> normal.Z;
        model.AddNormal(normal);
    }
    else if (command == "f")
    {
        std::vector<VertexReference> faceVertices;

        std::string token;
        while (stream >> token)
        {
            faceVertices.push_back(ParseFaceVertex(token));
        }

        AddFaceTriangles(faceVertices, model);
    }

    return true;
}

void ObjLoader::AddFaceTriangles(
    const std::vector<VertexReference>& faceVertices,
    Model& model) const
{
    if (faceVertices.size() < 3)
    {
        return;
    }

    for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
    {
        Triangle triangle;
        triangle.Vertices[0] = faceVertices[0];
        triangle.Vertices[1] = faceVertices[i];
        triangle.Vertices[2] = faceVertices[i + 1];
        model.AddTriangle(triangle);
    }
}

VertexReference ObjLoader::ParseFaceVertex(const std::string& token) const
{
    VertexReference vertexReference;

    std::vector<std::string> parts;
    std::string currentPart;

    for (char ch : token)
    {
        if (ch == '/')
        {
            parts.push_back(currentPart);
            currentPart.clear();
        }
        else
        {
            currentPart += ch;
        }
    }
    parts.push_back(currentPart);

    if (!parts.empty() && !parts[0].empty())
    {
        vertexReference.PositionIndex = std::stoi(parts[0]) - 1;
    }

    if (parts.size() >= 3 && !parts[2].empty())
    {
        vertexReference.NormalIndex = std::stoi(parts[2]) - 1;
    }

    return vertexReference;
}