#include<iostream>
#include<cmath>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<SOIL.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"shader.h"
#include"camera.h"
//公共参数
bool keys[1024];
const GLuint WIDTH = 1600, HEIGHT = 900;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH/2.0, lastY = HEIGHT/2.0;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLuint circle = 0;
bool firstMouse = true;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)//按键反馈
{
	
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void do_movement()//摄像机移动
{
	// 摄像机控制
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		circle = 1;
	if (circle)
		camera.ProcessKeyboard(JUMP, deltaTime);
	if (camera.Position.y == 0)
		circle = 0;
		//camera.ProcessKeyboard(JUMP, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)//鼠标反馈
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()//主函数
{
	//图形顶点
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	GLuint VBO, VAO;

	//图形位置
	/*
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};*/
	GLfloat f = -15.0f;
	glm::vec3 cubePositions[] = {
	glm::vec3(-1.0f,  0.0f,  f),
	glm::vec3(-1.0f,  1.0f,  f),
	glm::vec3(-1.0f,  2.0f,  f),
	glm::vec3(-1.0f,  3.0f,  f),
	glm::vec3(-1.0f,  4.0f,  f),
	glm::vec3(-1.0f,  5.0f,  f),
	glm::vec3(-1.0f,  6.0f,  f),
	glm::vec3(-2.0f,  5.0f,  f),
	glm::vec3(-3.0f,  4.0f,  f),
	glm::vec3(-3.0f,  3.0f,  f),
	glm::vec3(-4.0f,  5.0f,  f),
	glm::vec3(-5.0f,  6.0f,  f),
	glm::vec3(-5.0f,  5.0f,  f),
	glm::vec3(-5.0f,  4.0f,  f),
	glm::vec3(-5.0f,  3.0f,  f),
	glm::vec3(-5.0f,  2.0f,  f),
	glm::vec3(-5.0f,  1.0f,  f),
	glm::vec3(-5.0f,  0.0f,  f),
	glm::vec3(-5.0f,  6.0f,  f),
	glm::vec3(2.0f,  6.0f,  f),
	glm::vec3(3.0f,  6.0f,  f),
	glm::vec3(4.0f,  6.0f,  f),
	glm::vec3(1.0f,  5.0f,  f),
	glm::vec3(5.0f,  5.0f,  f),
	glm::vec3(1.0f,  4.0f,  f),
	glm::vec3(1.0f,  3.0f,  f),
	glm::vec3(1.0f,  2.0f,  f),
	glm::vec3(1.0f,  1.0f,  f),
	glm::vec3(5.0f,  1.0f,  f),
	glm::vec3(2.0f,  0.0f,  f),
	glm::vec3(3.0f,  0.0f,  f),
	glm::vec3(4.0f,  0.0f,  f),
	glm::vec3(0.0f,  0.0f,  0.0f)
	};
	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Demo 1", NULL, NULL);
	
	if (window == NULL)
	{
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		return -1;
	}
	//调节glfw参数
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//初始化glew
	glewExperimental=GL_TRUE; 
	glewInit();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, WIDTH, HEIGHT);
	Shader shader("vertex.txt", "fragment.txt");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//纹理设置与加载
	GLuint texture,ntexture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int w, h;
	unsigned char* image = SOIL_load_image("redstone_lamp_on.png", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &ntexture);
	glBindTexture(GL_TEXTURE_2D, ntexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("b.jpg", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	while (!glfwWindowShouldClose(window))//渲染循环
	{
		//调节刷新率
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//事件检测
		glfwPollEvents();
		do_movement();

		//设置背景
		glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//导入纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ntexture);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture2"), 1);
		
		shader.Use();

		//画面计算
		glm::mat4 view(1);
		glm::mat4 projection(1);
		glm::mat4 model(1);
		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT,0.1f, 50.0f);
		//projection = glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f, 0.1f, 100.0f);
		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");
			
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 33; i++)
		{
			//模型变换
			model = glm::mat4(1);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;// +glfwGetTime() * 2.0f;
			if(i==32)
			model = glm::rotate(model, (float)glfwGetTime() * 2.0f, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//绘制
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		model = glm::mat4(1);
		//model = glm::translate(model, camera.Position+glm::vec3(0.3f,  -0.3f,  -0.5f));
		GLfloat Rad = 0.6f;
		GLfloat SetX = cos(glm::radians(45.0f)) * Rad * cos(glm::radians(30.0f));
		GLfloat SetZ = sin(glm::radians(45.0f)) * Rad * cos(glm::radians(30.0f));
		GLfloat SetY = sin(glm::radians(-30.0f)) * Rad;
		model = glm::translate(model, camera.Position + glm::vec3(
			cos(glm::radians(camera.Yaw))*SetX- sin(glm::radians(camera.Yaw))*SetZ,
			cos(glm::radians(camera.Pitch))*SetY + sin(glm::radians(camera.Pitch)) * SetZ,
			sin(glm::radians(camera.Yaw))*SetX+ cos(glm::radians(camera.Yaw))*SetZ));
		model = glm::rotate(model, glm::radians(camera.Pitch), glm::vec3(cos(glm::radians(camera.Yaw)+90.0f), 0.0f, sin(glm::radians(camera.Yaw)+90.0f)));
		model = glm::rotate(model, glm::radians(-camera.Yaw-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.49f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindVertexArray(0);  

		glfwSwapBuffers(window);
	}
	//清空内存
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}