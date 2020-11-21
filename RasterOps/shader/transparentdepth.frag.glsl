#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout(binding=0) uniform sampler2D BaseTex;
layout(binding=1) uniform sampler2D AlphaTex;

uniform struct LightInfo {
  vec4 Position;  // Light position in cam. coords.
  vec3 L;         // D,S intensity
  vec3 La;        // Amb intensity
} Light ;

uniform struct MaterialInfo {
  vec3 Ks;            // Specular reflectivity
  vec3 Ka;
  vec3 Kd;
  float Shininess;    // Specular shininess factor
} Material;

uniform int textureRendering;

layout( location = 0 ) out vec4 FragColor;

vec3 material( vec3 position, vec3 n ) {

  vec3 ambient = Light.La * Material.Ka;
  vec3 s = normalize( Light.Position.xyz - position );
  float sDotN = max( dot(s,n), 0.0 );

  vec3 diffuse = Material.Kd * sDotN;  
   
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize( v + s );
    spec = Material.Ks *
            pow( max( dot(h,n), 0.0 ), Material.Shininess );
  }
  return ambient + Light.L*(diffuse + spec);
}

void main() {
    if(textureRendering == 1){
        FragColor = texture( AlphaTex, TexCoord );       
    }
    else {
        if( gl_FrontFacing ) {
            FragColor = vec4( material(Position,normalize(Normal)), 1.0 );
        } else {
            FragColor = vec4( material(Position,normalize(-Normal)), 1.0 );
        }
}
}
