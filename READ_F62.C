#include"bla.h"
/*************************************************************\
*  Процедура вывода на экран и просмотра накопленных данных о *
*  работе и состоянии элементов УВК (в том числе ненормы)     *
\*************************************************************/
void card()
{
	int k,i,j,god,mes,den;
	struct ftime d1,d2;
	unsigned char STROKA[50],DAT[12],byt;
#ifdef NALAD
	nom_func("16");
#endif
	if(iqw>=446)return; //если строка экрана ниже допуска
	if(klo==0)//если начало просмотра неиспр.УВК
	{GraphMode=VGAHI;clscreen();}

  //если нажата PgDn или PgUp или набирается пароль
	if((klaval==81)||(klaval==73)||(klaval==55))
	{
		kartina();//изображение "шапки" и картинки состояния УВК
		clear_half();
		shl();
	}
//$$$
	if(BEG_TIM[0]>48)//если есть время начала фиксаций данных по УВК
	{
		moveto(23*8,205);outtext(BEG_TIM);outtext(" - ");outtext(END_TIM);
	}

	klaval=-1;
	if(bil_bil==1)goto rd_down;//продолжаем читать
	if(bil_bil==2)goto rd_up;//продолжаем читать
	//начинаем читать
	if(DISK==0) //если просмотр данных накопленных на текущее реальное время
	{
		n_1=open("result\\oblom1.fix",O_RDWR|O_BINARY);
		n_2=open("result\\oblom2.fix",O_RDWR|O_BINARY);
	}
	else //если просмотр данных накопленных на текущее архивное время
	{
		n_1=open("disk\\oblom1.fix",O_RDWR | O_BINARY);
		n_2=open("disk\\oblom2.fix",O_RDWR |O_BINARY);
	}
	i=0;
	if(n_1>0)//если есть первый файл
	{
		number=read_str(n_1,STROKA,DAT);
		den=atoi(DAT);
		k=0;
		for(i=strlen(DAT)-4;i<strlen(DAT);i++)STROKA[k++]=DAT[i];
		STROKA[k]=0;
		god=atoi(STROKA);
		k=0;
		for(i=strlen(DAT)-7;i<strlen(DAT)-4;i++)
		if(DAT[i]=='-')continue;
		else STROKA[k++]=DAT[i];
		STROKA[k]=0;
		mes=atoi(STROKA);
		i=((god-2000)*12+mes)*30+den;

		strcpy(BEG_TIM,DAT);strcat(BEG_TIM," ");strcat(BEG_TIM,TIME);
	}
	j=0;
	if(n_2>0)//если есть второй файл
	{
		number=read_str(n_2,STROKA,DAT);
		den=atoi(DAT);
		k=0;
		for(j=strlen(DAT)-4;j<strlen(DAT);j++)STROKA[k++]=DAT[j];
		STROKA[k]=0;
		god=atoi(STROKA);
		k=0;
		for(j=strlen(DAT)-7;j<strlen(DAT)-4;j++)
		if(DAT[j]=='-')continue;
		else STROKA[k++]=DAT[j];
		STROKA[k]=0;
		mes=atoi(STROKA);
		j=((god-2000)*12+mes)*30+den;
		strcpy(END_TIM,DAT);strcat(END_TIM," ");strcat(END_TIM,TIME);
	}
	if((i>j)&&(n_1>0)&&(n_2>0))//если первый файл позже и есть оба
	{
		strcpy(BEG_TIM,END_TIM);//переназначить начало
		for(i=5;i<100;i++)
		{
			j=lseek(n_1,-i,2);//встать в конец первого файла
			read(n_1,&byt,1);
			if(byt==13)break;
		 }
		number=read_str(n_1,STROKA,DAT);
		strcpy(END_TIM,DAT);strcat(END_TIM," ");strcat(END_TIM,TIME);
	}
	else//если нет двух
		if(n_1>0)//если есть только первый
		{
			for(i=5;i<100;i++)
			{
				j=lseek(n_1,-i,2);//встать в конец первого файла
				read(n_1,&byt,1);
				if(byt==13)break;
			}
			number=read_str(n_1,STROKA,DAT);
			strcpy(END_TIM,DAT);strcat(END_TIM," ");strcat(END_TIM,TIME);
		}
		else
			if(n_2>0)//если есть только второй
			{
				strcpy(BEG_TIM,END_TIM);
				for(i=5;i<100;i++)
				{
					j=lseek(n_2,-i,2);//встать в конец первого файла
					read(n_2,&byt,1);
					if(byt==13)break;
				}
				number=read_str(n_2,STROKA,DAT);
				strcpy(END_TIM,DAT);strcat(END_TIM," ");strcat(END_TIM,TIME);
			}
			else //если нет файлов
			{
				setcolor(12);moveto(23*8,205);
				outtext("НЕТ ДАННЫХ");
				bil_bil=0;
				return;
			}
	setcolor(8);
//$$$
	moveto(23*8,205);outtext(BEG_TIM);outtext(" - ");outtext(END_TIM);

	if((n_1!=-1)&&(n_2!=-1))//если есть оба файла
	{
		if(filelength(n_1)>40000l)//если первый файл более 40кб
		{
			if(filelength(n_2)>40000l)//если второй файл более 40кб
			{
				getftime(n_1,&d1);
				getftime(n_2,&d2);
				//проверка того, что файл oblom1.fix создан раньше
				if(max(d1.ft_year,d2.ft_year)!=d1.ft_year)goto r2;
				else
					if(max(d1.ft_month,d2.ft_month)!=d1.ft_month)goto r2;
					else
						if(max(d1.ft_day,d2.ft_day)!=d1.ft_day)goto r2;
						else
							if(max(d1.ft_hour,d2.ft_hour)!=d2.ft_hour)goto r2;
							else goto r1;
			}
			else goto r2;
		}
		else goto r1;
	}
	else //если одного файла нет
	{
		if(n_1!=-1)goto r1;//если есть первый файл
		else goto r2;
	}
r1://чтение первого файла
  qf=n_1;
  lseek(qf,0l,0);
  goto rd_down;
r2://чтение второго файла
	qf=n_2;
  lseek(qf,0l,0);
rd_down:
	number=read_str(qf,STROKA,DAT);
	if(number==-1)//если файл исчерпан
	{ i=lseek(qf,0l,0);
		if((qf==n_1)&&(n_2>0))qf=n_2;
		else
			if((qf==n_2)&&(n_1>0))qf=n_1;
    iqw=446;
		vuhod();
		return;
	}
  iqw=iqw+10;
	if(iqw==436)vuhod();
	if(number>0)
	{
		setcolor(8);
		moveto(16,iqw);
		outtext(DAT);
		moveto(14*8,iqw);
		outtext(TIME);
		rashifr(number,iqw);
	}
	bil_bil=1;
	return;
rd_up:
	if(tell(qf)<1000L)//если стоим близко к началу файла
	{
		lseek(qf,0l,0);
		if((qf==n_1)&&(n_2>0))qf=n_2;
		else
			if((qf==n_2)&&(n_1>0))qf=n_1;
		i=lseek(qf,-1000l,2);
		if(i<0)lseek(qf,0l,0);
    else  while(byt!=13)read(qf,&byt,1);
    goto rd_down;
  }
  else
  {
  	i=lseek(qf,-1000l,1);
    while(byt!=13)read(qf,&byt,1);
    goto rd_down;
	}
}
//-----------------------------------------------------------------------
/****************************************************\
* Процедура расшифровки данных архива неисправностей *
* УВК и вывода текстовых строк в экран просмотра по  *
* нажатию клавиши F6 на резервной ПЭВМ.              *
\****************************************************/
void rashifr(int number,int iqw)
{
	int duk,kluch;
	char kod[4];
#ifdef NALAD
	nom_func("262");
#endif
	setlinestyle(0,0,0);
	if(qf==n_1)setcolor(1);
	if(qf==n_2)setcolor(5);
	kluch=0;

	if(number==9000)kluch=1;
	if((number>=9100)&&(number<9110))kluch=2;
	if((number>=9200)&&(number<9210))kluch=3;
	if((number>=9300)&&(number<9310))kluch=4;
	if((number>=9400)&&(number<9410))kluch=5;
	if((number>=8000)&&(number<8010))kluch=6;
	if((number>=8010)&&(number<8200))kluch=7;
	if((number>=7000)&&(number<8000))kluch=8;
	if((number>=8300)&&(number<8310))kluch=9;
	if((number>=8400)&&(number<8410))kluch=10;
	if(number==8450)kluch=11;
	if((number>=4000)&&(number<4010))kluch=12;
	if((number>=4010)&&(number<4020))kluch=13;
	if((number>=4020)&&(number<4030))kluch=14;
	if((number>=4030)&&(number<4040))kluch=15;
	if((number>=4040)&&(number<4050))kluch=16;
	if((number>=4050)&&(number<4060))kluch=17;
	if((number>=4060)&&(number<4070))kluch=18;
	if((number>=4070)&&(number<4080))kluch=19;
	if((number>=4120)&&(number<4130))kluch=20;
	if((number>=4130)&&(number<4140))kluch=21;
	if((number>=4140)&&(number<4150))kluch=22;
	if((number>6000)&&(number<6100))kluch=23;
	if((number>6100)&&(number<6200))kluch=24;
	if((number>6200)&&(number<6300))kluch=25;
	if((number>1000)&&(number<1100))kluch=26;
	if((number>1100)&&(number<1200))kluch=27;
	if((number>1200)&&(number<1300))kluch=28;
	if((number>2000)&&(number<2100))kluch=29;
	if((number>2100)&&(number<2200))kluch=30;
	if((number>2200)&&(number<2300))kluch=31;
	switch(kluch)
	{
		case 1: moveto(24*8,iqw);outtext("НАЧАЛО РАБОТЫ");return;

		case 2: moveto(24*8,iqw);number=number-9100;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						outtext(" ком. перекл. компл.");
						return;
		case 3: moveto(24*8,iqw);number=number-9200;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						outtext(" объединение групп");
						return;
		case 4:	moveto(24*8,iqw);number=number-9300;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						outtext(" перезап / объед");
						return;
		case 5: moveto(24*8,iqw);number=number-9400;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						outtext(" перезапуск ст.");
						return;
		case 6: moveto(24*8,iqw);number=number-8000;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						fraza(8000);
						return;
		case 7:	moveto(24*8,iqw);duk=number/10;number=number%10;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						fraza(duk*10);
						return;
		case 8: moveto(24*8,iqw);fraza2(number);return;
		case 9: moveto(24*8,iqw);number=number-8300;
						itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						outtext("ошибка в ПЗУ пр. МПСУ");
						return;
		case 10: moveto(24*8,iqw);number=number-8400;
						 itoa(number+1,kod,10);strcat(kod,":");outtext(kod);
						 outtext("ошибка в ПЗУ БД МПСУ");
						 return;
		case 11: moveto(24*8,iqw);outtext("ош. записи в файл");
						 return;
		case 12: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-3999,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext(" объед.гр. ");
						 for(bi=0;bi<=14;bi++)
						 if(mlu[bi]!=48)
						 { sbis[0]=0;sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(" ");
						 }
						 setcolor(8);
						 return;
		case 13: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4009,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext(" обр.гр. ");
						 for(bi=0;bi<=14;bi++)
						 if(mlv[bi]!=48)
						 { sbis[0]=0; sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(" ");
						 }
						 setcolor(8);
						 return;
		case 14: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4019,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext(" М201.1 нет 0");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mly[bi]!=48)
						 { sbis[0]=0; sbis[1]=0;
							 itoa(bi+1,sbis,10); outtext(sbis); outtext(", ");
						 }
						 setcolor(8);
						 return;
		case 15: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4029,kod,10);strcat(kod,":");
						 outtext(kod);
						 sbis[0]=0;sbis[1]=0;itoa(kot,sbis,10);
						 outtext("ЭВМ:");
						 outtext(sbis);outtext(" М201.2 нет 0");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mlz[bi]!=48)
						 {
							 sbis[0]=0; sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(", ");
						 }
						 setcolor(8);
						 return;
		case 16: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4039,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0;sbis[1]=0;itoa(kot,sbis,10);
						 outtext("ЭВМ:");outtext(sbis);
						 outtext(" М201.3 нет 0");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mlc[bi]!=48)
						 { sbis[0]=0;sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(", ");
						 }
						 setcolor(8);
						 return;

		case 17: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4049,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0;sbis[1]=0;itoa(kot,sbis,10);
						 outtext("ЭВМ:");outtext(sbis);
						 outtext(" М201.4 нет 0");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mls[bi]!=48)
						 { sbis[0]=0;sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(", ");
						 }
						 setcolor(8);
						 return;

		case 18: moveto(24*8,iqw);
						 itoa(number-4049,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext("сообщ.вых.интерф.МПСУ ");
						 k=0;
						 for(i_s=0;i_s<40;i_s++)STRoka[i_s]=0;
						 while(pako[kot][k]!=32)
						 {
							 STRoka[k]=pako[kot][k++];
							 if(k>=40)
							 {
								 strcpy(STRoka,"неизв.объект");
								 break;
							 }
						 }
						 moveto(52*8,iqw);
						 outtext(STRoka);outtext("-1");
						 return;

		case 19: moveto(24*8,iqw);
						 itoa(number-4059,kod,10);strcat(kod,":");
						 outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext("сообщ.вых.интерф.МПСУ ");
						 k=0;
						 for(i_s=0;i_s<40;i_s++)STRoka[i_s]=0;
						 while((pako[kot][k]!=32)&&(k<40))k++;
						 if(k>=40)k=0;
						 i_s=0;
						 while(pako[kot][k]!=0)
						 {
							 STRoka[i_s++]=pako[kot][k++];
							 if(k>=40)
							 {
								 strcpy(STRoka,"неизв.объект");
								 break;
							 }
						 }
						 moveto(52*8,iqw);
						 outtext(STRoka);
						 outtext("-2");
						 return;

		case 20: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4119,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext(" М201.1 нет 1");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mly[bi]!=48)
						 { sbis[0]=0;sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(", ");
						 }
						 setcolor(8);
						 return;

		case 21: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4129,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0; sbis[1]=0; itoa(kot,sbis,10);
						 outtext("ЭВМ:"); outtext(sbis);
						 outtext(" М201.2 нет 1");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mlz[bi]!=48)
						 { sbis[0]=0;sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(", ");
						 }
						 setcolor(8);
						 return;

		case 22: setcolor(4);
						 moveto(24*8,iqw);
						 itoa(number-4129,kod,10);strcat(kod,":");outtext(kod);
						 sbis[0]=0;sbis[1]=0;itoa(kot,sbis,10);
						 outtext("ЭВМ:");outtext(sbis);
						 outtext(" М201.3 нет 1");
						 moveto(51*8,iqw);
						 outtext("разр.");
						 for(bi=0;bi<=15;bi++)
						 if(mlc[bi]!=48)
						 { sbis[0]=0;sbis[1]=0;itoa(bi+1,sbis,10);
							 outtext(sbis);outtext(", ");
						 }
						 setcolor(8);
						 return;
	 case 23:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ББКП(переполнен)");
						 moveto(52*8,iqw);
						 itoa(number-6000,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;
	 case 24:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ББКП(паритет)");
						 moveto(52*8,iqw);
						 itoa(number-6100,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;

	 case 25:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ББКП(стоп-биты)");
						 moveto(52*8,iqw);
						 itoa(number-6200,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;

	 case 26:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ТУМС1(переполнен)");
						 moveto(52*8,iqw);
						 itoa(number-1000,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;
	 case 27:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ТУМС1(паритет)");
						 moveto(52*8,iqw);
						 itoa(number-1100,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;
	 case 28:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ТУМС1(стоп-биты)");
						 moveto(52*8,iqw);
						 itoa(number-1200,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;

	 case 29:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ТУМС2(переполнен)");
						 moveto(52*8,iqw);
						 itoa(number-2000,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;

	 case 30:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ТУМС2(паритет)");
						 moveto(52*8,iqw);
						 itoa(number-2100,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;

	 case 31:  setcolor(4);
						 moveto(24*8,iqw);
						 outtext("сбои в канале ТУМС2(стоп-биты)");
						 moveto(52*8,iqw);
						 itoa(number-2200,STRoka,10);
						 outtext(STRoka);
						 setcolor(8);
						 return;

	default:	moveto(52*8,iqw);
						if(fr1[number][13]==1) outtext("I:");
						else outtext("II:");
						if(fr1[number][0]==1)
						{
							for(i_s=0;i_s<40;i_s++)STRoka[i_s]=0;
							i_s=0;
							while(pako[number][i_s]!=':')
							{
								STRoka[i_s]=pako[number][i_s];
								i_s++;
								if(i_s>=40)
								{
									strcpy(STRoka,"неизв.объект");
									break;
								}
							}
							outtext(STRoka);
						}
						else outtext(pako[number]);
						return;
		}
	}
//-----------------------------------------------
int read_str(int hndl,unsigned char STROKA[50],unsigned char DAT[12])
{
	int i,number,j;
	for(i=0;i<50;i++)
	{
		j=read(hndl,&STROKA[i],1);
		if(eof(hndl))return(-1);
		if(j<=0)return(-1);
		if(STROKA[i]<=13)i--;
		if(STROKA[i]==32)break;
	}
	STROKA[i]=0;
	if(i<12)strcpy(DAT,STROKA);
	for(i=0;i<50;i++)
	{
		read(hndl,&STROKA[i],1);
		if((STROKA[i]==32)&&(i>2))break;
	}
	STROKA[i]=0;
	if(i<25)strcpy(TIME,STROKA);
	for(i=0;i<50;i++)
	{
		read(hndl,&STROKA[i],1);
		if((STROKA[i]==32)&&(i>2))break;
		if(STROKA[i]==13)break;
	}
	STROKA[i]=0;
	number=atoi(STROKA);
	read_sode(hndl,number);
	return(number);
}