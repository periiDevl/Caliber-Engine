#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D material;


uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float gamma;
uniform float exposure;

float FXAA_SPAN_MAX = 8.0;
float FXAA_REDUCE_MIN = 1.0/128.0;
float FXAA_REDUCE_MUL = 1.0/8.0;
uniform vec2 resolution;
vec3 ApplyFXAA() {
    vec2 inverse_resolution = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    vec3 rgbNW = texture(screenTexture, texCoords + (vec2(-1.0, -1.0) * inverse_resolution)).rgb;
    vec3 rgbNE = texture(screenTexture, texCoords + (vec2(1.0, -1.0) * inverse_resolution)).rgb;
    vec3 rgbSW = texture(screenTexture, texCoords + (vec2(-1.0, 1.0) * inverse_resolution)).rgb;
    vec3 rgbSE = texture(screenTexture, texCoords + (vec2(1.0, 1.0) * inverse_resolution)).rgb;
    vec3 rgbM = texture(screenTexture, texCoords).rgb;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * 0.25 * FXAA_REDUCE_MUL, FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * inverse_resolution;
    vec3 rgbA = 0.5 * (
        texture(screenTexture, texCoords.xy + dir * (1.0 / 3.0 - 0.5)).rgb +
        texture(screenTexture, texCoords.xy + dir * (2.0 / 3.0 - 0.5)).rgb);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(screenTexture, texCoords.xy + dir * (-0.5)).rgb +
        texture(screenTexture, texCoords.xy + dir * (0.5)).rgb);
    return vec3(rgbB);
}


void main()
{
    //vignette
    //---------------------
    vec3 vignettecolor = vec3(0, 0, 0);
    vec2 center = vec2(0.5, 0.5); // center of vignette effect
    float radius = 1.0; // radius of vignette effect
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


    vec3 fxaa = ApplyFXAA();

    FragColor.rgb = pow(mix(toneMapped, fxaa, vec3(1.0f / gamma)), vec3(1.0f / gamma));
    FragColor.rgb = mix(FragColor.rgb, vignettecolor, vignette);

    //FragColor.rgb = mix(toneMapped, vignettecolor, vignette);
    //FragColor.rgb = pow(toneMapped, vec3(1.0f / gamma));
    //FragColor = Monochrome(vec3(0.5f,0.5f,0.5f));
    //FragColor = FragColor * Identity();
}