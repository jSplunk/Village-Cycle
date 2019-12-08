//
// This demo performs lighting in world coordinate space and uses a texture to provide basic diffuse surface properties.
//

#version 330

//
// model-view-projection matrices
// note: seperate out model transform matrix so we can move vertices into world coords for lighting
//
uniform mat4 modelMatrix; // to calc world coords of vertex
uniform mat4 invTransposeModelMatrix; // inverse transpose of model matrix to transform normal vector into world coords

uniform mat4 viewProjectionMatrix; // to calc clip coords once lighting done in world space


//
// input vertex packet
//
layout (location = 0) in vec4 vertexPos;
layout (location = 6) in vec3 vertexOffset;

void main(void) {

	// vertex position in clip coords - necessary for pipeline
	gl_Position = viewProjectionMatrix * modelMatrix * (vertexPos + vec4(vertexOffset, 1.0f));
}
