#include"bla.h"
//------------------------------------------------------------------------
/***************************************************\
* Процедура формирования команд закрытия и          *
* открытия пути (УП) для передачи в резервную ПЭВМ. *
* poin-объект пути в базе, kod - код действия       *
\***************************************************/
void form_kom_put(int poin,int kod)
{ int nomer,i18,j18,n=0,SDVIG=0,FN=0,FIN=0,n_bait;
	nom_func("79");
	n_bait=11;
	n=poin;
	//получить номер объекта управления в markery

	//очистить буфер команд для резервной ПЭВМ
	for(i18=0;i18<n_bait;i18++)buf_ko_p[i18]=0;

	 //если для первой стойки
	if(fr1[n][13]==1)
	{ buf_ko_p[1]=0x61;

		//если управляется путь
		if(fr1[n][0]==5)
		{buf_ko_p[2]='I';SDVIG=STR1+SIG1+DOP1+UCH1;FN=0;FIN=PUT1;}

		//если управляется УП
		else
		{buf_ko_p[2]='F';SDVIG=STR1+SIG1+DOP1;FN=0;FIN=UCH1;}
	}
#ifdef KOL_SOO2
	else
	//если для второй стойки, то аналогично
	{ buf_ko_p[1]=0x62;
		if(fr1[n][0]==5)
		{ buf_ko_p[2]='I';
			SDVIG=STR2+SIG2+DOP2+UCH2;
			FN=PUT1;
			FIN=FN+PUT2;
		}
		else
		{ buf_ko_p[2]='F';
			SDVIG=STR2+SIG2+DOP2;
			FN=UCH1;
			FIN=FN+UCH2;
		}
	}
#endif
	nomer=9999;
	//поиск подгруппы
	for (i18=FN;i18<FIN;i18++)for(j18=0;j18<=4;j18++)
	{ if(fr1[n][0]==5) //если управляется путь
		{ if(spputi[i18][j18]==n)
			{podgr=podgruppa[i18+SDVIG-FN];nomer=j18;break;}
		}
		else//если управляется УП
		{ if(spspu[i18][j18]==n)
			{podgr=podgruppa[i18+SDVIG-FN];nomer=j18;break;}
		}
	}
	if(nomer==9999) return;
	buf_ko_p[3]=podgr;

	//поиск байта объекта в подгруппе
	for (j18=0;j18<=4;j18++)
	if(j18==nomer)buf_ko_p[j18+4]=kod;
	else buf_ko_p[j18+4]=124;
	buf_ko_p[n_bait-2]=check_sum(buf_ko_p,0);
	buf_ko_p[0]='(';buf_ko_p[10]=')';flms=1;
	kom_v_bufer_pvm(1,0x70); //0x70=буква "p"
	return;
}
//-------------------------------------------------------------------
/**************************************************\
* Процедура формирования команды для передачи      *
* ограничения стрелок в резервную ПЭВМ             *
* poin-номер в базе объекта стрелки                *
* kod -код установленного ограничения              *
\**************************************************/
void form_kom_str(int poin,unsigned char kod)
{
	int nomer,podegr,i18,j18,n=0,SDVIG=0,FN=0,FIN=0,nnom=9999,n_bait;
	nom_func("81");
	n_bait=11;
	// записать на диск
	disco();
	for(i18=0;i18<n_bait;i18++)buf_ko_p[i18]=0;//очистить буфер команд
	buf_ko_p[2]='S';//установить код группы "стрелки"
	point=poin;//установить номер для стрелки

	//найти стрелку, входящую в сообщения (имеет смысл для спаренных)
	if(Opred()<0)return;
	poin=point;//передача найденного номера
	n=poin;

	//для стрелок из первой стойки ТУМС сформировать заголовок
	if(fr1[n][13]==1){buf_ko_p[1]=0x61;SDVIG=0;FN=0;FIN=STR1;}
#ifdef KOL_SOO2
	//аналогично для стрелок второй стойки
	else {buf_ko_p[1]=0x62;SDVIG=0;FN=STR1;FIN=FN+STR2;}
#endif
	nomer=9999;
	for (i18=FN;i18<FIN;i18++)//пройти по подгруппам стрелок данной стойки
	for(j18=0;j18<=4;j18++)//пройти по объектам очередной подгруппы
	if (spstr[i18][j18]==n)//если найдена стрелка
	{
		podegr=podgruppa[i18+SDVIG-FN];//выбрать подгруппу
		nomer=j18;//взять номер объекта в подгруппе
		break;
	}
	if(nomer==9999)//если объект не найден
	{
		TEST_SPAR=2;
		nnom=poisk_ras(fr1[n][2],n,0,0);//поискать спаренную стрелку
		if(nnom==9999)return;//если не найдена парная - выход
		else//если найдена парная
		{
			for (i18=FN;i18<FIN;i18++)//найти для неё подгруппу
			for(j18=0;j18<=4;j18++)//найти для подгруппы номер байта
			if (spstr[i18][j18]==nnom)//если найдено
			{
				podegr=podgruppa[i18+SDVIG-FN];//взять подгруппу
				nomer=j18;//взять номер объекта
				break;
			}
		}
	}
	buf_ko_p[3]=podegr; //запомнить код подгруппы
	for (j18=0;j18<=4;j18++)// пройти по всем объектам подгруппы
	if (j18==nomer) buf_ko_p[j18+4]=kod; //для найденного объекта записать код
	else  buf_ko_p[j18+4]=124;//для остальных объектов - записать код-заполнитель
	buf_ko_p[n_bait-2]=check_sum(buf_ko_p,0);//получить и записать контрольную сумму
	buf_ko_p[0]='(';//заполнить начало и конец команды скобками-ограничителями
	buf_ko_p[n_bait-1]=')';
	add(0,0,'<');
	flms=1;//установить признак-требование передачи в соседнюю ПЭВМ

  //0x43=буква "C"
  if(kom_v_bufer_pvm(1,0x43)==-1)//если вызов процедуры неудачен
  {
    MY_COM=0;//сбросить признак наличия в ПЭВМ команды для соседа
    MY_INF=0;//сбросить признак наличия в ПЭВМ информации для соседа
    MY_FR4=0;//сбросить признак наличия в ПЭВМ ограничений для соседа
		POVTOR_COM=0;//сбросить требование повтора команды соседу
    POVTOR_INF=0;//сбросить требование повтора информации соседу
    POVTOR_FR4=0;//сбросить требование повтора ограничений соседу
  }
  return;
}
//-------------------------------------------------------
/*
zad2_marsh()
{ //процедура задания сквозного маршрута через две стойки
#ifdef KOL_SOO2
	int vv=0,vmv=0,n_bait,n_strel,jj,ii;
	unsigned int pol_strel;
	nom_func("422");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;
	buf_ko[2]='b';//записать в буфер для первой и второй стоек код поездного
	buf_ko_mm[2]='b';
	vmv=9999;
	if(fr1[nach_marsh][1]==1)//если маршрут нечетный
	{
		for(vmv=0;vmv<2;vmv++)//пройти по сквозным маршрутам
		if(Skvo[vmv].B1_s==nach_marsh) break;//если нашли маршрут с таким началом
	}
	else //если маршрут четный
	{
		for(vmv=2;vmv<4;vmv++)//пройти по сквозным четным
		if(Skvo[vmv].B1_s==nach_marsh) break;//если нашли такое начало
	}
	komanda2[fr1[Skvo[vmv].B2_s][13]-1]=Skvo[vmv].B2_s;
	komanda2[fr1[Skvo[vmv].B1_s][13]-1]=Skvo[vmv].B1_s;
	if(vmv>3)return;
	vv=vibor(Skvo[vmv].B2_s);//получить для начала второго полумаршрута код-индекс
	buf_ko[1]=BAM;//заполнить заголовок во втором буфере
	if(vv!=9999)buf_ko[4]=vv;//заполнить код-индекс
	buf_ko[3]=podgr;//заполнить подгруппу
	vv=vibor(Skvo[vmv].E2_s);//получить код-индекс для конца второго полумаршрута
	if(vv!=9999)  buf_ko[6]=vv;//заполнить подгруппу
	buf_ko[5]=podgr;
#ifdef NEW_FORMAT
	if(TRASSA[0].object)
	{
		n_strel=0;
		pol_strel=0;
		ii=0;
		while((TRASSA[ii].object&0xfff)!=Skvo[vmv].B2_s)ii++;
		for(jj=ii;jj<=ukaz_trass;jj++)
		{
			if(fr1[TRASSA[jj].object&0xfff][0]==1)//если стрелка
			{
				if((TRASSA[jj].object&0x4000)==0x4000)//если противошерстная
				{
					if((TRASSA[jj]&0x2000)!=0x2000) //если нет глушения
					{
						if((TRASSA[jj]&0x8000)==0x8000)//если в минусе
						{
							pol_strel=pol_strel|(1<<n_strel);
						}
						n_strel++;
					}
				}
			}
			if((TRASSA[jj]&0xfff)==end_marsh)break;
		}
	}
	buf_ko[7]=0x40|n_strel;
	buf_ko[8]=0x40|(pol_strel&0x3f);
	buf_ko[9]=0x40|((pol_strel&0xFC0)>>6);
	buf_ko[10]=0;
	buf_ko[11]=')';
#else
//???????????
	buf_ko[7]=124;//в свободные байты записать заполнитель
	buf_ko[8]=124;
//???????????
#endif
	buf_ko[n_bait-2]=check_sum(buf_ko); //дополнить команду контрольной суммой
	buf_ko[0]='(';
	buf_ko[n_bait-1]=')';
	pooo_com[Skvo[vmv].B2_s]=biostime(0,0L);//зафиксировать время команды
	//выделить номер бита, для которого дана команда
	com_bit[Skvo[vmv].B2_s]=buf_ko[2]&3;

	vv=vibor(Skvo[vmv].B1_s);//получить код-индекс для начала первого полумаршрута
	buf_ko_mm[1]=BAM;//вписать заголовок в первый буфер
	if(vv!=9999)buf_ko_mm[4]=vv;//вставить код-индекс
	buf_ko_mm[3]=podgr;//вставить код подгруппы
	vv=vibor(Skvo[vmv].E1_s);//получить код-индекс для конца первого полумаршрута
	if(vv!=9999)buf_ko_mm[6]=vv;//вставить код индекс
	buf_ko_mm[5]=podgr;//вставить код подгруппы
//???????????????????
#ifdef NEW_FORMAT
	if(TRASSA[0])
	{
		n_strel=0;
		pol_strel=0;
		ii=0;
		while((TRASSA[ii]&0xfff)!=Skvo[vmv].B1_s)ii++;
		for(jj=ii;jj<=ukaz_trass;jj++)
		{
			if(fr1[TRASSA[jj]&0xfff][0]==1)//если стрелка
			{
				if((TRASSA[jj]&0x4000)==0x4000)//если противошерстная
				{
					if((TRASSA[jj]&0x2000)!=0x2000) //если нет глушения
					{
						if((TRASSA[jj]&0x8000)==0x8000)//если в минусе
						{
							pol_strel=pol_strel|(1<<n_strel);
						}
						n_strel++;
					}
				}
			}
			if((TRASSA[jj]&0xfff)==Skvo[vmv].E1_s)break;
		}
	}
	buf_ko_mm[7]=0x40|n_strel;
	buf_ko_mm[8]=0x40|(pol_strel&0x3f);
	buf_ko_mm[9]=0x40|((pol_strel&0xFC0)>>6);
	buf_ko_mm[10]=0;
	buf_ko_mm[11]=')';
#else
	buf_ko_mm[7]=124;//в свободные байты записать заполнитель
	buf_ko_mm[8]=124;
#endif
	buf_ko_mm[n_bait-2]=check_sum(buf_ko_mm);//вставить контрольную сумму
	buf_ko_mm[0]='(';
	buf_ko_mm[n_bait-1]=')';
	pooo_com[Skvo[vmv].B1_s]=biostime(0,0L);//зафиксировать время команды
	//выделить номер бита, для которого дана команда
	com_bit[Skvo[vmv].B1_s]=buf_ko_mm[2]&3;
	home(modi);//вернуть курсор "домой"
	flagoutmsg=11;//выставить флаг-требование на передачу команд в 2 стойки
//  TIMER_SOB(Skvo[vmv].B2_s,11,Skvo[vmv].B2_s);//запустить таймер
#endif
	return;
}
*/
/*****************************************************************/
int vibor(int pot)
{
	//выбор кода команды для указания объекта, в зависимости от номера байта
  int nomer,n=0,SDVIG=0,FN=0,FIN=0,i11,j11;
	nom_func("388");
  if(DISK!=0)return(9999);
  n=pot;
  if(fr1[n][0]==2)
	{ if(fr1[n][13]==1)//если объект в первой стойке ТУМС
    { BAM=ZAGO_MPSU(0x61);//получить заголовок
      SDVIG=STR1;//взять сдвиг на сигналы
      FN=0;FIN=SIG1;
    }
#ifdef KOL_SOO2
    else
    {
			BAM=ZAGO_MPSU(0x62);//если вторая стойка
      SDVIG=STR2;FN=SIG1;FIN=FN+SIG2;
    }
#endif
    nomer=9999;
    for (i11=FN;i11<FIN;i11++)//пройти по строкам сообщений
    for(j11=0;j11<=4;j11++)//пройти по объектам строки
    if (spsig[i11][j11]==n)//если нашли заданный объект
    {
      podgr=podgruppa[i11+SDVIG-FN];//получить код подгруппы
      nomer=j11;//получить номер байта
      break;//прервать цикл
    }
  }
  if((fr1[n][0]==3)||(fr1[n][0]==4))
  {
    if(fr1[n][13]==1)//если объект в первой стойке ТУМС
    {
      BAM=ZAGO_MPSU(0x61);//получить заголовок
      SDVIG=STR1+SIG1+DOP1;//взять сдвиг на сигналы
      FN=0;FIN=UCH1;
    }
#ifdef KOL_SOO2
    else
    {
      BAM=ZAGO_MPSU(0x62);//если вторая стойка
      SDVIG=STR2+SIG2+DOP2;FN=UCH1;FIN=FN+UCH2;
		}
#endif
    nomer=9999;
    for (i11=FN;i11<FIN;i11++)//пройти по строкам сообщений
    for(j11=0;j11<=4;j11++)//пройти по объектам строки
    if (spspu[i11][j11]==n)//если нашли заданный объект
    {
      podgr=podgruppa[i11+SDVIG-FN];//получить код подгруппы
			nomer=j11;//получить номер байта
      break;//прервать цикл
    }
  }

  if(nomer==9999) return(9999);//если объект не найден вернуть 999
  switch(nomer)//переключатель по номеру байта в строке
  {
    case 0:return(64); //код для байта №0 = @
    case 1:return(65); //код для байта №1 = A
    case 2:return(66); //код для байта №2 = B
    case 3:return(67); //код для байта №3 = C
    case 4:return(68); //код для байта №4 = D
    default:return(64);//код для неопределенного байта = @
  }
}
//---------------------------------------------------------------
/********************************************************\
* Процедура вычисления контрольной суммы для содержимого *
* массива num[11]. Cкладывает все байты по модулю 2.     *
\********************************************************/
int check_sum(unsigned char num[12],int tip)
{              /*  */
	unsigned char sum=0;
	int i22,n_bait;
	nom_func("18");
	if(tip==1)n_bait=12;
	else n_bait=11;
	for(i22=1;i22<(n_bait-2);i22++)
	sum=sum^num[i22];
	sum=sum|0x40;
	return(sum);
}
//--------------------------------------------------------------------
/**********************************************************\
* Процедура формирования команд раздельного управления для *
* объектов группы "сигнал", объект воздействия в глобальной*
* переменной point, kod - код команды                      *
\**********************************************************/
int form_kosig(int kod)
{

	int nomer,podgr,i18,j18,SDVIG=0,FN=0,FIN=0,n=0,n_bait;

	nom_func("83");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return(1);
	buf_ko[2]='E';//заполнить группу
	n=point;//зафиксировать объект выбранный оператором
	//если объект из первой стойки: заголовок,сдвиг и граница
	if(fr1[n][13]==1){buf_ko[1]=ZAGO_MPSU(0x61);SDVIG=STR1;FN=0;FIN=SIG1;}
#ifdef KOL_SOO2
	else//для второй стойки - аналогично
	{ buf_ko[1]=ZAGO_MPSU(0x62);SDVIG=STR2;FN=SIG1;FIN=FN+SIG2;}
#endif
	nomer=9999;
	if((fr1[n][1]==16)&&(fr1[n][2]>=5)&&(fr1[n][2]<=8))//если РОН/РОЧ
	{
		buf_ko[1]=buf_ko[1]&3;
		buf_ko[1]=buf_ko[1]|0x50;
	}
	for (i18=FN;i18<FIN;i18++) //пройти по подгруппам
	{
		for(j18=0;j18<=4;j18++) //пройти по объектам подгруппы
		if(spsig[i18][j18]==n) //если найден объект
		{
			podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
			nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
			break;
		}
		if(nomer!=9999)break;
	}
	if(nomer==9999) return(1); //если не найден объект - выйти
	buf_ko[3]=podgr; // заполнить подгруппу
	for (j18=0;j18<(n_bait-6);j18++)
	if (j18==nomer) buf_ko[j18+4]=kod;//для найденного объекта - заполнить код
	else  buf_ko[j18+4]=124;// для прочих вписать заполнитель
	buf_ko[0]='(';
	return(0);
}
//-----------------------------------------------------------------
/******************************************************\
* Процедура формирования команд раздельного управления *
* для объектов группы "доп.объект"                     *
* Номер объекта хранится в глоб.переменной point       *
* Код команды в параметре функции kod                  *
* В итоге пакетная команда в буфере команд buf_ko      *
\******************************************************/
int form_kodop(int kod)
{
	int nomer,podgr,i18,j18,SDVIG=0,FN=0,FIN=0,n=0,n_bait;

	nom_func("83");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return(1); //при просмотре архива ничего не делать
	buf_ko[2]='Q';//заполнить группу
	n=point;//зафиксировать объект выбранный оператором
	//если объект из первой стойки: заголовок,сдвиг и граница
	if(fr1[n][13]==1)
	{buf_ko[1]=ZAGO_MPSU(0x61);SDVIG=STR1+SIG1;FN=0;FIN=DOP1;}
#ifdef KOL_SOO2
	else//для второй стойки - аналогично
	{ buf_ko[1]=ZAGO_MPSU(0x62);SDVIG=STR2+SIG2;FN=DOP1;FIN=FN+DOP2;}
#endif
	nomer=9999;
	if((fr1[n][1]>=155)&&(fr1[n][1]<=158))//если РОН/РОЧ
	{
		buf_ko[1]=buf_ko[1]&3;
		buf_ko[1]=buf_ko[1]|0x50;
		if(fr1[n][1]==155)//если РОН для нечетного отправления
		{
			if((kod=='M')&&(na==1))//если РОН отключается и неч.автодействие
			{
				w(265,999,"(нечетное)");
				zvuk_vkl(0,0);
				snyato_nechet();
				TIMER_SOB(n,11,n);
			}
		}
		if(fr1[n][1]==156)//если РОЧ для четного отправления
		{
			if((kod=='M')&&(cha==1))//если РОН отключается и чет.автодействие
			{
				w(265,999,"(четное)");
				zvuk_vkl(0,0);
				snyato_chet();
				TIMER_SOB(n,11,n);
			}
		}
	}
	for (i18=FN;i18<FIN;i18++) //пройти по подгруппам
	for(j18=0;j18<=4;j18++) //пройти по объектам подгруппы
#if DOP1>0
	if (spdop[i18][j18]==n) //если найден объект
	{
		podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
		nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
		break;
	}
#else
	if (spsig[i18][j18]==n) //если найден объект
	{
		podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
		nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
		break;
	}
#endif
	if(nomer==9999) return(1); //если не найден объект - выйти
	buf_ko[3]=podgr; // заполнить подгруппу
	for (j18=0;j18<=n_bait-6;j18++)
	if (j18==nomer) buf_ko[j18+4]=kod;//для найденного объекта - заполнить код
	else  buf_ko[j18+4]=124;// для прочих вписать заполнитель
#ifdef NEW_FORMAT
	buf_ko[9]=124;
#endif
	return(0);
}
//------------------------------------------------------------
/*****************************************************\
* Процедура определения кода команды, выдаваемой на   *
* объект nob с учетом наличия инверсии для данного    *
* объекта inv (0-нет инверсии).                       *
* Используется при нажатии различных кнопок           *
\*****************************************************/
int form_koda(int nob,int invr)
{
  int nobi=0;

	nom_func("73");

	//если объект типа кнопка+лампочка, то номер бита найти так
	if(fr1[nob][1]==53)nobi=fr1[nob][12]&0xf;
	else

		if((fr1[nob][1]==163)||//если текстовая кнопка
		((fr1[nob][1]>=155)&&(fr1[nob][1]<=158)))
		{
			for(nobi=0;nobi<5;nobi++)
			{
				if(fr1[nob][nobi+3]==9999)continue;
				if((fr1[nob][nobi+3]&0xff00)!=0)break;
			}
		}
		else
		{
				//если объект типа включение/выключение без ДОТ
				//или управление УКСПС, то номер бита искать так
			if((fr1[nob][1]!=166)&&(fr1[nob][1]!=27)&&(fr1[nob][1]!=16))nobi=fr1[nob][11];
			//во всех остальных случаях номер бита искать так
			else
#ifndef NEW_KNOPKA
			if(fr1[nob][1]==27)//укспс
				nobi=(fr1[nob][3]&0xf000)>>12;
			else
			nobi=(fr1[nob][11]&0xf000)>>12;
#else
			for(nobi=0;nobi<5;nobi++)
			{
				if(fr1[nob][nobi+3]==9999)continue;
				else
				if((fr1[nob][nobi+3]&0xff00)==0xff00)continue;
				else break;
			}
#endif
		}
 switch(nobi)
	{
		case 0: if(fr3[nob][nobi]==0)   //для сброшенного нулевого бита
						{
							if(invr==0)return('A');  //без инверсии
							else return('M');        //с инверсией
						}
						else  // для установленного нулевого бита
						{
							if(invr==0)return('M');   //без инверсии
							else return('A');         //с инверсией
						}
		case 1: if(fr3[nob][nobi]==0)    //для сброшенного первого бита
						{
							if(invr==0)return('B'); //без инверсии
							else  return('N');      //с инверсией
						}
						else         //для установленного первого бита
						{
							if(invr==0)return('N');  //без инверсии
							else  return('B');       //с инверсией
						}
	 default: return(9999);
	}

}
//------------------------------------------------------------------------
/*******************************************************\
* Процедура формирования команды открытия сигнала       *
* в раздельном режиме и для передачи в резервную ПЭВМ   *
* koda - код команды                                    *
* объект сигнала в глобальной переменной point          *
\*******************************************************/
void fotksig(int koda)
{

	int j,n_bait;

	nom_func("88");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;
	for(j=0;j<n_bait;j++)buf_ko_p[j]=0;//очистка буфера команд
	ot=0;
	vkl_kno(0,8); //кнопку отмены сделать серой
	if(form_kosig(koda)==1)
	{
		if(form_kodop(koda)==1)return;//если неудачное формирование - выход
	}
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//запись контрольной суммы
	pooo_com[point]=biostime(0,0L);//фиксация времени выдачи
	com_bit[point]=koda&3;  //выделить номер бита

	flagoutmsg=1;//установка требования для передачи в ТУМС

	//если был установлен признак маневрового сигнала
	if(manevro==1) manevro=0; //сброс маневрового признака
	for(j=0;j<11;j++)buf_ko_p[j]=buf_ko[j]; //перезапись в буфер обмена с ПЭВМ
	buf_ko_p[9]=check_sum(buf_ko_p,0);//запись контрольной суммы
	flms=1;//установка требования для передачи в резервную ПЭВМ
	return;
}
//-------------------------------------------------------------------------
/*************************************************************\
* Процедура определения номера основной спаренной стрелки для *
* второстепенной спаренной стрелки, заданной номером в point  *
\*************************************************************/
int Opred()
{
  int iks,jks,lom,FN=0,FIN=0;

	nom_func("202");
	if(fr1[point][13]==1){FN=0;FIN=STR1;}
#ifdef KOL_SOO2
  else{FN=STR1;FIN=FN+STR2;}
#endif
	if(fr1[point][12]!=9999)//если стрелка не одиночная
	{
		TEST_SPAR=2;
		lom=poisk_ras(fr1[point][2],point,0,0);//найти парную стрелку
	}
	//проверка наличия найденного номена в интерфейсе обмена
	for(iks=FN;iks<FIN;iks++)
		for(jks=0;jks<5;jks++)
			//если данный номер в интерфейсе - закончить
			if(point==spstr[iks][jks])return(0);
			//иначе
			else
				//если номер пары в интерфейсе - закончить
				if(spstr[iks][jks]==lom){point=lom;return(0);}
	//иначе закончить неудачей
	return(-1);
}

