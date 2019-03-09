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
 130;
 39.61334;24.83985;15.40141;,
 -0.01073;26.01245;14.81994;,
 -37.08352;25.59297;13.44552;,
 39.61334;24.83985;15.40141;,
 40.57953;25.80890;3.74866;,
 -0.01073;26.01245;14.81994;,
 -37.08352;25.59297;13.44552;,
 -0.01073;26.01245;14.81994;,
 -38.04206;25.80890;-4.12458;,
 -0.01073;26.01245;14.81994;,
 -36.81086;25.70712;-13.81498;,
 -38.04206;25.80890;-4.12458;,
 -0.01073;26.01245;14.81994;,
 38.33056;25.70712;-11.89707;,
 -36.81086;25.70712;-13.81498;,
 40.57953;25.80890;3.74866;,
 -38.04206;25.80890;-4.12458;,
 -36.81086;25.70712;-13.81498;,
 -35.89531;25.05777;-15.80840;,
 -36.81086;25.70712;-13.81498;,
 9.13433;25.77202;-17.41064;,
 -35.89531;25.05777;-15.80840;,
 38.33056;25.70712;-11.89707;,
 9.13433;25.77202;-17.41064;,
 -36.81086;25.70712;-13.81498;,
 38.06514;22.12045;-12.12733;,
 9.13433;25.77202;-17.41064;,
 38.33056;25.70712;-11.89707;,
 9.13433;25.77202;-17.41064;,
 8.06227;17.12339;-17.41064;,
 -24.02317;17.32092;-16.90360;,
 -35.89531;25.05777;-15.80840;,
 38.06514;22.12045;-12.12733;,
 32.85232;14.74945;-10.26381;,
 8.06227;17.12339;-17.41064;,
 9.13433;25.77202;-17.41064;,
 8.06227;17.12339;-17.41064;,
 8.06227;12.44969;-11.96012;,
 -24.11493;12.91434;-11.90951;,
 -24.02317;17.32092;-16.90360;,
 32.85232;14.74945;-10.26381;,
 8.06227;12.44969;-11.96012;,
 8.06227;17.12339;-17.41064;,
 8.06227;12.44969;-11.96012;,
 8.06227;12.18503;0.79620;,
 -24.15246;12.44969;0.79620;,
 -24.11493;12.91434;-11.90951;,
 32.85232;14.74945;-10.26381;,
 8.06227;12.18503;0.79620;,
 8.06227;12.44969;-11.96012;,
 32.85232;14.74945;-10.26381;,
 31.64143;13.37987;11.13058;,
 8.06227;12.18503;0.79620;,
 8.06227;12.18503;0.79620;,
 8.06227;14.39147;14.80155;,
 -20.03330;14.62402;14.81116;,
 -24.15246;12.44969;0.79620;,
 31.64143;13.37987;11.13058;,
 8.06227;14.39147;14.80155;,
 8.06227;12.18503;0.79620;,
 31.64143;13.37987;11.13058;,
 36.93574;19.09874;15.48312;,
 8.06227;14.39147;14.80155;,
 8.06227;14.39147;14.80155;,
 9.13433;21.07409;17.03472;,
 -32.29316;20.18518;15.87103;,
 -20.03330;14.62402;14.81116;,
 36.93574;19.09874;15.48312;,
 9.13433;21.07409;17.03472;,
 8.06227;14.39147;14.80155;,
 36.93574;19.09874;15.48312;,
 39.61334;24.83985;15.40141;,
 9.13433;21.07409;17.03472;,
 9.13433;21.07409;17.03472;,
 -37.08352;25.59297;13.44552;,
 -32.29316;20.18518;15.87103;,
 9.13433;21.07409;17.03472;,
 39.61334;24.83985;15.40141;,
 -37.08352;25.59297;13.44552;,
 39.61334;24.83985;15.40141;,
 39.99895;21.86422;3.74866;,
 40.57953;25.80890;3.74866;,
 36.93574;19.09874;15.48312;,
 36.93574;19.09874;15.48312;,
 35.30867;17.12339;3.74866;,
 39.99895;21.86422;3.74866;,
 31.64143;13.37987;11.13058;,
 35.30867;17.12339;3.74866;,
 36.93574;19.09874;15.48312;,
 31.64143;13.37987;11.13058;,
 35.30867;17.12339;3.74866;,
 39.99895;21.86422;3.74866;,
 38.06514;22.12045;-12.12733;,
 38.33056;25.70712;-11.89707;,
 40.57953;25.80890;3.74866;,
 35.30867;17.12339;3.74866;,
 32.85232;14.74945;-10.26381;,
 38.06514;22.12045;-12.12733;,
 39.99895;21.86422;3.74866;,
 -32.29316;20.18518;15.87103;,
 -35.33015;16.33325;3.74866;,
 -20.03330;14.62402;14.81116;,
 -32.29316;20.18518;15.87103;,
 -36.48849;21.07409;3.74866;,
 -35.33015;16.33325;3.74866;,
 -37.08352;25.59297;13.44552;,
 -36.48849;21.07409;3.74866;,
 -32.29316;20.18518;15.87103;,
 -24.15246;12.44969;0.79620;,
 -20.03330;14.62402;14.81116;,
 -37.08352;25.59297;13.44552;,
 -38.04206;25.80890;-4.12458;,
 -36.48849;21.07409;3.74866;,
 -35.33015;16.33325;3.74866;,
 -35.33015;16.33325;-8.06119;,
 -24.11493;12.91434;-11.90951;,
 -24.15246;12.44969;0.79620;,
 -36.48849;21.07409;3.74866;,
 -35.89531;25.05777;-15.80840;,
 -35.33015;16.33325;-8.06119;,
 -35.33015;16.33325;3.74866;,
 -38.04206;25.80890;-4.12458;,
 -35.89531;25.05777;-15.80840;,
 -36.48849;21.07409;3.74866;,
 -35.33015;16.33325;-8.06119;,
 -24.02317;17.32092;-16.90360;,
 -24.11493;12.91434;-11.90951;,
 -35.89531;25.05777;-15.80840;,
 -24.02317;17.32092;-16.90360;,
 -35.33015;16.33325;-8.06119;;
 
 52;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;15,13,12;,
 3;16,17,18;,
 3;19,20,21;,
 3;22,23,24;,
 3;25,26,27;,
 3;28,29,30;,
 3;28,30,31;,
 3;32,33,34;,
 3;32,34,35;,
 3;36,37,38;,
 3;36,38,39;,
 3;40,41,42;,
 3;43,44,45;,
 3;43,45,46;,
 3;47,48,49;,
 3;50,51,52;,
 3;53,54,55;,
 3;53,55,56;,
 3;57,58,59;,
 3;60,61,62;,
 3;63,64,65;,
 3;63,65,66;,
 3;67,68,69;,
 3;70,71,72;,
 3;73,74,75;,
 3;76,77,78;,
 3;79,80,81;,
 3;82,80,79;,
 3;83,84,85;,
 3;86,87,88;,
 3;89,33,90;,
 3;91,92,93;,
 3;91,93,94;,
 3;95,96,97;,
 3;95,97,98;,
 3;99,100,101;,
 3;102,103,104;,
 3;105,106,107;,
 3;100,108,109;,
 3;110,111,112;,
 3;113,114,115;,
 3;113,115,116;,
 3;117,118,119;,
 3;117,119,120;,
 3;121,122,123;,
 3;124,125,126;,
 3;127,128,129;;
 
 MeshMaterialList {
  1;
  52;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.166275;0.592941;0.203922;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\TEXTURE\\Object\\Aerial floor.jpg";
   }
  }
 }
 MeshNormals {
  46;
  0.003990;0.006762;0.999969;,
  -0.029023;0.153581;0.987710;,
  0.015689;0.999110;0.039151;,
  -0.027074;-0.480094;0.876799;,
  -0.005953;0.999982;0.001207;,
  -0.006718;0.996531;-0.082947;,
  -0.813984;-0.525681;-0.247164;,
  -0.013583;0.987705;-0.155739;,
  -0.194793;-0.913314;-0.357648;,
  -0.209809;-0.435788;-0.875253;,
  0.084760;-0.480806;-0.872720;,
  0.036744;-0.955811;-0.291677;,
  -0.128945;-0.989935;0.058333;,
  0.021390;-0.997887;0.061356;,
  -0.019362;-0.042069;0.998927;,
  0.002972;0.052761;0.998603;,
  0.924202;-0.381191;-0.023346;,
  0.735350;-0.677319;-0.022341;,
  0.198505;-0.121026;-0.972599;,
  -0.595876;-0.786614;0.161772;,
  -0.943459;-0.324288;0.068725;,
  -0.608846;-0.707574;-0.358672;,
  0.001017;0.999998;-0.001456;,
  0.013639;-0.659193;0.751850;,
  0.394259;-0.859530;-0.325221;,
  0.359264;-0.912802;0.194221;,
  0.060431;-0.343000;0.937389;,
  -0.163191;-0.959775;0.228475;,
  0.028224;0.995967;0.085165;,
  0.003947;0.999895;0.013931;,
  -0.011793;0.999847;0.012931;,
  -0.865139;-0.498659;-0.053610;,
  -0.686132;-0.723111;-0.079580;,
  -0.026425;0.946856;-0.320570;,
  0.077945;0.028641;-0.996546;,
  0.185446;0.049285;-0.981418;,
  -0.036771;0.084529;-0.995742;,
  -0.001236;-0.189842;0.981814;,
  0.953070;-0.284279;0.104132;,
  0.988727;-0.145522;-0.035256;,
  0.790114;-0.610247;0.057605;,
  0.876929;-0.471792;-0.091696;,
  0.985829;-0.104917;-0.130894;,
  -0.735936;-0.597343;0.318715;,
  -0.895746;-0.421466;0.141443;,
  -0.992408;-0.111114;-0.052733;;
  52;
  3;0,3,1;,
  3;28,2,29;,
  3;30,29,4;,
  3;29,5,4;,
  3;29,22,5;,
  3;2,22,29;,
  3;31,32,6;,
  3;5,7,33;,
  3;22,7,5;,
  3;18,34,35;,
  3;34,10,9;,
  3;34,9,36;,
  3;18,24,10;,
  3;18,10,34;,
  3;10,11,8;,
  3;10,8,9;,
  3;24,11,10;,
  3;11,13,12;,
  3;11,12,8;,
  3;24,13,11;,
  3;24,25,13;,
  3;13,23,27;,
  3;13,27,12;,
  3;25,23,13;,
  3;25,26,23;,
  3;23,15,14;,
  3;23,14,37;,
  3;26,15,23;,
  3;26,0,15;,
  3;15,1,14;,
  3;15,0,1;,
  3;38,16,39;,
  3;40,16,38;,
  3;40,17,16;,
  3;25,17,40;,
  3;25,24,17;,
  3;16,41,42;,
  3;16,42,39;,
  3;17,24,41;,
  3;17,41,16;,
  3;43,19,27;,
  3;43,20,19;,
  3;44,20,43;,
  3;19,12,27;,
  3;44,31,20;,
  3;19,21,8;,
  3;19,8,12;,
  3;20,6,45;,
  3;20,21,19;,
  3;31,6,20;,
  3;21,9,8;,
  3;6,9,21;;
 }
 MeshTextureCoords {
  130;
  0.571429;0.000000;,
  0.428571;0.142857;,
  0.428571;0.000000;,
  0.714286;0.000000;,
  0.714286;0.142857;,
  0.571429;0.142857;,
  0.285714;0.285714;,
  0.285714;0.428571;,
  0.142857;0.428571;,
  0.428571;0.571429;,
  0.428571;0.714286;,
  0.285714;0.714286;,
  0.714286;0.571429;,
  0.714286;0.714286;,
  0.571429;0.714286;,
  0.857143;0.571429;,
  0.142857;0.857143;,
  0.142857;1.000000;,
  0.000000;1.000000;,
  0.428571;0.000000;,
  0.428571;0.142857;,
  0.285714;0.142857;,
  0.714286;0.000000;,
  0.571429;0.142857;,
  0.571429;0.000000;,
  1.000000;0.000000;,
  0.857143;0.142857;,
  0.857143;0.000000;,
  0.428571;0.428571;,
  0.428571;0.571429;,
  0.285714;0.571429;,
  0.285714;0.428571;,
  1.000000;0.428571;,
  1.000000;0.571429;,
  0.857143;0.571429;,
  0.857143;0.428571;,
  0.428571;0.857143;,
  0.428571;1.000000;,
  0.285714;1.000000;,
  0.285714;0.857143;,
  1.000000;0.857143;,
  0.857143;1.000000;,
  0.857143;0.857143;,
  0.428571;0.285714;,
  0.428571;0.428571;,
  0.285714;0.428571;,
  0.285714;0.285714;,
  1.000000;0.285714;,
  0.857143;0.428571;,
  0.857143;0.285714;,
  1.000000;0.428571;,
  1.000000;0.571429;,
  0.857143;0.571429;,
  0.428571;0.714286;,
  0.428571;0.857143;,
  0.285714;0.857143;,
  0.285714;0.714286;,
  1.000000;0.714286;,
  0.857143;0.857143;,
  0.857143;0.714286;,
  1.000000;0.142857;,
  1.000000;0.285714;,
  0.857143;0.285714;,
  0.428571;0.428571;,
  0.428571;0.571429;,
  0.285714;0.571429;,
  0.285714;0.428571;,
  1.000000;0.428571;,
  0.857143;0.571429;,
  0.857143;0.428571;,
  1.000000;0.714286;,
  1.000000;0.857143;,
  0.857143;0.857143;,
  0.428571;0.857143;,
  0.285714;1.000000;,
  0.285714;0.857143;,
  0.571429;0.857143;,
  0.571429;1.000000;,
  0.428571;1.000000;,
  0.142857;0.000000;,
  0.142857;0.142857;,
  0.000000;0.142857;,
  0.285714;0.000000;,
  0.571429;0.000000;,
  0.571429;0.142857;,
  0.428571;0.142857;,
  0.857143;0.000000;,
  0.714286;0.142857;,
  0.714286;0.000000;,
  1.000000;0.428571;,
  0.857143;0.571429;,
  0.142857;0.571429;,
  0.142857;0.714286;,
  0.000000;0.714286;,
  0.000000;0.571429;,
  0.571429;0.571429;,
  0.571429;0.714286;,
  0.428571;0.714286;,
  0.428571;0.571429;,
  0.285714;0.000000;,
  0.142857;0.142857;,
  0.142857;0.000000;,
  0.571429;0.000000;,
  0.571429;0.142857;,
  0.428571;0.142857;,
  1.000000;0.000000;,
  0.857143;0.142857;,
  0.857143;0.000000;,
  0.000000;0.285714;,
  0.000000;0.142857;,
  1.000000;0.285714;,
  1.000000;0.428571;,
  0.857143;0.428571;,
  0.142857;0.571429;,
  0.142857;0.714286;,
  0.000000;0.714286;,
  0.000000;0.571429;,
  0.571429;0.571429;,
  0.571429;0.714286;,
  0.428571;0.714286;,
  0.428571;0.571429;,
  1.000000;0.571429;,
  0.857143;0.714286;,
  0.857143;0.571429;,
  0.285714;0.857143;,
  0.285714;1.000000;,
  0.142857;1.000000;,
  0.571429;0.857143;,
  0.428571;1.000000;,
  0.428571;0.857143;;
 }
}
