#include "basic_shape.hpp"

// Implementation of the BasicShape class for managing OpenGL geometric shapes.

// Default constructor. Initializes all member variables to default values.
BasicShape::BasicShape() {
    this->vao.id = 0;
    this->vboID = 0;
    this->number_vertices = 0;
    this->primitive = GL_TRIANGLES;
}
// Initializes the shape with vertex data, setting up VAO and VBO.
void BasicShape::Initialize(VAOStruct vao,float* vertex_data,
                int array_size_bytes, int num_vertices,
                GLuint primitive_type) {
    this->vao = vao;
    this->number_vertices = num_vertices;
    this->primitive = primitive_type;
    glGenBuffers(1, &(this->vboID));
    BindVAO(this->vao,this->vboID);
    glBufferData(GL_ARRAY_BUFFER,array_size_bytes, vertex_data, GL_STATIC_DRAW);
}

void BasicShape::InitializeEBO (unsigned int* indices, int number_bytes,
                            int number_indices, int primitive)
{
    //handle outline EBO
    BindVAO(this->vao, this->vboID);
    glGenBuffers(1, &(this->eboID));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_bytes,
                indices,GL_STATIC_DRAW);
    this->number_ebo_indices = number_indices;
}

// Renders the shape using the specified shader program.
void BasicShape::Draw(int shader_program) {
    glUseProgram(shader_program);
    this->Draw();
}

// Renders the shape using the specified shader program.
void BasicShape::Draw() {
    BindVAO(this->vao,this->vboID);
    glDrawArrays(this->primitive, 0, this->number_vertices);
    glBindVertexArray(0);
}

void BasicShape::DrawEBO (int shader_program, float line_width) {
    glUseProgram(shader_program);
    this->DrawEBO(line_width);
}

void BasicShape::DrawEBO (float line_width) {
    BindVAO(this->vao,this->vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->eboID);
    glLineWidth(line_width);
    glDrawElements(this->ebo_primitive,this->number_ebo_indices,
    GL_UNSIGNED_INT,0);
    glLineWidth(1.0f);
    glBindVertexArray(0);
}

// Cleans up OpenGL resources associated with the shape.
void BasicShape::DeallocateShape() {
    glDeleteBuffers(1, &(this->vboID));
}
