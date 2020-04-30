#include"bla.h"
int ar=0,novaya=0;
int who_is=0,ffa=0;
void read_neisprav()
{
	int k,i,j,god,mes,den;
	struct ftime d1,d2;
	unsigned char STROKA[50],DAT[12],byt;
#ifdef NALAD
  nom_func("269");
#endif
	if(iqw>=446)return;
	if(nikuda==0){GraphMode=VGAHI;clscreen();}//если начало просмотра неиспр.УВК
	if((klaval==81)||(klaval==73)||(klaval==55))
	{
		shapka();//изображение "шапки"
		iqw=50;
	}
	if(BEG_TIM[0]>48)
	{
		setcolor(14);
		moveto(23*8,12);outtext(BEG_TIM);outtext(" - ");outtext(END_TIM);
	}
	klaval=-1;
	if(bil_bil==1)goto rd_down;//продолжаем читать
	if(bil_bil==2)goto rd_up;//продолжаем читать
	//начинаем читать
	if(DISK==0)
	{
		n_1=open("result\\neispr1.fix",O_RDWR|O_BINARY);
		n_2=open("result\\neispr2.fix",O_RDWR|O_BINARY);
	}
	else
	{
		n_1=open("disk\\neispr1.fix",O_RDWR|O_BINARY);
		n_2=open("disk\\neispr2.fix",O_RDWR|O_BINARY);
	}
	i=0;
	if(n_1>0)//если есть первый файл
	{
		symb_1=read_str1(n_1,STROKA,DAT);
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
		symb_1=read_str1(n_2,STROKA,DAT); //$$$$$$
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
		symb_1=read_str1(n_1,STROKA,DAT);
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
			symb_1=read_str1(n_1,STROKA,DAT);
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
				symb_1=read_str1(n_2,STROKA,DAT);
				strcpy(END_TIM,DAT);strcat(END_TIM," ");strcat(END_TIM,TIME);
			}
			else //если нет файлов
			{
				setcolor(12);moveto(23*8,205);
				outtext("НЕТ ДАННЫХ");
				bil_bil=0;
				return;
			}
	setcolor(14);
	moveto(23*8,12);outtext(BEG_TIM);outtext(" - ");outtext(END_TIM);
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
	symb_1=read_str1(qf,STROKA,DAT);
	if(symb_1==255)//если файл исчерпан
	{ i=lseek(qf,0l,0);
		if((qf==n_1)&&(n_2>0))qf=n_2;
		else
			if((qf==n_2)&&(n_1>0))qf=n_1;
		iqw=446;
		vuhod1();
		return;
	}
	iqw=iqw+10;

	if(symb_1!=255)
	{
		setcolor(8);
		moveto(8,iqw-8);outtext(DAT);outtext(" ");outtext(TIME);
		reading();
	}
	if(iqw>445)vuhod1();
	bil_bil=1;
	return;
rd_up:
	if(tell(qf)<2000L)//если стоим близко к началу файла
	{
		lseek(qf,0l,0);
		if((qf==n_1)&&(n_2>0))qf=n_2;
		else
			if((qf==n_2)&&(n_1>0))qf=n_1;
		i=lseek(qf,-2000l,2);
		if(i<0)lseek(qf,0l,0);
		else  while(byt!=13)read(qf,&byt,1);
		goto rd_down;
	}
	else
	{
		i=lseek(qf,-2000l,1);
		while(byt!=13)read(qf,&byt,1);
		goto rd_down;
	}
}

/****************************************************/
void reading()
{
	unsigned char name[25],text[30],bukv[1];
	int h;
#ifdef NALAD
	nom_func("277");
#endif
	for(h=0;h<25;h++)name[h]=0;
	for(h=0;h<25;h++)
	{
		read(qf,&bukv,1);
		name[h]=bukv[0];
		if(name[h]==13)break;
	}
	switch(symb_1)
	{
		case '#': setcolor(4);strcpy(text,"Контроль положения стрелки ");break;
		case '&': setcolor(4);strcpy(text,"Контроль участка пути ");break;
		case '*': setcolor(4);strcpy(text,"Перекрытие светофора ");break;
		case '%': setcolor(4);strcpy(text,"Устройство питания ");break;
		case '@': setcolor(14);strcpy(text,"Неисправность устройства ");break;
		case '$': setcolor(8);strcpy(text,"Включение устройства ");break;
		case '': setcolor(4);strcpy(text,"Авария переезда ");break;
		case '': setcolor(4);strcpy(text,"Неисправность переезда ");break;
		case '': setcolor(4);strcpy(text,"Открыт и неисправен ");break;
		case '':	setcolor(4);strcpy(text,"Закрыт и неисправен ");break;
		case 0x3: setcolor(4);strcpy(text,"Изьят ключ жезл ");break;
		default:  return;
	}
	moveto(162,iqw-8);outtext(text);outtext(name);
	setcolor(YELLOW);
	line(1,iqw-8,1,iqw+10);
	line(639,iqw-10,639,iqw);
	line(20*8,iqw-10,20*8,iqw);
	return;
}
/***************************************************************/
void shapka()
{
#ifdef NALAD
	nom_func("301");
#endif          
  cleardevice();
  setbkcolor(cvv);
  setlinestyle(0,0,0);
  setcolor(YELLOW);
  moveto(80,1);
  outtext("ДИАГНОСТИКА  УСТРОЙСТВ  СЦБ");
  line(1,22,639,22);line(1,50,639,50);line(1,22,1,50);line(639,22,639,50);
  line(20*8,22,20*8,50);
  moveto(10*8,30);outtext("ВРЕМЯ");
  moveto(25*8,28);outtext("ДИАГНОСТИЧЕСКИЕ СООБЩЕНИЯ");
	return;
}
//--------------------------------------------------
unsigned char read_str1(int qf,unsigned char STROKA[50],unsigned char DAT[12])
{
  int i,j;
  unsigned char sym;
  for(i=0;i<50;i++)
	{
		j=read(qf,&STROKA[i],1);
		if(eof(qf))return(-1);
    if(j<=0)return(-1);
    if(STROKA[i]<=13)i--;
    if(STROKA[i]==32)break;
  }
  STROKA[i]=0;
  if(i<12)strcpy(DAT,STROKA);
  for(i=0;i<50;i++)
	{
		read(qf,&STROKA[i],1);
    if((STROKA[i]==32)&&(i>2))break;
  }
  STROKA[i]=0;
  if(i<25)strcpy(TIME,STROKA);
  for(i=0;i<50;i++)
	{
		read(qf,&STROKA[i],1);
    if(STROKA[i]==32)break;
    if(STROKA[i]==13)break;
  }
  STROKA[i]=0;
	sym=STROKA[i-1];;
  return(sym);
}