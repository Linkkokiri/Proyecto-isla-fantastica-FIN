/*
* 
* 09 - Animación
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <animatedmodel.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>
#include <particles.h>

#include <irrKlang.h>
using namespace irrklang;

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
Camera camera3rd(glm::vec3(0.686527f, 0.569451f, 2.4934f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

glm::vec3 position(0.686527f, 0.569451f, 2.4934f);//0.686527 mf, 0.567737 mf, 2.4934 mf
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     trdpersonOffset = 1.5f;
float     scaleV = 0.025f;
float     rotateCharacter = 0.0f;
float	  barco_offset = 0.0f;
float	  barco_offsetz = 0.0f;
float	  barco_rotation = 0.0f;
float	  valorpi = std::acos(-1);

// Shaders
Shader *mLightsShader;
Shader *proceduralShader;
Shader *wavesShader;

Shader *cubemapShader;
Shader *dynamicShader;

Shader	*staticShader;
Shader	*particlesShader;

// Partículas
Particles particlesSystem(200); // creamos 200 partículas

// Carga la información del modelo
Model   *particleModel;
Model	*house;
Model   *door;
Model   *moon;
Model   *gridMesh;

// Carga la información del modelo
Model* balon;
Model* silla;
Model* mesa;
Model* arbol;
Model* carpa;
Model* escalera;
Model* escEmb;
Model* faro;
Model* fuente;
//Hoteles
Model* hotel1;
Model* hotel2;
Model* hotel3;
//Tiendas
Model* oxxo;
Model* tienda2;
Model* tienda3;
Model* tienda4;
//OTROS
Model* mundo;
Model* oceano;
Model* pala;
Model* palmera;
Model* pesca;
Model* pino;
Model* plato;
Model* red;
Model* roca;
Model* salvavidas;
Model* silla_plegable;
Model* sombrilla;
Model* toalla;
Model* valla2;
Model* vaso;
Model* volcan;

// Modelos animados
AnimatedModel	*trabajador;
AnimatedModel   *vacacionista;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap *mainCubeMap;

// Light gLight;
std::vector<Light> gLights;

//ILUMINACION
Light light01;
float theta2 = 0.0f;

// Materiales
Material material;
Material material_marmol;
Material material_madera;
Material material_concreto;
Material material_plastico_verde;
Material material_plastico_rojo;
Material material_arena;
Material material_tierra;
Material material_porcelana;
Material material_tela;

float proceduralTime = -4.934802173098;
float wavesTime = 0.0f;

// Audio
ISoundEngine *SoundEngine = createIrrKlangDevice();

// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Animation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs");
	
	proceduralShader = new Shader("shaders2/12_ProceduralAnimation.vs", "shaders2/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders2/13_wavesAnimation.vs", "shaders2/13_wavesAnimation.fs");
	particlesShader = new Shader("shaders2/13_particles.vs", "shaders2/13_particles.fs");

	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	dynamicShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");

	// Máximo número de huesos: 100
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	balon = new Model("models/models_proy/Balon.fbx");
	silla = new Model("models/models_proy/Silla.fbx");
	mesa = new Model("models/models_proy/Mesa.fbx");
	mundo = new Model("models/models_proy/Mundo.fbx");
	oceano = new Model("models/models_proy/Oceano.fbx");
	//Hoteles
	hotel1 = new Model("models/models_proy/Hotel1.fbx");
	hotel2 = new Model("models/models_proy/Hotel2.fbx");
	hotel3 = new Model("models/models_proy/Hotel3.fbx");
	//Tiendas
	oxxo = new Model("models/models_proy/Oxxo.fbx");
	tienda2 = new Model("models/models_proy/Tienda2.fbx");
	tienda3 = new Model("models/models_proy/Tienda3.fbx");
	tienda4 = new Model("models/models_proy/Tienda4.fbx");
	//OTROS
	arbol = new Model("models/models_proy/arbol.fbx");
	carpa = new Model("models/models_proy/Carpa.fbx");
	escalera = new Model("models/models_proy/Escaleras.fbx");
	escEmb = new Model("models/models_proy/EscEmb.fbx");
	faro = new Model("models/models_proy/Faro.fbx");
	fuente = new Model("models/models_proy/Fuente.fbx");
	palmera = new Model("models/models_proy/Palmera.fbx");
	pala = new Model("models/models_proy/Pala.fbx");
	pesca = new Model("models/models_proy/Pesca.fbx");
	pino = new Model("models/models_proy/Pino.fbx");
	plato = new Model("models/models_proy/Plato.fbx");
	red = new Model("models/models_proy/Red.fbx");
	roca = new Model("models/models_proy/Roca.fbx");
	salvavidas = new Model("models/models_proy/Salvavidas.fbx");
	silla_plegable = new Model("models/models_proy/Silla_plegable.fbx");
	sombrilla = new Model("models/models_proy/Sombrilla.fbx");
	toalla = new Model("models/models_proy/Toalla.fbx");
	valla2 = new Model("models/models_proy/Valla_2.fbx");
	vaso = new Model("models/models_proy/Vaso.fbx");
	volcan = new Model("models/models_proy/Volcan.fbx");
	
	//gridMesh = new Model("models/IllumModels/grid.fbx");

	//house = new Model("models/IllumModels/House03.fbx");
	//door = new Model("models/IllumModels/Door.fbx");
	//moon = new Model("models/IllumModels/moon.fbx");
	//gridMesh = new Model("models/IllumModels/grid.fbx");

	//Para animacion

	trabajador = new AnimatedModel("models/models_proy/Trabajador.fbx");
	vacacionista = new AnimatedModel("models/models_proy/Vacacionista.fbx");
	particleModel = new Model("models/models_proy/Gota.fbx");

	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/07/posx.png",
		"textures/cubemap/07/negx.png",
		"textures/cubemap/07/posy.png",
		"textures/cubemap/07/negy.png",
		"textures/cubemap/07/posz.png",
		"textures/cubemap/07/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= trdpersonOffset * forwardView;
	camera3rd.Front = forwardView;

	//Musica
	SoundEngine->play2D("sound/mixkit-wind-leaves-617.mp3", true);

	// Lights configuration
	// 
	//light01.Position = glm::vec3(90.1399f, 0.0f, 1.86253f); //Derecho
	light01.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	//light01.Position = glm::vec3(-6.0673f, 14.0476f, 2.9631f);
	//light01.Position = glm::vec3(posLx, posLy, posLz);
	//light01.Color = glm::vec4(0.8f, 0.8f, 1.0f, 1.0f);
	light01.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light01.Power = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
	gLights.push_back(light01);

	Light light02;
	light02.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	light02.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	gLights.push_back(light02);

	Light light03;
	light03.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	light03.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	gLights.push_back(light03);

	Light light04;
	light04.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	light04.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	gLights.push_back(light04);
	
	// SoundEngine->play2D("sound/EternalGarden.mp3", true);

	// Configuración de propiedades materiales
	// Tabla: http://devernay.free.fr/cours/opengl/materials.html
	material.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	material.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	material.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	material.transparency = 1.0f;
	//material.shininess = 0.6f * 128.0f; //Establece que el brillo (shininess) puede variar entre 0 y 128, es estandar y controla la concentración del brillo especular:
	//Valores bajos producen reflejos más suaves y difusos, útiles para materiales más mate.
	//Valores altos producen reflejos especulares más concentrados, útiles para materiales pulidos y brillantes.

	material_arena.ambient = glm::vec4(0.3f, 0.25f, 0.1f, 1.0f);
	material_arena.diffuse = glm::vec4(0.7f, 0.6f, 0.3f, 1.0f);
	material_arena.specular = glm::vec4(0.1f, 0.1f, 0.05f, 1.0f);
	material_arena.transparency = 1.0f;

	material_tierra.ambient = glm::vec4(0.2f, 0.1f, 0.05f, 1.0f);
	material_tierra.diffuse = glm::vec4(0.4f, 0.2f, 0.1f, 1.0f);
	material_tierra.specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material_tierra.transparency = 1.0f;

	material_porcelana.ambient = glm::vec4(0.25f, 0.22f, 0.20f, 1.0f);
	material_porcelana.diffuse = glm::vec4(0.8f, 0.7f, 0.65f, 1.0f);
	material_porcelana.specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	material_porcelana.transparency = 1.0f;

	material_tela.ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
	material_tela.diffuse = glm::vec4(0.8f, 0.8f, 0.9f, 1.0f);
	material_tela.specular = glm::vec4(0.3f, 0.3f, 0.35f, 1.0f);
	material_tela.transparency = 1.0f;

	material_marmol.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	material_marmol.diffuse = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	material_marmol.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	material_marmol.transparency = 1.0f;
	//material_marmol.shininess = 0.4f * 128.0f; //Establece que el brillo (shininess) puede variar entre 0 y 128, es estandar y controla la concentración del brillo especular:
	//Valores bajos producen reflejos más suaves y difusos, útiles para materiales más mate.
	//Valores altos producen reflejos especulares más concentrados, útiles para materiales pulidos y brillantes.

	material_madera.ambient = glm::vec4(0.4f, 0.25f, 0.1f, 1.0f);
	material_madera.diffuse = glm::vec4(0.6f, 0.4f, 0.2f, 1.0f);
	material_madera.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	material_madera.transparency = 1.0f;
	//material_madera.shininess = 0.1f * 128.0f; //Establece que el brillo (shininess) puede variar entre 0 y 128, es estandar y controla la concentración del brillo especular:
	//Valores bajos producen reflejos más suaves y difusos, útiles para materiales más mate.
	//Valores altos producen reflejos especulares más concentrados, útiles para materiales pulidos y brillantes.

	material_plastico_verde.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material_plastico_verde.diffuse = glm::vec4(0.1f, 0.35f, 0.1f, 1.0f);
	material_plastico_verde.specular = glm::vec4(0.45f, 0.55f, 0.45f, 1.0f);
	material_plastico_verde.transparency = 1.0f;
	//material_plastico_verde.shininess = 0.25f * 128.0f; //Establece que el brillo (shininess) puede variar entre 0 y 128, es estandar y controla la concentración del brillo especular:
	//Valores bajos producen reflejos más suaves y difusos, útiles para materiales más mate.
	//Valores altos producen reflejos especulares más concentrados, útiles para materiales pulidos y brillantes.

	material_plastico_rojo.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	material_plastico_rojo.diffuse = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	material_plastico_rojo.specular = glm::vec4(0.7f, 0.6f, 0.6f, 1.0f);
	material_plastico_rojo.transparency = 1.0f;

	material_concreto.ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
	material_concreto.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	material_concreto.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	material_concreto.transparency = 1.0f;
	//material_concreto.shininess = 0.05f * 128.0f; //Establece que el brillo (shininess) puede variar entre 0 y 128, es estandar y controla la concentración del brillo especular:
	//Valores bajos producen reflejos más suaves y difusos, útiles para materiales más mate.
	//Valores altos producen reflejos especulares más concentrados, útiles para materiales pulidos y brillantes.

	return true;
}


void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//ANIMACION PARTICULAS
	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / 30.0f) {
		elapsedTime = 0.0f;

		particlesSystem.UpdatePhysics(deltaTime);
	}

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection;
	glm::mat4 view;

	if (activeCamera) {
		// Cámara en primera persona
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
	}
	else {
		// cámara en tercera persona
		projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera3rd.GetViewMatrix();
	}

	// Cubemap (fondo)
	{
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}
	
	 {
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		mLightsShader->setInt("numLights", 1);
		SetLightUniformVec3(mLightsShader, "Position", 0, gLights[0].Position);
		SetLightUniformVec3(mLightsShader, "Direction", 0, gLights[0].Direction);
		SetLightUniformVec4(mLightsShader, "Color", 0, gLights[0].Color);
		SetLightUniformVec4(mLightsShader, "Power", 0, gLights[0].Power);
		SetLightUniformInt(mLightsShader, "alphaIndex", 0, gLights[0].alphaIndex);
		SetLightUniformFloat(mLightsShader, "distance", 0, gLights[0].distance);
		/*for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}*/
		
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", material_arena.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_arena.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_arena.specular);
		mLightsShader->setFloat("transparency", material_arena.transparency);

		mundo->Draw(*mLightsShader);
		
	}

	glUseProgram(0);
	

	//ELEMENTOS NATURALES TOTAL 4 MODELOS
	//Arbol

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.602266f, 0.55145f, 2.50522f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_madera.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_madera.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_madera.specular);
		mLightsShader->setFloat("transparency", material_madera.transparency);

		arbol->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Pino

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.77153f, 0.564747f, 1.19079f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.132249f, 0.132249f, 0.132249f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_madera.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_madera.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_madera.specular);
		mLightsShader->setFloat("transparency", material_madera.transparency);

		pino->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Volcan

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.44178f, 0.564844f, 5.22297f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_tierra.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_tierra.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_tierra.specular);
		mLightsShader->setFloat("transparency", material_tierra.transparency);

		volcan->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Palmera

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.64993f, 0.570197f, 0.644846f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.285745f, 0.285745f, 0.285745f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_madera.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_madera.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_madera.specular);
		mLightsShader->setFloat("transparency", material_madera.transparency);

		palmera->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.77052f, 0.570197f, 2.32899f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.285745f, 0.285745f, 0.285745f));
		mLightsShader->setMat4("model", model);
		palmera->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.69197f, 0.570197f, 3.92931f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.285745f, 0.285745f, 0.285745f));
		mLightsShader->setMat4("model", model);
		palmera->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.49362f, 0.570197f, 3.92931f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.285745f, 0.285745f, 0.285745f));
		mLightsShader->setMat4("model", model);
		palmera->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.42188f, 0.570197f, 3.5905f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.285745f, 0.285745f, 0.285745f));
		mLightsShader->setMat4("model", model);
		palmera->Draw(*mLightsShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.12239f, 0.570197f, 2.35601f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.285745f, 0.285745f, 0.285745f));
		mLightsShader->setMat4("model", model);
		palmera->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//ELEMENTOS DE PLAYA: TOTAL 17 MODELOS

	//Carpa POSIBLE METAL

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.2089f, 0.569749f, 3.36616f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.898527f, 0.898527f, 0.898527f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_tela.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_tela.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_tela.specular);
		mLightsShader->setFloat("transparency", material_tela.transparency);

		carpa->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Escaleras

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		escalera->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Escaleras EMBARCADERO

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.29961f, 0.264203f, 4.4447f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		escEmb->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Faro

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.52872f, 0.579306f, 0.734825f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		faro->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Fuente

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.59529f, 0.564441f, 2.00441f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		fuente->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Mesa

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.63641f, 0.640855f, 3.36917f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_marmol.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_marmol.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_marmol.specular);
		mLightsShader->setFloat("transparency", material_marmol.transparency);

		mesa->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Pala

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.27719f, 0.231029f, 3.31957f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.470947f, 0.470947f, 0.470947f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_plastico_rojo.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_plastico_rojo.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_plastico_rojo.specular);
		mLightsShader->setFloat("transparency", material_plastico_rojo.transparency);

		pala->Draw(*mLightsShader);
	}

	glUseProgram(0);


	//************************************** ANIMACION BASICA **********************************
	//Pesca METALICO

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.1231f + barco_offset, -0.102578f, 6.20569f + barco_offsetz)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(barco_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		pesca->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//************************************** ANIMACION BASICA **********************************

	//Plato

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.63565f, 0.645532f, 3.31616f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.597466f, 0.597466f, 0.597466f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_porcelana.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_porcelana.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_porcelana.specular);
		mLightsShader->setFloat("transparency", material_porcelana.transparency);

		plato->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Red

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.73397f, 0.566191f, 2.22361f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.808916f, 0.808916f, 0.808916f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_tela.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_tela.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_tela.specular);
		mLightsShader->setFloat("transparency", material_tela.transparency);

		red->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Roca

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.71539f, 0.0f, 6.99498f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_tierra.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_tierra.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_tierra.specular);
		mLightsShader->setFloat("transparency", material_tierra.transparency);

		roca->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Salvavidas

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.81968f, 0.24103f, 5.69623f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_plastico_rojo.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_plastico_rojo.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_plastico_rojo.specular);
		mLightsShader->setFloat("transparency", material_plastico_rojo.transparency);

		salvavidas->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Silla

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.63824f, 0.604829f, 3.23271f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_marmol.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_marmol.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_marmol.specular);
		mLightsShader->setFloat("transparency", material_marmol.transparency);

		silla->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Silla plegable

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.51266f, 0.230874f, 3.75545f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.268536f, 0.268536f, 0.268536f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_marmol.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_marmol.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_marmol.specular);
		mLightsShader->setFloat("transparency", material_marmol.transparency);

		silla_plegable->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Sombrilla METAL

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.42905f, 0.304927f, 3.75986f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_tela.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_tela.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_tela.specular);
		mLightsShader->setFloat("transparency", material_tela.transparency);

		sombrilla->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Toalla

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.63992f, 0.624777f, 3.24644f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.659789f, 0.659789f, 0.659789f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_tela.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_tela.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_tela.specular);
		mLightsShader->setFloat("transparency", material_tela.transparency);

		toalla->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Valla 2

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.24238f, 0.563902f, 0.655061f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_madera.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_madera.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_madera.specular);
		mLightsShader->setFloat("transparency", material_madera.transparency);

		valla2->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Vaso

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.64185f, 0.656964f, 3.36445f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.619871f, 0.619871f, 0.619871f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_plastico_verde.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_plastico_verde.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_plastico_verde.specular);
		mLightsShader->setFloat("transparency", material_plastico_verde.transparency);

		vaso->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//HOTELES TOTAL 3 MODELOS

	//Hotel 1

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.76384f, 0.563334f, 1.39216f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		hotel1->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Hotel 2

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.60777f, 0.564852f, 2.06747f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.15295f, 1.15295f, 1.15295f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		hotel2->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Hotel 3

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.83215f, 1.08074f, 1.57556f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		hotel3->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//TIENDAS TOTAL 4 MODELOS

	//Oxxo

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.24126f, 0.564816f, 0.463295f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.609517f, 0.609517f, 0.609517f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		oxxo->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Tienda 2

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.01494f, 0.566899f, 0.396237f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.723348f, 0.723348f, 0.723348f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		tienda2->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Tienda 3

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.06516f, 0.564932f, 0.583206f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.317f, 1.317f, 1.317f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		tienda3->Draw(*mLightsShader);
	}

	glUseProgram(0);

	//Tienda 4

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.45049f, 0.563393f, 1.88437f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.540222f, 0.540222f, 0.540222f));
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		//mLightsShader->setInt("numLights", (int)gLights.size());
		/*
		mLightsShader->setInt("numLights", 1);
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}
		*/
		mLightsShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales

		mLightsShader->setVec4("MaterialAmbientColor", material_concreto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material_concreto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material_concreto.specular);
		mLightsShader->setFloat("transparency", material_concreto.transparency);

		tienda4->Draw(*mLightsShader);
	}

	glUseProgram(0);

	// Modelos para animacion procedural.

	/**/
	{
		// Activamos el shader de Phong
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.99928f, 0.580664f, 2.22111f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.016224f, 0.016224f, 0.016224f));
		proceduralShader->setMat4("model", model);
		
		/*
		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("amplitud", 2.0f);
		proceduralShader->setFloat("numpi", valorpi);
		proceduralShader->setFloat("frec", pow((2*pow(valorpi,2.0f)),-1.0f));
		proceduralShader->setFloat("angTheta", 0.0f);
		proceduralShader->setFloat("k", 0.05f);
		*/

		/*
		proceduralShader->setFloat("time", proceduralTime); //rango -12.5107554962135 a 26.9676621858219
		proceduralShader->setFloat("amplitud", 10.0f);
		proceduralShader->setFloat("numpi", valorpi);
		proceduralShader->setFloat("frec", pow((4 * pow(valorpi, 2.0f)), -1.0f));//1/4pi^2
		proceduralShader->setFloat("angTheta", -20.0f);
		proceduralShader->setFloat("k", 0.05f);
		*/
	
		proceduralShader->setFloat("time", proceduralTime); //rango -12.5107554962135 a 26.9676621858219
		proceduralShader->setFloat("amplitud", 3.0f);
		proceduralShader->setFloat("numpi", valorpi);
		proceduralShader->setFloat("frec", pow((2 * pow(valorpi, 2.0f)), -1.0f));//1/2pi^2
		proceduralShader->setFloat("angTheta", -valorpi * 0.5f);
		proceduralShader->setFloat("k", 0.05f);

		balon->Draw(*proceduralShader);

		if (proceduralTime <= 14.8044065868159f and proceduralTime >= -4.934802173098f) {    //Inicialmente fue de -20 a 20

			proceduralTime += 0.01f;

		}
		else {

			proceduralTime = -4.934802173098;

		}

	}

	glUseProgram(0);
	

	// Animacion de oceano
	
	{
		// Activamos el shader de Phong
		wavesShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));//2.0f
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		//wavesShader->setFloat("radius", 5.0f);
		//wavesShader->setFloat("height", 5.0f);

		oceano->Draw(*wavesShader);
		wavesTime += 0.01f;

	}

	glUseProgram(0);
	
	
	// Objeto animado. Modelos por keyframes

	{
		trabajador->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, position); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(2.98725f, 0.563248f, 2.11436f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, trabajador->gBones);

		// Dibujamos el modelo
		trabajador->Draw(*dynamicShader);
	}


	glUseProgram(0);

	{
		vacacionista->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, position); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(-6.13925f, 0.569451f, 2.2303f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, vacacionista->gBones);

		// Dibujamos el modelo
		vacacionista->Draw(*dynamicShader);

		//Personaje en primera persona
		model = glm::mat4(1.0f);
		model = glm::translate(model, position); // translate it down so it's at the center of the scene
		//model = glm::translate(model, glm::vec3(-7.44685f, 0.58504f, 2.27244f));
		//model = glm::translate(model, glm::vec3(FORWARD, 0.569451f, BACKWARD));
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, vacacionista->gBones);
		vacacionista->Draw(*dynamicShader);


	}


	glUseProgram(0);

	// Animación de partículas
	{
		// Activación del shader de las partículas
		particlesShader->use();
		particlesShader->setMat4("projection", projection);
		particlesShader->setMat4("view", view);

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 model;

		for (int psc = 0; psc < particlesSystem.particles.size(); psc++) {
			Particle p_i = particlesSystem.particles.at(psc);

			// Aplicamos transformaciones del modelo //
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(5.59529f, 0.564441f, 2.00441f));
			model = glm::translate(model, p_i.position); // translate it down so it's at the center of the scene 5.59529,0.564441 ,2.00441
			//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

			particlesShader->setMat4("model", model);

			// Dibujamos el modelo
			particleModel->Draw(*particlesShader);
		}

	}

	glUseProgram(0);

	// DIA Y NOCHE

	float velocidad = 0.15f;

	theta2 += velocidad;
	if (theta2 >= 360.0f) {
		theta2 -= 360.0f;
	}

	if (theta2 >= 270.0f && theta2 <= 360.0f) {

		gLights[0].Color.x = 0.6f;
		gLights[0].Color.y = 0.6f;
		gLights[0].Color.z = 0.8f;

	}
	else if (theta2 >= 0.0f && theta2 <= 60.0f) {

		gLights[0].Color.x = 1.0f;
		gLights[0].Color.y = 0.7f;
		gLights[0].Color.z = 0.6f;
	}
	else if (theta2 >= 60.0f && theta2 <= 120.0f) {

		gLights[0].Color.x = 1.0f;
		gLights[0].Color.y = 1.0f;
		gLights[0].Color.z = 1.0f;

	}
	else if (theta2 >= 120.0f && theta2 <= 180.0f) {

		gLights[0].Color.x = 1.0f;
		gLights[0].Color.y = 0.84f;
		gLights[0].Color.z = 0.6f;

	}
	else {

		gLights[0].Color.x = 0.8f;
		gLights[0].Color.y = 0.8f;
		gLights[0].Color.z = 1.0f;

	}

	float ang = glm::radians((float)theta2);
	gLights[0].Position.x = 60.0f * (float)cos(ang);
	gLights[0].Position.y = 60.0f * (float)sin(ang);

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
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
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	/*
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		door_offset += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		door_offset -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		door_rotation += 1.f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		door_rotation -= 1.f;*/

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		position = position + scaleV * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 0.18f;
		camera3rd.Position -= trdpersonOffset * 0.18f * forwardView;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position = position - scaleV * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 0.18f;
		camera3rd.Position -= trdpersonOffset * 0.18f * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacter += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 0.18f;
		camera3rd.Position -= trdpersonOffset * 0.18f * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 0.18f;
		camera3rd.Position -= trdpersonOffset * 0.18f * forwardView;
	}

	//Recorrido barco



	//Movimiento simple del baroc

	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {

		if (barco_offset < 0.0f) {//6.3

			barco_offset += 0.01f;

		}

	}

	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {

		if (barco_offset > -8.26f) {//-2.26

			barco_offset -= 0.01f;

		}

	}

	//barco_offsety

	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {

		if (barco_offsetz < 8.26f) {//6.3

			barco_offsetz += 0.01f;

		}

	}

	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {

		if (barco_offsetz > -0.645346) {//-2.26

			barco_offsetz -= 0.01f;

		}

	}

	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {

		if (barco_rotation < 180.0f) {//90.0f

			barco_rotation += 1.0f;

		}

	}

	if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {

		if (barco_rotation > -90.0f) {

			barco_rotation -= 1.0f;

		}

	}


	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		activeCamera = 0;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		activeCamera = 1;
	
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; 

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
