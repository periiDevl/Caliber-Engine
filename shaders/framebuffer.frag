#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D material;


uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float gamma;
uniform float exposure;




void main()
{
    //vignette
    //---------------------
    vec3 vignettecolor = vec3(0, 0, 0);
    vec2 center = vec2(0.5, 0.5); // center of vignette effect
    float radius = 0.3; // radius of vignette effect
    float vignettestrength = 0.5; // strength of vignette effect

    vec2 uv = texCoords - center;
    float dist = length(uv);
    float vignette = smoothstep(radius, radius + vignettestrength, dist);
    vec4 texel = texture(screenTexture, texCoords);
    //---------------------


   

    vec3 fragment = texture(screenTexture, texCoords).rgb;
    vec3 bloom = texture(bloomTexture, texCoords).rgb;

    vec3 color = fragment + bloom;
    vec3 toneMapped = vec3(1.0f) - exp(-color * exposure);


    FragColor.rgb = mix(pow(toneMapped, vec3(1.0f / gamma)), vignettecolor, vignette);

    //FragColor.rgb = mix(toneMapped, vignettecolor, vignette);
    //FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
    //FragColor = Monochrome(vec3(0.5f,0.5f,0.5f));
    //FragColor = FragColor * Identity();
}