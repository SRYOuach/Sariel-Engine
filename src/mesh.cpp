#include "mesh.hpp"

// --- initialize the generate a mesh of a given and predefined shape preset ---
sariel::Mesh::Mesh(uint preset)
{
    // generate vertix & index data
    generateData(preset);

    // generate the VAO, VBO & EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the vertex array to then modify the vertex and index buffers
    glBindVertexArray(VAO);

    // bind and sending vertix data to the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(sariel::vertix), vertices.data(), GL_STATIC_DRAW);

    // bind and sending element index data to the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    // --- specify vertix attribute layout to the vertex array buffer ---
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);

    // light normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    // texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void *)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    // unbind the VAO
    glBindVertexArray(0);
}

// --- move buffers' ownerships from one mesh object to the other and free the sender's stored data ---
sariel::Mesh::Mesh(Mesh &&other) noexcept
    : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), vertices(std::move(other.vertices)),
      indices(std::move(other.indices))
{
    other.EBO = other.VAO = other.VBO = 0;
}

sariel::Mesh &sariel::Mesh::operator=(Mesh &&other) noexcept
{
    if (this != &other)
    {
        // release our own GL resources first
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        // steal other's resources
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);

        // leave other in a safely-destructible state
        other.VAO = other.VBO = other.EBO = 0;
    }
    return *this;
}

// --- destruct a mesh object and free the corresponding allocated memory on the VRAM ---
sariel::Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// --- generate the vertix and index data of a given, predefined shape preset ---
void sariel::Mesh::generateData(uint preset)
{
    switch (preset)
    {
    // generate the vertix and index data of a trinagle
    case SARIEL_TRI:

        vertices.reserve(3);
        // insert three vertices (positions & tex coords) counter clockwise
        vertices.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        vertices.emplace_back(0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
        vertices.emplace_back(0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f);

        // draw them counter clockwise
        indices.insert(indices.end(), {0, 1, 2});
        break;
    // generate the vertix and index data of a square
    case SARIEL_QUAD:

        vertices.reserve(4);
        // insert four vertices (positions & tex coords) counter clockwise
        vertices.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0);
        vertices.emplace_back(0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0);
        vertices.emplace_back(0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 1);
        vertices.emplace_back(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 1);

        // draw them counter clockwise
        indices.insert(indices.end(), {0, 1, 2, 0, 3, 2});
        break;
    // generate the vertix and index data of a circle
    case SARIEL_CIRCLE: {
        vertices.reserve(101);
        indices.reserve(300);

        // insert 1 origin vertex (positions & tex coords)
        vertices.emplace_back(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f);

        // insert 100 vertices around the origin equally spaced out and equally far from the origin counter clockwise
        f32 theta = 0.0f, x = cosf(theta) / 2, y = sinf(theta) / 2;
        for (int i = 0; i < 100; i++)
        {
            theta += (2.0 * SARIEL_PI) / 100.0;
            x = cosf(theta) / 2;
            y = sinf(theta) / 2;

            vertices.emplace_back(x, y, 0.0f, 0.0f, 0.0f, 1.0f, x + 0.5, y + 0.5);
        }
        // draw them counter clockwise
        for (int i = 1; i <= 100; i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back((i % 100) + 1);
        }
        break;
    }
    // generate the vertix and idnex data of a cube
    case SARIEL_CUBE: {
        vertices.reserve(24);
        indices.reserve(36);

        // insert 24 vertices (4 per face, 6 faces) with positions & tex coords
        // front face (+Z)
        vertices.emplace_back(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        vertices.emplace_back(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
        vertices.emplace_back(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        vertices.emplace_back(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);

        // back face (-Z)
        vertices.emplace_back(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
        vertices.emplace_back(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
        vertices.emplace_back(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

        // right face (+X)
        vertices.emplace_back(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        vertices.emplace_back(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        vertices.emplace_back(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        vertices.emplace_back(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

        // left face (-X)
        vertices.emplace_back(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        vertices.emplace_back(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        vertices.emplace_back(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        vertices.emplace_back(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

        // top face (+Y)
        vertices.emplace_back(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
        vertices.emplace_back(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
        vertices.emplace_back(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
        vertices.emplace_back(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

        // bottom face (-Y)
        vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
        vertices.emplace_back(0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
        vertices.emplace_back(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
        vertices.emplace_back(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

        // draw each face as two triangles, counter clockwise (when viewed from outside)
        for (int face = 0; face < 6; face++)
        {
            int base = face * 4;
            indices.push_back(base + 0);
            indices.push_back(base + 1);
            indices.push_back(base + 2);

            indices.push_back(base + 0);
            indices.push_back(base + 2);
            indices.push_back(base + 3);
        }
        break;
    }
    case SARIEL_SPHERE: {
        vertices.reserve(10302);
        indices.reserve(60600);

        const f32 scale = 0.5f;

        f32 theta = 0.0f;
        f32 y = scale * cosf(theta);
        f32 d = scale * sinf(theta);
        f32 v = 1.0f - theta / SARIEL_PI;

        f32 zeta = 0.0f;
        f32 z = d * cosf(zeta);
        f32 x = d * sinf(zeta);
        f32 u = zeta / (2.0f * SARIEL_PI);

        // circle through sample points of equal latitude from north to south
        for (uint i = 0; i < 102; i++)
        {
            zeta = 0.0f, z = d, x = 0.0f;
            // circle through sample points of equal longitude at the current longitude starting on Z+

            vertices.emplace_back(x, y, z, x / scale, y / scale, z / scale, 0.0f, v);
            for (uint j = 0; j < 100; j++)
            {
                zeta += (2.0f * SARIEL_PI) / 100.0f;
                z = d * cosf(zeta);
                x = d * sinf(zeta);

                u = zeta / (2.0f * SARIEL_PI);

                vertices.emplace_back(x, y, z, x / scale, y / scale, z / scale, u, v);
            }

            theta += SARIEL_PI / 101.0f;
            y = scale * cosf(theta);
            d = scale * sinf(theta);
            v = 1.0f - theta / SARIEL_PI;
        }

        // indices of the surface quads of the sphere
        for (uint i = 0; i < 101; i++)
        {
            for (uint j = 0; j < 100; j++)
            {
                indices.push_back((101 * i) + j);
                indices.push_back((101 * i) + 101 + j);
                indices.push_back((101 * i) + j + 1);

                indices.push_back((101 * i) + j + 1);
                indices.push_back((101 * i) + 101 + j);
                indices.push_back((101 * i) + 101 + j + 1);
            }
        }

        break;
    }
    default:
        break;
    }
}
// --- transform the mesh's vertices' position vectors by a given transformation matrix ---
void sariel::Mesh::transform(glm::mat4 transformation)
{
    for (auto &vertex : vertices)
    {
        vertex.position = glm::vec3(transformation * glm::vec4(vertex.position, 1.0f));
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(sariel::vertix), vertices.data());
}

// --- return the Vertex Array Object's identifier ---
uint sariel::Mesh::getID() const
{
    return VAO;
}

// --- return the count of indices of the mesh ---
uint sariel::Mesh::indiceCount() const
{
    return static_cast<uint>(indices.size());
}
