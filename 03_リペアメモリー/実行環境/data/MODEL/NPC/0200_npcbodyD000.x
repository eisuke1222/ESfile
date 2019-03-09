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
 22;
 0.00000;0.04379;-7.55639;,
 0.00000;10.43553;-5.03536;,
 3.42487;10.43553;-4.07241;,
 5.13730;0.04379;-6.11197;,
 5.54155;10.43553;-1.55137;,
 8.31232;0.04379;-2.33041;,
 5.54155;10.43553;1.56480;,
 8.31232;0.04379;2.34384;,
 3.42487;10.43553;4.08583;,
 5.13730;0.04379;6.12539;,
 0.00000;10.43553;5.04878;,
 0.00000;0.04379;7.56982;,
 -3.42487;10.43553;4.08583;,
 -5.13730;0.04379;6.12539;,
 -5.54155;10.43553;1.56480;,
 -8.31232;0.04379;2.34384;,
 -5.54155;10.43553;-1.55137;,
 -8.31232;0.04379;-2.33042;,
 -3.42486;10.43553;-4.07241;,
 -5.13730;0.04379;-6.11197;,
 0.00000;10.43553;-5.03536;,
 0.00000;0.04379;-7.55639;;
 
 15;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;6,4,16,14;,
 4;12,8,6,14;,
 4;16,4,2,18;,
 3;1,18,2;,
 3;12,10,8;;
 
 MeshMaterialList {
  6;
  15;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.664800;0.501600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.571200;0.787200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.019200;0.012800;0.442400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.796800;0.800000;0.260000;1.000000;;
   5.000000;
   0.080000;0.080000;0.080000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.040800;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  11;
  -0.000000;0.235761;-0.971811;,
  0.525081;0.246406;-0.814601;,
  0.905527;0.266210;-0.330383;,
  0.905527;0.266210;0.330384;,
  0.525080;0.246406;0.814601;,
  0.000000;0.235761;0.971811;,
  -0.525080;0.246406;0.814601;,
  -0.905527;0.266210;0.330384;,
  -0.905527;0.266211;-0.330384;,
  -0.525080;0.246407;-0.814601;,
  0.000000;1.000000;0.000000;;
  15;
  4;0,0,1,1;,
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,8,8;,
  4;8,8,9,9;,
  4;9,9,0,0;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  3;10,10,10;,
  3;10,10,10;;
 }
 MeshTextureCoords {
  22;
  0.000000;1.000000;,
  0.000000;0.666670;,
  0.100000;0.666670;,
  0.100000;1.000000;,
  0.200000;0.666670;,
  0.200000;1.000000;,
  0.300000;0.666670;,
  0.300000;1.000000;,
  0.400000;0.666670;,
  0.400000;1.000000;,
  0.500000;0.666670;,
  0.500000;1.000000;,
  0.600000;0.666670;,
  0.600000;1.000000;,
  0.700000;0.666670;,
  0.700000;1.000000;,
  0.800000;0.666670;,
  0.800000;1.000000;,
  0.900000;0.666670;,
  0.900000;1.000000;,
  1.000000;0.666670;,
  1.000000;1.000000;;
 }
}
