#include "Mesh.h"


		
Engine::Mesh::Mesh(const std::vector<Vertex> vertecies, const std::vector<unsigned int> indecies) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertecies.size(), vertecies.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) &(((Vertex*)0)->position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(((Vertex*)0)->normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(((Vertex*)0)->uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)&(((Vertex*)0)->color));
    glEnableVertexAttribArray(3);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.size(), indecies.data(), GL_STATIC_DRAW);

    m_indexies_count = indecies.size();
}
Engine::Mesh::~Mesh() {
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Engine::Mesh::Bind() {
    glBindVertexArray(m_VAO);
}
void Engine::Mesh::Draw() {
    glDrawElements(GL_TRIANGLES, m_indexies_count, GL_UNSIGNED_INT, 0);
}