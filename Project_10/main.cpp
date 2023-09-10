
///////////////////////////////////////////////////*
//Fabian Concha Sifuentes
//Ejercicio 1
//Con tecla a se aumenta el tamaño del triangulo
//Con tecla s se reduce el tamaño del triangulo
//La figura Flecha es el triangulo
//////////////////////////////////////////////////


#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader
//Tiene una variable uniform (puede ser modificada mientras el programa corre) --> output: FragColor == variable uniform
const char* fragmentShaderSource = "#version 330 core\n"
"layout(location=0) out vec4 FragColor;\n"
"uniform vec4 u_color;"
"void main()\n"
"{\n"
"   FragColor=u_color;\n"
"}\n\0";

//Para detectar entradas por teclado
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

//para los inputs
char figure = 'a';

////////////////////////////////CLASE Transformaciones//////////////////////////////////////////////////////////
struct Transformaciones 
{
    //Creacion de las matrices Rx, Ry, Rz, T, S
	float rx[4][4], ry[4][4], rz[4][4], T[4][4], S[4][4], invrx[4][4], invry[4][4], invrz[4][4], invT[4][4], invS[4][4];

    //Inicializar las matrices
    Transformaciones() 
	{
        float tmp[4][4] = { {0,0,0,0},
							{0,0,0,0},
							{0,0,0,0},
							{0,0,0,1} };
        for (int i = 0; i < 4; i++) 
		{
            for (int j = 0; j < 4; j++) 
			{
                rx[i][j] = tmp[i][j];
                ry[i][j] = tmp[i][j];
                rz[i][j] = tmp[i][j];
				invrx[i][j] = tmp[i][j];
				invry[i][j] = tmp[i][j];
				invrz[i][j] = tmp[i][j];
                T[i][j] = tmp[i][j];
				invT[i][j] = tmp[i][j];
                S[i][j] = tmp[i][j];
				invS[i][j] = tmp[i][j];
            }
        }
    }

    float* multiplicacion(float matriz1[3], float matriz2[4][4], bool vector) 
	{
        float tmp[4][1];
        for (int i = 0; i < 4; i++) 
		{
            for (int j = 0; j < 1; j++) 
			{
                tmp[i][j] = 0;
            }
        }

        float new_m1[4][1];
        for (int i = 0; i < 3; i++) 
		{
            new_m1[i][0] = matriz1[i];
        }
        if (!vector) 
		{
            new_m1[3][0] = 0;
        }
        else 
		{
            new_m1[3][0] = 1;
        }

        for (int i = 0; i < 4; i++) 
		{
            for (int j = 0; j < 1; j++) 
			{
                for (int k = 0; k < 4; k++) 
				{
                    tmp[i][j] += matriz2[i][k] * new_m1[k][j];
                }
            }
        }

        for (int i = 0; i < 3; i++) 
		{
            matriz1[i] = tmp[i][0];
        }
        return matriz1;
    }

