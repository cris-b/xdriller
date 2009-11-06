uniform sampler2D tex0; // this should hold the texture rendered by the horizontal blur pass
varying vec2 vTexCoord;

const float blurSize = 1.0/512.0;

void main(void)
{
   vec4 sum = vec4(0.0);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y - 4.0*blurSize)) * 1.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y - 3.0*blurSize)) * 2.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y - 2.0*blurSize)) * 3.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y - blurSize)) * 4.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y)) * 5.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y + blurSize)) * 4.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y + 2.0*blurSize)) * 3.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y + 3.0*blurSize)) * 2.0/25.0;
   sum += texture2D(tex0, vec2(vTexCoord.x, vTexCoord.y + 4.0*blurSize)) * 1.0/25.0;

   gl_FragColor = sum;
}
