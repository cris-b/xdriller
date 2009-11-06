uniform sampler2D tex0; // the texture with the scene you want to blur
varying vec2 vTexCoord;

const float blurSize = 1.0/512.0; // I've chosen this size because this will result in that every step will be one pixel wide if the RTScene texture is of size 512x512

void main(void)
{
   vec4 sum = vec4(0.0);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(tex0, vec2(vTexCoord.x - 4.0*blurSize, vTexCoord.y)) * 1.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x - 3.0*blurSize, vTexCoord.y)) * 2.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x - 2.0*blurSize, vTexCoord.y)) * 3.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x - blurSize, vTexCoord.y)) * 4.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y)) * 5.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x + blurSize, vTexCoord.y)) * 4.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x + 2.0*blurSize, vTexCoord.y)) * 3.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x + 3.0*blurSize, vTexCoord.y)) * 2.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x + 4.0*blurSize, vTexCoord.y)) * 1.0/25.0;

   gl_FragColor = sum;
}
