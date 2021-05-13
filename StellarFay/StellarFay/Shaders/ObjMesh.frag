#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform float dissolve;

uniform sampler2D tex0;

void main()
{
	FragColor = vec4(texture(tex0, texCoord).xyz, dissolve);
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
