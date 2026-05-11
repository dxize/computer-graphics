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
        const std::filesystem::path& modelDirectory,
        int& currentMaterialIndex);

    bool LoadMtl(const std::string& mtlPath, Model& model);

    void ProcessMtlLine(
        const std::string& line,
        Model& model,
        Material& currentMaterial,
        bool& hasCurrentMaterial);

    void FinishCurrentMaterial(
        Model& model,
        Material& currentMaterial,
        bool& hasCurrentMaterial);

    void AddFaceTriangles(
        const std::vector<VertexReference>& faceVertices,
        int currentMaterialIndex,
        Model& model) const;

    VertexReference ParseFaceVertex(const std::string& token) const;
};