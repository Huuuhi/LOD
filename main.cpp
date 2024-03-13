

#include <glad/glad.h>

// #include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"


#include "generate.h"
#include <vector>
#include "dataBinder.h"
#include "OpenGLContext.h"
#include "Simplify.h"

#include "LoadObj.h"
#include "GLError.h"
#include <iostream>


using namespace std;


enum class RenderObject
{
  ORIGIN_VERTICES,
  OPENMESH_MESH
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
// data
vector<glm::vec3> vertices; // 网格中的顶点
vector<double> f_vertices;
vector<unsigned int> indices; // 三角形   
vector<float> scalarData;
vector<glm::vec3> normals;
vector<float> f_normals;


float maxScalar = std::numeric_limits<float>::lowest();
float minScalar = std::numeric_limits<float>::max();


int main()
{
    
    OpenGLContext openglContext;

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    Shader renderShader("resource/shader/render.vs", "resource/shader/render.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //generateCylinderVertices(1, 2, 30, 30, vertices, normals, scalarData);
    //generateCylinderIndices(30, 30, indices);
    //generateTwoCylindersVertices(1, 1.5, 2, 3000, 3000, f_vertices, f_normals, scalarData);
    //generateTwoCylindersIndices(3000, 3000, indices);
    //generateCylinderVertices(1, 30, 30, vertices, normals, scalarData);
    //generateCylinderIndices(30, 30, indices);
    // first, configure the cube's VAO (and VBO)


    std::string filePath = "resource/bunny.obj";
    //std::string filePath = "resource/sphere.obj";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            load_obj(filePath.c_str(), f_vertices, indices, scalarData, i, j);
        }
    }
    for (int i = 2; i < 4; i++)
    {
        for (int j = 2; j < 4; j++)
        {
            load_obj("resource/Cat_head.obj", f_vertices, indices, scalarData, i, j, 0, 0.01, 0.01, 0.01);
        }
    }



    DataBinder::GetInstance().bindData(f_vertices, indices, scalarData);

    glfwSetKeyCallback(openglContext.window, keyCallback);

    maxScalar = std::numeric_limits<float>::lowest();
    minScalar = std::numeric_limits<float>::max();
    // 最大最小值
    for (const auto& scalar : scalarData)
    {
        maxScalar = maxScalar > scalar ? maxScalar : scalar;
        minScalar = minScalar < scalar ? minScalar : scalar;
    }

    // 纹理 
    int32_t texture = DataBinder::GetInstance().createTexture(createLut(10, 0, 0.7));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(openglContext.window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(openglContext.window);

        // render
        // ------
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        renderShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        renderShader.setMat4("projection", projection);
        renderShader.setMat4("view", view);
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        renderShader.setMat4("model", model);
        //renderShader.setInt("textureMap", texture);
        
        renderShader.setFloat("minScalar", minScalar);
        renderShader.setFloat("maxScalar", maxScalar);
        
        DataBinder::GetInstance().bindTexture(texture);

        // 启用线框模式
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        // render
        glBindVertexArray(DataBinder::GetInstance().getVAO());
        GLCall(glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT, 0));
        
        


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLCall(glfwSwapBuffers(openglContext.window));
        glfwPollEvents();
    }


    
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly 轮询
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
        return ;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        return;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        Simplify s;
        SimplifyObject obj = s.simplify(f_vertices, indices, scalarData, 0.5);
        f_vertices = obj.vertices;
        indices = obj.indices;
        scalarData = obj.scalars;
        maxScalar = std::numeric_limits<float>::lowest();
        minScalar = std::numeric_limits<float>::max();
        // 最大最小值
        for (const auto& scalar : scalarData) 
        {
            maxScalar = maxScalar > scalar ? maxScalar : scalar;
            minScalar = minScalar < scalar ? minScalar : scalar;
        }
        DataBinder::GetInstance().bindData(f_vertices, indices, scalarData);
    }
}

