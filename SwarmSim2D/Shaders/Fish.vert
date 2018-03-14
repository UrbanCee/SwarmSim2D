
#version 400 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 pos;
layout( location = 2 ) in vec2 vel;
layout( location = 3 ) in vec3 col;
uniform vec2 scale;
out vec3 colFrag;

void
main()
{
    colFrag=col;
    float phi=atan(vel.y,vel.x);
    float x=cos(phi)*vPosition.x-sin(phi)*vPosition.y;
    float y=sin(phi)*vPosition.x+cos(phi)*vPosition.y;
    vec4 actualPos=vec4(x,y,vPosition.zw);
    actualPos.x+=pos.x;
    actualPos.y+=pos.y;
    actualPos.x*=scale.x;
    actualPos.y*=scale.y;
    gl_Position = actualPos;
}
