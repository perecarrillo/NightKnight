#version 330

uniform mat4 projection, modelview;
uniform vec2 texCoordDispl;

in vec2 position;
out vec2 vtexCoord;

void main()
{
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(position, 0.0, 1.0);
	// vtexCoord =  vec2((projection * modelview * vec4(position, 0.0, 1.0)).xy);
    // vtexCoord = gl_Position;
    vtexCoord = position;

}