/*********************************************\
* Процедура анализа наличия готовых маршрутов *
* для записи в буфер маршрутных команд стойки *
\*********************************************/
void MARSH_GLOB_LOCAL(void)
{
	int i_m,i_s,s_m,kk,zz;
	char numer;
	unsigned char sum;
	for(i_s=0;i_s<skoko_stoek;i_s++)     //Пройти по всем стойкам станции
	{

		if(TUMS_RABOT[i_s]!=0)
		{
//			add(0,0,'Ы'); $$
			continue;
		}
		if(KOMANDA_TUMS[i_s][10]!=0)
		{
//			add(0,0,'Ъ'); $$
			continue; //если для стойки уже есть
																					//неподтвержденнная - продолжить
		}
		//Пройти по локальным маршрутам стойки
		for(s_m=0;s_m<MARS_STOY;s_m++)
		{
			//если есть команда в стойке i_s строке s_m и она еще не выдана в ТУМС
			if((MARSHRUT_ST[i_s][s_m].NEXT_KOM[0]!=0)&&
			(MARSHRUT_ST[i_s][s_m].T_VYD==0l))
			{
				//если состояние маршрута "разбит на локальные" и стойка не занята
				if((MARSHRUT_ST[i_s][s_m].SOST&0xF)==0x7)
				{
					//заполнить команду для ТУМС
					for(kk=0;kk<12;kk++)KOMANDA_TUMS[i_s][kk]=MARSHRUT_ST[i_s][s_m].NEXT_KOM[kk];
					//установить признак выдачи маршрута
					switch(MYTHX_TEC[i_s])
					{
						case 0x50: MYTHX_TEC[i_s]=0x60;
											 break;

						case 0x60: MYTHX_TEC[i_s]=0x70;
											 break;
						case 0:
						case 0x6e:
						case 0x70: MYTHX_TEC[i_s]=0x50;
											 break;
						default: break;
					}
					KOMANDA_TUMS[i_s][7]=KOMANDA_TUMS[i_s][7]|MYTHX_TEC[i_s];
					sum=0;
					for(kk=1;kk<10;kk++)sum=sum^KOMANDA_TUMS[i_s][kk];
					sum=sum|0x40;
					KOMANDA_TUMS[i_s][10]=sum;
					MARSHRUT_ST[i_s][s_m].T_VYD=time(NULL);
					//зафиксировать время выдачи маршрута
					KOMANDA_TUMS[i_s][14]=s_m; //запомнить строку локальной таблицы
					MARSHRUT_ST[i_s][s_m].NEXT_KOM[12]=MYTHX_TEC[i_s];//запомнить МИФ
					//зафиксировать время ожидания для маршрута
					kk=MARSHRUT_ST[i_s][s_m].NUM-100;
					kk=MARSHRUT_ALL[kk].KOL_STR[i_s]*20+10;
					for(zz=0;zz<MARS_STOY;zz++)
					{
						if(zz==s_m)continue;
						if(MARSHRUT_ST[i_s][zz].T_VYD!=0)
						{
							i_m=MARSHRUT_ST[i_s][zz].NUM-100;
							if((i_m<(skoko_stoek*3))&&(i_m>=0))
							{
								kk=kk+MARSHRUT_ALL[i_m].KOL_STR[i_s]*20+10;
							}
						}
					}
					MARSHRUT_ST[i_s][s_m].T_MAX=kk;
					i_m=MARSHRUT_ST[i_s][s_m].NUM-100;
//					numer=MARSHRUT_ST[i_s][s_m].NUM;
//					add(i_s,9999,numer); //записать на диск $$
//					numer=s_m|0x40;
//					add(i_s,9999,numer);
					break;//закончить работу с маршрутами стойки
				}
			}
		}
	}
}
/*****************************************************\
* Передача маршрутных команд в буфер вывода ТУМСов    *
\*****************************************************/
void MARSH_V_TUMS(int st)
{
	int i,n_bait;

	nom_func("527");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(KOMANDA_TUMS[st][0]==0)return;//если для стойки нет команды - выйти
	if(TUMS_RABOT[st]!=0)return;//если стойка в работе - выйти
	if(REG_COM_TUMS[st][0]!=0)return;
	if(DISK!=0)return;
#ifdef KOL_SOO2
	if((fr3[KM1][BitKM]!=0)||(fr3[KM2][BitKM]!=0)||
	(fr3[KM1][5]==1)||(fr3[KM1][5]==1))
#else
	//если макет установлен или непарафазен
	if((fr3[KM1][BitKM]!=0)||(fr3[KM1][5]==1))
#endif
	{
		if(strelka_v_makete()==0)
		{
			w(257,999,""); //"Не указана стрелка для включенного макета"
			goto met1;
		}
	}
	i=psk_obect(KOMANDA_TUMS[st]);
	if((i<kol_VO)&&(i>0))
	{
		if(DISK!=0)
		{
			if((KOMANDA_TUMS[st][2]=='i')||(KOMANDA_TUMS[st][2]=='j'))w(275,i,"");
			else w(132,i,"-марш");
		}
	}
	for(i=0;i<n_bait;i++)//заполнить регистр команд стойки
	{
		REG_COM_TUMS[st][i]=KOMANDA_TUMS[st][i];//переписать команду в буфер команд ТУМСа
		buf_zap_disk[i]=KOMANDA_TUMS[st][i];
		KOMANDA_TUMS[st][i]=0;
	}
	if((REG_COM_TUMS[st][0]=='(')&&(REG_COM_TUMS[st][n_bait-1]==')'))
	{//если есть командa
		if(N_RAZ[st]>=3)
		{
met1: for(i=0;i<n_bait;i++)buf_ko[i]=REG_COM_TUMS[st][i]=0;
			N_RAZ[st]=0;
			flagoutmsg=0;
			return;
		}
		else N_RAZ[st]++;//записать число сделанных передач
		t_com[st]=biostime(0,0l);//запомнить время выдачи

		for(i=0;i<n_bait;i++)//скопировать команду в буфер вывода
		{ if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//если до конца
			 BUF_OUT[st][ukaz_zap[st]++]=REG_COM_TUMS[st][i];//записать
		}
		kom_v_bufer_pvm(2,st);
		TUMS_RABOT[st]=0xf;
		add(st,0,'>');
		if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//если до конца
	}
	else return; //ЕСЛИ НЕТ КОМАНДЫ - ВЫЙТИ
	if(st==0)outportb(BAZ_ADR1+1,3); //РАЗРЕШИТЬ ПЕРЕДАЧУ В КАНАЛ
	if(st==1)outportb(BAZ_ADR11+1,3);
	otkaz_tu[st]=0;
	return;
}
//=======================================================================
void  Analiz_Glob_Marsh(void)
{
	int i_m, i_s, s_m,mars_st,ik,ob_str,polojen;
	time_t T_TIME;
	unsigned char kateg, Sost=0;
	T_TIME=time(NULL); //зафиксировать время
	for(i_m=0; i_m<(skoko_stoek*3); i_m++)// пройти по глобальной таблице маршутов
	{
		if(MARSHRUT_ALL[i_m].SOST==0)continue;//если строка пустая - к следующей
		kateg=0xC0&MARSHRUT_ALL[i_m].SOST; //взять категорию глобального маршрута
		mars_st=0; //считаем маршрут не розданным
		Sost=0x3f; //изначально расчитываем на успешное завершение

		for(i_s=0;i_s<skoko_stoek;i_s++) //пройти по всем стойкам
		{ if(MARSHRUT_ALL[i_m].STOYKA[i_s]!=0)	//если стойка участвует
			{
				for(s_m=0;s_m<MARS_STOY;s_m++)  	//пройти по всем локальным в стойке
				{
					if(MARSHRUT_ST[i_s][s_m].NUM==0)continue;//если нет - к следующему

					if((MARSHRUT_ST[i_s][s_m].NUM-100)==i_m) //если найдено соответствие
					{
						mars_st++; //увеличить счетчик стоек
						if(MARSHRUT_ST[i_s][s_m].T_VYD==0)//если не выдан-установить состояние
						MARSHRUT_ST[i_s][s_m].SOST=(MARSHRUT_ST[i_s][s_m].SOST&0xC)|0x7;
						if((MARSHRUT_ST[i_s][s_m].SOST&0x1f)!=0x1f)//если маршрут не воспринят
						{
#ifdef WORK
							if((T_TIME-MARSHRUT_ST[i_s][s_m].T_VYD)>2)//если прошло более 2 сек от выдачи
#else
							if((T_TIME-MARSHRUT_ST[i_s][s_m].T_VYD)>200)
#endif
							{
								if(MARSHRUT_ST[i_s][s_m].T_VYD!=0) //если фиксировалась выдача
								{
									if(KOL_VYD_MARSH[i_s]==0)  //если не было повторов
									{
										MARSHRUT_ST[i_s][s_m].T_VYD=0;//сбросить время выдачи
										MARSHRUT_ST[i_s][s_m].SOST=MARSHRUT_ST[i_s][s_m].SOST&0x7;
										KOL_VYD_MARSH[i_s]++;	//увеличить число выдач
										TUMS_RABOT[i_s]=0;
									}
									else
									{
//										add(i_m,8888,'0'); $$
										w(21,MARSHRUT_ALL[i_m].NACH,"! ВЫШЛО ВРЕМЯ.");
										DeleteMarsh(i_m);//удалить маршрут
										return;//прекратить анализы маршрутов
									}
								}
							}
						}
						else //если маршрут воспринят
						{
							//пройти по всем стрелкам воспринятого маршрута
							for(ik=0;ik<20;ik++)
							{ //получить очередную стрелку
								ob_str=MARSHRUT_ALL[i_m].STREL[i_s][ik]&0xfff;
								polojen=MARSHRUT_ALL[i_m].STREL[i_s][ik]&0x1000;
								if(ob_str==0)continue;//если ее нет, идти дальше
								//если стрелка без контроля - прервать просмотр
								if(fr3[ob_str][0]==fr3[ob_str][1])
							break; //без контроля - бросить анализ стрелок
								if(polojen==0)//если нужна в плюсе
								{
									if((fr3[ob_str][0]!=1)||(fr3[ob_str][1]!=0))
							break; //не в плюсе - бросить стрелки
								}
								else //если нужна в минусе
								{
									if((fr3[ob_str][0]!=0)||(fr3[ob_str][1]!=1))
							break; //не в минусе - бросить стрелки
								}
							}
							if(ik>=20) //если все стрелки установлены (маршрут выполнен)
							{
								if(kateg==0x40)//для предварительного
								{
									//установить для локального - норму завершения
									MARSHRUT_ST[i_s][s_m].SOST=
									MARSHRUT_ST[i_s][s_m].SOST|0x3f;
								}
							}
							else //если стрелки не готовы
							{
								MARSHRUT_ST[i_s][s_m].SOST=
								(MARSHRUT_ST[i_s][s_m].SOST&0xC0)|0x1f; //хранить восприятие
								if((T_TIME-MARSHRUT_ST[i_s][s_m].T_VYD)> //если превышен макс.
								MARSHRUT_ST[i_s][s_m].T_MAX)
								{
//									add(i_m,8888,'1');$$
									DeleteMarsh(i_m);
									return;
								}
							}
						}
						Sost=(Sost&MARSHRUT_ST[i_s][s_m].SOST);// новое состояние и прерваться
						break; //это был единственный локальный стойки для глобального
					}//конец обработки найденного локального
				}//конец всех локальных в стойке
			}//конец анализа участвующей стойки
		}//конец прохода по стойкам для данного глобального маршрута

		if(mars_st!=0)//если в маршрут входит хотя бы одна стойка
		{
			MARSHRUT_ALL[i_m].SOST=kateg|Sost; //формируем вклад стоек в глоб.знач.
			//если состояние маршрута = удачное завершение
			if((Sost&0x3F)==0x3f)
			{
				 //если завершился предварительный маршрут
				if(kateg==0x40)
				{
					for(i_s=0;i_s<skoko_stoek;i_s++)
					{
						if(MARSHRUT_ALL[i_m].STOYKA[i_s]!=0)
						if(TUMS_RABOT[i_s]!=0)return;
					}
					PovtorMarsh(i_m);
					return;
				}
				else //если был исполнительный маршрут
				{
//					add(i_m,8888,'2');$$
					DelMarsh(i_m);
					continue;
				}
			}
			else
			//если состояние неудачное
			if((Sost&0x3f)==0x1)
			{
//				add(i_m,8888,'3');$$
				w(21,MARSHRUT_ALL[i_m].NACH,"");
				DeleteMarsh(i_m);
				return;
			}
		}
	}
}
//==================================================================
//================================================
void DeleteMarsh(int i_m)
{
	int i_s,s_m,ii,jj,kk,strelka,napr,maska;
	char kateg;
	for(i_s=0;i_s<skoko_stoek;i_s++)//пройти по всем стойкам
	{
		MARSHRUT_ALL[i_m].KOL_STR[i_s]=0; //удалить счетчики стрелок для всех стоек
		MARSHRUT_ALL[i_m].STOYKA[i_s]=0; //удалить признаки вхождения стоек в маршрут
		for(s_m=0;s_m<20;s_m++) //пройти по таблицам стрелок,сигналов и СП_УП
		{
			strelka=MARSHRUT_ALL[i_m].STREL[i_s][s_m]&0xfff;//выделить номер стрелки
			if(strelka!=0) //если найден номер стрелки
			{
				pooo[strelka]=0l; //сбросить счетчик времени для стрелки
				MARSHRUT_ALL[i_m].STREL[i_s][s_m]=0;//удалить стрелку из таблицы
			}
			if(s_m<10)
			{
				MARSHRUT_ALL[i_m].SIG[i_s][s_m]=0;//удалить сигнал из таблицы
				MARSHRUT_ALL[i_m].SP_UP[i_s][s_m]=0; //удалить СП или УП из таблицы
			}
		}
	}
	if(MARSHRUT_ALL[i_m].KMND!='b')kateg=0; //был маневровый
	else kateg=1;//был поездной
	MARSHRUT_ALL[i_m].KMND=0;    //очистить ячейку команды
	ii=MARSHRUT_ALL[i_m].NACH;
	jj=ii;
	napr=fr1[ii][1];//направление сигнала
	maska=0;
	switch(kateg) //переключатель по категории
	{
		case 0: //маневровый
						switch(napr) //переключатель по направлению
						{
							case 0: maska=maska+0x40;break;//четный маневровый
							case 1: maska=maska+0x80;break;//нечетный маневровый
							default: break;
						}
						break;

		case 1: //поездной
						switch(napr) //переключатель по направлению
						{
							case 0: maska=maska+0x10;break;//четный поездной
							case 1: maska=maska+0x20;break;//нечетный поездной
							default: break;
						}
						break;

		default: break;
	}
	ZERO_TRASSA(2);
	kk=0;
	while(1)
	{
		TRASSA_RIS[kk++].object=ii;
		fr3[ii][10]=fr3[ii][10]&(~maska);
		if((fr3[ii][10]&0xF0)==0)fr3[ii][10]=0;
		ii=fr3[jj][8];
		fr3[jj][8]=0;
		jj=ii;
		if((ii<0)||(ii>kol_VO))break;
		if((ii==0)&&(fr3[ii][10]==0))break;
		if(kk>=100)break;
	}
	ris_trassa(2);
	ZERO_TRASSA(2);
	MARSHRUT_ALL[i_m].NACH=0;    //очистить ячейку начала
	MARSHRUT_ALL[i_m].NACH0=0;    //очистить ячейку начала
	MARSHRUT_ALL[i_m].END=0;	//очистить ячейку конца
	MARSHRUT_ALL[i_m].NSTR=0;	//очистить число стрелок
	MARSHRUT_ALL[i_m].POL_STR=0;	//очистить положение стрелок
	MARSHRUT_ALL[i_m].SOST=0;    //очистить учетчик состояния глобального маршрута

	for(i_s=0;i_s<skoko_stoek;i_s++)	//пройти по стойкам
	for(s_m=0;s_m<MARS_STOY;s_m++) //пройти по локальным в стойке
	{
		if((MARSHRUT_ST[i_s][s_m].NUM-100)==i_m) //если локальный для удаляемого
		{
			for(ii=0;ii<13;ii++)MARSHRUT_ST[i_s][s_m].NEXT_KOM[ii]=0; //удалить команду
			TUMS_RABOT[i_s]=0; // разблокирование стойки не воспринявший маршрут
			MARSHRUT_ST[i_s][s_m].NUM=0; //удалить номер глобального
			MARSHRUT_ST[i_s][s_m].SOST=0; //очистить состояние
			MARSHRUT_ST[i_s][s_m].T_VYD=0l; //очистить время выдачи
			MARSHRUT_ST[i_s][s_m].T_MAX=0l; //очистить время максимума
		}
	}
	return;
}

