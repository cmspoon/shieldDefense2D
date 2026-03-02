#ifndef BUILD_SHAPES_HPP
#define BUILD_SHAPES_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include "basic_shape.hpp"

//Loads a texture into memory given a string file path and returns
// the ID of the texture.
unsigned int GetTexture (std::string file_path);

//Creates and returns a BasicShape textured rectangle given a VAO, a bottom left point, 
// a width, and a height.  Optionally a size variable to adjust how many times 
// the texture repeates.
BasicShape GetTexturedRectangle (VAOStruct vao, glm::vec3 bottom_left, float width, float heigth, float size = 1.0f);

//Creates and returns a BasicShape triangle given a VAO and three points.
BasicShape GetTriangle (VAOStruct vao,glm::vec3 a, glm::vec3 b, glm::vec3 c);
//Creates and returns a BasicShape circle given a VAO, a float radius, the number of points to render, and a location (centered)
BasicShape GetCircle (VAOStruct vao, float radius=1.0, int points= 40, glm::vec3 location = glm::vec3(0.0));

//Creates and returns a BasicShape circle given a VAO, a float radius, the number of points to render, and a location (centered)
BasicShape GetTexturedCircle (VAOStruct vao, float radius=1.0, int points= 40, glm::vec3 location = glm::vec3(0.0), float size = 1.0f);


BasicShape GetHull (VAOStruct vao, float width, float height,
                         glm::vec3 bottom_left=glm::vec3(0.0f));

BasicShape GetTexturedHull (VAOStruct vao, float width, float height,
                         glm::vec3 bottom_left=glm::vec3(0.0f), float size = 1.0f);


#endif //BUILD_SHAPES_HPP