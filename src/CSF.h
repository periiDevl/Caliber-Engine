#pragma once
class CSF {
public:
	const char* Default =
	R"(
	#version 330 core

	layout (location = 0) out vec4 FragColor;
	layout (location = 1) out vec4 BloomColor;

	in vec3 crntPos;
	in vec3 Normal;
	in vec3 color;
	in vec2 texCoord;
	in vec4 fragPosLight;



	uniform sampler2D diffuse0;
	uniform sampler2D specular0;
	uniform sampler2D shadowMap;
	uniform vec4 lightColor;
	uniform vec3 lightPos;

	uniform vec4 lColor;
	uniform vec3 lPos;

	uniform vec3 camPos;

	uniform float near = 0.0000000000001f;
	uniform float far;

	uniform float worldRadius;

	uniform float bias1;
	uniform float bias2;

	uniform float avgShadow = 1.0f;

	uniform int sampleRadius;

	const float levelShade = 300000000000000000000.0;

	uniform bool BPL_Lighting;
	uniform vec4 tint;
	uniform mat4 model;
	vec4 pointLight()
	{	
		vec3 lightVec = lightPos - crntPos;

		float dist = length(lightVec);
		float a = 3.0;
		float b = 0.7;
		float inten = 10.0f / (a * dist * dist + b * dist + 1.0f);

		float ambient = 0.0f;

		vec3 normal = normalize(Normal);
		vec3 lightDirection = normalize(lightVec);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		float specular = 0.0f;
		if (diffuse != 0.0f)
		{
			float specularLight = 0.50f;
			vec3 viewDirection = normalize(camPos - crntPos);
			vec3 halfwayVec = normalize(viewDirection + lightDirection);
			float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
			specular = specAmount * specularLight;
		};

		return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
	}


	vec4 direcLight()
	{


		float ambient = 0.20f;

		// diffuse lighting
		vec3 normal = normalize(Normal);
		vec3 lightDirection = normalize(lightPos);
		float diffuse = max(dot(normal, lightDirection), 0.0f);
		float level = floor(diffuse * levelShade);
		diffuse = level / levelShade;




		float specular = 0.0f;
		if (diffuse != 0.0f)
		{
			float specularLight = 0.20f;
			vec3 viewDirection = normalize(camPos - crntPos);
			vec3 halfwayVec = normalize(viewDirection + lightDirection);
			float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
			level = floor(specAmount * levelShade);
			specAmount = level / levelShade;
			specular = specAmount * specularLight;

			// non blinn
			if (!BPL_Lighting)
			{
				vec3 reflectionDirection = reflect(-lightDirection, normal);
				specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
				specular = specAmount * specularLight;
			}
		};


		float shadow = 0.0f;

		vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
		if (lightCoords.z <= 1.0f)
		{
			lightCoords = (lightCoords + 1.0f) / 2.0f;
			float currentDepth = lightCoords.z;


			float bias = max((bias1 / 300) * (1.0f - dot(normal, lightDirection)), (bias2 / 300000));


			
			vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);

			for (int y = -sampleRadius; y <= sampleRadius; y++)
			{
				for (int x = -sampleRadius; x <= sampleRadius; x++)
				{
					float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
					if (currentDepth > closestDepth + bias)
					{
						shadow += 1.0f;
					}
				}
			}

			shadow /= pow((sampleRadius * 2 + avgShadow), 2);
		}



		return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r * specular  * (1.0f - shadow)) * lightColor;
	}



	float linearizeDepth(float depth, float near, float far)
	{
		return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
	}



	void main()
	{
		// linear fog
		// black fog
		// FragColor = direcLight() + vec4(linearizeDepth(gl_FragCoord.z) * vec3(-1.0f, -1.0f, -1.0f), 1.0f);
		// normal

		float distance = length(crntPos);
		if (distance > worldRadius) {
			discard;
		}


		FragColor = direcLight() * tint + vec4(linearizeDepth(gl_FragCoord.z, near / 100, far) * vec3(1.0f, 1.0f, 1.0f), 1.0f);

		float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
		if (brightness > 0.15f)
			BloomColor = vec4(FragColor.rgb, 1.0f);
		else
			BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}




	)";

		const char* Unlit =
			R"(
#version 330 core
out vec4 FragColor;
uniform vec4 color;
void main()
{
   FragColor = color;
}
	)";

		const char* Frame =
			R"(
#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D material;


uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float gamma;
uniform float exposure;

uniform float minEdgeContrast;
uniform float subPixelAliasing;
uniform float maximumEdgeDetection;

float FXAA_SPAN_MAX = maximumEdgeDetection;
float FXAA_REDUCE_MIN = 1.0/ minEdgeContrast;
float FXAA_REDUCE_MUL = 1.0/subPixelAliasing;
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
    vec3 vignettecolor = vec3(0, 0, 0);
    vec2 center = vec2(0.5, 0.5); 
    float radius = 0.3; 
    float vignettestrength = 0.7;

    vec2 uv = texCoords - center;
    float dist = length(uv);
    float vignette = smoothstep(radius, radius + vignettestrength, dist);
    vec4 texel = texture(screenTexture, texCoords);

 
   

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
	)";


		const char* Blur =
			R"(
#version 330 core
out vec4 FragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform bool horizontal;

const int radius = 5;
uniform float spreadBlur = 1.7f;

float weights[radius];
void main()
{             
    float x = 0.0f;
    for (int i = 0; i < radius; i++)
    {
        if (spreadBlur <= 2.0f)
            x += 3.0f / radius;
        else
            x += 6.0f / radius;

        weights[i] = exp(-0.5f * pow(x / spreadBlur, 2.0f)) / (spreadBlur * sqrt(2 * 3.14159265f));
    }


    vec2 tex_offset = 1.0f / textureSize(screenTexture, 0);
    vec3 result = texture(screenTexture, texCoords).rgb * weights[0];

    if(horizontal)
    {
        for(int i = 1; i < radius; i++)
        {
            result += texture(screenTexture, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
            result += texture(screenTexture, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
        }
    }
    else
    {
        for(int i = 1; i < radius; i++)
        {
            result += texture(screenTexture, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weights[i];
            result += texture(screenTexture, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weights[i];
        }
    }
    FragColor = vec4(result, 1.0f);
}
	)";

		const char* NONE =
			R"(
#version 330 core

void main()
{
}
	)";


		const char* Ouline =
			R"(
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1, 0, 0, 1.0);
}
	)";


		const char* Skybox =
			R"(
#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;



void main()
{    
    FragColor = texture(skybox, texCoords);

}
	)";

};