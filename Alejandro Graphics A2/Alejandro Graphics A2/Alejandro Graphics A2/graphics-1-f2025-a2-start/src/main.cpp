#include "Window.h"
#include "Shader.h"
#include "raymath.h"
#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
using namespace std;
static const int line_vertex_count = 8;
static const Vector2 line_vertex_positions[line_vertex_count]
{
    { -1.0f,  -1.0f },   // bottom-left
    {  1.0f,  -1.0f },   // bottom-right

    {  1.0f, -1.0f },   // bottom-right
    {  1.0f,  1.0f },   // top-right

    {   1.0f,  1.0f },   // top-right
    {  -1.0f,  1.0f },   // top-left

    { -1.0f,   1.0f },   // top-left
    { -1.0f,  -1.0f }    // bottom-left
};

int main()
{
    // How to form the vertices for the 2nd square:
    /*Vector2 line_vertex_positions2[8];

    line_vertex_positions2[0] = Vector2Lerp(line_vertex_positions[0], line_vertex_positions[1], 0.5f);
    line_vertex_positions2[1] = Vector2Lerp(line_vertex_positions[2], line_vertex_positions[3], 0.5f);

    line_vertex_positions2[2] = Vector2Lerp(line_vertex_positions[2], line_vertex_positions[3], 0.5f);
    line_vertex_positions2[3] = Vector2Lerp(line_vertex_positions[4], line_vertex_positions[5], 0.5f);

    line_vertex_positions2[4] = Vector2Lerp(line_vertex_positions[4], line_vertex_positions[5], 0.5f);
    line_vertex_positions2[5] = Vector2Lerp(line_vertex_positions[6], line_vertex_positions[7], 0.5f);
    
    line_vertex_positions2[6] = Vector2Lerp(line_vertex_positions[6], line_vertex_positions[7], 0.5f);
    line_vertex_positions2[7] = Vector2Lerp(line_vertex_positions[0], line_vertex_positions[1], 0.5f);*/
    // (For full marks, you need to automate this with loops or recursion for 8 iterations [meaning 8 squares])

    CreateWindow(800, 800, "Graphics 1");
    
    // Hint: The a1_triangle shaders handle vertex position AND vertex colour.
    // Vertex colour is needed in order to receive full marks on this assignment!
    GLuint a2_lines_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a2_lines.vert");
    GLuint a2_lines_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a2_lines.frag");
    GLuint a2_lines_shader = CreateProgram(a2_lines_vert, a2_lines_frag);

    // Generate 8 shrinking squares
    vector<Vector2> vertices;

	Vector2 prev[line_vertex_count];
	for (int i = 0; i < line_vertex_count; i++) prev[i] = line_vertex_positions[i];

    for (int s = 0; s < 8; s++)
    {
        for (int i = 0; i < line_vertex_count; i++)
			vertices.push_back(prev[i]);
		Vector2 next[line_vertex_count];
        for (int i = 0; i < line_vertex_count; i+= 2)
        {
			next[i] = Vector2Lerp(prev[i], prev[(i + 1) % line_vertex_count], 0.5f);
			next[i + 1] = Vector2Lerp(prev[(i + 2) % line_vertex_count],prev[(i + 3) % line_vertex_count], 0.5f);
        }
		for (int i = 0; i < line_vertex_count; i++)
			prev[i] = next[i];
    }

    GLuint vbo_line_positions;
    glGenBuffers(1, &vbo_line_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_line_positions);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector2), vertices.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertex_positions2), line_vertex_positions2, GL_STATIC_DRAW);
    // Comment/uncomment to see 1st square vs 2nd square.
    // Your job is to automate the generation of squares so all 8 squares are rendered at once!

    GLuint vao_line;
    glGenVertexArrays(1, &vao_line);
    glBindVertexArray(vao_line);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_line_positions);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

    glBindVertexArray(GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    GLint u_color = glGetUniformLocation(a2_lines_shader, "u_color");
    GLint u_mvp = glGetUniformLocation(a2_lines_shader, "u_mvp");


    Vector3 colors[8] = {
        { 1.0f, 0.9f, 0.2f },   
        { 1.0f, 0.5f, 0.0f },   
        { 1.0f, 0.1f, 0.6f },   
        { 0.9f, 0.0f, 0.0f },   
        { 0.2f, 0.7f, 1.0f },   
        { 0.0f, 1.0f, 1.8f },   
        { 0.3f, 1.0f, 0.3f },   
        { 1.0f, 1.0f, 1.0f }    
	};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) SetWindowShouldClose(true);
        // Note: change MatrixOrtho's left/right/bottom/top values to modify the extents of your screen!
        Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0, 0,10 }, { 0,0,0 }, Vector3UnitY);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(a2_lines_shader);
        glBindVertexArray(vao_line);
        glLineWidth(5.0f);

        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));

        for (int s = 0; s < 8; s++)
        {
			Vector3 c = colors[s];
            glUniform3f(u_color, c.x, c.y, c.z);
			glDrawArrays(GL_LINES, s * line_vertex_count, line_vertex_count);
        }

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }

    glDeleteVertexArrays(1, &vao_line);
    glDeleteBuffers(1, &vbo_line_positions);
    glDeleteProgram(a2_lines_shader);
    glDeleteShader(a2_lines_frag);
    glDeleteShader(a2_lines_vert);

    DestroyWindow();
    return 0;
}
