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
 109;
 -9.38274;0.93347;-5.05048;,
 -9.31951;0.91122;4.81317;,
 9.02215;0.06558;4.51844;,
 8.95895;0.08785;-5.34520;,
 -9.17530;3.95132;-6.21777;,
 9.16643;3.10583;-6.36720;,
 -8.81922;8.18836;-6.18898;,
 9.52248;7.34286;-6.33841;,
 -8.52298;11.16495;-2.62915;,
 9.81874;10.31951;-2.77858;,
 -8.46007;11.13768;2.37642;,
 9.88167;10.29222;2.22698;,
 -8.66736;8.12230;5.89554;,
 9.67435;7.27680;5.74610;,
 -9.02342;3.88528;5.86675;,
 9.31827;3.03978;5.71732;,
 -9.31951;0.91122;4.81317;,
 9.02215;0.06558;4.51844;,
 -9.36375;0.34167;5.35274;,
 -9.38274;0.93347;-5.05048;,
 -9.43400;0.36645;-5.60689;,
 8.97793;-0.50397;5.05803;,
 8.90767;-0.47920;-5.90163;,
 8.95895;0.08785;-5.34520;,
 9.02215;0.06558;4.51844;,
 -9.17530;3.95132;-6.21777;,
 -9.20350;3.71957;-6.89072;,
 9.13819;2.87411;-7.04017;,
 9.16643;3.10583;-6.36720;,
 -8.81922;8.18836;-6.18898;,
 -8.80789;8.42740;-6.85875;,
 9.53383;7.58191;-7.00816;,
 9.52248;7.34286;-6.33841;,
 -8.52298;11.16495;-2.62915;,
 -8.47872;11.73482;-2.90339;,
 9.86299;10.88936;-3.05282;,
 9.81874;10.31951;-2.77858;,
 -8.46007;11.13768;2.37642;,
 -8.40882;11.70444;2.65839;,
 9.93289;10.85892;2.50896;,
 9.88167;10.29222;2.22698;,
 -8.66736;8.12230;5.89554;,
 -8.63914;8.35398;6.56849;,
 9.70257;7.50851;6.41906;,
 9.67435;7.27680;5.74610;,
 -9.02342;3.88528;5.86675;,
 -9.03477;3.64625;6.53649;,
 9.30692;2.80075;6.38707;,
 9.31827;3.03978;5.71732;,
 -5.06478;8.28056;-6.94029;,
 -5.46040;3.57280;-6.97228;,
 -9.20350;3.71957;-6.89072;,
 -8.80789;8.42740;-6.85875;,
 -8.47872;11.73482;-2.90339;,
 -4.73560;11.58804;-2.98493;,
 -5.69090;0.21959;-5.76761;,
 -9.43400;0.36645;-5.60689;,
 -8.40882;11.70444;2.65839;,
 -4.66571;11.55759;2.57684;,
 -5.62065;0.19481;5.19205;,
 -8.63914;8.35398;6.56849;,
 -4.89603;8.20721;6.48694;,
 -5.62065;0.19481;5.19205;,
 -5.29164;3.49947;6.45494;,
 -9.03477;3.64625;6.53649;,
 -9.36375;0.34167;5.35274;,
 8.03729;10.96320;-3.01744;,
 8.10716;10.93271;2.54433;,
 9.93289;10.85892;2.50896;,
 9.86299;10.88936;-3.05282;,
 9.53383;7.58191;-7.00816;,
 7.70810;7.65571;-6.97281;,
 7.87685;7.58236;6.45442;,
 9.70257;7.50851;6.41906;,
 9.13819;2.87411;-7.04017;,
 7.31247;2.94791;-7.00480;,
 7.48123;2.87457;6.42243;,
 9.30692;2.80075;6.38707;,
 8.90767;-0.47920;-5.90163;,
 7.08197;-0.40535;-5.83192;,
 7.15222;-0.43009;5.12775;,
 8.97793;-0.50397;5.05803;,
 8.97793;-0.50397;5.05803;,
 7.15222;-0.43009;5.12775;,
 -2.40048;11.47392;-2.99108;,
 -2.33058;11.44350;2.57069;,
 -2.72966;8.16645;-6.94645;,
 -2.56092;8.09305;6.48079;,
 -3.12528;3.45870;-6.97844;,
 -2.95653;3.38532;6.44879;,
 -3.35577;0.10541;-5.77976;,
 -3.28553;0.08066;5.17988;,
 -3.28553;0.08066;5.17988;,
 6.40092;11.04308;-3.01304;,
 6.47082;11.01265;2.54872;,
 6.07174;7.73565;-6.96841;,
 6.24050;7.66228;6.45882;,
 5.67611;3.02787;-7.00041;,
 5.84487;2.95450;6.42682;,
 5.44563;-0.32538;-5.82322;,
 5.51587;-0.35019;5.13643;,
 5.51587;-0.35019;5.13643;,
 9.42930;5.10038;-0.25011;,
 9.42930;5.10038;-0.25011;,
 9.42930;5.10038;-0.25011;,
 9.42930;5.10038;-0.25011;,
 9.42930;5.10038;-0.25011;,
 9.42930;5.10038;-0.25011;,
 9.42930;5.10038;-0.25011;;
 
 72;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;18,1,19,20;,
 4;21,22,23,24;,
 4;20,19,25,26;,
 4;22,27,28,23;,
 4;26,25,29,30;,
 4;27,31,32,28;,
 4;30,29,33,34;,
 4;31,35,36,32;,
 4;34,33,37,38;,
 4;35,39,40,36;,
 4;38,37,41,42;,
 4;39,43,44,40;,
 4;42,41,45,46;,
 4;43,47,48,44;,
 4;46,45,1,18;,
 4;47,21,24,48;,
 4;49,50,51,52;,
 4;49,52,53,54;,
 4;50,55,56,51;,
 4;54,53,57,58;,
 4;55,59,18,56;,
 4;58,57,60,61;,
 4;62,63,64,65;,
 4;61,60,64,63;,
 4;66,67,68,69;,
 4;66,69,70,71;,
 4;67,72,73,68;,
 4;71,70,74,75;,
 4;72,76,77,73;,
 4;75,74,78,79;,
 4;76,80,81,77;,
 4;79,78,82,83;,
 4;84,54,58,85;,
 4;84,86,49,54;,
 4;85,58,61,87;,
 4;86,88,50,49;,
 4;87,61,63,89;,
 4;88,90,55,50;,
 4;89,63,62,91;,
 4;90,92,59,55;,
 4;93,84,85,94;,
 4;93,94,67,66;,
 4;93,66,71,95;,
 4;93,95,86,84;,
 4;94,85,87,96;,
 4;94,96,72,67;,
 4;95,71,75,97;,
 4;95,97,88,86;,
 4;96,87,89,98;,
 4;96,98,76,72;,
 4;97,75,79,99;,
 4;97,99,90,88;,
 4;98,89,91,100;,
 4;98,100,80,76;,
 4;99,79,83,101;,
 4;99,101,92,90;,
 3;102,2,3;,
 3;103,3,5;,
 3;104,28,32;,
 3;104,32,36;,
 3;105,9,11;,
 3;106,11,13;,
 3;107,13,15;,
 3;108,15,17;;
 
 MeshMaterialList {
  13;
  72;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  3,
  3,
  4,
  4,
  3,
  3,
  4,
  4,
  3,
  3,
  4,
  4,
  3,
  3,
  4,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\�����ҁ[�g��.png";
   }
  }
  Material {
   0.599200;0.599200;0.599200;0.590000;;
   1.000000;
   0.240000;0.240000;0.240000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\�~���N�e�B�[���g.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.130000;0.130000;0.130000;;
  }
  Material {
   0.800000;0.586400;0.220000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.130000;0.095290;0.035750;;
  }
  Material {
   0.800000;0.718400;0.476800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.088000;0.207200;0.047200;1.000000;;
   47.000000;
   0.320000;0.320000;0.320000;;
   0.024200;0.056980;0.012980;;
  }
  Material {
   0.367200;0.367200;0.367200;1.000000;;
   10.000000;
   0.590000;0.590000;0.590000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.718400;0.542400;0.300800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.708800;0.034400;0.044000;1.000000;;
   32.000000;
   0.480000;0.480000;0.480000;;
   0.177200;0.008600;0.011000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\�g����.png";
   }
  }
  Material {
   0.248000;0.248000;0.248000;1.000000;;
   10.000000;
   0.370000;0.370000;0.370000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.248000;0.156800;0.140800;1.000000;;
   44.000000;
   0.680000;0.680000;0.680000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  88;
  -0.996016;0.088423;0.011565;,
  -0.996799;0.079154;0.011257;,
  -0.996757;0.079160;0.014456;,
  -0.996412;0.083640;0.012982;,
  -0.996412;0.083637;0.012984;,
  -0.996411;0.083644;0.012981;,
  -0.996411;0.083647;0.012980;,
  -0.995973;0.088430;0.014766;,
  0.996045;-0.088120;-0.011373;,
  0.996820;-0.078864;-0.011465;,
  0.996777;-0.078871;-0.014672;,
  0.996411;-0.083644;-0.012986;,
  0.996413;-0.083626;-0.012982;,
  0.996413;-0.083622;-0.012973;,
  0.996412;-0.083642;-0.012970;,
  0.996002;-0.088129;-0.014573;,
  0.046087;0.998936;0.001959;,
  0.017672;0.168436;0.985554;,
  -0.012256;-0.425782;0.904743;,
  -0.040547;-0.939678;0.339650;,
  -0.045982;-0.935744;-0.349670;,
  -0.026472;-0.413414;-0.910158;,
  -0.001453;0.183362;-0.983044;,
  0.007719;0.426101;-0.904643;,
  -0.022276;-0.169016;-0.985362;,
  0.037249;0.939849;-0.339552;,
  -0.031977;-0.340491;-0.939704;,
  0.045486;0.935757;0.349701;,
  -0.044067;-0.999027;-0.001976;,
  0.029316;0.413210;0.910164;,
  0.006771;-0.183007;0.983088;,
  0.038200;0.939802;-0.339577;,
  0.045624;0.935752;0.349695;,
  0.009036;0.426008;-0.904674;,
  0.028487;0.413274;0.910161;,
  -0.020675;-0.161628;-0.986635;,
  0.004868;-0.190303;0.981713;,
  -0.030068;-0.326275;-0.944797;,
  -0.001113;-0.366508;0.930414;,
  0.045075;0.939431;-0.339760;,
  0.046662;0.935716;0.349654;,
  0.018480;0.425312;-0.904858;,
  0.022569;0.413704;0.910132;,
  -0.011259;-0.166202;-0.986028;,
  -0.005961;-0.185566;0.982614;,
  -0.019952;-0.334355;-0.942236;,
  -0.013954;-0.358162;0.933555;,
  0.045050;0.939434;-0.339757;,
  0.046658;0.935717;0.349653;,
  0.018449;0.425315;-0.904857;,
  0.022587;0.413704;0.910131;,
  -0.011206;-0.164464;-0.986320;,
  -0.006012;-0.187283;0.982288;,
  -0.019830;-0.331034;-0.943410;,
  -0.014077;-0.361421;0.932297;,
  0.996443;-0.083268;-0.012951;,
  0.026988;0.339061;0.940377;,
  0.004943;0.353541;-0.935406;,
  0.996736;-0.079830;-0.011992;,
  0.995960;-0.089149;-0.010778;,
  0.996746;-0.079040;-0.015839;,
  0.996441;-0.083107;-0.014074;,
  0.996513;-0.082356;-0.013427;,
  0.996525;-0.082354;-0.012514;,
  0.996469;-0.083119;-0.011858;,
  0.995998;-0.088373;-0.013321;,
  -0.033178;-0.170921;-0.984726;,
  -0.002960;0.426837;-0.904324;,
  0.029485;0.940207;-0.339325;,
  -0.043832;-0.344866;-0.937628;,
  0.044324;0.935795;0.349746;,
  -0.039291;-0.999226;-0.002007;,
  0.036016;0.412702;0.910154;,
  0.001463;-0.352558;0.935789;,
  0.019356;-0.181297;0.983238;,
  0.016687;-0.348540;0.937145;,
  0.044609;0.935786;0.349736;,
  0.031436;0.940122;-0.339384;,
  -0.000266;0.426656;-0.904414;,
  0.034320;0.412832;0.910160;,
  -0.030071;-0.159626;-0.986719;,
  0.015671;-0.192462;0.981179;,
  -0.040259;-0.323094;-0.945510;,
  0.011782;-0.369943;0.928980;,
  -0.044652;-0.999001;-0.001971;,
  -0.040501;-0.999177;-0.001997;,
  -0.048871;-0.998803;-0.001944;,
  -0.048851;-0.998804;-0.001945;;
  72;
  4;16,16,16,16;,
  4;17,56,56,17;,
  4;18,17,17,18;,
  4;19,18,18,19;,
  4;20,19,19,20;,
  4;21,20,20,21;,
  4;22,21,21,22;,
  4;57,22,22,57;,
  4;0,0,1,1;,
  4;8,9,58,59;,
  4;1,1,2,2;,
  4;9,10,60,58;,
  4;2,2,3,3;,
  4;10,11,61,60;,
  4;3,3,4,4;,
  4;11,12,62,61;,
  4;4,4,5,5;,
  4;12,13,63,62;,
  4;5,5,6,6;,
  4;13,14,64,63;,
  4;6,6,7,7;,
  4;14,15,65,64;,
  4;7,7,0,0;,
  4;15,8,59,65;,
  4;23,24,66,67;,
  4;23,67,68,25;,
  4;24,26,69,66;,
  4;25,68,70,27;,
  4;28,28,71,71;,
  4;27,70,72,29;,
  4;73,30,74,75;,
  4;29,72,74,30;,
  4;31,32,76,77;,
  4;31,77,78,33;,
  4;32,34,79,76;,
  4;33,78,80,35;,
  4;34,36,81,79;,
  4;35,80,82,37;,
  4;36,38,83,81;,
  4;84,85,85,84;,
  4;39,25,27,40;,
  4;39,41,23,25;,
  4;40,27,29,42;,
  4;41,43,24,23;,
  4;42,29,30,44;,
  4;43,45,26,24;,
  4;44,30,73,46;,
  4;86,86,28,28;,
  4;47,39,40,48;,
  4;47,48,32,31;,
  4;47,31,33,49;,
  4;47,49,41,39;,
  4;48,40,42,50;,
  4;48,50,34,32;,
  4;49,33,35,51;,
  4;49,51,43,41;,
  4;50,42,44,52;,
  4;50,52,36,34;,
  4;51,35,37,53;,
  4;51,53,45,43;,
  4;52,44,46,54;,
  4;52,54,38,36;,
  4;87,84,84,87;,
  4;87,87,86,86;,
  3;55,59,58;,
  3;55,58,60;,
  3;55,60,61;,
  3;55,61,62;,
  3;55,62,63;,
  3;55,63,64;,
  3;55,64,65;,
  3;55,65,59;;
 }
 MeshTextureCoords {
  109;
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.375000;0.544830;,
  0.250000;0.544830;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.544830;,
  0.125000;0.544830;,
  0.125000;0.000000;,
  0.625000;0.000000;,
  0.625000;0.544830;,
  0.000000;0.544830;,
  0.750000;0.000000;,
  0.750000;0.544830;,
  1.000000;0.544830;,
  0.875000;0.544830;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.763630;,
  0.625000;0.763630;,
  0.625000;1.000000;,
  0.500000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.763630;,
  0.750000;0.763630;,
  0.750000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.763630;,
  0.875000;0.763630;,
  0.875000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.763630;,
  1.000000;0.763630;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.763630;,
  0.500000;0.586230;,
  0.625000;0.586230;,
  0.375000;0.586230;,
  0.750000;0.586230;,
  0.250000;0.586230;,
  0.875000;0.586230;,
  0.125000;0.586230;,
  1.000000;0.586230;,
  0.000000;0.586230;,
  0.500000;0.734070;,
  0.625000;0.734070;,
  0.375000;0.734070;,
  0.750000;0.734070;,
  0.250000;0.734070;,
  0.875000;0.734070;,
  0.125000;0.734070;,
  1.000000;0.734070;,
  0.000000;0.734070;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.000000;0.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}