// define our rectangular texture samplers
uniform sampler2DRect tex0;

// define our varying texture coordinates
varying vec2 texcoord0;
varying vec2 texdim0;

void main (void)
{
    float isodd = mod(texcoord0.y, 2.0); // returns 0 or 1.
    
    vec4 result;
    
    if(bool(isodd))
    {
        vec4 evenfield = texture2DRect(tex0, vec2(texcoord0.x, texcoord0.y + 1.0));
        vec4 oddfield = texture2DRect(tex0, texcoord0);
        
        result = mix(evenfield, oddfield, 0.5);
    }
    
    else
    {
        vec4 evenfield = texture2DRect(tex0, texcoord0);
        vec4 oddfield = texture2DRect(tex0, vec2(texcoord0.x, texcoord0.y - 1.0));
        
        result = mix(evenfield, oddfield, 0.5); 
    } 
    
    gl_FragColor = result; 
}