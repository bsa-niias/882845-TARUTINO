#include "bla.h"
/********************************************\
* Процедура записи данных в текущий архив    *
* kk - идентификатор типа данных             *
* ob - код символа отображения начала данных *
\********************************************/
void add(int kk,int sob, unsigned char ob)
{
	unsigned char ZAPIS[35];
	unsigned int ito,n_bait;
	nom_func("1");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return; //при просмотре архива выход сразу
	file_arc=open(NAME_FILE,O_APPEND|O_RDWR,O_BINARY);//открыть файл архива
	if(file_arc<0) //если открытие файла неудачное
	{
		clscreen();moveto(10,100);setcolor(14);
		outtext("Нарушение файловой структуры при открытии,работа невозможна");
		getch();FINAL_();
	}
	for(ito=0;ito<30;ito++)ZAPIS[ito]=0; //очистить строку записи
	strcpy(ZAPIS,TIME);   //скопировать в строку время
	if(sob==8888) //если удаление маршрута
	{
		strncat(ZAPIS,"{удал}",6);
		if(ob!=0)strncat(ZAPIS,&ob,1);
		strncat(ZAPIS,pako[MARSHRUT_ALL[kk].NACH],strlen(pako[MARSHRUT_ALL[kk].NACH]));
	}
	else
	if(sob==6666) //если создан локальный
	{
		strncat(ZAPIS," ",2);
		strncat(ZAPIS,MARSHRUT_ST[kk][ob].NEXT_KOM,13);
	}
	else
	if(sob==9999) //если подготовлен маршрут для выдачи в канал
	{
		strncat(ZAPIS,KOMANDA_TUMS[kk],13);
		strncat(ZAPIS,&ob,1); //добавить символ идентификатор вида сообщения
	}
	else
	{
		strncat(ZAPIS,&ob,1); //добавить символ идентификатор вида сообщения
		if((ob=='S')||(ob==175)||(ob==162)||(ob=='>')||(ob=='<')||(ob==0xF))
		{
			if(kk<2)
			{
				 //если команда, взять из буфера диска
				if(ob=='>')strncat(ZAPIS,buf_zap_disk,n_bait);

				//если команда из соседней ПЭВМ, взять из буфера ПЭВМ
				if(ob=='<')strncat(ZAPIS,buf_ko_p,n_bait);

				//если сообщение из стойки - взять из буфера стойки
				if((ob!='>')&&(ob!='<'))strncat(ZAPIS,bu[kk],n_bait);
			}

			if(kk==2)
			{
				BUF_COM_PVM[9]='n';
				BUF_COM_PVM[10]=check_sum(BUF_COM_PVM,1);
				BUF_COM_PVM[11]=')';
				BUF_COM_PVM[12]=0;
				strcat(ZAPIS,BUF_COM_PVM);
			}
			if(kk==3)strcat(ZAPIS,REG_COM);
			if(kk==4)strcat(ZAPIS,BUF_FR4);
			if(kk==5)strcat(ZAPIS,REG_FR4);
		}
	}
	//в резервную ПЭВМ передана команда установки автодействия
	if(kk==10)strncat(ZAPIS,REG_COM,n_bait);

	ZAPIS[strlen(ZAPIS)]=0xd;
	ZAPIS[strlen(ZAPIS)]=0;

	ito=write(file_arc,ZAPIS,strlen(ZAPIS));

	if(file_arc>0)ito=close(file_arc); //закрытие файла
	else ito=0;
	if(ito!=0)
	{
		clscreen();
		moveto(10,100);
		setcolor(14);
		outtext("Нарушение файловой структуры при закрытии,работа невозможна");
		getch();
		FINAL_();
	}
	file_arc=0;
	return;
}
//---------------------------------------------------------------
/**********************************************************\
* Процедура вывода на экран текущего реального времени при *
* вызове из основной программы main, виртуального времени  *
* при вызове из disk_arc().                                *
\**********************************************************/
void out_time()
{
	char vre[10]="",chas[3]="",min[3]="",sec[3];
	nom_func("213");
	if(help!=0)return;
	setlinestyle(0,0,0);setcolor(MAGENTA);rectangle(2,2,64,13);
	setfillstyle(SOLID_FILL,WHITE);bar(3,3,63,12);

	itoa(ho_ur,chas,10);
	itoa(mi_n,min,10);
	itoa(se_c,sec,10);
	setcolor(MAGENTA);
	if(ho_ur<10){strcpy(vre,"0");strcat(vre,chas);}
	else strcpy(vre,chas);
	strcat(vre,":");
	if(mi_n<10)strcat(vre,"0");
	strcat(vre,min);
	strcat(vre,":");
	if(se_c<10)strcat(vre,"0");
	strcat(vre,sec);

	outtextxy(4,4,vre);
  return;
}
//---------------------
void read_t(void)
{
	char h_0,h_1,m_0,m_1,s_0,s_1;
	nom_func("275");
//процедура чтения текущего времени из таймера реального времени
	rg.h.ah=0x2c; int86(0x21,&rg,&rg);
	h_0=rg.h.ch%10;h_1=rg.h.ch/10;
	m_0=rg.h.cl%10;m_1=rg.h.cl/10;
  s_0=rg.h.dh%10;s_1=rg.h.dh/10;
  ho_ur=h_0+h_1*10;
  mi_n=m_0+m_1*10;
  se_c=s_0+s_1*10;
  TIME[0]=0x20;    TIME[1]=h_1|0x30;TIME[2]=h_0|0x30;TIME[3]=0x3a;
  TIME[4]=m_1|0x30;TIME[5]=m_0|0x30;TIME[6]=0x3a;    TIME[7]=s_1|0x30;
  TIME[8]=s_0|0x30;TIME[9]=0x20; TIME[10]=0;
  return;
}
//--------------------------------------------------------------
int test_time1(long const_)
{
  int i,j;
	nom_func("355");
	SEC_time=biostime(0,0l);
  if(labs(SEC_time-FIR_time)>const_)
  {
    if(SEC_time<=18l)
    {
      TIME_OUT_PVM=SEC_time;
      j=peek(0,0x470);
      if(j!=1)
      {
        rg.x.cx=yearr_;
        rg.h.dh=monn_;
        rg.h.dl=dayy_;
        rg.h.ah=0x2b;
        int86(0x21,&rg,&rg);
      }
      second_time=SEC_time;
      first_time=SEC_time;
    }
    if((SEC_time>=1572462l)&&(SEC_time<=1572480l))
    {

      FIR_time=SEC_time;
      second_time=SEC_time;
      first_time=SEC_time;
      dayy_++;
      if((monn_==1)||(monn_==3)||(monn_==5)||(monn_==7)||(monn_==8)||(monn_==10)||(monn_==12))
      {if(dayy_>31){dayy_=1;monn_++;if(monn_>12){monn_=1;yearr_++;}}}
        else
        if(monn_==4||monn_==6||monn_==9||monn_==11)
          {if(dayy_>30){dayy_=1;monn_++;}}
        else
          if(monn_==2)
          {
            if((yearr_%4)==0){if(dayy_>29){dayy_=1;monn_++;}}
            else  if(dayy_>28){dayy_=1;monn_++;}
          }
      dat.day=dayy_;
      dat.month=monn_;
      dat.year=yearr_;
      for(i=0;i<17;i++)NAME_FILE[i]=0;
      strcpy(NAME_FILE,"RESULT//");
      if(dayy_<10){NAME_FILE[8]=0x30;NAME_FILE[9]=dayy_|0x30;}
      else {NAME_FILE[8]=(dayy_/10)|0x30;NAME_FILE[9]=(dayy_%10)|0x30;}
      if(monn_<10){NAME_FILE[10]=0x30;NAME_FILE[11]=monn_|0x30;}
      else{NAME_FILE[10]=(monn_/10)|0x30;NAME_FILE[11]=(monn_%10)|0x30;}
      strcat(NAME_FILE,".ogo");
      NAME_FILE[16]=0;
      if(file_arc>0)close(file_arc);
      file_arc=0;
      file_arc=open(NAME_FILE,O_CREAT|O_TRUNC|O_APPEND|O_WRONLY,S_IREAD|S_IWRITE|O_BINARY);
      if(file_arc<0)
      {
        clscreen();moveto(100,100);setcolor(14);
        outtext("Нарушение файловой структуры,работа невозможна");
        getch();FINAL_();exit(1);
      }
      if(file_arc>0)close(file_arc);
      file_arc=0;
      FIR_time=SEC_time;
      return(0);
    }
    else
    {
			read_t();
      return(1);
    }
  }
  else return(0);
}
//------------------------------------------------------------------
/*******************************************************\
* Процедура записи на диск имеющихся ограничений FR4    *
\*******************************************************/
void disco()
{
	nom_func("36");
  if(DISK==0)pointer=creat("result\\ogr.fr4",O_BINARY|S_IWRITE);
  else pointer=creat("disk\\ogr.fr4",O_BINARY|S_IWRITE);
  write(pointer,fr4,len);
  if(pointer>0)close(pointer);
  pointer=0;
  return;
}
//-------------------------------------------------------------------
/********************************************************\
* Процедура полной очистки экрана (заливки цветом фона)  *
\********************************************************/
void clscreen()
{
	nom_func("26");
  setfillstyle(1,7);
  bar(0,0,640,480);
  return;
}
//--------------------------------------------------------
void zvuk_vkl(int tip1,int dlit1)
{
	nom_func("439");
  if((STATUS!=1)||(TELEUP!=1)||(DU==1)){nosound();return;}
//  if(prorisovka==1)return;
  zvuk_sig.time_begin=biostime(0,0L);
  zvuk_sig.dlit=dlit1;
  zvuk_sig.tip=tip1;
  if(tip1!=3)sound(600);
  else sound(800);
  return;
}
//-------------------------------------------------------------------
/****************************************************\
*    обработка диагностических сообщений стойки      *
* int diagnoze(unsigned char bufdiag[11],int sto)    *
* bufdiag[11] - полученное диагностическое сообщение *
* sto - стойка ТУМС, от которой получено сообщение   *
\****************************************************/
int diagnoze(unsigned char bufdiag[12],int sto)
{
	unsigned char PODGR,GRUP;

	int bait,tester,objkt,ijk;
	tester=0;
	while(1)
	{
		if(bufdiag[3]!='y'){tester=9999;break;}

		GRUP=bufdiag[4];

		PODGR=bufdiag[5];

		bait=bufdiag[6]-64;
		if((bait<0)||(bait>4)){tester=9999;break;}
		switch(GRUP)
		{
			//сигналы
			case 'E': //если не перекрытие, то сообщение неправильное - выйти
								if(bufdiag[7]!='P'){tester=9999;break;}
								objkt=find_obj(GRUP,PODGR,bait,sto);
								//если объект не найден - выйти
								if((objkt<0)||(objkt>kol_VO)){tester=9999;break;}
								//проверить нет ли запрета на сообщения по данному объекту
								for(ijk=0;ijk<10;ijk++)
								if(FILTR[ijk]==objkt)//если есть запрет - выйти
								{
									tester=9999;
									break;
								}
								if(tester==9999)break;
#ifdef AVTOD
							//при установленном автодействии проверить не входит ли
							//сигнал в трассу автодействия, если входит, то автодействие
							//снять по тому направлению, куда входит сигнал
							if((na==1)||(cha==1))prov_avto_si(objkt);
#endif
								slom_sign(objkt);
								tester=1;
								break;
			//СП и УП
			case 'F': if((bufdiag[7]=='Z')||(bufdiag[7]=='S'))
								{
									objkt=find_obj(GRUP,PODGR,bait,sto);
									//если объект не найден - выйти
									if((objkt<0)||(objkt>kol_VO)){tester=9999;break;}
									for(ijk=0;ijk<10;ijk++)
									if(FILTR[ijk]==objkt)
									{
										tester=9999;
										break;
									}
									if(tester==9999)break;
									if(bufdiag[7]=='S')slom_uch(objkt,'С');
									if(bufdiag[7]=='Z')slom_uch(objkt,'З');
									tester=1;
									break;
								}
								else
								{
									tester=9999;break;
								}
			// пути
			case 'I': if((bufdiag[7]!='Z')&&(bufdiag[7]!='S')&&(bufdiag[7]!='T'))
								{
									tester=9999;break;
								}
								objkt=find_obj(GRUP,PODGR,bait,sto);
								//если объект не найден - выйти
								if((objkt<0)||(objkt>kol_VO)){tester=9999;break;}
								for(ijk=0;ijk<10;ijk++)
								if(FILTR[ijk]==objkt)
								{
									tester=9999;
									break;
								}
								if(tester==9999)break;
								if(bufdiag[7]=='T')slom_uch(objkt,'Т');
								if(bufdiag[7]=='S')slom_uch(objkt,'С');
								if(bufdiag[7]=='Z')slom_uch(objkt,'З');
								tester=1;
								break;

			default: tester=9999;break;
		}
		break;
	}

	return(tester);
}
//---------------------------------------------------------------------
/*********************************************************************\
* int find_obj(unsigned char GRU, unsigned char POD, int byt,int sto) *
* процедура поиска номера объекта по группе,подгруппе,байту и стойке  *
* GRU - код группы                                                    *
* POD - код подгруппы                                                 *
* byt - номер байта в сообщении                                       *
* sto - номер стойки                                                  *
* Возвращает номер найденного объекта в базе.                         *
\*********************************************************************/
int find_obj(unsigned char GRU, unsigned char POD, int byt, int st)
{
	int STRK=0,sb,obekt;
	sb=POD-48;
	if((st==0)&&(sb>KOL_SOO1))sb=POD-112+KOL_SOO1;
#ifdef KOL_SOO2
	if((st==1)&&(sb>KOL_SOO2))sb=POD-112+KOL_SOO2;
#endif
	switch(GRU)//переключатель по группам сообщений
	{ //сообщение по стрелкам
		case 'C': STRK=sb;
							if(st==1)STRK=STRK+STR1;
							obekt=spstr[STRK][byt];
							break;
		case 'E': if(st==0)STRK=sb-STR1;
							if(st==1)STRK=sb-STR2+SIG1;
							obekt=spsig[STRK][byt];
							break;
#if DOP1>0
		case 'Q': if(st==0)STRK=sb-STR1-SIG1;
							if(st==1)STRK=sb-STR2-SIG2+DOP1;
							obekt=spdop[STRK][byt];
							break;
#endif
		case 'F':	if(st==0)STRK=sb-STR1-SIG1-DOP1;
							if(st==1)STRK=sb-STR2-SIG2-DOP2+UCH1;
							obekt=spspu[STRK][byt];
							break;
		case 'I': if(st==0)STRK=sb-STR1-SIG1-DOP1-UCH1;
							if(st==1)STRK=sb-STR2-SIG2-DOP2-UCH2+PUT1;
							obekt=spputi[STRK][byt];
							break;
		case 'L': if(st==0)STRK=sb-STR1-SIG1-DOP1-UCH1-PUT1;
							if(st==1)STRK=sb-STR2-SIG2-DOP2-UCH2-PUT2+UPR1;
							obekt=spkont[STRK][byt];
							break;
		default:  obekt=-1;break;
	}

	return(obekt);
}
