#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec2 fTexCoord;

void main()
{
	vec4 pos = vec4(aPos, 1.0);
	gl_Position = projectionMat * viewMat * modelMat * pos;

	fTexCoord = aTexCoord;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
