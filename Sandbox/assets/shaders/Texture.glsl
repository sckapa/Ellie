#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoords;
out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	v_TexCoords = a_TexCoords;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 v_TexCoords;
in vec4 v_Color;

void main()
{
	// color = texture(u_Texture, v_TexCoords) * u_Color;
	color = v_Color;
}