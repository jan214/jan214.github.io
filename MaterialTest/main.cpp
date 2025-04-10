#include <iostream>
#include <functional>
#include <math.h>
#include <string>
#include <chrono>

#if 0
#include <windows.h>
#include <GL/gl.h>

typedef void (*GL_GENBUFFERS)(GLsizei, GLuint*);
GL_GENBUFFERS glGenBuffers;

typedef void (*GL_GENFRAMEBUFFERS)(GLsizei, GLuint* ids);
GL_GENFRAMEBUFFERS glGenFramebuffers;
#endif

#if __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
#if 1
#include "glad/glad.h"
#endif
#endif

#include "GLFW/include/glfw3.h"

#include "lodePNG.h"

#include "shader.h"
#include "object.h"
#include "resources.h"
#include "mymath.h"
#include "collision.h"

int width = 854;
int height = 480;

//static float* matrixMultiplication(float* matrix, float* matrix2) {
//    float* transformMatrix = new float[16] { matrix[0] * matrix2[0] + matrix[1] * matrix2[4] + matrix[2] * matrix2[8] + matrix[3] * matrix2[12],
//                                matrix[0]*matrix2[1] + matrix[1]*matrix2[5] + matrix[2]*matrix2[9] + matrix[3]*matrix2[13],
//                                matrix[0]*matrix2[2] + matrix[1]*matrix2[6] + matrix[2]*matrix2[10] + matrix[3]*matrix2[14],
//                                matrix[0]*matrix2[3] + matrix[1]*matrix2[7] + matrix[2]*matrix2[11] + matrix[3]*matrix2[15],
//                                matrix[4]*matrix2[0] + matrix[5]*matrix2[4] + matrix[6]*matrix2[8] + matrix[5]*matrix2[12],
//                                matrix[4]*matrix2[1] + matrix[5]*matrix2[5] + matrix[6]*matrix2[9] + matrix[7]*matrix2[13],
//                                matrix[4]*matrix2[2] + matrix[5]*matrix2[6] + matrix[6]*matrix2[10] + matrix[7]*matrix2[14],
//                                matrix[4]*matrix2[3] + matrix[5]*matrix2[7] + matrix[6]*matrix2[11] + matrix[7]*matrix2[15],
//                                matrix[8]*matrix2[0] + matrix[9]*matrix2[4] + matrix[10]*matrix2[8] + matrix[11]*matrix2[12],
//                                matrix[8]*matrix2[1] + matrix[9]*matrix2[5] + matrix[10]*matrix2[9] + matrix[11]*matrix2[13],
//                                matrix[8]*matrix2[2] + matrix[9]*matrix2[6] + matrix[10]*matrix2[10] + matrix[11]*matrix2[14],
//                                matrix[8]*matrix2[3] + matrix[9]*matrix2[7] + matrix[10]*matrix2[11] + matrix[11]*matrix2[15],
//                                matrix[12]*matrix2[0] + matrix[13]*matrix2[4] + matrix[14]*matrix2[8] + matrix[15]*matrix2[12],
//                                matrix[12]*matrix2[1] + matrix[13]*matrix2[5] + matrix[14]*matrix2[9] + matrix[15]*matrix2[13],
//                                matrix[12]*matrix2[2] + matrix[13]*matrix2[6] + matrix[14]*matrix2[10] + matrix[15]*matrix2[14],
//                                matrix[12]*matrix2[3] + matrix[13]*matrix2[7] + matrix[14]*matrix2[11] + matrix[15]*matrix2[15] };
//    return transformMatrix;
//}

