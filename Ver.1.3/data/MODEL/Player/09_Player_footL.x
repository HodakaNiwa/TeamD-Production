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
 44;
 -1.91412;-4.16932;-6.24618;,
 -1.13758;-3.02088;-6.20329;,
 0.01596;-3.49691;-8.50767;,
 -0.43433;-4.17249;-8.50792;,
 1.16671;-3.02088;-6.20329;,
 2.02078;-4.15288;-6.24618;,
 0.57241;-4.17942;-8.50784;,
 0.01596;-3.49691;-8.50767;,
 0.01596;-3.49691;-8.50767;,
 1.97863;-4.15644;-2.38281;,
 -1.88826;-4.16644;-2.37111;,
 -1.91412;-4.16932;-6.24618;,
 2.02078;-4.15288;-6.24618;,
 -0.43433;-4.17249;-8.50792;,
 0.57241;-4.17942;-8.50784;,
 0.57241;-4.17942;-8.50784;,
 1.68093;0.07518;-1.72838;,
 1.55322;0.17266;1.21913;,
 1.89433;-4.10698;1.63471;,
 1.51818;-1.40166;-2.60904;,
 -1.43546;0.17267;1.20304;,
 -1.86241;-4.10697;1.63471;,
 -1.53911;0.01888;-1.76153;,
 -1.54309;-1.40222;-2.52852;,
 -1.88826;-4.16644;-2.37111;,
 1.97863;-4.15644;-2.38281;,
 1.51818;-1.40166;-2.60904;,
 -1.54309;-1.40222;-2.52852;,
 1.51818;-1.40166;-2.60904;,
 -1.54309;-1.40222;-2.52852;,
 -1.34034;-2.13070;-3.78101;,
 1.34244;-2.13042;-3.82127;,
 1.16671;-2.32635;-4.31576;,
 -1.13758;-2.32635;-4.31576;,
 -1.94764;-4.16804;-4.31576;,
 -1.91795;-4.16724;-3.70230;,
 -1.88826;-4.16644;-2.37111;,
 1.97863;-4.15644;-2.38281;,
 1.99556;-4.15579;-3.70815;,
 2.01249;-4.15513;-4.31576;,
 -1.13758;-2.81920;-5.65517;,
 1.16671;-2.81920;-5.65517;,
 -1.92385;-4.16895;-5.68560;,
 2.01837;-4.15353;-5.68560;;
 
 25;
 4;0,1,2,3;,
 4;4,5,6,7;,
 3;8,1,4;,
 4;9,10,11,12;,
 4;12,11,13,14;,
 3;15,3,2;,
 4;16,17,18,19;,
 4;17,20,21,18;,
 4;20,22,23,21;,
 4;18,21,24,25;,
 3;26,18,25;,
 3;27,24,21;,
 4;16,28,29,22;,
 4;30,29,28,31;,
 4;30,31,32,33;,
 4;30,33,34,35;,
 4;30,35,36,29;,
 4;31,28,37,38;,
 4;31,38,39,32;,
 4;40,33,32,41;,
 4;40,41,4,1;,
 4;40,1,0,42;,
 4;40,42,34,33;,
 4;41,32,39,43;,
 4;41,43,5,4;;
 
 MeshMaterialList {
  6;
  25;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  0,
  1,
  1,
  0,
  0,
  1;;
  Material {
   0.409220;0.409220;0.409220;1.000000;;
   16.000000;
   0.760000;0.760000;0.760000;;
   0.062160;0.062160;0.062160;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.580000;0.220000;0.040800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.166400;0.166400;0.166400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.752800;0.652800;0.530400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  34;
  0.002347;-0.999974;0.006758;,
  -0.991032;0.128576;-0.036373;,
  0.989387;0.139013;-0.042281;,
  -0.909776;0.391592;-0.137710;,
  -0.001320;0.936332;-0.351113;,
  0.899079;0.415656;-0.137429;,
  -0.800129;0.524645;-0.290760;,
  -0.445760;0.836949;-0.317512;,
  0.423272;0.851855;-0.308518;,
  0.769837;0.572471;-0.282185;,
  -0.728266;0.497804;-0.470979;,
  -0.409320;0.830216;-0.378416;,
  0.695197;0.554140;-0.457853;,
  0.999087;0.021124;-0.037127;,
  -0.002678;0.098292;0.995154;,
  -0.997634;0.067513;-0.012951;,
  0.995528;0.094008;-0.009272;,
  -0.006653;0.900550;-0.434701;,
  -0.953928;0.261358;-0.147353;,
  0.944864;0.289052;-0.153888;,
  -0.500571;0.828144;-0.252203;,
  0.302668;0.907339;-0.291769;,
  -0.865590;0.492524;-0.090414;,
  0.846702;0.525153;-0.085500;,
  0.001015;-0.999987;0.005025;,
  -0.001351;-0.999948;0.010140;,
  0.000087;0.000312;-1.000000;,
  -0.995384;0.095881;-0.004197;,
  0.001312;-0.999907;0.013605;,
  0.979312;0.197373;-0.044642;,
  -0.986115;0.158961;-0.048049;,
  -0.011674;0.493670;-0.869571;,
  -0.011533;0.700613;-0.713448;,
  0.874663;0.478958;-0.074598;;
  25;
  4;6,7,11,10;,
  4;8,9,12,12;,
  3;11,7,8;,
  4;0,0,24,24;,
  4;24,24,25,25;,
  3;26,26,26;,
  4;13,13,16,2;,
  4;14,14,14,14;,
  4;15,15,1,27;,
  4;28,28,0,0;,
  3;2,16,29;,
  3;1,30,27;,
  4;31,32,32,31;,
  4;17,32,32,17;,
  4;17,17,4,4;,
  4;18,3,3,18;,
  4;18,18,30,1;,
  4;19,2,29,19;,
  4;19,19,5,5;,
  4;20,4,4,21;,
  4;20,21,8,7;,
  4;20,7,6,22;,
  4;20,22,3,3;,
  4;33,5,5,23;,
  4;21,23,9,8;;
 }
 MeshTextureCoords {
  44;
  0.000000;0.666667;,
  0.333333;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.666667;0.666667;,
  1.000000;0.666667;,
  1.000000;1.000000;,
  0.666667;1.000000;,
  0.500000;1.000000;,
  0.000000;0.333333;,
  0.333333;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.500000;0.500000;,
  0.000000;0.333333;,
  0.333333;0.333333;,
  0.333333;0.666667;,
  0.000000;0.666667;,
  0.666667;0.333333;,
  0.666667;0.666667;,
  1.000000;0.333333;,
  1.000000;0.666667;,
  0.666667;1.000000;,
  0.333333;1.000000;,
  0.000000;0.833333;,
  1.000000;0.833333;,
  0.666667;0.000000;,
  0.333333;0.000000;,
  0.333333;0.166667;,
  0.666667;0.166667;,
  0.666667;0.333333;,
  0.333333;0.333333;,
  0.000000;0.333333;,
  0.000000;0.166667;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.166667;,
  1.000000;0.333333;,
  0.333333;0.569869;,
  0.666667;0.569869;,
  0.000000;0.569869;,
  1.000000;0.569869;;
 }
}