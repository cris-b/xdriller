varying vec2 texCoord;
uniform float scale_x;
uniform float scale_y;

void main(void) 
{
	gl_Position = gl_Vertex + vec4(scale_x*0.03*sign(gl_Vertex.x),scale_y*0.01,0.0,0.0); 
	texCoord = gl_MultiTexCoord0.st;
}


