uniform sampler2D tex0;

varying vec2 texCoord;

void main()
{

	gl_FragColor = texture2D(tex0, texCoord);

}
