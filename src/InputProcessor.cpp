#include "InputProcessor.hpp"

InputProcessor::InputProcessor(GLFWwindow *win, Camera *cam, Car *player)
    : window(win),
      camera(cam),
      player(player),
      firstMouse(true),
      isReleased(false),
      isWireframe(false)
{
    glfwGetWindowSize(window, &lastX, &lastY);
    lastX = lastX / 2;
    lastY = lastY / 2;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
InputProcessor::~InputProcessor() {}

void InputProcessor::processMouse()
{
    GLdouble xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (abs(xpos - lastX) < 2.f && abs(ypos - lastY) < 2.f)
    {
        return;
    }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->setYaw(camera->getYaw() + xoffset);
    camera->setPitch(camera->getPitch() + yoffset);
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (camera->getPitch() > 89.0f)
        camera->setPitch(89.0f);
    if (camera->getPitch() < -89.0f)
        camera->setPitch(-89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
    front.y = sin(glm::radians(camera->getPitch()));
    front.z = sin(glm::radians(camera->getYaw())) * cos(glm::radians(camera->getPitch()));
    front = glm::normalize(front);

    camera->setFront(front);
}

void InputProcessor::processScroll()
{
}

void InputProcessor::processInput()
{
    buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
    axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

    processMouse();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 1.f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->setPosition(camera->getPosition() + cameraSpeed * camera->getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->setPosition(camera->getPosition() - cameraSpeed * camera->getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->setPosition(camera->getPosition() - glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->setPosition(camera->getPosition() + glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        switch (camera->getMode())
        {
        case CameraModes::FIRST_PERSON:
            camera->setMode(CameraModes::THIRD_PERSON);
            break;
        case CameraModes::THIRD_PERSON:
            camera->setMode(CameraModes::FIRST_PERSON);
        }
    }
    if (isReleased)
    {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            if (!isWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw only lines no fill
                isWireframe = true;
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Draw only lines no fill
                isWireframe = false;
            }
            isReleased = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
    {
        isReleased = true;
    }

    if (axes != nullptr)
    {
        if ((glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS) || (axes[5] > 0 && axes[5] == GLFW_PRESS))
        {
            player->accelerate();
        }
        if ((glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS) || (axes[0] < -0.2))
        {
            player->turnLeft();
        }
        if ((glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS) || (axes[0] > 0.2))
        {
            player->turnRight();
        }
        if ((glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS) || (axes[4] > 0 && axes[4] == GLFW_PRESS))
        {
            if (player->getCar()->getCurrentSpeedKmHour() > 1.f)
            {
                player->brake();
            }
            else
            {
                player->reverse();
            }
        }
        if (glfwGetKey(g_window, GLFW_KEY_K) == GLFW_PRESS || buttons[13] == GLFW_PRESS)
        {
            player->fire();
        }
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
        {
            isReleased = true;
        }

        if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            player->accelerate();
        }
        if (glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            player->turnLeft();
        }
        if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            player->turnRight();
        }
        if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (player->getCar()->getCurrentSpeedKmHour() > 1.f)
            {
                player->brake();
            }
            else
            {
                player->reverse();
            }
        }
        if (glfwGetKey(g_window, GLFW_KEY_K) == GLFW_PRESS)
        {
            player->fire();
        }
    }

    if (glfwGetKey(g_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        player->brake();
    }

    if (glfwGetKey(g_window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera->zoomIn();
    }
    if (glfwGetKey(g_window, GLFW_KEY_J) == GLFW_PRESS)
    {
        camera->zoomOut();
    }
}