#include"bla.h"
int f_hlp,punkt,pozic_x,pozic_y,N_HELP;
//--------------------------------------------------------------
/****************************************\
* Процедура подготовки экрана для вывода *
* текстов файла помощи help.txt.         *
\****************************************/
void h()
{
	nom_func("101");
	clscreen(); setfillstyle(SOLID_FILL,8);
  bar(0,0,getmaxx(),getmaxy());
  rama();
  help=1;
	return;
}
//------------------------------------------------------------------
/***********************************\
* Процедура очистки экрана и вывода *
* большой рамки экрана помощи       *
\***********************************/
void rama()
{
	nom_func("259");
	setfillstyle(SOLID_FILL,8);
	bar(0,0,639,479);
	setcolor(3);
	rectangle(2,2,638,478);
	rectangle(4,4,636,476);
	return;
}
//------------------------------------------------
void vio_vi()
{
	int step=0;
	char bukva[2]="";
	nom_func("392");
	if(klaval==27)
	{
		if(help==2)
		{
			help=0;
			setusercharsize(1,1,1,1);
			end_help();
			sbros_all();
			return;
		}
		if((help!=4)&&(help!=1)){if(f_hlp>0)close(f_hlp);f_hlp=0;}
		punkt=0;
		help=1;
	}
	if((help==3)&&(klaval==0))
	{
		klaval=getch();
		if(klaval==81)//PgDn
		{
		 cleardevice();
		 rama();
		 moveto(5,10);
		 step=punkt;
		 setcolor(15);
		 goto m1;
		}
		if(klaval==73)//PgDn
		{
		 cleardevice();
		 rama();
		 moveto(5,10);
		 step=0;
		 help=2;
		 setcolor(15);
		 lseek(f_hlp,0,0);
		 goto m1;
		}
	}
	if((help==4)&&(klaval==0))
	{
		klaval=getch();
		if(klaval==73)//PgDn
		{
		 cleardevice();
		 rama();
		 moveto(5,10);
		 step=0;
		 help=2;
		 setcolor(15);
		 f_hlp=open("dat\\help.txt",O_RDONLY);
		 lseek(f_hlp,0,0);
		 goto m1;
		}
	}
	if(help==1)
	{
		cleardevice();
		rama();
		N_HELP=0;
		f_hlp=open("dat\\help.txt",O_RDONLY);
		if(f_hlp<0){if(f_hlp>0)close(f_hlp);f_hlp=0;}
		moveto(5,10);
		bukva[1]=0;
		setcolor(12);setusercharsize(3,2,3,2);
		while((bukva[0]!='@')&&!eof(f_hlp))
		{
			read(f_hlp,bukva,1);
			if(bukva[0]==0xa)
			{
				moveto(5,gety()+16);
				N_HELP++;
			}
			if(bukva[0]!='@')outtext(bukva);
		}
		moveto(40,gety()+30);
		outtext("Введите номер пункта ");
		pozic_x=getx();
		pozic_y=gety();
		help=2;
		klaval=-1;
		return;
	}
	if(help==2)
	{
		if(klaval==-1)return;
		if(klaval==8)
		{
			if(punkt>9)punkt=punkt/10;
			else punkt=0;
			moveto(pozic_x,pozic_y);
			bar(pozic_x,pozic_y,pozic_x+30,pozic_y+15);
			if(punkt>0)
			{
				itoa(punkt,bukva,10);
				outtext(bukva);
			}
			return;
		}
		if(klaval==13)goto m0;
		if((klaval<48)||(klaval>57))return;
		itoa(klaval-48,bukva,10);
		outtext(bukva);
		klaval=klaval-48;
		punkt=punkt*10+klaval;
		if((punkt>N_HELP)||(punkt<=0))goto m0;
		return;
m0:
		if((punkt>N_HELP)||(punkt<=0))
		{
			help=1;
			punkt=0;
			klaval=-1;
			if(f_hlp>0)close(f_hlp);
			f_hlp=0;
			return;
		}
		cleardevice();
		rama();
		moveto(5,10);
		bukva[1]=0;
		setcolor(15);setusercharsize(1,1,1,1);
		step=0;
m1: if(help==3)
		{
			outtextxy(620,20,"");
			outtextxy(600,30,"PgUp");
		}
		while(!eof(f_hlp))
		{
			read(f_hlp,bukva,1);
			if(bukva[0]=='#')step++;
			if(step==punkt)
			{
				if(gety()>460){help=3;break;}
				if(bukva[0]==0xa)moveto(5,gety()+16);
				if((bukva[0]!='@')&&(bukva[0]!='#'))outtext(bukva);
			}
			if(step>punkt){help=4;break;}
		}
	}
	if(help==3)
	{
		outtextxy(600,450,"PgDn");
		outtextxy(620,460,"");
		return;
	}
m2:
	if(help==4){if(f_hlp>0)close(f_hlp);f_hlp=0;}
//  setfillstyle(SOLID_FILL,8);bar(240,316,360,320);
//  setcolor(14);moveto(250,313);outtext("Выход - 'Esc'");
	return;
}
