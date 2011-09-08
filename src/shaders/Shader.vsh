uniform mat4   modelViewProjectionMatrix;
uniform mat3   normalMatrix;
uniform vec3   lightDirection;
uniform float  opacity;
uniform bool   enableDiffuse;

// Flags to use a particular model for shading.
uniform bool   useToonShader;
uniform bool   useBlinnPhongShader;

attribute vec4 vertexPosition;
attribute vec3 vertexNormal;
attribute vec3 vertexColor;

varying lowp vec4  varVertexColor;
varying lowp vec4  varDiffuseColor;
varying lowp vec4  varAmbientColor;

varying highp vec4 varPosition;
varying highp vec3 varNormal;
varying highp vec3 varLightDirection;

void main()
{
  if(useBlinnPhongShader)
  {
    // Default ambient color for now.
    varAmbientColor = vec4(0.01, 0.01, 0.01, 0.0);

    // Default diffuse color for now.
    varDiffuseColor = vec4(0.1, 0.1, 0.1, 0.0);

    // Transform vertex normal into eye space.
    varNormal = normalize(normalMatrix * vertexNormal);

    // Save vertex color for shading later.
    varVertexColor = vec4(vertexColor, 0.0);
  }

  if(useBlinnPhongShader || useToonShader)
  {
    // Save position for shading later.
    varPosition = modelViewProjectionMatrix * vertexPosition;

    // Transform vertex normal into eye space.
    varNormal = normalize(normalMatrix * vertexNormal);

    // Save light direction (direction light for now)
    varLightDirection = normalize(lightDirection);
  }

  // GLSL still requires this.
  gl_Position = varPosition;
}
