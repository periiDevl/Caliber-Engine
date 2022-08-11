#version 330 core
layout (location = 0) in vec3 aPos;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;

uniform mat4 lightProjection;
uniform mat4 model;

// Imports the transformation matrices
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    // Calculates current position
	crntPos = vec3(translation * rotation * scale * vec4(aPos, 1.0f));

    gl_Position = lightProjection * model * vec4(crntPos, 1.0);
}