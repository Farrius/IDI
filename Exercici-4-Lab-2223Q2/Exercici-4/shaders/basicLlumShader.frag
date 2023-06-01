#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;

vec3 posFocusBlanca = vec3(5, 10, 5);
uniform vec3 posFocusVermell;
uniform vec3 posFocusBlau;
uniform vec3 posFocusVerd;
uniform vec3 posFocusGroc;

uniform vec3 colFocusBlanca;
uniform vec3 colFocusVermell;
uniform vec3 colFocusBlau;
uniform vec3 colFocusVerd;
uniform vec3 colFocusGroc;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
	vec3 Lb = normalize(vec3(View*vec4(posFocusBlanca, 1.0)) - fvertex);
	vec3 llumBlanca = Difus(normalize(fnormal), Lb, colFocusBlanca) + Especular(normalize(fnormal), Lb, fvertex, colFocusBlanca); 

	vec3 Lb1 = normalize(vec3(View*vec4(posFocusVermell, 1.0)) - fvertex);
	vec3 llumVermella = Difus(normalize(fnormal), Lb1, colFocusVermell) + Especular(normalize(fnormal), Lb1, fvertex, colFocusVermell); 

	vec3 Lb2 = normalize(vec3(View*vec4(posFocusBlau, 1.0)) - fvertex);
	vec3 llumBlava = Difus(normalize(fnormal), Lb2, colFocusBlau) + Especular(normalize(fnormal), Lb2, fvertex, colFocusBlau); 

	vec3 Lb3 = normalize(vec3(View*vec4(posFocusVerd, 1.0)) - fvertex);
	vec3 llumVerda = Difus(normalize(fnormal), Lb3, colFocusVerd) + Especular(normalize(fnormal), Lb3, fvertex, colFocusVerd); 

	vec3 Lb4 = normalize(vec3(View*vec4(posFocusGroc, 1.0)) - fvertex);
	vec3 llumGroga = Difus(normalize(fnormal), Lb4, colFocusGroc) + Especular(normalize(fnormal), Lb4, fvertex, colFocusGroc); 

    FragColor = vec4(Ambient() + llumBlanca + llumVermella + llumBlava + llumVerda + llumGroga, 1.0);
}
