//for debug stuffs
//for debug stuff in main
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Graphics/Mesh.h>
#include <Graphics/Model.h>

/*
	Things of note:

	* Texture maps must be flipped vertically in an image editor before use here. It will look weird if you dont do that bc of stbi_flip_vertically_on_load(true)
	
	* 
	
	* 

	* The type of lighting used in this demonstration can be called "phong lighting," because it is a combination of both the Phong Illumination Model and the Phong Shading Technique.
	  In the phong illumination model, a base ambient amount of light is applied to all models, then a diffuse value is calculated which gives an intensity value, and then a specular
	  value is calculated which gives the reflection of the smooth parts of a surface from a definite angle. The phong illumination model is ambient + specular + diffuse. In the 
	  Phong Shading technique, light is calculated on a per-pixel basis based on the average of the light values at each vertex on a face. The way we are able to do this is by 
	  calculating the shading on each "fragment," or pixel, in our fragment shader. We also have the creation of normal maps and specular maps, which provide the program with 
	  precalculated information with which it can base its math off of. it is pretty handy i think? Then we calculate their values based on the angle of their position with the light. 
	  You can see the actual math in the fragDefault.frag file. These maps have not been implemented *quite* yet but will be within the next day or so. 
	  Note that this is going to require our textures to have those things - don't ask me how to make them yet. . .
	 
	* anything that operates on a uniform using "glUniform" can only be used once the shader is activated.
	
	*  the suffix of "glUniform" changes based on the data type you give it. For a 4d matrix it is glUniformMatrix4fv, for a single float it is
	   glUniform1f, for a single int it is glUniform1i, for four floats like for an rgba color value or smth it is glUniform4f

	* the "position" or "coordinates" of something in a VBO are its local coords, not coords in world space.


	Terms of note: 
	
	Normals: In the calculation of the intensity of light on an object, normal vectors are vectors with a length of 1 that help us calculate how light needs to act on an object
			 You can either make these perpendicular to the surface of every triangle, called "face normals" OR you can make them perpendicular to each vertex, which is called 
			 vertex normals.The first is called "flat shading," where every triangle is clearly visible, whereas using vertex normals gives you "smooth shading." 
			 
	Model Matrix: This matrix takes local coordinates and converts them to world coords. This is the first step in giving your object proper perspective.

	View Matrix: This matrix takes the world coordinates and computers them into view coordinates, meaning points that have the same origin as camera/viewpoint.
				 This is the second step in giving your object perspective.

	Projection Matrix: This matrix takes the computed view coordinates and converts them into clip coordinates, which are coordinates that have been normalized
					   and "clipped" to fit screen view. This is what adds perspective. From here, conversion to screen coordinates happens automatically.
					   This is the final step in giving your object perspective.

	Binding: The concept of "binding" basically just means that you are telling openGL which of your objects it needs to be currently operating on.
			 It does this by essentially making your object a global variable, and then doing whatever it is that you tell it to. Care should be taken
			 to unbind something, or remove it from being the currently active global variable, when you are done using it.

	Buffering: The concept of using multiple regions of storage to prevent yourself from writing to a region that is still being used by another process. 
			   This means a lot of different things depending on the context you're using the word in, but a good example is how before things are drawn to the
			   screen buffer, they are first drawn in another buffer that is then swapped over to the screen buffer. This process is done basically everytime anything
			   is drawn in a game. A "buffer" is basically any block of memory that is to be used in this manner. 

	Index Buffer (ibo): Tells openGL in which order it should draw vertices. You do this by creating a list of indices (combinations of vertices) 
						that describe in which order openGL should draw your vertices in, then creating an IBO object that then inserts integer values
						as offsets in the VBO in order to render primitives while having to use less vertices. You don't have to use these, but it makes
						it a whole lot simpler I think. 

	Vertex Buffer Object (vbo): Where the data of your vertices is stored. It is just a buffer of memory that the gpu can access.

	Vertex Array Object (vao): An object that stores the information on the format/attributes of the vertices. In other words,
							   it is not where the vertex data is stored, but the object that describes your buffer objects. 
							   You can give them attributes such as color using glVertexAttributePointer. Basically though its just a place where
							   you store all your vertex array bindings (e.g. vertices[])

*/

//screen width/height
const unsigned int width = 800;
const unsigned int height = 800;



Vertex lightVertices[]		//these vertices make backfaces front faces and front faces backfaces. lmao 
{
	Vertex{glm::vec3( 0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3( 0.1f, -0.1f, -0.1f)},

	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},

	Vertex{glm::vec3( 0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3( 0.1f,  0.1f, -0.1f)},

	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)}
};


