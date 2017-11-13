// ENGINE
#include "../GameEngine/engine.hpp"
#include <stdlib.h>
#include <iostream>

#define WINDOW_SIZE 630
#define TILE_WIDTH (2.0f / WINDOW_SIZE * 70.0f)
#define TILE_HEIGHT TILE_WIDTH

class Sudoku
{
private:
    GLuint _VBO, _VAO;

    GLfloat _vertices[3] = {
        // Positions
        0.0f, 0.0f, 0.0f
    };
    void BufferVertexData()
    {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);

        // bind the VAO first, then bind and set vertex buffer(s)
        // and attribute pointer(s)
        glBindVertexArray(_VAO);

        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // good practice to unbind the VAO to prevent strange bugs
        // do NOT unbind the EBO, keep it bound to this VAO
        glBindVertexArray(0);
    }
    typedef struct {
        unsigned int cnt;
        bool vals[9];
    } _field_t;

    _field_t* _board;
    unsigned int _selection;
public:
    Sudoku()
    {
        _board = new _field_t[81];
        _selection = 0;

        for(unsigned int i = 0; i < 81; i++)
        {
            _board[i].cnt = 9;
            for(unsigned int j = 0; j < 9; j++)
            {
                _board[i].vals[j] = true;
            }
        }
        BufferVertexData();
    }
    ~Sudoku()
    {
        delete _board;
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
    }

    void KeyInput(int key)
    {
        switch(key) {
        case GLFW_KEY_1:
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
        case GLFW_KEY_6:
        case GLFW_KEY_7:
        case GLFW_KEY_8:
        case GLFW_KEY_9:
            break;
        case GLFW_KEY_UP:
            _selection -= (_selection >= 9) ? 9 : 0;
            break;
        case GLFW_KEY_DOWN:
            _selection += (_selection < 72) ? 9 : 0;
            break;
        case GLFW_KEY_LEFT:
            _selection -= (_selection % 9 > 0) ? 1 : 0;
            break;
        case GLFW_KEY_RIGHT:
            _selection += (_selection % 9 < 8) ? 1 : 0;
            break;
        default:
            break;
        }
    }
    void DrawBoard(Shaders::ShaderWrapper* shd, unsigned int size, Textures::Texture2D** tex)
    {

        glBindVertexArray(_VAO);
        for(unsigned int i = 0; i < 81; i++)
        {
            shd->SetUniform("position", glm::vec2(2.0f / 9.0f * (float)(i % 9) - 1.0f,
                                                    -2.0f / 9.0f * (float)(i / 9) + 1.0f));
            shd->SetUniform("width", TILE_WIDTH);
            shd->SetUniform("height", TILE_HEIGHT);
            shd->SetUniform("selection", (i == _selection));

            tex[1]->Bind();
            shd->SetUniformTexture("tex", GL_TEXTURE0);

            glDrawArrays(GL_POINTS, 0, 1);
        }
        glBindVertexArray(0);
    }
};

Windows::WindowedWindow* win;
Sudoku* sud;
Shaders::ShaderWrapper* shd;
Textures::Texture2D* tex_tiles[10];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS) { sud->KeyInput(key); }
}
int main()
{
    // WINDOW
    win = new Windows::WindowedWindow("Sudoku", 630, Windows::ASPECT_RATIO_1_1);
    win->SetKeyCallback(key_callback);
    glfwSetInputMode(win->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // SHADER
    shd = new Shaders::ShaderWrapper("shaders|tile", Shaders::SHADERS_VGF);
    shd->Activate();

    // TEXTURES
    unsigned long tex_options = TEX_GENERATE_MIPMAP | TEX_MIXED_FILTER;
    tex_tiles[0] = new Textures::Texture2D("images|number|tile_blank.png", tex_options);
    tex_tiles[1] = new Textures::Texture2D("images|number|tile_1.png", tex_options);
    tex_tiles[2] = new Textures::Texture2D("images|number|tile_2.png", tex_options);
    tex_tiles[3] = new Textures::Texture2D("images|number|tile_3.png", tex_options);
    tex_tiles[4] = new Textures::Texture2D("images|number|tile_4.png", tex_options);
    tex_tiles[5] = new Textures::Texture2D("images|number|tile_5.png", tex_options);
    tex_tiles[6] = new Textures::Texture2D("images|number|tile_6.png", tex_options);
    tex_tiles[7] = new Textures::Texture2D("images|number|tile_7.png", tex_options);
    tex_tiles[8] = new Textures::Texture2D("images|number|tile_8.png", tex_options);
    tex_tiles[9] = new Textures::Texture2D("images|number|tile_9.png", tex_options);

    // SUDOKU
    sud = new Sudoku();

    // GAME LOOP
    while(!win->ShouldClose())
    {
        win->WaitEvents();

        if(glfwGetKey(win->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            win->CloseWindow();
            continue;
        }

        win->ClearScreen();
        sud->DrawBoard(shd, 10, tex_tiles);
        win->SwapBuffers();
    }

    delete tex_tiles[0];
    delete tex_tiles[1];
    delete tex_tiles[2];
    delete tex_tiles[3];
    delete tex_tiles[4];
    delete tex_tiles[5];
    delete tex_tiles[6];
    delete tex_tiles[7];
    delete tex_tiles[8];
    delete tex_tiles[9];
    return 0;
}