#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "global.hpp"
#include "input.hpp"
#include "engine_key.hpp"

#define SARIEL_CAMERA_X 1
#define SARIEL_CAMERA_Y 2
#define SARIEL_CAMERA_Z 3
#define SARIEL_CAMERA_PITCH 4
#define SARIEL_CAMERA_YAW 5

namespace sariel
{
    class Camera
    {
    public:
        Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
               glm::vec3 face = glm::vec3(0.0f, 0.0f, -1.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               f32 sensitivity = 0.2f, f32 speed = 1.0f, f32 FOV = 90.0f);

        void setUpControls();
        listener *getListener();
        void camUpdate();

        glm::mat4 &getCamView();
        glm::mat4 &getProjection();

        void onViewportResize(int width, int height, EngineKey);
        void parameterUpdate(f32 FOV, f32 speed, f32 sensitivity);

    private:
        f32 aspectRatio = 16.0f / 9.0f;
        f32 FOV;
        f32 speed;
        f32 sensitivity;

        glm::vec3 camPos;
        glm::vec3 camTarget;

        glm::vec3 initCamFront;
        glm::vec3 initCamRight;
        f32 yaw = 0.0f;
        f32 pitch = 0.0f;

        glm::vec3 camFront;
        glm::vec3 camRight;
        glm::vec3 worldUp;

        glm::mat4 view;
        glm::mat4 projection;

        void updateOrientation();
        void computeProjection();
        void processMovement(uint axis, f32 magnitude);

        listener camListener;
    };

}