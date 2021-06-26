/*************************************************/
/* ��ʵսMATLAB֮�ļ������ݽӿڼ��������ӳ���    */
/* ��  �ߣ������� ��ά                           */
/* �����磺�������պ����ѧ������                */
/*************************************************/

#include "stdio.h"
#include "mat.h"
#include "matrix.h"
#define _START_FLAG 0xAAAAAAAA
#define _END_FLAG 0xEEEEEEEE

typedef struct _FRAME_HEAD
{
	unsigned int startv;
	unsigned int frameLen;
	unsigned int type;
}fmHead;

char * GetElementType(unsigned int type)
{
    switch(type)
    {
        case 1:
        {
            return "float";
        }
        case 2:
        {
            return "double";
        }
        case 3:
        {
            return "int16";
        }
        case 4:
        {
            return "int32";
        }
        default:
        {
            return "unknown";
        }
    }
}

int GetElementSize(unsigned int type)
{
    switch(type)
    {
        case 1:
        {
            return sizeof(float);
        }
        case 2:
        {
            return sizeof(double);
        }
        case 3:
        {
            return sizeof(short);
        }
        case 4:
        {
            return sizeof(int);
        }
        default:
        {
            return 1;//Ĭ�ϰ��ֽڼ���
        }
    }
}

mxClassID GetClassID(unsigned int type)
{
    switch(type)
    {
        case 1:
        {
            return mxSINGLE_CLASS;
        }
        case 2:
        {
            return mxDOUBLE_CLASS;
        }
        case 3:
        {
            return 	mxINT16_CLASS;
        }
        case 4:
        {
            return mxINT32_CLASS;
        }
        default:
        {
            return mxDOUBLE_CLASS;//Ĭ�ϰ��ֽڼ���
        }
    }
}

//��ȡ����֡
mxArray * ReadData(FILE * fp)
{
    int ib,ie;
    int fpos;
    int kk;
    unsigned int startv,endv;
    fmHead head;
    double filesize;
    char * fieldnames[5] = {"startv","frameLen","type","data","endv"}; 
    mxArray * pDataArray=NULL;
    mxArray * pOut = NULL;
    void * pData = NULL;
    int datasize;
    
    /*��ȡ�ļ���С*/
    fseek(fp,0,SEEK_SET);
    ib = ftell(fp);
    fseek(fp,0,SEEK_END);
    ie = ftell(fp);
    filesize = ie - ib;
    
    /*ͳ������֡����*/
    fseek(fp,0,SEEK_SET);
    fpos = ftell(fp);
    kk = 0;
    while(fpos<filesize)
    {
        fread(&startv,sizeof(unsigned int),1,fp);
        if(startv == _START_FLAG)
        {
            fseek(fp,-4,SEEK_CUR);
            fread(&head,sizeof(fmHead),1,fp);                  
            fseek(fp,head.frameLen-sizeof(fmHead)-sizeof(unsigned int),SEEK_CUR);
            fread(&endv,sizeof(unsigned int),1,fp);
            kk = kk + 1;           
        }
        fpos = ftell(fp);
    }
    
     /*������������ݽṹ��*/
     pOut = mxCreateStructMatrix(kk, 1, 5, (const char **)fieldnames);
     /*��������*/
    fseek(fp,0,SEEK_SET);
    fpos = ftell(fp);
    kk = 0;
    while(fpos<filesize)
    {
        fread(&startv,sizeof(unsigned int),1,fp);
        if(startv == _START_FLAG)
        {
            fseek(fp,-4,SEEK_CUR);
            fread(&head,sizeof(fmHead),1,fp);
            mxSetField(pOut,kk,"startv",mxCreateDoubleScalar(head.startv));
            mxSetField(pOut,kk,"frameLen",mxCreateDoubleScalar(head.frameLen));
            mxSetField(pOut,kk,"type",mxCreateDoubleScalar(head.type)); 
            datasize =  head.frameLen-sizeof(fmHead)-sizeof(unsigned int);
            pDataArray = mxCreateNumericMatrix(datasize/GetElementSize(head.type),1,GetClassID(head.type),mxREAL);            
            pData = mxGetData(pDataArray);
            fread(pData,datasize,1,fp);
            mxSetField(pOut,kk,"data",pDataArray);
            fread(&endv,sizeof(unsigned int),1,fp);
            mxSetField(pOut,kk,"endv",mxCreateDoubleScalar(endv));
            kk = kk + 1;           
        }
        fpos = ftell(fp);
    }
    
    return pOut;
}

int main(int argc, char* argv[])
{
	FILE * fp=NULL;	
    mxArray * pStruct;
    MATFile  * pMatFile;
	if(argc<3)
	{
		printf("�����������ļ��������MAT�ļ�����\n");
		return 1;
	}
	fp =  fopen(argv[1],"rb");
	if(!fp)
	{
		printf("�ļ��򿪴���,fp = 0x%x\n",fp);
		return 1;
	}    
    pStruct = ReadData(fp);    
    fclose(fp);
    fp = NULL;
    
    pMatFile = matOpen(argv[2],"w");
    matPutVariable(pMatFile,"framedata",pStruct);    
    matClose(pMatFile);
    
    mxDestroyArray(pStruct);
	return 0;
}

