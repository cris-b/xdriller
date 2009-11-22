uniform vec3 uCamera;

varying vec3 vNormal;
varying vec3 vVertex;
varying vec3 vLight;

#define shine 10.0

void main() {
	vec4 out_color = vec4(1.0, 0.3, 0.0, 1.0);

	// silhouette (black)
	vec4 silhouette_color = out_color*0.2;

	vec3 eyePos = uCamera;
	vec3 lightPos = vLight;

	vec3 Normal = normalize(gl_NormalMatrix * vNormal);
	vec3 EyeVert = normalize(eyePos - vVertex);
	vec3 LightVert = normalize(lightPos - vVertex);
	vec3 EyeLight = normalize(LightVert + EyeVert);

	float sil = max(dot(Normal, EyeVert), 0.0);
	if (sil > 0.0 && sil < 0.7)
		out_color = silhouette_color;
	else {
		float spec = pow(max(dot(Normal, EyeLight), 0.0), shine);
		if (spec < 0.1) {
			//out_color *= 0.9;
		}

		float diffuse = max(dot(Normal, LightVert), 0.0);
		if (diffuse < 0.1) {
			out_color *= 0.4;
		}
		else if (diffuse < 0.8) {
			out_color *= 0.8;
		}
	}

	gl_FragColor = out_color;
}
