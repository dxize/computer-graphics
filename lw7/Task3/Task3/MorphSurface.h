#pragma once

#include "Drawable.h"
#include "Vertex.h"

#include <GL/glew.h>

#include <vector>

class MorphSurface final : public Drawable
{
public:
    MorphSurface(int segmentsX, int segmentsY);
    ~MorphSurface() override;

    void build() override;
    void draw() const override;

private:
    std::vector<Vertex> createVertices() const;
    std::vector<unsigned int> createIndices() const;

    void uploadToGpu(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices
    );

    void createGpuObjects();
    void uploadVertexBuffer(const std::vector<Vertex>& vertices);
    void uploadIndexBuffer(const std::vector<unsigned int>& indices);
    void setupVertexAttributes();

    void release();

private:
    int m_segmentsX = 0;
    int m_segmentsY = 0;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;

    GLsizei m_indexCount = 0;
};