    //Rotacion en x
    void R_x(float angle, float* &vertices, int size) 
	{
        //Convertimos a grados
        float degree = angle * (3.141592653589793238463 / 180); //convertimos los angulos
        float tmp[3];
        float* res = new float[3];
        //Llenado de la matrix R_x con el angulo 
        rx[0][0] = 1;
        rx[1][1] = cos(degree); 
		rx[1][2] = -sin(degree);
        rx[2][1] = sin(degree); 
		rx[2][2] = cos(degree);
        //Multiplicamos por todos los vectores recibidos y actualizamos nuestra matriz
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, rx, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Rotacion inversa en x
	void Ri_x(float angle, float* &vertices, int size) 
	{
        //Convertimos a grados
        float degree = angle * (3.141592653589793238463 / 180); //convertimos los angulos
        float tmp[3];
        float* res = new float[3];
        //Llenado de la matrix R_x con el angulo 
        invrx[0][0] = 1;
        invrx[1][1] = cos(degree); 
		invrx[1][2] = sin(degree);
        invrx[2][1] = -sin(degree); 
		invrx[2][2] = cos(degree);
        //Multiplicamos por todos los vectores recibidos y actualizamos nuestra matriz
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, invrx, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	
	//Rotacion en y
    void R_y(float angle, float* &vertices, int size) 
	{
        float degree = angle * (3.141592653589793238463 / 180); //convertimos los angulos
        float tmp[3];
        float* res = new float[3];
        ry[0][0] = cos(degree); 
		ry[0][2] = sin(degree);
        ry[1][1] = 1;
        ry[2][0] = -sin(degree); 
		ry[2][2] = cos(degree);
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, ry, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 

        }
    }
	//Rotacion inversa en y
	void Ri_y(float angle, float* &vertices, int size) 
	{
        float degree = angle * (3.141592653589793238463 / 180); //convertimos los angulos
        float tmp[3];
        float* res = new float[3];
        invry[0][0] = cos(degree); 
		invry[0][2] = -sin(degree);
        invry[1][1] = 1;
        invry[2][0] = sin(degree); 
		invry[2][2] = cos(degree);
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, invry, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 

        }
    }
	//Rotacion en z
    void R_z(float angle, float* &vertices, int size) 
	{
        float degree = angle * (3.141592653589793238463 / 180); //convertimos los angulos
        float tmp[3];
        float* res = new float[3];
        rz[0][0] = cos(degree); 
		rz[0][1] = -sin(degree);
        rz[1][0] = sin(degree); 
		rz[1][1] = cos(degree);
        rz[2][2] = 1;
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, rz, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Rotacion inversa en z
    void Ri_z(float angle, float* &vertices, int size) 
	{
        float degree = angle * (3.141592653589793238463 / 180); //convertimos los angulos
        float tmp[3];
        float* res = new float[3];
        invrz[0][0] = cos(degree); 
		invrz[0][1] = sin(degree);
        invrz[1][0] = -sin(degree); 
		invrz[1][1] = cos(degree);
        invrz[2][2] = 1;
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, invrz, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Traslacion
    void Translation(float tx, float ty, float tz, float* &vertices, int size) 
	{
        float tmp[3];
        float* res = new float[3];
        T[0][0] = 1; T[0][3] = tx;
        T[1][1] = 1; T[1][3] = ty;
        T[2][2] = 1; T[2][3] = tz;
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, T, 1);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Inversa de traslacion
	void Translationi(float tx, float ty, float tz, float* &vertices, int size) 
	{
        float tmp[3];
        float* res = new float[3];
        invT[0][0] = 1; invT[0][3] = -tx;
        invT[1][1] = 1; invT[1][3] = -ty;
        invT[2][2] = 1; invT[2][3] = -tz;
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, invT, 1);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Intento de mover en circulos sin cambiar orientiacion de figura
	void Translation2(float tx, float ty, float tz, float* &vertices, int size) 
	{
        float tmp[3];
        float* res = new float[3];
		int aux = 0.4;
		for(int i = 0; i<8;i++)
		{
			T[0][0] = 1; T[0][3] = 1-aux;
			T[1][1] = 1; T[1][3] = 1-aux;
			T[2][2] = 1; T[2][3] = 0;
			aux += 0.2;
		}
		for(int i = 0; i<8;i++)
		{
			T[0][0] = 1; T[0][3] = -1+aux;
			T[1][1] = 1; T[1][3] = -1+aux;
			T[2][2] = 1; T[2][3] = 0;
			aux += 0.2;
		}
        for (int i = 0; i < size; i += 3) {
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, T, 1);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Escala
    void Scale(float sx, float sy, float sz, float* &vertices, int size) 
	{
        float tmp[3];
        float* res = new float[3];
        S[0][0] = sx;
        S[1][1] = sy;
        S[2][2] = sz;
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, S, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
	//Inversa de escala
	void Scalei(float sx, float sy, float sz, float* &vertices, int size) 
	{
        float tmp[3];
        float* res = new float[3];
        invS[0][0] = 1/sx;
        invS[1][1] = 1/sy;
        invS[2][2] = 1/sz;
        for (int i = 0; i < size; i += 3) 
		{
            tmp[0] = vertices[i];
            tmp[1] = vertices[i + 1];
            tmp[2] = vertices[i + 2];
            res = multiplicacion(tmp, invS, 0);
            vertices[i] = res[0];
            vertices[i + 1] = res[1];
            vertices[i + 2] = res[2];
			
			cout << res[0] << " - " << res[1] << " - " << res[2] << endl; 
        }
    }
};

///////////////////CLASE FIGURA////////////////////////////////////////////////////////////////////////

struct Figure 
{
    float *vertices;
    unsigned int *indxTriangulos;
    unsigned int* indxBorde;
    int vSize, i1Size, i2Size;
    unsigned int VBO, EBO1, EBO2;
    Transformaciones Transform;

    Figure():vertices(NULL),indxTriangulos(NULL),indxBorde(NULL),vSize(0),i1Size(0),i2Size(0),VBO(0),EBO1(0),EBO2(0)
    {}

    Figure(unsigned int VBO, unsigned int EBO1, unsigned int EBO2) :vertices(NULL), indxTriangulos(NULL), indxBorde(NULL), vSize(0), i1Size(0), i2Size(0)
    {
        this->VBO = VBO;
        this->EBO1 = EBO1;
        this->EBO2 = EBO2;
    }

    void setVertices(int size, float* vertices) 
	{
        vSize = size;
        this->vertices = new float[vSize];
        this->vertices = vertices;
        this->VBO = VBO;
    }

    void setContorno(int size, unsigned int* indices) 
	{
        i2Size = size;
        this->indxBorde = new unsigned int[i2Size];
        this->indxBorde = indices;
        this->EBO1 = EBO1;
    }

    void setTriangulos(int size, unsigned int* indices) 
	{
        i1Size = size;
        this->indxTriangulos = new unsigned int[i1Size];
        this->indxTriangulos = indices;
        this->EBO2 = EBO2;
    }

    //Crea la figura en el VBO
    void createVer_VBO() 
	{
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize*sizeof(float), vertices, GL_STATIC_DRAW);
    }

    //Crea los indices en EBO
    void createCon_EBO2() 
	{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i2Size* sizeof(int), indxBorde, GL_STATIC_DRAW);
    }

    //Crea los indices del trianguo en EBO2
    void createTri_EBO1() 
	{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, i1Size* sizeof(int), indxTriangulos, GL_STATIC_DRAW);
    }

    void update_buffers() 
	{
        createVer_VBO();
        createCon_EBO2();
        createTri_EBO1();
    }

    void rot_x(float angle) { Transform.R_x(angle, vertices, vSize); }
	void roti_x(float angle) { Transform.Ri_x(angle, vertices, vSize); }
    void rot_y(float angle) { Transform.R_y(angle, vertices, vSize); }
	void roti_y(float angle) { Transform.Ri_y(angle, vertices, vSize); }
    void rot_z(float angle) { Transform.R_z(angle, vertices, vSize); }
	void roti_z(float angle) { Transform.Ri_z(angle, vertices, vSize); }
    void traslation(float tx, float ty, float tz) { Transform.Translation(tx, ty, tz, vertices, vSize); }
	void traslation2(float tx, float ty, float tz) { Transform.Translation(tx, ty, tz, vertices, vSize); }
	void traslationi(float tx, float ty, float tz) { Transform.Translationi(tx, ty, tz, vertices, vSize); }
    void scale(float sx, float sy, float sz) { Transform.Scale(sx, sy, sz, vertices, vSize); }
	void scalei(float sx, float sy, float sz) { Transform.Scalei(sx, sy, sz, vertices, vSize); }
};

//Creacion del circulo
void create_circle(int angle, Figure &circulo);
int buff_circulo(int angle, Figure &circulo, float* &vertex);
//Creacion de las demás figuras
void create_figure(Figure& temp, int location);

int main()
{
    // glfw: initialize and configure (da la version y tipo de perfil en este caso CORE)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Creamos los shaders usando los source que estan al principio del programa
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader(igual que el vertex shader)
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    //Metemos los shaders en un programa determinado
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Eliminamos porque ya fueron insertados en el programa y ya no los necesitamos
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //----------------------------------ESTRELLA----------------------------------
    float vertices_estrella[] = {
         0.0f, 0.9f, 0.0f,  // top
         -0.95, 0.02f, 0.0f,  // bottom
         -0.6f, -0.9f, 0.0f,   // right

         0.6f, -0.9f, 0.0f, //left
         0.95f, 0.2f, 0.0f, //right
         0.0f, -0.48f, 0.0f, //bottom

         0.37f, -0.22f, 0.0f,  // top
         0.23, 0.2f, 0.0f,  // left
         -0.23f, 0.2f, 0.0f,   // bottom

         - 0.37f, -0.22f, 0.0f, //otro
         0.1f, 0.19f, 0.0f //otro
    };

    unsigned int indices_estrella[] = {  
		0,8,7,
		1,8,9,
		2,9,5,
		3,5,6,
		4,6,7,
		8,9,5,
		5,8,7,
		5,6,7,
    };

    unsigned int indices2_estrella[] = {  
		0,7,
		7,4,
		4,6,
		6,3,
		3,5,
		5,2,
		2,9,
		9,1,
		1,8,
		8,0
    };
    //----------------------------------ESTRELLA----------------------------------
	//----------------------------------CUADRADO----------------------------------
	float vertices_cuadrado[] = {
        //top triangle
        //0.0f, 0.6f, 0.0f, //top
         //0.4f, 0.1f, 0.0f, //right
         //-0.4, 0.1f, 0.0f, //left

         //rectangle (2 triangles)
         -0.2f, 0.1f, 0.0f,
         0.2f, 0.1f, 0.0f,
         -0.2f, -0.2f, 0.0f,
         0.2f, -0.2f, 0.0f
    };

    unsigned int indices_cuadrado[] = {
        //0, 1, 2
        //3, 4, 5,
        //5, 6, 4
	
	//Rectangulo
		0,1,2,
		2,3,1
    };

    unsigned int indices2_cuadrado[] = {
       /*0,1,
        1,4,
        4,6,
        6,5,
        5,3,
        3,2,
        2,0
		0,1,
		1,2,
		2,0*/
	
	//Rectangulo
		0,1,1,3,3,2,2,0
    };
    //-----------------------------------FLECHA-----------------------------------
    float vertices_flecha[] = {
        //top triangle
         0.0f, 0.6f, 0.0f, //top
         0.4f, 0.1f, 0.0f, //right
         -0.4, 0.1f, 0.0f, //left

         //rectangle (2 triangles)
         //-0.2f, 0.1f, 0.0f,
         //0.2f, 0.1f, 0.0f,
         //-0.2f, -0.2f, 0.0f,
         //0.2f, -0.2f, 0.0f
    };

    unsigned int indices_flecha[] = {
        0, 1, 2
        //3, 4, 5,
        //5, 6, 4
	
	//Rectangulo
	//	0,1,2,
	//	2,3,1
    };

    unsigned int indices2_flecha[] = {
       /*0,1,
        1,4,
        4,6,
        6,5,
        5,3,
        3,2,
        2,0*/
		0,1,
		1,2,
		2,0
	
	//Rectangulo
	//0,1,1,2,3,0
    };
    //-----------------------------------FLECHA-----------------------------------
    unsigned int VAO, VBO[3], EBO[6];
    //Generamos buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(6, EBO);
    
    //Bindeamos el VAO (significa que ahora OPENGL va a utilizar este VAO en especifico)
    glBindVertexArray(VAO);
    //VBO[0] = Estrella
    //VBO[1] = Flecha
    //VBO[2] = Circulo
 
    //Creo mis figuras--------------------
    //Le doy los Buffers que usara
    Figure Estrella(VBO[0], EBO[0], EBO[1]);
    //Creando vertices
    Estrella.setVertices(sizeof(vertices_estrella) / sizeof(float), vertices_estrella);
    //Indices de los triangulos
    Estrella.setTriangulos(sizeof(indices_estrella) / sizeof(unsigned int), indices_estrella);
    //Indices del contorno
    Estrella.setContorno(sizeof(indices2_estrella) / sizeof(unsigned int), indices2_estrella);

    Figure Flecha(VBO[1], EBO[2], EBO[3]);
    Flecha.setVertices(sizeof(vertices_flecha) / sizeof(float), vertices_flecha);
    Flecha.setTriangulos(sizeof(indices_flecha) / sizeof(unsigned int), indices_flecha);
    Flecha.setContorno(sizeof(indices2_flecha) / sizeof(unsigned int), indices2_flecha);
	
	Figure Cuadrado(VBO[2], EBO[4], EBO[5]);
    Cuadrado.setVertices(sizeof(vertices_cuadrado) / sizeof(float), vertices_cuadrado);
    Cuadrado.setTriangulos(sizeof(indices_cuadrado) / sizeof(unsigned int), indices_cuadrado);
    Cuadrado.setContorno(sizeof(indices2_cuadrado) / sizeof(unsigned int), indices2_cuadrado);

    //Figure Circulo(VBO[2], EBO[4], EBO[5]);

    //Creando en los buffers
    Estrella.update_buffers();
    Flecha.update_buffers();
	Cuadrado.update_buffers();

    //Aqui busco el lugar de mi variable uniform y lo guardo
    int location = glGetUniformLocation(shaderProgram, "u_color");
    if (location == -1) {
        printf("Error al conseguir uniform");
        exit(1);
    }

    //aqui desbindeamos todo (es buena practica)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Revisa el input que le demos
    glfwSetKeyCallback(window, keyCallback);

    //crea el circulo 
	float* auxvertex;
	//int mov = buff_circulo(5,Circulo,auxvertex)
	
    //int mov = buff_circulo(5, Circulo,auxvertex);
    float currColor = 0.1f;
    float adder = 0.005f;
	int flag = -1;


	Estrella.scale(0.15,0.15,0.15);
	Estrella.traslation(-0.5,-0.5,0);
	
	Flecha.scale(0.25,0.25,0.25);
	Cuadrado.scale(0.25,0.25,0.25);
	Cuadrado.traslation(-0.35,-0.35,0);
	Cuadrado.rot_x(0.75);
	//Flecha.traslation(0.5,0.5,0);
	
	//int time = ((int)glfwGetTime())%2;
	
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        if (currColor < 0.0f || currColor>1.0f) {
            adder *= -1.0f;
        }
        currColor += adder;

        glUniform4f(location, 1.0f, 0.5f, 0.2f, 1.0f);

        glLineWidth((GLfloat)5);

        glPointSize((GLfloat)5);

        switch (figure)
        {
        case 'a': {
			//Se crea la estrella
			create_figure(Estrella, location);
            Estrella.rot_z(0.099); //se le da rotacion en z para girar circularmente
            //Estrella.traslation(0.3,0.5,0);
            Estrella.update_buffers();
			
			glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f); //se usa otro uniform para dar un color diferente
			create_figure(Cuadrado, location); //Se crea el cuadrado
			
			
			//flag = time;
			Cuadrado.roti_z(0.099); //rota en z en sentido antihorario
			Cuadrado.update_buffers();
			
			
			glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f); //se le da otro color
            create_figure(Flecha, location); //se crea el triangulo
            //Flecha.roti_z(0.4);
			Flecha.scale(1.0002,1.0002,0); //se le cambia el tamaño - AUMENTA
            Flecha.update_buffers();
        }
            break;
        case 'b': {
			
			//Estrella.color(0.2f,0.4f,0.3f,0.1f);
			create_figure(Estrella, location); //Se crea la estrella
            Estrella.roti_z(0.099); //se le da rotacion en z
            //Estrella.traslation(0.3,0.5,0);
            Estrella.update_buffers();
			
			glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f); //se le da nuevo color
			create_figure(Cuadrado, location); //se crea cuadrado
			
			
			//flag = time;
			Cuadrado.rot_z(0.099); //rotacion horaria en z
			Cuadrado.update_buffers();
			//cout<<auxvertex[time*2]<<" - "<<auxvertex[(time*2)+1]<<endl;					
				
