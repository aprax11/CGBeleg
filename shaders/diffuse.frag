uniform vec3 color;
varying float shade;

void main(void){

  gl_FragColor= vec4(shade*color, 1);			
 }
