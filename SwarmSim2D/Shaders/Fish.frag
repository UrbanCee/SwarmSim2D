#version 400 core
in vec3 colFrag;
out vec4 fColor;

void main()
{
    fColor = vec4(colFrag.rgb, 1.0);
}
