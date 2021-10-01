#include "skm.cpp"

#include <conio.h>

typedef char PAD12[12];


typedef char BoneDef[100];
typedef char MatDef[128];
typedef unsigned long BitOS;



typedef struct SKMDATA_ALL
{
	SKMHeader_T Head[4];
	PAD12 pad;
	BoneDef  bones;
	MatDef  Mater;
	XYZ_ * VERT;
	XYZ_ * Normal;
	Texture_ * Texel;
	XYZi_ * index;
}SKMDATA_ALL;

ConvertVTX2SKM(char * fname)
{
PAD12 ZeroP = {0,0,0,0,0,0,0,0,0,0,0x0FF,0x0FF};

fstream file8;
SKMData skmobj;
SKMDATA_ALL ALLDATA;

file8.open(fname, ios::in | ios::out | ios::ate );
file8.seekg(0,ios::beg);
char parse_[255];
while(strcmp(parse_,"points") != 0) file8>>parse_;
file8.getline(parse_, 80,'\n');

XYZ_ tempxyz_[3];
Texture_ TempTexel;

int pos_ = 0;
skmobj.HData[0].count = 1;
skmobj.HData[1].count = 1;
skmobj.HData[2].count = 0;
skmobj.HData[3].count = 0;


//GET VERT DATA//////////////////

file8>>parse_[0];
while(parse_[0] == 'v'){   
skmobj.HData[2].count++;
file8>>tempxyz_[0].x>>tempxyz_[0].y>>tempxyz_[0].z; tempxyz_[0].w = 0;
skmobj.Vertdata_.enQueue(tempxyz_[0]);
file8>>parse_[0]; 
pos_ = file8.tellg();
}


skmobj.Vertdata_.iEnd();
while(skmobj.Vertdata_.HasPrev() ){

tempxyz_[0] = skmobj.Vertdata_.ShowData(skmobj.Vertdata_.gItorator());
printf("\n%f %f %f",tempxyz_[0].x,tempxyz_[0].y,tempxyz_[0].z);
skmobj.Vertdata_.gPrev();
};

tempxyz_[0] = skmobj.Vertdata_.ShowData(skmobj.Vertdata_.gItorator());
printf("\n%f %f %f",tempxyz_[0].x,tempxyz_[0].y,tempxyz_[0].z);

printf("count:%d",skmobj.HData[2].count);
system("pause");


//GET NORMAL DATA//////////////////
 
while(strcmp(parse_,"normals") != 0) file8>>parse_;
while(strcmp(parse_ , "vn") != 0) file8>>parse_;

while(strcmp(parse_ , "vn") == 0){   skmobj.HData[3].count++;
file8>>tempxyz_[0].x>>tempxyz_[0].y>>tempxyz_[0].z; tempxyz_[0].w = 0;
skmobj.Normdata_.enQueue(tempxyz_[0]);
file8>>parse_[0]; file8>>parse_[1]; 
pos_ = file8.tellg();
}


skmobj.Normdata_.iEnd();
while(skmobj.Normdata_.HasPrev() ){

tempxyz_[0] = skmobj.Normdata_.ShowData(skmobj.Normdata_.gItorator());
printf("\n%f %f %f",tempxyz_[0].x,tempxyz_[0].y,tempxyz_[0].z);
skmobj.Normdata_.gPrev();
};

tempxyz_[0] = skmobj.Normdata_.ShowData(skmobj.Normdata_.gItorator());
printf("\n%f %f %f",tempxyz_[0].x,tempxyz_[0].y,tempxyz_[0].z);

printf("count:%d",skmobj.HData[3].count);
system("pause");

//GET Texeture DATA//////////////////
 
while(strcmp(parse_,"texture") != 0) file8>>parse_;
while(strcmp(parse_ , "vt") != 0) file8>>parse_;

skmobj.HData[3].count = 0;
while(strcmp(parse_ , "vt") == 0){   skmobj.HData[3].count++;
file8>> TempTexel.x>> TempTexel.y;  TempTexel.z = 0;  TempTexel.u = 0;
skmobj.TexelCoords_.enQueue( TempTexel);
file8>>parse_[0]; file8>>parse_[1]; 
pos_ = file8.tellg();
}


skmobj.TexelCoords_.iEnd();
while(skmobj.TexelCoords_.HasPrev() ){

 TempTexel = skmobj.TexelCoords_.ShowData(skmobj.TexelCoords_.gItorator());
printf("\n%f %f", TempTexel.x, TempTexel.y);
skmobj.TexelCoords_.gPrev();
};

 TempTexel = skmobj.TexelCoords_.ShowData(skmobj.TexelCoords_.gItorator());
printf("\n%f %f", TempTexel.x, TempTexel.y);


printf("count:%d",skmobj.HData[3].count);
system("pause");



//GET FACE DATA//////////////////

while(strcmp(parse_,"faces") != 0) file8>>parse_;
file8.getline(parse_, 80, '\n');


skmobj.HData[3].count = 0;
XYZi_ tempi[3]; char cstrtemp[6];
int y = 0,x = 0; int lsen = -1;

vect3i::List_  	Norm_dataOrder_;
vect3i::List_  	Texture_dataOrder_;




do
{
parse_[0] = 0;
file8.getline(parse_, 80, '\n');
//printf("%s", parse_);
//system("pause");
if(parse_[0] != 'f') break;

x = 2;

for(int z = 0;z<3;++z)
{

while(isdigit(parse_[x]) ){ cstrtemp[y] = parse_[x]; ++x;++y;}
cstrtemp[y] = '\0';
tempi[0].i = (unsigned short) atoi(cstrtemp); y = 0;++x;

while(isdigit(parse_[x]) ){ cstrtemp[y] = parse_[x]; ++x;++y;}
cstrtemp[y] = '\0';
tempi[1].i = (unsigned short) atoi(cstrtemp); y = 0;++x;

while(isalnum(parse_[x])){  cstrtemp[y] = parse_[x]; ++x;++y;}
cstrtemp[y] = '\0';
tempi[2].i = (unsigned short) atoi(cstrtemp); y = 0; ++x;
 
--tempi[0].i;
--tempi[1].i;
--tempi[2].i;

skmobj.Facedata_.enQueue(tempi[0]);skmobj.HData[3].count++;
Texture_dataOrder_.enQueue(tempi[1]);
Norm_dataOrder_.enQueue(tempi[2]);
printf("\n%i / %i / %i ", tempi[0].i,tempi[1].i,tempi[2].i);
}



}while(parse_[0] == 'f');

printf("\ncout: %d\n",skmobj.HData[3].count);
system("pause");

//unwanted normals re-sort the normals lazy

y = 0;
//printf("\nlse: %d \n", lsen);


printf("\nInitializing model data: ");

printf(" size(%d) faces ",
(skmobj.HData[3].count = initOBJModelData(skmobj, &Texture_dataOrder_, &Norm_dataOrder_))/3 );



vect3::List_  	SortedVertexData_;
vect3::List_  	SortedNormdata_;
vect2::List_	SortedTextures_;


printf("count:%d",skmobj.HData[3].count /3);
system("pause");

file8.close();
////////////////////////////////////////////////////////
//face index entry

printf("\nCount:%d",skmobj.HData[3].count /3);
printf("\nModel data Initialize Done!");
printf("\ninit Header");


skmobj.HData[0].count = 1;
skmobj.HData[1].count = 1;



printf("\nVertx: %d Faces: %d\n", skmobj.HData[2].count, skmobj.HData[3].count/3);
system("pause");


skmobj.HData[0].offset = 0;
skmobj.HData[1].offset = 0;
skmobj.HData[2].offset = 0;
skmobj.HData[3].offset = skmobj.HData[2].count * 80 +16;
//24576

skmobj.HData[3].count /= 3;

ALLDATA.Head[0] = skmobj.HData[0];
ALLDATA.Head[1] = skmobj.HData[1];
ALLDATA.Head[2] = skmobj.HData[2];
ALLDATA.Head[3] = skmobj.HData[3];

 

strcpy(ALLDATA.pad,ZeroP);


strcpy(ALLDATA.bones, "Convert_1 ");
strcpy(ALLDATA.Mater, "Temp_material ");
printf("\ndone!");
ALLDATA.VERT = new XYZ_ [skmobj.Model_.Size()*2];
ALLDATA.Normal = new XYZ_ [skmobj.Model_.Size()*2];
ALLDATA.Texel = new Texture_ [skmobj.Model_.Size()*2];
ALLDATA.index = new XYZi_ [skmobj.HData[3].count*3*2];
printf("\ndone!");




 float dummyf  = 0;
long counts = 0;
XYZ_ zero_ = {0,0,0,0};
Texture_ tzero_ = {0,0,0,0};


skmobj.Model_.iEnd();

do
{



if(counts != 0)
{
skmobj.Model_.gPrev(); }


ALLDATA.VERT[counts] = skmobj.Model_.ShowData(skmobj.Model_.gItorator()).Vect;
ALLDATA.Normal[counts] = skmobj.Model_.ShowData(skmobj.Model_.gItorator()).Norm;
ALLDATA.Texel[counts] = skmobj.Model_.ShowData(skmobj.Model_.gItorator()).Texel;

++counts;

ALLDATA.VERT[counts] = zero_ ;
ALLDATA.Normal[counts] = zero_;
ALLDATA.Texel[counts] = tzero_;

++counts;
printf("\nverts:%f %f %f",ALLDATA.VERT[counts-2].x,ALLDATA.VERT[counts-2].y,ALLDATA.VERT[counts-2].z);
printf("\ntexel:%f %f %f", ALLDATA.Texel[counts-2].x , ALLDATA.Texel[counts-2].y);
}while(skmobj.Model_.HasPrev() );


printf("\ndone! Initializing face index count: %d", counts/2);

counts = 0;
skmobj.Facedata_.iEnd(); int Index =0;
while(skmobj.Facedata_.HasPrev())
{

ALLDATA.index[counts++].i = 0x00;
ALLDATA.index[counts++] =  skmobj.Facedata_.ShowData(skmobj.Facedata_.gItorator());
if((skmobj.Facedata_.HasPrev())) skmobj.Facedata_.gPrev(); else break;
ALLDATA.index[counts++] =  skmobj.Facedata_.ShowData(skmobj.Facedata_.gItorator());
if((skmobj.Facedata_.HasPrev())) skmobj.Facedata_.gPrev(); else break;
ALLDATA.index[counts++] =  skmobj.Facedata_.ShowData(skmobj.Facedata_.gItorator());
if((skmobj.Facedata_.HasPrev())) skmobj.Facedata_.gPrev(); else break;

};



printf("\ndone!");
printf("\nInit file stream");



 fstream ofile;
 string Sname;
 Sname.assign(fname);
 Sname.append(".mes");
 ofile.open((char*)Sname.data(), ios::in | ios::out | ios::ate | ios::trunc | ios::binary );
 ofile.seekp(0);

printf("\nWriting counts and offsets");
ofile.write(reinterpret_cast<char*>( &ALLDATA),16*2);
printf("\ndone!");
printf("\nWriting Mesh and index data");

counts = 0;

ofile.seekp(skmobj.HData[2].offset, ios::beg);
for(int x = 0; x< skmobj.Model_.Size()*2;++x)
{

if(x == (skmobj.Model_.Size()*2 -1))
{ 	ALLDATA.Texel[x].x = 11;
 	ALLDATA.Texel[x].y = 11;
 }

++counts;
ofile.write(reinterpret_cast<char*>(  (XYZ_*)&ALLDATA.VERT[x]),4*4);
ofile.write(reinterpret_cast<char*>((XYZ_*)&ALLDATA.Normal[x]),4*4);
ofile.write(reinterpret_cast<char*>((XYZ_*) &ALLDATA.Texel[x].x), 4);
ofile.write(reinterpret_cast<char*>((XYZ_*) &ALLDATA.Texel[x].y), 4);
}

printf("\nWrote %d verts to file",counts);

counts = 0; ALLDATA.index[0].i = 0x0000;
skmobj.Facedata_.iEnd(); 

while(skmobj.Facedata_.HasPrev())
{
ofile.write(reinterpret_cast<char*>( &ALLDATA.index[0].i),2);
ofile.write(reinterpret_cast<char*>(  &skmobj.Facedata_.ShowData(skmobj.Facedata_.gItorator()).i), 2);
if(skmobj.Facedata_.HasPrev()) skmobj.Facedata_.gPrev(); else break;
ofile.write(reinterpret_cast<char*>(  &skmobj.Facedata_.ShowData(skmobj.Facedata_.gItorator()).i), 2);
if(skmobj.Facedata_.HasPrev()) skmobj.Facedata_.gPrev(); else break;
ofile.write(reinterpret_cast<char*>(  &skmobj.Facedata_.ShowData(skmobj.Facedata_.gItorator()).i), 2);
if(skmobj.Facedata_.HasPrev()) skmobj.Facedata_.gPrev(); else break;
}

/*
SKMHeader_T Head[4];
	PAD12 pad;
	BoneDef  bones;
	MatDef  Mater;
	XYZ_ * VERT;
	XYZ_ * Normal;
	Texture_ * Texel;
	XYZi_ * index;
*/

printf("\nDone , exit!");

 ofile.close();



 delete  [] ALLDATA.Normal;
 delete  [] ALLDATA.Texel;
 delete  [] ALLDATA.VERT;
 delete  [] ALLDATA.index;

return 0;



//head
//44 bits counts, offsets
//bones defs per # 100 byte block (name 48 bytes + 52 bytes unknown data per bone def)
//128 byte block material mesh mdf name null terminated followed by  0x0CCh per material possible garbage
//points data blocks
//triangles data blocks

}



int main(int argv, char* argc[])
{

if(argv==2){
ConvertVTX2SKM(argc[1]);
cout<<"\nFile conversion complete!"<<endl;
system("pause");
return 0;
}
else{

cout<<"\nOBJ input file parameter missing\n <exe> <file_name_path>"<<endl;
system("pause");
return 1;
}

}
