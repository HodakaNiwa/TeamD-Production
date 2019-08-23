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
 -27.66582;0.40796;-7.43186;,
 -24.00578;27.64180;-0.13462;,
 -24.00578;27.64194;-0.13507;,
 -27.66586;0.40809;-7.43241;,
 -44.76422;51.23837;6.18811;,
 -44.76424;51.23850;6.18756;,
 -14.83638;54.90068;7.16938;,
 -14.83640;54.90081;7.16892;,
 0.00000;82.65327;14.60571;,
 0.00000;82.65339;14.60516;,
 14.83636;54.90067;7.16938;,
 14.83638;54.90081;7.16892;,
 44.76422;51.23837;6.18811;,
 44.76426;51.23850;6.18756;,
 24.00576;27.64180;-0.13462;,
 24.00578;27.64194;-0.13507;,
 27.66582;0.40797;-7.43186;,
 27.66586;0.40810;-7.43240;,
 0.00000;10.79487;-4.64874;,
 0.00000;10.79502;-4.64919;,
 -27.66582;0.40796;-7.43186;,
 -27.66586;0.40809;-7.43241;,
 -24.00578;27.64180;-0.13462;,
 -27.66582;0.40796;-7.43186;,
 0.00000;34.68676;11.76285;,
 -44.76422;51.23837;6.18811;,
 -14.83638;54.90068;7.16938;,
 0.00000;82.65327;14.60571;,
 14.83636;54.90067;7.16938;,
 44.76422;51.23837;6.18811;,
 24.00576;27.64180;-0.13462;,
 27.66582;0.40797;-7.43186;,
 0.00000;10.79487;-4.64874;,
 -27.66586;0.40809;-7.43241;,
 -24.00578;27.64194;-0.13507;,
 0.00000;39.69177;-6.91611;,
 -44.76424;51.23850;6.18756;,
 -14.83640;54.90081;7.16892;,
 0.00000;82.65339;14.60516;,
 14.83638;54.90081;7.16892;,
 44.76426;51.23850;6.18756;,
 24.00578;27.64194;-0.13507;,
 27.66586;0.40810;-7.43240;,
 0.00000;10.79502;-4.64919;;
 
 30;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 3;22,23,24;,
 3;25,22,24;,
 3;26,25,24;,
 3;27,26,24;,
 3;28,27,24;,
 3;29,28,24;,
 3;30,29,24;,
 3;31,30,24;,
 3;32,31,24;,
 3;23,32,24;,
 3;33,34,35;,
 3;34,36,35;,
 3;36,37,35;,
 3;37,38,35;,
 3;38,39,35;,
 3;39,40,35;,
 3;40,41,35;,
 3;41,42,35;,
 3;42,43,35;,
 3;43,33,35;;
 
 MeshMaterialList {
  1;
  30;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data/TEXTURE/ITEM/item_STAR.png";
   }
  }
 }
 MeshNormals {
  37;
  -0.991125;0.115625;0.065595;,
  -0.958655;-0.279740;-0.052219;,
  -0.762017;-0.627200;-0.161091;,
  -0.572926;0.791442;0.213017;,
  -0.888501;0.441064;0.126604;,
  0.572866;0.788209;0.224836;,
  0.125678;0.955845;0.265643;,
  0.957482;-0.287681;-0.021634;,
  0.989956;0.108881;0.090178;,
  -0.000002;-0.960382;-0.278688;,
  0.000000;-0.237297;0.971437;,
  0.000000;0.280213;-0.959938;,
  -0.125681;0.959931;0.250474;,
  0.888442;0.439486;0.132374;,
  0.761453;-0.635102;-0.129749;,
  -0.362217;-0.895255;-0.259455;,
  0.362213;-0.895078;-0.260073;,
  -0.338487;-0.332920;0.880108;,
  -0.138631;-0.396323;0.907584;,
  -0.177569;-0.152801;0.972174;,
  -0.212985;0.065210;0.974877;,
  0.000000;-0.059164;0.998248;,
  0.212985;0.065210;0.974877;,
  0.177569;-0.152801;0.972174;,
  0.338488;-0.332919;0.880108;,
  0.138631;-0.396323;0.907584;,
  0.000000;-0.566199;0.824269;,
  -0.138631;0.110566;-0.984153;,
  -0.338488;0.151737;-0.928656;,
  -0.177569;0.353757;-0.918328;,
  -0.212984;0.543912;-0.811664;,
  0.000000;0.447887;-0.894090;,
  0.212985;0.543912;-0.811663;,
  0.177569;0.353757;-0.918328;,
  0.338488;0.151737;-0.928656;,
  0.138631;0.110566;-0.984153;,
  0.000000;-0.078208;-0.996937;;
  30;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;12,3,3,12;,
  4;3,4,4,3;,
  4;13,5,5,13;,
  4;5,6,6,5;,
  4;14,7,7,14;,
  4;7,8,8,7;,
  4;15,9,9,15;,
  4;9,16,16,9;,
  3;17,18,10;,
  3;19,17,10;,
  3;20,19,10;,
  3;21,20,10;,
  3;22,21,10;,
  3;23,22,10;,
  3;24,23,10;,
  3;25,24,10;,
  3;26,25,10;,
  3;18,26,10;,
  3;27,28,11;,
  3;28,29,11;,
  3;29,30,11;,
  3;30,31,11;,
  3;31,32,11;,
  3;32,33,11;,
  3;33,34,11;,
  3;34,35,11;,
  3;35,36,11;,
  3;36,27,11;;
 }
 MeshTextureCoords {
  44;
  0.050000;0.495390;,
  0.080820;0.495390;,
  0.080820;0.031870;,
  0.050000;0.031870;,
  0.111650;0.495390;,
  0.111650;0.031870;,
  0.142470;0.495390;,
  0.142470;0.031870;,
  0.173300;0.495390;,
  0.173300;0.031870;,
  0.204120;0.495390;,
  0.204120;0.031870;,
  0.234940;0.495390;,
  0.234940;0.031870;,
  0.265770;0.495390;,
  0.265770;0.031870;,
  0.296590;0.495390;,
  0.296590;0.031870;,
  0.327420;0.495390;,
  0.327420;0.031870;,
  0.358240;0.495390;,
  0.358240;0.031870;,
  0.767990;0.983770;,
  0.913720;0.877890;,
  0.677930;0.718230;,
  0.587860;0.983770;,
  0.442140;0.877890;,
  0.386470;0.706580;,
  0.442140;0.535260;,
  0.587860;0.429390;,
  0.767990;0.429390;,
  0.913720;0.535260;,
  0.969380;0.706580;,
  0.913720;0.531940;,
  0.767990;0.426070;,
  0.677930;0.714920;,
  0.587860;0.426070;,
  0.442140;0.531940;,
  0.386470;0.703260;,
  0.442140;0.874570;,
  0.587860;0.980450;,
  0.767990;0.980450;,
  0.913720;0.874570;,
  0.969380;0.703260;;
 }
}