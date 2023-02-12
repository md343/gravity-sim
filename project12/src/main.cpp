#define NUM_PARTICLES 1024*16
#define WORK_GROUP_SIZE 1024
# define M_PI           3.14159265358979323846

#include"glad/glad.h"
#include<GLFW/glfw3.h>
#include<math.h>
#include<iostream>
//#include<GL/glew.h>
#include<stdio.h>
#include<stdlib.h>

//#include<GL/gl.h>
//#include<GL/glext.h>
#include<time.h>
#include<shaderClass.h>
#include<computeClass.h>
#include<VAO.h>
#include<VBO.h>
#include<EBO.h>
#include<SSBO.h>
#include<texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//using namespace std;
const unsigned int width = 1024;
const unsigned int height = 1024;


GLfloat vertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

struct pos
{
    glm::vec4 position;
	glm::vec4 velocity;
	glm::vec4 mass;
};

float prevTime = 0.0;
float crntTime = 0.0;
float timeDiff;

int main()
{
    srand (time(NULL));

	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Compute Shader Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, width, height);

	VAO VAO1;

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(0, 3, GL_FLOAT, 0);
	VAO1.LinkAttrib(1, 2, GL_FLOAT, 3 * sizeof(float));

	VAO1.LinkVertBuffer(VBO1, 5 * sizeof(GLfloat));
	VAO1.LinkElemBuffer(EBO1);

	Texture Tex0 = Texture(width, height);

	SSBO AgentSBO(NUM_PARTICLES, sizeof(struct pos));

	std::cout << NUM_PARTICLES << std::endl;
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    struct pos *points = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(struct pos), bufMask);
    float dist;
	float ang;
    for (int i = 0; i<NUM_PARTICLES;i++)
    {
		ang = ((float)(rand() % 10000)/5000 - 1)*M_PI;
		dist = ((float)(rand() % 10000)/10000)*0.5+0.0001;
        // points[i].position.x = ((float)(rand() % 10000)/5000 - 1)*0.5;
        // points[i].position.y = ((float)(rand() % 10000)/5000 - 1)*0.5;
        points[i].position.x = dist*cos(ang);
		points[i].position.y = dist*sin(ang);
		points[i].position.z = 1;



        points[i].position.w = 1;
		glm::vec3 direction = glm::cross(glm::vec3(-points[i].position.x, -points[i].position.y ,0.0f), glm::vec3(0.0f, 0.0f ,1.0f) );

		points[i].velocity.x = direction.x*dist/10;//((float)(rand() % 100)/50 - 1) * 0.005;
        points[i].velocity.y = direction.y*dist/10;//((float)(rand() % 100)/50 - 1) * 0.005;
		
		points[i].mass.x = 1;

		// if (i == 0) {
		// 	points[i].mass.x = 100;
		// 	points[i].velocity.x = 0;
		// 	points[i].velocity.y = 0;
		// }
        //points[i].velocity.x = (float) (i - NUM_PARTICLES/2)/100;
        //points[i].velocity.y = (float) (i - NUM_PARTICLES/2)/100;
        //points[i].position.z = 0;
        //points[i].position.w = 1;
		//points[i].mass = 1.0f;//min(exp((rand() % 5 + 1 ))/1500, 0.1);
		//std::cout << points[i].mass << endl;
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, AgentSBO.ID);
	AgentSBO.Bind(1);

	Shader vfShader = Shader("res/shaders/default.vert", "res/shaders/default.frag");

	Compute texCompute = Compute("res/shaders/texture.comp");
	Compute agentCompute = Compute("res/shaders/agent.comp");



    while (!glfwWindowShouldClose(window))
    {
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		prevTime = crntTime;
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		agentCompute.Activate();
		AgentSBO.Bind(1);
		glUniform1i(glGetUniformLocation(agentCompute.ID, "PARTICLES"), NUM_PARTICLES);
		glUniform1f(glGetUniformLocation(agentCompute.ID, "dt"), timeDiff);
		agentCompute.Dispatch(ceil(NUM_PARTICLES / WORK_GROUP_SIZE), 1, 1);

		glBindBuffer(GL_ARRAY_BUFFER, AgentSBO.ID);
		glVertexPointer(4, GL_FLOAT, sizeof(struct pos), (void *)0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// texCompute.Activate();
		// glUniform1i(glGetUniformLocation(texCompute.ID, "PARTICLES"), NUM_PARTICLES);
		// texCompute.Dispatch(ceil(width / 8), ceil(height / 4), 1);
		// vfShader.Activate();

		// Tex0.Bind(0);
		// glUniform1i(glGetUniformLocation(vfShader.ID, "screen"), 0);
		// VAO1.Bind();
		// glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
    }   

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	Tex0.Delete();

	AgentSBO.Delete();
	vfShader.Delete();
	texCompute.Delete();
	agentCompute.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

// glBindBuffer(GL_ARRAY_BUFFER, posBO);
// glVertexPointer(4, GL_FLOAT, 0, (void *)0);
// glEnableClientState(GL_VERTEX_ARRAY);
// glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
// glDisableClientState(GL_VERTEX_ARRAY);
// glBindBuffer(GL_ARRAY_BUFFER, 0);