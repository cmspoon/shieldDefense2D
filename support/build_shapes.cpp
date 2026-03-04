#include "build_shapes.hpp"
#include "stb_image_implementation.hpp"

unsigned int GetTexture (std::string file_path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 4) {
            format = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}


BasicShape GetTexturedRectangle (VAOStruct vao, glm::vec3 bottom_left, 
                                 float width, float height, float size)
{
    glm::vec3 bl = bottom_left;
    float rect_vertices[] {
        bl.x, bl.y, bl.z, 0.0, 0.0,  //bottom left
        bl.x+width, bl.y, bl.z, size, 0.0,  //bottom right
        bl.x+width, bl.y+height, bl.z, size, size, //top right
        bl.x, bl.y+height, bl.z, 0.0, size //top left
    };

    BasicShape new_shape;
    new_shape.Initialize(vao,rect_vertices,sizeof(rect_vertices),4,GL_TRIANGLE_FAN);

    unsigned int indices[] = {0,1,2,3};
    //Set the EBO for the shape here.
    new_shape.InitializeEBO(indices,sizeof(indices),4,GL_LINE_LOOP);
    return new_shape;



}

BasicShape GetTriangle (VAOStruct vao, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    float tri_vert[] {
        a.x, a.y, a.z,//bottom left vertex
        b.x, b.y, b.z,  //bottom right vertex
        c.x, c.y, c.z  //top vertex
    };
    
    BasicShape new_shape;
    new_shape.Initialize(vao,tri_vert,sizeof(tri_vert),3,GL_TRIANGLES);

    unsigned int indices[] = {0,1,2};
    //Set the EBO for the shape here.
    new_shape.InitializeEBO(indices,sizeof(indices),3,GL_LINE_LOOP);
    return new_shape;
}

BasicShape GetCircle (VAOStruct vao, float radius, int points, glm::vec3 location) {
    float circ_dat[(points)*3]{};
    float angle = 0.0f;
    for(int i = 0; i<points; i++) {
        circ_dat[i*3] = location.x + radius*cos(glm::radians(angle));
        circ_dat[i*3+1] = location.y + radius*sin(glm::radians(angle));
        circ_dat[i*3+2] = 0.0f;
        angle+=360.0f/points;
    } 

    BasicShape new_shape;
    new_shape.Initialize(vao,circ_dat,(points)*3*sizeof(float),points,GL_TRIANGLE_FAN);

    //for use with the EBO
    unsigned int indices[points]{};
    for(int i = 0; i<points; i++)
    {
        indices[i] = i;
    }
    //set the ebo for the shape here
    new_shape.InitializeEBO(indices,sizeof(indices),points,GL_LINE_LOOP);
    return new_shape;
}

BasicShape GetTexturedCircle (VAOStruct vao, float radius, int points, glm::vec3 location, float size) {
    float circ_dat[(points)*5]{};
    float angle = 0.0f;
    for(int i = 0; i<points; i++) {
        circ_dat[i*5] = location.x + radius*cos(glm::radians(angle));
        circ_dat[i*5+1] = location.y + radius*sin(glm::radians(angle));
        circ_dat[i*5+2] = 0.0f;
        circ_dat[i*5+3] = 0.5f*size + 0.5f*size*cos(glm::radians(angle));
        circ_dat[i*5+4] = 0.5f*size + 0.5f*size*sin(glm::radians(angle));
        angle+=360.0f/points;
    } 

    BasicShape new_shape;
    new_shape.Initialize(vao,circ_dat,(points)*5*sizeof(float),points,GL_TRIANGLE_FAN);

    //for use with the EBO
    unsigned int indices[points]{};
    for(int i = 0; i<points; i++)
    {
        indices[i] = i;
    }
    //set the ebo for the shape here
    new_shape.InitializeEBO(indices,sizeof(indices),points,GL_LINE_LOOP);
    return new_shape;
}
