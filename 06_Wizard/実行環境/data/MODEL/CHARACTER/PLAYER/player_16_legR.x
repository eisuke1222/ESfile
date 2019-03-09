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
 79;
 0.04519;0.00403;-0.01296;,
 0.04519;-0.21693;-1.27762;,
 -0.57000;-0.21693;-0.96266;,
 0.04519;0.00403;-0.01296;,
 -0.72666;-0.21693;0.02539;,
 0.04519;0.00403;-0.01296;,
 -0.57000;-0.21693;1.01345;,
 0.04519;0.00403;-0.01296;,
 0.04519;-0.21693;1.25170;,
 0.04519;0.00403;-0.01296;,
 0.66037;-0.21693;1.01345;,
 0.04519;0.00403;-0.01296;,
 0.81704;-0.21693;0.02539;,
 0.04519;0.00403;-0.01296;,
 0.66037;-0.21693;-0.96266;,
 0.04519;0.00403;-0.01296;,
 0.04519;-0.21693;-1.27762;,
 0.04519;-0.84618;-2.52590;,
 -1.09152;-0.84618;-1.99492;,
 -1.38101;-0.84618;-0.16924;,
 -1.09152;-0.84618;1.65645;,
 0.04519;-0.84618;2.14767;,
 1.18190;-0.84618;1.65645;,
 1.47138;-0.84618;-0.16924;,
 1.18190;-0.84618;-1.99492;,
 0.04519;-0.84618;-2.52590;,
 0.04519;-1.78791;-3.43176;,
 -1.44000;-1.78791;-2.76397;,
 -1.81823;-1.78791;-0.37860;,
 -1.44000;-1.78791;2.00677;,
 0.04519;-1.78791;2.67456;,
 1.53037;-1.78791;2.00677;,
 1.90860;-1.78791;-0.37860;,
 1.53037;-1.78791;-2.76397;,
 0.04519;-1.78791;-3.43176;,
 0.04519;-2.89877;-3.68331;,
 -1.56236;-2.89877;-2.96050;,
 -1.97176;-2.89877;-0.37860;,
 -1.56236;-2.89877;2.20331;,
 0.04519;-2.89877;2.92612;,
 1.65273;-2.89877;2.20331;,
 2.06213;-2.89877;-0.37860;,
 1.65273;-2.89877;-2.96050;,
 0.04519;-2.89877;-3.68331;,
 0.04519;-3.14805;-3.58789;,
 -1.51595;-3.14805;-2.88595;,
 -1.91352;-3.14805;-0.37860;,
 -1.51595;-3.14805;2.12876;,
 0.04519;-3.14805;2.83070;,
 1.60632;-3.14805;2.12876;,
 2.00390;-3.14805;-0.37860;,
 1.60632;-3.14805;-2.88595;,
 0.04519;-3.14805;-3.58789;,
 0.04519;-3.39819;-2.83488;,
 -1.14965;-3.39819;-2.29764;,
 -1.45394;-3.39819;-0.37860;,
 -1.14965;-3.39819;1.54045;,
 0.04519;-3.39819;2.07769;,
 1.24003;-3.39819;1.54045;,
 1.54431;-3.39819;-0.37860;,
 1.24003;-3.39819;-2.29764;,
 0.04519;-3.39819;-2.83488;,
 0.04519;-3.48798;-1.70793;,
 -0.60146;-3.48798;-1.41718;,
 -0.76614;-3.48798;-0.37860;,
 -0.60146;-3.48798;0.65998;,
 0.04519;-3.48798;0.95074;,
 0.69183;-3.48798;0.65998;,
 0.85651;-3.48798;-0.37860;,
 0.69183;-3.48798;-1.41718;,
 0.04519;-3.48798;-1.70793;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;,
 0.04519;-3.57668;-0.37860;;
 
 64;
 3;0,1,2;,
 3;3,2,4;,
 3;5,4,6;,
 3;7,6,8;,
 3;9,8,10;,
 3;11,10,12;,
 3;13,12,14;,
 3;15,14,16;,
 4;1,17,18,2;,
 4;2,18,19,4;,
 4;4,19,20,6;,
 4;6,20,21,8;,
 4;8,21,22,10;,
 4;10,22,23,12;,
 4;12,23,24,14;,
 4;14,24,25,16;,
 4;17,26,27,18;,
 4;18,27,28,19;,
 4;19,28,29,20;,
 4;20,29,30,21;,
 4;21,30,31,22;,
 4;22,31,32,23;,
 4;23,32,33,24;,
 4;24,33,34,25;,
 4;26,35,36,27;,
 4;27,36,37,28;,
 4;28,37,38,29;,
 4;29,38,39,30;,
 4;30,39,40,31;,
 4;31,40,41,32;,
 4;32,41,42,33;,
 4;33,42,43,34;,
 4;35,44,45,36;,
 4;36,45,46,37;,
 4;37,46,47,38;,
 4;38,47,48,39;,
 4;39,48,49,40;,
 4;40,49,50,41;,
 4;41,50,51,42;,
 4;42,51,52,43;,
 4;44,53,54,45;,
 4;45,54,55,46;,
 4;46,55,56,47;,
 4;47,56,57,48;,
 4;48,57,58,49;,
 4;49,58,59,50;,
 4;50,59,60,51;,
 4;51,60,61,52;,
 4;53,62,63,54;,
 4;54,63,64,55;,
 4;55,64,65,56;,
 4;56,65,66,57;,
 4;57,66,67,58;,
 4;58,67,68,59;,
 4;59,68,69,60;,
 4;60,69,70,61;,
 3;62,71,63;,
 3;63,72,64;,
 3;64,73,65;,
 3;65,74,66;,
 3;66,75,67;,
 3;67,76,68;,
 3;68,77,69;,
 3;69,78,70;;
 
 MeshMaterialList {
  1;
  64;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.056800;0.103200;0.047200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  58;
  0.000000;1.000000;-0.000090;,
  0.000001;0.950920;-0.309438;,
  -0.325450;0.924871;-0.196712;,
  -0.498584;0.866841;0.001203;,
  -0.339153;0.910155;0.237893;,
  0.000001;0.922370;0.386307;,
  0.339153;0.910155;0.237894;,
  0.498583;0.866841;0.001203;,
  0.325450;0.924871;-0.196713;,
  0.000001;0.810402;-0.585874;,
  -0.565492;0.737361;-0.369483;,
  -0.814030;0.580818;0.002319;,
  -0.613056;0.644894;0.456371;,
  0.000001;0.664443;0.747339;,
  0.613057;0.644893;0.456370;,
  0.814031;0.580817;0.002319;,
  0.565493;0.737360;-0.369482;,
  0.000001;0.482114;-0.876109;,
  -0.737348;0.412915;-0.534621;,
  -0.959402;0.282042;0.000959;,
  -0.751197;0.332828;0.570025;,
  0.000001;0.354464;0.935070;,
  0.751198;0.332827;0.570024;,
  0.959402;0.282040;0.000959;,
  0.737349;0.412913;-0.534620;,
  0.000001;-0.072400;-0.997376;,
  -0.796329;-0.063282;-0.601545;,
  -0.998937;-0.046103;-0.000000;,
  -0.796329;-0.063279;0.601545;,
  0.000001;-0.072396;0.997376;,
  0.796330;-0.063274;0.601544;,
  0.998937;-0.046094;-0.000000;,
  0.796329;-0.063276;-0.601544;,
  0.000001;-0.734336;-0.678786;,
  -0.581238;-0.700433;-0.414193;,
  -0.794309;-0.607514;-0.000000;,
  -0.581238;-0.700433;0.414193;,
  0.000001;-0.734336;0.678786;,
  0.581241;-0.700430;0.414194;,
  0.794315;-0.607507;-0.000000;,
  0.581242;-0.700429;-0.414194;,
  0.000000;-0.980156;-0.198227;,
  -0.199877;-0.971865;-0.124608;,
  -0.308750;-0.951143;-0.000000;,
  -0.199877;-0.971865;0.124608;,
  0.000000;-0.980156;0.198227;,
  0.199876;-0.971865;0.124608;,
  0.308749;-0.951144;-0.000000;,
  0.199877;-0.971865;-0.124608;,
  0.000000;-0.997332;-0.073002;,
  -0.076026;-0.996045;-0.045980;,
  -0.119071;-0.992886;-0.000000;,
  -0.076026;-0.996045;0.045980;,
  0.000000;-0.997332;0.073001;,
  0.076026;-0.996045;0.045979;,
  0.119071;-0.992886;-0.000000;,
  0.076026;-0.996045;-0.045980;,
  0.000000;-1.000000;-0.000000;;
  64;
  3;0,1,2;,
  3;0,2,3;,
  3;0,3,4;,
  3;0,4,5;,
  3;0,5,6;,
  3;0,6,7;,
  3;0,7,8;,
  3;0,8,1;,
  4;1,9,10,2;,
  4;2,10,11,3;,
  4;3,11,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;6,14,15,7;,
  4;7,15,16,8;,
  4;8,16,9,1;,
  4;9,17,18,10;,
  4;10,18,19,11;,
  4;11,19,20,12;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;15,23,24,16;,
  4;16,24,17,9;,
  4;17,25,26,18;,
  4;18,26,27,19;,
  4;19,27,28,20;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,32,24;,
  4;24,32,25,17;,
  4;25,33,34,26;,
  4;26,34,35,27;,
  4;27,35,36,28;,
  4;28,36,37,29;,
  4;29,37,38,30;,
  4;30,38,39,31;,
  4;31,39,40,32;,
  4;32,40,33,25;,
  4;33,41,42,34;,
  4;34,42,43,35;,
  4;35,43,44,36;,
  4;36,44,45,37;,
  4;37,45,46,38;,
  4;38,46,47,39;,
  4;39,47,48,40;,
  4;40,48,41,33;,
  4;41,49,50,42;,
  4;42,50,51,43;,
  4;43,51,52,44;,
  4;44,52,53,45;,
  4;45,53,54,46;,
  4;46,54,55,47;,
  4;47,55,56,48;,
  4;48,56,49,41;,
  3;49,57,50;,
  3;50,57,51;,
  3;51,57,52;,
  3;52,57,53;,
  3;53,57,54;,
  3;54,57,55;,
  3;55,57,56;,
  3;56,57,49;;
 }
 MeshTextureCoords {
  79;
  0.062500;0.000000;,
  0.000000;0.125000;,
  0.125000;0.125000;,
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
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.625000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.000000;0.875000;,
  0.125000;0.875000;,
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
  0.937500;1.000000;;
 }
}
