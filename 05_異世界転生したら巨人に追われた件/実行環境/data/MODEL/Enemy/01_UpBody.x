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
 77;
 -18.45732;44.60019;-7.36236;,
 -24.86052;44.29181;0.21081;,
 -24.86052;33.38456;0.21081;,
 -20.38821;33.38456;-11.85807;,
 -16.57104;44.29181;14.72119;,
 -21.34746;33.38456;14.72119;,
 17.20572;44.29181;14.72119;,
 22.65495;44.29181;0.21081;,
 25.76418;33.38456;0.21081;,
 21.47400;33.38456;14.72119;,
 17.64123;44.52799;-7.93455;,
 19.18665;33.38456;-12.51849;,
 -24.07968;22.47709;0.21081;,
 -20.51625;22.47709;-11.42739;,
 -20.22108;22.47709;13.01386;,
 24.54829;22.47709;0.21081;,
 21.47400;22.47709;13.01386;,
 19.53729;22.47709;-9.52509;,
 -23.65359;11.91602;0.21081;,
 -18.62955;11.91602;-10.37115;,
 -19.54980;11.91602;15.09583;,
 24.02661;11.91602;0.21081;,
 20.22096;11.91602;15.09583;,
 17.69082;11.91602;-8.46882;,
 -20.67171;0.19909;0.20742;,
 -15.13590;0.04033;-11.25120;,
 -17.75436;-0.06927;17.29378;,
 20.33328;0.18808;0.12627;,
 15.69756;0.13537;16.44430;,
 15.27300;0.09317;-9.47067;,
 -0.02145;-0.02903;0.20742;,
 -15.13590;0.04033;-11.25120;,
 -20.67171;0.19909;0.20742;,
 -0.02145;-0.02903;0.20742;,
 -17.75436;-0.06927;17.29378;,
 -0.02145;-0.02903;0.20742;,
 15.69756;0.13537;16.44430;,
 20.33328;0.18808;0.12627;,
 -0.02145;-0.02903;0.20742;,
 15.27300;0.09317;-9.47067;,
 -14.37492;49.08671;-4.75080;,
 -20.35542;49.08671;0.21081;,
 -14.37492;49.08671;9.99579;,
 14.50146;49.08671;9.99579;,
 19.77060;49.08671;0.21081;,
 14.52927;49.01457;-4.49925;,
 -7.17912;49.70923;-5.64324;,
 -10.68201;49.70923;0.21081;,
 -7.09737;49.70923;6.06483;,
 6.35388;49.70923;6.06483;,
 9.83487;49.70923;0.21081;,
 6.48693;49.74112;-5.60820;,
 -6.24459;55.74229;-6.08217;,
 -8.57763;56.47622;0.44352;,
 0.06327;57.16050;0.21081;,
 -5.84892;56.94526;6.83289;,
 0.06327;57.16050;0.21081;,
 6.37116;56.30508;6.50379;,
 8.70417;55.95088;-0.02190;,
 0.06327;57.16050;0.21081;,
 5.97546;55.10220;-6.41127;,
 0.06327;57.16050;0.21081;,
 6.37116;56.30508;6.50379;,
 6.37116;56.30508;6.50379;,
 15.69756;0.13537;16.44430;,
 -0.64866;49.70923;-8.65362;,
 -1.21266;44.60019;-12.18573;,
 -2.49705;32.49797;-17.38982;,
 -0.40245;55.43628;-6.23952;,
 -1.36752;22.47709;-16.25078;,
 -3.22872;56.42007;-3.07281;,
 -6.24417;55.74185;-6.08058;,
 -1.26558;11.91602;-15.19451;,
 -0.59805;0.06559;-16.13279;,
 -0.59805;0.06559;-16.13279;,
 -15.13590;0.04033;-11.25120;,
 -0.64782;49.71031;-8.65296;;
 
 63;
 4;3,2,1,0;,
 4;2,5,4,1;,
 4;9,8,7,6;,
 4;8,11,10,7;,
 4;13,12,2,3;,
 4;12,14,5,2;,
 4;16,15,8,9;,
 4;15,17,11,8;,
 4;19,18,12,13;,
 4;18,20,14,12;,
 4;22,21,15,16;,
 4;21,23,17,15;,
 4;25,24,18,19;,
 4;24,26,20,18;,
 4;28,27,21,22;,
 4;27,29,23,21;,
 3;32,31,30;,
 3;34,32,33;,
 3;37,36,35;,
 3;39,37,38;,
 4;0,1,41,40;,
 4;1,4,42,41;,
 4;6,7,44,43;,
 4;7,10,45,44;,
 4;40,41,47,46;,
 4;41,42,48,47;,
 4;43,44,50,49;,
 4;44,45,51,50;,
 4;46,47,53,52;,
 3;52,53,54;,
 4;47,48,55,53;,
 3;53,55,56;,
 4;49,50,58,57;,
 3;57,58,59;,
 4;50,51,60,58;,
 3;58,60,61;,
 4;4,6,43,42;,
 4;4,5,9,6;,
 4;5,14,16,9;,
 4;14,20,22,16;,
 4;20,26,28,22;,
 4;55,48,49,62;,
 4;48,42,43,49;,
 3;54,55,63;,
 3;34,33,64;,
 4;66,0,40,65;,
 4;67,3,0,66;,
 4;10,11,67,66;,
 4;68,60,51,65;,
 4;46,52,68,65;,
 4;69,13,3,67;,
 4;11,17,69,67;,
 4;70,54,60,68;,
 4;52,71,70,68;,
 4;72,19,13,69;,
 4;17,23,72,69;,
 4;73,25,19,72;,
 4;23,29,73,72;,
 3;39,33,74;,
 3;33,75,74;,
 3;40,46,65;,
 4;51,45,65,76;,
 4;45,10,66,76;;
 
 MeshMaterialList {
  1;
  63;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.800000;0.700000;0.480000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  78;
  -0.501522;0.483831;-0.717205;,
  -0.873067;0.480635;-0.082123;,
  -0.477315;0.463852;0.746332;,
  0.503082;0.479660;0.718912;,
  0.869355;0.485827;-0.090524;,
  0.500444;0.497440;-0.708597;,
  -0.651905;0.120216;-0.748711;,
  -0.997125;0.052196;-0.054930;,
  -0.948753;0.040558;0.313405;,
  0.951300;0.116291;0.285490;,
  0.988807;0.109575;-0.101259;,
  0.613418;0.109171;-0.782176;,
  -0.674438;-0.100450;-0.731466;,
  -0.996618;-0.073188;-0.037358;,
  -0.961033;-0.063049;0.269148;,
  0.966194;-0.066552;0.249077;,
  0.987095;-0.094801;-0.129059;,
  0.621127;-0.135315;-0.771940;,
  -0.652691;-0.121143;-0.747876;,
  -0.986583;-0.144086;-0.076771;,
  -0.964068;-0.101189;0.245629;,
  0.951796;-0.183475;0.245808;,
  0.971314;-0.184357;-0.150207;,
  0.626034;-0.100656;-0.773272;,
  -0.970946;-0.212887;-0.109285;,
  0.633835;-0.073911;-0.769929;,
  -0.960842;-0.177926;0.212428;,
  0.951277;-0.270976;-0.147119;,
  0.001358;-0.999998;-0.001267;,
  -0.258600;0.864099;-0.431808;,
  -0.376525;0.925274;-0.045787;,
  -0.197991;0.913414;0.355633;,
  0.225906;0.909608;0.348684;,
  0.430852;0.900834;-0.053524;,
  0.303404;0.784842;-0.540341;,
  -0.096606;0.982795;-0.157418;,
  -0.071414;0.997354;-0.013594;,
  -0.032147;0.994662;0.098050;,
  0.030503;0.994948;0.095650;,
  0.070297;0.997390;-0.016506;,
  0.156718;0.907128;-0.390587;,
  0.034663;0.995110;-0.092492;,
  -0.610354;0.220812;-0.760730;,
  -0.977709;0.209541;0.013318;,
  -0.876327;0.179611;0.446980;,
  0.996105;0.087368;-0.011868;,
  0.607184;0.130404;-0.783787;,
  0.919001;-0.306975;0.247396;,
  0.902320;0.074307;0.424614;,
  -0.644734;-0.110362;-0.756399;,
  0.052107;0.998630;-0.004740;,
  -0.032489;0.588902;-0.807551;,
  -0.019589;0.513261;-0.858009;,
  -0.016773;0.142984;-0.989583;,
  -0.008756;0.181257;-0.983397;,
  0.014202;-0.127171;-0.991779;,
  0.048166;0.989816;-0.133955;,
  0.034574;-0.042178;-0.998512;,
  0.035557;0.041199;-0.998518;,
  0.211683;0.659920;-0.720899;,
  -0.005625;-0.999983;0.001367;,
  -0.011046;-0.999936;-0.002650;,
  -0.002412;-0.999985;-0.004858;,
  0.008443;-0.999962;0.001951;,
  0.010674;-0.999941;0.002028;,
  0.007457;-0.999972;-0.000842;,
  -0.081026;0.992664;-0.089739;,
  -0.014973;0.997232;-0.072834;,
  -0.012857;0.999696;0.021014;,
  0.135490;0.986191;-0.095231;,
  0.096593;0.994584;0.038374;,
  0.087522;0.970753;-0.223559;,
  -0.000000;-0.077556;0.996988;,
  0.000000;0.019685;0.999806;,
  0.005963;0.171181;0.985222;,
  0.011923;0.148855;0.988787;,
  0.011695;-0.086021;0.996225;,
  0.002013;-0.999981;-0.005862;;
  63;
  4;6,7,1,0;,
  4;7,8,2,1;,
  4;9,10,4,3;,
  4;10,11,5,4;,
  4;12,13,7,6;,
  4;13,14,8,7;,
  4;15,16,10,9;,
  4;16,17,11,10;,
  4;18,19,13,12;,
  4;19,20,14,13;,
  4;21,22,16,15;,
  4;22,23,17,16;,
  4;49,24,19,18;,
  4;24,26,20,19;,
  4;47,27,22,21;,
  4;27,25,23,22;,
  3;61,60,28;,
  3;62,61,28;,
  3;64,63,28;,
  3;65,64,28;,
  4;0,1,30,29;,
  4;1,2,31,30;,
  4;3,4,33,32;,
  4;4,5,34,33;,
  4;29,30,36,35;,
  4;30,31,37,36;,
  4;32,33,39,38;,
  4;33,34,40,39;,
  4;42,43,43,42;,
  3;67,66,41;,
  4;43,44,44,43;,
  3;66,68,41;,
  4;48,45,45,48;,
  3;70,69,41;,
  4;45,46,46,45;,
  3;69,71,41;,
  4;2,3,32,31;,
  4;2,72,72,3;,
  4;72,73,73,72;,
  4;73,74,74,73;,
  4;74,75,75,74;,
  4;76,76,76,76;,
  4;37,31,32,38;,
  3;41,68,70;,
  3;62,28,63;,
  4;52,0,29,51;,
  4;53,6,0,52;,
  4;5,11,53,52;,
  4;54,46,46,51;,
  4;42,42,54,51;,
  4;55,12,6,53;,
  4;11,17,55,53;,
  4;56,41,71,56;,
  4;67,50,56,56;,
  4;57,18,12,55;,
  4;17,23,57,55;,
  4;58,49,18,57;,
  4;23,25,58,57;,
  3;65,28,77;,
  3;28,60,77;,
  3;29,35,51;,
  4;40,34,51,59;,
  4;34,5,52,59;;
 }
 MeshTextureCoords {
  77;
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.200000;,
  0.125000;0.200000;,
  0.375000;0.000000;,
  0.375000;0.200000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;0.200000;,
  0.625000;0.200000;,
  0.875000;0.000000;,
  0.875000;0.200000;,
  0.250000;0.400000;,
  0.125000;0.400000;,
  0.375000;0.400000;,
  0.750000;0.400000;,
  0.625000;0.400000;,
  0.875000;0.400000;,
  0.250000;0.600000;,
  0.125000;0.600000;,
  0.375000;0.600000;,
  0.750000;0.600000;,
  0.625000;0.600000;,
  0.875000;0.600000;,
  0.250000;0.800000;,
  0.125000;0.800000;,
  0.375000;0.800000;,
  0.750000;0.800000;,
  0.625000;0.800000;,
  0.875000;0.800000;,
  0.187500;1.000000;,
  0.125000;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.687500;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.187500;0.000000;,
  0.375000;0.000000;,
  0.312500;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.687500;0.000000;,
  0.875000;0.000000;,
  0.812500;0.000000;,
  0.000000;0.000000;,
  0.312500;0.000000;,
  0.312500;0.500000;,
  0.483560;0.000000;,
  0.483560;0.000000;,
  0.483560;0.200000;,
  0.483560;0.000000;,
  0.483560;0.400000;,
  0.089640;0.000000;,
  0.000000;0.000000;,
  0.483560;0.600000;,
  0.483560;0.800000;,
  0.450940;0.739040;,
  0.062500;0.500000;,
  0.000000;0.000000;;
 }
}