//static std::string* matrixString(std::string* matrix, std::string* matrix2) {
//    const std::string* string[16] = { matrix[0] + "*" + matrix2[0] + " + " + matrix[1] + "*" + matrix2[4] + " + " + matrix[2] + "*" + matrix2[8] + "+" + matrix[3] + "*" + matrix2[12],
//        matrix[0] + "*" + matrix2[1] + " + " + matrix[1] + "*" + matrix2[5] + " + " + matrix[2] + "*" + matrix2[9] + " + " + matrix[3] + "*" + matrix2[13],
//        matrix[0] + "*" + matrix2[2] + " + " + matrix[1] + "*" + matrix2[6] + " + " + matrix[2] + "*" + matrix2[10] + " + " + matrix[3] + "*" + matrix2[14],
//        matrix[0] + "*" + matrix2[3] + " + " + matrix[1] + "*" + matrix2[7] + " + " + matrix[2] + "*" + matrix2[11] + " + " + matrix[3] + "*" + matrix2[15],
//        matrix[4] + "*" + matrix2[0] + " + " + matrix[5] + "*" + matrix2[4] + " + " + matrix[6] + "*" + matrix2[8] + " + " + matrix[5] + "*" + matrix2[12],
//        matrix[4] + "*" + matrix2[1] + " + " + matrix[5] + "*" + matrix2[5] + " + " + matrix[6] + "*" + matrix2[9] + " + " + matrix[7] + "*" + matrix2[13],
//        matrix[4] + "*" + matrix2[2] + " + " + matrix[5] + "*" + matrix2[6] + " + " + matrix[6] + "*" + matrix2[10] + " + " + matrix[7] + "*" + matrix2[14],
//        matrix[4] + "*" + matrix2[3] + " + " + matrix[5] + "*" + matrix2[7] + " + " + matrix[6] + "*" + matrix2[11] + " + " + matrix[7] + "*" + matrix2[15],
//        matrix[8] + "*" + matrix2[0] + " + " + matrix[9] + "*" + matrix2[4] + " + " + matrix[10] + "*" + matrix2[8] + " + " + matrix[11] + "*" + matrix2[12],
//        matrix[8] + "*" + matrix2[1] + " + " + matrix[9] + "*" + matrix2[5] + " + " + matrix[10] + "*" + matrix2[9] + " + " + matrix[11] + "*" + matrix2[13],
//        matrix[8] + "*" + matrix2[2] + " + " + matrix[9] + "*" + matrix2[6] + " + " + matrix[10] + "*" + matrix2[10] + " + " + matrix[11] + "*" + matrix2[14],
//        matrix[8] + "*" + matrix2[3] + " + " + matrix[9] + "*" + matrix2[7] + " + " + matrix[10] + "*" + matrix2[11] + " + " + matrix[11] + "*" + matrix2[15],
//        matrix[12] + "*" + matrix2[0] + "+" + matrix[13] + "*" + matrix2[4] + " + " + matrix[14] + "*" + matrix2[8] + " + " + matrix[15] + "*" + matrix2[12],
//        matrix[12] + "*" + matrix2[1] + "+" + matrix[13] + "*" + matrix2[5] + " + " + matrix[14] + "*" + matrix2[9] + " + " + matrix[15] + "*" + matrix2[13],
//        matrix[12] + "*" + matrix2[2] + "+" + matrix[13] + "*" + matrix2[6] + " + " + matrix[14] + "*" + matrix2[10] + " + " + matrix[15] + "*" + matrix2[14],
//        matrix[12] + "*" + matrix2[3] + "+" + matrix[13] + "*" + matrix2[7] + " + " + matrix[14] + "*" + matrix2[11] + " + " + matrix[15] + "*" + matrix2[15] };
//}

std::function<void()> loop;

void mainloop(){
    loop();
}

