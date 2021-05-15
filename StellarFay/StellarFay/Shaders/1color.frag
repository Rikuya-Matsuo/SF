#version 330 core

out vec4 FragColor;

uniform vec3 color;
uniform float dissolve;

void main()
{
	FragColor = vec4(color, dissolve);
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
