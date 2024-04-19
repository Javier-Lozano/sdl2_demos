#include <SDL2/SDL.h>
#include <glad/glad.h>

///// Macros

#define WIN_W 800
#define WIN_H 600

///// Globals

SDL_Window *g_Window;
SDL_GLContext *g_GLContext;

unsigned int g_VBO, g_VAO, g_ShaderProgram;

float g_Vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

const char *g_VertexShaderSrc =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}";

const char *g_FragmentShaderSrc =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(0.5, 0.7, 0.8, 1.0);\n"
	"}";

///// Functions

int Init();
void Close();

int main()
{
	// Variables
	SDL_Event event;
	int loop = Init();

	while (loop)
	{
		///// Events

		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) { loop = 0; }
		}

		///// Rendering

		// Clear screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(g_ShaderProgram);
		glBindVertexArray(g_VAO);

		glDrawArrays(GL_TRIANGLES,0 ,3);

		SDL_GL_SwapWindow(g_Window);
	}

	Close();

	return 0;
}

int Init()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Couldn't initialize SDL. %s\n", SDL_GetError());
		return 0;
	}
	
	// Create Window
	g_Window = SDL_CreateWindow("Triangle [SDL2 OpenGL]", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_OPENGL);
	if (g_Window == NULL)
	{
		fprintf(stderr, "Couldn't create a Window. %s\n", SDL_GetError());
		return 0;
	}

	// Setup attributes and create context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	g_GLContext = SDL_GL_CreateContext(g_Window);
	if (g_GLContext == NULL)
	{
		fprintf(stderr, "Couldn't create an OpenGL context. %s\n", SDL_GetError());
		return 0;
	}

	// Load extensions trough GLAD
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		fprintf(stderr, "Couldn't initialize GLAD.\n");
		return 0;
	}

	// Set Viewport
	glViewport(0, 0, WIN_W, WIN_H);

	// Compile Shaders
	unsigned int vertex_shader, fragment_shader;
	int success = 0;
	char log[512];

	// Vertex Shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &g_VertexShaderSrc, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, log);
		fprintf(stderr, "Error: Vertex Shader Compilation Failed: %s\n", log);
		return 0;
	}

	// Fragment Shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &g_FragmentShaderSrc, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, log);
		fprintf(stderr, "Error: Fragment Shader Compilation Failed: %s\n", log);
		return 0;
	}

	// Link Shaders
	g_ShaderProgram = glCreateProgram();
	glAttachShader(g_ShaderProgram, vertex_shader);
	glAttachShader(g_ShaderProgram, fragment_shader);
	glLinkProgram(g_ShaderProgram);
	glGetProgramiv(g_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(g_ShaderProgram, 512, NULL, log);
		fprintf(stderr, "Error: Shader Program Link Failed: %s\n", log);
		return 0;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Vertex Specification.
	glGenVertexArrays(1, &g_VAO);	// Vertex Attributes
	glGenBuffers(1, &g_VBO);		// Vertex Data

	glBindVertexArray(g_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertices), g_Vertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Unset bindings
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 1;
}

void Close()
{
	// Cleanup OpenGL


	// Cleanup and Quit SDL
	SDL_DestroyWindow(g_Window);
	SDL_GL_DeleteContext(g_GLContext);

	SDL_Quit();
}
