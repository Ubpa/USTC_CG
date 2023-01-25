#pragma once

#include <UGM/UGM>

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
namespace Ubpa {
    class Camera
    {
    public:
        // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
        enum class Movement {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN,
        };

        // Default camera values
        static constexpr float YAW = -90.0f;
        static constexpr float PITCH = 0.0f;
        static constexpr float SPEED = 2.5f;
        static constexpr float SENSITIVITY = 0.1f;
        static constexpr float ZOOM = 45.0f;

        // Camera Attributes
        pointf3 Position;
        vecf3 Front;
        vecf3 Up;
        vecf3 Right;
        vecf3 WorldUp;
        // Euler Angles
        float Yaw;
        float Pitch;
        // Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // Constructor with vectors
        Camera(pointf3 position = pointf3(0.0f, 0.0f, 0.0f), vecf3 up = vecf3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        // Constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
        transformf GetViewMatrix() const;

        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Movement direction, float deltaTime);

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset);

    private:
        // Calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();
    };
}
