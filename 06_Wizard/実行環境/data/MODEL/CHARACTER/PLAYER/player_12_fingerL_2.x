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
 90;
 -0.00956;0.28862;0.44811;,
 0.03657;0.30009;0.49315;,
 0.03638;0.33078;0.42699;,
 0.09998;0.34232;0.45228;,
 0.10031;0.29978;0.52155;,
 0.17521;0.34641;0.45699;,
 0.17559;0.29807;0.53268;,
 0.04350;-0.28063;0.49801;,
 -0.00289;-0.27102;0.45280;,
 0.04406;-0.31242;0.43237;,
 0.10791;-0.32204;0.45785;,
 0.10721;-0.27834;0.52639;,
 0.18321;-0.32423;0.46260;,
 0.18242;-0.27464;0.53747;,
 2.61645;-0.45733;0.51234;,
 2.64913;-0.49140;0.46696;,
 2.63441;-0.43043;0.44697;,
 2.58746;-0.38620;0.47264;,
 2.56279;-0.42296;0.54081;,
 2.52696;-0.34132;0.47753;,
 2.49897;-0.38302;0.55198;,
 0.03932;0.29172;-0.51220;,
 -0.00706;0.28099;-0.46722;,
 0.03876;0.32351;-0.44656;,
 0.10251;0.33464;-0.47170;,
 0.10322;0.29094;-0.54025;,
 0.17776;0.33863;-0.47607;,
 0.17855;0.28905;-0.55093;,
 2.33425;-0.95402;0.45756;,
 2.28971;-0.93738;0.50258;,
 2.27252;-0.96214;0.43616;,
 2.21366;-0.93537;0.46147;,
 2.23752;-0.90086;0.53109;,
 2.14964;-0.89570;0.46626;,
 2.17674;-0.85646;0.54237;,
 2.65455;-0.47649;-0.44827;,
 2.62240;-0.44094;-0.49290;,
 2.63958;-0.41619;-0.42649;,
 2.59293;-0.37113;-0.45126;,
 2.56909;-0.40566;-0.52088;,
 2.53249;-0.32612;-0.45541;,
 2.50539;-0.36536;-0.53151;,
 -0.00039;-0.27865;-0.46255;,
 0.04624;-0.28901;-0.50735;,
 0.04643;-0.31971;-0.44119;,
 0.11043;-0.32972;-0.46615;,
 0.11011;-0.28719;-0.53541;,
 0.18576;-0.33200;-0.47045;,
 0.18538;-0.28368;-0.54616;,
 2.29566;-0.92101;-0.50267;,
 2.33967;-0.93911;-0.45768;,
 2.27768;-0.94790;-0.43730;,
 2.21913;-0.92032;-0.46242;,
 2.24381;-0.88355;-0.53059;,
 2.15517;-0.88049;-0.46668;,
 2.18317;-0.83879;-0.54114;,
 -0.00289;-0.27102;0.45280;,
 -0.00956;0.28862;0.44811;,
 -0.00706;0.28099;-0.46722;,
 2.33967;-0.93911;-0.45768;,
 2.65455;-0.47649;-0.44827;,
 2.64913;-0.49140;0.46696;,
 1.24496;-0.32273;0.53751;,
 2.17674;-0.85646;0.54237;,
 2.49897;-0.38302;0.55198;,
 1.40113;0.22833;0.53982;,
 0.17559;0.29807;0.53268;,
 1.40113;0.22833;0.53982;,
 1.41466;0.27569;0.46472;,
 1.23177;-0.36957;0.46201;,
 1.24496;-0.32273;0.53751;,
 1.41686;0.27901;-0.46835;,
 0.17521;0.34641;0.45699;,
 1.41466;0.27569;0.46472;,
 2.52696;-0.34132;0.47753;,
 2.53249;-0.32612;-0.45541;,
 1.23177;-0.36957;0.46201;,
 1.23397;-0.36626;-0.47107;,
 2.14964;-0.89570;0.46626;,
 0.18321;-0.32423;0.46260;,
 0.18576;-0.33200;-0.47045;,
 1.40368;0.23217;-0.54383;,
 1.41686;0.27901;-0.46835;,
 1.23397;-0.36626;-0.47107;,
 1.24750;-0.31890;-0.54617;,
 1.24750;-0.31890;-0.54617;,
 0.18538;-0.28368;-0.54616;,
 0.17855;0.28905;-0.55093;,
 1.40368;0.23217;-0.54383;,
 2.50539;-0.36536;-0.53151;;
 
 124;
 3;0,1,2;,
 3;3,2,1;,
 3;1,4,3;,
 3;5,3,4;,
 3;4,6,5;,
 3;7,8,9;,
 3;9,10,7;,
 3;11,7,10;,
 3;10,12,11;,
 3;13,11,12;,
 3;14,15,16;,
 3;16,17,14;,
 3;18,14,17;,
 3;17,19,18;,
 3;20,18,19;,
 3;21,22,23;,
 3;23,24,21;,
 3;25,21,24;,
 3;24,26,25;,
 3;27,25,26;,
 3;28,29,30;,
 3;31,30,29;,
 3;29,32,31;,
 3;33,31,32;,
 3;32,34,33;,
 3;35,36,37;,
 3;38,37,36;,
 3;36,39,38;,
 3;40,38,39;,
 3;39,41,40;,
 3;42,43,44;,
 3;45,44,43;,
 3;43,46,45;,
 3;47,45,46;,
 3;46,48,47;,
 3;49,50,51;,
 3;51,52,49;,
 3;53,49,52;,
 3;52,54,53;,
 3;55,53,54;,
 3;56,57,58;,
 3;56,58,42;,
 3;59,60,61;,
 3;59,61,28;,
 3;7,1,0;,
 3;7,0,8;,
 3;11,4,1;,
 3;11,1,7;,
 3;13,6,4;,
 3;13,4,11;,
 3;28,15,14;,
 3;28,14,29;,
 3;29,14,18;,
 3;29,18,32;,
 3;32,18,20;,
 3;32,20,34;,
 3;42,22,21;,
 3;42,21,43;,
 3;43,21,25;,
 3;43,25,46;,
 3;46,25,27;,
 3;46,27,48;,
 3;49,36,35;,
 3;49,35,50;,
 3;53,39,36;,
 3;53,36,49;,
 3;55,41,39;,
 3;55,39,53;,
 3;2,23,22;,
 3;2,22,0;,
 3;3,24,23;,
 3;3,23,2;,
 3;5,26,24;,
 3;5,24,3;,
 3;37,16,15;,
 3;37,15,35;,
 3;38,17,16;,
 3;38,16,37;,
 3;40,19,17;,
 3;40,17,38;,
 3;44,9,8;,
 3;44,8,42;,
 3;45,10,9;,
 3;45,9,44;,
 3;47,12,10;,
 3;47,10,45;,
 3;28,30,51;,
 3;28,51,50;,
 3;31,52,51;,
 3;31,51,30;,
 3;33,54,52;,
 3;33,52,31;,
 3;62,63,64;,
 3;62,64,65;,
 3;66,13,62;,
 3;66,62,65;,
 3;67,68,5;,
 3;67,5,6;,
 3;67,20,19;,
 3;67,19,68;,
 3;69,33,34;,
 3;69,34,70;,
 3;13,12,69;,
 3;13,69,70;,
 3;71,26,72;,
 3;71,72,73;,
 3;73,74,75;,
 3;73,75,71;,
 3;76,77,54;,
 3;76,54,78;,
 3;76,79,80;,
 3;76,80,77;,
 3;81,27,26;,
 3;81,26,82;,
 3;40,41,81;,
 3;40,81,82;,
 3;83,84,55;,
 3;83,55,54;,
 3;47,48,84;,
 3;47,84,83;,
 3;85,86,87;,
 3;85,87,88;,
 3;89,55,85;,
 3;89,85,88;;
 
 MeshMaterialList {
  1;
  124;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.752800;0.693600;0.608800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  64;
  -0.881558;0.420887;-0.213799;,
  -0.872330;-0.441893;0.209214;,
  -0.037954;0.384774;0.922230;,
  -0.267162;-0.522148;0.809930;,
  0.967511;-0.138708;0.211382;,
  0.550646;-0.793097;-0.260358;,
  0.243305;0.309912;-0.919107;,
  -0.058779;-0.484888;-0.872599;,
  -0.009078;0.925484;-0.378677;,
  0.505859;0.771148;0.386572;,
  -0.479548;-0.826367;-0.295216;,
  -0.043142;-0.937796;0.344497;,
  -0.495876;0.470820;0.729681;,
  -0.202616;0.483917;0.851335;,
  0.134565;-0.675601;0.724883;,
  -0.113785;-0.519468;0.846880;,
  -0.480644;-0.482455;-0.732269;,
  -0.186495;-0.488598;-0.852345;,
  0.672780;0.126176;-0.729004;,
  0.435505;0.300534;-0.848537;,
  -0.842013;0.345577;0.414234;,
  -0.393158;0.903575;-0.170235;,
  -0.498752;0.853136;0.152988;,
  -0.154666;0.952940;-0.260739;,
  0.892320;-0.172873;-0.416988;,
  0.887014;0.436998;-0.149127;,
  0.827152;0.533959;0.175237;,
  0.655794;0.705852;0.267781;,
  -0.831348;-0.365533;-0.418625;,
  -0.477380;-0.864811;-0.155599;,
  -0.372310;-0.912740;0.168199;,
  -0.133268;-0.956370;0.259992;,
  0.436072;-0.833682;0.338844;,
  -0.138679;-0.978644;-0.151736;,
  -0.135303;-0.973344;0.185186;,
  -0.417889;-0.867759;-0.269003;,
  0.207514;0.220235;0.953118;,
  -0.006741;0.961192;0.275797;,
  -0.054459;0.306543;-0.950298;,
  0.525032;0.807816;-0.267909;,
  -0.077265;-0.473760;0.877258;,
  -0.492090;-0.829350;0.264625;,
  -0.185456;-0.435454;-0.880900;,
  -0.036995;-0.960668;-0.275223;,
  -0.176949;0.953590;0.243629;,
  -0.539456;-0.470317;0.698420;,
  -0.241217;-0.475564;0.845963;,
  0.709187;0.071332;0.701402;,
  0.467105;0.245216;0.849519;,
  -0.546905;0.457332;-0.701243;,
  -0.248029;0.469750;-0.847240;,
  -0.401717;-0.885011;0.235325;,
  0.674249;0.699539;-0.236714;,
  -0.152821;-0.957531;-0.244501;,
  0.188902;-0.682506;-0.706046;,
  -0.062629;-0.520484;-0.851572;,
  0.165087;0.593862;0.787448;,
  -0.092218;-0.298085;0.950074;,
  0.311797;0.920788;0.234377;,
  -0.265072;-0.921342;0.284370;,
  0.219899;0.948926;-0.226240;,
  -0.289392;-0.890981;-0.349864;,
  0.173914;0.598000;-0.782400;,
  -0.070750;-0.394427;-0.916200;;
  124;
  3;20,12,22;,
  3;44,22,12;,
  3;12,13,44;,
  3;37,44,13;,
  3;13,2,37;,
  3;45,1,30;,
  3;30,31,45;,
  3;46,45,31;,
  3;31,11,46;,
  3;40,46,11;,
  3;47,4,26;,
  3;26,27,47;,
  3;48,47,27;,
  3;27,9,48;,
  3;36,48,9;,
  3;49,0,21;,
  3;21,23,49;,
  3;50,49,23;,
  3;23,8,50;,
  3;38,50,8;,
  3;32,14,34;,
  3;51,34,14;,
  3;14,15,51;,
  3;41,51,15;,
  3;15,3,41;,
  3;24,18,25;,
  3;52,25,18;,
  3;18,19,52;,
  3;39,52,19;,
  3;19,6,39;,
  3;28,16,29;,
  3;53,29,16;,
  3;16,17,53;,
  3;43,53,17;,
  3;17,7,43;,
  3;54,5,33;,
  3;33,35,54;,
  3;55,54,35;,
  3;35,10,55;,
  3;42,55,10;,
  3;1,20,0;,
  3;1,0,28;,
  3;5,24,4;,
  3;5,4,32;,
  3;45,12,20;,
  3;45,20,1;,
  3;46,13,12;,
  3;46,12,45;,
  3;40,2,13;,
  3;40,13,46;,
  3;32,4,47;,
  3;32,47,14;,
  3;14,47,48;,
  3;14,48,15;,
  3;15,48,36;,
  3;15,36,3;,
  3;28,0,49;,
  3;28,49,16;,
  3;16,49,50;,
  3;16,50,17;,
  3;17,50,38;,
  3;17,38,7;,
  3;54,18,24;,
  3;54,24,5;,
  3;55,19,18;,
  3;55,18,54;,
  3;42,6,19;,
  3;42,19,55;,
  3;22,21,0;,
  3;22,0,20;,
  3;44,23,21;,
  3;44,21,22;,
  3;37,8,23;,
  3;37,23,44;,
  3;25,26,4;,
  3;25,4,24;,
  3;52,27,26;,
  3;52,26,25;,
  3;39,9,27;,
  3;39,27,52;,
  3;29,30,1;,
  3;29,1,28;,
  3;53,31,30;,
  3;53,30,29;,
  3;43,11,31;,
  3;43,31,53;,
  3;32,34,33;,
  3;32,33,5;,
  3;51,35,33;,
  3;51,33,34;,
  3;41,10,35;,
  3;41,35,51;,
  3;57,3,36;,
  3;57,36,56;,
  3;2,40,57;,
  3;2,57,56;,
  3;56,58,37;,
  3;56,37,2;,
  3;56,36,9;,
  3;56,9,58;,
  3;59,41,3;,
  3;59,3,57;,
  3;40,11,59;,
  3;40,59,57;,
  3;60,8,37;,
  3;60,37,58;,
  3;58,9,39;,
  3;58,39,60;,
  3;59,61,10;,
  3;59,10,41;,
  3;59,11,43;,
  3;59,43,61;,
  3;62,38,8;,
  3;62,8,60;,
  3;39,6,62;,
  3;39,62,60;,
  3;61,63,42;,
  3;61,42,10;,
  3;43,7,63;,
  3;43,63,61;,
  3;63,7,38;,
  3;63,38,62;,
  3;6,42,63;,
  3;6,63,62;;
 }
 MeshTextureCoords {
  90;
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
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;0.000000;;
 }
}
