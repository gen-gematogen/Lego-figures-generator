#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/glut.h>
#include"HelpFile.hpp"
#include<math.h>

#include "imgui.h"
#include "imgui-sfml.h"
#include "imgui.cpp"
#include "imgui_draw.cpp"
#include "imgui-SFML.cpp"
#include "imgui_demo.cpp"
#include <tchar.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#ifndef GL_SRGB8_ALPHA8
#define GL_SRGB8_ALPHA8 0x8C43
#endif




#include <windows.h>
#include <stdio.h>
#include <iostream>

#include <gdiplus.h>

using namespace std;

#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

static const GUID png =
{ 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

string generateString(int a){
    string str = "";
    switch(a / 100){
        case(0):{str += '0';break;}
        case(1):{str += '1';break;}
        case(2):{str += '2';break;}
        case(3):{str += '3';break;}
        case(4):{str += '4';break;}
        case(5):{str += '5';break;}
        case(6):{str += '6';break;}
        case(7):{str += '7';break;}
        case(8):{str += '8';break;}
        case(9):{str += '9';break;}
    }
    switch((a % 100)/10){
        case(0):{str += '0';break;}
        case(1):{str += '1';break;}
        case(2):{str += '2';break;}
        case(3):{str += '3';break;}
        case(4):{str += '4';break;}
        case(5):{str += '5';break;}
        case(6):{str += '6';break;}
        case(7):{str += '7';break;}
        case(8):{str += '8';break;}
        case(9):{str += '9';break;}
    }
    switch(a % 10){
        case(0):{str += '0';break;}
        case(1):{str += '1';break;}
        case(2):{str += '2';break;}
        case(3):{str += '3';break;}
        case(4):{str += '4';break;}
        case(5):{str += '5';break;}
        case(6):{str += '6';break;}
        case(7):{str += '7';break;}
        case(8):{str += '8';break;}
        case(9):{str += '9';break;}
    }
    return str;
}


void SaveScreen(HWND hWnd)
{
   HDC hDC, CompDC;
   SYSTEMTIME SysTm;
   BITMAP bmp;
   PBITMAPINFO pbmi;
   HBITMAP hBitmap;
   BITMAPFILEHEADER BFHeader;
   WORD ClrBits;
   DWORD dwTmp;
   LPBYTE lpBits;
   HANDLE hFile;
   char FileName[22], dir[MAX_PATH];
    hDC=GetDC(hWnd);
   CompDC=CreateCompatibleDC(hDC);
   hBitmap=CreateCompatibleBitmap(hDC,GetDeviceCaps(hDC, HORZRES),GetDeviceCaps(hDC, VERTRES));
   SelectObject(CompDC,hBitmap);
   BitBlt(CompDC,0,0,GetDeviceCaps(hDC, HORZRES),GetDeviceCaps(hDC, VERTRES),hDC,0,0,SRCCOPY);
   ReleaseDC(hWnd, hDC);
   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);
   ClrBits=(WORD)bmp.bmPlanes*bmp.bmBitsPixel;

   char wayDir[MAX_PATH];//way directoria
   GetCurrentDirectory(sizeof(wayDir),wayDir);
   strcpy(dir,wayDir);
   if(ClrBits<=16)
   {
      ClrBits=16;
   }
   else
   {
      if(ClrBits<=24)
      {
         ClrBits=24;
      }
      else
      {
         ClrBits=32;
      }
   }

      pbmi=(PBITMAPINFO)new BYTE [sizeof(BITMAPINFOHEADER)];

   pbmi->bmiHeader.biClrUsed=0;
   pbmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
   pbmi->bmiHeader.biWidth=bmp.bmWidth;
   pbmi->bmiHeader.biHeight=bmp.bmHeight;
   pbmi->bmiHeader.biPlanes=bmp.bmPlanes;
   pbmi->bmiHeader.biBitCount=bmp.bmBitsPixel;
   if(ClrBits<24)
   {
      pbmi->bmiHeader.biClrUsed=2^ClrBits;
   }
   pbmi->bmiHeader.biCompression=BI_RGB;
   pbmi->bmiHeader.biSizeImage=(pbmi->bmiHeader.biWidth)*pbmi->bmiHeader.biHeight*(ClrBits/8);
   pbmi->bmiHeader.biClrImportant=0;
   lpBits= (UCHAR*)VirtualAlloc(NULL, pbmi->bmiHeader.biSizeImage, MEM_TOP_DOWN | MEM_COMMIT, PAGE_READWRITE);
   GetDIBits(CompDC, hBitmap,0,pbmi->bmiHeader.biHeight,lpBits,pbmi,DIB_RGB_COLORS);
   GetLocalTime(&SysTm);
   sprintf(FileName,"\\Perspective.bmp", SysTm.wYear,SysTm.wMonth,SysTm.wDay,SysTm.wHour,SysTm.wMinute,SysTm.wSecond,SysTm.wMilliseconds);
   strcat(dir,FileName);
   hFile=CreateFile(dir,GENERIC_READ|GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
   BFHeader.bfType=0x4d42;
   BFHeader.bfSize=sizeof(BITMAPFILEHEADER)+pbmi->bmiHeader.biSize+pbmi->bmiHeader.biClrUsed*sizeof(RGBQUAD)+pbmi->bmiHeader.biSizeImage;
   BFHeader.bfReserved1=0;
   BFHeader.bfReserved2=0;
   BFHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+pbmi->bmiHeader.biSize+pbmi->bmiHeader.biClrUsed*sizeof(RGBQUAD);
   WriteFile(hFile, &BFHeader, sizeof(BITMAPFILEHEADER),(LPDWORD)&dwTmp,0);
      WriteFile(hFile, pbmi,sizeof(BITMAPINFOHEADER)+pbmi->bmiHeader.biClrUsed*sizeof(RGBQUAD),(LPDWORD)&dwTmp,0);
    WriteFile(hFile,lpBits,pbmi->bmiHeader.biSizeImage,&dwTmp,0);
   CloseHandle(hFile);

   DeleteDC(CompDC);
   DeleteObject(hBitmap);
   VirtualFree(lpBits, 0, MEM_RELEASE) ;
   delete pbmi;
}
///===========================


using namespace sf;

Texture LoadTexture(String namefiletexture)
{
    Texture texture;
    texture.loadFromFile(namefiletexture);

    texture.generateMipmap();
    return texture;
}

IntRect spriteTransforamtNumberDoz(int number)
{
    int a;
    a=number/10;

    switch(a)
    {
        case(0):{return(IntRect(0,370,40,35));}
        case(1):{return(IntRect(45,370,45,35));}
        case(2):{return(IntRect(95,370,45,35));}
        case(3):{return(IntRect(150,370,45,35));}
        case(4):{return(IntRect(200,370,45,35));}
        case(5):{return(IntRect(250,370,45,35));}
        case(6):{return(IntRect(305,370,45,35));}
        case(7):{return(IntRect(355,370,45,35));}
        case(8):{return(IntRect(410,370,45,35));}
        case(9):{return(IntRect(460,370,45,35));}
    }
}
IntRect spriteTransforamtNumberUnit(int number)
{
    int b;
    b=number%10;
    switch(b)
    {
        case(0):{return(IntRect(0,370,40,35));}
        case(1):{return(IntRect(45,370,45,35));}
        case(2):{return(IntRect(95,370,45,35));}
        case(3):{return(IntRect(150,370,45,35));}
        case(4):{return(IntRect(200,370,45,35));}
        case(5):{return(IntRect(250,370,45,35));}
        case(6):{return(IntRect(305,370,45,35));}
        case(7):{return(IntRect(355,370,45,35));}
        case(8):{return(IntRect(410,370,45,35));}
        case(9):{return(IntRect(460,370,45,35));}
    }
}

const int sizeArr = 990;



int main()
{
    float angleX = 0, angleY = 0;
    float PI = 3.1415926535;
    float rotateX = 1, rotateY = 0;
    float R = 100.0;
    int D=5;
    //int a[D],b[D],c[D],d[D],e[D];
    bool marA=false,marScreen=false;
    int SumCubes=0;
    int markerScreen=0;
    bool rotR = false;
    cubeColor arrCubeColor[sizeArr];
    for(int i = 0;i<sizeArr;i++)
        {arrCubeColor[i].r=1;
        arrCubeColor[i].g=1;
        arrCubeColor[i].b=1;}
    float scaledCube = 3;

    int scrX = GetSystemMetrics(SM_CXSCREEN);
    int scrY = GetSystemMetrics(SM_CYSCREEN);

    dataCube arrDataCubes[sizeArr];
    cout << "length arrDistCube: " << sizeof(arrDataCubes) << "\n";

    int arrCubes[10];//arrNumberCubes;
    for (int i=0;i<10;i++)arrCubes[i]=0;

    bool exit = false;
    bool sRgb = false;

    while (!exit)
    {
        // Request a 24-bits depth buffer when creating the window
        ContextSettings contextSettings;
        contextSettings.depthBits = 24;
        contextSettings.sRgbCapable = sRgb;

        // Create the main window
        RenderWindow window(VideoMode(1500,800), "LEGO figures", Style::Default, contextSettings);
        window.setVerticalSyncEnabled(true);//управление кадрами
        //sf::WindowHandle handle = window.getSystemHandle();
        //ImGui::SFML::Init(window);

///GUI=============================
        Texture t;
        t.loadFromFile("button.jpg");
        Sprite buttonLego[10];
        Sprite numArr[20];

        buttonLego[0].setTexture(t);
        buttonLego[0].setTextureRect(IntRect(0,0,40,40));
        buttonLego[0].setPosition(window.getSize().x*0.7+window.getSize().x/150,window.getSize().y/80);///*********
        numArr[0].setTexture(t);
        numArr[0].setTextureRect(IntRect(0,370,40,35));
        numArr[0].setPosition(window.getSize().x*0.7+60,10);
        numArr[1].setTexture(t);
        numArr[1].setTextureRect(IntRect(0,370,40,35));
        numArr[1].setPosition(window.getSize().x*0.7+100,10);

        buttonLego[1].setTexture(t);
        buttonLego[1].setTextureRect(IntRect(0,0,80,40));
        buttonLego[1].setPosition(window.getSize().x*0.7+window.getSize().x/150,54);
        numArr[2].setTexture(t);
        numArr[2].setTextureRect(IntRect(0,370,40,35));
        numArr[2].setPosition(window.getSize().x*0.7+100,54);
        numArr[3].setTexture(t);
        numArr[3].setTextureRect(IntRect(0,370,40,35));
        numArr[3].setPosition(window.getSize().x*0.7+140,54);

        buttonLego[2].setTexture(t);
        buttonLego[2].setTextureRect(IntRect(0,0,120,40));
        buttonLego[2].setPosition(window.getSize().x*0.7+window.getSize().x/150,98);
        numArr[4].setTexture(t);
        numArr[4].setTextureRect(IntRect(0,370,40,35));
        numArr[4].setPosition(window.getSize().x*0.7+140,98);
        numArr[5].setTexture(t);
        numArr[5].setTextureRect(IntRect(0,370,40,35));
        numArr[5].setPosition(window.getSize().x*0.7+180,98);

        buttonLego[3].setTexture(t);
        buttonLego[3].setTextureRect(IntRect(0,0,160,40));
        buttonLego[3].setPosition(window.getSize().x*0.7+window.getSize().x/150,142);
        numArr[6].setTexture(t);
        numArr[6].setTextureRect(IntRect(0,370,40,35));
        numArr[6].setPosition(window.getSize().x*0.7+180,142);
        numArr[7].setTexture(t);
        numArr[7].setTextureRect(IntRect(0,370,40,35));
        numArr[7].setPosition(window.getSize().x*0.7+220,142);

        buttonLego[4].setTexture(t);
        buttonLego[4].setTextureRect(IntRect(0,0,240,40));
        buttonLego[4].setPosition(window.getSize().x*0.7+window.getSize().x/150,186);
        numArr[8].setTexture(t);
        numArr[8].setTextureRect(IntRect(0,370,40,35));
        numArr[8].setPosition(window.getSize().x*0.7+260,186);
        numArr[9].setTexture(t);
        numArr[9].setTextureRect(IntRect(0,370,40,35));
        numArr[9].setPosition(window.getSize().x*0.7+300,186);

        buttonLego[5].setTexture(t);
        buttonLego[5].setTextureRect(IntRect(0,0,320,40));
        buttonLego[5].setPosition(window.getSize().x*0.7+window.getSize().x/150,230);
        numArr[10].setTexture(t);
        numArr[10].setTextureRect(IntRect(0,370,40,35));
        numArr[10].setPosition(window.getSize().x*0.7+340,230);
        numArr[11].setTexture(t);
        numArr[11].setTextureRect(IntRect(0,370,40,35));
        numArr[11].setPosition(window.getSize().x*0.7+380,230);

        buttonLego[6].setTexture(t);
        buttonLego[6].setTextureRect(IntRect(0,40,80,80));
        buttonLego[6].setPosition(window.getSize().x*0.7+window.getSize().x/150,274);
        numArr[12].setTexture(t);
        numArr[12].setTextureRect(IntRect(0,370,40,35));
        numArr[12].setPosition(window.getSize().x*0.7+100,274);
        numArr[13].setTexture(t);
        numArr[13].setTextureRect(IntRect(0,370,40,35));
        numArr[13].setPosition(window.getSize().x*0.7+140,274);

        buttonLego[7].setTexture(t);
        buttonLego[7].setTextureRect(IntRect(0,40,120,80));
        buttonLego[7].setPosition(window.getSize().x*0.7+window.getSize().x/150,360);
        numArr[14].setTexture(t);
        numArr[14].setTextureRect(IntRect(0,370,40,35));
        numArr[14].setPosition(window.getSize().x*0.7+140,360);
        numArr[15].setTexture(t);
        numArr[15].setTextureRect(IntRect(0,370,40,35));
        numArr[15].setPosition(window.getSize().x*0.7+180,360);

        buttonLego[8].setTexture(t);
        buttonLego[8].setTextureRect(IntRect(0,40,160,80));
        buttonLego[8].setPosition(window.getSize().x*0.7+window.getSize().x/150,446);
        numArr[16].setTexture(t);
        numArr[16].setTextureRect(IntRect(0,370,40,35));
        numArr[16].setPosition(window.getSize().x*0.7+180,446);
        numArr[17].setTexture(t);
        numArr[17].setTextureRect(IntRect(0,370,40,35));
        numArr[17].setPosition(window.getSize().x*0.7+220,446);

        buttonLego[9].setTexture(t);
        buttonLego[9].setTextureRect(IntRect(0,40,400,80));
        buttonLego[9].setPosition(window.getSize().x*0.7+window.getSize().x/150,532);
        numArr[18].setTexture(t);
        numArr[18].setTextureRect(IntRect(0,370,40,35));
        numArr[18].setPosition(window.getSize().x*0.7+160,615);
        numArr[19].setTexture(t);
        numArr[19].setTextureRect(IntRect(0,370,40,35));
        numArr[19].setPosition(window.getSize().x*0.7+200,614);

        Sprite arrButtonComand[3];
        arrButtonComand[0].setTexture(t);
        arrButtonComand[0].setTextureRect(IntRect(0,125,198,80));
        arrButtonComand[0].setPosition(window.getSize().x*0.7+window.getSize().x/150,660);
        arrButtonComand[1].setTexture(t);
        arrButtonComand[1].setTextureRect(IntRect(0,200,198,80));
        arrButtonComand[1].setPosition(window.getSize().x*0.7+window.getSize().x/7,660);

        Sprite arrButtonComandColor[3];
        arrButtonComandColor[0].setTexture(t);
        arrButtonComandColor[0].setTextureRect(IntRect(400,12,105,28));
        arrButtonComandColor[0].setPosition(window.getSize().x*0.85,0);
        arrButtonComandColor[1].setTexture(t);
        arrButtonComandColor[1].setTextureRect(IntRect(400,53,173,28));
        arrButtonComandColor[1].setPosition(window.getSize().x*0.85,30);
        arrButtonComandColor[2].setTexture(t);
        arrButtonComandColor[2].setTextureRect(IntRect(400,96,196,28));
        arrButtonComandColor[2].setPosition(window.getSize().x*0.85,60);

///============================///

        Texture backgroundTexture;
        backgroundTexture.setSrgb(sRgb);
        backgroundTexture.loadFromFile("resources/background.jpg");
        Sprite background(backgroundTexture);
        background.setColor(sf::Color::White);


        Texture boxTexture1_1[6],boxTexture2_1[6],boxTexture3_1[6],boxTexture4_1[6],
        boxTexture6_1[6],boxTexture8_1[6],boxTexture2_2[6],boxTexture3_2[6],boxTexture4_2[6],
        boxTexture10_2[6],boxProection[6];
        {
            boxTexture1_1[0] = LoadTexture("resources/front1.png");
            boxTexture1_1[1] = LoadTexture("resources/front1.png");
            boxTexture1_1[2] = LoadTexture("resources/front1.png");
            boxTexture1_1[3] = LoadTexture("resources/front1.png");
            boxTexture1_1[4] = LoadTexture("resources/front1.png");
            boxTexture1_1[5] = LoadTexture("resources/front1.png");

            boxTexture2_1[0] = LoadTexture("resources/front1.png");
            boxTexture2_1[1] = LoadTexture("resources/front1.png");
            boxTexture2_1[2] = LoadTexture("resources/front1.png");
            boxTexture2_1[3] = LoadTexture("resources/front1.png");
            boxTexture2_1[4] = LoadTexture("resources/front1.png");
            boxTexture2_1[5] = LoadTexture("resources/front1.png");

            boxTexture3_1[0] = LoadTexture("resources/front1.png");
            boxTexture3_1[1] = LoadTexture("resources/front1.png");
            boxTexture3_1[2] = LoadTexture("resources/front1.png");
            boxTexture3_1[3] = LoadTexture("resources/front1.png");
            boxTexture3_1[4] = LoadTexture("resources/front1.png");
            boxTexture3_1[5] = LoadTexture("resources/front1.png");

            boxTexture4_1[0] = LoadTexture("resources/front1.png");
            boxTexture4_1[1] = LoadTexture("resources/front1.png");
            boxTexture4_1[2] = LoadTexture("resources/front1.png");
            boxTexture4_1[3] = LoadTexture("resources/front1.png");
            boxTexture4_1[4] = LoadTexture("resources/front1.png");
            boxTexture4_1[5] = LoadTexture("resources/front1.png");

            boxTexture6_1[0] = LoadTexture("resources/front1.png");
            boxTexture6_1[1] = LoadTexture("resources/front1.png");
            boxTexture6_1[2] = LoadTexture("resources/front1.png");
            boxTexture6_1[3] = LoadTexture("resources/front1.png");
            boxTexture6_1[4] = LoadTexture("resources/front1.png");
            boxTexture6_1[5] = LoadTexture("resources/front1.png");

            boxTexture8_1[0] = LoadTexture("resources/front1.png");
            boxTexture8_1[1] = LoadTexture("resources/front1.png");
            boxTexture8_1[2] = LoadTexture("resources/front1.png");
            boxTexture8_1[3] = LoadTexture("resources/front1.png");
            boxTexture8_1[4] = LoadTexture("resources/front1.png");
            boxTexture8_1[5] = LoadTexture("resources/front1.png");

            boxTexture2_2[0] = LoadTexture("resources/front1.png");
            boxTexture2_2[1] = LoadTexture("resources/front1.png");
            boxTexture2_2[2] = LoadTexture("resources/front1.png");
            boxTexture2_2[3] = LoadTexture("resources/front1.png");
            boxTexture2_2[4] = LoadTexture("resources/front1.png");
            boxTexture2_2[5] = LoadTexture("resources/front1.png");

            boxTexture3_2[0] = LoadTexture("resources/front1.png");
            boxTexture3_2[1] = LoadTexture("resources/front1.png");
            boxTexture3_2[2] = LoadTexture("resources/front1.png");
            boxTexture3_2[3] = LoadTexture("resources/front1.png");
            boxTexture3_2[4] = LoadTexture("resources/front1.png");
            boxTexture3_2[5] = LoadTexture("resources/front1.png");

            boxTexture4_2[0] = LoadTexture("resources/front1.png");
            boxTexture4_2[1] = LoadTexture("resources/front1.png");
            boxTexture4_2[2] = LoadTexture("resources/front1.png");
            boxTexture4_2[3] = LoadTexture("resources/front1.png");
            boxTexture4_2[4] = LoadTexture("resources/front1.png");
            boxTexture4_2[5] = LoadTexture("resources/front1.png");

            boxTexture10_2[0] = LoadTexture("resources/front1.png");
            boxTexture10_2[1] = LoadTexture("resources/front1.png");
            boxTexture10_2[2] = LoadTexture("resources/front1.png");
            boxTexture10_2[3] = LoadTexture("resources/front1.png");
            boxTexture10_2[4] = LoadTexture("resources/front1.png");
            boxTexture10_2[5] = LoadTexture("resources/front1.png");

            boxProection[0] = LoadTexture("resources/front_p.png");
            boxProection[1] = LoadTexture("resources/back.png");
            boxProection[2] = LoadTexture("resources/right.png");
            boxProection[3] = LoadTexture("resources/left.png");
            boxProection[4] = LoadTexture("resources/bottom.png");
            boxProection[5] = LoadTexture("resources/up.png");
        }

        Texture boxTexture1_1B[6],boxTexture2_1B[6],boxTexture3_1B[6],boxTexture4_1B[6],
        boxTexture6_1B[6],boxTexture8_1B[6],boxTexture2_2B[6],boxTexture3_2B[6],boxTexture4_2B[6],
        boxTexture10_2B[6];
        {
            boxTexture1_1B[0] = LoadTexture("resources/front1.png");
            boxTexture1_1B[1] = LoadTexture("resources/front1.png");
            boxTexture1_1B[2] = LoadTexture("resources/front1.png");
            boxTexture1_1B[3] = LoadTexture("resources/front1.png");
            boxTexture1_1B[4] = LoadTexture("resources/front1.png");
            boxTexture1_1B[5] = LoadTexture("resources/front1.png");

            boxTexture2_1B[0] = LoadTexture("resources/front1.png");
            boxTexture2_1B[1] = LoadTexture("resources/front1.png");
            boxTexture2_1B[2] = LoadTexture("resources/front1.png");
            boxTexture2_1B[3] = LoadTexture("resources/front1.png");
            boxTexture2_1B[4] = LoadTexture("resources/front1.png");
            boxTexture2_1B[5] = LoadTexture("resources/front1.png");

            boxTexture3_1B[0] = LoadTexture("resources/front1.png");
            boxTexture3_1B[1] = LoadTexture("resources/front1.png");
            boxTexture3_1B[2] = LoadTexture("resources/front1.png");
            boxTexture3_1B[3] = LoadTexture("resources/front1.png");
            boxTexture3_1B[4] = LoadTexture("resources/front1.png");
            boxTexture3_1B[5] = LoadTexture("resources/front1.png");

            boxTexture4_1B[0] = LoadTexture("resources/front1.png");
            boxTexture4_1B[1] = LoadTexture("resources/front1.png");
            boxTexture4_1B[2] = LoadTexture("resources/front1.png");
            boxTexture4_1B[3] = LoadTexture("resources/front1.png");
            boxTexture4_1B[4] = LoadTexture("resources/front1.png");
            boxTexture4_1B[5] = LoadTexture("resources/front1.png");

            boxTexture6_1B[0] = LoadTexture("resources/front1.png");
            boxTexture6_1B[1] = LoadTexture("resources/front1.png");
            boxTexture6_1B[2] = LoadTexture("resources/front1.png");
            boxTexture6_1B[3] = LoadTexture("resources/front1.png");
            boxTexture6_1B[4] = LoadTexture("resources/front1.png");
            boxTexture6_1B[5] = LoadTexture("resources/front1.png");

            boxTexture8_1B[0] = LoadTexture("resources/front1.png");
            boxTexture8_1B[1] = LoadTexture("resources/front1.png");
            boxTexture8_1B[2] = LoadTexture("resources/front1.png");
            boxTexture8_1B[3] = LoadTexture("resources/front1.png");
            boxTexture8_1B[4] = LoadTexture("resources/front1.png");
            boxTexture8_1B[5] = LoadTexture("resources/front1.png");

            boxTexture2_2B[0] = LoadTexture("resources/front1.png");
            boxTexture2_2B[1] = LoadTexture("resources/front1.png");
            boxTexture2_2B[2] = LoadTexture("resources/front1.png");
            boxTexture2_2B[3] = LoadTexture("resources/front1.png");
            boxTexture2_2B[4] = LoadTexture("resources/front1.png");
            boxTexture2_2B[5] = LoadTexture("resources/front1.png");

            boxTexture3_2B[0] = LoadTexture("resources/front1.png");
            boxTexture3_2B[1] = LoadTexture("resources/front1.png");
            boxTexture3_2B[2] = LoadTexture("resources/front1.png");
            boxTexture3_2B[3] = LoadTexture("resources/front1.png");
            boxTexture3_2B[4] = LoadTexture("resources/front1.png");
            boxTexture3_2B[5] = LoadTexture("resources/front1.png");

            boxTexture4_2B[0] = LoadTexture("resources/front1.png");
            boxTexture4_2B[1] = LoadTexture("resources/front1.png");
            boxTexture4_2B[2] = LoadTexture("resources/front1.png");
            boxTexture4_2B[3] = LoadTexture("resources/front1.png");
            boxTexture4_2B[4] = LoadTexture("resources/front1.png");
            boxTexture4_2B[5] = LoadTexture("resources/front1.png");

            boxTexture10_2B[0] = LoadTexture("resources/front1.png");
            boxTexture10_2B[1] = LoadTexture("resources/front1.png");
            boxTexture10_2B[2] = LoadTexture("resources/front1.png");
            boxTexture10_2B[3] = LoadTexture("resources/front1.png");
            boxTexture10_2B[4] = LoadTexture("resources/front1.png");
            boxTexture10_2B[5] = LoadTexture("resources/front1.png");
        }

        Texture boxTextureSkyBox[6];
            boxTextureSkyBox[0] = LoadTexture("resources/1.png");
            boxTextureSkyBox[1] = LoadTexture("resources/1.png");
            boxTextureSkyBox[2] = LoadTexture("resources/1.png");
            boxTextureSkyBox[3] = LoadTexture("resources/1.png");
            boxTextureSkyBox[4] = LoadTexture("resources/1.png");
            boxTextureSkyBox[5] = LoadTexture("resources/1.png");

        // Enable Z-buffer read and write
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearDepth(1.f);
        glDisable(GL_LIGHTING);// отключение освещения
        //glViewport(0, 0, window.getSize().x, window.getSize().y);//область на экране , где выводится изображение
        glViewport(0,0, window.getSize().x*0.7, window.getSize().y);
        glMatrixMode(GL_PROJECTION);// Setup a perspective projection
        glLoadIdentity();
        GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
        glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 1000000.f);
        glEnable(GL_TEXTURE_2D);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        //glOrtho(0,window.getSize().x,0,window.getSize().y,-1.0,1.0);
        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

        //glPolygonMode(GL_FRONT_AND_BACK	,GL_LINE);
        // Create a clock for measuring the time elapsed


        Clock clock;

        // Flag to track whether mipmapping is currently enabled
        bool mipmapEnabled = true;
        // Start game loop
        while (window.isOpen())
        {

            // Process events
            Event event;
            while (window.pollEvent(event))
            {
                //ImGui::SFML::ProcessEvent(event);

                if (event.type==Event::MouseButtonPressed)
                {
                if (event.mouseButton.button == sf::Mouse::Left)
                    {

                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+5*window.getSize().x/150)&&
                       (event.mouseButton.y>=window.getSize().y/80)&&
                       (event.mouseButton.y<=5*window.getSize().y/80))
                    {
                        buttonLego[0].setColor(sf::Color::Green);
                        if (arrCubes[0]<99)arrCubes[0]+=1;
                        numArr[0].setTextureRect(spriteTransforamtNumberDoz(arrCubes[0]));
                        numArr[1].setTextureRect(spriteTransforamtNumberUnit(arrCubes[0]));
                    }

                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+9*window.getSize().x/150)&&
                       (event.mouseButton.y>=5*window.getSize().y/80)&&
                       (event.mouseButton.y<=9*window.getSize().y/80))
                    {
                        buttonLego[1].setColor(sf::Color::Green);
                        if (arrCubes[1]<99)arrCubes[1]+=1;
                        numArr[2].setTextureRect(spriteTransforamtNumberDoz(arrCubes[1]));
                        numArr[3].setTextureRect(spriteTransforamtNumberUnit(arrCubes[1]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+13*window.getSize().x/150)&&
                       (event.mouseButton.y>=10*window.getSize().y/80)&&
                       (event.mouseButton.y<=14*window.getSize().y/80))
                    {
                        buttonLego[2].setColor(sf::Color::Green);
                        if (arrCubes[2]<99)arrCubes[2]+=1;
                        numArr[4].setTextureRect(spriteTransforamtNumberDoz(arrCubes[2]));
                        numArr[5].setTextureRect(spriteTransforamtNumberUnit(arrCubes[2]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+17*window.getSize().x/150)&&
                       (event.mouseButton.y>=14*window.getSize().y/80)&&
                       (event.mouseButton.y<=18*window.getSize().y/80))
                    {
                        buttonLego[3].setColor(sf::Color::Green);
                        if (arrCubes[3]<99)arrCubes[3]+=1;
                        numArr[6].setTextureRect(spriteTransforamtNumberDoz(arrCubes[3]));
                        numArr[7].setTextureRect(spriteTransforamtNumberUnit(arrCubes[3]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+25*window.getSize().x/150)&&
                       (event.mouseButton.y>=19*window.getSize().y/80)&&
                       (event.mouseButton.y<=23*window.getSize().y/80))
                    {
                        buttonLego[4].setColor(sf::Color::Green);
                        if (arrCubes[4]<99)arrCubes[4]+=1;
                        numArr[8].setTextureRect(spriteTransforamtNumberDoz(arrCubes[4]));
                        numArr[9].setTextureRect(spriteTransforamtNumberUnit(arrCubes[4]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+33*window.getSize().x/150)&&
                       (event.mouseButton.y>=23*window.getSize().y/80)&&
                       (event.mouseButton.y<=27*window.getSize().y/80))
                    {
                        buttonLego[5].setColor(sf::Color::Green);
                        if (arrCubes[5]<99)arrCubes[5]+=1;
                        numArr[10].setTextureRect(spriteTransforamtNumberDoz(arrCubes[5]));
                        numArr[11].setTextureRect(spriteTransforamtNumberUnit(arrCubes[5]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+28*window.getSize().x/150)&&
                       (event.mouseButton.y>=28*window.getSize().y/80)&&
                       (event.mouseButton.y<=36*window.getSize().y/80))
                    {
                        buttonLego[6].setColor(sf::Color::Green);
                        if (arrCubes[6]<99)arrCubes[6]+=1;
                        numArr[12].setTextureRect(spriteTransforamtNumberDoz(arrCubes[6]));
                        numArr[13].setTextureRect(spriteTransforamtNumberUnit(arrCubes[6]));
                    }

                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+37*window.getSize().x/150)&&
                       (event.mouseButton.y>=36*window.getSize().y/80)&&
                       (event.mouseButton.y<=44*window.getSize().y/80))
                    {
                        buttonLego[7].setColor(sf::Color::Green);
                        if (arrCubes[7]<99)arrCubes[7]+=1;
                        numArr[14].setTextureRect(spriteTransforamtNumberDoz(arrCubes[7]));
                        numArr[15].setTextureRect(spriteTransforamtNumberUnit(arrCubes[7]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+45*window.getSize().x/150)&&
                       (event.mouseButton.y>=45*window.getSize().y/80)&&
                       (event.mouseButton.y<=53*window.getSize().y/80))
                    {
                        buttonLego[8].setColor(sf::Color::Green);
                        if (arrCubes[8]<99)arrCubes[8]+=1;
                        numArr[16].setTextureRect(spriteTransforamtNumberDoz(arrCubes[8]));
                        numArr[17].setTextureRect(spriteTransforamtNumberUnit(arrCubes[8]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+54*window.getSize().x/150)&&
                       (event.mouseButton.y>=53*window.getSize().y/80)&&
                       (event.mouseButton.y<=61*window.getSize().y/80))
                    {
                        buttonLego[9].setColor(sf::Color::Green);
                        if (arrCubes[9]<99)arrCubes[9]+=1;
                        numArr[18].setTextureRect(spriteTransforamtNumberDoz(arrCubes[9]));
                        numArr[19].setTextureRect(spriteTransforamtNumberUnit(arrCubes[9]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+21*window.getSize().x/150)&&
                       (event.mouseButton.y>=66*window.getSize().y/80)&&
                       (event.mouseButton.y<=74*window.getSize().y/80))
                    {
                        arrButtonComand[0].setColor(sf::Color::Yellow);

                        SumCubes=arrCubes[0]+arrCubes[1]+arrCubes[2]+arrCubes[3]+arrCubes[4]+arrCubes[5]
                            +arrCubes[6]+arrCubes[7]+arrCubes[8]+arrCubes[9];

                        cout << SumCubes << "\n";

                        if(SumCubes!=0){
                                distribution_cubes(arrDataCubes,arrCubes[0],arrCubes[1],arrCubes[2],
                                                   arrCubes[3],arrCubes[4],arrCubes[5],
                                                   arrCubes[6],arrCubes[7],arrCubes[8],arrCubes[9]);
                            marA=true;
                        }else marA=false;
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+22*window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+43*window.getSize().x/150)&&
                       (event.mouseButton.y>=66*window.getSize().y/80)&&
                       (event.mouseButton.y<=74*window.getSize().y/80))
                    {
                        arrButtonComand[1].setColor(sf::Color::Yellow);
                        marScreen=true;
                        markerScreen=0;
                        window.setTitle("PRESS SPACE!!!!!");
                    }
                    ///===================
                    if((event.mouseButton.x>=window.getSize().x*0.85)
                       &&(event.mouseButton.x<=window.getSize().x*0.92)&&
                       (event.mouseButton.y>=0)&&
                       (event.mouseButton.y<=window.getSize().y*0.035))
                    {
                        arrButtonComandColor[0].setColor(sf::Color::Yellow);
                        arrButtonComandColor[1].setColor(sf::Color::White);
                        arrButtonComandColor[2].setColor(sf::Color::White);

                        for(int i = 0; i<SumCubes;i++)
                                   {arrCubeColor[i].r=1;
                                    arrCubeColor[i].g=1;
                                    arrCubeColor[i].b=1;}
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.85)
                       &&(event.mouseButton.x<=window.getSize().x*0.9653)&&
                       (event.mouseButton.y>=window.getSize().y*0.0375)&&
                       (event.mouseButton.y<=window.getSize().y*0.0725))
                    {
                        arrButtonComandColor[1].setColor(sf::Color::Yellow);
                        arrButtonComandColor[0].setColor(sf::Color::White);
                        arrButtonComandColor[2].setColor(sf::Color::White);

                        for(int i = 0; i<SumCubes;i++)
                                {
                                    if(rand()%2==0){
                                        arrCubeColor[i].r=1;
                                        arrCubeColor[i].g=1;
                                        arrCubeColor[i].b=1;
                                    }else{
                                        arrCubeColor[i].r=1;
                                        arrCubeColor[i].g=0;
                                        arrCubeColor[i].b=0;
                                    }

                                }
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.85)
                       &&(event.mouseButton.x<=window.getSize().x*0.98066)&&
                       (event.mouseButton.y>=window.getSize().y*0.075)&&
                       (event.mouseButton.y<=window.getSize().y*0.11))
                    {
                        arrButtonComandColor[2].setColor(sf::Color::Yellow);
                        arrButtonComandColor[1].setColor(sf::Color::White);
                        arrButtonComandColor[0].setColor(sf::Color::White);

                        for(int i = 0; i<SumCubes;i++)
                                {
                                    if(rand()%2==0){
                                        arrCubeColor[i].r=0;
                                        arrCubeColor[i].g=0.25;
                                        arrCubeColor[i].b=0;
                                    }else{
                                        arrCubeColor[i].r=0.05;
                                        arrCubeColor[i].g=0;
                                        arrCubeColor[i].b=0;
                                    }

                                }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (event.mouseButton.button == sf::Mouse::Right){

                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+5*window.getSize().x/150)&&
                       (event.mouseButton.y>=window.getSize().y/80)&&
                       (event.mouseButton.y<=5*window.getSize().y/80))
                    {
                        buttonLego[0].setColor(sf::Color::Red);
                        if (arrCubes[0]>0)arrCubes[0]--;
                        numArr[0].setTextureRect(spriteTransforamtNumberDoz(arrCubes[0]));
                        numArr[1].setTextureRect(spriteTransforamtNumberUnit(arrCubes[0]));
                    }

                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+9*window.getSize().x/150)&&
                       (event.mouseButton.y>=5*window.getSize().y/80)&&
                       (event.mouseButton.y<=9*window.getSize().y/80))
                    {
                        buttonLego[1].setColor(sf::Color::Red);
                        if (arrCubes[1]>0)arrCubes[1]--;
                        numArr[2].setTextureRect(spriteTransforamtNumberDoz(arrCubes[1]));
                        numArr[3].setTextureRect(spriteTransforamtNumberUnit(arrCubes[1]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+13*window.getSize().x/150)&&
                       (event.mouseButton.y>=10*window.getSize().y/80)&&
                       (event.mouseButton.y<=14*window.getSize().y/80))
                    {
                        buttonLego[2].setColor(sf::Color::Red);
                        if (arrCubes[2]>0)arrCubes[2]--;
                        numArr[4].setTextureRect(spriteTransforamtNumberDoz(arrCubes[2]));
                        numArr[5].setTextureRect(spriteTransforamtNumberUnit(arrCubes[2]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+17*window.getSize().x/150)&&
                       (event.mouseButton.y>=14*window.getSize().y/80)&&
                       (event.mouseButton.y<=18*window.getSize().y/80))
                    {
                        buttonLego[3].setColor(sf::Color::Red);
                        if (arrCubes[3]>0)arrCubes[3]--;
                        numArr[6].setTextureRect(spriteTransforamtNumberDoz(arrCubes[3]));
                        numArr[7].setTextureRect(spriteTransforamtNumberUnit(arrCubes[3]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+25*window.getSize().x/150)&&
                       (event.mouseButton.y>=19*window.getSize().y/80)&&
                       (event.mouseButton.y<=23*window.getSize().y/80))
                    {
                        buttonLego[4].setColor(sf::Color::Red);
                        if (arrCubes[4]>0)arrCubes[4]--;
                        numArr[8].setTextureRect(spriteTransforamtNumberDoz(arrCubes[4]));
                        numArr[9].setTextureRect(spriteTransforamtNumberUnit(arrCubes[4]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+33*window.getSize().x/150)&&
                       (event.mouseButton.y>=23*window.getSize().y/80)&&
                       (event.mouseButton.y<=27*window.getSize().y/80))
                    {
                        buttonLego[5].setColor(sf::Color::Red);
                        if (arrCubes[5]>0)arrCubes[5]--;
                        numArr[10].setTextureRect(spriteTransforamtNumberDoz(arrCubes[5]));
                        numArr[11].setTextureRect(spriteTransforamtNumberUnit(arrCubes[5]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+28*window.getSize().x/150)&&
                       (event.mouseButton.y>=28*window.getSize().y/80)&&
                       (event.mouseButton.y<=36*window.getSize().y/80))
                    {
                        buttonLego[6].setColor(sf::Color::Red);
                        if (arrCubes[6]>0)arrCubes[6]--;
                        numArr[12].setTextureRect(spriteTransforamtNumberDoz(arrCubes[6]));
                        numArr[13].setTextureRect(spriteTransforamtNumberUnit(arrCubes[6]));
                    }

                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+37*window.getSize().x/150)&&
                       (event.mouseButton.y>=36*window.getSize().y/80)&&
                       (event.mouseButton.y<=44*window.getSize().y/80))
                    {
                        buttonLego[7].setColor(sf::Color::Red);
                        if (arrCubes[7]>0)arrCubes[7]--;
                        numArr[14].setTextureRect(spriteTransforamtNumberDoz(arrCubes[7]));
                        numArr[15].setTextureRect(spriteTransforamtNumberUnit(arrCubes[7]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+45*window.getSize().x/150)&&
                       (event.mouseButton.y>=45*window.getSize().y/80)&&
                       (event.mouseButton.y<=53*window.getSize().y/80))
                    {
                        buttonLego[8].setColor(sf::Color::Red);
                        if (arrCubes[8]>0)arrCubes[8]--;
                        numArr[16].setTextureRect(spriteTransforamtNumberDoz(arrCubes[8]));
                        numArr[17].setTextureRect(spriteTransforamtNumberUnit(arrCubes[8]));
                    }
                    if((event.mouseButton.x>=window.getSize().x*0.7+window.getSize().x/150)
                       &&(event.mouseButton.x<=window.getSize().x*0.7+54*window.getSize().x/150)&&
                       (event.mouseButton.y>=53*window.getSize().y/80)&&
                       (event.mouseButton.y<=61*window.getSize().y/80))
                    {
                        buttonLego[9].setColor(sf::Color::Red);
                        if (arrCubes[9]>0)arrCubes[9]--;
                        numArr[18].setTextureRect(spriteTransforamtNumberDoz(arrCubes[9]));
                        numArr[19].setTextureRect(spriteTransforamtNumberUnit(arrCubes[9]));
                    }
                }
                }
            }
                if (event.type==Event::MouseButtonReleased)
                {
                    buttonLego[0].setColor(sf::Color::White);
                    buttonLego[1].setColor(sf::Color::White);
                    buttonLego[2].setColor(sf::Color::White);
                    buttonLego[3].setColor(sf::Color::White);
                    buttonLego[4].setColor(sf::Color::White);
                    buttonLego[5].setColor(sf::Color::White);
                    buttonLego[6].setColor(sf::Color::White);
                    buttonLego[7].setColor(sf::Color::White);
                    buttonLego[8].setColor(sf::Color::White);
                    buttonLego[9].setColor(sf::Color::White);
                    arrButtonComand[0].setColor(sf::Color::White);
                    arrButtonComand[1].setColor(sf::Color::White);

                }

                // Close window: exit
                if (event.type == Event::Closed)
                {
                    exit = true;
                    window.close();
                }

                // Space key: toggle sRGB conversion
                if ((event.type == Event::KeyPressed) )
                {
                    switch(event.key.code)
                    {
                        case Keyboard::Escape:
                            {
                                exit = true;
                                window.close();
                            }
                        case Keyboard::Down:
                            {
                                rotR=false;
                                rotateX-=10;
                                break;
                            }
                        case Keyboard::Up:
                            {
                                rotR=false;
                                rotateX+=10;
                                break;
                            }

                        case Keyboard::Right:
                            {
                                rotR=false;
                                rotateY+=10;
                                break;
                            }
                        case Keyboard::Left:
                            {
                                rotR=false;
                                rotateY-=10;
                                break;
                            }
                        case Keyboard::Dash:
                            {
                                if((marScreen)&&(scaledCube > 0))
                                    scaledCube -= 0.05;
                                else
                                    if (R<1000)
                                    {
                                        rotR=true;
                                        R+=1;
                                    }
                                break;
                            }
                        case Keyboard::Equal:
                            {
                                if(marScreen)
                                    scaledCube += 0.05;
                                else
                                    if (R>30)
                                    {
                                        rotR=true;
                                        R-=1;
                                    }
                                break;
                            }
                        case Keyboard::Space:
                            {
                                markerScreen=10;
                                window.setTitle("LEGO animals");
                                break;
                            }
                        case Keyboard::A:
                            {
                                for(int i = 0; i<SumCubes;i++)
                                   {arrCubeColor[i].r=1;
                                    arrCubeColor[i].g=1;
                                    arrCubeColor[i].b=1;}
                                break;
                            }
                        case Keyboard::S:
                            {
                                for(int i = 0; i<SumCubes;i++)
                                {
                                    if(rand()%2==0){
                                        arrCubeColor[i].r=1;
                                        arrCubeColor[i].g=1;
                                        arrCubeColor[i].b=1;
                                    }else{
                                        arrCubeColor[i].r=1;
                                        arrCubeColor[i].g=0;
                                        arrCubeColor[i].b=0;
                                    }

                                }
                                break;
                            }
                        case Keyboard::D:
                            {
                                for(int i = 0; i<SumCubes;i++)
                                {
                                    if(rand()%2==0){
                                        arrCubeColor[i].r=0;
                                        arrCubeColor[i].g=0.25;
                                        arrCubeColor[i].b=0;
                                    }else{
                                        arrCubeColor[i].r=0.05;
                                        arrCubeColor[i].g=0;
                                        arrCubeColor[i].b=0;
                                    }

                                }
                                break;
                            }
                        case Keyboard::T:
                            {
                                distribution_cubes(arrDataCubes,0,0,0,0,0,0,0,0,0,990);
                                SumCubes = 990;
                                break;
                            }
                        case Keyboard::F1:
                            {
                                system("notepad resources/справка.txt");
                                break;
                            }
                    }

                }

                // Adjust the viewport when the window is resized
                if (event.type == Event::Resized)
                    glViewport(0, 0, event.size.width*0.7, event.size.height);
            }

            float A=10.f,B=20.f;

            window.pushGLStates();
            window.draw(background);
            window.popGLStates();

            window.pushGLStates();

            window.draw(buttonLego[0]);
            window.draw(buttonLego[1]);
            window.draw(buttonLego[2]);
            window.draw(buttonLego[3]);
            window.draw(buttonLego[4]);
            window.draw(buttonLego[5]);
            window.draw(buttonLego[6]);
            window.draw(buttonLego[7]);
            window.draw(buttonLego[8]);
            window.draw(buttonLego[9]);

            window.draw(arrButtonComand[0]);
            window.draw(arrButtonComand[1]);

            window.draw(arrButtonComandColor[0]);
            window.draw(arrButtonComandColor[1]);
            window.draw(arrButtonComandColor[2]);

            window.draw(numArr[0]);
            window.draw(numArr[1]);
            window.draw(numArr[2]);
            window.draw(numArr[3]);
            window.draw(numArr[4]);
            window.draw(numArr[5]);
            window.draw(numArr[6]);
            window.draw(numArr[7]);
            window.draw(numArr[8]);
            window.draw(numArr[9]);
            window.draw(numArr[10]);
            window.draw(numArr[11]);
            window.draw(numArr[12]);
            window.draw(numArr[13]);
            window.draw(numArr[14]);
            window.draw(numArr[15]);
            window.draw(numArr[16]);
            window.draw(numArr[17]);
            window.draw(numArr[18]);
            window.draw(numArr[19]);
            window.popGLStates();

            // Clear the depth buffer
            glClear(GL_DEPTH_BUFFER_BIT);
/**
            ImGui::SFML::Update(window, clock.restart());

            ImGui::Begin("Nastroiki:"); // создаём окно
            ImGui::NewFrame;
            if (ImGui::TreeNode("kolichestvo kubicov:"))
            {
                static int vec1i[1] = {1};
                ImGui::DragInt("shtuk",vec1i,1,1,125);
                if(ImGui::Button("sozdat",{100,30}));
                ImGui::TreePop();
            }

            ImGui::End(); // end window

            ImGui::SFML::Render(window);
**/

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

           // gluPerspective(60.0, 1500/800, 0, 100);

            glTranslatef(0, 0, 0);

            if(!rotR){
            angleX=rotateX/(PI*100);
            angleY=rotateY/(PI*100);

            //angleX=rotateX/(PI*R);
            //angleY=rotateY/(PI*R);

            if(angleX>0)
            {
                angleX=0;
                rotateX-=10;
            }
            if(angleX<-PI)
            {
                angleX=-PI;
                rotateX+=10;
            }

            angleX=rotateX/(PI*100);
            angleY=rotateY/(PI*100);}
            //angleX=rotateX/(PI*R);
            //angleY=rotateY/(PI*R);}


            //--вращение камеры--//
            //------
            if(marScreen) gluLookAt(0,0,0,0,0,0,0.0,1.0,0.0);
            else gluLookAt(sin(angleX)*sin(angleY)*R,cos(angleX)*R,sin(angleX)*cos(angleY)*R,0,0,0,0.0,1.0,0.0);

            //std::cout<<"{"<<sin(angleX)*sin(angleY)*R<<"|"<<cos(angleX)*R<<"|"<<sin(angleX)*cos(angleY)*R<<"}";
            //gluLookAt(0,0,0,(sin(angleX)*sin(angleY)*R,cos(angleX)*R,sin(angleX)*cos(angleY)*R,0.0,1.0,0.0);

            glTranslatef(0, 0, 0);
            createBox(boxTextureSkyBox,5000.f,5000.f);
            glTranslatef(0, 0, 0);



            if(marScreen)
            {
            if (markerScreen==10)
                {
                    marScreen=false;
                    HWND hwind = FindWindow(NULL,"serg");
                    SaveScreen(hwind);

                    arrButtonComand[0].setTexture(t);
                    arrButtonComand[1].setTexture(t);
                }
            else{

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glOrtho(-200, 200, -200, 200, 200, 10000000);
            glLineWidth (100);
            //window.setSize(Vector2u(scrX,scrY));
            //window.setPosition(Vector2i(0,0));

            glBegin(GL_QUADS);
            glColor3d(1,1,1);
            glVertex3d(-5000,-5000,5000);
            glColor3d(1,1,1);
            glVertex3d(5000,-5000,5000);
            glColor3d(1,1,1);
            glVertex3d(5000,5000,5000);
            glColor3d(1,1,1);
            glVertex3d(-5000,5000,5000);
            glEnd();

            glLineWidth(1);
            glBegin(GL_LINES);
            glColor3d(1,0,0);
            glVertex3d(-5000,0,500);
            glVertex3d(5000,0,500);
            glVertex3d(0,-5000,500);
            glVertex3d(0,5000,500);
            glEnd();
            glColor3d(1,1,1);

            arrButtonComand[0].setTexture(backgroundTexture);
            arrButtonComand[1].setTexture(backgroundTexture);

            glScaled(2.5, 2.5, 2.5);
            glColor3d(1,1,1);
            createPerspectivBox(boxProection,-1,-10,100);///#################
            //glScaled(1/2, 1/2, 1/2);
            glScaled(1/2.5, 1/2.5, 1/2.5);

            glScaled(scaledCube,scaledCube,scaledCube);

            //glScalef((maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))/98,
                     //(maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))/98,
                     //(maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))/98);
            int translateX,translateY,translateZ=0;
                if(marA)
                    for(int j=0;j<4;j++)
                {
                    switch(j){
                        case(0):{
                            translateX=-(minCubeX(arrDataCubes,SumCubes)) + 10;translateY=-(minCube(arrDataCubes,SumCubes))+10;translateZ=500;
                            break;
                        }
                        case(1):{
                            translateX=-(minCubeX(arrDataCubes,SumCubes)) + 10;translateY=-(minCube(arrDataCubes,SumCubes))+10;translateZ=-500;
                            glRotated(180,0,1,0);
                            break;
                        }
                        case(2):{
                            glRotated(-180,0,1,0);
                            translateX=(minCubeX(arrDataCubes,SumCubes)) - 10;
                            translateX=-500;translateY=-abs(maxCube(arrDataCubes,SumCubes)+10)-10;translateZ=-(minCubeZ(arrDataCubes,SumCubes)) + 10;
                            glRotated(90,0,1,0);
                            break;
                        }
                        case(3):{
                            translateX=500;translateY=-abs(maxCube(arrDataCubes,SumCubes)+10)-10;translateZ=-(minCubeZ(arrDataCubes,SumCubes)) + 10;
                            glRotated(180,0,1,0);
                            break;
                        }
                    }



            for(int i=0 ;i<(SumCubes);i++)
            {
                //if (i%2==0)glColor3d(1,0,0);
                //else glColor3d(1,1,1);
                glColor3d(arrCubeColor[i].r,arrCubeColor[i].g,arrCubeColor[i].b);

                switch(arrDataCubes[i].type)
                {
                    case(0):{createBox1_1(boxTexture1_1B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ);break;}
                    case(1):{createBox2_1(boxTexture2_1B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(2):{createBox3_1(boxTexture3_1B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(3):{createBox4_1(boxTexture4_1B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(4):{createBox6_1(boxTexture6_1B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(5):{createBox8_1(boxTexture8_1B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(6):{createBox2_2(boxTexture2_2B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ);break;}
                    case(7):{createBox3_2(boxTexture3_2B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(8):{createBox4_2(boxTexture4_2B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                    case(9):{createBox10_2(boxTexture10_2B,arrDataCubes[i].x+translateX,arrDataCubes[i].y+translateY,arrDataCubes[i].z+translateZ,arrDataCubes[i].rotateCube);break;}
                }
            }
            glColor3d(1,1,1);

            }
            if(scaledCube != 0)
                glScaled((1/scaledCube),(1/scaledCube),(1/scaledCube));


                //glScalef(98/(maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes)+1),
                     //98/(maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes)+1),
                     //98/(maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))+1);
                //if (maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes)!=0)
           //glScaled(1/(abs(maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))/98),
                    // 1/abs((maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))/98),
                    // 1/abs((maxCube(arrDataCubes,SumCubes)-minCube(arrDataCubes,SumCubes))/98));
               /* if(marA)

                for(int i=0 ;i<SumCubes;i++)
            {

                switch(arrDataCubes[i].type)
                {
                    case(0):{createBox1_1(boxTexture1_1,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z);
                    createBox1_1(boxTexture1_1,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z);
                    createBox1_1(boxTexture1_1,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z);
                    createBox1_1(boxTexture1_1,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z);break;}
                    case(1):{createBox2_1(boxTexture2_1,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox2_1(boxTexture2_1,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox2_1(boxTexture2_1,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox2_1(boxTexture2_1,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(2):{createBox3_1(boxTexture3_1,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox3_1(boxTexture3_1,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox3_1(boxTexture3_1,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox3_1(boxTexture3_1,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(3):{createBox4_1(boxTexture4_1,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox4_1(boxTexture4_1,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox4_1(boxTexture4_1,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox4_1(boxTexture4_1,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(4):{createBox6_1(boxTexture6_1,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox6_1(boxTexture6_1,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox6_1(boxTexture6_1,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox6_1(boxTexture6_1,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(5):{createBox8_1(boxTexture8_1,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox8_1(boxTexture8_1,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox8_1(boxTexture8_1,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox8_1(boxTexture8_1,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(6):{createBox2_2(boxTexture2_2,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z);
                    createBox2_2(boxTexture2_2,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z);
                    createBox2_2(boxTexture2_2,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z);
                    createBox2_2(boxTexture2_2,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z);break;}
                    case(7):{createBox3_2(boxTexture3_2,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox3_2(boxTexture3_2,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox3_2(boxTexture3_2,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox3_2(boxTexture3_2,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(8):{createBox4_2(boxTexture4_2,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox4_2(boxTexture4_2,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox4_2(boxTexture4_2,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox4_2(boxTexture4_2,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(9):{createBox10_2(boxTexture10_2,arrDataCubes[i].x-100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox10_2(boxTexture10_2,arrDataCubes[i].x-100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox10_2(boxTexture10_2,arrDataCubes[i].x+100,arrDataCubes[i].y-100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);
                    createBox10_2(boxTexture10_2,arrDataCubes[i].x+100,arrDataCubes[i].y+100,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                }
            }*/
            //markerScreen++;
            }
            }
            else
            if(marA)
            {
                for(int i=0 ;i<(SumCubes);i++)
            {
                //if (i%2==0)glColor3d(0,0.25,0);
                //else glColor3d(0.05,0,0);
                glColor3d(arrCubeColor[i].r,arrCubeColor[i].g,arrCubeColor[i].b);

                switch(arrDataCubes[i].type)
                {
                    case(0):{createBox1_1(boxTexture1_1B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z);break;}
                    case(1):{createBox2_1(boxTexture2_1B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(2):{createBox3_1(boxTexture3_1B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(3):{createBox4_1(boxTexture4_1B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(4):{createBox6_1(boxTexture6_1B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(5):{createBox8_1(boxTexture6_1B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(6):{createBox2_2(boxTexture2_2B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z);break;}
                    case(7):{createBox3_2(boxTexture3_2B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(8):{createBox4_2(boxTexture4_2B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                    case(9):{createBox10_2(boxTexture10_2B,arrDataCubes[i].x,arrDataCubes[i].y,arrDataCubes[i].z,arrDataCubes[i].rotateCube);break;}
                }
            }

            }


            glLineWidth(1);
            glBegin(GL_LINES);

            glColor3d(1,0,0);
            glVertex3d(-5000,0,0);
            glVertex3d(5000,0,0);
            glVertex3d(0,-5000,0);
            glVertex3d(0,5000,0);
            glVertex3d(0,0,-5000);
            glVertex3d(0,0,5000);

            glEnd();

            glColor3d(1,1,1);

            // Finally, display the rendered frame on screen
            window.display();

        }
    }
    //ImGui::SFML::Shutdown();


    return EXIT_SUCCESS;
}
