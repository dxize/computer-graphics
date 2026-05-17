#pragma once

#include "Model.h"

#include <filesystem>
#include <string>
#include <vector>

class ObjLoader
{
public:
    bool Load(const std::string& objPath, Model& model);

private:
    bool ProcessObjLine(
        const std::string& line,
        Model& model,
        const std::filesystem::path& modelDirectory);

    void AddFaceTriangles(
        const std::vector<VertexReference>& faceVertices,
        Model& model) const;

    VertexReference ParseFaceVertex(const std::string& token) const;
};