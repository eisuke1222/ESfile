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
 180;
 -4.77299;16.76367;46.02449;,
 3.91081;16.76367;46.02449;,
 4.40701;16.76367;37.34069;,
 -4.52489;16.76367;37.58879;,
 4.41091;16.76367;29.53889;,
 -4.52129;16.76367;29.78699;,
 13.83302;16.76367;34.11569;,
 13.83662;16.76367;26.31389;,
 19.29361;16.76367;42.05459;,
 26.24071;16.76367;37.09229;,
 21.77462;16.76367;30.14549;,
 29.03192;16.76367;21.52349;,
 36.47522;16.76367;27.10589;,
 41.43751;16.76367;20.59319;,
 33.37382;16.76367;15.16559;,
 22.20902;16.76367;18.42209;,
 33.61802;16.76367;15.24869;,
 26.79511;16.76367;12.14729;,
 35.38981;16.76367;5.70659;,
 45.46921;16.76367;4.93109;,
 45.77941;16.76367;-2.97751;,
 35.70001;16.76367;-2.97751;,
 28.56661;16.76367;-2.20201;,
 33.14372;16.76367;-12.97111;,
 26.01061;16.76367;-12.19591;,
 40.50691;16.76367;-18.94951;,
 36.32011;16.76367;-25.46251;,
 29.65231;16.76367;-20.50021;,
 18.54932;16.76367;-21.70981;,
 21.27842;16.76367;-27.41611;,
 26.24071;16.76367;-37.09261;,
 19.04552;16.76367;-40.81411;,
 13.33891;16.76367;-31.88221;,
 11.10602;16.76367;-26.67181;,
 6.10951;16.76367;-34.20091;,
 3.87661;16.76367;-28.99051;,
 4.40701;16.76367;-46.02451;,
 -5.02109;16.76367;-45.52831;,
 -5.26919;16.76367;-34.61131;,
 -6.50999;16.76367;-28.16071;,
 -13.03439;16.76367;-32.14441;,
 -14.27488;16.76367;-25.69351;,
 -18.91529;16.76367;-41.31031;,
 -26.35859;16.76367;-35.85211;,
 -21.39659;16.76367;-28.65691;,
 -18.66718;16.76367;-21.95791;,
 -28.81199;16.76367;-20.82961;,
 -26.08289;16.76367;-14.13061;,
 -37.02748;16.76367;-26.17561;,
 -41.74139;16.76367;-19.72471;,
 -33.30569;16.76367;-14.76271;,
 -36.31379;16.76367;-3.49501;,
 -29.21939;16.76367;-2.80591;,
 -45.71129;16.76367;-2.60521;,
 -45.71129;16.76367;5.58239;,
 -36.28319;16.76367;5.33429;,
 -29.31689;16.76367;6.08039;,
 -34.79459;16.76367;15.50669;,
 -27.59938;16.76367;13.76999;,
 -41.49328;16.76367;20.46899;,
 -37.27559;16.76367;27.66419;,
 -29.58419;16.76367;22.95029;,
 -23.13329;16.76367;19.22849;,
 -21.86789;16.76367;29.69219;,
 -15.41698;16.76367;25.97039;,
 -26.35859;16.76367;38.08499;,
 -18.41909;16.76367;41.80649;,
 -14.94569;16.76367;34.61129;,
 -4.77299;0.50367;46.02449;,
 -4.52489;0.50367;37.58879;,
 4.40701;0.50367;37.34069;,
 3.91081;0.50367;46.02449;,
 -4.52129;0.50367;29.78699;,
 4.41091;0.50367;29.53889;,
 13.83662;0.50367;26.31389;,
 13.83302;0.50367;34.11569;,
 19.29361;0.50367;42.05459;,
 21.77462;0.50367;30.14549;,
 26.24071;0.50367;37.09229;,
 29.03192;0.50367;21.52349;,
 36.47522;0.50367;27.10589;,
 33.37382;0.50367;15.16559;,
 41.43751;0.50367;20.59319;,
 22.20902;0.50367;18.42209;,
 26.79511;0.50367;12.14729;,
 33.61802;0.50367;15.24869;,
 35.38981;0.50367;5.70659;,
 45.46921;0.50367;4.93109;,
 35.70001;0.50367;-2.97751;,
 45.77941;0.50367;-2.97751;,
 28.56661;0.50367;-2.20201;,
 26.01061;0.50367;-12.19591;,
 33.14372;0.50367;-12.97111;,
 40.50691;0.50367;-18.94951;,
 29.65231;0.50367;-20.50021;,
 36.32011;0.50367;-25.46251;,
 18.54932;0.50367;-21.70981;,
 21.27842;0.50367;-27.41611;,
 26.24071;0.50367;-37.09261;,
 13.33891;0.50367;-31.88221;,
 19.04552;0.50367;-40.81411;,
 11.10602;0.50367;-26.67181;,
 3.87661;0.50367;-28.99051;,
 6.10951;0.50367;-34.20091;,
 4.40701;0.50367;-46.02451;,
 -5.26919;0.50367;-34.61131;,
 -5.02109;0.50367;-45.52831;,
 -6.50999;0.50367;-28.16071;,
 -14.27488;0.50367;-25.69351;,
 -13.03439;0.50367;-32.14441;,
 -18.91529;0.50367;-41.31031;,
 -21.39659;0.50367;-28.65691;,
 -26.35859;0.50367;-35.85211;,
 -18.66718;0.50367;-21.95791;,
 -26.08289;0.50367;-14.13061;,
 -28.81199;0.50367;-20.82961;,
 -37.02748;0.50367;-26.17561;,
 -33.30569;0.50367;-14.76271;,
 -41.74139;0.50367;-19.72471;,
 -29.21939;0.50367;-2.80591;,
 -36.31379;0.50367;-3.49501;,
 -45.71129;0.50367;-2.60521;,
 -36.28319;0.50367;5.33429;,
 -45.71129;0.50367;5.58239;,
 -29.31689;0.50367;6.08039;,
 -34.79459;0.50367;15.50669;,
 -27.59938;0.50367;13.76999;,
 -41.49328;0.50367;20.46899;,
 -29.58419;0.50367;22.95029;,
 -37.27559;0.50367;27.66419;,
 -23.13329;0.50367;19.22849;,
 -15.41698;0.50367;25.97039;,
 -21.86789;0.50367;29.69219;,
 -26.35859;0.50367;38.08499;,
 -14.94569;0.50367;34.61129;,
 -18.41909;0.50367;41.80649;,
 -0.29669;16.76367;10.36409;,
 -0.29669;0.50367;10.36409;,
 -5.60639;0.50367;9.13049;,
 -5.60639;16.76367;9.13049;,
 -9.73109;0.50367;-0.04771;,
 -9.31408;0.50367;-4.65601;,
 -9.31408;16.76367;-4.65601;,
 -9.73109;16.76367;-0.04771;,
 3.81661;0.50367;9.70199;,
 8.16091;0.50367;5.20949;,
 8.16091;16.76367;5.20949;,
 3.81661;16.76367;9.70199;,
 8.85242;0.50607;-2.27701;,
 8.84432;16.76367;-2.28541;,
 5.23501;0.50367;-7.32751;,
 5.23501;16.76367;-7.32751;,
 2.10301;0.50367;-9.30571;,
 2.10301;16.76367;-9.30571;,
 -3.82559;16.76367;-8.46631;,
 -3.82559;0.50367;-8.46631;,
 -9.64229;16.76367;4.71089;,
 -9.64229;0.50367;4.71089;,
 -7.04519;0.50367;11.63429;,
 -7.04519;16.76367;11.63429;,
 -0.41369;0.50367;13.32899;,
 -0.41369;16.76367;13.32899;,
 4.94251;16.76367;12.01889;,
 4.94251;0.50367;12.01889;,
 10.36532;16.76397;6.19229;,
 10.36532;0.50397;6.19229;,
 11.03942;16.76427;-2.79721;,
 11.04751;0.50667;-2.78851;,
 6.61442;16.76367;-8.74621;,
 6.61442;0.50367;-8.74621;,
 2.45881;16.76367;-11.09011;,
 2.45881;0.50367;-11.09011;,
 -4.58579;0.50367;-10.35601;,
 -4.58579;16.76367;-10.35601;,
 -11.32289;0.50367;-5.67361;,
 -11.32289;16.76367;-5.67361;,
 -12.12569;0.50367;5.73029;,
 -12.12569;16.76367;5.73029;,
 -12.51628;16.76367;-0.14761;,
 -12.51628;0.50367;-0.14761;;
 
 262;
 4;0,1,2,3;,
 4;2,4,5,3;,
 4;2,6,7,4;,
 4;8,9,10,6;,
 3;7,6,10;,
 3;10,11,7;,
 4;12,13,14,11;,
 3;7,11,15;,
 4;11,16,17,15;,
 3;17,16,18;,
 4;19,20,21,18;,
 3;17,18,21;,
 3;21,22,17;,
 4;21,23,24,22;,
 4;25,26,27,23;,
 3;24,23,27;,
 3;28,24,27;,
 3;28,27,29;,
 4;30,31,32,29;,
 3;29,32,28;,
 3;33,28,32;,
 4;32,34,35,33;,
 4;36,37,38,34;,
 3;34,38,35;,
 3;39,35,38;,
 4;38,40,41,39;,
 4;42,43,44,40;,
 3;41,40,44;,
 3;45,41,44;,
 4;44,46,47,45;,
 4;48,49,50,46;,
 3;47,46,50;,
 4;50,51,52,47;,
 4;53,54,55,51;,
 4;55,56,52,51;,
 3;57,56,55;,
 3;56,57,58;,
 4;59,60,61,57;,
 3;58,57,61;,
 3;62,58,61;,
 4;61,63,64,62;,
 4;65,66,67,63;,
 3;63,67,64;,
 3;5,64,67;,
 3;5,67,3;,
 4;68,69,70,71;,
 4;70,69,72,73;,
 4;70,73,74,75;,
 4;76,75,77,78;,
 3;74,77,75;,
 3;77,74,79;,
 4;80,79,81,82;,
 3;74,83,79;,
 4;79,83,84,85;,
 3;84,86,85;,
 4;87,86,88,89;,
 3;84,88,86;,
 3;88,84,90;,
 4;88,90,91,92;,
 4;93,92,94,95;,
 3;91,94,92;,
 3;96,94,91;,
 3;96,97,94;,
 4;98,97,99,100;,
 3;97,96,99;,
 3;101,99,96;,
 4;99,101,102,103;,
 4;104,103,105,106;,
 3;103,102,105;,
 3;107,105,102;,
 4;105,107,108,109;,
 4;110,109,111,112;,
 3;108,111,109;,
 3;113,111,108;,
 4;111,113,114,115;,
 4;116,115,117,118;,
 3;114,117,115;,
 4;117,114,119,120;,
 4;121,120,122,123;,
 4;122,120,119,124;,
 3;125,122,124;,
 3;124,126,125;,
 4;127,125,128,129;,
 3;126,128,125;,
 3;130,128,126;,
 4;128,130,131,132;,
 4;133,132,134,135;,
 3;132,131,134;,
 3;72,134,131;,
 3;72,69,134;,
 4;0,68,71,1;,
 4;76,78,9,8;,
 4;80,82,13,12;,
 4;87,89,20,19;,
 4;93,95,26,25;,
 4;98,100,31,30;,
 4;104,106,37,36;,
 4;110,112,43,42;,
 4;116,118,49,48;,
 4;53,121,123,54;,
 4;59,127,129,60;,
 4;65,133,135,66;,
 4;70,75,6,2;,
 4;1,71,70,2;,
 4;76,8,6,75;,
 4;77,79,11,10;,
 4;9,78,77,10;,
 4;85,86,18,16;,
 4;13,82,85,16;,
 4;86,87,19,18;,
 4;21,88,92,23;,
 4;20,89,88,21;,
 4;23,92,93,25;,
 4;94,97,29,27;,
 4;26,95,94,27;,
 4;97,98,30,29;,
 4;103,104,36,34;,
 4;31,100,99,32;,
 4;99,103,34,32;,
 4;37,106,105,38;,
 4;105,109,40,38;,
 4;109,110,42,40;,
 4;44,43,112,111;,
 4;50,49,118,117;,
 4;51,50,117,120;,
 4;121,53,51,120;,
 4;115,46,44,111;,
 4;116,48,46,115;,
 4;55,54,123,122;,
 4;57,55,122,125;,
 4;59,57,125,127;,
 4;61,60,129,128;,
 4;63,61,128,132;,
 4;133,65,63,132;,
 4;67,134,69,3;,
 4;66,135,134,67;,
 4;68,0,3,69;,
 4;80,12,11,79;,
 4;136,137,138,139;,
 4;140,141,142,143;,
 4;144,145,146,147;,
 4;145,148,149,146;,
 4;148,150,151,149;,
 4;151,150,152,153;,
 4;154,153,152,155;,
 4;142,154,155,141;,
 4;147,136,137,144;,
 4;139,156,157,138;,
 4;157,156,143,140;,
 4;33,35,102,101;,
 4;39,107,102,35;,
 4;108,107,39,41;,
 4;108,41,45,113;,
 4;28,33,101,96;,
 4;28,96,91,24;,
 4;90,22,24,91;,
 4;84,17,22,90;,
 4;114,113,45,47;,
 4;114,47,52,119;,
 4;119,52,56,124;,
 4;126,124,56,58;,
 4;130,126,58,62;,
 4;62,64,131,130;,
 4;15,17,84,83;,
 4;74,7,15,83;,
 4;73,4,7,74;,
 4;72,5,4,73;,
 4;131,64,5,72;,
 4;158,159,139,138;,
 4;160,158,138,137;,
 4;161,160,137,136;,
 4;159,161,136,139;,
 4;159,158,160,161;,
 4;162,161,136,147;,
 4;163,162,147,144;,
 4;160,163,144,137;,
 4;161,160,137,136;,
 4;161,162,163,160;,
 4;164,162,147,146;,
 4;165,164,146,145;,
 4;163,165,145,144;,
 4;162,163,144,147;,
 4;162,164,165,163;,
 4;164,165,145,146;,
 4;166,164,146,149;,
 4;167,166,149,148;,
 4;165,167,148,145;,
 4;165,164,166,167;,
 4;166,167,148,149;,
 4;168,166,149,151;,
 4;169,168,151,150;,
 4;167,169,150,148;,
 4;167,166,168,169;,
 4;168,169,150,151;,
 4;170,168,151,153;,
 4;171,170,153,152;,
 4;169,171,152,150;,
 4;169,168,170,171;,
 4;171,172,155,152;,
 4;170,171,152,153;,
 4;173,170,153,154;,
 4;172,173,154,155;,
 4;172,171,170,173;,
 4;172,174,141,155;,
 4;173,172,155,154;,
 4;175,173,154,142;,
 4;174,175,142,141;,
 4;174,172,173,175;,
 4;158,159,139,138;,
 4;176,158,138,157;,
 4;177,176,157,156;,
 4;159,177,156,139;,
 4;159,158,176,177;,
 4;177,178,143,156;,
 4;176,177,156,157;,
 4;179,176,157,140;,
 4;178,179,140,143;,
 4;178,177,176,179;,
 4;175,178,143,142;,
 4;174,175,142,141;,
 4;179,174,141,140;,
 4;178,179,140,143;,
 4;178,175,174,179;,
 4;160,158,138,137;,
 4;160,163,162,161;,
 4;163,165,164,162;,
 4;165,167,166,164;,
 4;167,169,168,166;,
 4;168,169,171,170;,
 4;171,172,173,170;,
 4;172,174,175,173;,
 4;175,174,179,178;,
 4;159,177,176,158;,
 4;177,178,179,176;,
 4;160,137,144,163;,
 4;163,144,145,165;,
 4;148,167,165,145;,
 4;150,169,167,148;,
 4;169,150,152,171;,
 4;172,171,152,155;,
 4;172,155,141,174;,
 4;141,140,179,174;,
 4;179,140,157,176;,
 4;157,138,158,176;,
 4;162,147,136,161;,
 4;164,146,147,162;,
 4;166,149,146,164;,
 4;166,168,151,149;,
 4;168,170,153,151;,
 4;170,173,154,153;,
 4;154,173,175,142;,
 4;177,156,143,178;,
 4;177,159,139,156;,
 4;155,154,142,141;,
 4;155,152,153,154;,
 4;153,152,150,151;,
 4;151,150,148,149;,
 4;146,149,148,145;,
 4;146,145,144,147;,
 4;136,147,144,137;,
 4;138,157,156,139;,
 4;157,140,143,156;;
 
 MeshMaterialList {
  2;
  262;
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
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.310400;0.310400;0.310400;1.000000;;
   6.000000;
   0.700000;0.700000;0.700000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  147;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.034335;0.000000;-0.999410;,
  0.462561;0.000000;0.886587;,
  0.908935;0.000144;0.416938;,
  0.965107;0.000173;-0.261857;,
  0.686280;0.000027;-0.727337;,
  0.209774;0.000000;-0.977750;,
  -0.365107;0.000000;-0.930966;,
  0.864271;0.000000;0.503027;,
  0.999359;0.000000;0.035785;,
  -0.928904;0.000000;0.370321;,
  0.505113;0.000000;-0.863053;,
  0.999222;0.000000;0.039431;,
  -0.867039;0.000000;-0.498241;,
  0.000033;-1.000000;0.000019;,
  -0.899424;0.000000;0.437076;,
  0.000139;-1.000000;0.000019;,
  -0.407199;0.000000;0.913339;,
  0.000169;-1.000000;-0.000021;,
  0.227002;0.000625;0.973894;,
  0.000063;-1.000000;-0.000022;,
  0.716967;0.000000;0.697107;,
  0.980695;0.000000;0.195545;,
  0.379738;0.000000;0.925094;,
  0.000000;-0.000000;1.000000;,
  0.581245;0.000000;0.813729;,
  0.795418;0.000000;0.606061;,
  0.999232;0.000000;0.039193;,
  0.841186;0.000000;-0.540746;,
  0.459408;0.000000;-0.888225;,
  -0.052557;0.000000;-0.998618;,
  -0.591348;0.000000;-0.806416;,
  -0.807404;0.000000;-0.589999;,
  -1.000000;0.000000;0.000000;,
  -0.862707;0.000000;0.505704;,
  -0.424421;0.000000;0.905465;,
  0.323716;0.000000;0.946154;,
  0.998371;0.000000;0.057048;,
  -0.823916;0.000000;0.566712;,
  0.765057;0.000000;0.643963;,
  0.841162;0.000000;-0.540783;,
  0.983194;0.000000;0.182562;,
  0.564275;0.000000;-0.825587;,
  0.076712;0.000000;0.997053;,
  0.968808;0.000000;-0.247814;,
  0.000000;0.000000;-1.000000;,
  0.630326;0.000000;0.776331;,
  0.636790;0.000000;-0.771037;,
  -0.597027;0.000000;-0.802221;,
  0.889818;0.000000;0.456316;,
  0.989792;0.000000;-0.142522;,
  -0.842692;0.000000;-0.538397;,
  0.305408;0.000000;-0.952222;,
  -0.999742;0.000000;-0.022720;,
  -0.302775;0.000000;-0.953062;,
  0.841657;0.000000;-0.540012;,
  -0.823224;0.000000;0.567717;,
  -0.507007;0.000000;0.861942;,
  -0.966163;0.000000;-0.257933;,
  -0.094263;0.000000;-0.995547;,
  -0.725949;0.000000;-0.687748;,
  0.545413;0.000000;-0.838167;,
  0.026306;0.000000;0.999654;,
  -0.989462;0.000000;0.144795;,
  -0.595251;0.000000;-0.803540;,
  0.522547;0.000000;0.852610;,
  -0.657959;0.000000;0.753053;,
  -0.881718;0.000000;-0.471777;,
  -0.274732;0.000000;0.961521;,
  0.900559;0.000000;0.434734;,
  -0.999568;0.000000;-0.029398;,
  -0.599995;0.000000;0.800004;,
  -0.570279;0.000000;-0.821451;,
  0.158920;0.000000;0.987291;,
  -0.738435;0.000000;0.674325;,
  -0.999826;0.000000;0.018658;,
  -0.434223;0.000000;0.900806;,
  -0.115067;0.000000;0.993358;,
  0.192477;0.000000;0.981301;,
  0.484869;0.000000;0.874587;,
  0.687896;0.000000;0.725810;,
  -0.679336;0.000000;0.733827;,
  -0.897060;0.000000;0.441910;,
  -0.997967;0.000000;0.063734;,
  -0.929776;0.000000;-0.368125;,
  0.870640;0.000000;0.491922;,
  0.990135;0.000000;0.140117;,
  0.994557;0.000000;-0.104198;,
  0.899251;0.000000;-0.437432;,
  0.718458;0.000000;-0.695570;,
  0.503390;0.000000;-0.864060;,
  -0.749795;0.000000;-0.661670;,
  -0.516502;0.000000;-0.856286;,
  -0.177767;0.000000;-0.984073;,
  0.153965;0.000000;-0.988076;,
  -0.527124;0.000000;0.849788;,
  -0.005156;0.000000;0.999987;,
  -0.000033;1.000000;-0.000019;,
  -0.237591;0.000000;-0.971365;,
  -0.506035;0.000000;-0.862513;,
  -0.000152;1.000000;0.000127;,
  0.899424;0.000000;-0.437076;,
  -0.916151;-0.000142;-0.400832;,
  0.407199;0.000000;-0.913339;,
  -0.000274;1.000000;0.000014;,
  -0.960460;-0.000165;0.278417;,
  -0.227002;-0.000625;-0.973894;,
  -0.000156;1.000000;-0.000131;,
  -0.661160;-0.000021;0.750245;,
  -0.716967;0.000000;-0.697107;,
  -0.203431;0.000000;0.979089;,
  -0.980695;0.000000;-0.195545;,
  0.927743;0.000000;-0.373218;,
  0.348156;0.000000;0.937437;,
  -0.927743;0.000000;0.373218;,
  0.451897;0.000000;-0.892070;,
  0.570713;0.000000;0.821150;,
  0.757991;0.000000;-0.652265;,
  0.925493;0.000000;-0.378765;,
  -0.379738;0.000000;-0.925094;,
  -0.035845;0.000000;0.999357;,
  0.997799;0.000000;-0.066306;,
  -0.992815;0.000000;-0.119661;,
  -0.897179;0.000000;-0.441668;,
  0.506035;0.000000;0.862513;,
  0.916151;0.000142;0.400832;,
  0.960460;0.000165;-0.278417;,
  0.661160;0.000021;-0.750245;,
  0.203431;0.000000;-0.979089;,
  -0.348156;0.000000;-0.937437;,
  -0.925493;0.000000;0.378765;,
  0.000033;-1.000000;0.000019;,
  0.000152;-1.000000;-0.000127;,
  0.000274;-1.000000;-0.000014;,
  0.000156;-1.000000;0.000131;,
  -0.000033;1.000000;-0.000019;,
  -0.000139;1.000000;-0.000019;,
  -0.000169;1.000000;0.000021;,
  -0.000063;1.000000;0.000022;,
  0.365107;0.000000;0.930966;,
  -0.209774;0.000000;0.977750;,
  -0.686280;-0.000027;0.727337;,
  -0.965107;-0.000173;0.261857;,
  -0.908935;-0.000144;-0.416938;,
  -0.462561;0.000000;-0.886587;,
  0.928904;0.000000;-0.370321;;
  262;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;25,25,25,25;,
  4;26,26,26,26;,
  4;27,27,27,27;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;30,30,30,30;,
  4;31,31,31,31;,
  4;32,32,32,32;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;61,61,61,61;,
  4;62,62,62,62;,
  4;63,63,63,63;,
  4;64,64,64,64;,
  4;65,65,65,65;,
  4;66,66,66,66;,
  4;67,67,67,67;,
  4;68,68,68,68;,
  4;69,69,69,69;,
  4;70,70,70,70;,
  4;71,71,71,71;,
  4;72,72,72,72;,
  4;2,2,12,12;,
  4;10,9,9,10;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,6,7,7;,
  4;8,7,7,8;,
  4;73,8,8,73;,
  4;3,74,74,3;,
  4;75,11,11,75;,
  4;11,11,76,76;,
  4;77,78,78,77;,
  4;79,79,78,78;,
  4;80,79,79,80;,
  4;80,80,81,81;,
  4;82,77,77,82;,
  4;82,82,83,83;,
  4;84,84,83,83;,
  4;85,85,84,84;,
  4;86,81,81,86;,
  4;86,86,87,87;,
  4;87,87,88,88;,
  4;89,88,88,89;,
  4;90,89,89,90;,
  4;90,91,91,90;,
  4;92,85,85,92;,
  4;93,93,92,92;,
  4;94,94,93,93;,
  4;95,95,94,94;,
  4;91,91,95,95;,
  4;14,14,14,14;,
  4;1,1,1,1;,
  4;13,13,13,13;,
  4;0,0,0,0;,
  4;96,96,97,97;,
  4;15,1,1,15;,
  4;16,16,16,16;,
  4;0,98,98,0;,
  4;13,13,13,13;,
  4;99,100,100,99;,
  4;17,15,15,17;,
  4;18,18,18,18;,
  4;98,101,101,98;,
  4;102,102,102,102;,
  4;100,103,103,100;,
  4;104,104,104,104;,
  4;19,17,17,19;,
  4;20,20,20,20;,
  4;101,105,105,101;,
  4;103,103,106,106;,
  4;107,107,107,107;,
  4;21,19,19,21;,
  4;22,22,22,22;,
  4;105,108,108,105;,
  4;106,106,109,109;,
  4;110,110,110,110;,
  4;1,21,21,1;,
  4;23,23,23,23;,
  4;108,0,0,108;,
  4;109,109,111,111;,
  4;0,0,0,0;,
  4;112,112,112,112;,
  4;1,1,1,1;,
  4;113,113,113,113;,
  4;114,111,111,114;,
  4;0,0,0,0;,
  4;115,115,115,115;,
  4;1,1,1,1;,
  4;116,116,116,116;,
  4;117,114,114,117;,
  4;14,14,14,14;,
  4;0,0,0,0;,
  4;24,24,24,24;,
  4;1,1,1,1;,
  4;118,118,119,119;,
  4;1,1,1,1;,
  4;120,120,120,120;,
  4;0,0,0,0;,
  4;121,121,121,121;,
  4;122,119,119,122;,
  4;0,0,0,0;,
  4;116,116,116,116;,
  4;1,1,1,1;,
  4;121,121,121,121;,
  4;123,124,124,123;,
  4;1,1,1,1;,
  4;97,125,125,97;,
  4;125,126,126,125;,
  4;126,127,127,126;,
  4;127,128,128,127;,
  4;128,128,129,129;,
  4;129,130,130,129;,
  4;130,124,124,130;,
  4;124,124,123,123;,
  4;96,131,131,96;,
  4;131,123,123,131;,
  4;1,1,132,132;,
  4;132,132,133,133;,
  4;134,134,133,133;,
  4;135,135,134,134;,
  4;135,135,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;136,136,0,0;,
  4;137,137,136,136;,
  4;138,138,137,137;,
  4;138,139,139,138;,
  4;139,0,0,139;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;140,140,9,9;,
  4;140,141,141,140;,
  4;141,141,142,142;,
  4;142,142,143,143;,
  4;144,143,143,144;,
  4;144,144,145,145;,
  4;2,145,145,2;,
  4;12,146,146,12;,
  4;146,10,10,146;;
 }
 MeshTextureCoords {
  180;
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
  0.000000;0.000000;;
 }
}
