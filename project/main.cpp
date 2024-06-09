#include "SDLApplication.h"
#include "OpenGLBindings.h"

#include <stdio.h>

SDLWindow* window;

// Shader sources
#ifdef GLES3_API
const char* vertexSource =
    "attribute vec4 position;    \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = vec4(position.xyz, 1.0);  \n"
    "}                            \n";
const char* fragmentSource =
    "precision mediump float;\n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4 (1.0, 1.0, 1.0, 1.0 );\n"
    "}\n";
#else
const char* vertexSource =
    "#version 120           \n"
    "attribute vec3 position;  \n"
    "void main()               \n"
    "{                         \n"
    "   gl_Position = vec4(position, 1.0); \n"
    "}                         \n";

const char* fragmentSource =
    "#version 120           \n"
    "void main()                 \n"
    "{                           \n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.75, 1.0); \n"
    "}                           \n";
#endif

void drawTriangle() {
    int vao = gl_create_vertex_array();
	gl_bind_vertex_array(vao);

	int vbo = gl_create_buffer();
	gl_bind_buffer(GL_ARRAY_BUFFER, vbo);

	float vertices[] = {
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	gl_buffer_data(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create + Compile vertex shader
	int vertexShader = gl_create_shader(GL_VERTEX_SHADER);
	gl_shader_source(vertexShader, vertexSource);
	gl_compile_shader(vertexShader);
	gl_get_shader_info_log(vertexShader);

    // Create + Compile fragment shader
	int fragmentShader = gl_create_shader(GL_FRAGMENT_SHADER);
	gl_shader_source(fragmentShader, fragmentSource);
	gl_compile_shader(fragmentShader);
	gl_get_shader_info_log(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	int shaderProgram = gl_create_program();
	gl_attach_shader(shaderProgram, vertexShader);
	gl_attach_shader(shaderProgram, fragmentShader);
	gl_link_program(shaderProgram);
    gl_get_shader_info_log(shaderProgram);

	gl_use_program(shaderProgram);

	// Specify the layout of the vertex data
	int posAttrib = gl_get_attrib_location(shaderProgram, "position");
	gl_enable_vertex_attrib_array(posAttrib);
	gl_vertex_attrib_pointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0);

	gl_bind_vertex_array(0);
}

void drawTriangleLoop(RenderEvent* event) {
    // printf("Drawing Triangle..\n");

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

	window->ContextFlip();
}

int main() {
    printf("Running Application..\n");

	SDLApplication* app;
	RenderEvent::callback = drawTriangleLoop;

	{
		int flags = 0;
		flags |= WINDOW_FLAG_HARDWARE;
		flags |= WINDOW_FLAG_COLOR_DEPTH_32_BIT;
		flags |= WINDOW_FLAG_DEPTH_BUFFER;
		flags |= WINDOW_FLAG_STENCIL_BUFFER;
		flags |= WINDOW_FLAG_VSYNC;

		app = new SDLApplication();
		window = new SDLWindow(app, 800, 600, flags, "Window Title");
	}

	drawTriangle();
	app->Exec();
	window->Close();

	delete app;
	delete window;

    return 0;
}
