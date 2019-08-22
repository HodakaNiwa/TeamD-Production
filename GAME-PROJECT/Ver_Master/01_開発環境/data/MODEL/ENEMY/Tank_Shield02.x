xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 160;
 -33.24300;-31.88052;17.62962;,
 -39.07950;-31.88052;8.69031;,
 -39.07950;32.81886;8.69031;,
 -33.24300;32.81886;17.62962;,
 -24.15246;-31.88052;24.72393;,
 -24.15246;32.81886;24.72393;,
 -12.69771;-31.88052;29.27874;,
 -12.69771;32.81886;29.27874;,
 -0.00000;-31.88052;30.84822;,
 0.00000;32.81886;30.84822;,
 12.69771;-31.88052;29.27874;,
 12.69771;32.81886;29.27874;,
 24.15246;-31.88052;24.72393;,
 24.15246;32.81886;24.72393;,
 33.24300;-31.88052;17.62962;,
 33.24300;32.81886;17.62962;,
 39.07950;-31.88052;8.69031;,
 39.07950;32.81886;8.69031;,
 -33.24300;32.81886;17.62962;,
 -39.07950;32.81886;8.69031;,
 -48.84936;32.81886;11.16765;,
 -41.55375;32.81886;22.34181;,
 -24.15246;32.81886;24.72393;,
 -30.19056;32.81886;31.20966;,
 -12.69771;32.81886;29.27874;,
 -15.87213;32.81886;36.90318;,
 0.00000;32.81886;30.84822;,
 0.00000;32.81886;38.86503;,
 12.69771;32.81886;29.27874;,
 15.87213;32.81886;36.90318;,
 24.15246;32.81886;24.72393;,
 30.19059;32.81886;31.20966;,
 33.24300;32.81886;17.62962;,
 41.55378;32.81886;22.34181;,
 39.07950;32.81886;8.69031;,
 48.84939;32.81886;11.16765;,
 -41.55375;32.81886;22.34181;,
 -48.84936;32.81886;11.16765;,
 -48.84936;-31.88052;11.16765;,
 -41.55375;-31.88052;22.34181;,
 -30.19056;32.81886;31.20966;,
 -30.19056;-31.88052;31.20966;,
 -15.87213;32.81886;36.90318;,
 -15.87213;-31.88052;36.90318;,
 0.00000;32.81886;38.86503;,
 0.00000;-31.88052;38.86503;,
 15.87213;32.81886;36.90318;,
 15.87213;-31.88052;36.90318;,
 30.19059;32.81886;31.20966;,
 30.19059;-31.88052;31.20966;,
 41.55378;32.81886;22.34181;,
 41.55378;-31.88052;22.34181;,
 48.84939;32.81886;11.16765;,
 48.84939;-31.88052;11.16765;,
 -41.55375;-31.88052;22.34181;,
 -48.84936;-31.88052;11.16765;,
 -39.07950;-31.88052;8.69031;,
 -33.24300;-31.88052;17.62962;,
 -30.19056;-31.88052;31.20966;,
 -24.15246;-31.88052;24.72393;,
 -15.87213;-31.88052;36.90318;,
 -12.69771;-31.88052;29.27874;,
 0.00000;-31.88052;38.86503;,
 -0.00000;-31.88052;30.84822;,
 15.87213;-31.88052;36.90318;,
 12.69771;-31.88052;29.27874;,
 30.19059;-31.88052;31.20966;,
 24.15246;-31.88052;24.72393;,
 41.55378;-31.88052;22.34181;,
 33.24300;-31.88052;17.62962;,
 48.84939;-31.88052;11.16765;,
 39.07950;-31.88052;8.69031;,
 39.07950;32.81886;8.69031;,
 48.84939;32.81886;11.16765;,
 48.84939;-31.88052;11.16765;,
 39.07950;-31.88052;8.69031;,
 -39.07950;32.81886;8.69031;,
 -39.07950;-31.88052;8.69031;,
 -48.84936;-31.88052;11.16765;,
 -48.84936;32.81886;11.16765;,
 22.85553;-31.61079;-24.74577;,
 26.86830;-31.61079;-18.59973;,
 26.86827;-14.12502;-18.59973;,
 22.85553;-14.12502;-24.74577;,
 16.60551;-31.61079;-29.62332;,
 16.60551;-14.12502;-29.62332;,
 8.73003;-31.61079;-32.75487;,
 8.73003;-14.12502;-32.75487;,
 0.00000;-31.61079;-33.83394;,
 0.00000;-14.12502;-33.83394;,
 -8.73003;-31.61079;-32.75487;,
 -8.73003;-14.12502;-32.75487;,
 -16.60551;-31.61079;-29.62332;,
 -16.60551;-14.12502;-29.62332;,
 -22.85550;-31.61079;-24.74577;,
 -22.85550;-14.12502;-24.74577;,
 -26.86827;-31.61079;-18.59973;,
 -26.86827;-14.12502;-18.59973;,
 22.85553;-14.12502;-24.74577;,
 26.86827;-14.12502;-18.59973;,
 33.58536;-14.12502;-20.30298;,
 28.56942;-14.12502;-27.98553;,
 16.60551;-14.12502;-29.62332;,
 20.75688;-14.12502;-34.08243;,
 8.73003;-14.12502;-32.75487;,
 10.91256;-14.12502;-37.99689;,
 0.00000;-14.12502;-33.83394;,
 0.00000;-14.12502;-39.34572;,
 -8.73003;-14.12502;-32.75487;,
 -10.91253;-14.12502;-37.99689;,
 -16.60551;-14.12502;-29.62332;,
 -20.75688;-14.12502;-34.08243;,
 -22.85550;-14.12502;-24.74577;,
 -28.56939;-14.12502;-27.98553;,
 -26.86827;-14.12502;-18.59973;,
 -33.58533;-14.12502;-20.30298;,
 28.56942;-14.12502;-27.98553;,
 33.58536;-14.12502;-20.30298;,
 33.58536;-31.61079;-20.30298;,
 28.56942;-31.61079;-27.98553;,
 20.75688;-14.12502;-34.08243;,
 20.75688;-31.61079;-34.08243;,
 10.91256;-14.12502;-37.99689;,
 10.91256;-31.61079;-37.99689;,
 0.00000;-14.12502;-39.34572;,
 0.00000;-31.61079;-39.34572;,
 -10.91253;-14.12502;-37.99689;,
 -10.91253;-31.61079;-37.99689;,
 -20.75688;-14.12502;-34.08243;,
 -20.75688;-31.61079;-34.08243;,
 -28.56939;-14.12502;-27.98553;,
 -28.56939;-31.61079;-27.98553;,
 -33.58533;-14.12502;-20.30298;,
 -33.58533;-31.61079;-20.30298;,
 28.56942;-31.61079;-27.98553;,
 33.58536;-31.61079;-20.30298;,
 26.86830;-31.61079;-18.59973;,
 22.85553;-31.61079;-24.74577;,
 20.75688;-31.61079;-34.08243;,
 16.60551;-31.61079;-29.62332;,
 10.91256;-31.61079;-37.99689;,
 8.73003;-31.61079;-32.75487;,
 0.00000;-31.61079;-39.34572;,
 0.00000;-31.61079;-33.83394;,
 -10.91253;-31.61079;-37.99689;,
 -8.73003;-31.61079;-32.75487;,
 -20.75688;-31.61079;-34.08243;,
 -16.60551;-31.61079;-29.62332;,
 -28.56939;-31.61079;-27.98553;,
 -22.85550;-31.61079;-24.74577;,
 -33.58533;-31.61079;-20.30298;,
 -26.86827;-31.61079;-18.59973;,
 -26.86827;-14.12502;-18.59973;,
 -33.58533;-14.12502;-20.30298;,
 -33.58533;-31.61079;-20.30298;,
 -26.86827;-31.61079;-18.59973;,
 26.86827;-14.12502;-18.59973;,
 26.86830;-31.61079;-18.59973;,
 33.58536;-31.61079;-20.30298;,
 33.58536;-14.12502;-20.30298;;
 
 68;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;18,19,20,21;,
 4;22,18,21,23;,
 4;24,22,23,25;,
 4;26,24,25,27;,
 4;28,26,27,29;,
 4;30,28,29,31;,
 4;32,30,31,33;,
 4;34,32,33,35;,
 4;36,37,38,39;,
 4;40,36,39,41;,
 4;42,40,41,43;,
 4;44,42,43,45;,
 4;46,44,45,47;,
 4;48,46,47,49;,
 4;50,48,49,51;,
 4;52,50,51,53;,
 4;54,55,56,57;,
 4;58,54,57,59;,
 4;60,58,59,61;,
 4;62,60,61,63;,
 4;64,62,63,65;,
 4;66,64,65,67;,
 4;68,66,67,69;,
 4;70,68,69,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;84,80,83,85;,
 4;86,84,85,87;,
 4;88,86,87,89;,
 4;90,88,89,91;,
 4;92,90,91,93;,
 4;94,92,93,95;,
 4;96,94,95,97;,
 4;98,99,100,101;,
 4;102,98,101,103;,
 4;104,102,103,105;,
 4;106,104,105,107;,
 4;108,106,107,109;,
 4;110,108,109,111;,
 4;112,110,111,113;,
 4;114,112,113,115;,
 4;116,117,118,119;,
 4;120,116,119,121;,
 4;122,120,121,123;,
 4;124,122,123,125;,
 4;126,124,125,127;,
 4;128,126,127,129;,
 4;130,128,129,131;,
 4;132,130,131,133;,
 4;134,135,136,137;,
 4;138,134,137,139;,
 4;140,138,139,141;,
 4;142,140,141,143;,
 4;144,142,143,145;,
 4;146,144,145,147;,
 4;148,146,147,149;,
 4;150,148,149,151;,
 4;152,153,154,155;,
 4;156,157,158,159;;
 
 MeshMaterialList {
  1;
  68;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/CHARACTER/ENEMY_TANK_UV.png";
   }
  }
 }
 MeshNormals {
  41;
  0.837331;0.000000;-0.546696;,
  0.736263;0.000000;-0.676696;,
  0.497377;0.000000;-0.867535;,
  0.248104;0.000000;-0.968733;,
  0.000000;0.000000;-1.000000;,
  -0.248104;0.000000;-0.968733;,
  -0.497377;0.000000;-0.867535;,
  -0.736263;0.000000;-0.676696;,
  -0.837331;0.000000;-0.546696;,
  0.000000;1.000000;0.000000;,
  -0.837332;0.000000;0.546694;,
  -0.736262;0.000000;0.676696;,
  -0.497376;0.000000;0.867535;,
  -0.248105;0.000000;0.968733;,
  0.000000;0.000000;1.000000;,
  0.248104;0.000000;0.968733;,
  0.497375;0.000000;0.867535;,
  0.736263;0.000000;0.676696;,
  0.837332;0.000000;0.546694;,
  -0.837332;-0.000001;0.546695;,
  -0.736263;-0.000000;0.676695;,
  -0.497376;0.000000;0.867535;,
  -0.248104;0.000000;0.968733;,
  0.248104;0.000000;0.968733;,
  0.497377;0.000000;0.867535;,
  0.736263;0.000000;0.676695;,
  0.837331;0.000000;0.546696;,
  0.837332;0.000000;-0.546694;,
  0.736262;0.000000;-0.676697;,
  0.497375;0.000000;-0.867535;,
  0.248105;0.000000;-0.968733;,
  -0.000000;0.000000;-1.000000;,
  -0.248105;0.000000;-0.968733;,
  -0.497376;0.000000;-0.867535;,
  -0.736263;0.000000;-0.676696;,
  -0.837332;0.000000;-0.546694;,
  0.000000;-1.000000;-0.000000;,
  0.245790;0.000000;-0.969323;,
  -0.245791;0.000000;-0.969323;,
  -0.245792;0.000000;0.969323;,
  0.245791;0.000000;0.969323;;
  68;
  4;1,0,0,1;,
  4;2,1,1,2;,
  4;3,2,2,3;,
  4;4,3,3,4;,
  4;5,4,4,5;,
  4;6,5,5,6;,
  4;7,6,6,7;,
  4;8,7,7,8;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;11,10,10,11;,
  4;12,11,11,12;,
  4;13,12,12,13;,
  4;14,13,13,14;,
  4;15,14,14,15;,
  4;16,15,15,16;,
  4;17,16,16,17;,
  4;18,17,17,18;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;20,19,19,20;,
  4;21,20,20,21;,
  4;22,21,21,22;,
  4;14,22,22,14;,
  4;23,14,14,23;,
  4;24,23,23,24;,
  4;25,24,24,25;,
  4;26,25,25,26;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;28,27,27,28;,
  4;29,28,28,29;,
  4;30,29,29,30;,
  4;31,30,30,31;,
  4;32,31,31,32;,
  4;33,32,32,33;,
  4;34,33,33,34;,
  4;35,34,34,35;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;39,39,39,39;,
  4;40,40,40,40;;
 }
 MeshTextureCoords {
  160;
  0.460601;0.619100;,
  0.413635;0.619100;,
  0.413635;0.869100;,
  0.460601;0.869100;,
  0.507566;0.619100;,
  0.507566;0.869100;,
  0.554532;0.619100;,
  0.554532;0.869100;,
  0.601497;0.619100;,
  0.601497;0.869100;,
  0.648462;0.619100;,
  0.648462;0.869100;,
  0.695428;0.619100;,
  0.695428;0.869100;,
  0.742393;0.619100;,
  0.742393;0.869100;,
  0.789358;0.619100;,
  0.789358;0.869100;,
  0.460601;0.620167;,
  0.413635;0.620167;,
  0.413635;0.870167;,
  0.460601;0.870167;,
  0.507566;0.620167;,
  0.507566;0.870167;,
  0.554532;0.620167;,
  0.554532;0.870167;,
  0.601497;0.620167;,
  0.601497;0.870167;,
  0.648462;0.620167;,
  0.648462;0.870167;,
  0.695428;0.620167;,
  0.695428;0.870167;,
  0.742393;0.620167;,
  0.742393;0.870167;,
  0.789358;0.620167;,
  0.789358;0.870167;,
  0.460601;0.622699;,
  0.413635;0.622699;,
  0.413635;0.872699;,
  0.460601;0.872699;,
  0.507566;0.622699;,
  0.507566;0.872699;,
  0.554532;0.622699;,
  0.554532;0.872699;,
  0.601497;0.622699;,
  0.601497;0.872699;,
  0.648462;0.622699;,
  0.648462;0.872699;,
  0.695428;0.622699;,
  0.695428;0.872699;,
  0.742393;0.622699;,
  0.742393;0.872699;,
  0.789358;0.622699;,
  0.789358;0.872699;,
  0.460601;0.625230;,
  0.413635;0.625230;,
  0.413635;0.875230;,
  0.460601;0.875230;,
  0.507566;0.625230;,
  0.507566;0.875230;,
  0.554532;0.625230;,
  0.554532;0.875230;,
  0.601497;0.625230;,
  0.601497;0.875230;,
  0.648462;0.625230;,
  0.648462;0.875230;,
  0.695428;0.625230;,
  0.695428;0.875230;,
  0.742393;0.625230;,
  0.742393;0.875230;,
  0.789358;0.625230;,
  0.789358;0.875230;,
  0.789358;0.704877;,
  0.789358;0.790056;,
  0.789358;0.875235;,
  0.789358;0.619698;,
  0.413635;0.620561;,
  0.413635;0.876098;,
  0.413635;0.790919;,
  0.413635;0.705739;,
  0.459818;0.620678;,
  0.412853;0.620678;,
  0.412853;0.870678;,
  0.459818;0.870678;,
  0.506783;0.620678;,
  0.506783;0.870678;,
  0.553749;0.620678;,
  0.553749;0.870678;,
  0.600714;0.620678;,
  0.600714;0.870678;,
  0.647679;0.620678;,
  0.647679;0.870678;,
  0.694645;0.620678;,
  0.694645;0.870678;,
  0.741610;0.620678;,
  0.741610;0.870678;,
  0.788576;0.620678;,
  0.788576;0.870678;,
  0.459818;0.621620;,
  0.412853;0.621620;,
  0.412853;0.871620;,
  0.459818;0.871620;,
  0.506783;0.621620;,
  0.506783;0.871620;,
  0.553749;0.621620;,
  0.553749;0.871620;,
  0.600714;0.621620;,
  0.600714;0.871620;,
  0.647679;0.621620;,
  0.647679;0.871620;,
  0.694645;0.621620;,
  0.694645;0.871620;,
  0.741610;0.621620;,
  0.741610;0.871620;,
  0.788576;0.621620;,
  0.788576;0.871620;,
  0.459818;0.622196;,
  0.412853;0.622196;,
  0.412853;0.872196;,
  0.459818;0.872196;,
  0.506783;0.622196;,
  0.506783;0.872196;,
  0.553749;0.622196;,
  0.553749;0.872196;,
  0.600714;0.622196;,
  0.600714;0.872196;,
  0.647679;0.622196;,
  0.647679;0.872196;,
  0.694645;0.622196;,
  0.694645;0.872196;,
  0.741610;0.622196;,
  0.741610;0.872196;,
  0.788576;0.622196;,
  0.788576;0.872196;,
  0.459818;0.622819;,
  0.412853;0.622819;,
  0.412853;0.872819;,
  0.459818;0.872819;,
  0.506783;0.622819;,
  0.506783;0.872819;,
  0.553749;0.622819;,
  0.553749;0.872819;,
  0.600714;0.622819;,
  0.600714;0.872819;,
  0.647679;0.622819;,
  0.647679;0.872819;,
  0.694645;0.622819;,
  0.694645;0.872819;,
  0.741610;0.622819;,
  0.741610;0.872819;,
  0.788576;0.622819;,
  0.788576;0.872819;,
  0.788576;0.704777;,
  0.788576;0.788146;,
  0.788576;0.871516;,
  0.788576;0.621407;,
  0.412852;0.621615;,
  0.412852;0.871724;,
  0.412852;0.788354;,
  0.412852;0.704984;;
 }
}
