#include "camera.hpp"

// --- initialize a camera object at the origin ---
sariel::Camera::Camera(glm::vec3 pos, glm::vec3 face, glm::vec3 up, f32 sensitivity, f32 speed, f32 FOV)
    : camPos(pos), camFront(face), worldUp(up), sensitivity(sensitivity), speed(speed), FOV(FOV)
{
    camTarget = (camPos + camFront);
    camRight = glm::normalize(glm::cross(camFront, worldUp));

    initCamFront = camFront;
    initCamRight = camRight;

    view = glm::lookAt(camPos, camTarget, worldUp);

    computeProjection();

    camListener = listener(true, true);
    setUpControls();
}
void sariel::Camera::camUpdate()
{
    if (camListener.boundHoldKeys[GLFW_KEY_W])
        processMovement(SARIEL_CAMERA_Z, 0.1f);
    if (camListener.boundHoldKeys[GLFW_KEY_S])
        processMovement(SARIEL_CAMERA_Z, -0.1f);

    if (camListener.boundHoldKeys[GLFW_KEY_A])
        processMovement(SARIEL_CAMERA_X, -0.1f);
    if (camListener.boundHoldKeys[GLFW_KEY_D])
        processMovement(SARIEL_CAMERA_X, 0.1f);

    if (camListener.boundHoldKeys[GLFW_KEY_SPACE])
        processMovement(SARIEL_CAMERA_Y, 0.1f);
    if (camListener.boundHoldKeys[GLFW_KEY_LEFT_CONTROL])
        processMovement(SARIEL_CAMERA_Y, -0.1f);

    processMovement(SARIEL_CAMERA_PITCH, camListener.mouseOffsetY);
    processMovement(SARIEL_CAMERA_YAW, camListener.mouseOffsetX);
}

// --- process camera movement ---
void sariel::Camera::processMovement(uint axis, f32 magnitude)
{
    switch (axis)
    {
    case SARIEL_CAMERA_Z:
        camPos += speed * magnitude * glm::normalize(glm::cross(worldUp, camRight));
        camTarget = camPos + camFront;
        break;

    case SARIEL_CAMERA_X:
        camPos += speed * magnitude * camRight;
        camTarget = camPos + camFront;
        break;

    case SARIEL_CAMERA_Y:
        camPos += speed * magnitude * worldUp;
        camTarget = camPos + camFront;
        break;

    case SARIEL_CAMERA_PITCH:
        pitch += sensitivity * magnitude;
        pitch = glm::clamp(pitch, -89.9f, 89.9f);
        updateOrientation();

        break;

    case SARIEL_CAMERA_YAW:
        yaw -= sensitivity * magnitude;
        updateOrientation();

        break;

    default:
        break;
    }

    view = glm::lookAt(camPos, camTarget, worldUp);
}

void sariel::Camera::setUpControls()
{
    camListener.bindKey(GLFW_KEY_W, false);
    camListener.bindKey(GLFW_KEY_A, false);
    camListener.bindKey(GLFW_KEY_S, false);
    camListener.bindKey(GLFW_KEY_D, false);
    camListener.bindKey(GLFW_KEY_SPACE, false);
    camListener.bindKey(GLFW_KEY_LEFT_CONTROL, false);
}

sariel::listener *sariel::Camera::getListener()
{
    return &camListener;
}

void sariel::Camera::computeProjection()
{
    projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
}

glm::mat4 &sariel::Camera::getCamView()
{
    return view;
}

glm::mat4 &sariel::Camera::getProjection()
{
    computeProjection();
    return projection;
}

void sariel::Camera::onViewportResize(int width, int height, EngineKey)
{
    aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);
    computeProjection();
}

void sariel::Camera::parameterUpdate(f32 FOV, f32 speed, f32 sensitivity)
{
    this->FOV = FOV;
    this->speed = speed;
    this->sensitivity = sensitivity;
}

void sariel::Camera::updateOrientation()
{
    glm::quat pitchRotation = glm::angleAxis(glm::radians(pitch), initCamRight);
    glm::quat yawRotation = glm::angleAxis(glm::radians(yaw), worldUp);

    camFront = glm::normalize(yawRotation * (pitchRotation * initCamFront));
    camRight = glm::normalize(yawRotation * initCamRight);

    camTarget = camPos + camFront;
}