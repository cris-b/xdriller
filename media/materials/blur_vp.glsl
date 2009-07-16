varying vec2 texCoord[9];



void main()
{
	vec2 inPos = sign(gl_Vertex.xy);
	gl_Position = vec4(inPos.xy, 0.0, 1.0);

//
//    5 1 6  ->  1 2 1
//    2 0 3  ->  2 4 2
//    7 4 8  ->  1 2 1
//

	texCoord[0]  = (vec2(inPos.x, -inPos.y) +1.0)/2.0;

	const float size = 0.004;
	texCoord[1] = texCoord[0] + vec2(0.0, -1.0)*size;
	texCoord[2] = texCoord[0] + vec2(-1.0, 0.0)*size;
	texCoord[3] = texCoord[0] + vec2(1.0, 0.0)*size;
	texCoord[4] = texCoord[0] + vec2(0.0, -1.0)*size;
	texCoord[5] = texCoord[0] + vec2(-1.0, -1.0)*size;
	texCoord[6] = texCoord[0] + vec2(1.0, -1.0)*size;
	texCoord[7] = texCoord[0] + vec2(-1.0, 1.0)*size;
	texCoord[8] = texCoord[0] + vec2(1.0, 1.0)*size;
}
