#include<stdio.h>
#include"bla.h"
int Zy=180;
//--------------------------------------------------------
void ZZagruzka()
{
	int Ntu=0,zjj=0,zj=0,jj,i,n_bait;
	nom_func("440");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	for(i=0;i<MAX_SIZE_OUT;i++)BUF_OUT_PVM[i]=0;/* очистить буфер */
	for(i=0;i<MAX_SIZE_IN;i++)BUF_IN_PVM[i]=0;/* очистить буфер */
	NEISPRAVEN=0;
	outportb(BAZ_ADR1+1,0);/* закрыть прерывания от ТУМС-1*/
	outportb(BAZ_ADR1,0);
	outportb(BAZ_ADR11+1,0);/* закрыть прерывания от ТУМС-2*/
	outportb(BAZ_ADR11,0);
	setlinestyle(0,0,0); setcolor(BLUE); rectangle(5,5,634,475);
	moveto(100,30);
	//settextstyle(0,0,2);
	outtext("ЗАГРУЗКА ПРОГРАММЫ:");
	setcolor(11);
	//settextstyle(0,0,0);
	moveto(10,60);
	outtext("1. ОПРЕДЕЛЕНИЕ НОМЕРА ПЭВМ РМ ДСП.........");
#ifdef WORK
	STATUS=2;
	for(zjj=0;zjj<kol_VO;zjj++)
	{
		pooo[zjj]=0;
		pooo_com[zjj]=0;
		com_bit[zjj]=0;
		if((fr1[zjj][0]!=6)&&(fr1[zjj][0]!=7)&&(fr1[zjj][1]!=9999))
/*		&&
		(fr1[zjj][0]!=5)&&(fr2[zjj][0]!=0))*/
		{
			fr3[zjj][0]=fr3[zjj][1]=fr3[zjj][2]=fr3[zjj][3]=fr3[zjj][4]=0;
			fr3[zjj][5]=1;
		}
		else fr3[zjj][0]=fr3[zjj][1]=fr3[zjj][2]=fr3[zjj][3]=fr3[zjj][4]=fr3[zjj][5]=0;
/*		if((fr1[zjj][0]==5)&&((fr1[zjj][1]==0)||(fr1[zjj][2]==0)||(fr1[zjj][3]==0)))
		fr3[zjj][0]=fr3[zjj][1]=fr3[zjj][2]=fr3[zjj][3]=fr3[zjj][4]=fr3[zjj][5]=0;
*/
		if((fr1[zjj][0]==5)&&(fr1[zjj][1]==1)&&(fr1[zjj][2]==1))
		fr3[zjj][0]=fr3[zjj][1]=fr3[zjj][2]=fr3[zjj][3]=fr3[zjj][4]=fr3[zjj][5]=0;
		fr3[zjj][10]=0;
		fr3[zjj][11]=0;
	}
	for(zjj=0;zjj<MAX_SIZE_IN;zjj++)BUF_IN_PVM[zjj]=0;
	for(zjj=0;zjj<MAX_SIZE_OUT;zjj++)BUF_OUT_PVM[zjj]=0;
	for(zjj=0;zjj<n_bait;zjj++){BUF_COM_PVM[zjj]=0;REG_COM[zjj]=0;}
	for(zjj=0;zjj<6;zjj++){BUF_FR4[zjj]=0;REG_FR4[zjj]=0;}
	for(zjj=0;zjj<4;zjj++){BUF_INF[zjj]=0;REG_INF[zjj]=0;}
	for(zjj=0;zjj<3;zjj++)
	{REG_KVIT_INF[zjj]=0;REG_KVIT_COM[zjj]=0;REG_KVIT_FR4[zjj]=0;}
//  инициировать порт переключателя основная/резервная ПЭВМ
	outportb(BAZ_ADR5+1,0x0);// запретить прерывания  COM для переключателя резерва
	outportb(BAZ_ADR5+4,0x0);
	delay(500);
	//установить DTR и RTS - сигнал опроса осн/рез и N ПЭВМ
	outportb(BAZ_ADR5+4,0x3);
	delay(500);
	zjj=inportb(BAZ_ADR5+6);
	t_p_pvm(200);
	FFFlag=0;ZAGRUZKA=2;
	start_port(1);
#else
NOMER_ARMA=1;
#endif
	Ntu=0;
	for(i=0;i<kol_VO;i++)for(jj=0;jj<3;jj++)fr4[i][jj]=0;//сброс массива ограничений
	if(STATUS==1)
	{
		pointer=open("result\\ogr.fr4",O_BINARY|S_IWRITE);//открытие файла ограничений
		if(pointer!=-1)//если файл ограничений есть
		{
			read(pointer,&fr4,len);//прочитать ограничения из файла
			if(pointer>0)close(pointer);//закрыть файл
			pointer=0;
			for(i=0;i<kol_VO;i++)fr4[i][0]=fr4[i][0]&1;//сброс лишних ограничений
		}
		else
		{
			if(pointer>0)close(pointer);//если файла нет - закрыть неудачную попытку
			pointer=0;
		}
		disco();//создать файл, записать ограничения.
	}
/*
#ifndef WORK
	return;
#endif
*/
	setcolor(8);
	if((NEISPRAVEN==0)&&(STATUS==0)&&((NOMER_ARMA==1)||(NOMER_ARMA==2)))
	{
		for(zjj=0;zjj<kol_VO;zjj++){fr4[zjj][0]=0;fr4[zjj][1]=0;fr4[zjj][2]=0;}
		FFFlag=1;
		setcolor(11);
		outtextxy(10,100,"3. ПРИЕМ УСТАНОВЛЕННЫХ ОГРАНИЧЕНИЙ ИЗ ОСНОВНОЙ ПЭВМ.......");
		PRIEM_FR4=1;
		OBMEN_PVM=0xC0;
		kom_v_bufer_pvm(0,OBMEN_PVM);//запрос в основной машине данных по fr4
		OBMEN_PVM=0;
		OBMEN_PVM_OLD=OBMEN_PVM;
		FORM_BUF_PVM_OUT();
		outportb(BAZ_ADR4+1,3);//разрешение передачи
		zjj=0;
		setcolor(8);line(10,110,630,110);line(10,120,630,120);
		for(zjj=1;zjj<=63;zjj++)line(10*zjj,110,10*zjj,120);
		setcolor(1);
		FIR_time=biostime(0,0L);
		SEC_time=FIR_time;
		zjj=0;
		while(PRIEM_FR4==1)
		{
			what_is_new();
			if(N_OGR>0){setcolor(1);moveto(10*(N_OGR)+1,111),outtext("█");}
			SEC_time=biostime(0,0L);
			if(SEC_time-FIR_time>3L)
			{
				pust_pvm++;
				FIR_time=SEC_time;
				zjj++;
				if(zjj>=10)
				{
					NEISPRAVEN=1;
					break;
				}
			}
			FORM_BUF_PVM_OUT();
		}
		POVTOR_FR4=0;POVTOR_COM=0;POVTOR_INF=0;
		disco();
	}
	if(NEISPRAVEN==1)
	{
		moveto(348,100);setcolor(7);outtext(">");setcolor(12);
		moveto(10,130);outtext("Нет связи с ПЭВМ, ограничения не приняты из ПЭВМ-");
		PRIEM_FR4=0;POVTOR_FR4=0;POVTOR_COM=0;POVTOR_INF=0;
		if(NOMER_ARMA==1) outtext("2 !!!");
		else
			if(NOMER_ARMA==2) outtext("1 !!!");
			else  outtext("???");
	}
	setlinestyle(0,0,3);setcolor(12);
	line(90,355,190,355);
	line(376,355,525,355);
	line(91,354,91,410);
	line(90,409,524,409);
	line(524,354,524,410);
	setusercharsize(2,1,2,1);
	setcolor(12);
	outtextxy(200,350,"ВНИМАНИЕ !!!");
	setusercharsize(1,1,1,1);
	setcolor(8);
	outtextxy(100,370,"После выполнения загрузки данных проверьте установку");
	outtextxy(100,380,"всех ограничений, имеющихся для станционных объектов,");
	outtextxy(100,390,"и, при необходимости,введите их в АРМы ДСП");
	setlinestyle(0,0,0);
	setcolor(14);
	moveto(10,270);
	if(FFFlag==1) outtext("4.");
	else outtext("3.");
	outtext(" ЗАГРУЗКА ДАННЫХ ИЗ МПСУ................");
	ZAGRUZKA=1;
	setcolor(8); zj=0; Ntu=0;
	line(10,300,(KOL_SOO1+1)*10,300);
	line(10,315,(KOL_SOO1+1)*10,315);
#ifdef KOL_SOO2
	line(10,320,(KOL_SOO2+1)*10,320);
	line(10,335,(KOL_SOO2+1)*10,335);
	for(zj=0;zj<=KOL_SOO2;zj++)line(10*zj+10,320,10*zj+10,335);
#endif
	for(zj=0;zj<=KOL_SOO1;zj++)line(10*zj+10,300,10*zj+10,315);
	zj=0;
	Init_TEST_SOOB(0);
#ifdef KOL_SOO2
	Init_TEST_SOOB(1);
#endif
	outportb(BAZ_ADR1+1,1);
	outportb(BAZ_ADR11+1,1);
	pusto=0;
	zj=0;
	FIR_time=biostime(0,0L);
	SEC_time=FIR_time;
	while(ZAGRUZKA==1)
	{
		if(TEST_SUM_SOOB()==0)ZAGRUZKA=0;
		Ntu=test_time1(18L);
		if(Ntu>0)
		{
			if(otkaz_ts[0]==1){setcolor(12);moveto(250,150);outtext("Отказ I канала ТС");}
			if(otkaz_ts[1]==1){setcolor(12);moveto(250,160);outtext("Отказ II канала ТС");}
			zj++;
			FIR_time=SEC_time;
			if(zj>20)break;
		}
		consentr();
		proverka_svyazi();
	}
	ZAGRUZKA=0;
	jj=0;
	t_pust[0]=0;t_pust[1]=0;
	cha=0;na=0;
	form_kom_avtod('A');
	return;
}
/************************************************************/
void t_p_pvm()
{
  int cl=0,cll=0,kokl=0;
	nom_func("327");
	disable();
  cll=inportb(BAZ_ADR5+6);
agg:
  cl=cll&0x10; /* проверяем CTS - номер АРМа */
  if(NOMER_ARMA==3)/* если номер пока не определен */
  {
    if(kokl>100)
    {
      proverka_NA();
      clear_K();
      goto jj;
		}
    if(cl==0x10) NOMER_ARMA=1;// если CTS установлен, это первый АРМ
    else NOMER_ARMA=2;// иначе второй АРМ
    delay(100);
    kokl++;
    goto agg;
  }
jj:
  kokl=0;
  OK(350,60);
  Rezult(200,70,NOMER_ARMA);
  setcolor(11);
  //settextstyle(0,0,0);
  moveto(10,80);
  outtext("2. ОПРЕДЕЛЕНИЕ СТАТУСА ПЭВМ РМ ДСП........");
agg1:
  if(STATUS==2)
  {
    if(kokl>100)
    {
			slom_interf(7600);
      add(0,0,'Д');
      proverka_STA();
    }
    else
    {
			cl=cll&0x80;
			if(cl==0x80)
			{
				cl=cll&0x20;
				if(cl==0)
				{
					if(STATUS==1) return;
					STATUS=1;
					slom_interf(7400);
					add(0,0,'Г');
				}
			}
			else
			{
				cl=cll&0x20;
				if(cl==0x20)
				{
					cl=cll&0x80;
					if(cl==0)
					{
						if(STATUS==0) return;
						STATUS=0;
						slom_interf(7500);
						add(0,0,'Д');
					}
				}
				else
				{
					kokl++;
					goto agg1;
				}
			}
		} /* else kokl*/
	}
viho:
	OK(350,80);
	Rezult_2(200,90,STATUS);
	enable();
	return;
}
//---------------------------------------------------------------------
/****************************************************\
* Процедура объявления на экране о нарушении         *
* конфигурации каналов и прекращения работы системы. *
\****************************************************/
void Change_adresa()
{
	nom_func("17");
	reset_int_vect();
  slom_interf(7700);
  if(STATUS==1)kom_v_bufer_pvm(158,0);// 'Ю'
	add(0,0,'ю');
  setcolor(14);
  moveto(200,Zy);
  outtext("ПРОИЗВЕДЕНА РЕКОНФИГУРАЦИЯ");
  Zy=Zy+10;moveto(200,Zy);
  outtext("АДРЕСОВ КАНАЛОВ СВЯЗИ СООТВЕТСТВЕННО");
  Zy=Zy+10;moveto(200,Zy);
  outtext("ПОДКЛЮЧЕНИЮ: II - ТУМС1, I - ТУМС2");
  getch();exit(0);
	return;
}
//--------------------------------------------------------------
/******************************************\
* Процедура выдачи сообщения о ненорме при *
* определении номера АРМа                  *
\******************************************/
void proverka_NA()
{
	nom_func("252");
	setfillstyle(SOLID_FILL,8);bar(100,100,530,200);
  setcolor(LIGHTRED);
  setlinestyle(0,0,0);rectangle(103,103,527,197);zvuk_vkl(0,0);
  moveto(120,110); outtext("        Не удается определить номер ПЭВМ.");
  setcolor(7);
  moveto(110,128); outtext("   Следует выполнить проверку исправности цепей  ");
  moveto(110,136); outtext("           соединителя P5 адаптера 8-COM.");
  setcolor(15); moveto(110,185);
  outtext("       Для выхода нажмите любую клавишу");
  getch();
  FINAL_();
	return;
}
//-------------------------------------------------------------
FILE *fdal;
void Rezult(int x,int y,int nA)
{
  char qqq[2]="";
	nom_func("280");
	setcolor(YELLOW); moveto(x,y);outtext("ДАННАЯ ПЭВМ - НОМЕР ");
  setfillstyle(SOLID_FILL,7);bar(x+160,y,x+180,y+9);
  itoa(nA,qqq,10);moveto(x+160,y);outtext(qqq);
	return;
}
//----------------------------------------------------------------
void Rezult_2(int x,int y,int sA)
{
	nom_func("281");
	setcolor(YELLOW);moveto(x,y);outtext("ДАННАЯ ПЭВМ - ");
  setfillstyle(SOLID_FILL,7);bar(x+112,y,x+112+160,y+9);
  moveto(x+112,y);
  if(sA==1) outtext("ОСНОВНАЯ");
  else
    if(sA==0) outtext("РЕЗЕРВНАЯ");
    else
    {
      setcolor(RED); outtext("СТАТУС НЕ ОПРЕДЕЛЕН");
    }
	return;
}
//---------------------------------------------------------------------
/*********************************************************\
* Процедура выдачи текста "ОК" в указанное место экрана.  *
\*********************************************************/
void OK(int x,int y)
{
	nom_func("199");
	setcolor(MAGENTA);moveto(x,y);outtext("Ok");
	return;
}
//------------------------------------------------------------------
/************************************\
* Процедура выдачи текста на экран о *
* ненорме в определении статуса ПЭВМ *
\************************************/
void proverka_STA()
{
	nom_func("253");
	setfillstyle(SOLID_FILL,8);bar(100,100,530,200);
  //settextstyle(0,0,0);
  setcolor(LIGHTRED);setlinestyle(0,0,0);rectangle(103,103,527,197);
  zvuk_vkl(0,0);
  setcolor(7);moveto(110,128);
  outtext("     Следует выполнить проверку состояния");
  moveto(110,136);
  outtext("           переключателя 'ОСН/РЕЗ'.");
	nosound();
	return;
}
//------------------------------------------------------------------
/***********************************************\
* Очистка части экрана при загрузке программы и *
* определении статуса ПЭВМ                      *
\***********************************************/
void clear_K()
{
	nom_func("24");
	setfillstyle(SOLID_FILL,7);
  bar(100,100,530,200);
	return;
}

