varying vec3 vNormal;
varying vec3 vVertex;
varying vec3 vLight;


void main(void) {
	vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vNormal = gl_Normal;
	vLight = gl_LightSource[0].position.xyz;

	gl_Position = ftransform();
}
