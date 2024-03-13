#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include <vector>


const float PI = 3.14159265359f;

void generateSphereVertices(float radius, int numSlices, int numStacks,
    std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<float>& scalarData) 
{
    vertices.clear();
    normals.clear();
    scalarData.clear();
    vertices.reserve((numStacks + 10) * (numSlices + 10) * 100);
    normals.reserve((numStacks + 10) * (numSlices + 10) * 100);
    scalarData.reserve((numStacks + 10) * (numSlices + 10) * 100);
    float phi, theta;
    glm::vec3 vertex;
    float scalar;
    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta = 2 * PI * j / numSlices;
            vertex.x = radius * sin(phi) * cos(theta);
            vertex.y = radius * sin(phi) * sin(theta);
            vertex.z = radius * cos(phi);
            scalar =  vertex.x;
            scalarData.emplace_back(scalar);
            vertices.emplace_back(vertex);
            glm::vec3 normal = glm::normalize(vertex); // 根据顶点位置计算法线向量
            normals.emplace_back(normal); // 存储法线向量
        }
    }
    glm::vec3 topVertex = glm::vec3(0.0f, 0.0f, radius);
    glm::vec3 bottomVertex = glm::vec3(0.0f, 0.0f, -radius);
    vertices.emplace_back(topVertex);
    vertices.emplace_back(bottomVertex);
    normals.emplace_back(glm::normalize(topVertex));
    normals.emplace_back(glm::normalize(bottomVertex));
    scalarData.emplace_back(0);
    scalarData.emplace_back(0);
    return;
}

void generateSphereIndices(int numSlices, int numStacks, std::vector<unsigned int>& indices)
{
    indices.clear();
    indices.reserve((numStacks + 10) * (numSlices + 10) * 600);
    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            int k1 = i * (numSlices + 1) + j;
            int k2 = k1 + (numSlices + 1);

            indices.emplace_back(k1);
            indices.emplace_back(k2);
            indices.emplace_back(k1 + 1);

            indices.emplace_back(k1 + 1);
            indices.emplace_back(k2);
            indices.emplace_back(k2 + 1);
        }
    }
    return;
}


void generateCylinderVertices(float radius, float height, int numSlices, int numStacks,
    std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<float>& scalarData)
{
    vertices.clear();
    normals.clear();
    scalarData.clear();
    vertices.reserve((numStacks + 1) * (numSlices + 1));
    normals.reserve((numStacks + 1) * (numSlices + 1));
    scalarData.reserve((numStacks + 1) * (numSlices + 1));

    float phi, theta;
    glm::vec3 vertex;
    float scalar;

    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta =  PI * j / numSlices;
            vertex.x = radius * cos(theta);
            vertex.y = radius * sin(theta);
            vertex.z = height * i / numStacks;
            scalar = vertex.x;
            scalarData.emplace_back(scalar);
            vertices.emplace_back(vertex);
            glm::vec3 normal = glm::normalize(glm::vec3(vertex.x, vertex.y, 0.0f)); // 法线垂直于切面
            normals.emplace_back(normal);
        }
    }

    return;
}

void generateCylinderIndices(int numSlices, int numStacks, std::vector<unsigned int>& indices)
{
    indices.clear();
    indices.reserve(numStacks * numSlices * 6);

    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            int k1 = i * (numSlices + 1) + j;
            int k2 = k1 + (numSlices + 1);

            indices.emplace_back(k1);
            indices.emplace_back(k2);
            indices.emplace_back(k1 + 1);

            indices.emplace_back(k1 + 1);
            indices.emplace_back(k2);
            indices.emplace_back(k2 + 1);
        }
    }

    return;
}

void generateTwoCylindersVertices(float radius1, float radius2, float height, int numSlices, int numStacks,
    std::vector<double>& vertices, std::vector<float>& normals, std::vector<float>& scalarData)
{
    vertices.clear();
    normals.clear();
    scalarData.clear();
    vertices.reserve((numStacks + 1) * (numSlices + 1));
    normals.reserve((numStacks + 1) * (numSlices + 1));
    scalarData.reserve((numStacks + 1) * (numSlices + 1));

    float phi, theta;
    float x, y, z;
    float scalar;

    // Cylinder with radius1
    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta = PI * j / numSlices;
            x = radius1 * cos(theta);
            y = radius1 * sin(theta);
            z = height * i / numStacks;
            scalar = x;
            scalarData.emplace_back(scalar);
            vertices.emplace_back(x);
            vertices.emplace_back(y);
            vertices.emplace_back(z);
            float normalX = x;
            float normalY = y;
            float normalZ = 0.0f; // 法线垂直于切面
            float normalLength = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
            normals.emplace_back(normalX / normalLength);
            normals.emplace_back(normalY / normalLength);
            normals.emplace_back(normalZ / normalLength);
        }
    }

    // Cylinder with radius2
    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta = PI * j / numSlices;
            x = radius2 * cos(theta);
            y = radius2 * sin(theta);
            z = height * i / numStacks;
            scalar = x;
            scalarData.emplace_back(scalar);
            vertices.emplace_back(x);
            vertices.emplace_back(y);
            vertices.emplace_back(z);
            float normalX = x;
            float normalY = y;
            float normalZ = 0.0f; // 法线垂直于切面
            float normalLength = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);
            normals.emplace_back(normalX / normalLength);
            normals.emplace_back(normalY / normalLength);
            normals.emplace_back(normalZ / normalLength);
        }
    }
}

