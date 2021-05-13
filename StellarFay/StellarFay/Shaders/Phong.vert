#version 330 core

layout(location = 0) in vec3 attPos;
layout(location = 1) in vec2 attTexCoord;
layout(location = 2) in vec3 attNormal;

out vec3 fFragPos;
out vec2 fTexCoord;
out vec3 fNormal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat4 invertedModelMat;

void main()
{
	gl_Position = projectionMat * viewMat * modelMat * vec4(attPos, 1.0);

	fFragPos = vec3(modelMat * vec4(attPos, 1.0));

	fTexCoord = attTexCoord;

	fNormal = vec3(transpose(invertedModelMat)) * attNormal;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
