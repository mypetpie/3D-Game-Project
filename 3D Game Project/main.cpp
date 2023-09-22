//for debug stuffs
//for debug stuff in main
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//from 0,0: z is forward and back, x is left to right. y is up and down. 

//game objects in scene. Eventually "Model" will be contained within an entity that has a physics collider and other things associated with it. 
#include <Graphics/Model.h>
#include <Game Objects/Lights/PointLight.h>
#include <Game Objects/Terrain/TerrainObject.h>

// screen width/height. Note that because the normalization of the camera's orientation vector is based on assuming the window is a square, 
// if the resolution of the game is not a square, you will spin around.
const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
	//initialize glfw
	glfwInit();

	//tell glfw what version we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		//core profile = only modern commands


	//create window - width, height, name. first null = not fullscreen. 
	GLFWwindow* window = glfwCreateWindow(width, height, "Ramona's 4th Foray Into 3D Graphics using OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::wclog << "Failed to create GLFW window. OnO" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//load GLAD. Allows us to use openGL
	gladLoadGL();

	//define viewport: top left to bottom right to match the way screens write
	glViewport(0, 0, width,height);

	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW); //for terrain to work this has to be on but it loads models inside out now! models use GL_CCW winding order. I think this could be fucking up the textures too.

	//this will flip all our material texture coords so that they match with openGL's winding order - i think it goes counter clockwise and stbi loads them clockwise??
	//stbi_set_flip_vertically_on_load(true);

	//light cube. im pretty sure without one of these you can see nothing, so naturally it has its own thing now as like a wrapper basically. 
	pointLight light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	TerrainObject terrain("Assets/Heightmaps/TestWorld/TestWorld.png", "Assets/Heightmaps/TestWorld/diffuse.png"); //renders start at around like y 10 for some reason maybe cause of the scale?
	//backpack
	Model backpack("Assets/Models/Backpack/backpack.obj");
	backpack.setPos(glm::vec3(0.0f, 10.0f, 0.0f));

	//Tower
	Model tower("Assets/Models/Tower/testTower.obj");
	tower.setPos(glm::vec3(1.0f, 10.0f, 30.0f));

	//Barrel Test obj
	//Model barrel("Assets/Models/Barrel2/Barrel2.obj");
	//barrel.setPos(glm::vec3(-1.0f, 2.0f, 0.0f));
	
	//enable depth buffer so openGL knows which triangle to render on top of which
	glEnable(GL_DEPTH_TEST);
	//create camera matrix
	Camera camera(width, height, glm::vec3(0.0f, 32.0f, 13.0f));	//how big the screen is, where you want the object to start in worldspace

	//create imgui context, a variable to deal with inputs and outputs, choose the style, and then initialize imgui for GLFW and OpenGL3 to tell it what window to use and what version glsl we use
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//instruct back-end to not alter mouse cursor. leave this responsibility to input functions.
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::StyleColorsDark();
	//take glfw window, set the initialization to true(?), and tell compiler what version of glsl we use in our program.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//game loop:
	while (!glfwWindowShouldClose(window))
	{
		//background color
		glClearColor(0.0f, 0.0f, 0.6f, 1.0f);		//good desert color 0.702f, 0.448f, 0.102f, 1.0f
		//clear back buffer then assign new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//tell imgui we are working on a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//update inputs and camera matrix, then export it to vertex shader
		if (!io.WantCaptureMouse)
		{

			camera.Inputs(window);
		}
		camera.updateMatrix(45.0f, 0.1f, 300.0f);

		light.Draw(camera);
		terrain.Draw(camera);
		backpack.Draw(camera, light.modelPos, light.color);
		tower.Draw(camera, light.modelPos, light.color);

		//barrel.Draw(camera, light.modelPos, light.color);

		//imgui stuff begins AFTER everything is drawn and updated for the frame. It draws to the back buffer and then you swap buffers !
		//Make imgui window:
		ImGui::Begin("Debug");
		ImGui::Text("Light Stuff: ");
		ImGui::SliderFloat4("lightColor", glm::value_ptr(light.color), 1.0f, 0.0f);
		ImGui::SliderFloat3("lightPos", glm::value_ptr(light.modelPos), 40.0f, 0.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);	//always swap buffers after drawing or doing something to an object. You won't see anything happen otherwise !!

		//process events from computer 
		glfwPollEvents();
	}

	//destroy imgui stuff and shut it down
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	//delete objects bc end of program:	
	light.Delete();
	//backpack.delModelSh();
	//tower.delModelSh();
	//barrel.delModelSh();
	//delete window
	glfwDestroyWindow(window);
	//terminate GLFW:
	glfwTerminate();
	return 0;
}