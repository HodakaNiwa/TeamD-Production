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
 92;
 17.17390;-0.76617;-17.17389;,
 -17.17390;-0.76617;-17.17389;,
 -17.17390;21.60689;-17.17389;,
 17.17390;21.31945;-17.17389;,
 17.17390;21.31945;-17.17389;,
 -17.17390;21.60689;-17.17389;,
 -17.17390;21.60689;17.17389;,
 16.21471;21.82563;16.66817;,
 16.21471;21.82563;16.66817;,
 -17.17390;21.60689;17.17389;,
 -17.17390;-0.76617;17.17389;,
 17.17390;-0.76617;17.17389;,
 -17.17390;-0.76617;-17.17389;,
 -17.17390;-0.76617;17.17389;,
 -17.17390;21.60689;17.17389;,
 -17.17390;21.60689;-17.17389;,
 17.17390;-0.76617;17.17389;,
 17.17390;-0.76617;-17.17389;,
 17.17390;21.31945;-17.17389;,
 16.21471;21.82563;16.66817;,
 -5.65806;23.68131;1.90103;,
 -4.83706;23.10590;2.14022;,
 -4.83706;23.10590;0.00000;,
 -7.67392;24.56689;0.00000;,
 -3.88689;22.74123;2.10646;,
 -3.88689;22.74123;0.00000;,
 -2.81787;22.57035;1.90103;,
 -2.81787;22.57035;0.00000;,
 -1.36273;22.08823;1.57959;,
 -1.36273;22.08823;0.00000;,
 0.34995;21.63399;0.00002;,
 -0.35313;21.82045;0.00000;,
 -4.83706;23.10590;-2.14022;,
 -5.65806;23.68131;-1.90103;,
 -3.88689;22.74123;-2.10646;,
 -2.81787;22.57035;-1.90103;,
 -1.36273;22.08823;-1.57959;,
 0.34995;21.63399;-0.00002;,
 -5.69088;23.62777;-1.90103;,
 -4.85712;23.07322;-2.14022;,
 -4.90640;22.99284;0.00000;,
 -7.69116;24.53875;0.00000;,
 -3.90695;22.70852;-2.10646;,
 -3.95622;22.62812;0.00000;,
 -2.83793;22.53764;-1.90103;,
 -2.88720;22.45725;0.00000;,
 -1.38278;22.05553;-1.57959;,
 -1.43205;21.97517;0.00000;,
 0.32990;21.60125;-0.00002;,
 -0.42246;21.70739;0.00000;,
 -4.85712;23.07322;2.14022;,
 -5.69088;23.62777;1.90103;,
 -3.90695;22.70852;2.10646;,
 -2.83793;22.53764;1.90103;,
 -1.38278;22.05553;1.57959;,
 0.32990;21.60125;0.00002;,
 5.91724;23.68131;-1.90103;,
 5.09625;23.10590;-2.14022;,
 5.09625;23.10590;0.00000;,
 7.93310;24.56689;0.00000;,
 4.14608;22.74123;-2.10646;,
 4.14608;22.74123;0.00000;,
 3.07706;22.57035;-1.90103;,
 3.07706;22.57035;0.00000;,
 1.62191;22.08823;-1.57959;,
 1.62191;22.08823;0.00000;,
 -0.09076;21.63399;-0.00002;,
 0.61231;21.82045;0.00000;,
 5.09625;23.10590;2.14022;,
 5.91724;23.68131;1.90103;,
 4.14608;22.74123;2.10646;,
 3.07706;22.57035;1.90103;,
 1.62191;22.08823;1.57959;,
 -0.09076;21.63399;0.00002;,
 5.95007;23.62777;1.90103;,
 5.11630;23.07322;2.14022;,
 5.16557;22.99284;0.00000;,
 7.95035;24.53875;0.00000;,
 4.16613;22.70852;2.10646;,
 4.21541;22.62812;0.00000;,
 3.09711;22.53764;1.90103;,
 3.14639;22.45725;0.00000;,
 1.64196;22.05553;1.57959;,
 1.69124;21.97517;0.00000;,
 -0.07071;21.60125;0.00002;,
 0.68165;21.70739;0.00000;,
 5.11630;23.07322;-2.14022;,
 5.95007;23.62777;-1.90103;,
 4.16613;22.70852;-2.10646;,
 3.09711;22.53764;-1.90103;,
 1.64196;22.05553;-1.57959;,
 -0.07071;21.60125;-0.00002;;
 
 45;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;21,24,25,22;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;23,22,32,33;,
 4;22,25,34,32;,
 4;25,27,35,34;,
 4;27,29,36,35;,
 4;29,31,37,36;,
 4;38,39,40,41;,
 4;39,42,43,40;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;41,40,50,51;,
 4;40,43,52,50;,
 4;43,45,53,52;,
 4;45,47,54,53;,
 4;47,49,55,54;,
 4;56,57,58,59;,
 4;57,60,61,58;,
 4;60,62,63,61;,
 4;62,64,65,63;,
 4;64,66,67,65;,
 4;59,58,68,69;,
 4;58,61,70,68;,
 4;61,63,71,70;,
 4;63,65,72,71;,
 4;65,67,73,72;,
 4;74,75,76,77;,
 4;75,78,79,76;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,84,85,83;,
 4;77,76,86,87;,
 4;76,79,88,86;,
 4;79,81,89,88;,
 4;81,83,90,89;,
 4;83,85,91,90;;
 
 MeshMaterialList {
  1;
  45;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\OBJECT\\ChocoTira_UV.png";
   }
  }
 }
 MeshNormals {
  69;
  0.000000;0.000000;-1.000000;,
  0.000908;0.999972;-0.007359;,
  0.007573;0.011191;0.999909;,
  -0.523339;-0.849428;0.067738;,
  -0.442282;-0.895131;0.055924;,
  -0.257010;-0.965415;0.043807;,
  -0.232288;-0.971440;0.048443;,
  -0.253949;-0.964375;0.074093;,
  -0.197698;-0.975664;0.094844;,
  0.516832;0.855342;-0.035696;,
  0.439363;0.898131;-0.017921;,
  0.259480;0.965748;0.000000;,
  0.236946;0.971523;0.000000;,
  0.285563;0.958360;-0.000005;,
  0.256356;0.966582;-0.000009;,
  0.517162;0.855888;0.000000;,
  0.439434;0.898275;0.000000;,
  0.285563;0.958360;0.000000;,
  0.256356;0.966582;0.000000;,
  0.516832;0.855342;0.035696;,
  0.439363;0.898131;0.017921;,
  0.285563;0.958360;0.000005;,
  0.256356;0.966582;0.000009;,
  -0.523339;-0.849428;-0.067738;,
  -0.442282;-0.895131;-0.055924;,
  -0.257010;-0.965415;-0.043807;,
  -0.232288;-0.971440;-0.048443;,
  -0.253949;-0.964375;-0.074093;,
  -0.197698;-0.975664;-0.094844;,
  -0.524544;-0.851383;0.000000;,
  -0.442976;-0.896534;0.000000;,
  -0.257257;-0.966343;0.000000;,
  -0.232561;-0.972582;-0.000000;,
  -0.254649;-0.967034;0.000000;,
  -0.198594;-0.980082;0.000000;,
  0.523337;-0.849430;-0.067737;,
  0.442277;-0.895133;-0.055923;,
  0.257005;-0.965416;-0.043807;,
  0.232287;-0.971440;-0.048443;,
  0.253949;-0.964376;-0.074093;,
  0.197698;-0.975664;-0.094845;,
  -0.516833;0.855342;0.035696;,
  -0.439363;0.898131;0.017921;,
  -0.259480;0.965749;0.000000;,
  -0.236946;0.971523;0.000000;,
  -0.285562;0.958360;0.000005;,
  -0.256356;0.966582;0.000009;,
  -0.517163;0.855887;0.000000;,
  -0.439434;0.898275;0.000000;,
  -0.285562;0.958360;0.000000;,
  -0.256356;0.966582;0.000000;,
  -0.516833;0.855342;-0.035696;,
  -0.439363;0.898131;-0.017921;,
  -0.285562;0.958360;-0.000005;,
  -0.256356;0.966582;-0.000009;,
  0.523337;-0.849430;0.067737;,
  0.442277;-0.895133;0.055923;,
  0.257005;-0.965416;0.043807;,
  0.232287;-0.971440;0.048443;,
  0.253949;-0.964376;0.074093;,
  0.197698;-0.975664;0.094845;,
  0.524541;-0.851385;-0.000000;,
  0.442970;-0.896536;-0.000000;,
  0.257252;-0.966344;0.000000;,
  0.232560;-0.972582;-0.000000;,
  0.254649;-0.967034;-0.000000;,
  0.198594;-0.980082;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.999674;0.021217;0.014171;;
  45;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;67,67,67,67;,
  4;68,68,68,68;,
  4;9,10,16,15;,
  4;10,11,11,16;,
  4;11,12,12,11;,
  4;12,13,17,12;,
  4;13,14,18,17;,
  4;15,16,20,19;,
  4;16,11,11,20;,
  4;11,12,12,11;,
  4;12,17,21,12;,
  4;17,18,22,21;,
  4;23,24,30,29;,
  4;24,25,31,30;,
  4;25,26,32,31;,
  4;26,27,33,32;,
  4;27,28,34,33;,
  4;29,30,4,3;,
  4;30,31,5,4;,
  4;31,32,6,5;,
  4;32,33,7,6;,
  4;33,34,8,7;,
  4;41,42,48,47;,
  4;42,43,43,48;,
  4;43,44,44,43;,
  4;44,45,49,44;,
  4;45,46,50,49;,
  4;47,48,52,51;,
  4;48,43,43,52;,
  4;43,44,44,43;,
  4;44,49,53,44;,
  4;49,50,54,53;,
  4;55,56,62,61;,
  4;56,57,63,62;,
  4;57,58,64,63;,
  4;58,59,65,64;,
  4;59,60,66,65;,
  4;61,62,36,35;,
  4;62,63,37,36;,
  4;63,64,38,37;,
  4;64,65,39,38;,
  4;65,66,40,39;;
 }
 MeshTextureCoords {
  92;
  0.544670;0.993490;,
  0.893750;0.993490;,
  0.893750;0.644400;,
  0.544670;0.644400;,
  0.052990;0.592090;,
  0.699330;0.592090;,
  0.699330;0.057760;,
  0.052990;0.057760;,
  0.895150;0.649270;,
  0.546080;0.646450;,
  0.543260;0.995520;,
  0.892340;0.998340;,
  0.547220;0.993490;,
  0.896310;0.993490;,
  0.896310;0.644400;,
  0.547220;0.644400;,
  0.550670;0.993490;,
  0.899750;0.993490;,
  0.899750;0.644400;,
  0.550670;0.644400;,
  0.261230;0.689260;,
  0.236080;0.712980;,
  0.294200;0.777120;,
  0.319340;0.753390;,
  0.210940;0.736700;,
  0.269060;0.800840;,
  0.185800;0.760420;,
  0.243920;0.824560;,
  0.160660;0.784140;,
  0.218780;0.848280;,
  0.135520;0.807870;,
  0.193640;0.872000;,
  0.352320;0.841250;,
  0.377460;0.817530;,
  0.327180;0.864970;,
  0.302040;0.888700;,
  0.276900;0.912420;,
  0.251750;0.936140;,
  0.250530;0.949620;,
  0.277820;0.925330;,
  0.217100;0.857110;,
  0.189810;0.881400;,
  0.305110;0.901040;,
  0.244380;0.832820;,
  0.332390;0.876750;,
  0.271670;0.808530;,
  0.359680;0.852460;,
  0.298960;0.784240;,
  0.386970;0.828170;,
  0.326240;0.759950;,
  0.156370;0.788890;,
  0.129080;0.813180;,
  0.183660;0.764600;,
  0.210940;0.740310;,
  0.238230;0.716020;,
  0.265520;0.691730;,
  0.261230;0.689260;,
  0.236080;0.712980;,
  0.294200;0.777120;,
  0.319340;0.753390;,
  0.210940;0.736700;,
  0.269060;0.800840;,
  0.185800;0.760420;,
  0.243920;0.824560;,
  0.160660;0.784140;,
  0.218780;0.848280;,
  0.135520;0.807870;,
  0.193640;0.872000;,
  0.352320;0.841250;,
  0.377460;0.817530;,
  0.327180;0.864970;,
  0.302040;0.888700;,
  0.276900;0.912420;,
  0.251750;0.936140;,
  0.250530;0.949620;,
  0.277820;0.925330;,
  0.217100;0.857110;,
  0.189810;0.881400;,
  0.305110;0.901040;,
  0.244380;0.832820;,
  0.332390;0.876750;,
  0.271670;0.808530;,
  0.359680;0.852460;,
  0.298960;0.784240;,
  0.386970;0.828170;,
  0.326240;0.759950;,
  0.156370;0.788890;,
  0.129080;0.813180;,
  0.183660;0.764600;,
  0.210940;0.740310;,
  0.238230;0.716020;,
  0.265520;0.691730;;
 }
}
