#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES 
#include <cmath>
#include <vector>
//CIRCULO
#include <iostream>

//En vez de process input, usar GLkeyboard_callback

//Paolo Delgado
//Fabian Concha

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";


int buff_circulo(int angulo, unsigned int VBO, unsigned int EBO);

char figure;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
   
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float* vertices_circulo;
    unsigned int* indices_circulo;
    unsigned int* indices2_circulo;
   
    unsigned int VAO, VBO, EBO;
   
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int triangulos = buff_circulo(20, VBO, EBO); 
	
	glLineWidth(3.0f);
	glPointSize(5.0f);
	
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
       
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		
		int location = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(location, 1.0f, 0.5f, 0.2f, 1.0f);
        glDrawElements(GL_TRIANGLES, triangulos*3+3, GL_UNSIGNED_INT, 0);
        
        glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_LINE_LOOP, triangulos*3+3, GL_UNSIGNED_INT, 0);
		glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_POINTS, triangulos*3+3, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


int buff_circulo(int angulo, unsigned int VBO, unsigned int EBO) {

    int triangulos = 360 / angulo; //cantidad de triangulos
	int pos = 1; 
    float x2 = 0.4f, y2 = 0.0f, radio = 0.4f; //inicializar posicoin y radio del circulo
	float angulo_triangulo = 0.0, valor_radian = 0.0; //Inicializar variables para calcular vertices en radianes
	
    float *vertices = new float[(triangulos * 3) + 6];
    unsigned int*indices = new unsigned int[(triangulos * 3) + 3];
    
	//Inicializar primeros dos vertices
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;

    vertices[3] = x2;
    vertices[4] = y2;
    vertices[5] = 0.0f;

	//Cálculo y almacenamiento de los vertices
    for (int i = 6; i <= triangulos * 3 + 3; i += 3) {
        angulo_triangulo += angulo;
        valor_radian = angulo_triangulo * (M_PI/ 180);
        x2 = radio * cos(valor_radian);
        y2 = radio * sin(valor_radian);
        
        vertices[i] = x2;
        vertices[i + 1] = y2;
        vertices[i + 2] = 0.0f;
    }
    //Creacion de los indices
    for (int i = 0; i <= triangulos * 3; i += 3) {
        indices[i] = 0;
        indices[i + 1] = pos;
        indices[i + 2] = pos + 1;
        pos++;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (triangulos * 3 + 6) * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (triangulos * 3 + 6) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    return triangulos;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}