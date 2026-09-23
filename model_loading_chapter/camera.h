#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


struct Camera {
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;

  glm::quat Orientation;
  float phi;
  float theta;
  
  float MovementSpeed;
  float Sensitivity;
  float Zoom;
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : 
      Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), Sensitivity(SENSITIVITY), 
      Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        updateCameraVectors();
      }
  Camera(float posX, float posY, float posZ, float upX, float upY, 
      float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
      MovementSpeed(SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        updateCameraVectors();
      }

  glm::mat4 get_view_matrix() {
      glm::mat4 rotation = glm::mat4_cast(glm::conjugate(Orientation));
      glm::mat4 translation = glm::mat4(1.0f);
      translation[3][0] = -Position.x;
      translation[3][1] = -Position.y;
      translation[3][2] = -Position.z;

      return rotation * translation;
  }

  void process_keyboard(Camera_Movement direction, float deltaTime) {
      float velocity = MovementSpeed * deltaTime;
      if (direction == RIGHT) {
        Position += Right * velocity;
      }
      if (direction == LEFT) {
        Position -= Right * velocity;
      }
      if (direction == FORWARD) {
        Position += Front * velocity;
      }
      if (direction == BACKWARD) {
        Position -= Front * velocity;
      }
  }

  void process_mouse_move(float xOffset, float yOffset, GLboolean constrainPitch = true) {
    xOffset *= Sensitivity * glm::radians(1.0f);
    yOffset *= Sensitivity * glm::radians(1.0f);
    
    glm::quat pitchQuat = glm::angleAxis(yOffset, glm::vec3(1.0f, 0.0, 0.0));
    glm::quat yawQuat = glm::angleAxis(-xOffset, WorldUp);

    Orientation = yawQuat * Orientation * pitchQuat;
    Orientation = normalize(Orientation);
    updateCameraVectors();
  }

  void process_mouse_scroll(float yOffset) {
    Zoom -= (float)yOffset;
    if (Zoom < 1.0f) {
      Zoom = 1.0f;
    }
    if (Zoom > 45.0f) {
      Zoom = 45.0f;
    }
  }
  private:
  void updateCameraVectors() {
    Front = Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
    Right = Orientation * glm::vec3(1.0f, 0.0f, 0.0f);
    Up = Orientation * glm::vec3(0.0f, 1.0f, 0.0f);
  }
};

#endif
