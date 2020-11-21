#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout(binding=0) uniform sampler2D Tex1;

uniform struct LightInfo {
  vec4 Position;  // Light position in cam. coords.
  vec3 La;        // Amb intensity
    vec3 Ld;         // D,S intensity
    vec3 Ls;         // D,S intensity
} Light ;

uniform struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;            // Specular reflectivity
    float Shininess;    // Specular shininess factor
} Material ;

layout( location = 0 ) out vec4 FragColor;

void main() {
    vec3 n = normalize(Normal);
    vec4 camCoords = vec4(Position, 1.0);
    
    vec3 ambient = Light.La * Material.Ka ;

    vec3 s = normalize(vec3(Light.Position - camCoords));
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;

    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 ) {
        /* Phong reflection model
        vec3 r = reflect( -s, n );
        spec = Light.Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );
        */

        vec3 v = normalize(-camCoords.xyz);
        vec3 h = normalize(v + s);
        spec = Light.Ls * Material.Ks * pow( max( dot(h,n), 0.0 ), Material.Shininess );
    }

    vec3 BlinnLight = ambient + diffuse + spec;

    FragColor = vec4(BlinnLight, 0.5f);
}
