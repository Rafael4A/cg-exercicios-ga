#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Common/include/Shader.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupGeometry();

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

bool rotateX = false;
bool rotateY = false;
bool rotateZ = false;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraSpeed = 0.05;

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Atividades camera", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("../../Common/shaders/camera.vs", "../../Common/shaders/camera.fs");

	GLuint VAO = setupGeometry();

	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1);
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	// Matriz de view: posição e orientação da câmera
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

	// Matriz de projeção perspectiva: definindo o volume de visualização (frustum)
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1); 

		if (rotateX) {
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateY) {
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (rotateZ) {
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
		
		// Atualiza posicao e orientacao da camera
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 42);
		
		glDrawArrays(GL_POINTS, 0, 42);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Visao da frente
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0, 0.0, 3.0); 
		cameraFront = glm::vec3(0.0, 0.0, -1.0);
	}

	// Visao de tras
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0, 0.0, -3.0);
		cameraFront = glm::vec3(0.0, 0.0, 1.0);
	}

	// Visao da esquerda
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraPos = glm::vec3(-3.0f, 0.0f, 0.0f);
		cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	// Visao da direita
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraPos = glm::vec3(3.0f, 0.0f, 0.0f);
		cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	//Visao do topo
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		cameraPos = glm::vec3(0.0f, 3.0f, 0.0f);
		cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
		cameraUp = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	float cameraSpeed = 0.05;

	if (key == GLFW_KEY_W) {
		cameraPos += cameraFront * cameraSpeed;
	}
	if (key == GLFW_KEY_S) {
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (key == GLFW_KEY_A) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_D) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

int setupGeometry() {

	GLfloat vertices[] = {

		// Base da pirâmide: 2 triângulos
		// x    y    z    r    g    b
		-0.5, -0.5, -0.5, 0.0, 0.0, 1.0, // Ponto A (azul)
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0, // Ponto B (verde)
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Ponto C (vermelho)

		0.5, -0.5,  0.5, 1.0, 1.0, 0.0, // Ponto D (amarelo)
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0, // Ponto B (verde)
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Ponto C (vermelho)

		// Topo do cubo: 2 triângulos
		// x    y    z    r    g    b
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0, // Ponto R (ciano)
		0.5, 0.5, -0.5, 0.0, 0.0, 0.0, // Ponto S (preto)
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0, // Ponto T (roxo)

		-0.5, 0.5,  -0.5, 1.0, 1.0, 0.0, // Ponto U (amarelo)
		0.5, 0.5, -0.5, 0.0, 0.0, 0.0, // Ponto S (preto)
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0, // Ponto T (roxo)

		// Lado 1
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0, // Ponto R (ciano)
		0.5, -0.5,  0.5, 1.0, 1.0, 0.0, // Ponto D (amarelo)
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Ponto C (vermelho)

		0.5, 0.5, -0.5, 0.0, 0.0, 0.0, // Ponto S (preto)
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Ponto C (vermelho)
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0, // Ponto R (ciano)

		// Lado 2
		0.5, 0.5, -0.5, 0.0, 0.0, 0.0, // Ponto S (preto)
		-0.5, 0.5,  -0.5, 1.0, 1.0, 0.0, // Ponto U (amarelo)
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Ponto C (vermelho)

		-0.5, 0.5,  -0.5, 1.0, 1.0, 0.0, // Ponto U (amarelo)
		-0.5, -0.5, -0.5, 0.0, 0.0, 1.0, // Ponto A (azul)
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0, // Ponto C (vermelho)

		// Lado 3
		0.5, -0.5,  0.5, 1.0, 1.0, 0.0, // Ponto D (amarelo)
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0, // Ponto B (verde)
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0, // Ponto R (ciano)

		0.5, 0.5, 0.5, 0.0, 1.0, 1.0, // Ponto R (ciano)
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0, // Ponto B (verde)
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0, // Ponto T (roxo)

		// Lado 4
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0, // Ponto B (verde)
		-0.5, -0.5, -0.5, 0.0, 0.0, 1.0, // Ponto A (azul)
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0, // Ponto T (roxo)

		-0.5, 0.5,  -0.5, 1.0, 1.0, 0.0, // Ponto U (amarelo)
		-0.5, -0.5, -0.5, 0.0, 0.0, 1.0, // Ponto A (azul)
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0, // Ponto T (roxo)

		// Chao
		-5.0, -0.5, -5.0, 0.5, 0.5, 0.5,
		-5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		5.0, -0.5, -5.0, 0.5, 0.5, 0.5,

		-5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		5.0, -0.5, -5.0, 0.5, 0.5, 0.5
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}
