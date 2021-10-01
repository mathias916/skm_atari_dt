






#include <iostream.h>
#include <stddef.h>
#include <iomanip.h>

//#define	system	printf

typedef struct XYZi_
{
unsigned short i;
}XYZi_;

typedef struct XYZ_
{
float x,y,z,w;
}XYZ_;

typedef struct Texture_
{
float x,y,z,u;
}Normal_;

typedef struct ModelData_
{
XYZ_ Vect;
XYZ_ Norm;
Texture_ Texel;
}ModelData_;



namespace vect
{

typedef float DataT_;
typedef int DataT_Key;
#include <list_.h>

};

namespace vect3
{

typedef XYZ_ DataT_;
typedef int DataT_Key;
#include <list_.h>

};

namespace vect3i
{



typedef XYZi_ DataT_;
typedef int DataT_Key;
#include <list_.h>

};

namespace vect2
{

typedef Texture_ DataT_;
typedef int DataT_Key;
#include <list_.h>
};

namespace ModelSD
{

typedef ModelData_ DataT_;
typedef int DataT_Key;
#include <list_.h>
};



typedef struct SKMHeader_T
{
unsigned long count;
unsigned long offset;
}SKMHeader_T;


typedef struct SKMData
{

SKMHeader_T HData[4];

//WORD 2 bytes
//DWORD 4 bytes

 vect3::List_  	Vertdata_; //Vertex data size: DWORD
vect3i::List_  	Facedata_; //index data size: Word
 vect3::List_  	Normdata_;
 vect2::List_	TexelCoords_;

 ModelSD::List_   Model_;


}SKMData;

#include <fstream.h>

SKMData GData;
SKMData Bones;


