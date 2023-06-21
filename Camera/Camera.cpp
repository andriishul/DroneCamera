// Camera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
using namespace std;

class shader
{
private:
	unsigned programID;
	const char* fragmentSource, * vertexSource;

public:
	shader() :programID{(unsigned) - 1} {}
	//fragment,vertex
	shader(const char* _fragmentSource, const char* _vertexSource) :fragmentSource(_fragmentSource)
		, vertexSource(_vertexSource), programID(-1)
	{
		const char* err = attach();
		if (err) {
			fprintf(stderr, "Error %s\n", err);
			assert(0);
		}


	}
	//returns null if successfully completed
	const char* attach() {

		int success;
		programID = glCreateProgram();


		auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char* errorMsg = new char[800];
			char* msg = new char[800];
			glGetShaderInfoLog(vertexShader, 800, nullptr, errorMsg);

			sprintf_s(msg, 800, "VERTEX_SHADER COMPILATION ERROR\n%s\n", errorMsg);
			delete[]errorMsg;
			return msg;
		}

		auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char* errorMsg = new char[800];
			char* msg = new char[800];
			glGetShaderInfoLog(fragmentShader, 800, nullptr, errorMsg);

			sprintf_s(msg, 800, "FRAGMENT_SHADER COMPILATION ERROR\n%s\n", errorMsg);
			delete[]errorMsg;
			return msg;
		}

		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		glad_glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char* errorMsg = new char[800];
			char* msg = new char[800];
			glGetProgramInfoLog(programID, 800, nullptr, errorMsg);

			sprintf_s(msg, 800, "PROGRAM LINKING ERROR\n%s\n", errorMsg);
			delete[]errorMsg;
			return msg;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return nullptr;
	}
	void use() {
		if (programID == -1)
			assert(0);/*shader isnot compiled*/
		glUseProgram(programID);
	}
	unsigned& ProgramId() { return programID; }
};



class Floor
{
private:
	unsigned vao;

	void setup_mesh()
	{

		float planeVertices[] = {
			// positions            // normals         // texcoords
			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1,

			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  1,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1,
			 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  1, 1
		};
		unsigned vbo;
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);



		assert(!glGetError());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof planeVertices, planeVertices, GL_STATIC_DRAW);
		assert(!glGetError());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float_t), 0);

		assert(!glGetError());
		glEnableVertexAttribArray(0);


		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float_t), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		assert(!glGetError());

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float_t), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		assert(!glGetError());

	}
public:
	Floor()
	{
		setup_mesh();
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="s">:floor shader</param>
	/// <param name="id">:texture</param>
	void Render(shader& s, std::vector<unsigned> id = {})
	{
		s.use();
		glBindVertexArray(vao);
		for (int i = 0; i < id.size(); i++)
			if (id[i] != -1) {
				glActiveTexture(GL_TEXTURE0 + 75 + i);
				glBindTexture(GL_TEXTURE_2D, id[i]);
			}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

	}
};

glm::vec2 GetResolution(GLFWwindow* &window) 
{
	int width, height;
	glfwGetWindowSize(window,&width,&height);
	return { width,height };
}

int main()
{
	cam::Camera c{ glm::vec3{0,0,9},glm::vec3{0,2,0} };
	c.rotate(0,10);
	auto pitch = c.pitch();
	
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return 0;
	/* Loop until the user closes the window */

	shader s = { "#version 430 core\nvoid main(){gl_FragColor = vec4(gl_SampleID*0.1,gl_PrimitiveID,gl_FragCoord.x,1.0);}","#version 430 core\nlayout(location = 0)in vec3 aPos;\nlayout(location = 1)in vec3 aNormal;\nlayout(location = 2)in vec2 aTexCoords;\nuniform mat4 model;\nuniform mat4 proj;\nuniform mat4 view;\nvoid main(){gl_Position = proj * view * model* vec4(aPos,1.0);}" };
	Floor floor;
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			c.translate(c.forward() * 0.01f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			c.translate(c.forward() * -0.01f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			c.translate(c.right() * 0.01f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			c.translate(c.right() * -0.01f);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			c.rotate(-3.f, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			c.rotate(3.f, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			c.rotate(0, 3);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			c.rotate(0, -3);
		}


		glViewport(0,0,GetResolution(window).x,GetResolution(window).y);
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Swap front and back buffers */
		s.use();
		glUniformMatrix4fv(glGetUniformLocation(s.ProgramId(),"proj"),1,false,&glm::perspective(glm::radians(45.f),1920.f/1080.f,0.1f,100.f)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(s.ProgramId(), "view"), 1, false, &(*c)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(s.ProgramId(), "model"), 1, false, &glm::mat4{ 1.f }[0][0]);
		floor.Render(s);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
