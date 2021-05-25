#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform float dissolve;

void main()
{
	vec4 color = texture(tex, fTexCoord);
	color.a *= dissolve;

	FragColor = color;
}

//////////////////////////////////////////////////////////////
// Project		: StellarFay
// Copyright (c) 2021 Rikuya Matsuo. All rights reserved.
//////////////////////////////////////////////////////////////
