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
 123;
 0.00000;13.79382;0.00000;,
 1.76200;13.49282;-1.54800;,
 0.00000;13.49282;-2.18900;,
 0.00000;13.79382;0.00000;,
 2.49200;13.49282;0.00000;,
 0.00000;13.79382;0.00000;,
 1.76200;13.49282;1.54800;,
 0.00000;13.79382;0.00000;,
 0.00000;13.49282;2.18900;,
 0.00000;13.79382;0.00000;,
 -1.76200;13.49282;1.54800;,
 0.00000;13.79382;0.00000;,
 -2.49200;13.49282;0.00000;,
 0.00000;13.79382;0.00000;,
 -1.76200;13.49282;-1.54800;,
 0.00000;13.79382;0.00000;,
 0.00000;13.49282;-2.18900;,
 3.12900;12.06182;-2.74800;,
 0.00000;12.06182;-3.88600;,
 4.42500;12.06182;0.00000;,
 3.12900;12.06182;2.74800;,
 0.00000;12.06182;3.88600;,
 -3.12900;12.06182;2.74800;,
 -4.42500;12.06182;0.00000;,
 -3.12900;12.06182;-2.74800;,
 0.00000;12.06182;-3.88600;,
 3.83116;9.58563;-3.31524;,
 0.00000;9.58563;-4.68846;,
 5.41809;9.58563;0.00000;,
 3.83116;9.58563;3.31524;,
 0.00000;9.58563;4.68846;,
 -3.83116;9.58563;3.31524;,
 -5.41809;9.58563;0.00000;,
 -3.83116;9.58563;-3.31524;,
 0.00000;9.58563;-4.68846;,
 4.14682;6.84587;-3.58839;,
 0.00000;6.84587;-5.07476;,
 5.86449;6.84587;0.00000;,
 4.14682;6.84587;3.58839;,
 0.00000;6.84587;5.07476;,
 -4.14682;6.84587;3.58839;,
 -5.86449;6.84587;0.00000;,
 -4.14682;6.84587;-3.58839;,
 0.00000;6.84587;-5.07476;,
 3.83116;4.10611;-3.31524;,
 0.00000;4.10611;-4.68846;,
 5.41809;4.10611;0.00000;,
 3.83116;4.10611;3.31524;,
 0.00000;4.10611;4.68846;,
 -3.83116;4.10611;3.31524;,
 -5.41809;4.10611;0.00000;,
 -3.83116;4.10611;-3.31524;,
 0.00000;4.10611;-4.68846;,
 3.12867;1.62967;-2.74816;,
 0.00000;1.62967;-3.88649;,
 4.42460;1.62967;0.00000;,
 3.12867;1.62967;2.74816;,
 0.00000;1.62967;3.88649;,
 -3.12867;1.62967;2.74816;,
 -4.42460;1.62967;0.00000;,
 -3.12867;1.62967;-2.74816;,
 0.00000;1.62967;-3.88649;,
 1.76215;0.19843;-1.54784;,
 0.00000;0.19843;-2.18897;,
 2.49206;0.19843;0.00000;,
 1.76215;0.19843;1.54784;,
 0.00000;0.19843;2.18897;,
 -1.76215;0.19843;1.54784;,
 -2.49206;0.19843;0.00000;,
 -1.76215;0.19843;-1.54784;,
 0.00000;0.19843;-2.18897;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;-0.10240;-0.00000;,
 0.00000;13.89546;0.00000;,
 1.85211;13.57907;-1.62717;,
 0.00000;13.57907;-2.30095;,
 0.00000;13.89546;0.00000;,
 2.61944;13.57907;0.00000;,
 0.00000;13.89546;0.00000;,
 1.85211;13.57907;1.62717;,
 0.00000;13.89546;0.00000;,
 0.00000;13.57907;2.30095;,
 0.00000;13.89546;0.00000;,
 -1.85211;13.57907;1.62717;,
 0.00000;13.89546;0.00000;,
 -2.61944;13.57907;0.00000;,
 0.00000;13.89546;0.00000;,
 0.00000;13.57907;-2.30095;,
 -1.85211;13.57907;-1.62717;,
 0.00000;13.89546;0.00000;,
 0.00000;12.07489;-4.08473;,
 -3.28902;12.07489;-2.88853;,
 0.00000;12.07489;-4.08473;,
 3.28902;12.07489;-2.88853;,
 -4.65130;12.07489;0.00000;,
 4.65130;12.07489;0.00000;,
 -3.28902;12.07489;2.88853;,
 3.28902;12.07489;2.88853;,
 0.00000;12.07489;4.08473;,
 -4.23039;8.49400;-4.23039;,
 0.00000;8.49400;-5.98267;,
 0.00000;3.68406;-5.88950;,
 -4.23039;2.59324;-4.16451;,
 -5.98267;-0.04024;0.00000;,
 -5.98267;8.49400;0.00000;,
 0.00000;8.49400;-5.98267;,
 4.23039;8.49400;-4.23039;,
 4.23039;2.59324;-4.16451;,
 0.00000;3.68406;-5.88950;,
 -4.23039;-2.67372;4.16451;,
 -4.23039;8.49400;4.23039;,
 5.98267;8.49400;0.00000;,
 5.98267;-0.04024;-0.00000;,
 0.00000;-3.76454;5.88950;,
 0.00000;8.49400;5.98267;,
 4.23039;8.49400;4.23039;,
 4.23039;-2.67372;4.16451;;
 
 128;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 3;63,62,71;,
 3;62,64,72;,
 3;64,65,73;,
 3;65,66,74;,
 3;66,67,75;,
 3;67,68,76;,
 3;68,69,77;,
 3;69,70,78;,
 3;79,80,81;,
 3;82,83,80;,
 3;84,85,83;,
 3;86,87,85;,
 3;88,89,87;,
 3;90,91,89;,
 3;92,93,94;,
 3;95,94,91;,
 4;96,97,94,93;,
 4;98,81,80,99;,
 4;97,100,91,94;,
 4;99,80,83,101;,
 4;100,102,89,91;,
 4;101,83,85,103;,
 4;102,104,87,89;,
 4;103,85,87,104;,
 4;105,97,96,106;,
 4;105,106,107,108;,
 4;105,108,109,110;,
 4;105,110,100,97;,
 4;111,98,99,112;,
 4;111,112,113,114;,
 4;110,109,115,116;,
 4;110,116,102,100;,
 4;112,99,101,117;,
 4;112,117,118,113;,
 4;116,115,119,120;,
 4;116,120,104,102;,
 4;117,101,103,121;,
 4;117,121,122,118;,
 4;120,119,122,121;,
 4;120,121,103,104;,
 3;81,80,79;,
 3;80,83,82;,
 3;83,85,84;,
 3;85,87,86;,
 3;87,89,88;,
 3;89,91,90;,
 3;94,93,92;,
 3;91,94,95;,
 4;93,94,97,96;,
 4;99,80,81,98;,
 4;94,91,100,97;,
 4;101,83,80,99;,
 4;91,89,102,100;,
 4;103,85,83,101;,
 4;89,87,104,102;,
 4;104,87,85,103;,
 4;106,96,97,105;,
 4;108,107,106,105;,
 4;110,109,108,105;,
 4;97,100,110,105;,
 4;112,99,98,111;,
 4;114,113,112,111;,
 4;116,115,109,110;,
 4;100,102,116,110;,
 4;117,101,99,112;,
 4;113,118,117,112;,
 4;120,119,115,116;,
 4;102,104,120,116;,
 4;121,103,101,117;,
 4;118,122,121,117;,
 4;121,122,119,120;,
 4;104,103,121,120;;
 
 MeshMaterialList {
  2;
  128;
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.116000;0.116000;0.116000;1.000000;;
   6.000000;
   0.510000;0.510000;0.510000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.166400;0.166400;0.166400;1.000000;;
   6.000000;
   0.280000;0.280000;0.280000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  124;
  -0.000000;1.000000;0.000000;,
  0.000000;0.915158;-0.403095;,
  0.255969;0.922913;-0.287597;,
  0.365287;0.930895;0.000000;,
  0.255969;0.922913;0.287597;,
  0.000000;0.915158;0.403095;,
  -0.255969;0.922913;0.287597;,
  -0.365287;0.930895;0.000000;,
  -0.255969;0.922913;-0.287597;,
  0.000000;0.562522;-0.826782;,
  0.540471;0.587879;-0.601904;,
  0.788314;0.615274;0.000000;,
  0.540471;0.587879;0.601904;,
  0.000000;0.562522;0.826782;,
  -0.540471;0.587879;0.601904;,
  -0.788314;0.615274;0.000000;,
  -0.540471;0.587879;-0.601904;,
  0.000000;0.226416;-0.974031;,
  0.649215;0.245513;-0.719891;,
  0.963757;0.266781;0.000000;,
  0.649215;0.245513;0.719891;,
  0.000000;0.226416;0.974031;,
  -0.649215;0.245513;0.719891;,
  -0.963757;0.266781;0.000000;,
  -0.649215;0.245513;-0.719891;,
  0.000000;0.000000;-1.000000;,
  0.669741;0.000000;-0.742595;,
  1.000000;0.000000;0.000000;,
  0.669741;0.000000;0.742595;,
  0.000000;0.000000;1.000000;,
  -0.669741;0.000000;0.742595;,
  -1.000000;0.000000;0.000000;,
  -0.669741;0.000000;-0.742595;,
  0.000000;-0.226353;-0.974045;,
  0.649228;-0.245498;-0.719884;,
  0.963747;-0.266817;-0.000000;,
  0.649228;-0.245498;0.719884;,
  0.000000;-0.226353;0.974045;,
  -0.649228;-0.245498;0.719884;,
  -0.963747;-0.266817;0.000000;,
  -0.649228;-0.245498;-0.719884;,
  0.000000;-0.562490;-0.826804;,
  0.540530;-0.587833;-0.601897;,
  0.788350;-0.615226;-0.000000;,
  0.540530;-0.587833;0.601897;,
  0.000000;-0.562490;0.826804;,
  -0.540530;-0.587833;0.601897;,
  -0.788350;-0.615226;0.000000;,
  -0.540530;-0.587833;-0.601897;,
  0.000000;-0.915183;-0.403040;,
  0.255997;-0.922912;-0.287576;,
  0.365334;-0.930876;-0.000000;,
  0.255997;-0.922912;0.287576;,
  0.000000;-0.915183;0.403040;,
  -0.255997;-0.922912;0.287576;,
  -0.365334;-0.930876;0.000000;,
  -0.255997;-0.922912;-0.287576;,
  -0.000000;-1.000000;0.000000;,
  0.000000;1.000000;-0.000000;,
  0.000000;-0.015259;-0.999883;,
  0.704691;-0.008801;-0.709460;,
  0.999998;-0.001775;-0.000332;,
  0.705672;-0.003995;0.708527;,
  0.000000;-0.006750;0.999977;,
  -0.705672;-0.003995;0.708527;,
  -0.999998;-0.001775;-0.000332;,
  -0.704691;-0.008801;-0.709460;,
  0.255969;0.922913;-0.287597;,
  0.000000;0.915158;-0.403095;,
  0.365287;0.930895;0.000000;,
  -0.255969;0.922913;-0.287597;,
  0.255969;0.922913;0.287597;,
  -0.365287;0.930895;0.000000;,
  0.000000;0.915158;0.403095;,
  -0.255969;0.922913;0.287597;,
  0.000000;0.620934;-0.783863;,
  -0.538156;0.616766;-0.574446;,
  0.538156;0.616766;-0.574446;,
  -0.789597;0.613626;0.000000;,
  0.789597;0.613626;0.000000;,
  -0.538156;0.616766;0.574446;,
  0.538156;0.616766;0.574446;,
  0.000000;0.620934;0.783863;,
  -0.685631;0.205313;-0.698396;,
  0.000000;0.224622;-0.974446;,
  -0.982494;0.186294;-0.000169;,
  0.685631;0.205313;-0.698396;,
  -0.685782;0.207619;0.697565;,
  0.982494;0.186294;-0.000169;,
  0.000000;0.228822;0.973468;,
  0.685782;0.207619;0.697565;,
  0.000000;-0.915158;0.403095;,
  -0.255969;-0.922913;0.287597;,
  -0.000000;-1.000000;0.000000;,
  -0.365287;-0.930895;0.000000;,
  -0.255969;-0.922913;-0.287597;,
  0.000000;-0.915158;-0.403095;,
  0.255969;-0.922913;-0.287597;,
  0.365287;-0.930895;-0.000000;,
  0.255969;-0.922913;0.287597;,
  0.538156;-0.616766;0.574446;,
  0.000000;-0.620934;0.783863;,
  -0.538156;-0.616766;0.574446;,
  0.789597;-0.613626;-0.000000;,
  -0.789597;-0.613626;0.000000;,
  0.538156;-0.616766;-0.574446;,
  -0.538156;-0.616766;-0.574446;,
  0.000000;-0.620934;-0.783863;,
  0.000000;-0.224622;0.974446;,
  0.685631;-0.205313;0.698396;,
  0.704691;0.008801;0.709460;,
  0.000000;0.015259;0.999883;,
  0.982494;-0.186294;0.000169;,
  0.999998;0.001775;0.000332;,
  -0.685631;-0.205313;0.698396;,
  -0.704691;0.008801;0.709460;,
  0.685782;-0.207619;-0.697565;,
  0.705672;0.003995;-0.708527;,
  -0.982494;-0.186294;0.000169;,
  -0.999998;0.001775;0.000332;,
  0.000000;-0.228822;-0.973468;,
  0.000000;0.006750;-0.999977;,
  -0.685782;-0.207619;-0.697565;,
  -0.705672;0.003995;-0.708527;;
  128;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;,
  3;58,67,68;,
  3;58,69,67;,
  3;58,71,69;,
  3;58,73,71;,
  3;58,74,73;,
  3;58,72,74;,
  3;58,68,70;,
  3;58,70,72;,
  4;75,76,70,68;,
  4;75,68,67,77;,
  4;76,78,72,70;,
  4;77,67,69,79;,
  4;78,80,74,72;,
  4;79,69,71,81;,
  4;80,82,73,74;,
  4;81,71,73,82;,
  4;83,76,75,84;,
  4;83,84,59,66;,
  4;83,66,65,85;,
  4;83,85,78,76;,
  4;84,75,77,86;,
  4;84,86,60,59;,
  4;85,65,64,87;,
  4;85,87,80,78;,
  4;86,77,79,88;,
  4;86,88,61,60;,
  4;87,64,63,89;,
  4;87,89,82,80;,
  4;88,79,81,90;,
  4;88,90,62,61;,
  4;89,63,62,90;,
  4;89,90,81,82;,
  3;91,92,93;,
  3;92,94,93;,
  3;94,95,93;,
  3;95,96,93;,
  3;96,97,93;,
  3;97,98,93;,
  3;99,91,93;,
  3;98,99,93;,
  4;91,99,100,101;,
  4;102,92,91,101;,
  4;99,98,103,100;,
  4;104,94,92,102;,
  4;98,97,105,103;,
  4;106,95,94,104;,
  4;97,96,107,105;,
  4;107,96,95,106;,
  4;108,101,100,109;,
  4;110,111,108,109;,
  4;112,113,110,109;,
  4;100,103,112,109;,
  4;114,102,101,108;,
  4;111,115,114,108;,
  4;116,117,113,112;,
  4;103,105,116,112;,
  4;118,104,102,114;,
  4;115,119,118,114;,
  4;120,121,117,116;,
  4;105,107,120,116;,
  4;122,106,104,118;,
  4;119,123,122,118;,
  4;122,123,121,120;,
  4;107,106,122,120;;
 }
 MeshTextureCoords {
  123;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.183890;,
  0.000000;0.183890;,
  0.187500;0.000000;,
  0.250000;0.183890;,
  0.312500;0.000000;,
  0.375000;0.183890;,
  0.437500;0.000000;,
  0.500000;0.183890;,
  0.562500;0.000000;,
  0.625000;0.183890;,
  0.687500;0.000000;,
  0.750000;0.183890;,
  0.937500;0.000000;,
  1.000000;0.183890;,
  0.875000;0.183890;,
  0.812500;0.000000;,
  1.000000;0.354800;,
  0.875000;0.354800;,
  0.000000;0.354800;,
  0.125000;0.354800;,
  0.750000;0.354800;,
  0.250000;0.354800;,
  0.625000;0.354800;,
  0.375000;0.354800;,
  0.500000;0.354800;,
  0.875000;0.526390;,
  1.000000;0.526390;,
  1.000000;1.000000;,
  0.875000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.526390;,
  0.000000;0.526390;,
  0.125000;0.526390;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.526390;,
  0.250000;0.526390;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.526390;,
  0.375000;0.526390;,
  0.375000;1.000000;;
 }
}