int main(){
    GLFWwindow* window;

    setbuf(stdout, NULL);

    if(!glfwInit()){
        printf("glfwInit failed\n");
    }

    window = glfwCreateWindow(width,height,"Test",nullptr,nullptr);

    if(!window)
        glfwTerminate();

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

#if !__EMSCRIPTEN__
    gladLoadGL();
#endif

#if 0
    glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
    if(!glGenBuffers){
        printf("loading error");
    }

    glGenFramebuffers = (GL_GENFRAMEBUFFERS)wglGetProcAddress("glGenFramebuffers");
    if(!glGenFramebuffers){
        printf("loading error");
    }
#endif

    const char* postProcessingVertexShader = "#version 300 es\n"
                                "precision lowp float;\n"
//                                "attribute vec3 position;\n"
                                "in vec3 position;\n"
//                                "attribute vec2 texCoord;\n"
                                "in vec2 texCoord;\n"
//                                "varying vec2 aTexCoord;\n"
                                "uniform vec3 lightPosition[2];\n"
                                "uniform vec3 lightColor[2];\n"
                                "out vec2 aTexCoord;\n"
                                "out vec3 aLightPosition[2];\n"
                                "out vec3 aLightColor[2];\n"
                                "void main(){\n"
                                "aTexCoord = texCoord;\n"
                                "aLightPosition = lightPosition;\n"
                                "aLightColor = lightColor;\n"
                                "gl_Position = vec4(position,1.0);\n"
                                "}";

    const char* postProcessingFragmentShader = "#version 300 es\n"
                                    "precision lowp float;\n"
//                                    "varying vec2 aTexCoord;\n"
                                    "in vec2 aTexCoord;\n"
                                    "out vec4 fragColor;\n"
                                    "uniform sampler2D albedoTexture;\n"
                                    "uniform sampler2D positionTexture;\n"
                                    "uniform sampler2D normalTexture;\n"
                                    "uniform sampler2D heightTexture;\n"
                                    "uniform vec3 viewPosition;\n"
                                    "in vec3 aLightPosition[2];\n"
                                    "in vec3 aLightColor[2];\n"
                                    "vec3 fresnelSchlick(float cosTheta, vec3 F0){\n"
                                    "   return F0 + (vec3(1.0,1.0,1.0)-F0)*pow(clamp(1.0-cosTheta,0.0,1.0),5.0);\n"
                                    "}\n"
                                    "float geometrySchlickGGX(float normalDotViewVector,float roughness){\n"
                                    "   float a = roughness+1.0;\n"
                                    "   float k = (a*a)/8.0;\n"
                                    "   float numerator = normalDotViewVector;\n"
                                    "   float denominator = normalDotViewVector*(1.0-k)+k;\n"
                                    "   return numerator/denominator;\n"
                                    "}\n"
                                    "float geometrySmith(vec3 normal,vec3 view,vec3 light,float roughness){\n"
                                    "   float normalDotViewDirection = max(dot(normal,view),0.1);\n"
                                    "   float normalDotLightDirection = max(dot(normal,light),0.1);\n"
                                    "   float ggx2 = geometrySchlickGGX(normalDotViewDirection,roughness);\n"
                                    "   float ggx1 = geometrySchlickGGX(normalDotLightDirection,roughness);\n"
                                    "   return ggx1*ggx2;\n"
                                    "}\n"
                                    "float distributionGGX(vec3 normal,vec3 halfvector,float roughness){\n"
                                    "   float a = roughness*roughness;\n"
                                    "   float a2 = a*a;\n"
                                    "   float normalDotHalfvector = max(dot(normal,halfvector),0.1);\n"
                                    "   float normalDotHalfvector2 = normalDotHalfvector*normalDotHalfvector;\n"
                                    "   float numerator = a2;\n"
                                    "   float denominator = (normalDotHalfvector2*(a2-1.0)+1.0);\n"
                                    "   denominator = 3.14159*denominator*denominator;\n"
                                    "   return numerator/denominator;\n"
                                    "}\n"
                                    "void main(){\n"
    //                                    "gl_FragColor = vec4(texture2D(normalTexture,aTexCoord).xyz,1.0);\n
        
                                        // Goroud Shading
                                        "vec3 position = texture(positionTexture,aTexCoord).xyz;\n"
                                        "vec3 albedo = texture(albedoTexture,aTexCoord).xyz;\n"
                                        "vec3 normal = texture(normalTexture,aTexCoord).xyz;\n"
                                        "vec3 viewDirection = normalize(viewPosition - position);\n"

                                        "vec3 resultColor = vec3(0.0);\n"
                                        "for(int lightIndex = 0; lightIndex < 2; lightIndex++){\n"
                                        "   vec3 lightDirection = normalize(aLightPosition[lightIndex] - position);\n"
                                        "   float diffuse = max(dot(lightDirection, normal), 0.0);\n"
                                        "   resultColor += diffuse * aLightColor[lightIndex];\n"
                                        "}\n"

                                        //"vec3 lightDirection = normalize(aLightPosition[0] - position);\n"
                                        //"float diffuse = max(dot(lightDirection, normal), 0.0); \n"
                                        "vec3 lightcolor = albedo * resultColor;\n"

                                        //"float diffuse = 0.0;\n"
                                        //"float specular = 0.0;\n"
                                        //"vec3 lightcolor = albedo*0.3;\n"
                                        //"for(int counter = 0; counter < 2; counter++){\n"
                                        //    "vec3 lightDirection = normalize(lightPosition[counter] - position);\n"
                                        //    "diffuse = max(dot(lightDirection, normal), 0.0); \n"
                                        //    "if(counter == 0 && diffuse > 0.95) lightcolor += vec3(0.0,0.0,1.0) * diffuse;\n"
                                        //    "if(counter == 1 && diffuse > 0.95) lightcolor += vec3(1.0,0.0,0.0) * diffuse;\n"
                                        //    "vec3 reflect = reflect(-lightDirection, normal);\n"
                                        //    "specular += pow(max(dot(viewDirection,reflect),0.0),16.0);\n"
                                        //"}\n"
                                        
                                        "fragColor = vec4(lightcolor,1.0);\n"
                                        //"fragColor = vec4(albedo*diffuse,1.0);\n"

                                        // Physical Based Rendering (not really working)
                                        //"vec3 albedo = texture(albedoTexture,aTexCoord).xyz;\n"
                                        //"albedo = (albedo+1.0)/2.0;\n"
                                        //"vec3 lightDirection = normalize(lightPosition-texture(positionTexture,aTexCoord).xyz);\n"
                                        //"vec3 normalDirection = normalize(texture(normalTexture,aTexCoord).xyz);\n"
                                        //"vec3 viewDirection = normalize(viewPosition-texture(positionTexture,aTexCoord).xyz);\n"
                                        //"vec3 halfvector = normalize(viewDirection+lightDirection);\n"
                                        //"vec3 F0 = vec3(0.04,0.04,0.04);\n"
                                        //"float metallic = 0.3;\n"
                                        //"float roughness = 0.8;\n"
                                        //"F0 = mix(F0,albedo,metallic);\n"
                                        //"float nDotV = max(dot(normalDirection,viewDirection),0.1);\n"
                                        //"float nDotL = max(dot(normalDirection,lightDirection),0.1);\n"
                                        //"float hDotV = max(dot(halfvector,viewDirection),0.1);\n"
                                        //"vec3 lightColor = vec3(1.0,1.0,1.0);\n"
                                        //"float D = distributionGGX(normalDirection,halfvector,roughness);\n"
                                        //"float G = geometrySmith(normalDirection,viewDirection,lightDirection,roughness);\n"
                                        //"vec3 F = fresnelSchlick(hDotV,F0);\n"
                                        //"vec3 numerator = D*F*G;\n"
                                        //"float denominator = 4.0*nDotL*nDotV;\n"
                                        //"vec3 specular = numerator/max(denominator,0.001);\n"
                                        //"vec3 kSpecular = F;\n"
                                        //"vec3 kDiffuse = vec3(1.0,1.0,1.0)-kSpecular;\n"
                                        //"kDiffuse *= 1.0-metallic;\n"
                                        //"vec3 Lo = lightColor*(kDiffuse*(albedo/3.14159)+specular)*nDotL;\n"
                                        //"vec3 ambient = vec3(0.03,0.03,0.03)*albedo;\n"
                                        //"vec3 color = ambient + Lo;\n"
                                        //"vec3 vector1 = vec3(1.0,1.0,1.0);\n"
                                        //"color = color/(color+vector1);\n"
                                        //"color = pow(color, vector1/vec3(2.2,2.2,2.2));\n"

                                        //"color = texture(normalTexture,aTexCoord).xyz;\n"
                                        //"color = texture(normalTexture,aTexCoord).xyz;\n"
                                        //"float diffuse1 = max(dot(lightDirection,normalDirection),0.2);\n"
                                        //"vec3 reflectionVector1 = reflect(-lightDirection,normalDirection);\n"
                                        //"float specular1 = pow(max(dot(reflectionVector1,viewDirection),0.0),32.0);\n"
                                        //"color = vec3(1.0,0.0,0.0)*(diffuse1+specular1);\n"
    //                                    "fragColor = vec4((viewDirection+1.0)/2.0,1.0);\n"
                                        //"fragColor = vec4(color,1.0);\n"
    //                                    "gl_FragColor = vec4(0.0,1.0,0.0,1.0);\n"
    //                                    "fragColor = vec4(aTexCoord.x > 0.5 ? texture(positionTexture,aTexCoord).xyz : texture(normalTexture,aTexCoord).xyz,1.0);\n"
                                    "}";

    const char* objectVertexShader = "#version 300 es\n"
                                "precision lowp float;\n"
//                                "attribute vec3 position;\n"
                                "in vec3 position;\n"
                                "in vec2 textureCoordinate;\n"
                                "in vec3 normal;\n"
                                "in vec3 tangent;\n"
                                "in vec3 bitangent;\n"
//                                "in sampler2D normalTexture;\n"
                                "out vec4 aPosition;\n"
                                "out vec4 bPosition;\n"
//                                "attribute vec3 normal;\n"
//                                "in vec3 normal;\n"
//                                "in int bone;\n"
                                "out vec2 aTextureCoordinate;\n"
                                "out vec4 bNormal;\n"
                                "out vec4 bTangent;\n"
                                "out vec4 bBitangent;\n"
                                "out mat3 matrixBTN;\n"
                                "out vec3 aViewDirection;\n"
//                                "flat out int aBone;\n"
                                "uniform mat4 modelMatrix;\n"
//                                "varying vec3 color;\n"
                                "uniform mat4 perspectiveMatrix;\n"
                                "uniform mat4 viewMatrix;\n"
                                "uniform vec3 viewPosition;\n"
                                "void main(){\n"
    //                                "aBone = bone;\n"
                                    "aPosition = modelMatrix * vec4(position,1.0);\n"
                                    "bPosition = perspectiveMatrix * viewMatrix * aPosition;\n"
                                    "aTextureCoordinate = textureCoordinate;\n"
                                    "mat3 rotationMatrix = mat3(modelMatrix);\n"
                                    "bNormal = normalize(vec4(rotationMatrix * normal,1.0));\n"
                                    "bTangent = normalize(vec4(rotationMatrix * tangent,1.0));\n"
                                    "bBitangent = normalize(vec4(rotationMatrix * bitangent,1.0));\n"
                                    "matrixBTN = mat3(bitangent, tangent, normal);\n"
                                    "aViewDirection = normalize(matrixBTN * (aPosition.xyz - viewPosition));\n"
                                    "gl_Position = bPosition;\n"
                                "}";

    const char* objectFragmentShader = "#version 300 es\n"
                                        "precision lowp float;\n"
                                        "layout(location = 0) out vec4 albedoColor;\n"
                                        "layout(location = 1) out vec4 positionColor;\n"
                                        "layout(location = 2) out vec4 normalColor;\n"
                                        "uniform sampler2D baseTexture;\n"
                                        "uniform sampler2D normalTexture;\n"
                                        "uniform sampler2D heightTexture;\n"
//                                        "varying vec3 color;\n"
                                        "in vec4 aPosition;\n"
                                        "in vec4 bPosition;\n"
                                        "in vec2 aTextureCoordinate;\n"
                                        "in vec4 bNormal;\n"
                                        "in vec4 bTangent;\n"
                                        "in vec4 bBitangent;\n"
                                        "in mat3 matrixBTN;\n"
                                        "in vec3 aViewDirection;\n"
//                                        "flat in int aBone;\n"
                                        "void main(){\n"
                                            // parallax occlusion mapping
                                            "vec2 texturePosition = aTextureCoordinate;\n"
                                            "vec2 textureDirection = vec2(aViewDirection.y/aViewDirection.z, aViewDirection.x/aViewDirection.z);\n"
                                            "float heightValue = 1.0;\n"
                                            "float beforeValue = 1.0;\n"
                                            "const float layersValue = 1.0/32.0;\n"
                                            "for(int height = 8; height > 0; height--){\n"
                                                "float textureHeight = texture(heightTexture,texturePosition).x;\n"
                                                "heightValue -= layersValue;\n"
                                                "if(textureHeight > heightValue){\n"
                                                    "break;\n"
                                                "}\n"
                                                "texturePosition += textureDirection * (textureHeight*0.1);\n"
                                            "}\n"
                                            "float weight = heightValue / (heightValue - beforeValue);\n"
                                            "vec2 previousTextureCoordinate = texturePosition + textureDirection;\n"
                                            "vec2 finalTextureCoordinate = previousTextureCoordinate * weight + texturePosition * (1.0-weight);\n"
                                            "albedoColor = vec4(texture(baseTexture,texturePosition).xyz,1.0);\n"
                                            "vec4 heightColor = vec4(texture(heightTexture,aTextureCoordinate).xyz,1.0);\n"
                                            "positionColor = vec4(aPosition.xyz,1.0);\n"
                                            "vec3 unpackedNormal = texture(normalTexture,texturePosition).xyz * 2.0 - 1.0;\n"
                                            "vec3 worldNormal = matrixBTN * unpackedNormal;\n"
                                            //"normalColor = vec4(matrixBTN * unpackedNormal,texture(heightTexture,aTextureCoordinate).x);\n"/*"vec4(normalize(texture(normalTexture,aTextureCoordinate).xyz*2.0-1.0) * matrixBTN,1.0);\n"*/
                                            //"gl_FragColor = vec4(bPosition.xyz,1.0);\n"
                                            "normalColor = vec4(worldNormal,1.0);\n"
                                        "}";

    unsigned int size = 512u /*1024u*/;
    std::vector<unsigned char> baseTextureSource;
    std::vector<unsigned char> normalTextureSource;
    std::vector<unsigned char> heightTextureSource;
    std::vector<unsigned char> roughnessTextureSource;
    std::vector<unsigned char> ambientOcclusionTextureSource;

    std::vector<unsigned char> baseBrickSource;
    std::vector<unsigned char> heightBrickSource;
    std::vector<unsigned char> normalBrickSource;
#if __EMSCRIPTEN__
    lodepng::decode(baseTextureSource,size,size,"Resources/Stylized_Sand_001_basecolor.png");
    lodepng::decode(normalTextureSource,size,size,"Resources/Stylized_Sand_001_normal.png");
    lodepng::decode(heightTextureSource,size,size,"Resources/Stylized_Sand_001_height.png");
    lodepng::decode(roughnessTextureSource,size,size,"Resources/Stylized_Sand_001_roughness.png");
    lodepng::decode(ambientOcclusionTextureSource,size,size,"Resources/Stylized_Sand_001_ambientOcclusion.png");
    lodepng::decode(baseBrickSource, size, size, "Resources/bricks.png");
    lodepng::decode(normalBrickSource, size, size, "Resources/bricks_normal.png");
    lodepng::decode(heightBrickSource, size, size, "Resources/bricks_height.png");
#else
    lodepng::decode(baseTextureSource,size,size,"C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/Stylized_Sand_001_basecolor.png");
    lodepng::decode(normalTextureSource,size,size,"C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/Stylized_Sand_001_normal.png");
    lodepng::decode(heightTextureSource,size,size,"C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/Stylized_Sand_001_height.png");
    lodepng::decode(roughnessTextureSource,size,size,"C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/Stylized_Sand_001_roughness.png");
    lodepng::decode(ambientOcclusionTextureSource,size,size,"C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/Stylized_Sand_001_ambientOcclusion.png");

    lodepng::decode(baseBrickSource, size, size, "C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/bricks.png");
    lodepng::decode(normalBrickSource, size, size, "C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/bricks_normal.png");
    lodepng::decode(heightBrickSource, size, size, "C:/Users/Jan/Documents/QtProjects/MaterialTest/Resources/bricks_height.png");
#endif

    // camera control values.
    float viewRotation[3] = {0.0f,0.0f,0.0f};
    float viewPosition[3] = {0.0f,0.0f,20.0f};

    float lightPosition[6] = { -5.0f,0.0f,-1.5f, 5.0f,0.0f,-1.5f };
    float lightColors[6] = { 0.0f,0.0f,1.0f, 1.0f,0.0f,0.0f };

    float HalfFOV = 35.0f;
    float aspectScale = 1.0f/((float)width/(float)height);
    float fovScale = 1.0f / (tanf(HalfFOV) * 3.141f/180.0f);

    float nearClippingPlane = 0.1f;
    float farClippingPlane = 10.0f;

    float perspectiveMatrix[16] = {fovScale * aspectScale,0.0f,0.0f,0.0f,
                                   0.0f,fovScale,0.0f,0.0f,
                                   0.0f,0.0f,-farClippingPlane/(farClippingPlane-nearClippingPlane),-farClippingPlane,
                                   0.0f,0.0f,-farClippingPlane*nearClippingPlane/(farClippingPlane-nearClippingPlane),0.0f};

    float viewMatrix[16] = {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
    float transformMatrix[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    Shader postProcessingShader(postProcessingVertexShader,postProcessingFragmentShader);
//    postProcessingShader.addAttribute("indices",3);
    postProcessingShader.addAttribute("position",3);
    postProcessingShader.addAttribute("texCoord",2);
    postProcessingShader.addUniform("viewPosition", 3, &viewPosition[0]);
    postProcessingShader.addUniform("lightPosition", 3, &lightPosition[0]);
    postProcessingShader.addUniform("lightColor", 3, &lightColors[0]);
    Shader objectShader(objectVertexShader,objectFragmentShader);
    objectShader.perspectiveMatrix = &perspectiveMatrix[0];
    objectShader.viewMatrix = &transformMatrix[0];
//    objectShader.addAttribute("indices",3);
    objectShader.addAttribute("position",3);
    objectShader.addAttribute("textureCoordinate",2);
    objectShader.addAttribute("normal",3);
    objectShader.addAttribute("tangent", 3);
    objectShader.addAttribute("bitangent", 3);
//    objectShader.addAttribute("bone",1);
    //objectShader.addUniform("viewPosition", 3);
    objectShader.addUniform("modelMatrix",16,nullptr);
    objectShader.addUniform("perspectiveMatrix",16,&perspectiveMatrix[0]);
    objectShader.addUniform("viewMatrix",16,&transformMatrix[0]);
    objectShader.addUniform("viewPosition", 3, &viewPosition[0]);
    objectShader.addSamplerUniform("baseTexture");
    objectShader.addSamplerUniform("normalTexture");
    objectShader.addSamplerUniform("heightTexture");

    Object postProcessingQuad(postProcessingShader, nullptr, nullptr);
    postProcessingQuad.addElementBuffer<int>(Resources::quadIndices,6);
    postProcessingQuad.indicesCount = 6;
    postProcessingQuad.verticesCount = 6;
    postProcessingQuad.addBuffer<float>(Resources::vertices,6);
    postProcessingQuad.addBuffer<float>(Resources::textureCoords,6);
    postProcessingShader.addObject(&postProcessingQuad);
    Object object2(objectShader,nullptr,nullptr);
    object2.translate(0.25f,0.0f,-1.8f);
    object2.addElementBuffer<int>(Resources::indices, 3);
    object2.indicesCount = 3;
    object2.verticesCount = 3;
    object2.addBuffer<float>(Resources::triangle2,3);
    object2.addBuffer<float>(Resources::textureCoordstriangle2,3);
    object2.addBuffer<float>(Resources::triangleNormals2,3);
    object2.addBuffer<float>(Resources::triangleTangents2, 3);
    object2.addBuffer<float>(Resources::triangleBitangents2, 3);
//    object2->addBuffer<int>(triangleBones,3);
    objectShader.addObject(&object2);
    Object object3(objectShader,nullptr,nullptr);
    object3.translate(0.0f,0.0f,-1.5f);
    object3.addElementBuffer<int>(Resources::indices, 3);
    object3.indicesCount = 3;
    object3.verticesCount = 3;
    object3.addBuffer<float>(Resources::triangle,3);
    object3.addBuffer<float>(Resources::textureCoordstriangle,3);
    object3.addBuffer<float>(Resources::triangleNormals2,3);
    object3.addBuffer<float>(Resources::triangleTangents2, 3);
    object3.addBuffer<float>(Resources::triangleBitangents2, 3);
//    object3->addBuffer<int>(triangleBones,3);
    objectShader.addObject(&object3);
    float scale[2] = { 1.0f,1.0f };
    float moveDir4[3] = { 0.0f,0.0f,0.0f };
    float rotationDir4[3] = { 0.0f,0.0f,0.0f };
    Object object4(objectShader, new CollisionBox(&moveDir4[0],&rotationDir4[0]), nullptr);
    object4.translate(0.0f,0.0f,-3.0f);
    object4.rotate(0.5f, 0.0f, /*0.785398f*/0.0f);
    object4.addElementBuffer<int>(Resources::cubeIndices,108);
    object4.indicesCount = 36;//sizeof(cubeIndices)/sizeof(int)/*36*/;
    object4.verticesCount = 36;//sizeof(cubeVertices)/sizeof(float)/*24*/;
    object4.addBuffer<float>(Resources::cubeVertices,108);
    object4.addBuffer<float>(Resources::cubeTextureCoordinates,72);
    object4.addBuffer<float>(Resources::cubeNormals,108);
    object4.addBuffer<float>(Resources::cubeTangents, 108);
    object4.addBuffer<float>(Resources::cubeBitangents, 108);
//    object4->addBuffer<int>(bones,36);
    //object4->skeletalAnimationComponent.addBone(Resources::armatureTranslation, Resources::armatureRotation, nullptr, nullptr);
    //object4->skeletalAnimationComponent.addBone(Resources::bone1Translation,Resources::bone1Rotation,Resources::bone1Inverse,object4->skeletalAnimationComponent.bones[0]);
    //object4->skeletalAnimationComponent.addBone(Resources::bone2Translation,Resources::bone2Rotation,Resources::bone2Inverse,object4->skeletalAnimationComponent.bones[1]);
    //object4->skeletalAnimationComponent.newTransformMatrix(Resources::animation1testTranslation,Resources::animation1testRotation,1);

    objectShader.addObject(&object4);
    float moveDir5[3] = { 0.0f,-1.0f,0.0f };
    Object object5(objectShader, new CollisionBox(&moveDir5[0],&moveDir4[0]), nullptr);
    object5.translate(1.75f, 5.0f, -3.0f);
    object5.rotate(0.0f, 0.0f, /*0.785398f*/0.0f);
    object5.addElementBuffer<int>(Resources::cubeIndices, 108);
    object5.indicesCount = 36;
    object5.verticesCount = 36;
    object5.addBuffer<float>(Resources::cubeVertices, 108);
    object5.addBuffer<float>(Resources::cubeTextureCoordinates, 72);
    object5.addBuffer<float>(Resources::cubeNormals, 108);
    object5.addBuffer<float>(Resources::cubeTangents, 108);
    object5.addBuffer<float>(Resources::cubeBitangents, 108);
    
    objectShader.addObject(&object5);

    Object* collisionObjects[2] = { &object4,&object5 };

    std::vector<GLuint> objectTextures;

    GLuint objectBaseTexture;
    glGenTextures(1, &objectBaseTexture);
    glBindTexture(GL_TEXTURE_2D, objectBaseTexture);
    //glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,size,size,0,GL_RGBA,GL_UNSIGNED_BYTE,&baseTextureSource[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &baseBrickSource[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    objectTextures.push_back(objectBaseTexture);

    GLuint objectNormalTexture;
    glGenTextures(1, &objectNormalTexture);
    glBindTexture(GL_TEXTURE_2D, objectNormalTexture);
    //glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,size,size,0,GL_RGBA,GL_UNSIGNED_BYTE,&normalTextureSource[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &normalBrickSource[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    objectTextures.push_back(objectNormalTexture);

    GLuint objectHeightTexture;
    glGenTextures(1, &objectHeightTexture);
    glBindTexture(GL_TEXTURE_2D, objectHeightTexture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &heightTextureSource[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, &heightBrickSource[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    objectTextures.push_back(objectHeightTexture);

    std::vector<GLuint> gBufferTextures;

    GLuint customFramebuffer;
    glGenFramebuffers(1,&customFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,customFramebuffer);

    GLuint gBufferAlbedoTexture;
    glGenTextures(1,&gBufferAlbedoTexture);
    glBindTexture(GL_TEXTURE_2D,gBufferAlbedoTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gBufferAlbedoTexture,0);

    gBufferTextures.push_back(gBufferAlbedoTexture);

    postProcessingShader.addSamplerUniform("albedoTexture");

    GLuint gBufferPositionTexture;
    glGenTextures(1,&gBufferPositionTexture);
    glBindTexture(GL_TEXTURE_2D,gBufferPositionTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // emscripten(webgl) only allows GL_CLAMP_TO_EDGE for non square textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,gBufferPositionTexture,0);

    gBufferTextures.push_back(gBufferPositionTexture);

    //GLuint positionTextureLocation = glGetUniformLocation(shader.program,"positionTexture");
    //glUniform1i(positionTextureLocation,0);

    postProcessingShader.addSamplerUniform("positionTexture");

    GLuint gBufferNormalTexture;
    glGenTextures(1,&gBufferNormalTexture);
    glBindTexture(GL_TEXTURE_2D,gBufferNormalTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // emscripten(webgl) only allows GL_CLAMP_TO_EDGE for non square textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,gBufferNormalTexture,0);

    gBufferTextures.push_back(gBufferNormalTexture);

    postProcessingShader.addSamplerUniform("normalTexture");

    GLuint gBufferDepthTexture;
    glGenTextures(1,&gBufferDepthTexture);
    glBindTexture(GL_TEXTURE_2D,gBufferDepthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,gBufferDepthTexture,0);

    gBufferTextures.push_back(gBufferDepthTexture);

    postProcessingShader.addSamplerUniform("depthTexture");

    const GLenum buffers[]{GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};

    glDrawBuffers(3,buffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        printf("framebuffer not complete");
        return -1;
    }

    glViewport(0, 0, width, height);

    // this should be in bindUniforms.
    //GLuint viewPositionUniform = glGetUniformLocation(postProcessingShader.program,"viewPosition");

    glBindFramebuffer(GL_FRAMEBUFFER,0);

    std::chrono::steady_clock::time_point nowTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> deltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - nowTime);
    float deltaTimeSeconds = deltaTime.count() / 1000.0f;
    float speed = 10.0f;

    // collision test should be moved into the loop when its working correctly
    const bool collisionTest = object5.collisionComponent->overlap(collisionObjects[0]->collisionComponent);
    //printf("colliding start: %s\n", collisionTest ? "true" : "false");

    loop = [&window,&postProcessingShader,&objectShader,&customFramebuffer,&gBufferTextures,&objectTextures,&viewRotation,&viewPosition,&transformMatrix,&nowTime,&endTime,&deltaTime,&deltaTimeSeconds,&speed,&collisionObjects](){
        endTime = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - nowTime);
        deltaTimeSeconds = deltaTime.count() / 1000.0f;
        nowTime = std::chrono::steady_clock::now();

        if (collisionObjects[1]->collisionComponent->overlap(collisionObjects[0]->collisionComponent)) {
            //printf("collisionObject[0]->rotation: %f %f %f\n", collisionObjects[1]->rotation[0], collisionObjects[1]->rotation[1], collisionObjects[1]->rotation[2]);
            //collisionObjects[1]->collisionComponent->moveDirection[0] = 0.0f;
            //collisionObjects[1]->collisionComponent->moveDirection[1] = 0.0f;
            //collisionObjects[1]->collisionComponent->moveDirection[2] = 0.0f;
        } else {
        }

        collisionObjects[1]->translateDelta(collisionObjects[1]->collisionComponent->moveDirection[0] * deltaTimeSeconds, collisionObjects[1]->collisionComponent->moveDirection[1] * deltaTimeSeconds, collisionObjects[1]->collisionComponent->moveDirection[2] * deltaTimeSeconds);
        collisionObjects[1]->rotateDelta(collisionObjects[1]->collisionComponent->rotationDirection[0] * deltaTimeSeconds, collisionObjects[1]->collisionComponent->rotationDirection[1] * deltaTimeSeconds, collisionObjects[1]->collisionComponent->rotationDirection[2] * deltaTimeSeconds);
        collisionObjects[1]->collisionComponent->moveDirection[1] -= 0.5f * deltaTimeSeconds;
        //collisionObjects[1]->collisionComponent->rotationDirection[0] -= 0.5f * deltaTimeSeconds;
        //collisionObjects[1]->collisionComponent->rotationDirection[1] -= 0.5f * deltaTimeSeconds;
        //collisionObjects[1]->collisionComponent->rotationDirection[2] -= 0.5f * deltaTimeSeconds;

        collisionObjects[0]->rotateDelta(collisionObjects[0]->collisionComponent->rotationDirection[0] * deltaTimeSeconds, collisionObjects[0]->collisionComponent->rotationDirection[1] * deltaTimeSeconds, collisionObjects[0]->collisionComponent->rotationDirection[2] * deltaTimeSeconds);
        //printf("rotation: %f %f %f\n", collisionObjects[0]->rotation[0], collisionObjects[0]->rotation[1], collisionObjects[0]->rotation[2]);

        if (collisionObjects[1]->translation[1] <= -3.0f) {
            collisionObjects[1]->translate(1.5f, 5.0f, -3.0f);
            collisionObjects[1]->collisionComponent->moveDirection[0] = 0.0f;
            collisionObjects[1]->collisionComponent->moveDirection[1] = -1.0f;
            collisionObjects[1]->collisionComponent->moveDirection[2] = 0.0f;
            collisionObjects[1]->collisionComponent->rotationDirection[0] = 0.0f;
            collisionObjects[1]->collisionComponent->rotationDirection[1] = 0.0f;
            collisionObjects[1]->collisionComponent->rotationDirection[2] = 0.0f;
            collisionObjects[1]->rotate(0.0f, 0.0f, 0.0f);
        }

        if(glfwGetKey(window,GLFW_KEY_D)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewPosition[0] += sinf(viewRotation[1] + 1.57f) * speed * deltaTimeSeconds;
            viewPosition[2] += cosf(viewRotation[1] + 1.57f) * speed * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_A)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewPosition[0] -= sinf(viewRotation[1] + 1.57f) * speed * deltaTimeSeconds;
            viewPosition[2] -= cosf(viewRotation[1] + 1.57f) * speed * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_W)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewPosition[0] -= sinf(viewRotation[1]) * speed * deltaTimeSeconds;
            viewPosition[2] -= cosf(viewRotation[1]) * speed * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_S)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewPosition[0] += sinf(viewRotation[1]) * speed * deltaTimeSeconds;
            viewPosition[2] += cosf(viewRotation[1]) * speed * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_Q)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewRotation[1] += 0.1f * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_E)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewRotation[1] -= 0.1f * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_R)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewRotation[0] -= 0.1f * deltaTimeSeconds;
        }

        if(glfwGetKey(window,GLFW_KEY_F)){
//            printf("Q key pressed: %f",viewPosition[0]);
            viewRotation[0] += 0.1f * deltaTimeSeconds;
        }

        float translate[16] = {1.0f,0.0f,0.0f,-viewPosition[0],0.0f,1.0f,0.0f,-viewPosition[1],0.0f,0.0f,1.0f,-viewPosition[2],0.0f,0.0f,0.0f,1.0f};
        float rotationx[16] = {1.0f,0.0f,0.0f,0.0f,0.0f,cosf(-viewRotation[0]),-sinf(-viewRotation[0]),0.0f,0.0f,sinf(-viewRotation[0]),cosf(-viewRotation[0]),0.0f,0.0f,0.0f,0.0f,1.0f};
        float rotationy[16] = {cosf(-viewRotation[1]),0.0f,sinf(-viewRotation[1]),0.0f,0.0f,1.0f,0.0f,0.0f,-sinf(-viewRotation[1]),0.0f,cosf(-viewRotation[1]),0.0f,0.0f,0.0f,0.0f,1.0f};
        float rotationz[16] = {cosf(-viewRotation[2]),-sinf(-viewRotation[2]),0.0f,0.0f,sinf(-viewRotation[2]),cosf(-viewRotation[2]),0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};

        MyMath::matrixMultiplication(rotationy, rotationx);
        MyMath::matrixMultiplication(rotationx, rotationz);
        MyMath::matrixMultiplication(rotationz, translate);

        //Math::matrixMultiplication(rotationy, translate);

        //printf("transformMatrix:\n");
        //for (int counter = 0; counter < 4; counter++) {
        //    printf("%f %f %f %f\n", translate[counter * 4], translate[counter * 4 + 1], translate[counter * 4 + 2], translate[counter * 4 + 3]);
        //}
        //printf("\n");
        //printf("objectShaderMatrix:\n");
        //for (int counter = 0; counter < 4; counter++) {
        //    printf("%f %f %f %f\n", objectShader.viewMatrix[counter * 4], objectShader.viewMatrix[counter * 4 + 1], objectShader.viewMatrix[counter * 4 + 2], objectShader.viewMatrix[counter * 4 + 3]);
        //}
        //printf("\n");

        objectShader.viewMatrix = translate;// updatedViewMatrix;

        glBindFramebuffer(GL_FRAMEBUFFER, customFramebuffer);
        glUseProgram(objectShader.program);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.bindSamplerUniforms(objectTextures);
        objectShader.draw();

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glUseProgram(postProcessingShader.program);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        postProcessingShader.bindSamplerUniforms(gBufferTextures);
        postProcessingShader.draw();

        glBindTexture(GL_TEXTURE_2D,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    };

#if __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop,0,true);
#else
    while (!glfwWindowShouldClose(window)){
        mainloop();
    }
#endif

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
