

#include <Timer.h>




#include <iostream.h>
#include <stddef.h>

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
fname1.append(".an8");
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
while( pos < GData.HData[3].offset ){ 

++count;

dfile.read(reinterpret_cast<char *>(&tempxyz),4*4);
if(count % 2 == 1)
GData.Vertdata_.enQueue(tempxyz);


dfile.read(reinterpret_cast<char *>(&tempxyz),4*4);
if(count % 2 == 1)
GData.Normdata_.enQueue(tempxyz);


dfile.read(reinterpret_cast<char *>(&TempTexel),4*2);
if(count % 2 == 1)
GData.TexelCoords_.enQueue(TempTexel);



pos = dfile.tellg();
}

printf("\ncount: %d\n",count);
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


dfile.read(reinterpret_cast<char *>(&tempI.i),2);
GData.Facedata_.enQueue(tempI);
 
dfile.read(reinterpret_cast<char *>(&tempI.i),2);
if(!dfile.eof())
GData.Facedata_.enQueue(tempI);

dfile.read(reinterpret_cast<char *>(&tempI.i),2);
if(!dfile.eof())
GData.Facedata_.enQueue(tempI);


 //pos = dfile.tellg();
//printf("id:%d :",id);

}

//.AN8 file format heading inits 
ofile<<"header {\n  version { \"0.95\" }\n  build { \"2007.04.02\" }\n}\nenvironment {\n  grid { 0 5 10 50 }\n";
ofile<<"  framerate { 24 }\n}\nobject { \"Convert_1.0\"\n  mesh {\n    name { \"mesh01\" }\n    base {\n";
ofile<<"      origin { (0 0 0) }\n    }\n    material { \" -- default --\" }\n    smoothangle { 45 }\n";
ofile<<"    /* "<<GData.HData[2].count<<" points, "<<GData.HData[3].count<<" faces, "<<GData.HData[2].count<<" uvCoords */\n";
ofile<<"    materiallist {\n      materialname { \" -- default --\" }\n    }";


ofile<<"\n    points {";

GData.Vertdata_.iEnd(); count = 0;
while(GData.Vertdata_.HasPrev()){  ++count; if(count%3==1) ofile<<endl<<"      ";
ofile<<"("
<<GData.Vertdata_.ShowData(GData.Vertdata_.gItorator()).x<<" "
<<GData.Vertdata_.ShowData(GData.Vertdata_.gItorator()).y<<" "
<<GData.Vertdata_.ShowData(GData.Vertdata_.gItorator()).z
<<") ";

GData.Vertdata_.gPrev();
//if(count % 50 == 0) 
//system("pause"); 
}

ofile<<"\n    }";
ofile<<"\n    texcoords {";

GData.TexelCoords_.iEnd(); count = 0;
while(GData.TexelCoords_.HasPrev()){ ++count; if(count%3==1) ofile<<"\n      ";
ofile<<"("
<<GData.TexelCoords_.ShowData(GData.TexelCoords_.gItorator()).x<<" "
<<GData.TexelCoords_.ShowData(GData.TexelCoords_.gItorator()).y<<")"
<<" ";
GData.TexelCoords_.gPrev();   
}


ofile<<"\n    }";
ofile<<"\n    faces {";

dfile.clear();
GData.Facedata_.iEnd(); count = 0;
while(GData.Facedata_.HasPrev()){ ++count; if(count%3 == 1 && count != 1) ofile<<")\n      3 4 0 -1 ( "; if(count == 1) ofile<<"\n      3 4 0 -1 ( ";
ofile<<"("<<GData.Facedata_.ShowData(GData.Facedata_.gItorator()).i<<" "<<GData.Facedata_.ShowData(GData.Facedata_.gItorator()).i<<") ";
GData.Facedata_.gPrev();
}
ofile<<")\n    }\n  }\n}";



printf("\ncount: %d\n",count);
//system("pause");