void generateTwoCylindersVertices(float radius1, float radius2, float height, int numSlices, int numStacks,
    std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<float>& scalarData)
{
    vertices.clear();
    normals.clear();
    scalarData.clear();
    vertices.reserve((numStacks + 1) * (numSlices + 1));
    normals.reserve((numStacks + 1) * (numSlices + 1));
    scalarData.reserve((numStacks + 1) * (numSlices + 1));

    float phi, theta;
    glm::vec3 vertex;
    float scalar;

    // Cylinder with radius1
    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta = PI * j / numSlices;
            vertex.x = radius1 * cos(theta);
            vertex.y = radius1 * sin(theta);
            vertex.z = height * i / numStacks;
            scalar = vertex.x;
            scalarData.emplace_back(scalar);
            vertices.emplace_back(vertex);
            glm::vec3 normal = glm::normalize(glm::vec3(vertex.x, vertex.y, 0.0f)); // 法线垂直于切面
            normals.emplace_back(normal);
        }
    }

    // Cylinder with radius2
    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta = PI * j / numSlices;
            vertex.x = radius2 * cos(theta);
            vertex.y = radius2 * sin(theta);
            vertex.z = height * i / numStacks;
            scalar = vertex.x;
            scalarData.emplace_back(scalar);
            vertices.emplace_back(vertex);
            glm::vec3 normal = glm::normalize(glm::vec3(vertex.x, vertex.y, 0.0f)); // 法线垂直于切面
            normals.emplace_back(normal);
        }
    }

    return;
}

void generateTwoCylindersIndices(int numSlices, int numStacks, std::vector<unsigned int>& indices)
{
    indices.clear();
    indices.reserve(20 * numStacks * numSlices * 6);

    // Indices for the first cylinder
    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            int k1 = i * (numSlices + 1) + j;
            int k2 = k1 + (numSlices + 1);

            indices.emplace_back(k1);
            indices.emplace_back(k2);
            indices.emplace_back(k1 + 1);

            indices.emplace_back(k1 + 1);
            indices.emplace_back(k2);
            indices.emplace_back(k2 + 1);
        }
    }

    // Indices for the second cylinder
    int offset = (numSlices + 1) * (numStacks + 1);
    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            int k1 = i * (numSlices + 1) + j + offset;
            int k2 = k1 + (numSlices + 1);

            indices.emplace_back(k1);
            indices.emplace_back(k2);
            indices.emplace_back(k1 + 1);

            indices.emplace_back(k1 + 1);
            indices.emplace_back(k2);
            indices.emplace_back(k2 + 1);
        }
    }

    return;
}


std::vector<uint8_t> createLut(int numColors, double startHue, double endHue)
{
    std::vector<uint8_t> colors;
    colors.reserve(4 * numColors);
    for (int i = 0; i < numColors; i++)
    {
        double hue = endHue - (i / static_cast<double>(numColors - 1)) * (endHue - startHue);
        double saturation = 1.0;
        double value = 1.0;
        int h = static_cast<int>(hue * 6);
        double f = hue * 6 - h;
        double p = value * (1 - saturation);
        double q = value * (1 - f * saturation);
        double t = value * (1 - (1 - f) * saturation);
        auto addColor = [&colors](double a, double b, double c)
        {
            colors.emplace_back(static_cast<uint8_t>(a * 255));
            colors.emplace_back(static_cast<uint8_t>(b * 255));
            colors.emplace_back(static_cast<uint8_t>(c * 255));
            colors.emplace_back(static_cast<uint8_t>(255));
        };
        switch (h)
        {
        case 0:
            addColor(value, t, p);
            break;
        case 1:
            addColor(q, value, p);
            break;
        case 2:
            addColor(p, value, t);
            break;
        case 3:
            addColor(p, q, value);
            break;
        case 4:
            addColor(t, p, value);
            break;
        default:
            addColor(value, p, q);
            break;
        }
    }
    return colors;
}

