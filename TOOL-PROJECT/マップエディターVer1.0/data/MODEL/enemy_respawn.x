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
 140;
 36.25824;39.08306;0.00000;,
 40.79051;39.08306;4.53228;,
 20.39525;74.40870;4.53228;,
 18.12912;70.48367;0.00000;,
 45.32278;39.08306;0.00000;,
 22.66139;78.33374;0.00000;,
 40.79051;39.08306;-4.53228;,
 20.39525;74.40870;-4.53228;,
 36.25824;39.08306;0.00000;,
 18.12912;70.48367;0.00000;,
 -20.39525;74.40870;4.53228;,
 -18.12912;70.48367;0.00000;,
 -22.66139;78.33374;0.00000;,
 -20.39525;74.40870;-4.53228;,
 -18.12912;70.48367;0.00000;,
 -40.79051;39.08305;4.53228;,
 -36.25824;39.08305;0.00000;,
 -45.32278;39.08305;0.00000;,
 -40.79051;39.08305;-4.53228;,
 -36.25824;39.08305;0.00000;,
 -20.39525;3.75744;4.53228;,
 -18.12912;7.68247;0.00000;,
 -22.66138;-0.16761;-0.00000;,
 -20.39525;3.75744;-4.53228;,
 -18.12912;7.68247;0.00000;,
 20.39525;3.75744;4.53228;,
 18.12912;7.68247;0.00000;,
 22.66138;-0.16761;-0.00000;,
 20.39525;3.75744;-4.53228;,
 18.12912;7.68247;0.00000;,
 40.79051;39.08306;4.53228;,
 36.25824;39.08306;0.00000;,
 45.32278;39.08306;0.00000;,
 40.79051;39.08306;-4.53228;,
 36.25824;39.08306;0.00000;,
 36.25824;39.08306;0.00000;,
 40.79051;42.28786;3.20481;,
 20.39525;67.26686;-21.77417;,
 18.12912;61.28661;-22.20354;,
 45.32278;39.08306;0.00000;,
 22.66139;66.83751;-27.75444;,
 40.79051;35.87825;-3.20481;,
 20.39525;60.85724;-28.18379;,
 36.25824;39.08306;0.00000;,
 18.12912;61.28661;-22.20354;,
 -20.39525;67.26686;-21.77417;,
 -18.12912;61.28661;-22.20354;,
 -22.66139;66.83751;-27.75444;,
 -20.39525;60.85724;-28.18379;,
 -18.12912;61.28661;-22.20354;,
 -40.79051;42.28786;3.20481;,
 -36.25824;39.08305;0.00000;,
 -45.32278;39.08305;0.00000;,
 -40.79051;35.87825;-3.20480;,
 -36.25824;39.08305;0.00000;,
 -20.39525;17.30888;28.18379;,
 -18.12912;16.87952;22.20354;,
 -22.66138;11.32862;27.75444;,
 -20.39525;10.89927;21.77417;,
 -18.12912;16.87952;22.20354;,
 20.39525;17.30888;28.18379;,
 18.12912;16.87952;22.20354;,
 22.66138;11.32862;27.75444;,
 20.39525;10.89927;21.77417;,
 18.12912;16.87952;22.20354;,
 40.79051;42.28786;3.20481;,
 36.25824;39.08306;0.00000;,
 45.32278;39.08306;0.00000;,
 40.79051;35.87825;-3.20481;,
 36.25824;39.08306;0.00000;,
 36.25824;39.08305;0.00000;,
 40.79051;42.28786;-3.20481;,
 20.39525;17.30888;-28.18379;,
 18.12912;16.87952;-22.20354;,
 45.32278;39.08305;0.00000;,
 22.66139;11.32862;-27.75444;,
 40.79051;35.87825;3.20481;,
 20.39525;10.89927;-21.77417;,
 36.25824;39.08305;0.00000;,
 18.12912;16.87952;-22.20354;,
 -20.39525;17.30888;-28.18379;,
 -18.12912;16.87952;-22.20354;,
 -22.66139;11.32862;-27.75444;,
 -20.39525;10.89927;-21.77417;,
 -18.12912;16.87952;-22.20354;,
 -40.79051;42.28786;-3.20480;,
 -36.25824;39.08306;0.00000;,
 -45.32278;39.08306;0.00000;,
 -40.79051;35.87825;3.20481;,
 -36.25824;39.08306;0.00000;,
 -20.39525;67.26686;21.77417;,
 -18.12912;61.28661;22.20354;,
 -22.66138;66.83751;27.75444;,
 -20.39525;60.85724;28.18379;,
 -18.12912;61.28661;22.20354;,
 20.39525;67.26686;21.77417;,
 18.12912;61.28661;22.20354;,
 22.66138;66.83751;27.75444;,
 20.39525;60.85724;28.18379;,
 18.12912;61.28661;22.20354;,
 40.79051;42.28786;-3.20481;,
 36.25824;39.08305;0.00000;,
 45.32278;39.08305;0.00000;,
 40.79051;35.87825;3.20481;,
 36.25824;39.08305;0.00000;,
 36.25824;39.08306;-0.00000;,
 40.79051;43.61534;-0.00000;,
 20.39525;43.61534;-35.32562;,
 18.12912;39.08306;-31.40059;,
 45.32278;39.08306;-0.00000;,
 22.66139;39.08306;-39.25067;,
 40.79051;34.55078;-0.00000;,
 20.39525;34.55078;-35.32562;,
 36.25824;39.08306;-0.00000;,
 18.12912;39.08306;-31.40059;,
 -20.39525;43.61534;-35.32562;,
 -18.12912;39.08306;-31.40058;,
 -22.66139;39.08306;-39.25067;,
 -20.39525;34.55078;-35.32562;,
 -18.12912;39.08306;-31.40058;,
 -40.79051;43.61534;0.00000;,
 -36.25824;39.08306;0.00000;,
 -45.32278;39.08306;0.00000;,
 -40.79051;34.55078;0.00000;,
 -36.25824;39.08306;0.00000;,
 -20.39525;43.61534;35.32562;,
 -18.12912;39.08306;31.40059;,
 -22.66138;39.08306;39.25067;,
 -20.39525;34.55078;35.32562;,
 -18.12912;39.08306;31.40059;,
 20.39525;43.61534;35.32562;,
 18.12912;39.08306;31.40059;,
 22.66138;39.08306;39.25067;,
 20.39525;34.55078;35.32562;,
 18.12912;39.08306;31.40059;,
 40.79051;43.61534;-0.00000;,
 36.25824;39.08306;-0.00000;,
 45.32278;39.08306;-0.00000;,
 40.79051;34.55078;-0.00000;,
 36.25824;39.08306;-0.00000;;
 
 96;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;11,10,15,16;,
 4;10,12,17,15;,
 4;12,13,18,17;,
 4;13,14,19,18;,
 4;16,15,20,21;,
 4;15,17,22,20;,
 4;17,18,23,22;,
 4;18,19,24,23;,
 4;21,20,25,26;,
 4;20,22,27,25;,
 4;22,23,28,27;,
 4;23,24,29,28;,
 4;26,25,30,31;,
 4;25,27,32,30;,
 4;27,28,33,32;,
 4;28,29,34,33;,
 4;35,36,37,38;,
 4;36,39,40,37;,
 4;39,41,42,40;,
 4;41,43,44,42;,
 4;38,37,45,46;,
 4;37,40,47,45;,
 4;40,42,48,47;,
 4;42,44,49,48;,
 4;46,45,50,51;,
 4;45,47,52,50;,
 4;47,48,53,52;,
 4;48,49,54,53;,
 4;51,50,55,56;,
 4;50,52,57,55;,
 4;52,53,58,57;,
 4;53,54,59,58;,
 4;56,55,60,61;,
 4;55,57,62,60;,
 4;57,58,63,62;,
 4;58,59,64,63;,
 4;61,60,65,66;,
 4;60,62,67,65;,
 4;62,63,68,67;,
 4;63,64,69,68;,
 4;70,71,72,73;,
 4;71,74,75,72;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;73,72,80,81;,
 4;72,75,82,80;,
 4;75,77,83,82;,
 4;77,79,84,83;,
 4;81,80,85,86;,
 4;80,82,87,85;,
 4;82,83,88,87;,
 4;83,84,89,88;,
 4;86,85,90,91;,
 4;85,87,92,90;,
 4;87,88,93,92;,
 4;88,89,94,93;,
 4;91,90,95,96;,
 4;90,92,97,95;,
 4;92,93,98,97;,
 4;93,94,99,98;,
 4;96,95,100,101;,
 4;95,97,102,100;,
 4;97,98,103,102;,
 4;98,99,104,103;,
 4;105,106,107,108;,
 4;106,109,110,107;,
 4;109,111,112,110;,
 4;111,113,114,112;,
 4;108,107,115,116;,
 4;107,110,117,115;,
 4;110,112,118,117;,
 4;112,114,119,118;,
 4;116,115,120,121;,
 4;115,117,122,120;,
 4;117,118,123,122;,
 4;118,119,124,123;,
 4;121,120,125,126;,
 4;120,122,127,125;,
 4;122,123,128,127;,
 4;123,124,129,128;,
 4;126,125,130,131;,
 4;125,127,132,130;,
 4;127,128,133,132;,
 4;128,129,134,133;,
 4;131,130,135,136;,
 4;130,132,137,135;,
 4;132,133,138,137;,
 4;133,134,139,138;;
 
 MeshMaterialList {
  1;
  96;
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
   0.065882;0.000000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  96;
  -0.707108;0.000000;0.707105;,
  0.707108;-0.000000;0.707105;,
  0.707108;-0.000000;-0.707105;,
  -0.353554;-0.612374;0.707105;,
  0.353554;0.612374;0.707105;,
  0.353554;0.612374;-0.707105;,
  0.353554;-0.612374;0.707105;,
  -0.353554;0.612374;0.707105;,
  -0.353554;0.612374;-0.707105;,
  0.707108;0.000000;0.707105;,
  -0.707108;-0.000000;0.707105;,
  -0.707108;-0.000000;-0.707105;,
  0.353554;0.612375;0.707104;,
  -0.353554;-0.612374;0.707105;,
  -0.353554;-0.612374;-0.707105;,
  -0.353554;0.612375;0.707104;,
  0.353554;-0.612374;0.707105;,
  0.353554;-0.612374;-0.707105;,
  -0.707108;0.499999;0.499999;,
  0.707107;0.500000;0.500000;,
  0.707107;-0.500000;-0.500000;,
  -0.353554;0.066986;0.933013;,
  0.353553;0.933013;0.066989;,
  0.353554;-0.066988;-0.933013;,
  0.353554;0.066986;0.933013;,
  -0.353554;0.933013;0.066989;,
  -0.353554;-0.066988;-0.933013;,
  0.707108;0.499999;0.499999;,
  -0.707107;0.500000;0.500000;,
  -0.707107;-0.500000;-0.500000;,
  0.353554;0.933013;0.066986;,
  -0.353554;0.066989;0.933013;,
  -0.353554;-0.933013;-0.066989;,
  -0.353554;0.933013;0.066987;,
  0.353554;0.066989;0.933013;,
  0.353554;-0.933013;-0.066989;,
  -0.707108;0.499999;-0.499999;,
  0.707107;0.500000;-0.500000;,
  0.707107;-0.500000;0.500000;,
  -0.353554;0.933013;-0.066986;,
  0.353554;0.066988;-0.933013;,
  0.353553;-0.933013;0.066989;,
  0.353554;0.933013;-0.066986;,
  -0.353554;0.066988;-0.933013;,
  -0.353553;-0.933013;0.066989;,
  0.707108;0.499999;-0.499999;,
  -0.707107;0.500000;-0.500000;,
  -0.707107;-0.500000;0.500000;,
  0.353554;0.066986;-0.933013;,
  -0.353554;0.933012;-0.066989;,
  -0.353554;-0.066989;0.933013;,
  -0.353554;0.066986;-0.933013;,
  0.353554;0.933012;-0.066989;,
  0.353554;-0.066989;0.933013;,
  -0.707108;0.707105;0.000000;,
  0.707108;0.707106;0.000000;,
  0.707108;-0.707105;0.000000;,
  -0.353554;0.707104;0.612375;,
  0.353554;0.707105;-0.612374;,
  0.353554;-0.707105;-0.612374;,
  0.353554;0.707104;0.612375;,
  -0.353554;0.707105;-0.612374;,
  -0.353554;-0.707105;-0.612374;,
  0.707108;0.707105;-0.000000;,
  -0.707108;0.707106;0.000000;,
  -0.707108;-0.707105;0.000000;,
  0.353554;0.707104;-0.612375;,
  -0.353554;0.707105;0.612374;,
  -0.353554;-0.707105;0.612374;,
  -0.353554;0.707104;-0.612375;,
  0.353554;0.707105;0.612374;,
  0.353554;-0.707105;0.612374;,
  -0.707108;0.000000;-0.707105;,
  -0.353554;-0.612374;-0.707105;,
  0.353554;-0.612374;-0.707105;,
  0.707109;0.000000;-0.707105;,
  0.353554;0.612375;-0.707104;,
  -0.353554;0.612375;-0.707104;,
  -0.707108;-0.499999;-0.499999;,
  -0.353554;-0.933013;-0.066986;,
  0.353554;-0.933012;-0.066986;,
  0.707108;-0.499999;-0.499999;,
  0.353554;-0.066986;-0.933013;,
  -0.353554;-0.066986;-0.933013;,
  -0.707108;-0.499999;0.499999;,
  -0.353554;-0.066986;0.933013;,
  0.353554;-0.066986;0.933013;,
  0.707108;-0.499999;0.499999;,
  0.353554;-0.933013;0.066986;,
  -0.353554;-0.933013;0.066986;,
  -0.707109;-0.707105;-0.000000;,
  -0.353554;-0.707104;0.612375;,
  0.353554;-0.707104;0.612375;,
  0.707109;-0.707105;-0.000000;,
  0.353554;-0.707104;-0.612375;,
  -0.353554;-0.707104;-0.612375;;
  96;
  4;0,0,3,3;,
  4;1,1,4,4;,
  4;2,2,5,5;,
  4;72,72,73,73;,
  4;3,3,6,6;,
  4;4,4,7,7;,
  4;5,5,8,8;,
  4;73,73,74,74;,
  4;6,6,9,9;,
  4;7,7,10,10;,
  4;8,8,11,11;,
  4;74,74,75,75;,
  4;9,9,12,12;,
  4;10,10,13,13;,
  4;11,11,14,14;,
  4;75,75,76,76;,
  4;12,12,15,15;,
  4;13,13,16,16;,
  4;14,14,17,17;,
  4;76,76,77,77;,
  4;15,15,0,0;,
  4;16,16,1,1;,
  4;17,17,2,2;,
  4;77,77,72,72;,
  4;18,18,21,21;,
  4;19,19,22,22;,
  4;20,20,23,23;,
  4;78,78,79,79;,
  4;21,21,24,24;,
  4;22,22,25,25;,
  4;23,23,26,26;,
  4;79,79,80,80;,
  4;24,24,27,27;,
  4;25,25,28,28;,
  4;26,26,29,29;,
  4;80,80,81,81;,
  4;27,27,30,30;,
  4;28,28,31,31;,
  4;29,29,32,32;,
  4;81,81,82,82;,
  4;30,30,33,33;,
  4;31,31,34,34;,
  4;32,32,35,35;,
  4;82,82,83,83;,
  4;33,33,18,18;,
  4;34,34,19,19;,
  4;35,35,20,20;,
  4;83,83,78,78;,
  4;36,36,39,39;,
  4;37,37,40,40;,
  4;38,38,41,41;,
  4;84,84,85,85;,
  4;39,39,42,42;,
  4;40,40,43,43;,
  4;41,41,44,44;,
  4;85,85,86,86;,
  4;42,42,45,45;,
  4;43,43,46,46;,
  4;44,44,47,47;,
  4;86,86,87,87;,
  4;45,45,48,48;,
  4;46,46,49,49;,
  4;47,47,50,50;,
  4;87,87,88,88;,
  4;48,48,51,51;,
  4;49,49,52,52;,
  4;50,50,53,53;,
  4;88,88,89,89;,
  4;51,51,36,36;,
  4;52,52,37,37;,
  4;53,53,38,38;,
  4;89,89,84,84;,
  4;54,54,57,57;,
  4;55,55,58,58;,
  4;56,56,59,59;,
  4;90,90,91,91;,
  4;57,57,60,60;,
  4;58,58,61,61;,
  4;59,59,62,62;,
  4;91,91,92,92;,
  4;60,60,63,63;,
  4;61,61,64,64;,
  4;62,62,65,65;,
  4;92,92,93,93;,
  4;63,63,66,66;,
  4;64,64,67,67;,
  4;65,65,68,68;,
  4;93,93,94,94;,
  4;66,66,69,69;,
  4;67,67,70,70;,
  4;68,68,71,71;,
  4;94,94,95,95;,
  4;69,69,54,54;,
  4;70,70,55,55;,
  4;71,71,56,56;,
  4;95,95,90,90;;
 }
 MeshTextureCoords {
  140;
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.166667;0.250000;,
  0.166667;0.000000;,
  0.000000;0.500000;,
  0.166667;0.500000;,
  0.000000;0.750000;,
  0.166667;0.750000;,
  0.000000;1.000000;,
  0.166667;1.000000;,
  0.333333;0.250000;,
  0.333333;0.000000;,
  0.333333;0.500000;,
  0.333333;0.750000;,
  0.333333;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.666667;0.250000;,
  0.666667;0.000000;,
  0.666667;0.500000;,
  0.666667;0.750000;,
  0.666667;1.000000;,
  0.833333;0.250000;,
  0.833333;0.000000;,
  0.833333;0.500000;,
  0.833333;0.750000;,
  0.833333;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.166667;0.250000;,
  0.166667;0.000000;,
  0.000000;0.500000;,
  0.166667;0.500000;,
  0.000000;0.750000;,
  0.166667;0.750000;,
  0.000000;1.000000;,
  0.166667;1.000000;,
  0.333333;0.250000;,
  0.333333;0.000000;,
  0.333333;0.500000;,
  0.333333;0.750000;,
  0.333333;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.666667;0.250000;,
  0.666667;0.000000;,
  0.666667;0.500000;,
  0.666667;0.750000;,
  0.666667;1.000000;,
  0.833333;0.250000;,
  0.833333;0.000000;,
  0.833333;0.500000;,
  0.833333;0.750000;,
  0.833333;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.166667;0.250000;,
  0.166667;0.000000;,
  0.000000;0.500000;,
  0.166667;0.500000;,
  0.000000;0.750000;,
  0.166667;0.750000;,
  0.000000;1.000000;,
  0.166667;1.000000;,
  0.333333;0.250000;,
  0.333333;0.000000;,
  0.333333;0.500000;,
  0.333333;0.750000;,
  0.333333;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.666667;0.250000;,
  0.666667;0.000000;,
  0.666667;0.500000;,
  0.666667;0.750000;,
  0.666667;1.000000;,
  0.833333;0.250000;,
  0.833333;0.000000;,
  0.833333;0.500000;,
  0.833333;0.750000;,
  0.833333;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.166667;0.250000;,
  0.166667;0.000000;,
  0.000000;0.500000;,
  0.166667;0.500000;,
  0.000000;0.750000;,
  0.166667;0.750000;,
  0.000000;1.000000;,
  0.166667;1.000000;,
  0.333333;0.250000;,
  0.333333;0.000000;,
  0.333333;0.500000;,
  0.333333;0.750000;,
  0.333333;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.666667;0.250000;,
  0.666667;0.000000;,
  0.666667;0.500000;,
  0.666667;0.750000;,
  0.666667;1.000000;,
  0.833333;0.250000;,
  0.833333;0.000000;,
  0.833333;0.500000;,
  0.833333;0.750000;,
  0.833333;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;;
 }
}