int LoadRaDATA(char *filen)
{
int tempf[2];
fstream dfile;
fstream ofile;
dfile.open(filen, ios::in | ios::out | ios::ate | ios::binary );
dfile.seekg(0);

char Buff[255];
string fname1;
fname1.assign(filen);
fname1.append(".obj");
ofile.open((char*)fname1.data(),  ios::in | ios::out | ios::trunc | ios::ate );
ofile.seekg(0);

XYZ_ tempxyz;
int count;

dfile.read(reinterpret_cast<char *>(&GData.HData),4*8);

printf("\nbones: %d materials: %d points: %d faces: %d \n", GData.HData[0].count,GData.HData[1].count,GData.HData[2].count,GData.HData[3].count);
printf("\n@bit : %d @bit     : %d @bit  : %d @bit : %d \n", GData.HData[0].offset,GData.HData[1].offset,GData.HData[2].offset,GData.HData[3].offset);

dfile.seekg(GData.HData[2].offset);
int pos = dfile.tellg(); float tempfl;
Texture_ TempTexel;

count = 0;
while( pos < GData.HData[3].offset && count/2 < GData.HData[2].count){ 


++count;

dfile.read(reinterpret_cast<char *>(&tempxyz),4*4);
if(count % 2 == 1)
GData.Vertdata_.enQueue(tempxyz);

if(count % 2 == 0)
Bones.Vertdata_.enQueue(tempxyz);

dfile.read(reinterpret_cast<char *>(&tempxyz),4*4);
if(count % 2 == 1)
GData.Normdata_.enQueue(tempxyz);

if(count % 2 == 0)
Bones.Normdata_.enQueue(tempxyz);

dfile.read(reinterpret_cast<char *>(&TempTexel),4*2);
if(count % 2 == 1)
GData.TexelCoords_.enQueue(TempTexel);

if(count % 2 == 0)
Bones.TexelCoords_.enQueue(TempTexel);



pos = dfile.tellg();
}

printf("\ncount: %d\n",count/2);
//system("pause");


XYZi_ tempI; count = 0;
dfile.clear();
dfile.seekg(GData.HData[3].offset, ios::beg);
while(!dfile.fail() )
{
++count;

//id
short id;
dfile.read(reinterpret_cast<char *>(&id),2);
if(dfile.eof()) break;

dfile.read(reinterpret_cast<char *>(&tempI.i),2);
if(!dfile.eof())
GData.Facedata_.enQueue(tempI);
if(dfile.eof()) break;
 
dfile.read(reinterpret_cast<char *>(&tempI.i),2);
if(!dfile.eof())
GData.Facedata_.enQueue(tempI);
if(dfile.eof()) break;

dfile.read(reinterpret_cast<char *>(&tempI.i),2);
if(!dfile.eof())
GData.Facedata_.enQueue(tempI);
if(dfile.eof()) break;


 //pos = dfile.tellg();
//printf("id:%d :",id);

}

dfile.close();

//write obj file format

ofile<<"\# Created with SKMTools 1.2"<<endl;
ofile<<"\# Object \"Convert_1.0\":"<<endl;
ofile<<"\# ComRec:"<<endl;
ofile<<"g mesh01"<<endl;
ofile<<"\# No. points "<<GData.HData[2].count<<":"<<endl;
GData.Vertdata_.iEnd();
count = 0;
char nbuff[255];
do{
if(count != 0)
GData.Vertdata_.gPrev();
sprintf(nbuff,"%f %f %f",GData.Vertdata_.gItorator()->data_.x,GData.Vertdata_.gItorator()->data_.y,GData.Vertdata_.gItorator()->data_.z);
ofile<<"v "<<nbuff<<endl;
++count;
}while(GData.Vertdata_.HasPrev());

ofile<<endl;

ofile<<"\# No. normals "<<GData.HData[2].count<<":"<<endl;
GData.Normdata_.iEnd();
count = 0;
do{
if(count != 0)
GData.Normdata_.gPrev();
sprintf(nbuff,"%f %f %f",GData.Normdata_.gItorator()->data_.x,GData.Normdata_.gItorator()->data_.y,GData.Normdata_.gItorator()->data_.z);
ofile<<"vn "<<nbuff<<endl;
++count;
}while(GData.Normdata_.HasPrev());

ofile<<endl;

ofile<<"\# No. texture coordinates "<<GData.HData[2].count<<":"<<endl; 
GData.TexelCoords_.iEnd();
count = 0;

do{
if(count != 0)
GData.TexelCoords_.gPrev();
sprintf(nbuff,"%f %f",GData.TexelCoords_.gItorator()->data_.x,GData.TexelCoords_.gItorator()->data_.y);
ofile<<"vt "<<nbuff<<endl;
++count;
}while(GData.TexelCoords_.HasPrev());

ofile<<endl;

ofile<<"\# No. faces "<<GData.HData[3].count<<":";

GData.Facedata_.iEnd();
count = 0;
do{
if(count != 0)
GData.Facedata_.gPrev();

if(count % 3 == 0 || count == 0)
ofile<<endl<<"f ";
ofile<<GData.Facedata_.gItorator()->data_.i+1<<"/"<<GData.Facedata_.gItorator()->data_.i+1<<"/"<<GData.Facedata_.gItorator()->data_.i+1<<" ";
++count;
}while(GData.Facedata_.HasPrev());



ofile<<endl;
ofile.close();


string bonestr;
bonestr.assign(filen);
bonestr.append(".SKB");
ofile.open((char*)bonestr.data(),ios::in | ios::out | ios::trunc | ios::binary);
ofile.seekp(0,ios::beg);

Bones.Vertdata_.iEnd(); Bones.Normdata_.iEnd(); Bones.TexelCoords_.iEnd(); 
count = 0;
do{

if(count != 0){
Bones.Vertdata_.gPrev();Bones.Normdata_.gPrev();Bones.TexelCoords_.gPrev();
}


ofile.write(reinterpret_cast<char*>(&Bones.Vertdata_.ShowData(Bones.Vertdata_.gItorator())    ), 4*4);
ofile.write(reinterpret_cast<char*>(&Bones.Normdata_.ShowData(Bones.Normdata_.gItorator())    ), 4*4);
ofile.write(reinterpret_cast<char*>(&Bones.TexelCoords_.ShowData(Bones.TexelCoords_.gItorator()) ), 4*2);
++count;

}while(Bones.Vertdata_.HasPrev());

ofile.close();

return 1;
}

#ifndef MAIN__1.0


int main(int argv, char * argc[])
{

if(argv == 2){
char buffn[255];
strcpy(buffn, argc[1]);
LoadRaDATA(buffn);
cout<<"\nfile converted!";
cout<<endl;

system("pause");
return 0;
}
else{
printf("skm file input parameter needed\ninput: <exe> <filname.skm>\nOutput:<filename.skm.an8>\n");
system("pause");
return 1;}

}

#endif
