#include "Textures.h"



Engine::Textures::Textures(const std::string texture_name) {
    std::vector<unsigned char> data{};
    unsigned int width = 0;
    unsigned int height = 0;

    lodepng::decode(data, width, height, "Resources/Textures/" + texture_name);

    FlipYTexture(data, width, height);


    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    data.clear();
}
Engine::Textures::~Textures() {
    glDeleteTextures(1, &m_textureID);
}

void Engine::Textures::Bind(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Engine::Textures::FlipYTexture(std::vector<unsigned char>& data, unsigned int width, unsigned int height) {
    if (data.empty() || width == 0 || height == 0) {
        return;
    }

    for (unsigned int h = 0; h < height / 2; h++) {
        for (unsigned int w = 0; w < width; w++) {
            unsigned int topIndex = (h * width + w) * 4;
            unsigned int bottomIndex = ((height - 1 - h) * width + w) * 4;

            for (unsigned int c = 0; c < 4; c++) {
                unsigned char tmp = data[topIndex + c];
                data[topIndex + c] = data[bottomIndex + c];
                data[bottomIndex + c] = tmp;
            }
        }
    }
}