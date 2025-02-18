#include "RefEngWindow_GLFW.h"

#include "RefEngine.h"

#include "utils/pow2assert.h"

#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;
using namespace reng;

static void errorCallback(int errorCode, const char* errorDesc)
{
    cerr << "GLFW Error(" << errorCode << "): " << errorDesc << endl;
}

void keyCallback(GLFWwindow* m_pWindow, int key, int /*scanCode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_pWindow, 1);
    }
}


RefEngWindow::RefEngWindow() :
    m_isValid(false),
    m_pRefEngine(std::make_unique<RefEngine>())
{
}

RefEngWindow::~RefEngWindow()
{
    if (m_isValid) Destroy();
}

bool RefEngWindow::Init()
{
    POW2_ASSERT(!m_isValid);
    std::cout << "GLFW: " << glfwGetVersionString() << std::endl;

    m_isValid = glfwInit() == GL_TRUE;
    if (!m_isValid) return false;

    // Required to get MacOS out of compatiblity mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_pWindow = glfwCreateWindow(1024, 780, "RefEng", nullptr, nullptr);
    if (m_pWindow == nullptr) return false;

    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(m_pWindow, keyCallback);
    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1);

    m_pRefEngine->Init(  );

    m_isValid = true;
    return true;
}

void RefEngWindow::Destroy()
{
    POW2_ASSERT(m_isValid);

	m_pRefEngine->DeInit();

    if (m_pWindow != nullptr) {
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
    if (m_isValid) { glfwTerminate(); }


    m_isValid = false;
}

void RefEngWindow::Run()
{
    POW2_ASSERT_MSG(m_isValid, "Call Init() first and check return code.");

    double currentTime = glfwGetTime();
    double deltaTime = 1 / 60.f;
    double elapsedTime = 0;
    double accumulator = 0.0;
    bool isRunning = true;

    while (isRunning) {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25) frameTime = 0.25;
        currentTime = newTime;
        accumulator += frameTime;
        while (accumulator >= deltaTime && isRunning) {
            glfwPollEvents();
            if (glfwWindowShouldClose(m_pWindow)) return;
            isRunning = m_pRefEngine->Update(deltaTime);
            elapsedTime += deltaTime;
            accumulator -= deltaTime;
        }
        m_pRefEngine->Draw();


        glfwSwapBuffers(m_pWindow);
    }
}

glm::vec2 RefEngWindow::GetSize() const
{
	int width, height;
	glfwGetWindowSize(m_pWindow, &width, &height);
	return glm::vec2(width, height);
}