			//Cuadrado.update_buffers();
			//Cuadrado.roti_z(0.099);
			//Cuadrado.update_buffers();
			
			
			glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f); //nuevo color
            create_figure(Flecha, location); //se crea el TRIANGULO
            //Flecha.roti_z(0.4);
			Flecha.scale(0.9991,0.9991,0); //se le cambia el tamaño - SE ENCOGE
            Flecha.update_buffers();
			
			//glUniform4f(location, 1.0f, 0.5f, 0.2f, 1.0f);
			//create_figure(Circulo, location);
			//Circulo.rot_y(0.75);
			//Circulo.scale(0.9999,0.9999,0);
			//Circulo.update_buffers();
			//Circulo.scale(1.0001,1.0001,0);
			//Circulo.update_buffers();
        }   
            break;
        //caso del circulo
        /*case 'c': {
			
			//Estrella.color(0.2f,0.4f,0.3f,0.1f);
			create_figure(Estrella, location);
            Estrella.rot_z(0.4);
            //Estrella.traslation(0.3,0.5,0);
            Estrella.update_buffers();
			
			glUniform4f(location, 1.0f, 0.5f, 0.2f, 1.0f);
			create_figure(Cuadrado, location);
			
			/*int time = ((int)glfwGetTime())%5;
			if(flag != time)
			{
				flag = time;
				Cuadrado.roti_z(0.4);
				cout<<auxvertex[time*2]<<" - "<<auxvertex[(time*2)+1]<<endl;					
			}
			Cuadrado.update_buffers();
			
			Cuadrado.rot_y(0.75);
			Cuadrado.scale(0.999,0.999,0);
			Cuadrado.update_buffers();
			//Circulo.scale(1.0001,1.0001,0);
			Cuadrado.update_buffers();
        }
            break;
        case 'd':{
			//Estrella.color(0.2f,0.4f,0.3f,0.1f);
			create_figure(Estrella, location);
            //Estrella.rot_z(0.4);
            //Estrella.traslation(0.3,0.5,0);
			
			/*int time = ((int)glfwGetTime())%mov;
			if(flag != time)
			{
				flag = time;
				Estrella.traslation(auxvertex[time*2], auxvertex[(time*2)+1],0);
				cout<<auxvertex[time*2]<<" - "<<auxvertex[(time*2)+1]<<endl;					
			}
			Estrella.update_buffers();
		}*/
		}
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(6, EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

//Dibuja la figura que queramos
void create_figure(Figure &temp, int location) {
    glBindBuffer(GL_ARRAY_BUFFER, temp.VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO1);
    glDrawElements(GL_TRIANGLES, temp.i1Size, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.EBO2);
    glUniform4f(location, 0.0f, 0.0f, 3.0f, 1.0f); //color de los bordes
    glDrawElements(GL_LINES, temp.i2Size, GL_UNSIGNED_INT, 0);
    glUniform4f(location, 0.0f, 0.0f, 0.0f, 1.0f); //color de los puntos
    glDrawElements(GL_POINTS, temp.i1Size*2, GL_UNSIGNED_INT, 0);
}

//Key input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //si presionas a muestra la estrella
    if (key == GLFW_KEY_A)
    {
        figure='a';
    }
    //si presiones b muestra la flecha
    if (key == GLFW_KEY_S)
    {
        figure = 'b';
    }
    //si presiones d muestra el circulo
    if (key == GLFW_KEY_D)
    {
        figure = 'c';
    }
	if (key == GLFW_KEY_F)
	{
		figure = 'f';
	}
}