GLuint lightIndices[] 
{
	//front
	0, 1, 2,
	0, 2, 3,

	0, 4, 7,
	0, 7, 3,

	3, 7, 6,
	3, 6, 2,

	2, 6, 5,
	2, 5, 1,

	1, 5, 4,
	1, 4, 0,
	//back
	4, 5, 6,
	4, 6, 7
};

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
	//glFrontFace(GL_CW);

	Texture textures[] //old texture array for floor plane
	{
		//load textures for floor plane:			  				  
		Texture("Assets/Textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		//Texture("Assets/Textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) //the color channel is only red bc we only have 1 channel. white = bright black = dar

	};

	

	//this will flip all our material texture coords so that they match with openGL's winding order - i think it goes counter clockwise and stbi loads them clockwise??
	//stbi_set_flip_vertically_on_load(true);

	// Light cubes creation:		this is exactly the same as the steps above. just a new cube but with the light shader. 
	Shader lightShader("Assets/Shaders/lightVert.vert", "Assets/Shaders/lightFrag.frag"); 
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh light(lightVerts, lightInd, tex);

	//backpack			NOTE: A NEW SHADER PROGRAM MUST BE MADE FOR EACH MESH OBJECT EXCEPT FOR LIGHTS. Honestly the generation of these NEEDS to be in the model class like 
	//it is absolutely ridiculous to keep going back and doing all this code where we set the model matrix and how things interact with light etc. when it should all be
	//handled by Model. PLEASE STREAMLINE.
	Shader backpackSh("Assets/Shaders/vertDefault.vert", "Assets/Shaders/fragDefault.frag");
	Model backpack("Assets/Models/Backpack/backpack.obj");

	//Tower
	Shader towerSh("Assets/Shaders/vertDefault.vert", "Assets/Shaders/fragDefault.frag");
	Model tower("Assets/Models/Tower/testTower.obj");

	//Barrel Test obj
	Shader barrelSh("Assets/Shaders/vertDefault.vert", "Assets/Shaders/fragDefault.frag");
	Model barrel("Assets/Models/Barrel2/Barrel2.obj");

	//light info. color = rgba
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.5f, 0.0f);

	//barrel info
	glm::vec3 barrelPos = glm::vec3(-1.0f, 2.0f, 0.0f);
	glm::mat4 barrelModelM = glm::mat4(1.0f);
	barrelModelM = glm::translate(barrelModelM, barrelPos);

	//tower info
	glm::vec3 towerPos = glm::vec3(-20.0f, 2.0f, 0.0f);
	glm::mat4 towerModelM = glm::mat4(1.0f);
	towerModelM = glm::translate(towerModelM, towerPos);

	//backpack info
	glm::vec3 backpackPos = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::mat4 backpackModelM = glm::mat4(1.0f);
	backpackModelM = glm::translate(backpackModelM, backpackPos);
	
	

	//enable depth buffer so openGL knows which triangle to render on top of which
	glEnable(GL_DEPTH_TEST);
	//create camera matrix
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 2.0f));	//how big the screen is, where you want the object to start in worldspace

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
		glClearColor(0.0f, 0.0f, 0.002f, 1.0f);		//good desert color 0.702f, 0.448f, 0.102f, 1.0f
		//clear back buffer then assign new color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//tell imgui we are working on a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//set model matrix and translate every frame so we can alter the position as needed
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		//the next 14 or so lines SHOULD BE AUTOMATED BY MODEL CLASS
		//inform shader of neccesary light obj data - world pos, color. If you want to "animate" a light, you need to throw each of these inside the while loop and also change lightPos according to sin(time) in there too. 
		lightShader.activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		lightShader.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);	//these go to fragment shader
		//barrel:
		barrelSh.activate();
		glUniformMatrix4fv(glGetUniformLocation(barrelSh.ID, "model"), 1, GL_FALSE, glm::value_ptr(barrelModelM));
		barrelSh.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		barrelSh.setVec3("pointLights[0].position", lightPos.x, lightPos.y, lightPos.z);
		//Tower:
		towerSh.activate();
		glUniformMatrix4fv(glGetUniformLocation(barrelSh.ID, "model"), 1, GL_FALSE, glm::value_ptr(towerModelM));
		towerSh.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		towerSh.setVec3("pointLights[0].position", lightPos.x, lightPos.y, lightPos.z);
		//set the position in world space and calculate the color of the light on the object, as well as its local position and the lights position. done for every object in scene, and if you move something this process must be done first
		backpackSh.activate();
		glUniformMatrix4fv(glGetUniformLocation(backpackSh.ID, "model"), 1, GL_FALSE, glm::value_ptr(backpackModelM));
		backpackSh.setVec4("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		backpackSh.setVec3("pointLights[0].position", lightPos.x, lightPos.y, lightPos.z);


		//update inputs and camera matrix, then export it to vertex shader
		if (!io.WantCaptureMouse)
		{

			camera.Inputs(window);
		}
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		light.Draw(lightShader, camera);
		backpack.Draw(backpackSh, camera);
		tower.Draw(towerSh, camera);
		barrel.Draw(barrelSh, camera);

		//imgui stuff begins AFTER everything is drawn and updated for the frame.
		//Make imgui window:
		ImGui::Begin("Debug");
		ImGui::Text("Light Stuff: ");
		ImGui::SliderFloat4("lightColor", glm::value_ptr(lightColor), 1.0f, 0.0f);
		ImGui::SliderFloat3("lightPos", glm::value_ptr(lightPos), 10.0f, 0.0f);
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
	//shaderProgram.Delete();
	//lightShader.Delete();
	//plane2.Delete();
	barrelSh.Delete();
	towerSh.Delete();
	backpackSh.Delete();
	//delete window
	glfwDestroyWindow(window);
	//terminate GLFW:
	glfwTerminate();
	return 0;
}