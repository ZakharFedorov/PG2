#include <iostream>
#include <stack>
#include <random>
#include <chrono>
#include <windows.h>



// OpenCV 
// does not depend on GL
#include <opencv2\opencv.hpp>

// OpenGL Extension Wrangler: allow all multiplatform GL functions
#include <GL/glew.h> 
// WGLEW = Windows GL Extension Wrangler (change for different platform) 
// platform specific functions (in this case Windows)
#include <GL/wglew.h> 

// GLFW toolkit
// Uses GL calls to open GL context, i.e. GLEW must be first.
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//... other callback methods
void error_callback(int error, const char* description);
void fbsize_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

class App {
    GLFWwindow* window = NULL;
public:
    //no need to *window in callback methods, it belongs to class... 
  

    void scroll_callback(double xoffset, double yoffset) {
        std::cout << "mouse wheel\n";
    }
    void key_callback(int key, int scancode, int action, int mods);

    App();

    void init(void);
    int run(void);

    ~App();
private:
};

void App::key_callback(int key, int scancode, int action, int mods){
std::cout << "Key pressed: " << key << '\n';

if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
{
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_V:
        break;
    default:
        break;
    }
}

    }



App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "Constructed...\n";

}


void App::init()
{
    try {
        glfwSetErrorCallback(error_callback);
        // init glfw
        // https://www.glfw.org/documentation.html
        glfwInit();

        // open window (GL canvas) with no special properties
        // https://www.glfw.org/docs/latest/quick.html#quick_create_window
        window = glfwCreateWindow(800, 600, "OpenGL context", NULL, NULL);
        glfwMakeContextCurrent(window);

        // init glew
        // http://glew.sourceforge.net/basic.html
        glewInit();
        wglewInit();

        if (glfwExtensionSupported("ARB_debug_output"))
        {
            glDebugMessageCallback(MessageCallback, 0);
            glEnable(GL_DEBUG_OUTPUT);

            //default is asynchronous debug output, use this to simulate glGetError() functionality
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

            std::cout << "GL_DEBUG enabled." << std::endl;
        }
        else
            std::cout << "GL_DEBUG NOT SUPPORTED!" << std::endl;
        

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::cout << "ver " << major << '.' << minor << '\n';

        const char* vendor = (const char*)glGetString(GL_VENDOR);
        std::cout << "Vendor is: " << vendor << '\n';
        const char* renderer = (const char*)glGetString(GL_RENDERER);
        std::cout << "Renderer is: " << renderer << '\n';
        const char* version = (const char*)glGetString(GL_VERSION);
        std::cout << "VERSION is: " << version << '\n';
        const char* lan_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        std::cout << "LANGUAGE_VERSION is: " << lan_version << '\n';

        GLint profile;
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
        if (const auto errorCode = glGetError()) {
            std::cout << "Pending GL error while obtaining profile: " << errorCode << std::endl;
            return;
        }
        if (profile && GL_CONTEXT_CORE_PROFILE_BIT) {
            std::cout << "Core profile" << std::endl;
        }
        else {
            std::cout << "Compatibility profile" << std::endl;
        }


    }
    catch (std::exception const& e) {
        std::cerr << "Init failed : " << e.what() << std::endl;
        throw;
    }
    std::cout << "Initialized...\n";
}


int App::run(void)
{
    try {

        while (!glfwWindowShouldClose(window))
        {
            auto start = std::chrono::steady_clock::now();

            // Creating variables for FPS calculation
            auto startTime = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point lastTime = startTime;
            int frameCount = 0;

            while (true) {
                // If the "S" key is pressed, everything stops
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    break;
                }

                // Clear OpenGL canvas, both color buffer and Z-buffer
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Swap front and back buffers
                glfwSwapBuffers(window);

                // Poll for and process events
                glfwPollEvents();

                // FPS calculation
                frameCount++;
                std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsedTime = currentTime - lastTime;
                if (elapsedTime.count() >= 1.0) {
                    double fps = static_cast<double>(frameCount) / elapsedTime.count();
                    std::cout << "FPS: " << fps << std::endl;
                    frameCount = 0;
                    lastTime = currentTime;
                }
            }

            auto end = std::chrono::steady_clock::now();

            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "elapsed time: " << elapsed_seconds.count() << "sec" << std::endl;
        }
    }
    catch (std::exception const& e) {
        std::cerr << "App failed : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Finished OK...\n";
    return EXIT_SUCCESS;
}

App::~App()
{
    // clean-up
    cv::destroyAllWindows();
    std::cout << "Bye...\n";
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

// define our application
App app;

int main()
{
    app.init();
    return app.run();
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    auto const src_str = [source]() {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
    }();

    auto const type_str = [type]() {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
    }();

    std::cout << "[GL CALLBACK]: " <<
        "source = " << src_str <<
        ", type = " << type_str <<
        ", severity = " << severity_str <<
        ", ID = '" << id << '\'' <<
        ", message = '" << message << '\'' << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    app.scroll_callback(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    app.key_callback(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    //... call app 
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //...
}

void fbsize_callback(GLFWwindow* window, int width, int height)
{
};

void error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << '\n';
}