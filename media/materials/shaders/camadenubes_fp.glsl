uniform sampler2D tex0;

varying vec2 texCoord;

varying vec3 position;

varying vec4 diffuse, ambient;
varying vec3 normal,lightDir,halfVector;

void main()
{

    vec3 n,halfV;
    float NdotL,NdotHV;
    vec3 ct,cf;
    vec4 texel;
    float at,af;

    vec4 color = ambient; //texture2D(tex0, texCoord);

    n = normalize(normal);
    NdotL = max(dot(n,lightDir),0.0);

    if (NdotL > 0.0) {
        color += diffuse * NdotL;
        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += gl_FrontMaterial.specular *
                gl_LightSource[0].specular *
                pow(NdotHV, gl_FrontMaterial.shininess);
    }


    cf = color.rgb;
    af = gl_FrontMaterial.diffuse.a;

    texel = texture2D(tex0, texCoord);
    ct = texel.rgb;
    at = texel.a;

    color = vec4(ct * cf, at * af);

	if(position.y > 5.4) gl_FragColor =  color;
	else
	{
	    float fogFactor = (5.4 - position.y) / 2.0;
	    fogFactor = clamp(fogFactor, 0.0, 1.0);
	    gl_FragColor =  mix(color,vec4(1.0,1.0,1.0,1.0),fogFactor);
	}
}
