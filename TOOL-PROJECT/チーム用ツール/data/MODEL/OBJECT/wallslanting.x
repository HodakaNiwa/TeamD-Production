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
 24;
 18.84122;25.92042;-27.16477;,
 18.84122;0.40677;-27.16477;,
 -25.35296;0.40677;17.02939;,
 -25.35296;25.92042;17.02939;,
 -18.84122;34.89446;23.54114;,
 25.35296;34.89446;-20.65303;,
 -18.84122;51.43358;23.54114;,
 25.35296;51.43358;-20.65303;,
 -18.84122;128.17659;23.54114;,
 -18.84122;151.91978;23.54114;,
 25.35296;151.91978;-20.65303;,
 25.35296;128.17659;-20.65303;,
 25.35296;111.70289;-20.65303;,
 25.35296;68.46499;-20.65303;,
 -18.84122;68.46499;23.54114;,
 -18.84122;111.70289;23.54114;,
 22.85951;68.46499;-23.14648;,
 -21.33467;68.46499;21.04769;,
 22.85951;51.43358;-23.14648;,
 -21.33467;51.43358;21.04769;,
 -21.33467;111.70289;21.04769;,
 22.85951;111.70289;-23.14648;,
 -21.33467;128.17659;21.04769;,
 22.85951;128.17659;-23.14648;;
 
 15;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;6,7,5,4;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,14,13;,
 4;18,16,13,7;,
 4;19,18,7,6;,
 4;17,19,6,14;,
 4;17,16,18,19;,
 4;20,21,12,15;,
 4;22,20,15,8;,
 4;23,22,8,11;,
 4;21,23,11,12;,
 4;21,20,22,23;;
 
 MeshMaterialList {
  2;
  15;
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
  0,
  0,
  0,
  0,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game129\\Desktop\\èAêEçÏïi\\images.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game129\\Desktop\\èAêEçÏïi\\the-greek-ornament-meander08.png";
   }
  }
 }
 MeshNormals {
  7;
  -0.707107;0.000000;-0.707107;,
  -0.707107;0.000000;-0.707107;,
  -0.651520;0.388644;-0.651520;,
  0.000000;-1.000000;-0.000000;,
  0.707107;0.000000;-0.707107;,
  0.000000;1.000000;0.000000;,
  -0.707107;0.000000;0.707107;;
  15;
  4;2,1,1,2;,
  4;2,2,2,2;,
  4;0,0,2,2;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;6,6,6,6;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;1,1,1,1;;
 }
 MeshTextureCoords {
  24;
  1.442061;-0.796021;,
  1.442061;0.479661;,
  -0.767648;0.479661;,
  -0.767648;-0.796021;,
  -0.442061;-1.244723;,
  1.767648;-1.244723;,
  -0.442061;-2.071679;,
  1.767648;-2.071679;,
  -0.442061;-5.908829;,
  -0.442061;-7.095989;,
  1.767648;-7.095989;,
  1.767648;-5.908829;,
  1.767648;-5.085145;,
  1.767648;-2.923249;,
  -0.442061;-2.923249;,
  -0.442061;-5.085145;,
  1.642975;-2.923249;,
  -0.566733;-2.923249;,
  1.642975;-2.071679;,
  -0.566733;-2.071679;,
  -0.566733;-5.085145;,
  1.642975;-5.085145;,
  -0.566733;-5.908829;,
  1.642975;-5.908829;;
 }
}
