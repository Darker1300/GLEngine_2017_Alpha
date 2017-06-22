#version 410

in vec2 vUV;
in vec3 vPosition;

uniform sampler2D colourMap;
uniform sampler2D depthMap;

out vec4 fragColour;

void main()
{
   vec4 sum = vec4(0);
   int j;
   int i;

   for( i= -4 ;i < 4; i++)
   {
        for (j = -3; j < 3; j++)
        {
            sum += texture2D(colourMap, vUV + vec2(j, i)*0.004) * 0.25;
        }
   }
       if (texture2D(colourMap, vUV).r < 0.3)
    {
       fragColour = sum*sum*0.012 + texture2D(colourMap, vUV);
    }
    else
    {
        if (texture2D(colourMap, vUV).r < 0.5)
        {
            fragColour = sum*sum*0.009 + texture2D(colourMap, vUV);
        }
        else
        {
            fragColour = sum*sum*0.0075 + texture2D(colourMap, vUV);
        }
    }
}