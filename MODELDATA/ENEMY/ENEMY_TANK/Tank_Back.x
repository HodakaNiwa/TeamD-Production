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
 340;
 52.62003;11.80491;3.60141;,
 52.62003;12.40641;-0.19638;,
 58.68558;12.40641;-0.19638;,
 58.68558;11.80491;3.60141;,
 52.62003;10.05927;7.02747;,
 58.68558;10.05927;7.02747;,
 52.62003;7.34034;9.74643;,
 58.68558;7.34034;9.74643;,
 52.62003;3.91428;11.49204;,
 58.68558;3.91428;11.49204;,
 52.62003;0.11649;12.09354;,
 58.68558;0.11649;12.09354;,
 52.62003;-3.68130;11.49204;,
 58.68558;-3.68130;11.49204;,
 52.62003;-7.10736;9.74643;,
 58.68558;-7.10736;9.74643;,
 52.62003;-9.82629;7.02747;,
 58.68558;-9.82629;7.02747;,
 52.62003;-11.57193;3.60141;,
 58.68558;-11.57193;3.60141;,
 52.62003;-12.17343;-0.19638;,
 58.68558;-12.17343;-0.19638;,
 52.62003;-11.57193;-3.99414;,
 58.68558;-11.57193;-3.99414;,
 52.62003;-9.82629;-7.42023;,
 58.68558;-9.82629;-7.42023;,
 52.62003;-7.10736;-10.13913;,
 58.68558;-7.10736;-10.13913;,
 52.62003;-3.68130;-11.88480;,
 58.68558;-3.68130;-11.88480;,
 52.62003;0.11649;-12.48630;,
 58.68558;0.11649;-12.48630;,
 52.62003;3.91428;-11.88480;,
 58.68558;3.91428;-11.88480;,
 52.62003;7.34034;-10.13913;,
 58.68558;7.34034;-10.13913;,
 52.62003;10.05927;-7.42023;,
 58.68558;10.05927;-7.42023;,
 52.62003;11.80491;-3.99414;,
 58.68558;11.80491;-3.99414;,
 52.62003;12.40641;-0.19638;,
 58.68558;12.40641;-0.19638;,
 58.75035;13.70721;-0.19638;,
 58.75035;13.04205;4.00338;,
 58.75035;11.11161;7.79205;,
 58.75035;8.10492;10.79877;,
 58.75035;4.31625;12.72918;,
 58.75035;0.11649;13.39440;,
 58.75035;-4.08327;12.72918;,
 58.75035;-7.87194;10.79877;,
 58.75035;-10.87866;7.79205;,
 58.75035;-12.80907;4.00338;,
 58.75035;-13.47426;-0.19638;,
 58.75035;-12.80907;-4.39614;,
 58.75035;-10.87866;-8.18481;,
 58.75035;-7.87194;-11.19150;,
 58.75035;-4.08327;-13.12194;,
 58.75035;0.11649;-13.78713;,
 58.75035;4.31625;-13.12194;,
 58.75035;8.10492;-11.19150;,
 58.75035;11.11164;-8.18481;,
 58.75035;13.04205;-4.39614;,
 58.75035;13.70721;-0.19638;,
 52.55526;13.70721;-0.19638;,
 52.55526;13.04205;4.00338;,
 52.55526;11.11161;7.79205;,
 52.55526;8.10492;10.79877;,
 52.55526;4.31625;12.72918;,
 52.55526;0.11649;13.39440;,
 52.55526;-4.08327;12.72918;,
 52.55526;-7.87194;10.79877;,
 52.55526;-10.87866;7.79205;,
 52.55526;-12.80907;4.00338;,
 52.55526;-13.47426;-0.19638;,
 52.55526;-12.80907;-4.39614;,
 52.55526;-10.87866;-8.18481;,
 52.55526;-7.87194;-11.19150;,
 52.55526;-4.08327;-13.12194;,
 52.55526;0.11649;-13.78713;,
 52.55526;4.31625;-13.12194;,
 52.55526;8.10492;-11.19150;,
 52.55526;11.11164;-8.18481;,
 52.55526;13.04205;-4.39614;,
 52.55526;13.70721;-0.19638;,
 52.62003;12.40641;-0.19638;,
 52.62003;11.80491;3.60141;,
 52.62003;10.05927;7.02747;,
 52.62003;7.34034;9.74643;,
 52.62003;3.91428;11.49204;,
 52.62003;0.11649;12.09354;,
 52.62003;-3.68130;11.49204;,
 52.62003;-7.10736;9.74643;,
 52.62003;-9.82629;7.02747;,
 52.62003;-11.57193;3.60141;,
 52.62003;-12.17343;-0.19638;,
 52.62003;-11.57193;-3.99414;,
 52.62003;-9.82629;-7.42023;,
 52.62003;-7.10736;-10.13913;,
 52.62003;-3.68130;-11.88480;,
 52.62003;0.11649;-12.48630;,
 52.62003;3.91428;-11.88480;,
 52.62003;7.34034;-10.13913;,
 52.62003;10.05927;-7.42023;,
 52.62003;11.80491;-3.99414;,
 52.62003;12.40641;-0.19638;,
 56.26566;10.47771;6.90909;,
 54.06078;1.97058;5.50998;,
 58.83696;1.97058;5.50998;,
 56.63205;10.47771;6.90909;,
 56.26566;-3.03768;11.30049;,
 56.63205;-3.03768;11.30049;,
 54.06078;-4.73763;3.33036;,
 58.83696;-4.73763;3.33036;,
 56.26566;-11.39064;-0.19638;,
 56.63205;-11.39064;-0.19638;,
 54.06078;-4.73763;-3.72306;,
 58.83696;-4.73763;-3.72306;,
 56.26566;-3.03768;-11.69322;,
 56.63205;-3.03768;-11.69322;,
 54.06078;1.97058;-5.90268;,
 58.83696;1.97058;-5.90268;,
 56.26566;10.47771;-7.30182;,
 56.63205;10.47771;-7.30182;,
 54.06078;6.11649;-0.19638;,
 58.83696;6.11649;-0.19638;,
 56.26566;10.47771;6.90909;,
 56.63205;10.47771;6.90909;,
 54.06078;1.97058;5.50998;,
 56.26566;10.47771;6.90909;,
 51.94449;0.11649;-0.19638;,
 56.26566;-3.03768;11.30049;,
 54.06078;-4.73763;3.33036;,
 56.26566;-11.39064;-0.19638;,
 54.06078;-4.73763;-3.72306;,
 56.26566;-3.03768;-11.69322;,
 54.06078;1.97058;-5.90268;,
 56.26566;10.47771;-7.30182;,
 54.06078;6.11649;-0.19638;,
 56.63205;10.47771;6.90909;,
 58.83696;1.97058;5.50998;,
 60.95325;0.11649;-0.19638;,
 56.63205;-3.03768;11.30049;,
 58.83696;-4.73763;3.33036;,
 56.63205;-11.39064;-0.19638;,
 58.83696;-4.73763;-3.72306;,
 56.63205;-3.03768;-11.69322;,
 58.83696;1.97058;-5.90268;,
 56.63205;10.47771;-7.30182;,
 58.83696;6.11649;-0.19638;,
 -54.42921;2.11803;-0.23397;,
 -54.42921;1.69191;0.60234;,
 54.42921;1.69191;0.60234;,
 54.42921;2.11803;-0.23397;,
 -54.42921;1.02822;1.26603;,
 54.42921;1.02822;1.26603;,
 -54.42921;0.19191;1.69215;,
 54.42921;0.19191;1.69215;,
 -54.42921;-0.73515;1.83897;,
 54.42921;-0.73515;1.83897;,
 -54.42921;-1.66218;1.69215;,
 54.42921;-1.66218;1.69215;,
 -54.42921;-2.49849;1.26603;,
 54.42921;-2.49849;1.26603;,
 -54.42921;-3.16218;0.60234;,
 54.42921;-3.16218;0.60234;,
 -54.42921;-3.58830;-0.23397;,
 54.42921;-3.58830;-0.23397;,
 -54.42921;-3.73515;-1.16103;,
 54.42921;-3.73515;-1.16103;,
 -54.42921;-3.58830;-2.08809;,
 54.42921;-3.58830;-2.08809;,
 -54.42921;-3.16218;-2.92440;,
 54.42921;-3.16218;-2.92440;,
 -54.42921;-2.49849;-3.58806;,
 54.42921;-2.49849;-3.58806;,
 -54.42921;-1.66218;-4.01418;,
 54.42921;-1.66218;-4.01418;,
 -54.42921;-0.73515;-4.16103;,
 54.42921;-0.73515;-4.16103;,
 -54.42921;0.19191;-4.01418;,
 54.42921;0.19191;-4.01418;,
 -54.42921;1.02822;-3.58806;,
 54.42921;1.02822;-3.58806;,
 -54.42921;1.69191;-2.92440;,
 54.42921;1.69191;-2.92440;,
 -54.42921;2.11803;-2.08809;,
 54.42921;2.11803;-2.08809;,
 -54.42921;2.26485;-1.16103;,
 54.42921;2.26485;-1.16103;,
 -54.42921;2.11803;-0.23397;,
 54.42921;2.11803;-0.23397;,
 -55.23963;10.47771;6.90909;,
 -57.44454;1.97058;5.50998;,
 -52.66833;1.97058;5.50998;,
 -54.87324;10.47771;6.90909;,
 -55.23963;-3.03768;11.30049;,
 -54.87324;-3.03768;11.30049;,
 -57.44454;-4.73763;3.33036;,
 -52.66833;-4.73763;3.33036;,
 -55.23963;-11.39064;-0.19638;,
 -54.87324;-11.39064;-0.19638;,
 -57.44454;-4.73763;-3.72306;,
 -52.66833;-4.73763;-3.72306;,
 -55.23963;-3.03768;-11.69322;,
 -54.87324;-3.03768;-11.69322;,
 -57.44454;1.97058;-5.90268;,
 -52.66833;1.97058;-5.90268;,
 -55.23963;10.47771;-7.30182;,
 -54.87324;10.47771;-7.30182;,
 -57.44454;6.11649;-0.19638;,
 -52.66833;6.11649;-0.19638;,
 -55.23963;10.47771;6.90909;,
 -54.87324;10.47771;6.90909;,
 -57.44454;1.97058;5.50998;,
 -55.23963;10.47771;6.90909;,
 -59.56080;0.11649;-0.19638;,
 -55.23963;-3.03768;11.30049;,
 -57.44454;-4.73763;3.33036;,
 -55.23963;-11.39064;-0.19638;,
 -57.44454;-4.73763;-3.72306;,
 -55.23963;-3.03768;-11.69322;,
 -57.44454;1.97058;-5.90268;,
 -55.23963;10.47771;-7.30182;,
 -57.44454;6.11649;-0.19638;,
 -54.87324;10.47771;6.90909;,
 -52.66833;1.97058;5.50998;,
 -50.55207;0.11649;-0.19638;,
 -54.87324;-3.03768;11.30049;,
 -52.66833;-4.73763;3.33036;,
 -54.87324;-11.39064;-0.19638;,
 -52.66833;-4.73763;-3.72306;,
 -54.87324;-3.03768;-11.69322;,
 -52.66833;1.97058;-5.90268;,
 -54.87324;10.47771;-7.30182;,
 -52.66833;6.11649;-0.19638;,
 -58.08921;11.80491;3.60141;,
 -58.08921;12.40641;-0.19638;,
 -52.02366;12.40641;-0.19638;,
 -52.02366;11.80491;3.60141;,
 -58.08921;10.05927;7.02747;,
 -52.02366;10.05927;7.02747;,
 -58.08921;7.34034;9.74643;,
 -52.02366;7.34034;9.74643;,
 -58.08921;3.91428;11.49204;,
 -52.02366;3.91428;11.49204;,
 -58.08921;0.11649;12.09354;,
 -52.02366;0.11649;12.09354;,
 -58.08921;-3.68130;11.49204;,
 -52.02366;-3.68130;11.49204;,
 -58.08921;-7.10736;9.74643;,
 -52.02366;-7.10736;9.74643;,
 -58.08921;-9.82629;7.02747;,
 -52.02366;-9.82629;7.02747;,
 -58.08921;-11.57193;3.60141;,
 -52.02366;-11.57193;3.60141;,
 -58.08921;-12.17343;-0.19638;,
 -52.02366;-12.17343;-0.19638;,
 -58.08921;-11.57193;-3.99414;,
 -52.02366;-11.57193;-3.99414;,
 -58.08921;-9.82629;-7.42023;,
 -52.02366;-9.82629;-7.42023;,
 -58.08921;-7.10736;-10.13913;,
 -52.02366;-7.10736;-10.13913;,
 -58.08921;-3.68130;-11.88480;,
 -52.02366;-3.68130;-11.88480;,
 -58.08921;0.11649;-12.48630;,
 -52.02366;0.11649;-12.48630;,
 -58.08921;3.91428;-11.88480;,
 -52.02366;3.91428;-11.88480;,
 -58.08921;7.34034;-10.13913;,
 -52.02366;7.34034;-10.13913;,
 -58.08921;10.05927;-7.42023;,
 -52.02366;10.05927;-7.42023;,
 -58.08921;11.80491;-3.99414;,
 -52.02366;11.80491;-3.99414;,
 -58.08921;12.40641;-0.19638;,
 -52.02366;12.40641;-0.19638;,
 -51.95889;13.70721;-0.19638;,
 -51.95889;13.04205;4.00338;,
 -51.95889;11.11161;7.79205;,
 -51.95889;8.10492;10.79877;,
 -51.95889;4.31625;12.72918;,
 -51.95889;0.11649;13.39440;,
 -51.95889;-4.08327;12.72918;,
 -51.95889;-7.87194;10.79877;,
 -51.95889;-10.87866;7.79205;,
 -51.95889;-12.80907;4.00338;,
 -51.95889;-13.47426;-0.19638;,
 -51.95889;-12.80907;-4.39614;,
 -51.95889;-10.87866;-8.18481;,
 -51.95889;-7.87194;-11.19150;,
 -51.95889;-4.08327;-13.12194;,
 -51.95889;0.11649;-13.78713;,
 -51.95889;4.31625;-13.12194;,
 -51.95889;8.10492;-11.19150;,
 -51.95889;11.11164;-8.18481;,
 -51.95889;13.04205;-4.39614;,
 -51.95889;13.70721;-0.19638;,
 -58.15398;13.70721;-0.19638;,
 -58.15398;13.04205;4.00338;,
 -58.15398;11.11161;7.79205;,
 -58.15398;8.10492;10.79877;,
 -58.15398;4.31625;12.72918;,
 -58.15398;0.11649;13.39440;,
 -58.15398;-4.08327;12.72918;,
 -58.15398;-7.87194;10.79877;,
 -58.15398;-10.87866;7.79205;,
 -58.15398;-12.80907;4.00338;,
 -58.15398;-13.47426;-0.19638;,
 -58.15398;-12.80907;-4.39614;,
 -58.15398;-10.87866;-8.18481;,
 -58.15398;-7.87194;-11.19150;,
 -58.15398;-4.08327;-13.12194;,
 -58.15398;0.11649;-13.78713;,
 -58.15398;4.31625;-13.12194;,
 -58.15398;8.10492;-11.19150;,
 -58.15398;11.11164;-8.18481;,
 -58.15398;13.04205;-4.39614;,
 -58.15398;13.70721;-0.19638;,
 -58.08921;12.40641;-0.19638;,
 -58.08921;11.80491;3.60141;,
 -58.08921;10.05927;7.02747;,
 -58.08921;7.34034;9.74643;,
 -58.08921;3.91428;11.49204;,
 -58.08921;0.11649;12.09354;,
 -58.08921;-3.68130;11.49204;,
 -58.08921;-7.10736;9.74643;,
 -58.08921;-9.82629;7.02747;,
 -58.08921;-11.57193;3.60141;,
 -58.08921;-12.17343;-0.19638;,
 -58.08921;-11.57193;-3.99414;,
 -58.08921;-9.82629;-7.42023;,
 -58.08921;-7.10736;-10.13913;,
 -58.08921;-3.68130;-11.88480;,
 -58.08921;0.11649;-12.48630;,
 -58.08921;3.91428;-11.88480;,
 -58.08921;7.34034;-10.13913;,
 -58.08921;10.05927;-7.42023;,
 -58.08921;11.80491;-3.99414;,
 -58.08921;12.40641;-0.19638;;
 
 240;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;18,16,17,19;,
 4;20,18,19,21;,
 4;22,20,21,23;,
 4;24,22,23,25;,
 4;26,24,25,27;,
 4;28,26,27,29;,
 4;30,28,29,31;,
 4;32,30,31,33;,
 4;34,32,33,35;,
 4;36,34,35,37;,
 4;38,36,37,39;,
 4;40,38,39,41;,
 4;3,2,42,43;,
 4;5,3,43,44;,
 4;7,5,44,45;,
 4;9,7,45,46;,
 4;11,9,46,47;,
 4;13,11,47,48;,
 4;15,13,48,49;,
 4;17,15,49,50;,
 4;19,17,50,51;,
 4;21,19,51,52;,
 4;23,21,52,53;,
 4;25,23,53,54;,
 4;27,25,54,55;,
 4;29,27,55,56;,
 4;31,29,56,57;,
 4;33,31,57,58;,
 4;35,33,58,59;,
 4;37,35,59,60;,
 4;39,37,60,61;,
 4;41,39,61,62;,
 4;43,42,63,64;,
 4;44,43,64,65;,
 4;45,44,65,66;,
 4;46,45,66,67;,
 4;47,46,67,68;,
 4;48,47,68,69;,
 4;49,48,69,70;,
 4;50,49,70,71;,
 4;51,50,71,72;,
 4;52,51,72,73;,
 4;53,52,73,74;,
 4;54,53,74,75;,
 4;55,54,75,76;,
 4;56,55,76,77;,
 4;57,56,77,78;,
 4;58,57,78,79;,
 4;59,58,79,80;,
 4;60,59,80,81;,
 4;61,60,81,82;,
 4;62,61,82,83;,
 4;64,63,84,85;,
 4;65,64,85,86;,
 4;66,65,86,87;,
 4;67,66,87,88;,
 4;68,67,88,89;,
 4;69,68,89,90;,
 4;70,69,90,91;,
 4;71,70,91,92;,
 4;72,71,92,93;,
 4;73,72,93,94;,
 4;74,73,94,95;,
 4;75,74,95,96;,
 4;76,75,96,97;,
 4;77,76,97,98;,
 4;78,77,98,99;,
 4;79,78,99,100;,
 4;80,79,100,101;,
 4;81,80,101,102;,
 4;82,81,102,103;,
 4;83,82,103,104;,
 4;105,106,107,108;,
 4;106,109,110,107;,
 4;109,111,112,110;,
 4;111,113,114,112;,
 4;113,115,116,114;,
 4;115,117,118,116;,
 4;117,119,120,118;,
 4;119,121,122,120;,
 4;121,123,124,122;,
 4;123,125,126,124;,
 3;127,128,129;,
 3;130,127,129;,
 3;131,130,129;,
 3;132,131,129;,
 3;133,132,129;,
 3;134,133,129;,
 3;135,134,129;,
 3;136,135,129;,
 3;137,136,129;,
 3;128,137,129;,
 3;138,139,140;,
 3;139,141,140;,
 3;141,142,140;,
 3;142,143,140;,
 3;143,144,140;,
 3;144,145,140;,
 3;145,146,140;,
 3;146,147,140;,
 3;147,148,140;,
 3;148,138,140;,
 4;149,150,151,152;,
 4;150,153,154,151;,
 4;153,155,156,154;,
 4;155,157,158,156;,
 4;157,159,160,158;,
 4;159,161,162,160;,
 4;161,163,164,162;,
 4;163,165,166,164;,
 4;165,167,168,166;,
 4;167,169,170,168;,
 4;169,171,172,170;,
 4;171,173,174,172;,
 4;173,175,176,174;,
 4;175,177,178,176;,
 4;177,179,180,178;,
 4;179,181,182,180;,
 4;181,183,184,182;,
 4;183,185,186,184;,
 4;185,187,188,186;,
 4;187,189,190,188;,
 4;191,192,193,194;,
 4;192,195,196,193;,
 4;195,197,198,196;,
 4;197,199,200,198;,
 4;199,201,202,200;,
 4;201,203,204,202;,
 4;203,205,206,204;,
 4;205,207,208,206;,
 4;207,209,210,208;,
 4;209,211,212,210;,
 3;213,214,215;,
 3;216,213,215;,
 3;217,216,215;,
 3;218,217,215;,
 3;219,218,215;,
 3;220,219,215;,
 3;221,220,215;,
 3;222,221,215;,
 3;223,222,215;,
 3;214,223,215;,
 3;224,225,226;,
 3;225,227,226;,
 3;227,228,226;,
 3;228,229,226;,
 3;229,230,226;,
 3;230,231,226;,
 3;231,232,226;,
 3;232,233,226;,
 3;233,234,226;,
 3;234,224,226;,
 4;235,236,237,238;,
 4;239,235,238,240;,
 4;241,239,240,242;,
 4;243,241,242,244;,
 4;245,243,244,246;,
 4;247,245,246,248;,
 4;249,247,248,250;,
 4;251,249,250,252;,
 4;253,251,252,254;,
 4;255,253,254,256;,
 4;257,255,256,258;,
 4;259,257,258,260;,
 4;261,259,260,262;,
 4;263,261,262,264;,
 4;265,263,264,266;,
 4;267,265,266,268;,
 4;269,267,268,270;,
 4;271,269,270,272;,
 4;273,271,272,274;,
 4;275,273,274,276;,
 4;238,237,277,278;,
 4;240,238,278,279;,
 4;242,240,279,280;,
 4;244,242,280,281;,
 4;246,244,281,282;,
 4;248,246,282,283;,
 4;250,248,283,284;,
 4;252,250,284,285;,
 4;254,252,285,286;,
 4;256,254,286,287;,
 4;258,256,287,288;,
 4;260,258,288,289;,
 4;262,260,289,290;,
 4;264,262,290,291;,
 4;266,264,291,292;,
 4;268,266,292,293;,
 4;270,268,293,294;,
 4;272,270,294,295;,
 4;274,272,295,296;,
 4;276,274,296,297;,
 4;278,277,298,299;,
 4;279,278,299,300;,
 4;280,279,300,301;,
 4;281,280,301,302;,
 4;282,281,302,303;,
 4;283,282,303,304;,
 4;284,283,304,305;,
 4;285,284,305,306;,
 4;286,285,306,307;,
 4;287,286,307,308;,
 4;288,287,308,309;,
 4;289,288,309,310;,
 4;290,289,310,311;,
 4;291,290,311,312;,
 4;292,291,312,313;,
 4;293,292,313,314;,
 4;294,293,314,315;,
 4;295,294,315,316;,
 4;296,295,316,317;,
 4;297,296,317,318;,
 4;299,298,319,320;,
 4;300,299,320,321;,
 4;301,300,321,322;,
 4;302,301,322,323;,
 4;303,302,323,324;,
 4;304,303,324,325;,
 4;305,304,325,326;,
 4;306,305,326,327;,
 4;307,306,327,328;,
 4;308,307,328,329;,
 4;309,308,329,330;,
 4;310,309,330,331;,
 4;311,310,331,332;,
 4;312,311,332,333;,
 4;313,312,333,334;,
 4;314,313,334,335;,
 4;315,314,335,336;,
 4;316,315,336,337;,
 4;317,316,337,338;,
 4;318,317,338,339;;
 
 MeshMaterialList {
  1;
  240;
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
    "ENEMY_TANK_UV.png";
   }
  }
 }
 MeshNormals {
  186;
  0.000000;-1.000000;0.000001;,
  0.000000;-0.951058;-0.309013;,
  0.000000;-0.809020;-0.587781;,
  0.000000;-0.587782;-0.809019;,
  0.000000;-0.309010;-0.951059;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.309010;-0.951059;,
  0.000000;0.587782;-0.809019;,
  0.000000;0.809020;-0.587781;,
  0.000000;0.951058;-0.309013;,
  0.000000;1.000000;0.000001;,
  0.000000;0.951058;0.309012;,
  0.000000;0.809018;0.587784;,
  0.000000;0.587784;0.809018;,
  0.000000;0.309016;0.951057;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.309016;0.951057;,
  0.000000;-0.587784;0.809018;,
  0.000000;-0.809018;0.587784;,
  0.000000;-0.951058;0.309012;,
  0.998763;-0.049731;0.000000;,
  0.998763;-0.047297;-0.015368;,
  0.998763;-0.040234;-0.029232;,
  0.998763;-0.029232;-0.040234;,
  0.998763;-0.015368;-0.047297;,
  0.998763;-0.000000;-0.049730;,
  0.998763;0.015368;-0.047297;,
  0.998763;0.029231;-0.040234;,
  0.998763;0.040234;-0.029231;,
  0.998763;0.047297;-0.015368;,
  0.998763;0.049731;0.000000;,
  0.998763;0.047297;0.015368;,
  0.998763;0.040233;0.029231;,
  0.998763;0.029231;0.040234;,
  0.998763;0.015368;0.047297;,
  0.998763;0.000000;0.049731;,
  0.998763;-0.015368;0.047297;,
  0.998763;-0.029231;0.040234;,
  0.998763;-0.040233;0.029231;,
  0.998763;-0.047297;0.015368;,
  0.000000;1.000000;0.000000;,
  0.000000;0.951057;0.309017;,
  0.000000;0.809017;0.587785;,
  0.000000;0.587786;0.809016;,
  0.000000;0.309020;0.951055;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.309020;0.951055;,
  0.000000;-0.587784;0.809018;,
  0.000000;-0.809018;0.587784;,
  0.000000;-0.951056;0.309017;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.951056;-0.309017;,
  0.000000;-0.809016;-0.587786;,
  0.000000;-0.587785;-0.809017;,
  0.000000;-0.309020;-0.951056;,
  0.000000;0.309020;-0.951056;,
  0.000000;0.587785;-0.809017;,
  0.000000;0.809016;-0.587786;,
  0.000000;0.951058;-0.309014;,
  0.000000;-0.162283;0.986744;,
  0.000000;0.340410;0.940277;,
  0.000000;0.756347;0.654171;,
  0.000000;-0.798045;0.602597;,
  0.000000;-0.468361;0.883538;,
  0.000000;-0.798044;-0.602600;,
  0.000000;-0.978002;-0.208597;,
  0.000000;0.340410;-0.940277;,
  0.000000;-0.162287;-0.986744;,
  0.000000;0.852266;0.523109;,
  -0.999990;-0.004422;-0.000001;,
  0.999990;-0.004422;-0.000001;,
  0.000000;0.951059;0.309010;,
  0.000000;0.809017;0.587785;,
  0.000000;0.587785;0.809017;,
  0.000000;0.309010;0.951059;,
  0.000000;-0.000002;1.000000;,
  0.000000;-0.309013;0.951058;,
  0.000000;-0.587785;0.809017;,
  0.000000;-0.809017;0.587785;,
  0.000000;-0.951054;0.309025;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.951054;-0.309025;,
  0.000000;-0.809011;-0.587794;,
  0.000000;-0.587776;-0.809024;,
  0.000000;-0.309028;-0.951053;,
  0.000000;-0.000003;-1.000000;,
  0.000000;0.309026;-0.951054;,
  0.000000;0.587776;-0.809024;,
  0.000000;0.809011;-0.587794;,
  0.000000;0.951059;-0.309010;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.162283;0.986744;,
  0.000000;0.340410;0.940277;,
  0.000000;0.756347;0.654171;,
  0.000000;-0.798046;0.602597;,
  0.000000;-0.468361;0.883537;,
  0.000000;0.852266;0.523109;,
  -0.999990;-0.004423;-0.000001;,
  0.999990;-0.004423;-0.000001;,
  0.000000;-1.000000;0.000001;,
  0.000000;-0.809020;-0.587781;,
  0.000000;-0.309010;-0.951059;,
  0.000000;0.951058;-0.309013;,
  0.000000;1.000000;0.000001;,
  0.000000;0.951058;0.309012;,
  0.000000;0.809018;0.587784;,
  0.000000;0.587784;0.809018;,
  0.000000;-0.309016;0.951057;,
  0.000000;-0.587784;0.809018;,
  0.000000;1.000000;0.000000;,
  0.000000;0.951057;0.309017;,
  0.000000;0.587786;0.809016;,
  0.000000;0.309020;0.951055;,
  0.000000;-0.000000;1.000000;,
  0.000000;-0.309020;0.951055;,
  0.000000;-0.587784;0.809018;,
  0.000000;-0.951056;0.309017;,
  0.000000;-0.951056;-0.309017;,
  0.000000;-0.000000;-1.000000;,
  0.000000;0.951058;-0.309014;,
  -0.998763;-0.047297;-0.015368;,
  -0.998763;-0.049731;0.000000;,
  -0.998763;-0.040234;-0.029232;,
  -0.998763;-0.029232;-0.040234;,
  -0.998763;-0.015368;-0.047297;,
  -0.998763;-0.000000;-0.049730;,
  -0.998763;0.015368;-0.047297;,
  -0.998763;0.029231;-0.040234;,
  -0.998763;0.040234;-0.029231;,
  -0.998763;0.047297;-0.015368;,
  -0.998763;0.049731;0.000000;,
  -0.998763;0.047297;0.015368;,
  -0.998763;0.040233;0.029231;,
  -0.998763;0.029231;0.040234;,
  -0.998763;0.015368;0.047297;,
  -0.998763;0.000000;0.049731;,
  -0.998763;-0.015368;0.047297;,
  -0.998763;-0.029231;0.040234;,
  -0.998763;-0.040233;0.029231;,
  -0.998763;-0.047297;0.015368;,
  0.000000;-0.978001;0.208598;,
  0.000000;-0.468354;-0.883541;,
  0.000000;0.756349;-0.654169;,
  0.000000;0.852267;-0.523107;,
  -0.943025;0.094888;0.318905;,
  -0.945621;0.265797;0.187491;,
  -0.940103;-0.099466;0.326056;,
  -0.943038;-0.272926;0.190240;,
  -0.936169;-0.351551;-0.000001;,
  -0.943037;-0.272927;-0.190241;,
  -0.940103;-0.099465;-0.326056;,
  -0.943024;0.094889;-0.318907;,
  -0.945621;0.265796;-0.187493;,
  -0.943057;0.332630;-0.000000;,
  0.945620;0.265798;0.187492;,
  0.943025;0.094888;0.318905;,
  0.940103;-0.099466;0.326057;,
  0.943038;-0.272926;0.190240;,
  0.936168;-0.351553;-0.000001;,
  0.943037;-0.272928;-0.190240;,
  0.940103;-0.099466;-0.326058;,
  0.943024;0.094889;-0.318907;,
  0.945620;0.265798;-0.187494;,
  0.943057;0.332630;-0.000000;,
  0.000000;-0.978001;0.208598;,
  0.000000;0.852267;-0.523107;,
  -0.943027;0.094886;0.318901;,
  -0.945621;0.265797;0.187491;,
  -0.940103;-0.099466;0.326055;,
  -0.943039;-0.272922;0.190237;,
  -0.936169;-0.351551;-0.000001;,
  -0.943039;-0.272924;-0.190238;,
  -0.940103;-0.099466;-0.326056;,
  -0.943026;0.094887;-0.318903;,
  -0.945621;0.265796;-0.187493;,
  -0.943059;0.332626;-0.000000;,
  0.945621;0.265797;0.187491;,
  0.943027;0.094886;0.318901;,
  0.940103;-0.099466;0.326055;,
  0.943039;-0.272922;0.190237;,
  0.936169;-0.351551;-0.000001;,
  0.943039;-0.272924;-0.190238;,
  0.940103;-0.099466;-0.326056;,
  0.943026;0.094887;-0.318903;,
  0.945621;0.265796;-0.187493;,
  0.943059;0.332626;-0.000000;;
  240;
  4;1,0,0,1;,
  4;2,1,1,2;,
  4;3,2,2,3;,
  4;4,3,3,4;,
  4;5,4,4,5;,
  4;6,5,5,6;,
  4;7,6,6,7;,
  4;8,7,7,8;,
  4;9,8,8,9;,
  4;10,9,9,10;,
  4;11,10,10,11;,
  4;12,11,11,12;,
  4;13,12,12,13;,
  4;14,13,13,14;,
  4;15,14,14,15;,
  4;16,15,15,16;,
  4;17,16,16,17;,
  4;18,17,17,18;,
  4;19,18,18,19;,
  4;0,19,19,0;,
  4;21,20,20,21;,
  4;22,21,21,22;,
  4;23,22,22,23;,
  4;24,23,23,24;,
  4;25,24,24,25;,
  4;26,25,25,26;,
  4;27,26,26,27;,
  4;28,27,27,28;,
  4;29,28,28,29;,
  4;30,29,29,30;,
  4;31,30,30,31;,
  4;32,31,31,32;,
  4;33,32,32,33;,
  4;34,33,33,34;,
  4;35,34,34,35;,
  4;36,35,35,36;,
  4;37,36,36,37;,
  4;38,37,37,38;,
  4;39,38,38,39;,
  4;20,39,39,20;,
  4;41,40,40,41;,
  4;42,41,41,42;,
  4;43,42,42,43;,
  4;44,43,43,44;,
  4;45,44,44,45;,
  4;46,45,45,46;,
  4;47,46,46,47;,
  4;48,47,47,48;,
  4;49,48,48,49;,
  4;50,49,49,50;,
  4;51,50,50,51;,
  4;52,51,51,52;,
  4;53,52,52,53;,
  4;54,53,53,54;,
  4;5,54,54,5;,
  4;55,5,5,55;,
  4;56,55,55,56;,
  4;57,56,56,57;,
  4;58,57,57,58;,
  4;40,58,58,40;,
  4;120,121,121,120;,
  4;122,120,120,122;,
  4;123,122,122,123;,
  4;124,123,123,124;,
  4;125,124,124,125;,
  4;126,125,125,126;,
  4;127,126,126,127;,
  4;128,127,127,128;,
  4;129,128,128,129;,
  4;130,129,129,130;,
  4;131,130,130,131;,
  4;132,131,131,132;,
  4;133,132,132,133;,
  4;134,133,133,134;,
  4;135,134,134,135;,
  4;136,135,135,136;,
  4;137,136,136,137;,
  4;138,137,137,138;,
  4;139,138,138,139;,
  4;121,139,139,121;,
  4;59,60,60,59;,
  4;60,61,61,60;,
  4;140,62,62,140;,
  4;62,63,63,62;,
  4;141,64,64,141;,
  4;64,65,65,64;,
  4;142,66,66,142;,
  4;66,67,67,66;,
  4;68,68,68,68;,
  4;143,143,143,143;,
  3;144,145,69;,
  3;146,144,69;,
  3;147,146,69;,
  3;148,147,69;,
  3;149,148,69;,
  3;150,149,69;,
  3;151,150,69;,
  3;152,151,69;,
  3;153,152,69;,
  3;145,153,69;,
  3;154,155,70;,
  3;155,156,70;,
  3;156,157,70;,
  3;157,158,70;,
  3;158,159,70;,
  3;159,160,70;,
  3;160,161,70;,
  3;161,162,70;,
  3;162,163,70;,
  3;163,154,70;,
  4;71,72,72,71;,
  4;72,73,73,72;,
  4;73,74,74,73;,
  4;74,75,75,74;,
  4;75,76,76,75;,
  4;76,77,77,76;,
  4;77,78,78,77;,
  4;78,79,79,78;,
  4;79,80,80,79;,
  4;80,81,81,80;,
  4;81,82,82,81;,
  4;82,83,83,82;,
  4;83,84,84,83;,
  4;84,85,85,84;,
  4;85,86,86,85;,
  4;86,87,87,86;,
  4;87,88,88,87;,
  4;88,89,89,88;,
  4;89,90,90,89;,
  4;90,71,71,90;,
  4;91,92,92,91;,
  4;92,93,93,92;,
  4;164,94,94,164;,
  4;94,95,95,94;,
  4;141,64,64,141;,
  4;64,65,65,64;,
  4;142,66,66,142;,
  4;66,67,67,66;,
  4;96,96,96,96;,
  4;165,165,165,165;,
  3;166,167,97;,
  3;168,166,97;,
  3;169,168,97;,
  3;170,169,97;,
  3;171,170,97;,
  3;172,171,97;,
  3;173,172,97;,
  3;174,173,97;,
  3;175,174,97;,
  3;167,175,97;,
  3;176,177,98;,
  3;177,178,98;,
  3;178,179,98;,
  3;179,180,98;,
  3;180,181,98;,
  3;181,182,98;,
  3;182,183,98;,
  3;183,184,98;,
  3;184,185,98;,
  3;185,176,98;,
  4;1,99,99,1;,
  4;100,1,1,100;,
  4;3,100,100,3;,
  4;101,3,3,101;,
  4;5,101,101,5;,
  4;6,5,5,6;,
  4;7,6,6,7;,
  4;8,7,7,8;,
  4;102,8,8,102;,
  4;103,102,102,103;,
  4;104,103,103,104;,
  4;105,104,104,105;,
  4;106,105,105,106;,
  4;14,106,106,14;,
  4;15,14,14,15;,
  4;107,15,15,107;,
  4;108,107,107,108;,
  4;18,108,108,18;,
  4;19,18,18,19;,
  4;99,19,19,99;,
  4;21,20,20,21;,
  4;22,21,21,22;,
  4;23,22,22,23;,
  4;24,23,23,24;,
  4;25,24,24,25;,
  4;26,25,25,26;,
  4;27,26,26,27;,
  4;28,27,27,28;,
  4;29,28,28,29;,
  4;30,29,29,30;,
  4;31,30,30,31;,
  4;32,31,31,32;,
  4;33,32,32,33;,
  4;34,33,33,34;,
  4;35,34,34,35;,
  4;36,35,35,36;,
  4;37,36,36,37;,
  4;38,37,37,38;,
  4;39,38,38,39;,
  4;20,39,39,20;,
  4;110,109,109,110;,
  4;42,110,110,42;,
  4;111,42,42,111;,
  4;112,111,111,112;,
  4;113,112,112,113;,
  4;114,113,113,114;,
  4;115,114,114,115;,
  4;48,115,115,48;,
  4;116,48,48,116;,
  4;80,116,116,80;,
  4;117,80,80,117;,
  4;52,117,117,52;,
  4;53,52,52,53;,
  4;54,53,53,54;,
  4;118,54,54,118;,
  4;55,118,118,55;,
  4;56,55,55,56;,
  4;57,56,56,57;,
  4;119,57,57,119;,
  4;109,119,119,109;,
  4;120,121,121,120;,
  4;122,120,120,122;,
  4;123,122,122,123;,
  4;124,123,123,124;,
  4;125,124,124,125;,
  4;126,125,125,126;,
  4;127,126,126,127;,
  4;128,127,127,128;,
  4;129,128,128,129;,
  4;130,129,129,130;,
  4;131,130,130,131;,
  4;132,131,131,132;,
  4;133,132,132,133;,
  4;134,133,133,134;,
  4;135,134,134,135;,
  4;136,135,135,136;,
  4;137,136,136,137;,
  4;138,137,137,138;,
  4;139,138,138,139;,
  4;121,139,139,121;;
 }
 MeshTextureCoords {
  340;
  0.429111;0.028686;,
  0.410076;0.028686;,
  0.410076;0.090289;,
  0.429111;0.090289;,
  0.448146;0.028686;,
  0.448146;0.090289;,
  0.467183;0.028686;,
  0.467183;0.090289;,
  0.486218;0.028686;,
  0.486218;0.090289;,
  0.505253;0.028686;,
  0.505253;0.090289;,
  0.524287;0.028686;,
  0.524287;0.090289;,
  0.543322;0.028686;,
  0.543322;0.090289;,
  0.562357;0.028686;,
  0.562357;0.090289;,
  0.581392;0.028686;,
  0.581392;0.090289;,
  0.600427;0.028686;,
  0.600427;0.090289;,
  0.619462;0.028686;,
  0.619462;0.090289;,
  0.638496;0.028686;,
  0.638496;0.090289;,
  0.657531;0.028686;,
  0.657531;0.090289;,
  0.676566;0.028686;,
  0.676566;0.090289;,
  0.695601;0.028686;,
  0.695601;0.090289;,
  0.714636;0.028686;,
  0.714636;0.090289;,
  0.733671;0.028686;,
  0.733671;0.090289;,
  0.752705;0.028686;,
  0.752705;0.090289;,
  0.771742;0.028686;,
  0.771742;0.090289;,
  0.790777;0.028686;,
  0.790777;0.090289;,
  0.410076;0.151893;,
  0.429111;0.151893;,
  0.448146;0.151893;,
  0.467183;0.151893;,
  0.486218;0.151893;,
  0.505253;0.151893;,
  0.524287;0.151893;,
  0.543322;0.151893;,
  0.562357;0.151893;,
  0.581392;0.151893;,
  0.600427;0.151893;,
  0.619462;0.151893;,
  0.638496;0.151893;,
  0.657531;0.151893;,
  0.676566;0.151893;,
  0.695601;0.151893;,
  0.714636;0.151893;,
  0.733671;0.151893;,
  0.752705;0.151893;,
  0.771742;0.151893;,
  0.790777;0.151893;,
  0.410076;0.213496;,
  0.429111;0.213496;,
  0.448146;0.213496;,
  0.467183;0.213496;,
  0.486218;0.213496;,
  0.505253;0.213496;,
  0.524287;0.213496;,
  0.543322;0.213496;,
  0.562357;0.213496;,
  0.581392;0.213496;,
  0.600427;0.213496;,
  0.619462;0.213496;,
  0.638496;0.213496;,
  0.657531;0.213496;,
  0.676566;0.213496;,
  0.695601;0.213496;,
  0.714636;0.213496;,
  0.733671;0.213496;,
  0.752705;0.213496;,
  0.771742;0.213496;,
  0.790777;0.213496;,
  0.410076;0.275101;,
  0.429111;0.275101;,
  0.448146;0.275101;,
  0.467183;0.275101;,
  0.486218;0.275101;,
  0.505253;0.275101;,
  0.524287;0.275101;,
  0.543322;0.275101;,
  0.562357;0.275101;,
  0.581392;0.275101;,
  0.600427;0.275101;,
  0.619462;0.275101;,
  0.638496;0.275101;,
  0.657531;0.275101;,
  0.676566;0.275101;,
  0.695601;0.275101;,
  0.714636;0.275101;,
  0.733671;0.275101;,
  0.752705;0.275101;,
  0.771742;0.275101;,
  0.790777;0.275101;,
  0.205776;0.186495;,
  0.213778;0.186495;,
  0.213778;0.119645;,
  0.205776;0.119645;,
  0.221780;0.186495;,
  0.221780;0.119645;,
  0.229782;0.186495;,
  0.229782;0.119645;,
  0.237784;0.186495;,
  0.237784;0.119645;,
  0.245786;0.186495;,
  0.245786;0.119645;,
  0.253788;0.186495;,
  0.253788;0.119645;,
  0.261790;0.186495;,
  0.261790;0.119645;,
  0.269792;0.186495;,
  0.269792;0.119645;,
  0.277794;0.186495;,
  0.277794;0.119645;,
  0.285796;0.186495;,
  0.285796;0.119645;,
  0.256955;0.185975;,
  0.274363;0.173327;,
  0.246197;0.154256;,
  0.235439;0.185975;,
  0.218031;0.173327;,
  0.211382;0.152864;,
  0.218031;0.132400;,
  0.235439;0.119753;,
  0.256955;0.119753;,
  0.274363;0.132400;,
  0.281012;0.152864;,
  0.274363;0.132506;,
  0.256955;0.119859;,
  0.246197;0.154362;,
  0.235439;0.119859;,
  0.218031;0.132506;,
  0.211382;0.152970;,
  0.218031;0.173434;,
  0.235439;0.186081;,
  0.256955;0.186081;,
  0.274363;0.173434;,
  0.281012;0.152970;,
  0.413851;0.284506;,
  0.432420;0.284506;,
  0.432420;0.022650;,
  0.413851;0.022650;,
  0.450989;0.284506;,
  0.450989;0.022650;,
  0.469558;0.284506;,
  0.469558;0.022650;,
  0.488127;0.284506;,
  0.488127;0.022650;,
  0.506697;0.284506;,
  0.506697;0.022650;,
  0.525266;0.284506;,
  0.525266;0.022650;,
  0.543835;0.284506;,
  0.543835;0.022650;,
  0.562404;0.284506;,
  0.562404;0.022650;,
  0.580974;0.284506;,
  0.580974;0.022650;,
  0.599543;0.284506;,
  0.599543;0.022650;,
  0.618112;0.284506;,
  0.618112;0.022650;,
  0.636681;0.284506;,
  0.636681;0.022650;,
  0.655250;0.284506;,
  0.655250;0.022650;,
  0.673820;0.284506;,
  0.673820;0.022650;,
  0.692389;0.284506;,
  0.692389;0.022650;,
  0.710958;0.284506;,
  0.710958;0.022650;,
  0.729527;0.284506;,
  0.729527;0.022650;,
  0.748096;0.284506;,
  0.748096;0.022650;,
  0.766666;0.284506;,
  0.766666;0.022650;,
  0.785235;0.284506;,
  0.785235;0.022650;,
  0.205776;0.186495;,
  0.213778;0.186495;,
  0.213778;0.119645;,
  0.205776;0.119645;,
  0.221780;0.186495;,
  0.221780;0.119645;,
  0.229782;0.186495;,
  0.229782;0.119645;,
  0.237784;0.186495;,
  0.237784;0.119645;,
  0.245786;0.186495;,
  0.245786;0.119645;,
  0.253788;0.186495;,
  0.253788;0.119645;,
  0.261790;0.186495;,
  0.261790;0.119645;,
  0.269792;0.186495;,
  0.269792;0.119645;,
  0.277794;0.186495;,
  0.277794;0.119645;,
  0.285796;0.186495;,
  0.285796;0.119645;,
  0.256955;0.185975;,
  0.274363;0.173327;,
  0.246197;0.154256;,
  0.235439;0.185975;,
  0.218031;0.173327;,
  0.211382;0.152864;,
  0.218031;0.132400;,
  0.235439;0.119753;,
  0.256955;0.119753;,
  0.274363;0.132400;,
  0.281012;0.152864;,
  0.274363;0.132506;,
  0.256955;0.119859;,
  0.246197;0.154362;,
  0.235439;0.119859;,
  0.218031;0.132506;,
  0.211382;0.152970;,
  0.218031;0.173434;,
  0.235439;0.186081;,
  0.256955;0.186081;,
  0.274363;0.173434;,
  0.281012;0.152970;,
  0.429111;0.028686;,
  0.410076;0.028686;,
  0.410076;0.090289;,
  0.429111;0.090289;,
  0.448146;0.028686;,
  0.448146;0.090289;,
  0.467183;0.028686;,
  0.467183;0.090289;,
  0.486218;0.028686;,
  0.486218;0.090289;,
  0.505253;0.028686;,
  0.505253;0.090289;,
  0.524287;0.028686;,
  0.524287;0.090289;,
  0.543322;0.028686;,
  0.543322;0.090289;,
  0.562357;0.028686;,
  0.562357;0.090289;,
  0.581392;0.028686;,
  0.581392;0.090289;,
  0.600427;0.028686;,
  0.600427;0.090289;,
  0.619462;0.028686;,
  0.619462;0.090289;,
  0.638496;0.028686;,
  0.638496;0.090289;,
  0.657531;0.028686;,
  0.657531;0.090289;,
  0.676566;0.028686;,
  0.676566;0.090289;,
  0.695601;0.028686;,
  0.695601;0.090289;,
  0.714636;0.028686;,
  0.714636;0.090289;,
  0.733671;0.028686;,
  0.733671;0.090289;,
  0.752705;0.028686;,
  0.752705;0.090289;,
  0.771742;0.028686;,
  0.771742;0.090289;,
  0.790777;0.028686;,
  0.790777;0.090289;,
  0.410076;0.151893;,
  0.429111;0.151893;,
  0.448146;0.151893;,
  0.467183;0.151893;,
  0.486218;0.151893;,
  0.505253;0.151893;,
  0.524287;0.151893;,
  0.543322;0.151893;,
  0.562357;0.151893;,
  0.581392;0.151893;,
  0.600427;0.151893;,
  0.619462;0.151893;,
  0.638496;0.151893;,
  0.657531;0.151893;,
  0.676566;0.151893;,
  0.695601;0.151893;,
  0.714636;0.151893;,
  0.733671;0.151893;,
  0.752705;0.151893;,
  0.771742;0.151893;,
  0.790777;0.151893;,
  0.410076;0.213496;,
  0.429111;0.213496;,
  0.448146;0.213496;,
  0.467183;0.213496;,
  0.486218;0.213496;,
  0.505253;0.213496;,
  0.524287;0.213496;,
  0.543322;0.213496;,
  0.562357;0.213496;,
  0.581392;0.213496;,
  0.600427;0.213496;,
  0.619462;0.213496;,
  0.638496;0.213496;,
  0.657531;0.213496;,
  0.676566;0.213496;,
  0.695601;0.213496;,
  0.714636;0.213496;,
  0.733671;0.213496;,
  0.752705;0.213496;,
  0.771742;0.213496;,
  0.790777;0.213496;,
  0.410076;0.275101;,
  0.429111;0.275101;,
  0.448146;0.275101;,
  0.467183;0.275101;,
  0.486218;0.275101;,
  0.505253;0.275101;,
  0.524287;0.275101;,
  0.543322;0.275101;,
  0.562357;0.275101;,
  0.581392;0.275101;,
  0.600427;0.275101;,
  0.619462;0.275101;,
  0.638496;0.275101;,
  0.657531;0.275101;,
  0.676566;0.275101;,
  0.695601;0.275101;,
  0.714636;0.275101;,
  0.733671;0.275101;,
  0.752705;0.275101;,
  0.771742;0.275101;,
  0.790777;0.275101;;
 }
}