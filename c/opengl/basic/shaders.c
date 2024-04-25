#include <SDL2/SDL.h>
#include <glad/glad.h>

///// Macros

#define WIN_W 800
#define WIN_H 600

///// Enum

enum {
	SHADER_BASIC,
	SHADER_SINE,
	SHADER_RGB
};

///// Globals

SDL_Window *g_Window;
SDL_GLContext *g_GLContext;

unsigned int g_VBO, g_VAO;
unsigned int g_BasicProgram, g_SineProgram, g_RGBProgram;

float g_Vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

///// Functions

int Init();
void Close();

char *LoadGLSL(const char *path);
unsigned int CreateShaderProgram(const char *vertex_shader_src, const char *fragment_shader_src);
unsigned int CreateShaderProgramFromFiles(const char *vertex_shader_path, const char *fragment_shader_path);

int main()
{
	// Variables
	SDL_Event event;
	int loop = Init();
	int selection = SHADER_BASIC;
	float time = 0.0f;

	while (loop)
	{
		///// Events

		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) { loop = 0; }
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) { selection--; }
				else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) { selection++; }

				if (selection < SHADER_BASIC) { selection = SHADER_RGB; }
				if (selection > SHADER_RGB) { selection = SHADER_BASIC; }
			}
		}

		///// Update

		// Get Time
		time = (float)SDL_GetTicks() / 1000.0f;

		// Shader Programs
		switch (selection)
		{
			case SHADER_BASIC:
				glUseProgram(g_BasicProgram);
				break;
			case SHADER_SINE:
				int vertex_color_location = glGetUniformLocation(g_SineProgram, "color");
				glUseProgram(g_SineProgram);
				glUniform4f(vertex_color_location, 0.0f, SDL_sin(time), 0.0f, 1.0f);
				break;
			case SHADER_RGB:
				glUseProgram(g_RGBProgram);
				break;
		}

		///// Rendering

		// Clear screen
		glClearColor(0.1f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind VAO
		glBindVertexArray(g_VAO);

		// Draw Triangle
		glDrawArrays(GL_TRIANGLES,0 ,3);

		// Update to Screen
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
	g_BasicProgram	= CreateShaderProgramFromFiles("glsl/v_basic.glsl", "glsl/f_basic.glsl");
	g_SineProgram	= CreateShaderProgramFromFiles("glsl/v_sine.glsl", "glsl/f_sine.glsl");
	g_RGBProgram	= CreateShaderProgramFromFiles("glsl/v_rgb.glsl", "glsl/f_rgb.glsl");

	// Vertex Specification.
	glGenVertexArrays(1, &g_VAO);	// Vertex Attribute Object
	glGenBuffers(1, &g_VBO);		// Vertex Buffer Object

	glBindVertexArray(g_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertices), g_Vertices, GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unset bindings
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 1;
}

void Close()
{
	// Cleanup OpenGL
	glDeleteVertexArrays(1, &g_VAO);
	glDeleteBuffers(1, &g_VBO);
	glDeleteProgram(g_BasicProgram);
	glDeleteProgram(g_SineProgram);
	glDeleteProgram(g_RGBProgram);

	// Cleanup and Quit SDL
	SDL_DestroyWindow(g_Window);
	SDL_GL_DeleteContext(g_GLContext);

	SDL_Quit();
}

char *LoadGLSL(const char *path)
{
	// Variables
	FILE *file;
	size_t file_size;
	char *shader_src;

	// Open file
	file = fopen(path, "rb");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Couldn't open %s.\n", path);
		return NULL;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	// Allocate memory for shader source code
	shader_src = calloc(file_size + 1, sizeof(char)); // The size + 1 is for the NULL character

	// Store shader source code
	fread(shader_src, sizeof(char), file_size, file);

	// Close File
	fclose(file);

	return shader_src;
}

unsigned int CreateShaderProgram(const char *vertex_shader_src, const char *fragment_shader_src)
{
	// Variables
	unsigned int vertex_shader, fragment_shader, shader_program;
	int success;
	char log[512];

	// Check src pointers
	if (vertex_shader_src == NULL)
	{
		fprintf(stderr, "Error: vertex_shader_src is NULL.");
		return 0;
	}

	if (fragment_shader_src == NULL)
	{
		fprintf(stderr, "Error: fragment_shader_src is NULL.");
		return 0;
	}

	// Compile Vertex Shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const char**)&vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, log);
		fprintf(stderr, "Error: Vertex Shader Compilation Failed: %s\n", log);
		return 0;
	}

	// Compile Fragment Shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const char**)&fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, log);
		fprintf(stderr, "Error: Vertex Shader Compilation Failed: %s\n", log);
		return 0;
	}

	// Create Shader Program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, log);
		fprintf(stderr, "Error: Shader Program Link Failed: %s\n", log);
		return 0;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

unsigned int CreateShaderProgramFromFiles(const char *vertex_shader_path, const char *fragment_shader_path)
{
	// Variables
	char *vertex_shader_src, *fragment_shader_src;
	unsigned int shader_program;

	// Load GLSL shader files
	vertex_shader_src	= LoadGLSL(vertex_shader_path);
	fragment_shader_src	= LoadGLSL(fragment_shader_path);

	// Compile Shaders
	shader_program = CreateShaderProgram(vertex_shader_src, fragment_shader_src);

	// Free dynamic arrays containing the shaders' source code
	free(vertex_shader_src); 
	free(fragment_shader_src); 

	return shader_program;
}

