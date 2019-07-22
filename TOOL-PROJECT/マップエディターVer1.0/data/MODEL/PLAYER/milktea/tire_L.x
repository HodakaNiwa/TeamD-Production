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
 141;
 2.88816;2.88855;-3.39191;,
 2.99093;1.66256;-1.95136;,
 2.98724;-0.42836;-2.70409;,
 2.88140;-0.44119;-4.77109;,
 2.99093;1.66256;-1.95136;,
 2.90449;4.26777;-0.06223;,
 3.00023;2.37347;-0.06266;,
 3.00980;1.57402;1.89169;,
 3.00023;2.37347;-0.06266;,
 2.92078;2.88855;3.26755;,
 3.00980;1.57402;1.89169;,
 2.92755;-0.44119;4.64675;,
 3.01268;-0.47018;2.48136;,
 2.92078;-3.77092;3.26755;,
 3.01023;-2.18474;1.97955;,
 3.01268;-0.47018;2.48136;,
 2.90449;-5.15014;-0.06223;,
 3.00023;-3.27202;-0.06266;,
 3.01023;-2.18474;1.97955;,
 3.00023;-3.27202;-0.06266;,
 2.90449;-5.15014;-0.06223;,
 2.88816;-3.77092;-3.39191;,
 2.99093;-2.26838;-1.95136;,
 2.98724;-0.42836;-2.70409;,
 2.99093;-2.26838;-1.95136;,
 2.59942;1.21131;-1.53337;,
 2.59656;-0.40195;-2.11416;,
 2.25361;-0.40928;-5.18471;,
 2.25628;3.21509;-3.68338;,
 2.27875;4.71636;-0.05917;,
 2.60653;1.75981;-0.07615;,
 2.59942;1.21131;-1.53337;,
 2.61392;1.14299;1.43165;,
 2.60653;1.75981;-0.07615;,
 2.29180;3.21509;3.56526;,
 2.29915;-0.40928;5.06644;,
 2.61616;-0.43422;1.88668;,
 2.61392;1.14299;1.43165;,
 2.61426;-1.75709;1.49955;,
 2.61616;-0.43422;1.88668;,
 2.29649;-4.03365;3.56524;,
 2.60653;-2.59598;-0.07615;,
 2.61426;-1.75709;1.49955;,
 2.27875;-5.53492;-0.05917;,
 2.27875;-5.53492;-0.05917;,
 2.25628;-4.03365;-3.68338;,
 2.59942;-1.82161;-1.53337;,
 2.60653;-2.59598;-0.07615;,
 2.59656;-0.40195;-2.11416;,
 2.59942;-1.82161;-1.53337;,
 3.01389;0.87791;-0.11696;,
 3.07379;-0.45630;-0.07140;,
 3.01000;0.47912;-0.91459;,
 3.01821;0.44722;0.76036;,
 3.07379;-0.45630;-0.07140;,
 3.01389;0.87791;-0.11696;,
 3.01944;-0.46203;1.01564;,
 3.07379;-0.45630;-0.07140;,
 3.01821;0.44722;0.76036;,
 3.01821;-1.21177;0.76036;,
 3.07379;-0.45630;-0.07140;,
 3.01944;-0.46203;1.01564;,
 3.01396;-1.69032;-0.10097;,
 3.07379;-0.45630;-0.07140;,
 3.01821;-1.21177;0.76036;,
 3.00990;-1.17987;-0.93048;,
 3.07379;-0.45630;-0.07140;,
 3.01396;-1.69032;-0.10097;,
 3.00838;-0.46203;-1.24949;,
 3.01000;0.47912;-0.91459;,
 3.07379;-0.45630;-0.07140;,
 3.00838;-0.46203;-1.24949;,
 3.07379;-0.45630;-0.07140;,
 3.00990;-1.17987;-0.93048;,
 1.63188;2.88855;-3.38577;,
 1.62515;-0.44119;-4.76494;,
 1.53959;-0.42836;-2.69696;,
 1.54325;1.66256;-1.94425;,
 1.54325;1.66256;-1.94425;,
 1.55255;2.37347;-0.05560;,
 1.64826;4.26777;-0.05603;,
 1.56213;1.57402;1.89863;,
 1.66457;2.88855;3.27375;,
 1.55255;2.37347;-0.05560;,
 1.56213;1.57402;1.89863;,
 1.56502;-0.47018;2.48839;,
 1.67134;-0.44119;4.65278;,
 1.66457;-3.77092;3.27375;,
 1.56502;-0.47018;2.48839;,
 1.56257;-2.18474;1.98669;,
 1.64826;-5.15014;-0.05603;,
 1.56257;-2.18474;1.98669;,
 1.55255;-3.27202;-0.05560;,
 1.55255;-3.27202;-0.05560;,
 1.54325;-2.26838;-1.94425;,
 1.63188;-3.77092;-3.38577;,
 1.64826;-5.15014;-0.05603;,
 1.54325;-2.26838;-1.94425;,
 1.53959;-0.42836;-2.69696;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 1.54710;-0.41509;-0.10591;,
 -2.94770;0.27641;0.24328;,
 -2.95336;0.06928;0.72358;,
 1.55458;0.06912;0.62096;,
 1.56032;0.27625;0.14061;,
 -2.95589;-0.41679;0.91680;,
 1.55213;-0.41695;0.81406;,
 -2.95359;-0.89706;0.70955;,
 1.55448;-0.89722;0.60686;,
 -2.94781;-1.09019;0.22345;,
 1.56014;-1.09035;0.12075;,
 -2.94208;-0.88306;-0.25677;,
 1.56598;-0.88322;-0.35946;,
 -2.93967;-0.39699;-0.44998;,
 1.56835;-0.39715;-0.55261;,
 -2.94198;0.08328;-0.24282;,
 1.56602;0.08312;-0.34549;,
 -2.94770;0.27641;0.24328;,
 1.56032;0.27625;0.14061;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 -2.94773;-0.40689;0.23345;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;,
 1.56024;-0.40705;0.13074;;
 
 88;
 4;0,1,2,3;,
 4;4,0,5,6;,
 4;7,8,5,9;,
 4;10,9,11,12;,
 4;13,14,15,11;,
 4;16,17,18,13;,
 4;19,20,21,22;,
 4;3,23,24,21;,
 4;1,25,26,2;,
 4;3,27,28,0;,
 4;0,28,29,5;,
 4;6,30,31,4;,
 4;7,32,33,8;,
 4;5,29,34,9;,
 4;9,34,35,11;,
 4;12,36,37,10;,
 4;14,38,39,15;,
 4;11,35,40,13;,
 4;17,41,42,18;,
 4;13,40,43,16;,
 4;20,44,45,21;,
 4;22,46,47,19;,
 4;23,48,49,24;,
 4;21,45,27,3;,
 3;50,51,52;,
 3;53,54,55;,
 3;56,57,58;,
 3;59,60,61;,
 3;62,63,64;,
 3;65,66,67;,
 3;68,69,70;,
 3;71,72,73;,
 4;25,69,68,26;,
 4;50,52,31,30;,
 4;33,32,53,55;,
 4;37,36,56,58;,
 4;59,61,39,38;,
 4;42,41,62,64;,
 4;47,46,65,67;,
 4;49,48,71,73;,
 4;74,75,76,77;,
 4;78,79,80,74;,
 4;81,82,80,83;,
 4;84,85,86,82;,
 4;87,86,88,89;,
 4;90,87,91,92;,
 4;93,94,95,96;,
 4;75,95,97,98;,
 4;75,74,28,27;,
 4;74,80,29,28;,
 4;80,82,34,29;,
 4;82,86,35,34;,
 4;86,87,40,35;,
 4;87,90,43,40;,
 4;96,95,45,44;,
 4;95,75,27,45;,
 3;99,79,78;,
 3;100,77,76;,
 3;101,98,97;,
 3;102,94,93;,
 3;103,92,91;,
 3;104,89,88;,
 3;105,81,83;,
 3;106,85,84;,
 4;107,108,109,110;,
 4;108,111,112,109;,
 4;111,113,114,112;,
 4;113,115,116,114;,
 4;115,117,118,116;,
 4;117,119,120,118;,
 4;119,121,122,120;,
 4;121,123,124,122;,
 3;125,108,107;,
 3;126,111,108;,
 3;127,113,111;,
 3;128,115,113;,
 3;129,117,115;,
 3;130,119,117;,
 3;131,121,119;,
 3;132,123,121;,
 3;133,110,109;,
 3;134,109,112;,
 3;135,112,114;,
 3;136,114,116;,
 3;137,116,118;,
 3;138,118,120;,
 3;139,120,122;,
 3;140,122,124;;
 
 MeshMaterialList {
  13;
  88;
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11,
  11;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game112\\Desktop\\授業系\\チーム（たっぴー）\\企画書用イラスト\\たっぴー紅茶.png";
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
    "C:\\Users\\game129\\Desktop\\学校\\チーム制作\\Player\\ミルクティー中身.png";
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
    "C:\\Users\\game129\\Desktop\\学校\\チーム制作\\Player\\紅茶顔.png";
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
  92;
  0.868828;-0.495094;-0.004430;,
  0.872983;-0.346939;0.342833;,
  0.878868;-0.001447;0.477062;,
  0.883350;0.332577;0.330282;,
  0.883323;0.468746;-0.004199;,
  0.879998;0.333148;-0.338550;,
  0.874350;-0.001787;-0.485292;,
  0.869160;-0.348007;-0.351356;,
  0.967362;-0.253265;-0.008266;,
  0.968235;-0.176159;0.177449;,
  0.965688;0.005382;0.259648;,
  0.967151;0.170701;0.188364;,
  0.972436;0.233142;-0.003730;,
  0.968550;0.152419;-0.196671;,
  0.963493;-0.000408;-0.267734;,
  0.968021;-0.163652;-0.190138;,
  0.523077;-0.852281;-0.002881;,
  0.535126;-0.599908;0.594769;,
  0.581182;0.813768;-0.002854;,
  0.548648;-0.003001;-0.836048;,
  0.996688;-0.080959;-0.007683;,
  0.997432;-0.056212;0.044389;,
  0.997576;-0.002936;0.069527;,
  0.998257;0.042506;0.040933;,
  0.998358;0.056971;-0.005909;,
  0.997972;0.034234;-0.053667;,
  0.996892;-0.003556;-0.078695;,
  0.997195;-0.051182;-0.054610;,
  0.999990;-0.001854;-0.004046;,
  0.972990;0.153496;-0.172423;,
  0.975198;0.221306;-0.003656;,
  0.975261;0.145050;0.166814;,
  0.971761;-0.003573;0.235938;,
  0.972769;-0.150597;0.176185;,
  0.974554;-0.224035;-0.007225;,
  0.968520;-0.162239;-0.188804;,
  0.969699;-0.009035;-0.244137;,
  -0.868407;-0.495833;0.004423;,
  -0.869158;-0.347702;0.351665;,
  -0.874564;-0.001817;0.484906;,
  -0.880504;0.332268;0.338099;,
  -0.883286;0.468814;0.004466;,
  -0.883227;0.333115;-0.330067;,
  -0.878872;-0.001716;-0.477054;,
  -0.872617;-0.347938;-0.342752;,
  -0.999637;0.026450;0.005041;,
  -0.999750;0.018195;-0.013004;,
  -0.999803;-0.000104;-0.019828;,
  -0.999772;-0.017058;-0.012851;,
  -0.999696;-0.024170;0.004908;,
  -0.999606;-0.016390;0.022766;,
  -0.999570;-0.000005;0.029317;,
  -0.999594;0.017424;0.022527;,
  0.556026;-0.002650;0.831161;,
  0.575087;0.578139;0.578818;,
  0.570105;0.578033;-0.583831;,
  0.528383;-0.600518;-0.600158;,
  -0.999988;-0.000001;0.004913;,
  -0.999930;0.000296;-0.011848;,
  0.000364;0.999895;0.014480;,
  0.016360;0.696687;0.717189;,
  0.022772;-0.014545;0.999635;,
  0.015847;-0.717217;0.696670;,
  -0.000367;-0.999894;-0.014524;,
  -0.016359;-0.696673;-0.717203;,
  -0.022763;0.014448;-0.999636;,
  -0.015841;0.717165;-0.696724;,
  0.999930;-0.000274;0.011868;,
  -0.575868;0.577983;-0.578197;,
  -0.556635;-0.002980;-0.830752;,
  -0.581135;0.813803;0.002845;,
  -0.570929;0.577855;0.583201;,
  -0.549311;-0.003330;0.835612;,
  -0.527720;-0.601021;0.600238;,
  -0.521555;-0.853213;0.002851;,
  -0.534366;-0.600468;-0.594888;,
  -0.999930;0.000338;-0.011807;,
  -0.999931;0.000215;-0.011772;,
  -0.999929;0.000407;-0.011929;,
  -0.999928;0.000320;-0.011961;,
  -0.999929;0.000288;-0.011900;,
  -0.999930;0.000278;-0.011799;,
  -0.999930;0.000275;-0.011791;,
  -0.999930;0.000249;-0.011824;,
  0.999930;-0.000291;0.011840;,
  0.999930;-0.000304;0.011846;,
  0.999930;-0.000275;0.011860;,
  0.999930;-0.000274;0.011817;,
  0.999929;-0.000318;0.011900;,
  0.999929;-0.000241;0.011949;,
  0.999930;-0.000215;0.011872;,
  0.999930;-0.000279;0.011859;;
  88;
  4;5,9,10,6;,
  4;9,5,4,8;,
  4;15,8,4,3;,
  4;15,3,2,14;,
  4;1,13,14,2;,
  4;0,12,13,1;,
  4;12,0,7,11;,
  4;6,10,11,7;,
  4;9,21,22,10;,
  4;6,19,55,5;,
  4;5,55,18,4;,
  4;8,20,21,9;,
  4;15,27,20,8;,
  4;4,18,54,3;,
  4;3,54,53,2;,
  4;14,26,27,15;,
  4;13,25,26,14;,
  4;2,53,17,1;,
  4;12,24,25,13;,
  4;1,17,16,0;,
  4;0,16,56,7;,
  4;11,23,24,12;,
  4;10,22,23,11;,
  4;7,56,19,6;,
  3;30,28,29;,
  3;31,28,30;,
  3;32,28,31;,
  3;33,28,32;,
  3;34,28,33;,
  3;35,28,34;,
  3;36,29,28;,
  3;36,28,35;,
  4;21,29,36,22;,
  4;30,29,21,20;,
  4;20,27,31,30;,
  4;27,26,32,31;,
  4;33,32,26,25;,
  4;25,24,34,33;,
  4;24,23,35,34;,
  4;23,22,36,35;,
  4;42,43,47,46;,
  4;46,45,41,42;,
  4;52,40,41,45;,
  4;52,51,39,40;,
  4;38,39,51,50;,
  4;37,38,50,49;,
  4;49,48,44,37;,
  4;43,44,48,47;,
  4;43,42,68,69;,
  4;42,41,70,68;,
  4;41,40,71,70;,
  4;40,39,72,71;,
  4;39,38,73,72;,
  4;38,37,74,73;,
  4;37,44,75,74;,
  4;44,43,69,75;,
  3;57,45,46;,
  3;57,46,47;,
  3;57,47,48;,
  3;57,48,49;,
  3;57,49,50;,
  3;57,50,51;,
  3;57,52,45;,
  3;57,51,52;,
  4;59,60,60,59;,
  4;60,61,61,60;,
  4;61,62,62,61;,
  4;62,63,63,62;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,66,66,65;,
  4;66,59,59,66;,
  3;58,76,77;,
  3;58,78,76;,
  3;58,79,78;,
  3;58,80,79;,
  3;58,81,80;,
  3;58,82,81;,
  3;58,83,82;,
  3;58,77,83;,
  3;67,84,85;,
  3;67,85,86;,
  3;67,86,87;,
  3;67,87,88;,
  3;67,88,89;,
  3;67,89,90;,
  3;67,90,91;,
  3;67,91,84;;
 }
 MeshTextureCoords {
  141;
  0.625000;0.000000;,
  0.659210;0.000000;,
  0.709150;0.000000;,
  0.750000;0.000000;,
  0.590790;0.000000;,
  0.500000;0.000000;,
  0.536690;0.000000;,
  0.414640;0.000000;,
  0.463310;0.000000;,
  0.375000;0.000000;,
  0.335360;0.000000;,
  0.250000;0.000000;,
  0.292140;0.000000;,
  0.125000;0.000000;,
  0.163060;0.000000;,
  0.207860;0.000000;,
  0.000000;0.000000;,
  0.039520;0.000000;,
  0.086940;0.000000;,
  0.960480;0.000000;,
  1.000000;0.000000;,
  0.875000;0.000000;,
  0.916010;0.000000;,
  0.790850;0.000000;,
  0.833990;0.000000;,
  0.659210;0.000000;,
  0.709150;0.000000;,
  0.750000;0.000000;,
  0.625000;0.000000;,
  0.500000;0.000000;,
  0.536690;0.000000;,
  0.590790;0.000000;,
  0.414640;0.000000;,
  0.463310;0.000000;,
  0.375000;0.000000;,
  0.250000;0.000000;,
  0.292140;0.000000;,
  0.335360;0.000000;,
  0.163060;0.000000;,
  0.207860;0.000000;,
  0.125000;0.000000;,
  0.039520;0.000000;,
  0.086940;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.875000;0.000000;,
  0.916010;0.000000;,
  0.960480;0.000000;,
  0.790850;0.000000;,
  0.833990;0.000000;,
  0.547460;0.000000;,
  0.563740;0.000000;,
  0.577770;0.000000;,
  0.425260;0.000000;,
  0.438970;0.000000;,
  0.453620;0.000000;,
  0.301630;0.000000;,
  0.313750;0.000000;,
  0.325930;0.000000;,
  0.173030;0.000000;,
  0.185460;0.000000;,
  0.198030;0.000000;,
  0.049560;0.000000;,
  0.063230;0.000000;,
  0.076390;0.000000;,
  0.925870;0.000000;,
  0.938240;0.000000;,
  0.951060;0.000000;,
  0.694710;0.000000;,
  0.671230;0.000000;,
  0.684180;0.000000;,
  0.803330;0.000000;,
  0.812420;0.000000;,
  0.824420;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.709150;0.000000;,
  0.659210;0.000000;,
  0.590790;0.000000;,
  0.536690;0.000000;,
  0.500000;0.000000;,
  0.414640;0.000000;,
  0.375000;0.000000;,
  0.463310;0.000000;,
  0.335360;0.000000;,
  0.292140;0.000000;,
  0.250000;0.000000;,
  0.125000;0.000000;,
  0.207860;0.000000;,
  0.163060;0.000000;,
  0.000000;0.000000;,
  0.086940;0.000000;,
  0.039520;0.000000;,
  0.960480;0.000000;,
  0.916010;0.000000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  0.833990;0.000000;,
  0.790850;0.000000;,
  0.563740;0.000000;,
  0.684180;0.000000;,
  0.812420;0.000000;,
  0.938240;0.000000;,
  0.063230;0.000000;,
  0.185460;0.000000;,
  0.438970;0.000000;,
  0.313750;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
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
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
