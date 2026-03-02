#ifndef ENVIRONMENT_SETUP_HPP
#define ENVIRONMENT_SETUP_HPP

/// \file environment_setup.hpp
/// \brief Functions for initializing and managing the OpenGL environment using GLFW.

#include "basic_shape.hpp"
#include <iostream>
#include <string>

/// \brief GLFW callback for handling window resize events.
/// \param window The GLFW window that was resized.
/// \param width New width of the window.
/// \param height New height of the window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


/// \brief Initializes the GLFW environment and creates a window.
/// \param width Initial width of the window.
/// \param height Initial height of the window.
/// \param title Title of the window.
/// \return Pointer to the created GLFW window.
GLFWwindow* InitializeEnvironment(int width, int height, std::string title);



#endif // ENVIRONMENT_SETUP_HPP