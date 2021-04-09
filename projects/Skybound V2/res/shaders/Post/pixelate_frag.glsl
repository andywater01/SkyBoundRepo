#version 420

layout(location = 0) in vec2 inUV;

out vec4 frag_color;

layout (binding = 0) uniform sampler2D s_screenTex;

//Lower the number, closer to regular screen
uniform float u_Intensity = 512.0;

void main()
{
        float Pixels = u_Intensity;
        float dx = 15.0 * (1.0 / Pixels);
        float dy = 10.0 * (1.0 / Pixels);
        vec2 pixelatedCoord = vec2(dx * floor(inUV.x / dx),
                          dy * floor(inUV.y / dy));
        frag_color = texture(s_screenTex, pixelatedCoord);
}