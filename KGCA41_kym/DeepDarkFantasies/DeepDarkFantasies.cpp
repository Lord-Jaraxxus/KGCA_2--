#include <iostream>

class TGraphicObject
{
public:
    virtual TGraphicObject* Clone() = 0;
};
class TTriangle : public TGraphicObject
{
public:
    int*    pDataTTriangle = new int(5);
    int     iDataTTriangle[100];
    int     iTest = 0;
public:
    //virtual TGraphicObject* Clone()
    //{
    //    return new TTriangle(*this);
    //}
    virtual TTriangle* Clone()
    {
        return new TTriangle(*this);
    }
};

class TRectangle : public TGraphicObject
{
public:
    int*    pDataTRectangle = new int(5);
    int     iDataTRectangle[100];
public:
    virtual TGraphicObject* NewObject()
    {
        return new TRectangle;
    }
    virtual TGraphicObject* Clone()
    {
        return new TRectangle(*this);
    }
};


int main()
{

    //int a[10] = { 0, };
    //int b[10] = { 0, };
    ////b = a;
    //int aa = 0;
    //int bb = 1;
    //bb = aa;

    //TTriangle TriObj;
    //for (int i = 0; i < 100; i++)
    //{
    //    TriObj.iDataTTriangle[i] = rand();
    //}

    //TTriangle TriObj_Clone;
    //TriObj_Clone = TriObj;

    TTriangle* pTriObj = new TTriangle;
    for (int i = 0; i < 100; i++)
    {
        pTriObj->iDataTTriangle[i] = rand();
    }

    TRectangle* pRectObj = new TRectangle;
    for (int i = 0; i < 100; i++)
    {
        pRectObj->iDataTRectangle[i] = rand();
    }

    TTriangle* pObjA = new TTriangle(*pTriObj);
    
    //delete pTriObj;
    *pTriObj->pDataTTriangle = 525;
    pTriObj->iDataTTriangle[0] = 525;
    pTriObj->iTest = 525;
    //pTriObj->pDataTTriangle = OEO;

    TGraphicObject* pObjB = pRectObj->Clone();

    std::cout << "Hello World!\n";
}