int L; GData.Facedata_.iEnd();
while(GData.Facedata_.HasPrev())
{

L=0; GData.Vertdata_.iEnd(); GData.Normdata_.iEnd(); GData.TexelCoords_.iEnd();
while(GData.Vertdata_.HasPrev()  && (L < GData.Facedata_.ShowData(GData.Facedata_.gItorator()).i) ){++L; 
GData.Vertdata_.gPrev(); GData.Normdata_.gPrev(); GData.TexelCoords_.gPrev();}

ModelData_ TempModel;
TempModel.Vect = GData.Vertdata_.ShowData(GData.Vertdata_.gItorator());
TempModel.Norm = GData.Normdata_.ShowData(GData.Normdata_.gItorator());
TempModel.Texel = GData.TexelCoords_.ShowData(GData.TexelCoords_.gItorator());

GData.Model_.enQueue(TempModel);
GData.Facedata_.gPrev();
}

ofile.close();
dfile.close();
return 1;
}


//used for data from OBJ to MES
//wave obj fucked up index format solution trial function
//initiaize model via 3 index parameters
//this pairs the givin obj index data with vertex data
//sorting it into indexed specific elements ( "a bitch and a half")

unsigned int initOBJModelData(SKMData & Entry, 
vect3i::List_   * Texture_index,
vect3i::List_   * Normal_index)
{
ModelData_ *TempModel; unsigned int count = 0;
TempModel = new ModelData_[Entry.Vertdata_.Size()];

unsigned int L; 
Entry.Vertdata_.iEnd();Entry.Normdata_.iEnd();Entry.TexelCoords_.iEnd();
int e = 0;

do
{

if(e != 0)
{ Entry.Vertdata_.gPrev();Entry.Normdata_.gPrev();Entry.TexelCoords_.gPrev(); }

TempModel[e].Vect = Entry.Vertdata_.ShowData(Entry.Vertdata_.gItorator());
TempModel[e].Norm = Entry.Normdata_.ShowData(Entry.Normdata_.gItorator());
TempModel[e].Texel = Entry.TexelCoords_.ShowData(Entry.TexelCoords_.gItorator());

printf("\ne: %d vert: x:%f y:%f z:%f", e,  
 TempModel[e].Vect.x,TempModel[e].Vect.y,TempModel[e].Vect.z);

++e;
}while(Entry.Vertdata_.HasPrev());

cout<<endl;
system("pause");


Entry.Facedata_.iEnd();Normal_index->iEnd();Texture_index->iEnd();


Timer_ t1;
do
{

if(count != 0 )
{ Entry.Facedata_.gPrev(); Normal_index->gPrev(); Texture_index->gPrev(); }

e = Entry.Facedata_.ShowData(Entry.Facedata_.gItorator()).i;


L= 0; Entry.Normdata_.iEnd();
while(Entry.Normdata_.HasPrev()  && (L < Normal_index->ShowData(Normal_index->gItorator()).i) )
{++L; Entry.Normdata_.gPrev();}


L= 0; Entry.TexelCoords_.iEnd();
while( Entry.TexelCoords_.HasPrev()  && (L < Texture_index->ShowData(Texture_index->gItorator()).i) )
{++L;  Entry.TexelCoords_.gPrev();} 

L= 0; Entry.Vertdata_.iEnd();
while( Entry.Vertdata_.HasPrev()  && (L < Entry.Facedata_.ShowData(Entry.Facedata_.gItorator()).i) )
{++L;  Entry.Vertdata_.gPrev();} 



TempModel[e].Vect = Entry.Vertdata_.ShowData(Entry.Vertdata_.gItorator());
TempModel[e].Norm = Entry.Normdata_.ShowData(Entry.Normdata_.gItorator());
TempModel[e].Texel = Entry.TexelCoords_.ShowData(Entry.TexelCoords_.gItorator());


printf("\ne: %d texeli: %d x:%f y:%f", e, Texture_index->ShowData(Texture_index->gItorator()).i, 
 TempModel[e].Texel.x,TempModel[e].Texel.y);


++count;
}while(Entry.Facedata_.HasPrev());

for(e = 0; e < Entry.Vertdata_.Size() ;++e){ Entry.Model_.enQueue(TempModel[e]); }

delete [] TempModel;
return count;
}




/*

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
*/