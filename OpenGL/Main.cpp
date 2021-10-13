#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <assimp/Importer.hpp>
#include "Model.h"
#include "Scene.h"
#include <iomanip>
#include "Cubemap.h"
#include "Framebuffer.h"
#include "Instrumentation.h"



int windowHeight = 600;
int windowWidth = 1024;
float aspect = (float)windowWidth / (float)windowHeight;
float fov = glm::radians(80.0f);
Camera camera(fov,aspect,{ 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f },0.0f,0.0f);

bool firstMouse = true;
double lastX = 400.0;
double lastY = 300.0;
double pitch = 0.0;
double yaw = 0.0;
float x = 0.0f;
float z = 0.0f;
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::unique_ptr<Framebuffer> rendertarget = nullptr; // GLOBAL RESOURCE

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    windowHeight = height;
    windowWidth = width;
    camera.aspect = (float)windowWidth / (float)windowHeight;
    glViewport(0, 0, width, height);
    glBindTexture(GL_TEXTURE_2D, rendertarget->getColorAttachement());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindRenderbuffer(GL_RENDERBUFFER, rendertarget->getDepthStencilAttachement());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void processInput(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD,dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, dt);
    //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    //    camera.MovementSpeed = 20.0f;
    //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    //    camera.MovementSpeed = 250.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        x += 5.0f * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        x -= 5.0f * dt;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        z += 5.0f * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        z -= 5.0f * dt;
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);

    rendertarget = std::make_unique<Framebuffer>();
    rendertarget->init(windowWidth, windowHeight);

    Shader screenShader("vertexScreen.glsl", "fragmentScreen.glsl");
    screenShader.use();
    screenShader.setUniformInt("screenTexture", 0);
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(float) * 4, (void*)(sizeof(float)*2));
    glEnableVertexAttribArray(1);

    Instrumentor::Get().BeginSession("ModelLoading");
    Model sponza("sponza");
    Model backpack("backpack");
    Instrumentor::Get().EndSession();

    Scene scene(camera);

    glm::mat4 world = glm::mat4(1.0f);
    world = glm::translate(world, { 0.0f ,0.0f, 0.0f });
    world = glm::rotate(world, glm::radians(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    world = glm::scale(world, glm::vec3(1.0f, 1.0f, 1.0f));

    sponza.getModelRootNode()->SetWorldTransformation(world);

    world = glm::translate(world, { 0.0f ,1.5f, 0.0f });
    world = glm::rotate(world, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    world = glm::scale(world, glm::vec3(1.0f, 1.0f, 1.0f));

    backpack.getModelRootNode()->SetWorldTransformation(world);

    scene.GetRootNode()->AddChild(sponza.getModelRootNode());
    scene.GetRootNode()->AddChild(backpack.getModelRootNode());

    std::vector<std::string> faces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg" };

    Cubemap skybox(faces,"assets/skybox/");
    Shader cubemapShader("cubeMapVertex.glsl", "cubeMapFragment.glsl");
    cubemapShader.use();
    cubemapShader.setUniformInt("cubemap", 0);

    std::unique_ptr<Framebuffer> depthFramebuffer = std::make_unique<Framebuffer>();
    depthFramebuffer->initDepth(1024,1024);

    double interval = 0.0f;	
    LARGE_INTEGER currentFrame = {};
    LARGE_INTEGER LastFrame = {};
    LARGE_INTEGER fq = {};
    QueryPerformanceFrequency(&fq);
    while (!glfwWindowShouldClose(window))
    {             
        do {
           QueryPerformanceCounter(&currentFrame);
           interval = (double)(currentFrame.QuadPart - LastFrame.QuadPart) / (double)fq.QuadPart;
        } while (interval < 0.016);
        QueryPerformanceCounter(&LastFrame);
        glfwPollEvents();
        processInput(window, 0.016f);

        //draw into shadow map
        depthFramebuffer->bind();
        glViewport(0, 0, 1024, 1024);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_FRONT);
        scene.DrawShadowMap(); //draw the scene with depthmap shader

        //DRAW TO TEXTURE
        rendertarget->bind();
        glViewport(0, 0, windowWidth, windowHeight);
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);    
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        scene.Draw(depthFramebuffer->getDepthAttachement(),x,z);

        cubemapShader.use();
        cubemapShader.setMat4("PROJ", glm::perspective(camera.fov, camera.aspect, 0.1f, 5000.0f));
        cubemapShader.setMat4("VIEW", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        skybox.Draw();
        
        //DRAW TEXTURE TO QUAD
        Framebuffer::bindDefault();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rendertarget->getColorAttachement());
        glBindVertexArray(quadVBO);
        screenShader.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);

        //GLenum error;
        //if ((error = glGetError()) != GL_NO_ERROR)
        //{
        //    std::cout << error << std::endl;
        //    std::cout << glGetError() << std::endl;
        //    glfwSetWindowShouldClose(window, GLFW_TRUE);
        //}
        
    }

    glfwTerminate();
    
	return 0;
}