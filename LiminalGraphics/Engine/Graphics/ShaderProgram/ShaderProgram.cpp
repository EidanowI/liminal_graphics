#include "ShaderProgram.h"



Engine::ShaderProgram::ShaderProgram() {
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNor;\n"
        "layout (location = 2) in vec2 aUV;\n"
        "layout (location = 3) in vec4 aCol;\n"
        "out vec4 color;\n"
        "out vec2 uv;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   color = aCol;\n"
        "   uv = aUV;\n"
        "}";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

#ifdef DEBUG
    {
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
#endif

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 color;\n"
        "in vec2 uv;\n"
        "uniform sampler2D tex1;\n"
        "uniform sampler2D tex2;\n"
        "uniform sampler2D tex3;\n"
        "void main(){\n"
        "FragColor = mix(texture(tex1, uv), texture(tex2, uv), texture(tex3, uv).r);}";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

#ifdef DEBUG
    {
        int  success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
#endif


    m_shaderProgramID = glCreateProgram();

    glAttachShader(m_shaderProgramID, vertexShader);
    glAttachShader(m_shaderProgramID, fragmentShader);
    glLinkProgram(m_shaderProgramID);

#ifdef DEBUG
    {
        int  success;
        char infoLog[512];
        glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(m_shaderProgramID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_ERROR\n" << infoLog << std::endl;
        }
    }
#endif

    glUseProgram(m_shaderProgramID);

    glUniform1i(glGetUniformLocation(m_shaderProgramID, "tex1"), 0);
    glUniform1i(glGetUniformLocation(m_shaderProgramID, "tex2"), 1);
    glUniform1i(glGetUniformLocation(m_shaderProgramID, "tex3"), 2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
Engine::ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_shaderProgramID);
}


void Engine::ShaderProgram::Bind() {
    glUseProgram(m_shaderProgramID);
}