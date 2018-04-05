#ifndef DEFINE_H
#define DEFINE_H
#include <glad\glad.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float MOUSE_SENSITIVITY = 0.05f;

const glm::vec3 X_NORM_VECTOR = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Y_NORM_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Z_NORM_VECTOR = glm::vec3(0.0f, 0.0f, 1.0f);

const float SQUARED_DEGREE = 90.0f;
#endif