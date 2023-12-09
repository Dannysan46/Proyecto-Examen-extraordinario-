/*---------------------------------------------------------*/
/* ----------------  Proyecto Examen extraordinario               -----------*/
/*-----------------    2024-1   ---------------------------*/
/*-- Barrionuevo Pérez Daniel Alejandro	No. de cuenta: 316303116 --*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.27f, 0.10f, 0.8f));
float MovementSpeed = 1000.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 30;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

bool reversa = false;
bool state = false;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, 0.0f, -1.0f);

float myVar = 0.0f;
float  var1 = 0.0f;
float  var2 = 0.0f;
bool var3 = false;
float var4 = 0.0f;

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		movAuto_y = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

int circuito_auto = 0;

//Keyframes (Manipulación y dibujo)
float	posXU = 0.0f,
		posXD = 0.0f,
		posXT = 0.0f,
		posYU = 0.0f,
		posYD = 0.0f,
		posYT = 0.0f,
		posZ = 0.0f,


		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		giroBrazoDer = 0.0f,
		giroBrazoIzq = 0.0f,
		rotRodDer = 0.0f,

		giroCabeza = 0.0f,
		girodisco = 0.0f,
		girocalendario = 0.0f,
		giroojo = 0.0f,
		giropiedra = 0.0f;

// Para calculo de invremento que se va a realizar
float	incXU = 0.0f,
		incXD = 0.0f,
		incXT = 0.0f,
		incYU = 0.0f,
		incYD = 0.0f,
		incYT = 0.0f,

		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		girobrazoDerInc = 0.0f,
		giroBrazoIzqInc = 0.0f,
		rotRodDerInc = 0.0f,

		giroCabezaInc = 0.0f,
		girodiscoInc = 0.0f,
		girocalendarioInc = 0.0f,
		giroojoInc = 0.0f,
		giropiedraInc = 0.0f;

#define MAX_FRAMES 100  //Nos va a indicar cuantos cuadros claves va a poder guardar nuestro sistema
int i_max_steps = 30;  //Cuantos cuadros intermedios nos va a generar . Entre menos cuadros pida, más rápida va a ser la transicion pero será más brusca
int i_curr_steps = 0;

// Esta estructura es la que se va a encargar de guardar los datos para mi sistema
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posXU;		//Variable para PosicionX
	float posXD;
	float posXT;
	float posYU;		//Variable para PosicionY
	float posYD;
	float posYT;
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float giroBrazoDer;
	float giroBrazoIzq;
	float rotRodDer;

	float giroCabeza;  //De estructura, encargada de guardar los datos  
	float girodisco;
	float girocalendario;
	float giroojo;
	float giropiedra;

}FRAME;

//
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 100;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posXU = posXU;
	KeyFrame[FrameIndex].posXD = posXD;
	KeyFrame[FrameIndex].posXT = posXT;
	KeyFrame[FrameIndex].posYU = posYU;
	KeyFrame[FrameIndex].posYD = posYD;
	KeyFrame[FrameIndex].posYT = posYT;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].giroBrazoDer = giroBrazoDer;

	KeyFrame[FrameIndex].giroBrazoIzq = giroBrazoIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;

	KeyFrame[FrameIndex].giroCabeza = giroCabeza; //Con esto el sistema va a poder guardar la informacion
	KeyFrame[FrameIndex].girodisco = girodisco;
	KeyFrame[FrameIndex].girocalendario = girocalendario;
	KeyFrame[FrameIndex].giroojo = giroojo;
	KeyFrame[FrameIndex].giropiedra = giropiedra;


	FrameIndex++;
}

// Este es para que inicie desde el cuadro 0 para la animacion 
void resetElements(void)
{
	posXU = KeyFrame[0].posXU;
	posXD = KeyFrame[0].posXD;
	posXT = KeyFrame[0].posXT;
	posYU = KeyFrame[0].posYU;
	posYD = KeyFrame[0].posYD;
	posYT = KeyFrame[0].posYT;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	giroBrazoDer = KeyFrame[0].giroBrazoDer;

	giroBrazoIzq = KeyFrame[0].giroBrazoIzq;
	rotRodDer = KeyFrame[0].rotRodDer;

	giroCabeza = KeyFrame[0].giroCabeza; //Sustituye lo del escenario y comienza desde el cuadro clave cero
	girodisco = KeyFrame[0].girodisco;
	girocalendario = KeyFrame[0].girocalendario;
	giroojo = KeyFrame[0].giroojo;
	giropiedra = KeyFrame[0].giropiedra;

}

//La funcion de interpolacion es la encargada de realizar la operacion matematica 
void interpolation(void)
{
	incXU = (KeyFrame[playIndex + 1].posXU - KeyFrame[playIndex].posXU) / i_max_steps;
	incXD = (KeyFrame[playIndex + 1].posXD - KeyFrame[playIndex].posXD) / i_max_steps;
	incXT = (KeyFrame[playIndex + 1].posXT - KeyFrame[playIndex].posXT) / i_max_steps;
	incYU = (KeyFrame[playIndex + 1].posYU - KeyFrame[playIndex].posYU) / i_max_steps;
	incYD = (KeyFrame[playIndex + 1].posYD - KeyFrame[playIndex].posYD) / i_max_steps;
	incYT = (KeyFrame[playIndex + 1].posYT - KeyFrame[playIndex].posYT) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	girobrazoDerInc = (KeyFrame[playIndex + 1].giroBrazoDer - KeyFrame[playIndex].giroBrazoDer) / i_max_steps;

	giroBrazoIzqInc = (KeyFrame[playIndex + 1].giroBrazoIzq - KeyFrame[playIndex].giroBrazoIzq) / i_max_steps;
	rotRodDerInc = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;


	giroCabezaInc = (KeyFrame[playIndex + 1].giroCabeza - KeyFrame[playIndex].giroCabeza) / i_max_steps;
	girodiscoInc = (KeyFrame[playIndex + 1].girodisco - KeyFrame[playIndex].girodisco) / i_max_steps;
	girocalendarioInc = (KeyFrame[playIndex + 1].girocalendario - KeyFrame[playIndex].girocalendario) / i_max_steps;
	giroojoInc = (KeyFrame[playIndex + 1].giroojo - KeyFrame[playIndex].giroojo) / i_max_steps;
	giropiedraInc = (KeyFrame[playIndex + 1].giropiedra - KeyFrame[playIndex].giropiedra) / i_max_steps;

}

void animate(void)
{
	lightPosition.x = 300.0f * sin(myVar);
	lightPosition.z = 300.0f * cos(myVar);
	myVar += 0.1f;

	if (var3) {
		var4 = 0.8f;
		var1 = lightPosition.x;
		var2 = lightPosition.z;
	}
	else {
		var1 = 0.0f;
		var2 = 0.0f;
		var4 = 0.0f;
	}


	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posXU += incXU;
			posXD += incXD;
			posXT += incXT;
			posYU += incYU;
			posYD += incYD;
			posYT += incYT;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			giroBrazoDer += girobrazoDerInc;

			giroBrazoIzq += giroBrazoIzqInc;
			rotRodDer += rotRodDerInc;

			giroCabeza += giroCabezaInc; //Se le va aestar sumando a la variable de dibujo para que haga la transicion
			girodisco += girodiscoInc;
			girocalendario += girocalendarioInc;
			giroojo += giroojoInc;
			giropiedra += giropiedraInc;

			i_curr_steps++;
		}
	}

	


}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC 20241", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models               AQUI SE CARGAN LOS OBJETOS
	// -----------
	Model base("resources/objects/Base/base.obj");
	Model toroideuno("resources/objects/Toroide/toroideuno.obj");
	Model toroidedos("resources/objects/Toroide/toroidedos.obj");
	Model toroidetres("resources/objects/Toroide/toroidetres.obj");
	Model plano("resources/objects/Plano/Plano.obj");



	//Inicialización de KeyFrames
	//for (int i = 0; i < MAX_FRAMES; i++)
	{

		KeyFrame[0].posXU = 0.0f;
		KeyFrame[0].posYU = 0.0f;
		KeyFrame[0].posXD = 0.0f;
		KeyFrame[0].posYD = 0.0f;
		//Primer animacion
		KeyFrame[0].posXT = 0.0f;
		KeyFrame[0].posYT = 0.0f;
		
		KeyFrame[1].posYT = 0.05f;
		//KeyFrame[1].posXT = 0.0f;

		KeyFrame[2].posYT = 0.05f;
		KeyFrame[2].posXT = 0.42f;
		
		KeyFrame[3].posYT = -0.1f;
		KeyFrame[3].posXT = 0.42f;

		//Segunda animacion
		KeyFrame[4].posYD = 0.09f;
		KeyFrame[4].posXD = 0.0f;
		KeyFrame[4].posYT = -0.1f;
		KeyFrame[4].posXT = 0.42f;

		KeyFrame[5].posYD = 0.09f;
		KeyFrame[5].posXD = 0.21f;
		KeyFrame[5].posYT = -0.1f;
		KeyFrame[5].posXT = 0.42f;

		KeyFrame[6].posYD = -0.05f;   //CHECAR VALOR
		KeyFrame[6].posXD = 0.21f;
		KeyFrame[6].posYT = -0.1f;
		KeyFrame[6].posXT = 0.42f;

		//Tercera animacion
		KeyFrame[7].posYD = -0.05f;
		KeyFrame[7].posXD = 0.21f;
		KeyFrame[7].posYT = 0.08f;
		KeyFrame[7].posXT = 0.42f;

		KeyFrame[8].posYD = -0.05f;
		KeyFrame[8].posXD = 0.21f;
		KeyFrame[8].posYT = 0.1f;
		KeyFrame[8].posXT = 0.21f;

		KeyFrame[9].posYD = -0.05f;
		KeyFrame[9].posXD = 0.21f;
		KeyFrame[9].posYT = -0.05f;
		KeyFrame[9].posXT = 0.21f;

		//Cuarta animacion
		KeyFrame[10].posYU = 0.15f;
		KeyFrame[10].posXU = 0.0f;
		KeyFrame[10].posYD = -0.05f;
		KeyFrame[10].posXD = 0.21f;
		KeyFrame[10].posYT = -0.05f;
		KeyFrame[10].posXT = 0.21f;

		KeyFrame[11].posYU = 0.15f;
		KeyFrame[11].posXU = 0.42f;
		KeyFrame[11].posYD = -0.05f;
		KeyFrame[11].posXD = 0.21f;
		KeyFrame[11].posYT = -0.05f;
		KeyFrame[11].posXT = 0.21f;

		KeyFrame[12].posYU = 0.0f;
		KeyFrame[12].posXU = 0.42f;
		KeyFrame[12].posYD = -0.05f;
		KeyFrame[12].posXD = 0.21f;
		KeyFrame[12].posYT = -0.05f;
		KeyFrame[12].posXT = 0.21f;

		//Quinta animacion 
		KeyFrame[13].posYU = 0.0f;
		KeyFrame[13].posXU = 0.42f;
		KeyFrame[13].posYD = -0.05f;
		KeyFrame[13].posXD = 0.21f;
		KeyFrame[13].posYT = 0.05f;
		KeyFrame[13].posXT = 0.21f;

		KeyFrame[14].posYU = 0.0f;
		KeyFrame[14].posXU = 0.42f;
		KeyFrame[14].posYD = -0.05f;
		KeyFrame[14].posXD = 0.21f;
		KeyFrame[14].posYT = 0.05f;
		KeyFrame[14].posXT = 0.0f;

		KeyFrame[15].posYU = 0.0f;
		KeyFrame[15].posXU = 0.42f;
		KeyFrame[15].posYD = -0.05f;
		KeyFrame[15].posXD = 0.21f;
		KeyFrame[15].posYT = -0.1f;
		KeyFrame[15].posXT = 0.0f;

		//Sexta animacion 
		KeyFrame[16].posYU = 0.0f;
		KeyFrame[16].posXU = 0.42f;
		KeyFrame[16].posYD = 0.09f;
		KeyFrame[16].posXD = 0.21f;
		KeyFrame[16].posYT = -0.1f;
		KeyFrame[16].posXT = 0.0f;

		KeyFrame[17].posYU = 0.0f;
		KeyFrame[17].posXU = 0.42f;
		KeyFrame[17].posYD = 0.09f;
		KeyFrame[17].posXD = 0.42f;
		KeyFrame[17].posYT = -0.1f;
		KeyFrame[17].posXT = 0.0f;

		KeyFrame[18].posYU = 0.0f;
		KeyFrame[18].posXU = 0.42f;
		KeyFrame[18].posYD = 0.0f;
		KeyFrame[18].posXD = 0.42f;
		KeyFrame[18].posYT = -0.1f;
		KeyFrame[18].posXT = 0.0f;

		//Septima animacion
		KeyFrame[19].posYU = 0.0f;
		KeyFrame[19].posXU = 0.42f;
		KeyFrame[19].posYD = 0.0f;
		KeyFrame[19].posXD = 0.42f;
		KeyFrame[19].posYT = 0.1f;
		KeyFrame[19].posXT = 0.0f;

		KeyFrame[20].posYU = 0.0f;
		KeyFrame[20].posXU = 0.42f;
		KeyFrame[20].posYD = 0.0f;
		KeyFrame[20].posXD = 0.42f;
		KeyFrame[20].posYT = 0.1f;
		KeyFrame[20].posXT = 0.42f;

		KeyFrame[21].posYU = 0.0f;
		KeyFrame[21].posXU = 0.42f;
		KeyFrame[21].posYD = 0.0f;
		KeyFrame[21].posXD = 0.42f;
		KeyFrame[21].posYT = 0.0f;
		KeyFrame[21].posXT = 0.42f;

		

		
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection); //Direccion de los 
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.9f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.000032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.01f);
		staticShader.setFloat("pointLight[1].linear", 0.0009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(var1, var2, var4));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(var1, var2, var4));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.4f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f))); //Apartura que va a tener 
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f))); //difuminacion  
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personajes Animados Complejos
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//Base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f));
		staticShader.setMat4("model", model);
		base.Draw(staticShader);

		//Toroide uno
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(posXU, posYU, 0));
		model = glm::scale(model, glm::vec3(0.025f));
		staticShader.setMat4("model", model);
		toroideuno.Draw(staticShader);

		//Toroide dos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(posXD, posYD, 0));
		model = glm::scale(model, glm::vec3(0.025f));
		staticShader.setMat4("model", model);
		toroidedos.Draw(staticShader);

		//Toroide tres
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(posXT, posYT, 0));
		model = glm::scale(model, glm::vec3(0.025f));
		staticShader.setMat4("model", model);
		toroidetres.Draw(staticShader);

		//Plano
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, -50.0f));
		//model = glm::scale(model, glm::vec3(0.025f));
		staticShader.setMat4("model", model);
		plano.Draw(staticShader);

		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}