varying vec2 texCoord;
varying vec3 position;

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;


void main(void)
{

	texCoord = gl_MultiTexCoord0.st;

    normal = normalize(gl_NormalMatrix * gl_Normal);
    lightDir = normalize(vec3(gl_LightSource[0].position));
    halfVector = normalize(gl_LightSource[0].halfVector.xyz);

    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;



    position = gl_Vertex.xyz;

	gl_Position = ftransform();
}