//recibe el angulo y los buffers que usara para crear el circulo
void create_circle(int angle, Figure &Circulo) {
    //Numero de iteraciones que tendremos
    int iterations = 360 / angle;
    //Punto x inicial, y punto y inicial, la magnitud siempre sera la misma
    //currentangle mantiene un seguimiento del angulo en total, hasta llegar a 360
    //degree solo convierte de angulo a radian, la funcion cos y sin toman como parametro radianes.
    float pointx = 0.4f, pointy = 0.0f, magnitude = 0.4f, currentAngle = 0.0, degree = 0.0;
    //Creo los vectores de vertices e indices.
    float *vertices_circulo = new float[(iterations * 3) + 6];
    unsigned int*indices_circulo = new unsigned int[(iterations * 3) + 3];
    unsigned int*indices2_circulo = new unsigned int[iterations * 2];
    //El primer punto sera en el centro (para luego usarlo en los indices y crear triangulos)
    vertices_circulo[0] = 0.0f;
    vertices_circulo[1] = 0.0f;
    vertices_circulo[2] = 0.0f;

    //Luego nuestro segundo punto sera x=0.4
    vertices_circulo[3] = pointx;
    vertices_circulo[4] = pointy;
    vertices_circulo[5] = 0.0f;

    //Creando vertices
    //empezamos en 6 porque ya creamos 2 puntos, luego recorremos de 3 en 3 para ir punto por punto
    for (int i = 6; i <= iterations * 3 + 3; i += 3) {
        //sumamos el angulo (hasta llegar a 360)
        currentAngle += angle;
        //convertimos a radianes
        degree = currentAngle * (3.141592653589793238463 / 180);
        //por trigonometria, x sera hipotenusa*cos(angulo)
        //y sera hipotenusa*sin(angulo) e hipotenusa sera la magnitud (que nunca cambia)
        pointx = magnitude * cos(degree);
        pointy = magnitude * sin(degree);
        //creamos los puntos
        vertices_circulo[i] = pointx;
        vertices_circulo[i + 1] = pointy;
        vertices_circulo[i + 2] = 0.0f;
    }

    //Creando indices
    int count = 1;
    int i = 0;
    for (i; i <= iterations * 3; i += 3) {
        //siempre los triangulos usaron el punto del centro del circulo
        indices_circulo[i] = 0;
        indices_circulo[i + 1] = count;
        indices_circulo[i + 2] = count + 1;
        count++;
    }
    count = 1;
    i = 0;
    for (i; i < iterations * 2; i += 2) {
        //siempre los triangulos usaron el punto del centro del circulo
        indices2_circulo[i] = count;
        indices2_circulo[i + 1] = count + 1;
        count++;
    }

    Circulo.setVertices((iterations * 3 + 6), vertices_circulo);
    Circulo.setTriangulos((iterations * 3 + 3), indices_circulo);
    Circulo.setContorno((iterations * 2), indices2_circulo);

    Circulo.update_buffers();
}

