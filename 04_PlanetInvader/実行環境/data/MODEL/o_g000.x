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
 72;
 -500.01001;-61.02830;-104.81258;,
 -500.01001;-16.47910;-84.62607;,
 -333.31000;-16.47910;-84.62607;,
 -333.31000;-61.02830;-104.81258;,
 -333.31000;-112.62110;-108.04812;,
 -500.01001;-112.62110;-108.04812;,
 -166.64999;-61.02830;-104.81258;,
 -166.64999;-112.62110;-108.04812;,
 -333.35001;-112.62110;-108.04812;,
 -333.35001;-61.02830;-104.81258;,
 -333.35001;-16.47910;-84.62607;,
 -166.64999;-16.47910;-84.62607;,
 0.01000;-61.02830;-104.81258;,
 0.01000;-112.62110;-108.04812;,
 -166.69000;-112.62110;-108.04812;,
 -166.69000;-61.02830;-104.81258;,
 -166.69000;-16.47910;-84.62607;,
 0.01000;-16.47910;-84.62607;,
 166.67000;-61.02830;-104.81258;,
 166.67000;-112.62110;-108.04812;,
 -0.03000;-112.62110;-108.04812;,
 -0.03000;-61.02830;-104.81258;,
 -0.03000;-16.47910;-84.62607;,
 166.67000;-16.47910;-84.62607;,
 166.63000;-61.02830;-104.81258;,
 166.63000;-16.47910;-84.62607;,
 333.32999;-16.47910;-84.62607;,
 333.32999;-61.02830;-104.81258;,
 333.32999;-112.62110;-108.04812;,
 166.63000;-112.62110;-108.04812;,
 499.98999;-61.02830;-104.81258;,
 499.98999;-112.62110;-108.04812;,
 333.29001;-112.62110;-108.04812;,
 333.29001;-61.02830;-104.81258;,
 333.29001;-16.47910;-84.62607;,
 499.98999;-16.47910;-84.62607;,
 -500.01001;-3.00000;-65.30197;,
 -500.01001;-3.00000;52.53249;,
 -333.31000;-3.00000;52.53249;,
 -333.31000;-3.00000;-65.30197;,
 -333.31000;-16.47910;-84.66607;,
 -500.01001;-16.47910;-84.66607;,
 -333.35001;-3.00000;-65.30197;,
 -333.35001;-3.00000;52.53249;,
 -166.64999;-3.00000;52.53249;,
 -166.64999;-3.00000;-65.30197;,
 -166.64999;-16.47910;-84.66607;,
 -333.35001;-16.47910;-84.66607;,
 -166.69000;-3.00000;-65.30197;,
 -166.69000;-3.00000;52.53249;,
 0.01000;-3.00000;52.53249;,
 0.01000;-3.00000;-65.30197;,
 0.01000;-16.47910;-84.66607;,
 -166.69000;-16.47910;-84.66607;,
 -0.03000;-3.00000;-65.30197;,
 -0.03000;-3.00000;52.53249;,
 166.67000;-3.00000;52.53249;,
 166.67000;-3.00000;-65.30197;,
 166.67000;-16.47910;-84.66607;,
 -0.03000;-16.47910;-84.66607;,
 166.63000;-3.00000;-65.30197;,
 166.63000;-3.00000;52.53249;,
 333.32999;-3.00000;52.53249;,
 333.32999;-3.00000;-65.30197;,
 333.32999;-16.47910;-84.66607;,
 166.63000;-16.47910;-84.66607;,
 333.29001;-3.00000;-65.30197;,
 333.29001;-3.00000;52.53249;,
 499.99011;-3.00000;52.53249;,
 499.99011;-3.00000;-65.30197;,
 499.99011;-16.47910;-84.66607;,
 333.29001;-16.47910;-84.66607;;
 
 24;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;6,7,8,9;,
 4;6,9,10,11;,
 4;12,13,14,15;,
 4;12,15,16,17;,
 4;18,19,20,21;,
 4;18,21,22,23;,
 4;24,25,26,27;,
 4;24,27,28,29;,
 4;30,31,32,33;,
 4;30,33,34,35;,
 4;36,37,38,39;,
 4;36,39,40,41;,
 4;42,43,44,45;,
 4;42,45,46,47;,
 4;48,49,50,51;,
 4;48,51,52,53;,
 4;54,55,56,57;,
 4;54,57,58,59;,
 4;60,61,62,63;,
 4;60,63,64,65;,
 4;66,67,68,69;,
 4;66,69,70,71;;
 
 MeshMaterialList {
  3;
  24;
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\field003.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\field002.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  36;
  0.000000;0.412733;-0.910852;,
  0.000000;0.062590;-0.998039;,
  0.000000;0.241627;-0.970369;,
  0.000000;0.412733;-0.910852;,
  0.000000;0.062590;-0.998039;,
  0.000000;0.241627;-0.970369;,
  0.000000;0.412733;-0.910852;,
  0.000000;0.062590;-0.998039;,
  0.000000;0.241627;-0.970369;,
  0.000000;0.412733;-0.910852;,
  0.000000;0.062590;-0.998039;,
  0.000000;0.241627;-0.970369;,
  0.000000;0.412733;-0.910852;,
  0.000000;0.062590;-0.998039;,
  0.000000;0.241627;-0.970369;,
  0.000000;0.412733;-0.910852;,
  0.000000;0.062590;-0.998039;,
  0.000000;0.241627;-0.970369;,
  0.000000;1.000000;0.000000;,
  0.000000;0.820738;-0.571305;,
  0.000000;0.954133;-0.299385;,
  0.000000;1.000000;0.000000;,
  0.000000;0.820738;-0.571305;,
  0.000000;0.954133;-0.299385;,
  0.000000;1.000000;0.000000;,
  0.000000;0.820738;-0.571305;,
  0.000000;0.954133;-0.299385;,
  0.000000;1.000000;0.000000;,
  0.000000;0.820738;-0.571305;,
  0.000000;0.954133;-0.299385;,
  0.000000;1.000000;0.000000;,
  0.000000;0.820738;-0.571305;,
  0.000000;0.954133;-0.299385;,
  0.000000;1.000000;0.000000;,
  0.000000;0.820738;-0.571305;,
  0.000000;0.954133;-0.299385;;
  24;
  4;2,0,0,2;,
  4;2,2,1,1;,
  4;5,4,4,5;,
  4;5,5,3,3;,
  4;8,7,7,8;,
  4;8,8,6,6;,
  4;11,10,10,11;,
  4;11,11,9,9;,
  4;14,12,12,14;,
  4;14,14,13,13;,
  4;17,16,16,17;,
  4;17,17,15,15;,
  4;20,18,18,20;,
  4;20,20,19,19;,
  4;23,21,21,23;,
  4;23,23,22,22;,
  4;26,24,24,26;,
  4;26,26,25,25;,
  4;29,27,27,29;,
  4;29,29,28,28;,
  4;32,30,30,32;,
  4;32,32,31,31;,
  4;35,33,33,35;,
  4;35,35,34,34;;
 }
 MeshTextureCoords {
  72;
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.706870;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.706870;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.706870;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.706870;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.706870;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.706870;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.706870;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.706870;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.706870;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.706870;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.706870;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.706870;,
  1.000000;1.000000;,
  0.000000;1.000000;;
 }
}
