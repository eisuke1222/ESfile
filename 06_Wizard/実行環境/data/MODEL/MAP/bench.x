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
 43;
 0.00000;7.27899;0.00000;,
 2.83766;7.27899;-4.45423;,
 -1.14309;7.27899;-5.15614;,
 0.00000;7.27899;0.00000;,
 5.15614;7.27899;-1.14309;,
 0.00000;7.27899;0.00000;,
 4.45423;7.27899;2.83766;,
 0.00000;7.27899;0.00000;,
 1.14309;7.27899;5.15614;,
 0.00000;7.27899;0.00000;,
 -2.83766;7.27899;4.45423;,
 0.00000;7.27899;0.00000;,
 -5.15614;7.27899;1.14309;,
 0.00000;7.27899;0.00000;,
 -4.45423;7.27899;-2.83766;,
 0.00000;7.27899;0.00000;,
 -1.14309;7.27899;-5.15614;,
 0.00000;-0.44633;-0.00000;,
 0.00000;-0.44633;-6.87931;,
 4.86441;-0.44633;-4.86441;,
 0.00000;-0.44633;-0.00000;,
 6.87931;-0.44633;-0.00000;,
 0.00000;-0.44633;-0.00000;,
 4.86441;-0.44633;4.86441;,
 0.00000;-0.44633;-0.00000;,
 0.00000;-0.44633;6.87931;,
 0.00000;-0.44633;-0.00000;,
 -4.86441;-0.44633;4.86441;,
 0.00000;-0.44633;-0.00000;,
 -6.87931;-0.44633;0.00000;,
 0.00000;-0.44633;-0.00000;,
 -4.86441;-0.44633;-4.86441;,
 0.00000;-0.44633;-0.00000;,
 0.00000;-0.44633;-6.87931;,
 3.38966;3.41633;-4.58086;,
 -0.84231;3.41633;-5.63600;,
 5.63600;3.41633;-0.84231;,
 -0.84231;3.41633;-5.63600;,
 -4.58086;3.41633;-3.38966;,
 4.58086;3.41633;3.38966;,
 -5.63600;3.41633;0.84231;,
 0.84231;3.41633;5.63600;,
 -3.38966;3.41633;4.58086;;
 
 32;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,19;,
 3;20,19,21;,
 3;22,21,23;,
 3;24,23,25;,
 3;26,25,27;,
 3;28,27,29;,
 3;30,29,31;,
 3;32,31,33;,
 4;34,19,18,35;,
 4;34,35,2,1;,
 4;34,1,4,36;,
 4;34,36,21,19;,
 4;37,33,31,38;,
 4;37,38,14,16;,
 4;36,4,6,39;,
 4;36,39,23,21;,
 4;38,31,29,40;,
 4;38,40,12,14;,
 4;39,6,8,41;,
 4;39,41,25,23;,
 4;40,29,27,42;,
 4;40,42,10,12;,
 4;41,8,10,42;,
 4;41,42,27,25;;
 
 MeshMaterialList {
  1;
  32;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\MODEL\\MAP\\Wood.jpg";
   }
  }
 }
 MeshNormals {
  26;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.597392;0.205070;-0.775286;,
  -0.125790;0.205070;-0.970630;,
  0.970630;0.205070;-0.125790;,
  -0.775286;0.205070;-0.597392;,
  0.775286;0.205070;0.597392;,
  -0.970630;0.205070;0.125790;,
  0.125790;0.205070;0.970630;,
  -0.597392;0.205070;0.775286;,
  0.624113;0.298334;-0.722136;,
  -0.069313;0.298334;-0.951941;,
  -0.181036;0.108743;-0.977446;,
  0.563147;0.108743;-0.819171;,
  0.977446;0.108743;-0.181036;,
  0.951941;0.298334;-0.069313;,
  -0.722136;0.298334;-0.624113;,
  -0.819171;0.108743;-0.563147;,
  0.819171;0.108743;0.563147;,
  0.722136;0.298334;0.624113;,
  -0.951941;0.298334;0.069313;,
  -0.977446;0.108743;0.181036;,
  0.181036;0.108743;0.977446;,
  0.069313;0.298334;0.951941;,
  -0.624113;0.298334;0.722136;,
  -0.563147;0.108743;0.819171;;
  32;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;2,10,11,3;,
  4;2,3,12,13;,
  4;2,13,14,4;,
  4;2,4,15,10;,
  4;3,11,16,5;,
  4;3,5,17,12;,
  4;4,14,18,6;,
  4;4,6,19,15;,
  4;5,16,20,7;,
  4;5,7,21,17;,
  4;6,18,22,8;,
  4;6,8,23,19;,
  4;7,20,24,9;,
  4;7,9,25,21;,
  4;8,22,25,9;,
  4;8,9,24,23;;
 }
 MeshTextureCoords {
  43;
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  1.000000;0.500000;,
  0.875000;0.500000;,
  0.375000;0.500000;,
  0.750000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;;
 }
}
