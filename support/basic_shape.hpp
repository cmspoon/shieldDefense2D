#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP

#include "vertex_attribute.hpp"

/// \brief BasicShape provides a base class for OpenGL geometric shapes, managing VAO and VBO.

class BasicShape {
    protected:
        VAOStruct vao;  ///< Vertex Array Object structure.
        unsigned int vboID = 0;  ///< OpenGL Vertex Buffer Object ID.
        int number_vertices = 0;  ///< Number of vertices in the shape.
        GLuint primitive = GL_TRIANGLES;  ///< OpenGL primitive rendering type.
        unsigned int eboID = 0;
        int number_ebo_indices = 0; 
        bool delete_ebo = false;
        int ebo_primitive = GL_LINE_LOOP;
    public:
        /// Default constructor.
        BasicShape();
        /// Initializes the shape with vertex data and rendering parameters.
        /// \param vao The Vertex Array Object structure.
        /// \param vertex_data Pointer to the vertex data array.
        /// \param array_size_bytes Size of the vertex data in bytes.
        /// \param num_vertices Number of vertices in the shape.
        /// \param primitive_type OpenGL primitive type (default GL_TRIANGLES).
        void Initialize(VAOStruct vao, float* vertex_data,
                        int array_size_bytes, int num_vertices,
                        GLuint primitive_type = GL_TRIANGLES);
        /// Renders the shape using the specified shader program.
        /// \param shader_program The ID of the shader program to use.
        void Draw(int shader_program);

        void InitializeEBO (unsigned int* indices, int number_bytes,
                            int number_indices, int primitive);

        /// Renders the shape using the specified shader program.
        void Draw();

        //Draws an EBO
        void DrawEBO (int shader_program, float line_width = 1.0f);

        //Draws an EBO without a shader program)
        void DrawEBO(float line_width = 1.0f);

        /// Cleans up OpenGL resources associated with the shape.
        void DeallocateShape();

};

#endif // BASIC_SHAPE_HPP