int buff_circulo(int angulo, Figure &Circulo, float* &vertex) {

    int triangulos = 360 / angulo,pos = 1;; 
    float x2 = 0.4f, y2 = 0.0f, radio = 0.4f;
	float angulo_triangulo = 0.0, valor_radian = 0.0;
	
    float *vertices = new float[(triangulos * 3) + 6];
	vertex = new float [(triangulos *2)+2];
    unsigned int*indices = new unsigned int[(triangulos * 3) + 3];
    
    vertices[0] = 0.0f;
	vertex[0] = x2;
    vertices[1] = 0.0f;
	vertex[1] = y2;
    vertices[2] = 0.0f;

    vertices[3] = x2;
    vertices[4] = y2;
    vertices[5] = 0.0f;

    for (int i = 6, j = 2; i <= triangulos * 3 + 3; i += 3, j+=2) 
	{
        angulo_triangulo += angulo;
        valor_radian = angulo_triangulo * (M_PI/ 180);
        x2 = radio * cos(valor_radian);
        y2 = radio * sin(valor_radian);
        
        vertices[i] = x2;
		vertex[j] = x2;
        vertices[i + 1] = y2;
		vertex[j+1] = y2;
        vertices[i + 2] = 0.0f;
    }
    
    for (int i = 0; i <= triangulos * 3; i += 3) {
        indices[i] = 0;
        indices[i + 1] = pos;
        indices[i + 2] = pos + 1;
        pos++;
    }
    
       Circulo.setVertices((triangulos * 3 + 6), vertices);
	   Circulo.setTriangulos((triangulos * 3 + 3), indices);
	   Circulo.setContorno((triangulos * 3 + 3), indices);

	   Circulo.update_buffers();
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