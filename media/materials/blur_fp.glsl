uniform sampler2D tex0;


varying vec2 texCoord[9];

void main()
{
	vec4 sum = (texture2D(tex0, texCoord[0])*4.0+
			   texture2D(tex0, texCoord[1])*2.0+
			   texture2D(tex0, texCoord[2])*2.0+
			   texture2D(tex0, texCoord[3])*2.0+
			   texture2D(tex0, texCoord[4])*2.0+
			   texture2D(tex0, texCoord[5])+
			   texture2D(tex0, texCoord[6])+
			   texture2D(tex0, texCoord[7])+
			   texture2D(tex0, texCoord[8]))/16.0;

    float gray = dot(vec3(sum[0],sum[1],sum[2]), vec3(0.299, 0.587, 0.114));

    vec4 bw = vec4 ((gray+sum[0])/2.0,(gray+sum[1])/2.0,(gray+sum[2])/2.0,1.0);


	gl_FragColor = bw;

}

