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
 63;
 -0.77013;-1.87182;-2.39195;,
 -2.04324;-0.93648;-1.92884;,
 -2.67340;-6.75469;-0.82594;,
 -1.37555;-6.81931;-1.22488;,
 -2.89945;-0.21842;-0.71644;,
 -3.51437;-6.57538;0.21857;,
 -2.64807;0.00954;0.78216;,
 -3.47695;-6.34955;1.50958;,
 -1.74876;-0.34120;1.99456;,
 -2.67573;-6.16373;2.55405;,
 -0.40616;-1.13607;2.45763;,
 -1.37845;-6.08879;2.95303;,
 0.86696;-2.07144;1.99456;,
 -0.08057;-6.15341;2.55405;,
 1.58426;-2.79002;0.78216;,
 0.72213;-6.33284;1.50958;,
 1.47181;-3.01741;-0.71644;,
 0.72303;-6.55855;0.21857;,
 0.57248;-2.66670;-1.92884;,
 -0.07823;-6.74436;-0.82594;,
 -0.77013;-1.87182;-2.39195;,
 -1.37555;-6.81931;-1.22488;,
 -0.70009;-0.79277;-1.10414;,
 -2.04324;-0.93648;-1.92884;,
 -1.24515;-0.36753;-0.68098;,
 -0.01792;-1.24047;-1.10099;,
 -0.05317;-0.38320;0.04595;,
 -1.45213;-0.11787;0.04045;,
 -1.21353;-0.15484;0.80624;,
 -0.59282;-0.48559;1.30060;,
 0.15770;-0.97903;1.29637;,
 0.71375;-1.42144;0.79962;,
 0.85939;-1.63608;0.04021;,
 0.57178;-1.56026;-0.67569;,
 -2.89945;-0.21842;-0.71644;,
 -2.64807;0.00954;0.78216;,
 -1.74876;-0.34120;1.99456;,
 -0.40616;-1.13607;2.45763;,
 0.86696;-2.07144;1.99456;,
 1.58426;-2.79002;0.78216;,
 1.47181;-3.01741;-0.71644;,
 0.57248;-2.66670;-1.92884;,
 -2.63939;-7.38446;-0.82594;,
 -1.34155;-7.44908;-1.22488;,
 -1.34301;-7.08383;0.86407;,
 -3.48039;-7.20517;0.21857;,
 -1.34301;-7.08383;0.86407;,
 -3.44296;-6.97933;1.50958;,
 -1.34301;-7.08383;0.86407;,
 -2.64175;-6.79353;2.55405;,
 -1.34301;-7.08383;0.86407;,
 -1.34448;-6.71857;2.95303;,
 -1.34301;-7.08383;0.86407;,
 -0.04661;-6.78319;2.55405;,
 -1.34301;-7.08383;0.86407;,
 0.75611;-6.96263;1.50958;,
 -1.34301;-7.08383;0.86407;,
 0.75700;-7.18835;0.21857;,
 -1.34301;-7.08383;0.86407;,
 -0.04425;-7.37415;-0.82594;,
 -1.34301;-7.08383;0.86407;,
 -1.34155;-7.44908;-1.22488;,
 -1.34301;-7.08383;0.86407;;
 
 60;
 4;3,2,1,0;,
 4;2,5,4,1;,
 4;5,7,6,4;,
 4;7,9,8,6;,
 4;9,11,10,8;,
 4;11,13,12,10;,
 4;13,15,14,12;,
 4;15,17,16,14;,
 4;17,19,18,16;,
 4;19,21,20,18;,
 3;0,23,22;,
 3;24,22,23;,
 3;0,22,25;,
 3;22,24,26;,
 3;24,27,26;,
 3;27,28,26;,
 3;28,29,26;,
 3;29,30,26;,
 3;30,31,26;,
 3;31,32,26;,
 3;32,33,26;,
 3;33,25,26;,
 3;25,22,26;,
 3;23,34,24;,
 3;27,24,34;,
 3;34,35,27;,
 3;28,27,35;,
 3;35,36,28;,
 3;29,28,36;,
 3;36,37,29;,
 3;30,29,37;,
 3;37,38,30;,
 3;31,30,38;,
 3;38,39,31;,
 3;32,31,39;,
 3;39,40,32;,
 3;33,32,40;,
 3;40,41,33;,
 3;25,33,41;,
 3;41,0,25;,
 4;2,3,43,42;,
 3;42,43,44;,
 4;5,2,42,45;,
 3;45,42,46;,
 4;7,5,45,47;,
 3;47,45,48;,
 4;9,7,47,49;,
 3;49,47,50;,
 4;11,9,49,51;,
 3;51,49,52;,
 4;13,11,51,53;,
 3;53,51,54;,
 4;15,13,53,55;,
 3;55,53,56;,
 4;17,15,55,57;,
 3;57,55,58;,
 4;19,17,57,59;,
 3;59,57,60;,
 4;21,19,59,61;,
 3;61,59,62;;
 
 MeshMaterialList {
  14;
  60;
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
  3,
  3,
  3,
  3;;
  Material {
   0.483200;0.483200;0.483200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game112\\Desktop\\�f��\\���f�����O\\bg_biceps_a_1.jpg";
   }
  }
  Material {
   0.552000;0.552000;0.552000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game112\\Desktop\\�f��\\���f�����O\\bg_biceps_a_2.jpg";
   }
  }
  Material {
   0.800000;0.664800;0.473600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.633600;0.360800;0.213600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.796800;0.169600;1.000000;;
   16.000000;
   0.300000;0.300000;0.300000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.031200;0.000000;1.000000;;
   19.000000;
   0.380000;0.380000;0.380000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.373600;0.373600;0.373600;1.000000;;
   5.000000;
   0.190000;0.190000;0.190000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.345000;0.086000;0.063000;1.000000;;
   24.000000;
   0.400000;0.400000;0.400000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.091200;0.091200;0.091200;1.000000;;
   24.000000;
   0.520000;0.520000;0.520000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Player\\Eye.png";
   }
  }
  Material {
   0.263200;0.084800;0.009600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  58;
  0.537028;0.832195;-0.138031;,
  -0.062561;-0.223549;-0.972683;,
  -0.603984;-0.091024;-0.791781;,
  -0.958562;0.054513;-0.279618;,
  -0.923028;0.155175;0.352051;,
  -0.555708;0.167758;0.814276;,
  0.026863;0.097281;0.994894;,
  0.747688;0.399817;0.530197;,
  0.953355;0.266970;0.140858;,
  0.926359;0.207585;-0.314274;,
  0.678194;0.240979;-0.694249;,
  -0.031822;-0.112367;-0.993157;,
  -0.584148;-0.060833;-0.809365;,
  -0.955200;0.001540;-0.295959;,
  -0.935665;0.052950;0.348895;,
  -0.564428;0.068794;0.822610;,
  0.013801;0.048634;0.998721;,
  0.583925;0.002997;0.811802;,
  0.947926;-0.057305;0.313294;,
  0.939929;-0.113130;-0.322079;,
  0.553362;-0.135640;-0.821822;,
  0.352169;0.759170;-0.547392;,
  0.179367;0.894988;-0.408441;,
  0.117893;0.982078;-0.147053;,
  0.203488;0.969199;0.138733;,
  0.402538;0.860633;0.311887;,
  0.629074;0.716341;0.301863;,
  0.792175;0.596902;0.127151;,
  0.831933;0.538468;-0.133937;,
  0.740856;0.553113;-0.381049;,
  0.560306;0.633561;-0.533533;,
  -0.000757;-0.000045;-1.000000;,
  -0.563272;-0.030407;-0.825712;,
  -0.948924;-0.051209;-0.311322;,
  -0.938347;-0.050628;0.341967;,
  -0.567538;-0.030620;0.822777;,
  0.000750;0.000039;1.000000;,
  0.572648;0.030891;0.819219;,
  0.947860;0.051134;0.314558;,
  0.943302;0.050888;-0.328012;,
  0.567523;0.030620;-0.822788;,
  0.003915;-0.985049;0.172230;,
  0.210868;0.815227;-0.539388;,
  0.436250;0.662600;-0.608808;,
  0.064754;0.949289;-0.307665;,
  0.078486;0.996697;0.020839;,
  0.248907;0.921449;0.298290;,
  0.505219;0.768434;0.392762;,
  0.003914;-0.985048;0.172237;,
  0.003902;-0.985049;0.172233;,
  0.003911;-0.985051;0.172217;,
  0.003918;-0.985052;0.172213;,
  0.003933;-0.985049;0.172231;,
  0.003925;-0.985047;0.172240;,
  0.003907;-0.985048;0.172238;,
  0.003911;-0.985049;0.172233;,
  0.003911;-0.985050;0.172226;,
  0.003920;-0.985049;0.172230;;
  60;
  4;11,12,2,1;,
  4;12,13,3,2;,
  4;13,14,4,3;,
  4;14,15,5,4;,
  4;15,16,6,5;,
  4;16,17,7,6;,
  4;17,18,8,7;,
  4;18,19,9,8;,
  4;19,20,10,9;,
  4;20,11,1,10;,
  3;43,42,21;,
  3;22,21,42;,
  3;43,21,30;,
  3;21,22,0;,
  3;22,23,0;,
  3;23,24,0;,
  3;24,25,0;,
  3;25,26,0;,
  3;26,27,0;,
  3;27,28,0;,
  3;28,29,0;,
  3;29,30,0;,
  3;30,21,0;,
  3;42,44,22;,
  3;23,22,44;,
  3;44,45,23;,
  3;24,23,45;,
  3;45,46,24;,
  3;25,24,46;,
  3;46,47,25;,
  3;26,25,47;,
  3;47,7,26;,
  3;27,26,7;,
  3;7,8,27;,
  3;28,27,8;,
  3;8,9,28;,
  3;29,28,9;,
  3;9,10,29;,
  3;30,29,10;,
  3;10,43,30;,
  4;12,11,31,32;,
  3;49,48,41;,
  4;13,12,32,33;,
  3;50,49,41;,
  4;14,13,33,34;,
  3;51,50,41;,
  4;15,14,34,35;,
  3;52,51,41;,
  4;16,15,35,36;,
  3;53,52,41;,
  4;17,16,36,37;,
  3;54,53,41;,
  4;18,17,37,38;,
  3;55,54,41;,
  4;19,18,38,39;,
  3;56,55,41;,
  4;20,19,39,40;,
  3;57,56,41;,
  4;11,20,40,31;,
  3;48,57,41;;
 }
 MeshTextureCoords {
  63;
  0.000000;0.000000;,
  0.100000;0.000000;,
  0.100000;1.000000;,
  0.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.300000;0.000000;,
  0.300000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.700000;0.000000;,
  0.700000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  0.900000;0.000000;,
  0.900000;1.000000;,
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
  0.100000;1.000000;,
  0.000000;1.000000;,
  0.050000;1.000000;,
  0.200000;1.000000;,
  0.150000;1.000000;,
  0.300000;1.000000;,
  0.250000;1.000000;,
  0.400000;1.000000;,
  0.350000;1.000000;,
  0.500000;1.000000;,
  0.450000;1.000000;,
  0.600000;1.000000;,
  0.550000;1.000000;,
  0.700000;1.000000;,
  0.650000;1.000000;,
  0.800000;1.000000;,
  0.750000;1.000000;,
  0.900000;1.000000;,
  0.850000;1.000000;,
  1.000000;1.000000;,
  0.950000;1.000000;;
 }
}
