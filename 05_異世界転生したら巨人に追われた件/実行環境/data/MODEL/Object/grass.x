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
 27;
 -53.99707;-0.12216;5.97564;,
 -40.75769;48.47867;0.01951;,
 -28.61246;-0.04838;-6.90417;,
 -49.23163;0.44091;0.45505;,
 -32.73160;48.45592;-0.20848;,
 -19.91911;-0.58188;-1.98008;,
 -36.77295;-0.12216;-3.89809;,
 -22.37257;48.47867;-0.47968;,
 -8.00609;-0.04838;1.58859;,
 22.05736;-0.12216;-6.16587;,
 8.87891;48.47867;-0.11051;,
 -3.19567;-0.04838;6.90417;,
 17.34805;0.44091;-0.60968;,
 0.85545;48.45592;0.17778;,
 -11.93860;-0.58188;2.04559;,
 4.93403;-0.12216;3.83688;,
 -9.50042;48.47867;0.52681;,
 -23.88739;-0.04838;-1.43342;,
 53.99707;-0.12216;-6.16587;,
 40.81869;48.47867;-0.11051;,
 28.74410;-0.04838;6.90417;,
 49.28781;0.44091;-0.60968;,
 32.79518;48.45592;0.17778;,
 20.00113;-0.58188;2.04559;,
 36.87377;-0.12216;3.83688;,
 22.43930;48.47867;0.52681;,
 8.05235;-0.04838;-1.43342;;
 
 18;
 3;0,1,2;,
 3;1,0,2;,
 3;3,4,5;,
 3;4,3,5;,
 3;6,7,8;,
 3;7,6,8;,
 3;9,10,11;,
 3;10,9,11;,
 3;12,13,14;,
 3;13,12,14;,
 3;15,16,17;,
 3;16,15,17;,
 3;18,19,20;,
 3;19,18,20;,
 3;21,22,23;,
 3;22,21,23;,
 3;24,25,26;,
 3;25,24,26;;
 
 MeshMaterialList {
  1;
  18;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.172800;0.668000;0.138400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Object\\grass.jpg";
   }
  }
 }
 MeshNormals {
  18;
  -0.452464;0.013980;-0.891673;,
  -0.082278;0.014503;-0.996504;,
  0.187301;0.013588;-0.982209;,
  0.459636;0.013995;0.887997;,
  0.089782;0.014507;0.995856;,
  -0.179828;0.013582;0.983604;,
  0.459637;0.013994;0.887997;,
  0.089782;0.014507;0.995856;,
  -0.179828;0.013582;0.983604;,
  0.452464;-0.013980;0.891673;,
  0.082278;-0.014503;0.996504;,
  -0.187301;-0.013588;0.982209;,
  -0.459636;-0.013995;-0.887997;,
  -0.089782;-0.014507;-0.995856;,
  0.179828;-0.013582;-0.983604;,
  -0.459637;-0.013994;-0.887997;,
  -0.089782;-0.014507;-0.995856;,
  0.179828;-0.013582;-0.983604;;
  18;
  3;0,0,0;,
  3;9,9,9;,
  3;1,1,1;,
  3;10,10,10;,
  3;2,2,2;,
  3;11,11,11;,
  3;3,3,3;,
  3;12,12,12;,
  3;4,4,4;,
  3;13,13,13;,
  3;5,5,5;,
  3;14,14,14;,
  3;6,6,6;,
  3;15,15,15;,
  3;7,7,7;,
  3;16,16,16;,
  3;8,8,8;,
  3;17,17,17;;
 }
 MeshTextureCoords {
  27;
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;;
 }
}
