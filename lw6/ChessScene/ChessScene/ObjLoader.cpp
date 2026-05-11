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

    int currentMaterialIndex = -1;

    std::string line;
    while (std::getline(file, line))
    {
        if (!ProcessObjLine(line, model, modelDirectory, currentMaterialIndex))
        {
            return false;
        }
    }

    return true;
}

bool ObjLoader::ProcessObjLine(
    const std::string& line,
    Model& model,
    const std::filesystem::path& modelDirectory,
    int& currentMaterialIndex)
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

        AddFaceTriangles(faceVertices, currentMaterialIndex, model);
    }
    else if (command == "mtllib")
    {
        std::string mtlFileName;
        stream >> mtlFileName;

        std::filesystem::path mtlPath = modelDirectory / mtlFileName;
        if (!LoadMtl(mtlPath.string(), model))
        {
            return false;
        }
    }
    else if (command == "usemtl")
    {
        std::string materialName;
        stream >> materialName;

        currentMaterialIndex = model.FindMaterialIndex(materialName);
    }

    return true;
}

void ObjLoader::AddFaceTriangles(
    const std::vector<VertexReference>& faceVertices,
    int currentMaterialIndex,
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
        triangle.MaterialIndex = currentMaterialIndex;

        model.AddTriangle(triangle);
    }
}

bool ObjLoader::LoadMtl(const std::string& mtlPath, Model& model)
{
    std::ifstream file(mtlPath);
    if (!file.is_open())
    {
        std::cout << "Не удалось открыть MTL-файл: " << mtlPath << "\n";
        return false;
    }

    Material currentMaterial;
    bool hasCurrentMaterial = false;

    std::string line;
    while (std::getline(file, line))
    {
        ProcessMtlLine(line, model, currentMaterial, hasCurrentMaterial);
    }

    FinishCurrentMaterial(model, currentMaterial, hasCurrentMaterial);

    return true;
}

void ObjLoader::ProcessMtlLine(
    const std::string& line,
    Model& model,
    Material& currentMaterial,
    bool& hasCurrentMaterial)
{
    std::istringstream stream(line);

    std::string command;
    if (!(stream >> command))
    {
        return;
    }

    if (command == "newmtl")
    {
        FinishCurrentMaterial(model, currentMaterial, hasCurrentMaterial);

        currentMaterial = Material();
        stream >> currentMaterial.Name;
        hasCurrentMaterial = true;
    }
    else if (command == "Ka" && hasCurrentMaterial)
    {
        stream
            >> currentMaterial.Ambient[0]
            >> currentMaterial.Ambient[1]
            >> currentMaterial.Ambient[2];

        currentMaterial.Ambient[3] = 1.0f;
    }
    else if (command == "Kd" && hasCurrentMaterial)
    {
        stream
            >> currentMaterial.Diffuse[0]
            >> currentMaterial.Diffuse[1]
            >> currentMaterial.Diffuse[2];

        currentMaterial.Diffuse[3] = 1.0f;
    }
    else if (command == "Ks" && hasCurrentMaterial)
    {
        stream
            >> currentMaterial.Specular[0]
            >> currentMaterial.Specular[1]
            >> currentMaterial.Specular[2];

        currentMaterial.Specular[3] = 1.0f;
    }
    else if (command == "Ns" && hasCurrentMaterial)
    {
        stream >> currentMaterial.Shininess;
    }
}

void ObjLoader::FinishCurrentMaterial(
    Model& model,
    Material& currentMaterial,
    bool& hasCurrentMaterial)
{
    if (hasCurrentMaterial)
    {
        model.AddMaterial(currentMaterial);
        hasCurrentMaterial = false;
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