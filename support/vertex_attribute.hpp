#ifndef VERTEX_ATTRIBUTE_HPP
#define VERTEX_ATTRIBUTE_HPP

/// \file vertex_attribute.hpp
/// \brief Structures and functions for managing OpenGL vertex attributes and VAO binding.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

/// \struct AttributePointer
/// \brief Describes a vertex attribute pointer for OpenGL vertex buffer objects.
struct AttributePointer {
    int values_per_vertex;  ///< Number of values per vertex (e.g., 3 for vec3).
    int type_data;  ///< OpenGL data type (e.g., GL_FLOAT).
    bool normalized;  ///< Whether the data should be normalized.
    int stride_bytes;  ///< Stride in bytes between consecutive vertices.
    int offset_bytes;  ///< Offset in bytes to the first component.
};

/// \struct VAOStruct
/// \brief Structure holding VAO ID and its associated attribute pointers.
struct VAOStruct {
    unsigned int id;  ///< OpenGL VAO ID.
    std::vector<AttributePointer> attributes;  ///< List of attribute pointers for the VAO.
};

/// \brief Builds an AttributePointer struct with given parameters.
/// \param values_per_vertex Number of components per vertex.
/// \param type_data OpenGL data type (e.g., GL_FLOAT).
/// \param normalized Whether the data should be normalized.
/// \param stride_bytes Stride in bytes between consecutive vertices.
/// \param offset_bytes Offset in bytes to the first component.
/// \return The constructed AttributePointer.
AttributePointer BuildAttribute(int values_per_vertex, int type_data, bool normalized, int stride_bytes, int offset_bytes);

/// \brief Binds the VAO and sets up vertex attributes.
/// \param vao The VAOStruct containing VAO information.
/// \param vboID The VBO ID to bind.
/// \param buffer_type Buffer type (default GL_ARRAY_BUFFER).
void BindVAO(VAOStruct vao, unsigned int vboID, int buffer_type = GL_ARRAY_BUFFER);

#endif // VERTEX_ATTRIBUTE_HPP