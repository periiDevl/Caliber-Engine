#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D material;


uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float gamma;
uniform float exposure;

const float offset = 1.0 / 300.0;
const vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);

const float sobel_dx_kernel[9] = float[](
    1, 0, -1,
    2, 0, 2,
    1, 0, -1
);

const float sobel_dy_kernel[9] = float[](
     1,  2,  1,
     0,  0,  0,
    -1, -2, -1
);

const float gauss_blur_kernel[9] = float[](
    1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
    2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
    1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
);

vec4 Monochrome(vec3 tintColor) {
    vec4 color = texture(material, texCoords);
    float average = 0.299*color.r + 0.587*color.g + 0.114*color.b;
    return vec4(tintColor * vec3(average), 1.0);
}

vec4 Edge(vec3 tintColor) {

    vec3 dx = vec3(0);
    vec3 dy = vec3(0);
    for(int i = 0; i < 9; i++)
    {
        dx += sobel_dx_kernel[i] * vec3(texture(material, texCoords.st + offsets[i]));
        dy += sobel_dy_kernel[i] * vec3(texture(material, texCoords.st + offsets[i]));
    }

    float r = sqrt(dx.r * dx.r + dy.r * dy.r);
    float g = sqrt(dx.g * dx.g + dy.g * dy.g);
    float b = sqrt(dx.b * dx.b + dy.b * dy.b);
    
    return vec4(tintColor * vec3(r, g, b), 1.0);
}

vec4 Blur(vec3 tintColor) {

    vec3 color = vec3(0);

    for(int i = 0; i < 9; i++)
    {
        color += gauss_blur_kernel[i] * vec3(texture(material, texCoords.st + offsets[i]));
    }
    
    return vec4(tintColor * color, 1.0);
}

vec4 Identity() {
    return texture(material, texCoords);
}

void main()
{
    //vignette
    //---------------------
    //vec3 vignettecolor = vec3(0, 0, 0);
    //vec2 center = vec2(0.5, 0.5); // center of vignette effect
    //float radius = 0.3; // radius of vignette effect
    //float vignettestrength = 0.5; // strength of vignette effect

    //vec2 uv = texCoords - center;
    //float dist = length(uv);
    //float vignette = smoothstep(radius, radius + vignettestrength, dist);
    //vec4 texel = texture(screenTexture, texCoords);
    //---------------------


   

    vec3 fragment = texture(screenTexture, texCoords).rgb;
    vec3 bloom = texture(bloomTexture, texCoords).rgb;

    vec3 color = fragment + bloom;
    vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);

    FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
    //FragColor.rgb = mix(toneMapped, vignettecolor, vignette);
    //FragColor = Monochrome(vec3(0.5f,0.5f,0.5f));
    //FragColor = FragColor * Identity();
}