//==================================================================
//Повтор маршрута для формирования команды после установки стрелок
void PovtorMarsh(int i_m)
{
	unsigned int NACH=MARSHRUT_ALL[i_m].NACH; //взять начало маршрута
	unsigned int NACH0=MARSHRUT_ALL[i_m].NACH0;//взять вариантную точку
	int END=MARSHRUT_ALL[i_m].END; //взять конец маршрута
	switch(MARSHRUT_ALL[i_m].KMND) //сформировать признак категории маршрута
	{
		case 'a': mane1=1; break;
		case 'b': mane1=0; break;
		case 'd': mane1=1; break;
		default:  break;
	}
//	add(i_m,8888,'4'); //записать, что выдаем с открытием$$
	nach_marsh=NACH;
	nach0_marsh=NACH0;
	end_marsh=END;
//	poka=1;
//	vozvr=1;
//	klaval=1;
	DeleteMarsh(i_m);//удалить выполненный предварительный маршрут
	marsh1(1);
//	add(0,0,'3'); $$
	marsh2(1);
	nach_marsh=0;
	nach0_marsh=0;
	end_marsh=0;
	half_marsh=0;
	half_marsh1=0;
	return;
}
//===========================================================================
//===============================================================
//Анализ признаков состояния работы стоек с маршрутами по данным,
//принятым из стойки, здесь PODGR - байт с данными о MYTHX
int ANALIZ_MYTHX(unsigned char PODGR,int ST)
{
	int svoi,tum,s_m,ijk,zamena;
	char sym_myt[2];
	unsigned char prov;
	svoi=0;//изначально считаем, что MYTHX чужой
	zamena=0; //изначально считаем, что MYTHX не менялся
	tum=ST;
	if(ZAGRUZKA==1)
	{
		if((PODGR&0xf)==0x9)MYTHX_TEC[tum]=0x50;
		else
			if((PODGR&0xf)==0xA)MYTHX_TEC[tum]=0x60;
			else
				if((PODGR&0xf)==0xC)MYTHX_TEC[tum]=0x70;
				else	MYTHX_TEC[tum]=0x6e;
	}
	//переключатель по действующему для стойки маршруту
	switch(MYTHX_TEC[tum])
	{
		//если действуует 1-ый маршрут и стойка доложила о первом,то свой
		case 0x50: if((PODGR&0xf)==0x9)svoi=0xf;break;

		//если действует 2-ой и стойка доложила о втором, то свой
		case 0x60: if((PODGR&0xf)==0xA)svoi=0xf;break;

		//если нет никакого, то все свои
		case 0x6e:
		case 0:     svoi=0xf;break;

			//если действует 3-ий и стойка доложила о третьем, то свой
		case 0x70: if((PODGR&0xf)==0xC)svoi=0xf;break;
		default: break;
	}
	if(PODGR==0x6e)
	{
		svoi=0xf;
		TUMS_RABOT[tum]=0;
		KOL_VYD_MARSH[tum]=0;
	}

	if(svoi!=0)//если свой
	{
		//если изменился
		if(MYTHX[tum]!=PODGR)
		{
			zamena=1;
			for(ijk=0;ijk<12;ijk++)REG_COM_TUMS[tum][ijk]=0;
		}

		for(s_m=0;s_m<MARS_STOY;s_m++) // пройти по всем маршрутам стойки
		{
			//выделить MYTHX из подгруппы и наложить на MYTHX команды
			// в 12 - ом байте лежит MYTHX, с которым шла команда
			// в подгруппе в младшем полубайте лежит MYTHX действующий в ТУМСе
			// 59 - 1ый выдан и первый в работе
			// 6A - второй выдан и второй в работе
			// 7C - третий выдан и третий в работе
			prov=MARSHRUT_ST[tum][s_m].NEXT_KOM[12]|(PODGR&0xf);
			// если миф в стойке соответствует выданному в команде
			if((prov==0x59)||(prov==0x6A)||(prov==0x7C))
			{
				switch(PODGR&0xF0)
				{            //маршрут в работе
										 //установить состояние восприятия и убрать команду
					case 0x70: MARSHRUT_ST[tum][s_m].SOST=
										 (MARSHRUT_ST[tum][s_m].SOST&0xC0)|0x1f;
										 for(ijk=0;ijk<15;ijk++)KOMANDA_TUMS[tum][ijk]=0;
										 KOL_VYD_MARSH[tum]=0;
										 TUMS_RABOT[tum]=0xf; //выставить флаг ТУМС в работе
										 ZAMKNI_FR3(MARSHRUT_ST[tum][s_m].NUM-100);
										 break;

										 //неудачное окончание
					case 0x50: MARSHRUT_ST[tum][s_m].SOST=0x1; //установить неудачу
										 //сбросить команду
										 for(ijk=0;ijk<15;ijk++)KOMANDA_TUMS[tum][ijk]=0;
										 KOL_VYD_MARSH[tum]=0;
										 //убрать флаг ТУМС в работе
										 TUMS_RABOT[tum]=0;
										 break;

										 //удачное окончание
					case 0x60: MARSHRUT_ST[tum][s_m].SOST=
										 (MARSHRUT_ST[tum][s_m].SOST&0xC0)|0x1f;
										 for(ijk=0;ijk<15;ijk++)KOMANDA_TUMS[tum][ijk]=0;
										 TUMS_RABOT[tum]=0;
										 break;

						default: break;
				}
				break;//завершить работу с маршрутами стойки - это был единственный
			}
			else //если мифы не соответствуют
			{
				switch(PODGR&0xF0)
				{
					case 0x70: TUMS_RABOT[tum]=0xf;break; //стойка занята

					default: TUMS_RABOT[tum]=0; break;
				}

			}
		} //конец прохода по локальным маршрутам
	}
	if(svoi!=0)MYTHX[tum]=PODGR; //если был свой MYTHX, запомнить его
/*
	switch(MYTHX[tum]&0xf)
	{
		case 0x9: sym_myt[0]='1';break;
		case 0xa:	sym_myt[0]='2';break;
		case 0xc: sym_myt[0]='3';break;
		default:  sym_myt[0]=MYTHX[tum];break;
	}

	sym_myt[1]=0;
	setcolor(7);
	outtextxy(543,420+tum*20,"█");
	switch(MYTHX[tum]&0xf0)
	{
		case 0x70: setcolor(14);break;//в работе
		case 0x60: setcolor(10);break;//удачный конец
		case 0x50: setcolor(12);break;//неудачный конец
		default:	 setcolor(15);break;//неопределенность
	}
	outtextxy(543,420+tum*20,sym_myt);

	setcolor(7);
	outtextxy(553,420+tum*20,"█");
	switch(MYTHX_TEC[tum]&0xf0)
	{
		case 0x70: sym_myt[0]='3';setcolor(15);break;//задан третий
		case 0x60: sym_myt[0]='2';setcolor(15);break;//задан второй
		case 0x50: sym_myt[0]='1';setcolor(15);break;//задан первый
		default:	 sym_myt[0]='?';setcolor(15);break;//неопределенность
	}
	outtextxy(553,420+tum*20,sym_myt);
*/
	if(MYTHX[tum]==0)//
	{
		MYTHX[tum]=PODGR;//если первый прием - запомнить
		return(1);
	}
	return(zamena);
}
//******************************************
void ZAMKNI_FR3(int i_m)
{
	int jj,ii;
	jj=0;
	ii=MARSHRUT_ALL[i_m].NACH;
	while(1)
	{
		fr3[ii][10]=fr3[ii][10]|3;
		ii=fr3[ii][8];
		TRASSA_RIS[jj++].object=ii;
		if((ii==0)&&(fr3[ii][8]==0))break;
		if(ii>kol_VO)break;
		if(jj>=100)break;
	}
	ris_trassa(2);
	ZERO_TRASSA(2);
}
//================================================
void DelMarsh(int i_m)
{
	int i_s,s_m,ii,strelka;

	for(i_s=0;i_s<skoko_stoek;i_s++)//пройти по всем стойкам
	{
		MARSHRUT_ALL[i_m].KOL_STR[i_s]=0; //удалить счетчики стрелок для всех стоек
		MARSHRUT_ALL[i_m].STOYKA[i_s]=0; //удалить признаки вхождения стоек в маршрут
		for(s_m=0;s_m<20;s_m++) //пройти по таблицам стрелок,сигналов и СП_УП
		{
			strelka=MARSHRUT_ALL[i_m].STREL[i_s][s_m]&0xfff;//выделить номер стрелки
			if(strelka!=0) //если найден номер стрелки
			{
				pooo[strelka]=0l; //сбросить счетчик времени для стрелки
				MARSHRUT_ALL[i_m].STREL[i_s][s_m]=0;//удалить стрелку из таблицы
			}
			if(s_m<10)
			{
				MARSHRUT_ALL[i_m].SIG[i_s][s_m]=0;//удалить сигнал из таблицы
				MARSHRUT_ALL[i_m].SP_UP[i_s][s_m]=0; //удалить СП или УП из таблицы
			}
		}
	}
	MARSHRUT_ALL[i_m].KMND=0;    //очистить ячейку команды
	MARSHRUT_ALL[i_m].NACH=0;    //очистить ячейку начала
	MARSHRUT_ALL[i_m].NACH0=0;    //очистить ячейку начала
	MARSHRUT_ALL[i_m].END=0;	//очистить ячейку конца
	MARSHRUT_ALL[i_m].NSTR=0;	//очистить число стрелок
	MARSHRUT_ALL[i_m].POL_STR=0;	//очистить положение стрелок
	MARSHRUT_ALL[i_m].SOST=0;    //очистить учетчик состояния глобального маршрута

	for(i_s=0;i_s<skoko_stoek;i_s++)	//пройти по стойкам
	for(s_m=0;s_m<MARS_STOY;s_m++) //пройти по локальным в стойке
	{
		if((MARSHRUT_ST[i_s][s_m].NUM-100)==i_m) //если локальный для удаляемого
		{
			for(ii=0;ii<13;ii++)MARSHRUT_ST[i_s][s_m].NEXT_KOM[ii]=0; //удалить команду
			TUMS_RABOT[i_s]=0; //$$$$ 13_04_07 - разблокирование стойки не воспринявший маршрут
			MARSHRUT_ST[i_s][s_m].NUM=0; //удалить номер глобального
			MARSHRUT_ST[i_s][s_m].SOST=0; //очистить состояние
			MARSHRUT_ST[i_s][s_m].T_VYD=0l; //очистить время выдачи
			MARSHRUT_ST[i_s][s_m].T_MAX=0l; //очистить время максимума
		}
	}
	return;
}
//==============================================================
void ZAPOLNI_MARSH(int tipus,int i)
{
	int jj,kol_str[skoko_stoek],ind_str[skoko_stoek],ind_sig[skoko_stoek];
	int n_strel,obect,nop,stoyka,strlk,stk,dz;
	unsigned long pol_strel;
	for(jj=0;jj<skoko_stoek;jj++)
	{
			kol_str[jj]=0;
			ind_str[jj]=0;
			ind_sig[jj]=0;
	}
	pol_strel=0l;
	n_strel=0;
	jj=0;
	while(1)
	{
		if((tipus!=0)&&(TRASSA[jj].object==0xFFFF))break;
		if((tipus==0)&&(TRASSA0[jj].object==0xFFFF))break;
		if(tipus!=0)
		{
			obect=TRASSA[jj].object&0xfff;
			nop=fr1[nach_marsh][1];
		}
		else
		{
			obect=TRASSA0[jj].object&0xfff;
			nop=fr1[nach_marsh_oper][1];
		}
		if(fr1[obect][0]==2)//если сигнал
		{
			if(fr1[obect][1]==nop)//если  данного направления
			{
				if(((tipus==0)&&(mane==1))||
					((tipus==1)&&(mane1==1)))
				 //если маневровый маршрут
				{
					if(fr1[obect][6]!=1)//если есть маневровый огонь
					{
						stoyka=fr1[obect][13]-1;
						MARSHRUT_ALL[i].SIG[stoyka][ind_sig[stoyka]++]=obect;
						if(ind_sig[stoyka]>=10)ind_sig[stoyka]=9;
					}
				}
				else
				{
					if(fr1[obect][6]!=0)//если есть поездной огонь
					{
						stoyka=fr1[obect][13]-1;
						MARSHRUT_ALL[i].SIG[stoyka][ind_sig[stoyka]++]=obect;
						if(ind_sig[stoyka]>=10)ind_sig[stoyka]=9;
					}
				}
			}
		}
		if(((tipus!=0)&&(fr1[TRASSA[jj].object&0xfff][0]==1))||//если стрелка
		((tipus==0)&&(fr1[TRASSA0[jj].object&0xfff][0]==1)))
		{
			if(tipus!=0)strlk=TRASSA[jj].object&0xfff;
			else strlk=TRASSA0[jj].object&0xfff;
			stk=fr1[strlk][13]-1;
			//если противошерстная
			if(((tipus!=0)&&((TRASSA[jj].object&0x4000)==0x4000))||
			((tipus==0)&&((TRASSA0[jj].object&0x4000)==0x4000)))
			{
				 //если нет глушения
				if(((tipus!=0)&&((TRASSA[jj].object&0x2000)!=0x2000))||
				((tipus==0)&&((TRASSA0[jj].object&0x2000)!=0x2000)))
				{ //если нужна в минусе
					if(((tipus!=0)&&((TRASSA[jj].object&0x8000)==0x8000))||
					((tipus==0)&&((TRASSA0[jj].object&0x8000)==0x8000)))
					{
						pol_strel=pol_strel|(1<<n_strel);
						if((fr3[strlk][0]==1)||(fr3[strlk][1]==0))kol_str[stk]++;
						MARSHRUT_ALL[i].STREL[stoyka][ind_str[stoyka]++]=obect|0x1000;
						if(ind_str[stoyka]>=20)ind_str[stoyka]=19;
					}
					else //если нужна в плюсе
					{
						if((fr3[strlk][0]==0)||(fr3[strlk][1]==1))kol_str[stk]++;
						MARSHRUT_ALL[i].STREL[stoyka][ind_str[stoyka]++]=obect;
						if(ind_str[stoyka]>=20)ind_str[stoyka]=19;
					}
					n_strel++;
				}
			}
			else //если пошерстная
			{  //если нет глушения
				if(((tipus!=0)&&((TRASSA[jj].object&0x2000)!=0x2000))||
				((tipus==0)&&((TRASSA0[jj].object&0x2000)!=0x2000)))
				{ //если нужна в минусе
					if(((tipus!=0)&&((TRASSA[jj].object&0x8000)==0x8000))||
					((tipus==0)&&((TRASSA0[jj].object&0x8000)==0x8000)))
					{
						if((fr3[strlk][0]==1)||(fr3[strlk][1]==0))kol_str[stk]++;
						MARSHRUT_ALL[i].STREL[stoyka][ind_str[stoyka]++]=obect|0x1000;
						if(ind_str[stoyka]>=20)ind_str[stoyka]=19;
					}
					else //если нужна в плюсе
					{
						if((fr3[strlk][0]==0)||(fr3[strlk][1]==1))kol_str[stk]++;
						MARSHRUT_ALL[i].STREL[stoyka][ind_str[stoyka]++]=obect;
						if(ind_str[stoyka]>=20)ind_str[stoyka]=19;
					}
				}
			}
		}
		if(((tipus!=0)&&(fr1[TRASSA[jj].object&0xfff][0]==7))||//если ДЗ
		((tipus==0)&&(fr1[TRASSA0[jj].object&0xfff][0]==7)))
		{
			strlk=0;
			if(tipus!=0)dz=TRASSA[jj].object&0xfff;
			else dz=TRASSA0[jj].object&0xfff;
			if(fr1[dz][1]==14)strlk=fr1[dz][3];
			if(fr1[dz][1]==15)
			{
				if((tipus==0)&&(mane==0))strlk=fr1[dz][3];
				if((tipus==1)&&(mane1==0))strlk=fr1[dz][3];
			}
			if(strlk!=0)
			{
				stk=fr1[strlk][13]-1;
				if(fr1[dz][4]==1) //если стрелка нужна в плюсе
				{
					if((fr3[strlk][0]==0)||(fr3[strlk][1]==1))kol_str[stk]++;
					MARSHRUT_ALL[i].STREL[stoyka][ind_str[stoyka]++]=strlk;
					if(ind_str[stoyka]>=20)ind_str[stoyka]=19;
				}
				else//если нужна в минусе
				{
					if((fr3[strlk][0]==1)||(fr3[strlk][1]==0))kol_str[stk]++;
					MARSHRUT_ALL[i].STREL[stoyka][ind_str[stoyka]++]=strlk|0x1000;
					if(ind_str[stoyka]>=20)ind_str[stoyka]=19;
				}
			}
		}
		if((tipus!=0)&&((TRASSA[jj++].object&0xfff)==end_marsh))break;
		if((tipus==0)&&((TRASSA0[jj++].object&0xfff)==end_marsh_oper))break;
		if(jj>=100)break;
	}
	if(tipus==0)
	{
		if(mane==1)MARSHRUT_ALL[i].KMND='a';
		else MARSHRUT_ALL[i].KMND='b';
	}
	if(tipus==1)
	{
		if(mane1==1)MARSHRUT_ALL[i].KMND='a';
		else MARSHRUT_ALL[i].KMND='b';
	}
	MARSHRUT_ALL[i].NSTR=n_strel;
	MARSHRUT_ALL[i].POL_STR=pol_strel;
	for(jj=0;jj<skoko_stoek;jj++)MARSHRUT_ALL[i].KOL_STR[jj]=kol_str[jj];
	ris_trassa(tipus);
	if(half_marsh1!=0)vozvr=0;
	if((vozvr==0)&&(tipus==0))voper();
	else  poka=0;
}
//-----------------------------------------------------------------
/*****************************************************\
* Процедура поиска объекта для команды, полученной из *
* канала ДЦ. buf[11] - буфер команды ДЦ.              *
\*****************************************************/
int psk_obect(unsigned char buf[12])
{
	int i,st,N_pod;

	nom_func("230");
	if((buf[1]&0x3)==0x1)st=0;
	else
		if((buf[1]&0x3)==0x2)st=1;
		else return(-1);
	N_pod=buf[3]-0x30;
	switch(st)//если стойка 1
	{
		case 0: if(N_pod<STR1)goto strelka;
						else if(N_pod<(STR1+SIG1))
								 {N_pod=N_pod-STR1;goto signal;}
								 else if(N_pod<(STR1+SIG1+UCH1))
											{N_pod=N_pod-STR1-SIG1;goto razdel;}
											else if(N_pod<(STR1+SIG1+UCH1+PUT1))return(-1);
													 else if(N_pod<(STR1+SIG1+UCH1+PUT1+UPR1))
																{N_pod=N_pod-STR1-SIG1-UCH1-PUT1;goto kontr;}
																else return(-1);

		case 1: if(N_pod<STR2)goto strelka;
						else if(N_pod<(STR2+SIG2))
								 {N_pod=N_pod-STR2;goto signal;}
								 else if(N_pod<(STR2+SIG2+UCH2))
											{N_pod=N_pod-STR2-SIG2;goto razdel;}
											else if(N_pod<(STR2+SIG2+UCH2+PUT2))return(-1);
													 else if(N_pod<(STR2+SIG2+UCH2+PUT2+UPR2))
																{N_pod=N_pod-STR2-SIG2-UCH2-PUT2;goto kontr;}
																else return(-1);

		default:  return(-1);
	}
strelka:
	if(buf[2]!='C')return(-1);
	if(st==1)N_pod=N_pod+STR1;
	for(i=0;i<5;i++)if(buf[4+i]!=124)break;
	if(i==5)return(-1);
	return(spstr[N_pod][i]);
signal:
	if((buf[2]=='a')||(buf[2]=='b')||(buf[2]=='i')||(buf[2]=='j')||
	(buf[2]=='d'))goto marsh;
	if(buf[2]!='E')return(-1);
	 if(st==1)N_pod=N_pod+SIG1;
	for(i=0;i<5;i++)if(buf[4+i]!=124)break;
	if(i==5)return(-1);
	return(spsig[N_pod][i]);
razdel:
	if(buf[2]!='F')return(-1);
	if(st==1)N_pod=N_pod+UCH1;
	for(i=0;i<5;i++)if(buf[4+i]!=124)break;
	if(i==5)return(-1);
	return(spspu[N_pod][i]);
kontr:
	if(buf[2]!='C')return(-1);
	if(st==1)N_pod=N_pod+UPR1;
	for(i=0;i<5;i++)if(buf[4+i]!=124)break;
	if(i==5)return(-1);
	return(spkont[N_pod][i]);
marsh:
	i=buf[4]&0xBF;
	if(st==1)N_pod=N_pod+SIG1;
	nach_marsh=spsig[N_pod][i];
	N_pod=buf[5]-0x30;
	switch(st)
	{
		case 0: if(N_pod<(STR1+SIG1)){N_pod=N_pod-STR1;break;}
						else return(-1);

		case 1: if(N_pod<(STR2+SIG2)){N_pod=N_pod-STR2;break;}
						else return(-1);
		default:  return(-1);
	}
	i=buf[6]&0xBF;
	if(st==1)N_pod=N_pod+SIG1;
	end_marsh=spsig[N_pod][i];
	if(((buf[7]&0xf0)>=0x50)&&((buf[7]&0xf0)<=0x70))return(nach_marsh);
	else
	{
		N_pod=buf[7]-0x30;
		switch(st)
		{
			case 0: if(N_pod<(STR1+SIG1)){N_pod=N_pod-STR1;goto sig;}
							else if(N_pod<(STR1+SIG1+UCH1)){N_pod=N_pod-STR1-SIG1;goto sp;}
										else return(-1);

			case 1: if(N_pod<(STR2+SIG2)){N_pod=N_pod-STR2;goto sig;}
							else if(N_pod<(STR2+SIG2+UCH2)){N_pod=N_pod-STR2-SIG2;goto sp;}
									 else return(-1);
			default:  return(-1);
		}
sp: i=buf[8]&0xBF;
		if(st==1)N_pod=N_pod+UCH1;
		half_marsh=spspu[N_pod][i];return(nach_marsh);
sig:i=buf[8]&0xBF;
		if(st==1)N_pod=N_pod+SIG1;
		half_marsh=spsig[N_pod][i];return(nach_marsh);
	}
}
