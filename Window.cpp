#include "Shader.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
static int window_width = 800;
static int window_height = 600;

int main()
{
	glfwInit(); //Required for intialization so that additional machine specific feature checks can be done
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //openGL version 3.3, 3 major and
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //3 minor
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Using core profile allows for smaller subset of
																   //non-deprecated, supported features
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Starting Window", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Window creation failure" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Checking if we could use GLAD to get glfw functions since we dont access them directly
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Unable to load OpenGL functions" << std::endl;
		return -1;
	}

	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //tell GLFW to call this func every time window size changes
	
	float verticesForTriangle1[] = {
		-0.25f, 0.0f, 0.0f, //left
		0.25f, 0.0f, 0.0f, //right
		0.0f, 0.25f, 0.0f, //top
	};

	float verticesForTriangle2[] =
	{
		-0.25f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, //leftvertices and color
		0.25f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, //right vertices and color
		0.0f, -0.25f, 0.0f,  0.0f, 0.0f, 1.0f  //bottom vertices and color
	};

	unsigned int VBO[2];//vertex buffer object
	unsigned int VAO[2]; //vertex array object that maintains linked VBOs;

	Shader *shaderProgram1 = new Shader("vs_triangle1.glsl", "fs_triangle1.glsl");
	Shader *shaderProgram2 = new Shader("vs_triangle1.glsl", "fs_triangle2.glsl");

	//Create Buffer Object and VAO
	//Importance of VAO:
	//The process of creating vertex info, creating a vertex buffer and binding it, as well as
	//configuring vertex attributes using glVertexAttribPointer func should be done everytime we want to draw a new object.
	//What if we had a way to cache this info in an object so that we could keep reusing it?
	//Thats where VAO comes into play.

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); //GL_ARRAY_BUFFER type is used for storing vertex attributes
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesForTriangle1), verticesForTriangle1, GL_STATIC_DRAW);
	//Generate Vertex attributes pointer which also specifies how the vertex attributes in the array should be read
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0); //Enabling reading from array at vertex attribute location 0(layout location = 0 in shader code)

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesForTriangle2), verticesForTriangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Every shader call from now on in the render loop will call this shader program object
	//Since we only use 1 program its ok to put this outside the render loop and reuse it
	//But once we use more than 1 program then we have to call this INSIDE THE RENDER LOOP

	//RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		//render loop should contain functionality in the following order:
		//input
		processInput(window);

		//rendering commands
		glClearColor(0.8, 0.2, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram1->Use();

		float timeValue = glfwGetTime();
		int uniformLocation = glGetUniformLocation(shaderProgram1->ID, "triangle1Color");
		float currentGreenValue = sin(timeValue * 3.0f) + 0.5f;
		glUniform4f(uniformLocation, 0.0f, currentGreenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shaderProgram2->Use();
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//check and call events and swap the buffers
		glfwSwapBuffers(window); //double buffering to avoid flickering
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}