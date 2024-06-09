#pragma once

#include "OpenGL.h"

#include <string>

// Let's mimic Lime's actual implementation, why? Because it's a good idea to do so

int gl_create_vertex_array() {
    GLuint vao = 0;

    #ifndef GLES3_API
	glGenVertexArrays(1, &vao);
    #else
	glGenVertexArraysOES(1, &vao);
    #endif

    return vao;
}

void gl_bind_vertex_array(int vao) {
    #ifndef GLES3_API
	glBindVertexArray(vao);
    #else
	glBindVertexArraysOES(vao);
    #endif
}

int gl_create_buffer() {
    GLuint vbo = 0;
	glGenBuffers(1, &vbo);
    return vbo;
}

void gl_bind_buffer(int target, int buffer) {
    glBindBuffer(target, buffer);
}

void gl_buffer_data(int target, int size, const void* data, int usage) {
    glBufferData(target, size, data, usage);
}

int gl_create_shader(int type) {
    return glCreateShader(type);
}

void gl_shader_source(int shader, std::string source) {
    const char* csource = source.c_str();
    glShaderSource(shader, 1, &csource, 0);
}

void gl_compile_shader(int shader) {
    glCompileShader(shader);
}

int gl_create_program() {
    return glCreateProgram();
}

void gl_attach_shader(int program, int shader) {
    glAttachShader(program, shader);
}

void gl_link_program(int program) {
    glLinkProgram(program);
}

void gl_use_program(int program) {
    glUseProgram(program);
}

void gl_enable_vertex_attrib_array(int index) {
    glEnableVertexAttribArray(index);
}

void gl_vertex_attrib_pointer(int index, int size, int type, bool normalized, int stride) {
    glVertexAttribPointer(index, size, type, normalized, stride, 0);
}

int gl_get_attrib_location(int program, std::string name) {
    return glGetAttribLocation(program, name.c_str());
}

void gl_get_shader_info_log(int handle) {
    GLuint shader = handle;

	GLint logLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	if(logLength == 0) {
		return;
	}

	std::string buffer(logLength, 0);
	GLint written = 0;
	glGetShaderInfoLog(shader, logLength, 0, &buffer[0]);

	printf("Shader Info Log: %s\n", buffer.c_str());
}
