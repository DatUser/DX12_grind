#define PI 					3.1415926
#define TWO_PI 				6.2831852
#define FOUR_PI 			12.566370
#define INV_PI 				0.3183099
#define INV_TWO_PI 			0.1591549
#define INV_FOUR_PI 		0.0795775
#define EPSILON       		1e-3

static float3 f0 = 0.04;//float3(1.00, 0.71, 0.29); //Pre-computed (default is water value)
static float roughness = 0.5;
static float metalness = 0.;// = 0.1;//1 if metallic 0 otherwise

// Use dotNH for microdetails
float3 fresnelSchlick(float dotHV, float3 albedo)
{
  float3 F0 = lerp(f0, albedo, metalness);
  float3 f90 = 1.;
  return F0 + (f90 - F0) * pow(1.0 - dotHV, 5.0);
}

float distribGGX(float dotNH, float alpha2)
{
  float dotNH2 = pow(dotNH, 2.0);
  float bot = dotNH2 * (alpha2 - 1.0) + 1.0;
  return alpha2 / (PI * bot * bot + EPSILON);
}

float geometrySmith(float dotNV, float dotNL, float alpha, float alpha2)
{
  float kdirect = pow(alpha + 1.0, 2.0) / 8.0;
  float kIBL = alpha2 / 2.0;
  float k = kdirect;
  float Gobstruction = dotNV / (dotNV * (1.0 - k) + k);
  float Gshadowing = dotNL / (dotNL * (1.0 - k) + k);
  return Gshadowing * Gobstruction;
}

float3 cook_torrance_brdf(float3 wo, float3 wi, float3 n, float3 albedo)
{
    //bissector of v and lightdir
    float3 h = normalize(wi + wo);

    //Storing results
    float dotNV = max(abs(dot(n, wo)), 0.0);
    float dotNL = max(dot(n, wi), 0.0);
    float dotVN = max(dot(wo, n), 0.0);
    float dotNH = max(dot(n, h), 0.0);
    float dotLH = max(dot(wi, h), 0.0);
    float dotVH = max(dot(wo, h), 0.0);
    float dotHV = max(dot(h, wo), 0.0);
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    //Calculating Normal Distribution
    float nDistrib = distribGGX(dotNH, alpha2);

    //Calculate Schlick Fresnel approximation
    //Represents ks
    float3 nFresnel = fresnelSchlick(dotHV, albedo);

    //Calculate Smith GGX
    float nGeometric = geometrySmith(dotNV, dotNL, alpha, alpha2);

    //Computing Cook-Torrance GGX model
    float3 specular = (nDistrib * nFresnel * nGeometric) /
    (4.0 * dotNV * dotNL + EPSILON);

    //Computing diffuse Lambert
    float3 kd = float3(1.0, 1.0, 1.0);
    kd = (kd - nFresnel) * (1.0 - metalness);
    float3 diffuse = kd * albedo / PI;

    float3 color = (diffuse + specular) * dotNL * /*lightColor.rgb*/float3(0.1, 0.7, 0.2);
    color = color / (color + float3(1.0, 1.0, 1.0));

	// gamma
    color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

    //return h;
    return color;
}
