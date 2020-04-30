#include"bla.h"
void *f_o;
//-------------------------------------------------------------------------
/*********************************************************\
* Процедура записи на диск сообщения о неисправности СЦБ  *
* для объекта с именем "slovo" в графу типа "znak"        *
\*********************************************************/
void neisprav_na_disk(char znak,char slovo[20])
{
	unsigned dt1,dt2,tm1,tm2;

	nom_func("306");
	if(pusto!=0)return;
  if(nikuda==1)return;

  //если не просмотр архива, а получение данных из канала ТУМС
  if(DISK==0)
	{
	 n_ob1=open("result\\neispr1.fix",O_RDWR,O_BINARY);
   n_ob2=open("result\\neispr2.fix",O_RDWR,O_BINARY);
  }
  //если получение данных из файла архива
  else
  {
	 n_ob1=open("disk\\neispr1.fix",O_RDWR,O_BINARY);
   n_ob2=open("disk\\neispr2.fix",O_RDWR,O_BINARY);
  }

  //если есть оба файла
  if((n_ob1!=-1)&&(n_ob2!=-1))
  {
		if(filelength(n_ob1)>40000l)//если первый файл более 40кб
		{
			if(filelength(n_ob2)>40000l)//если второй файл более 40кб
      {
				_dos_getftime(n_ob1,&dt1,&tm1);
				_dos_getftime(n_ob2,&dt2,&tm2);
				//проверка того, что файл oblom1.fix создан раньше
				if((max(dt1,dt2)==dt2)&&(dt1!=dt2))goto sozd1;//если второй моложе
				else
					if((max(tm1,tm2)==tm2)&&(tm2!=tm1))goto sozd1;//если второй моложе
					else goto sozd2;
			}
			else
			{
				f_ob1=n_ob2;
				close(n_ob1);
				n_ob1=0;
				goto pisat;
			}
		}
		else
		{
			f_ob1=n_ob1;
			close(n_ob2);
			n_ob2=0;
			goto pisat;
		}
	}
	else //если одного файла нет
	{
		if(n_ob1!=-1)//если есть первый файл
		{
			if(filelength(n_ob1)>40000l)goto sozd2;//если первый файл более 40кб
			else {f_ob1=n_ob1;goto pisat;}
		}
		else goto sozd1;
	}
sozd1:
	if(n_ob1>0)close(n_ob1);
	n_ob1=0;
	if(n_ob2>0)close(n_ob2);
	n_ob2=0;

  //если не просмотр архива
	if(DISK==0)n_ob1=creat("result\\neispr1.fix",S_IREAD|S_IWRITE);
  //если просмотр архива
	else n_ob1=creat("disk\\neispr1.fix",S_IREAD|S_IWRITE);

	if(n_ob1==-1)goto error;//если файл не удалось создать
	else
	{
		if(n_ob1>0)close(n_ob1);
		n_ob1=0;
	}
	if(DISK==0)n_ob1=open("result\\neispr1.fix",O_RDWR|O_BINARY);
	else n_ob1=open("disk\\neispr1.fix",O_RDWR|O_BINARY);
	if(n_ob1==-1)goto error;//если файл не удалось открыть
	f_ob1=n_ob1;
	goto pisat;
sozd2:
	if(n_ob2>0)close(n_ob2);
	n_ob2=0;
	if(n_ob1>0)close(n_ob1);
	n_ob1=0;
	if(DISK==0)n_ob2=creat("result\\neispr2.fix",S_IREAD|S_IWRITE);
	else n_ob2=creat("disk\\neispr2.fix",S_IREAD|S_IWRITE);
	if(n_ob2==-1)goto error;//если файл не удалось создать
	else
	{
		if(n_ob2>0)close(n_ob2);
		n_ob2=0;
	}
	if(DISK==0)n_ob2=open("result\\neispr2.fix",O_RDWR,O_BINARY);
	else n_ob2=open("disk\\neispr2.fix",O_RDWR,O_BINARY);
	if(n_ob2==-1)goto error;//если файл не удалось открыть
	f_ob1=n_ob2;
pisat:
	add_word(znak,slovo);
	if(f_ob1>0)close(f_ob1);
  if(f_ob1==n_ob1)n_ob1=0;
  if(f_ob1==n_ob2)n_ob2=0;
	f_ob1=0;
	return;
error:
	w(44,999," (файл neispr#.fix)");
	soob_for_oper=1;
	return;
}
//-------------------------------------------------------------------------
/*************************************************\
* Процедура записи в текущий файл neispr#.fix     *
* данных о ненорме состояния объектов СЦБ         *
* kto - символ типа ненормового объекта           *
* slo - строка условного наименования объекта     *
\*************************************************/
void add_word(char kto,char slo[20])
{
	int yui;
	unsigned char STROKA[50],dat[5];
	unsigned char dn[3],ms[3],gd[5],chs[3],mnt[3],scn[3];

	nom_func("3");
	for(yui=0;yui<50;yui++)STROKA[yui]=0; //очистка строки для записи

	if(DISK==0) //если нет просмотра архива, то запись текущих даты и времени
  {
		itoa(dayy_,dat,10);
		strcpy(STROKA,dat);strcat(STROKA,"-");
		itoa(monn_,dat,10);
		strcat(STROKA,dat);strcat(STROKA,"-");
		itoa(yearr_,dat,10);
		strcat(STROKA,dat);
	 	strcat(STROKA,TIME);
  }
  else //если просмотр архива, то запись считанных даты и времени
  {
		itoa(b_h1,chs,10);itoa(b_m1,mnt,10);itoa(b_s1,scn,10);
    itoa(dayy_,dn,10);itoa(monn_,ms,10);itoa(yearr_,gd,10);
  	strcpy(STROKA,dn);strcat(STROKA,"-");
    strcat(STROKA,ms);strcat(STROKA,"-");
    strcat(STROKA,gd);strcat(STROKA," ");
    strcat(STROKA,chs);strcat(STROKA,":");
		strcat(STROKA,mnt);strcat(STROKA,":");
    strcat(STROKA,scn);strcat(STROKA," ");
  }
  strcat(STROKA,&kto); //запись кода и слова
  strcat(STROKA," ");
	strcat(STROKA,slo);
  STROKA[strlen(STROKA)]=0xd;
	STROKA[strlen(STROKA)]=0;
  lseek(f_ob1,0,2);
	write(f_ob1,STROKA,strlen(STROKA));
  return;
}
//------------------------------------------
void slom_interf(int chi)
{
	struct ftime d1,d2;

	nom_func("306");
	if(pusto!=0)return;
  if(klo==1)return;
  if(DISK==0)
	{
	 n_ob1=open("result\\oblom1.fix",O_RDWR,O_BINARY);
   n_ob2=open("result\\oblom2.fix",O_RDWR,O_BINARY);
  }
  else
  {
	 n_ob1=open("disk\\oblom1.fix",O_RDWR,O_BINARY);
	 n_ob2=open("disk\\oblom2.fix",O_RDWR,O_BINARY);
	}
	if((n_ob1!=-1)&&(n_ob2!=-1))//если есть оба файла
	{
		if(filelength(n_ob1)>40000l)//если первый файл более 40кб
		{
			if(filelength(n_ob2)>40000l)//если второй файл более 40кб
			{
				getftime(n_ob1,&d1);
				getftime(n_ob2,&d2);
				//проверка того, что файл oblom1.fix создан раньше
				if(max(d1.ft_year,d2.ft_year)!=d1.ft_year)goto sozd1;
				else
					if(max(d1.ft_month,d2.ft_month)!=d1.ft_month)goto sozd1;
					else
						if(max(d1.ft_day,d2.ft_day)!=d1.ft_day)goto sozd1;
						else
							if(max(d1.ft_hour,d2.ft_hour)!=d2.ft_hour)goto sozd1;
							else goto sozd2;
			}
			else
			{
				f_ob1=n_ob2;
				close(n_ob1);
				n_ob1=0;
				goto pisat;
			}
		}
		else
		{
			f_ob1=n_ob1;
			close(n_ob2);
			n_ob2=0;
			goto pisat;
		}
	}
	else //если одного файла нет
	{
		if(n_ob1!=-1)//если есть первый файл
		{
			if(filelength(n_ob1)>40000l)goto sozd2;//если первый файл более 40кб
			else {f_ob1=n_ob1;goto pisat;}
		}
		else goto sozd1;
	}
sozd1:
	if(n_ob1>0)close(n_ob1);
	n_ob1=0;
	if(n_ob2>0)close(n_ob2);
	n_ob2=0;
	if(DISK==0)n_ob1=creat("result\\oblom1.fix",S_IREAD|S_IWRITE);
	else n_ob1=creat("disk\\oblom1.fix",S_IREAD|S_IWRITE);
	if(n_ob1==-1)goto error;//если файл не удалось создать
	else
	{
	 if(n_ob1>0)close(n_ob1);
	 n_ob1=0;
	}
	if(DISK==0)n_ob1=open("result\\oblom1.fix",O_RDWR|O_BINARY);
	else n_ob1=open("disk\\oblom1.fix",O_RDWR|O_BINARY);
	if(n_ob1==-1)goto error;//если файл не удалось открыть
	f_ob1=n_ob1;
	goto pisat;
sozd2:
	if(n_ob2>0)close(n_ob2);
	n_ob2=0;
	if(n_ob1>0)close(n_ob1);
	n_ob1=0;
	if(DISK==0)n_ob2=creat("result\\oblom2.fix",S_IREAD|S_IWRITE);
	else n_ob2=creat("disk\\oblom2.fix",S_IREAD|S_IWRITE);
	if(n_ob2==-1)goto error;//если файл не удалось создать
	else
	{
		if(n_ob2>0)close(n_ob2);
		n_ob2=0;
	}
	if(DISK==0)n_ob2=open("result\\oblom2.fix",O_RDWR,O_BINARY);
	else n_ob2=open("disk\\oblom2.fix",O_RDWR,O_BINARY);
	if(n_ob2==-1)goto error;//если файл не удалось открыть
	f_ob1=n_ob2;
pisat:
	add_oblom(chi);
  if(f_ob1>0)close(f_ob1);
  if(f_ob1==n_ob1)n_ob1=0;
  if(f_ob1==n_ob2)n_ob2=0;
	f_ob1=0;
	return;
error:
	w(44,999," (файл oblom#.fix)");
	soob_for_oper=1;
	return;
}
//-------------------------------------------------------------------
/***********************************************\
* Процедура определения активного полукомплекта *
\***********************************************/
int kotora(int ks)
{
	nom_func("133");
	if((fr3[ks][0]==0)&&(fr3[ks][1]==0)&&(fr3[ks][2]==1))return(1);
	if((fr3[ks][0]==1)&&(fr3[ks][1]==1)&&(fr3[ks][2]==0))return(2);
	if((fr3[ks][0]==0)&&(fr3[ks][1]==0)&&(fr3[ks][2]==0))return(1);
	if((fr3[ks][0]==1)&&(fr3[ks][1]==0)&&(fr3[ks][2]==0))return(2);
	return(0);
}
/*********************************************\
* Процедура записи в текущий файл oblom#.fix *
* данных о неисправностях интерфейса УВК      *
* chis - код ненормы состояния интерфейса     *
\*********************************************/
void add_oblom(int chis)
{
	int Tch=0,yui;
	unsigned char STROKA[50],dat[5],chs[3],min[3],sec[3],cifr[2];
	nom_func("2");
	cifr[1]=0;
	for(yui=0;yui<50;yui++)STROKA[yui]=0;//очистить строку

	//записать в строку день
	itoa(dayy_,dat,10);
	strcpy(STROKA,dat);strcat(STROKA,"-");

	//записать в строку месяц
	itoa(monn_,dat,10);
	strcat(STROKA,dat);strcat(STROKA,"-");

	//записать в строку год
	itoa(yearr_,dat,10);
	strcat(STROKA,dat);strcat(STROKA," ");

	if(DISK==0) //если нет просмотра архива -  добавить текущее время
	{
		strcat(STROKA,TIME);strcat(STROKA," ");
	}
	else //при просмотре архива добавить считанное время
	{
		itoa(b_h1,chs,10);strcat(STROKA,chs);strcat(STROKA,":");
		itoa(b_m1,min,10);strcat(STROKA,min);strcat(STROKA,":");
		itoa(b_s1,sec,10);strcat(STROKA,sec);strcat(STROKA," ");
	}
	itoa(chis,dat,10);strcat(STROKA,dat);

  // работа с данными по объединению групп
	if((chis>=4000)&&(chis<4010))
	{
		Tch=chis-4000;
		strcat(STROKA," ");//пробел
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr); //записать
		strcat(STROKA," ");//пробел
		for(yui=0;yui<=14;yui++)//записать признаки объединения групп
		{cifr[0]=mas_lu[Tch][yui]+48;strcat(STROKA,cifr);}
	}

	//работа с данными по обрыву групп
	if((chis>=4010)&&(chis<4020))
	{
		Tch=chis-4010;
		strcat(STROKA," ");//пробел
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr); //записать
		strcat(STROKA," ");//пробел
		for(yui=0;yui<=14;yui++)//записать признаки обрыва групп
		{cifr[0]=mas_lv[Tch][yui]+48;strcat(STROKA,cifr);}
	}

	//работа с данными по потерям 0 в М201-1
	if((chis>=4020)&&(chis<4030))
	{
	 Tch=chis-4020;
	 strcat(STROKA," ");//пробел
	 cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
	 strcat(STROKA,cifr);//записать активный полукомплект
	 strcat(STROKA," ");//пробел
	 for(yui=0;yui<=15;yui++)//записать признаки потери 0 М201-1
	 {cifr[0]=(mas_ly[Tch][yui]&0xff)+48;strcat(STROKA,cifr);}
	}

	//работа с данными по потерям 0 в М201-2
	if((chis>=4030)&&(chis<4040))
	{
		Tch=chis-4030;
		strcat(STROKA," ");//пробел
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr); //записать
		strcat(STROKA," ");//пробел
		for(yui=0;yui<=15;yui++)//записать признаки потери 0 М201-2
		{cifr[0]=(mas_lz[Tch][yui]&0xff)+48;strcat(STROKA,cifr);}
	}

	//работа с данными по потерям 0 в М201-3
	if((chis>=4040)&&(chis<4050))
	{
		Tch=chis-4040;
		strcat(STROKA," ");
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr);//записать
		strcat(STROKA," ");
		for(yui=0;yui<=15;yui++)//записать признаки потери 0 М201-3
		{cifr[0]=(mas_lc[Tch][yui]&0xff)+48;strcat(STROKA,cifr);}
	}

	//работа с данными по потерям 0 в М201-4
	if((chis>=4050)&&(chis<4060))
	{
		Tch=chis-4050;
		strcat(STROKA," ");
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr);//записать
		strcat(STROKA," ");
		for(yui=0;yui<=15;yui++)//записать признаки потери 0 М201-4
		{cifr[0]=(mas_ls[Tch][yui]&0xff)+48;strcat(STROKA,cifr);}
	}

	//работа с данными по потерям 1 в М201-1
	if((chis>=4120)&&(chis<4130))
	{
	 Tch=chis-4120;
	 strcat(STROKA," ");//пробел
	 cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
	 strcat(STROKA,cifr);//записать активный полукомплект
	 strcat(STROKA," ");//пробел
	 for(yui=0;yui<=15;yui++)//записать признаки потери 1 М201-1
	 {cifr[0]=((mas_ly[Tch][yui]&0xff00)>>8)+48;strcat(STROKA,cifr);}
	}

	if((chis>=4130)&&(chis<4140))
	{
		Tch=chis-4130;
		strcat(STROKA," ");//пробел
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr); //записать
		strcat(STROKA," ");//пробел
		for(yui=0;yui<=15;yui++)//записать признаки потери 1 М201-2
		{cifr[0]=((mas_lz[Tch][yui]&0xff00)>>8)+48;strcat(STROKA,cifr);}
	}
	if((chis>=4140)&&(chis<4150))
	{
		Tch=chis-4140;
		strcat(STROKA," ");
		cifr[0]=kotora(KSK[Tch])+48;//определить активный полукомплект
		strcat(STROKA,cifr);//записать
		strcat(STROKA," ");
		for(yui=0;yui<=15;yui++)//записать признаки потери 1 М201-3
		{cifr[0]=((mas_lc[Tch][yui]&0xff00)>>8)+48;strcat(STROKA,cifr);}
	}

	if((chis>=4060)&&(chis<4080))//если ненорма датчиков дешифратора
	{
		strcat(STROKA," ");
		itoa(ELE,dat,10);
		strcat(STROKA,dat);
	}

	STROKA[strlen(STROKA)]=0xd;
	STROKA[strlen(STROKA)]=0;
	lseek(f_ob1,0l,SEEK_END);
	Tch=write(f_ob1,STROKA,strlen(STROKA));
	return;
}
