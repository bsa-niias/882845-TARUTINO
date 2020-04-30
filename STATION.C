#include "bla.h"
/******************************************************************\
*    Процедура проверки исходного состояния объектов разделки,     *
*    выполняемая перед выдачей команды для следующего обекта       *
*    разделки (проверяется наличие несброшенной предварительной)   *
* nom - номер объекта управления (в markery) для этого элемента    *
\******************************************************************/
int all_null(int nom)
{
	int iqc,vot_on=0;
	vot_on=markery[nom][6];
	for(iqc=Start_R;iqc<Start_R+WAZO;iqc++)
	if((vot_on!=iqc)&&(fr3[iqc][0]==1))return(1); //другой объект в разделке
	return(0); //других объектов в разделке нет
}
/*************************************************\
* Процедура анализа состояния таймеров системы    *
* и выработки действий в зависимости от состояния *
* таймеров                                        *
\*************************************************/
void analiz_time()
{
	int i,obekt,tip;
  long tim,tt;

	nom_func("9");
	tim=biostime(0,0l); //получить текущее время
  for(i=0;i<25;i++) //пройти по всем возможным таймерам
  {
    if(TIMER_N[i][0]!=0)//если обнаружен включенный таймер
    {
			obekt=TIMER_N[i][0]; // взять номер объекта,для которого запущен таймер
      tip=TIMER_N[i][1];   // получить тии объекта
      if(((tim-pooo[obekt])>70L)||   // если время ожидания вышло или
			(tip==22)||(tip==21)||(tip==3))//стрелка в пути в другой стойке - 22 или
                                     //событие ожидания неисправности - 21 или
                                     //огневое реле - 3
      switch(tip) //действовать в зависимости от типа ожидаемого события
      {
        case 30:  //если ожидание исполнительной части ответственной команды
									tt=tim-pooo[obekt];
									if(DISK!=0)tt=tt*koef_uskor;
									if(tt>360L)
									{
										if(pooo_com[obekt]!=0l)
										{
											zvuk_vkl(1,9l);
											w(239,obekt," НЕ ИСПОЛНЕНА");
										}
										TIMER_N[i][0]=0;
										TIMER_N[i][1]=0;
										TIMER_N[i][2]=0;
										pooo_com[obekt]=0l;
										com_bit[obekt]=0;
									}
									break;
				case 1: // если тип - аварийный перевод стрелок
								tt=tim-pooo_com[obekt];
								if(DISK!=0)tt=tt*koef_uskor;
								if(tt>360)// если время ожидания вышло
								{
									AVARI=0;  avap=0;
									AVARIIN=0;
									pooo_com[AVAch]=0l;
									com_bit[AVAch]=0;
									pooo_com[AVAn]=0l;
									com_bit[AVAn]=0;
									vkl_kno(AViA,8);
									vibeg=0;
									vib_ma=0;
									AV_zdem=0;
									oper=0;
                  zvuk_vkl(1,20);
                  t(0);
                  w(220,obekt," ВРЕМЯ ИСТЕКЛО");
									TIMER_N[i][0]=0;
                  TIMER_N[i][1]=0;
                  TIMER_N[i][2]=0;
                }
                break;
        case 2: // если тип - анализ исправности огневых пригласительных
								if(pooo[obekt]!=0)
                {
									tt=tim-pooo[obekt];
									if(DISK!=0)tt=tt*koef_uskor;
									if(tt>36L)// если время ожидания вышло
									{
										obekt=TIMER_N[i][0];
										ps_pso(obekt,5);
										pooo[TIMER_N[i][0]]=0l;
                  }
                }
								if(pooo[obekt]==0)
                {
									TIMER_N[i][0]=0;
                  TIMER_N[i][1]=0;
                  TIMER_N[i][2]=0;
                }
                break;
        case 3: // если тип - анализ исправности огневых входных соседа
								tt=tim-pooo[obekt];
								if(DISK!=0)tt=tt*koef_uskor;
								if(tt>36L)// если время ожидания вышло
								{

									obekt=TIMER_N[i][0];
									lru_lko(obekt,TIMER_N[i][2],5);
									pooo[TIMER_N[i][0]]=0l;
									TIMER_N[i][0]=0;
									TIMER_N[i][1]=0;
									TIMER_N[i][2]=0;
								}
								break;
			 case 20:// если работаем с комплексными лампами переезда ЧДК
							 //	- 10 сек задержка
								tt=tim-pooo[obekt];
								if(DISK!=0)tt=tt*koef_uskor;
								if(tt>540L)
								{
									lampa_N(obekt,TIMER_N[i][2],1);
									TIMER_N[i][0]=0;
									TIMER_N[i][1]=0;
									TIMER_N[i][2]=0;
									pooo[obekt]=0l;
								}
								break;

			 case 21://если работаем с комплексной лампой
								//не открывается > 2с
								tt=tim-pooo[obekt];
								if(DISK!=0)tt=tt*koef_uskor;
								if(tt>36L)
								{
									lampa_3c(obekt,TIMER_N[i][2],1);
									TIMER_N[i][0]=0;
									TIMER_N[i][1]=0;
									TIMER_N[i][2]=0;
									pooo[obekt]=0l;
								}
								break;

				 case 66://если работаем с переездом
								//не открывается > 2с
								tt=tim-pooo_com[obekt];
								if(DISK!=0)tt=tt*koef_uskor;
								if(tt>170L)
								{
									pooo_com[obekt]=0l;
									home(modi);
									com_bit[obekt]=0;
									TIMER_N[i][0]=0;
									TIMER_N[i][1]=0;
									TIMER_N[i][2]=0;
									t(0);
									flagoutmsg=0;
								}
								break;

				case 18://огневое реле
								tt=tim-pooo[obekt];
								if(DISK!=0)tt=tt*koef_uskor;
								if(tt>36L)
								{
									//fr3[obekt][11]=fr3[obekt][
									ognev(obekt,TIMER_N[i][2],5);
 									TIMER_N[i][0]=0;
									TIMER_N[i][1]=0;
									TIMER_N[i][2]=0;
									pooo[obekt]=0l;
								}
								break;

				case 22: //таймер для перевода стрелок в другой стойке
									point=TIMER_N[i][0];
									pooo_com[point]=biostime(0,0l);
									oper=fr3[point][7];
									com_bit[point]=oper&3;
									fr3[point][7]=0;
									TIMER_N[i][0]=0;
									TIMER_N[i][1]=0;
									TIMER_N[i][2]=0;
									formkps(oper);//сформировать команду на перевод стрелки
									flagoutmsg=1;
									break;
#ifdef OGRAD
				case  46: //ограждение
									tt=tim-pooo[obekt];
									if(DISK!=0)tt=tt*koef_uskor;
									if(tt>70L)// если время ожидания вышло
									{
										analiz_ogr(TIMER_N[i][2],0);// выполнить анализ состояния
										pooo[obekt]=0l;
										TIMER_N[i][0]=0;
										TIMER_N[i][1]=0;
										TIMER_N[i][2]=0;
									}
									break;
#endif
#ifdef POLUAVTOMAT
				case 51:  // если тип=полуавтоматика-отправление
									tt=tim-pooo[obekt];
									if(DISK!=0)tt=tt*koef_uskor;
									if(tt>70L)// если время ожидания вышло
									{
										analiz_otprav(TIMER_N[i][0],0);// выполнить анализ состояния
									}
									break;
				case 52:    // если тип=полуавтоматика-прием
									tt=tim-pooo[obekt];
									if(DISK!=0)tt=tt*koef_uskor;
									if(tt>70L)// если время ожидания вышло
									{
										analiz_priem(TIMER_N[i][0],0);// выполнить анализ состояния
									}
									break;

#endif
				default:  break;
			}
			if(tip==22)break; //если перевод стрелки в пути в другой стойке
		}
cikl:
	}
#ifndef NEW_FORMAT
	for(i=0;i<skoko_stoek;i++) //пройти по всем стойкам
	{
		j=0;
		if(komanda2[i]!=0) //если в стойку выдавалась маршрутная команда
		{
			tt=tim-pooo_com[komanda2[i]]; //вычисляем время,прошедшее после выдачи
			if(DISK!=0)tt=tt*koef_uskor;
			if(tt>1400)//если прошло более 80 сек
			{
				tek=komanda2[i]; //получаем объект начала маршрута
				while(1)         //"бесконечный" цикл
				{
					sled=fr3[tek][8]&0xfff; //получить следующий объект
					if((sled<0)||(sled>=kol_VO))break;
					if((fr3[tek][10]==0)&&(fr3[8]==0))break; //если последний - прервать
					fr3[tek][8]=0;
					if(fr3[tek][10]!=0)
					fr3[tek][10]=0;
					if(sled==0xfff)break; //если следующего нет - прервать
					tek=sled;             //следующий сделать текущим
				}
				komanda2[i]=0;  //сбросить фиксацию выдачи маршрутной команды
				if(marshrut[i-1][0]!=0) //сбросить память маршрута для стойки
				{
					marshrut[i-1][0]=0;
					marshrut[i-1][1]=0;
					marshrut[i-1][2]=0;
				}
			}
		}
		for(j=0;j<5;j++) //пройти по массиву памяти желаемого положения стрелок
		{
			if((strelki[i][0]!=0)&&(pooo_com[strelki[i][0]]!=0)) //если ждем перевода
			{
				if((biostime(0,0l)-pooo_com[strelki[i][0]])>740) //если время ожидания вышло
				{
					pooo_com[strelki[i][0]]=0l; //сбросить ожидание перевода
					com_bit[strelki[i][0]]=0;
					strelki[j][0]=0;
					strelki[j][1]=0;
					strelki[j][2]=0;
					w(21,999,"(не установлены стрелки)");
				}
			}
		}
	}
	if((komanda2[0]==0)&&(komanda2[1]==0)&&(vozvr==0))
	//если нет маршрутов
	if((marshrut[0][0]==0)&&(marshrut[1][0]==0))
	//со всех объектов снять замыкания
	for(i=0;i<kol_VO;i++)
	if(fr3[i][10]!=0)
	{
		fr3[i][10]=0;
		fr3[i][8]=0;
	}
#endif
	return;
}
//---------------------------------------------------------------
/***********************************************\
*   Процедура формирования команд для перевода  *
*   режимов работы сигналов "день - ночь",      *
*   включается в работу после второго нажатия   *
*   кнопки,что является ответом на вопрос.      *
\***********************************************/
void dn_au(int nom)
{
  int ob_au,ob_dn,ob_nn;
  ob_nn=fr1[nom][3]; //объект для выдачи команды включения ночного реж.
	ob_dn=fr1[nom][5]; //объект для выдачи команды включения дневн.реж.
	ob_au=fr1[nom][9]; //номер объекта для включения режима "автомат"

	nom_func("43");
	if(klaval==13)
	{ ob_nn=fr1[nom][3];
		ob_dn=fr1[nom][5];
		ob_au=fr1[nom][9];
    if(ob_au!=9999)//если есть кнопка "автомат"
    { if(nom==ob_au)//если нажата кнопка "автомат"
      {
				if(COD_DN>7) //если автоматический режим уже установлен
        { w(202,999,"");return;} //сообщаем об этом и уходим

				// "выдана команда установить режим "автомат""
        else  w(206,999,"");
      }
    }
    if((nom==ob_nn)||(nom==ob_dn))//если нажата кнопка режима
    {
      //если дневной режим не установлен
      if((COD_DN==2)||(COD_DN==3)||(COD_DN==8))

      //выдана команда включить дневной режим
			{w(207,999,"");nom=ob_dn;}
      else

        //если установлен дневной режим
        if((COD_DN==4)||(COD_DN==5)||(COD_DN==9))
        //выдана команда включить ночной режим
				{w(208,999,"");nom=ob_nn;}

        //иначе
        //НЕИСПРАВНА СХЕМА ПЕРЕКЛЮЧЕНИЯ РЕЖИМОВ СИГНАЛОВ
        else {w(209,999,"");return;}
    }
    oppp=0; puti=0;
    if(nom==ob_nn)nom=ob_dn;
    else
      if(nom==ob_dn)nom=ob_nn;
    form_kom_knopka(nom,0);
    home(modi);
    return;
  }
  else return;
}
//-------------------------------------------------------------------
/************************************************************\
* Процедура включения разделки объекта nom типа РИ, выдается *
* дважды в качестве предварительной и исполнительной.        *
\************************************************************/
void first_razd(int nom,int tip)
{
	int n_bait,nomer,podgr,n,i17,j17,SDVIG=0,FN=0,FIN=0;

	nom_func("62");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	//сформировать код группы для РИ-объекта
	buf_ko[2]='F';

	//получить номер в базе для объекта выбранного маркером
	n=markery[nom][6];

	//для объекта из первой стойки
	if(fr1[n][13]==1)
	{
		//записать заголовок
		buf_ko[1]=ZAGO_MPSU(0x61);

		//получить смещение на СП и РИ
		SDVIG=STR1+SIG1+DOP1;

		//получить конец для объектов СП и РИ
		FN=0;
		FIN=UCH1;
	}
#ifdef KOL_SOO2
	//то же для второй стойки
	else
	{
		buf_ko[1]=ZAGO_MPSU(0x62);
		SDVIG=STR2+SIG2+DOP2;
		FN=UCH1;
		FIN=FN+UCH2;
	}
#endif
	nomer=9999; //начать поиск объекта с несуществующего номера

	//пройти по подгруппам группы СП+РИ
	for (i17=FN;i17<FIN;i17++)

	//пройти по объектам каждой подгруппы
	for(j17=0;j17<=4;j17++)

	//если объект найден
	if (spspu[i17][j17]==n)
	{
		podgr=podgruppa[i17+SDVIG-FN];//получить код подгруппы
		nomer=j17; //запомнить порядковый номер объекта в подгруппе
		break;
	}
	if(nomer==9999) return;//если объект не найден - выход

	buf_ko[3]=podgr;//для найденного объекта записать код подгруппы

	for (i17=0;i17<=4;i17++)
	if (i17==nomer)
	{
		//буква 'T'для байта, соответствующего объекта (исполнительная)
		if(tip==1)buf_ko[i17+4]='T';
		else
			//буква 'S' для байта, соответствующего объекта (предварительная)
			if(tip==0)buf_ko[i17+4]='S';
	}
	else buf_ko[i17+4]=124; //для прочих объектов - |
	if(n_bait==12)buf_ko[9]=124;
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//записать контрольную сумму
	pooo_com[n]=biostime(0,0L);//включить таймер на разделку данного элемента
	com_bit[n]=buf_ko[nomer+4]&0xf;
	fr3[n][9]=9;//установить флаг включенного таймера
	flagoutmsg=1;//выставить требование передачи в ТУМС
	return;
}
//----------------------------------------------------------------------
/****************************************************\
* Процедура формирования предварительной команды     *
* для  вспомогательной смены направления             *
* nom - номер объекта управления в markery           *
\****************************************************/
void form_first_kom(int nom)
{

	int n_bait,nomer,podgr,kodd=0,n,i18,j18,SDVIG=0,FN=0,FIN=0;

	nom_func("71");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	//ПОСЫЛАЕМ ПРЕДВАРИТЕЛЬНУЮ КОМАНДУ
	if(zapusk!=3)w(11,999,"");
	buf_ko[2]='E'; //заполнить код группы
	n=markery[nom][6];//получить номер выбранного маркером объекта для базы

	if(fr1[n][13]==1)//если объект из первой стойки
	{
		buf_ko[1]=ZAGO_MPSU(0x61);//заполнить заголовок
		SDVIG=STR1;FN=0;FIN=SIG1;//получить смещение и границу для сигналов
	}
#ifdef KOL_SOO2
	else
	{
		//аналогичные действия для второй стойки
		buf_ko[1]=ZAGO_MPSU(0x62);
		SDVIG=STR2;FN=SIG1;FIN=FN+SIG2;
	}
#endif
	//если предварительная команда
	if(((fr1[n][1]==31)||(fr1[n][1]==131))&&(zapusk==1))n=fr1[n][2];
	nomer=9999;

	for (i18=FN;i18<FIN;i18++)//пройти по подгруппам сигнала
	{
		for(j18=0;j18<=4;j18++)//пройти по объектам подгрупп
		if (spsig[i18][j18]==n)//если найден нужный объект
		{
			podgr=podgruppa[i18+SDVIG-FN];//запомнить подгруппу
			nomer=j18;//запомнить порядковый номер в подгруппе для объекта
			break;
		}
		if(nomer!=9999)break;
	}

	if(nomer==9999)//если объект не найден в сигналах
	{
		buf_ko[2]='Q'; //заполнить код группы доп.объектов
		n=markery[nom][6];//получить номер выбранного маркером объекта для базы
		if(fr1[n][13]==1)//если объект из первой стойки
		{
			buf_ko[1]=ZAGO_MPSU(0x61);//заполнить заголовок
			SDVIG=STR1+SIG1;FN=0;FIN=DOP1;//получить смещение и границу для сигналов
		}
#ifdef KOL_SOO2
		else
		{
			buf_ko[1]=ZAGO_MPSU(0x62);//аналогичные действия для второй стойки
			SDVIG=STR2+SIG2;FN=DOP1;FIN=FN+DOP2;
		}
#endif
		//если предварительная команда
		if(((fr1[n][1]==31)||(fr1[n][1]==131))&&(zapusk==1))n=fr1[n][2];
		nomer=9999;
#if DOP1>0
		for (i18=FN;i18<FIN;i18++)//пройти по подгруппам
		{
			for(j18=0;j18<=4;j18++)//пройти по объектам подгрупп
			if (spdop[i18][j18]==n)//если найден нужный объект
			{
				podgr=podgruppa[i18+SDVIG-FN];//запомнить подгруппу
				nomer=j18;//запомнить порядковый номер в подгруппе для объекта
				break;
			}
			if(nomer!=9999)break;
		}
#endif
		if(nomer==9999) return;//если объект не найден - выйти
	}
	buf_ko[3]=podgr;//для найденного объекта заполнить группу
#ifdef VSP
	for(i18=0;i18<VSP;i18++)
	{
		if(mark[i18][0]==nom)
			if(mark[i18][1]==1) priem_=1;
			else priem_=-1;
	}
	if(i18>VSP) return;
	if(priem_==1) kodd='X';//установить для приема свой код команды
	else
		if(priem_==-1) kodd='T';//для отправления - свой код команды
#endif
	if((fr1[n][1]==30)||(fr1[n][1]==31)||(fr1[n][1]==130)||(fr1[n][1]==131))
	kodd='T';
	for(j18=0;j18<=(n_bait-6);j18++)
	if(j18==nomer) buf_ko[j18+4]=kodd;//для найденного объекта - записать код
	else  buf_ko[j18+4]=124;//для прочих объектов - записать заполнитель
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//дополнить команду контрольной суммой
	pooo_com[n]=biostime(0,0L);//зафиксировать время выдачи команды
	com_bit[n]=kodd&0xf;
	fr3[n][9]=9;//установить признак выдачи предварительной команды
	zapretvybora=1;
	flagoutmsg=1;//выставить флаг требования передачи команды в ТУМС
	vtora=1;//установить признак ожидания исполнительной команды
	return;
}
#ifdef VSP
//------------------------------------------------------------------
/****************************************************\
* Процедура формирования исполнительной команды для  *
* вспомогательной смены направления                  *
* nom - объект управления в markery                  *
\****************************************************/
void form_second_kom(int nom)
{
	int n,nomer,podgr,kodd=0,SDVIG=0,FN=0,FIN=0,i18,j18,n_bait;
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	buf_ko[2]='E'; //заполнить код группы
	n=markery[nom][6]-1;//сместиться на полуобъект
	if(fr1[n][13]==1)//если для первой стойки
	{
		buf_ko[1]=ZAGO_MPSU(0x61);//заполнить заголовок
		SDVIG=STR1;FN=0;FIN=SIG1;//получить необходимый сдвиг и границу
	}
#ifdef KOL_SOO2
	else// если вторая стойка, то для нее - то же самое
	{
		buf_ko[1]=ZAGO_MPSU(0x62);
		SDVIG=STR2;FN=SIG1;FIN=FN+SIG2;
	}
#endif
	nomer=9999;
	for (i18=FN;i18<FIN;i18++)//пройти по подгруппам
	for(j18=0;j18<=4;j18++)//пройти по объектам в подгруппе
	if (spsig[i18][j18]==n)//если найден объект
	{
		podgr=podgruppa[i18+SDVIG-FN];//запомнить подгруппу
		nomer=j18;//запомнить порядковый номер объекта
		break;
	}
	if(nomer==9999)
	{
		buf_ko[2]='Q'; //заполнить код группы
		n=markery[nom][6]-1;//получить номер выбранного маркером объекта для базы
		if(fr1[n][13]==1)//если объект из первой стойки
		{
			buf_ko[1]=ZAGO_MPSU(0x61);//заполнить заголовок
			SDVIG=STR1+SIG1;FN=0;FIN=DOP1;//получить смещение и границу для сигналов
		}
#ifdef KOL_SOO2
		else
		{
			buf_ko[1]=ZAGO_MPSU(0x62);//аналогичные действия для второй стойки
			SDVIG=STR2+SIG2;FN=DOP1;FIN=FN+DOP2;
    }
#endif
    //если предварительная команда

    if((fr1[n][1]==31)&&(zapusk==1))
			n=fr1[n][2]; //взять предварительный объект
    nomer=9999;
    for (i18=FN;i18<FIN;i18++)//пройти по подгруппам
    {
      for(j18=0;j18<=4;j18++)//пройти по объектам подгрупп
      if (spdop[i18][j18]==n)//если найден нужный объект
      {
        podgr=podgruppa[i18+SDVIG-FN];//запомнить подгруппу
        nomer=j18;//запомнить порядковый номер в подгруппе для объекта
        break;
      }
      if(nomer!=9999)break;
    }
    if(nomer==9999) return;//если объект не найден - выйти
  }
  buf_ko[3]=podgr;//заполнить подгруппу

  for(i18=0;i18<VSP;i18++) //пройти по всем кнопкам вспомогательной смены
  	if(mark[i18][0]==nom)break; //если найдена нажатая кнопка
  if(i18>VSP) return; //если кнопка не найдена - выход

	if(mark[i18][1]==1) priem_=1; //если она для приема
  else priem_=-1;     //если она для отправления

	if(priem_==1) kodd='X';//заполнить код для приема
	else
		if(priem_==-1) kodd='T';//или заполнить код для отправления
		else return;

	for(j18=0;j18<=(n_bait-6);j18++)
	if(j18==nomer) buf_ko[j18+4]=kodd;//для найденного объекта записать команду
	else  buf_ko[j18+4]=124;  //для остальных объектов - заполнитель
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//добавить контрольную сумму
	pooo_com[n]=biostime(0,0L);//зафиксировать время
	com_bit[n]=kodd&0xf;
	fr3[n][9]=9;//установить признак выдачи команды
	flagoutmsg=1;//установить флаг-требование на передачу в ТУМС
	zapretvybora=0;
	priem_=0;//сбросить признак вспомогательного приема/отправления
	return;
}
#endif
//---------------------------------------------------------------
/***********************************************\
* Процедура формирования команды для включения  *
* выдержки времени на выполнение искусственной  *
* разделки. nom - номер об.управления в markery *
\***********************************************/
void gri_razd(int nom)
{
	int nomer,podgr,n,i17,j17,SDVIG=0,FN=0,FIN=0,n_bait;

	nom_func("100");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	buf_ko[2]='F';//заполнить группу
	n=markery[nom][6];//получить объект
	if(fr1[n][13]==1)//для стойки №1 получить заголовок, сдвиг и границы
	{
		buf_ko[1]=ZAGO_MPSU(0x61);
    SDVIG=STR1+SIG1+DOP1;FN=0;FIN=UCH1;
  }
#ifdef KOL_SOO2
  else //то же самое для второй стойки
  {
    buf_ko[1]=ZAGO_MPSU(0x62);
    SDVIG=STR2+SIG2+DOP2;FN=UCH1;FIN=FN+UCH2;
  }
#endif
  nomer=9999;
  for (i17=FN;i17<FIN;i17++)//пройти по подгруппам
  for(j17=0;j17<=4;j17++)//пройти по объектам подгруппы
  if (spspu[i17][j17]==n)//если объект найден
  {
    podgr=podgruppa[i17+SDVIG-FN];//зафиксировать подгруппу
    nomer=j17;//зафиксировать номер в подгруппе
    break;
  }
  if(nomer==9999) return;//если объект не найден - выход
  buf_ko[3]=podgr;//заполнить подгруппу
  for (i17=0;i17<=4;i17++)
	if (i17==nomer) buf_ko[i17+4]='L'; //для выбранного объекта записать команду
	else buf_ko[i17+4]=124; //для прочих объектов записать заполнитель
	if(n_bait==12)buf_ko[9]=124;
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//дополнить команду контрольной суммой
	pooo_com[n]=biostime(0,0L);//зафиксировать время
	com_bit[n]=buf_ko[nomer+4]&0xf;
	fr3[n][9]=9;//запомнить факт выдачи команды
	flagoutmsg=1;//установить запрос на передачу в ТУМС
	return;
}
//------------------------------------------------------------------
/********************************************************\
* Процедура иницализации команды открытия-закрытия       *
* переезда для случая автоматической выдачи двух команд  *
* подряд: предварительной и исполнительной без участия   *
* оператора для исполнительной команды.                  *
* nom - номер в базе для объекта предварительной команды.*
* Объект исполнительной команды в базе должени идти сразу*
* за объектом предварительной команды.                   *
\********************************************************/
void k_per(int nom)
{
	nom_func("113");
	if (klaval==13)
	{ //ПОСЫЛАЕМ КОМАНДУ НА ОТКРЫТИЕ ПЕРЕЕЗДА
    if((fr3[nom][0]==1)&&(fr3[nom+1][1]==1))w(24,999,"");
    else  w(25,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ЗАКРЫТИЕ ПЕРЕЕЗДА
    oppp=0;puti=0;
    zp_zp(nom);
    return;
	}
  return;
}
//-------------------------------------------------------------------
/*********************************************************\
* Процедура подготовки команды для объектов, управляемых  *
* некоторыми кнопками.                                    *
\*********************************************************/
void knopa(int nom)
{
	int	bit,     //бит воздействия на включение
	ob_vkl,  //объект включения
	ob_otkl, //объект отключения
	so1,     //сообщение на включение
	so2,     //сообщение на отключение
	inv;     //признак необходимости инвертировать команду
	nom_func("121");
	if(klaval==13)
	{
		if((fr1[nom][1]==163)||
		((fr1[nom][1]>=155)&&(fr1[nom][1]<=158)))
		{
			for(bit=0;bit<5;bit++)
			if((fr1[nom][bit+3]&0xff00)!=0)break;
			so1=fr1[nom][10];
			so2=fr1[nom][11];
		}
		else
		if(fr1[nom][1]!=166) //если кнопка не имеет двух объектов воздействия
		{ bit=fr1[nom][11]&0xf;
			ob_vkl=nom;ob_otkl=nom;
			so1=fr1[nom][3];so2=fr1[nom][4];
			inv=0;
		}
		else //если в кнопке два объекта воздействия (включения и отключения)
		{ bit=(fr1[nom][3]&0xf000)>>12;     //номер бита воздействия для включения
			ob_vkl=fr1[nom][3]&0xfff;         //объект включения
			ob_otkl=fr1[nom][4]&0xfff;        //объект отключения
			so1=fr1[nom][8];                  //номер сообщения для включения
			so2=fr1[nom][9];                  //номер сообщения для отключения
		}
		if(fr1[nom][1]==53)  //если объект кнопки имеет лампочку
		{
			bit=fr1[nom][12]&0xf; //бит для кнопки
			ob_vkl=nom;
			ob_otkl=nom;
			so1=fr1[nom][2]; //сообщение на включение
			so2=fr1[nom][3]; //сообщение на отключение
			inv=fr1[nom][11];//признак инверсии
		}
		else
		//если у кнопки есть объект влияния и это не ДСО для СН
		// и не кнопка с двумя объектами воздействия
		if(fr3[nom][bit]==1)w(so2,999,""); //если включено - спросить об откл.
		else
			if(fr3[nom][bit]==0) w(so1,999,""); //если отключено - спросить о вкл.

		if(fr1[nom][2]!=0)//если это не кнопка нормализации
		{
			if(fr1[nom][1]==166) //если кнопка на два объекта
			{
				if(fr3[ob_vkl][bit]==0){nom=ob_vkl;inv=0;}
				if(fr3[ob_vkl][bit]==1){nom=ob_otkl;inv=1;}
			}
			if(fr1[nom][2]==3)inv=1;
			else inv=0;

			form_kom_knopka(nom,inv);//сформировать комануы для индивидуальной выдачи
			if(fr1[nom][2]==33)pooo_com[nom]=0;

		}
out:
		oppp=0;puti=0;
		home(modi);   //вернуть курсор на место с точки modi
		return;
	}
	else return;
}
//------------------------------------------------------------------
/*********************************\
* Процедура изображения на экране *
* кнопки ответственных  команд    *
\*********************************/
void knopka_OK()
{
	int cv_kn;
	nom_func("123");
	if(pusto==0)
	{
		if(otv_kom==0)cv_kn=8;// если нет нажатия кнопки ОК - цвет серый
		else cv_kn=12;  // иначе(когда нажатие) - цвет красный
	}
	if(otv_kom_oper==2)cv_kn=11;
	else
	{
		if(otv_kom==1)//если есть нажатие ОК
		{
			sbros_all();//сброс всех предварительно набранных команд и переменных
			klaval=0;
			if((nikuda==0)&&(klo==0)&&(help==0)) //если основное окно программы
			{
				t(0); //гашение окна сообщений
				if(perekluch==0) // если не выполняется переключение экранов
				if(vspz==1) home(pat); //удаление курсора с позиции
				else
				if(modi!=-1) home(modi);
				else  home(i3);
			}
			po_ma=0;vspz=0;zzzzx=0;u=0;modi=-1;
			add(0,0,'Y'); //нажатие кнопки ОК
			//сброс всех кнопок и переменных,связанных со стрелками
			finish_strelok();
			povt1=povt2=poka=vozvr=0;
			flagoutmsg=0;
			cv_kn=12;
		}
		else // если нет нажатия кнопки ОК
		{
			if((nikuda==0)&&(klo==0)&&(help==0))
			{
				if(perekluch==0)
				if(modi!=-1)
				{ //если был выбран объект ГРИ
					if(gri==1){gri=0;fr3[markery[modi][6]][9]=0;}
					if(zalet!=0)
					{ was[(markery[modi][7]&0xfff)-10]=0;
						fr3[markery[modi][6]][9]=0;
						vtora=0;
						flagoutmsg=0;
					}
					if(zabeg!=0){fr3[markery[modi][6]][9]=0;vtora=0;flagoutmsg=0;}
					home(modi);
				}
				else home(pat);
				cv_kn=8;
			}
			add(0,0,'y'); //отпускание кнопки
			u=0;
			modi=-1;
			zzzzx=0;
			if(perekluch==0)
			{ if(zalet!=0) zalet=0;
				if(zabeg!=0) zabeg=0;
				t_otpusk=biostime(0,0L);//зафиксировать время отпускания кнопки
			}
		}
	}
	if((klo==1)||(help!=0)||(nikuda==1))return;
	if((pusto&3)!=0)cv_kn=11;
	setlinestyle(0,0,0);
	if(cv_kn==8)cv_kn=7;
	setfillstyle(1,cv_kn);
	fillellipse(XOK,YOK,8,8);
	setcolor(8);
	circle(XOK,YOK,8);
	outtextxy(XOK-7,YOK-3,"OK");
	return;
}
//------------------------------------------------------------------------
/********************************************************\
* Процедура выдачи текстовых вопросов при первоначальной *
* инициализации кнопок управления переездами.            *
\********************************************************/
void kom_per(int pop)
{
	nom_func("126");
	if((nikuda==1)||(klo==1)||(help!=0)) return;
	if((fr3[pop][0]==1)&&(fr3[pop+1][1]==1))w(23,999,"");//ОТКРЫТЬ ПЕРЕЕЗД?
	else w(22,999,"");//ЗАКРЫТЬ ПЕРЕЕЗД?
	oppp=1;
	puti=1;
	return;
}
//-------------------------------------------------------------------------
/*******************************************************\
* Процедура трассировки маршрута по топологии           *
* станции и проверка достижимости конца маршрута.       *
* При предварительном задании трассы оператором(tipus=0)*
* или из таблицы маршрутов (tipus=1)                    *
\*******************************************************/
void marsh1(int tipus)
{
	int jj,i,otvet;
//nop,strlk,kol_str[skoko_stoek];
	unsigned int element,napravlenie,kategoria,maska;
	int ioy;
//,stk,n_strel,stoyka,obect,ind_sig[skoko_stoek],ind_str[skoko_stoek];
//	unsigned long pol_strel;
	nom_func("164");
	// если нажимается первая точка, запоминаем начало маршрута
 /*	if(povt1==0)
	{ //фиксируем направление сигнала
		if(tipus!=0)nop=fr1[nach_marsh][1];
		else nop=fr1[nach_marsh_oper][1];
	}
	*/
	if((poka==0)&&(tipus==0))
	{ for (i=0;i<kol_VO;i++)
		if(((fr1[i][1]<2)||(fr1[i][0]==7))&&(fr1[i][0]!=5))fr3[i][6]=0;//для основных объектов - сброс вспомогательного слова
		if(mane==0)
		{
			if((fr1[markery[modi][6]][2]==0)&&//если сигнал маневровый и
			(fr1[markery[modi][6]][6]==0))    // и светофор маневровый
			mane=1;// установить признак маневрового маршрута
			else
				if(manevro==1) mane=1;//если маршрут маневровый  установить признак
				else mane=0;//для поездного сбросить признак
		}
		if(fr1[markery[modi][6]][0]!=2) return;//если выбран не сигнал - выход
		else //если выбран сигнал
		{ povt1=1;//зафиксировать факт нажатия
			ko=markery[modi][6];//запомнить объект управления
			if(fr1[markery[modi][6]][6]==3&&mane==0)ko++; //если 2 белых - сместиться
		}
		if(povt2==0)//если второй сигнал не выбран
		{ //присвоить началу маршрута значение выбранного сигнала
			if(tipus!=0)nach_marsh=ko;
			else nach_marsh_oper=ko;
			n_m=modi;//запомнить объект управления
			povt2=1;//зафиксировать
			poka=1;
			koni();
			return;
		}
		else  poka=0;
	}
	 //взять строку для начала маршрута
	if(tipus!=0)
	{
		NM=fra[nach_marsh][0];
		END=find_end(tipus);//найти конец маршрута
	}
	else
	{
		NM=fra[nach_marsh_oper][0];
		END_OPER=find_end(tipus);//найти конец маршрута
	}

	if(((tipus!=0)&&(END==9999))||//если конец не найден
	((tipus==0)&&(END_OPER==9999)))
	{ioy=1;goto nn;}
	if(tipus!=0)NE=fra[END][0];//взять строку для конца маршрута
	else NE=fra[END_OPER][0];

	if(tipus!=0) //если идет установка маршрута из таблицы
	{
		if((fr1[nach_marsh][3]==7)&&(fr1[end_marsh][3]==7))//если сквозной
		if(fra[nach_marsh][0]!=fra[end_marsh][0]){ioy=1;goto not;}

		if(nach_marsh==end_marsh)//если конец и начало на одном сигнале
		{ ioy=1;goto not;}

		//если начало-конец рядом
		if((nach_marsh-end_marsh==1)||(nach_marsh-end_marsh==-1))
		{ ioy=1; goto not;}
	}
	else //если идет установка маршрута оператором
	{
		//если сквозной
		if((fr1[nach_marsh_oper][3]==7)&&(fr1[end_marsh_oper][3]==7))
		if(fra[nach_marsh_oper][0]!=fra[end_marsh_oper][0]){ioy=1;goto not;}
		//если конец и начало на одном сигнале
		if(nach_marsh_oper==end_marsh_oper)
		{ ioy=1;goto not;}
		//если начало-конец рядом
		if((nach_marsh_oper-end_marsh_oper==1)||
		(nach_marsh_oper-end_marsh_oper==-1))
		{ ioy=1; goto not;}
	}

	ZAHOD=0;//установить признак поиска задаваемого маршрута
	ZERO_TRASSA(tipus);
	if(prov_avail(tipus)==1) //если конец не соответствует
	{
		w(134,999,"");//ТОЧКА КОНЦА МАРШРУТА УКАЗАНА НЕВЕРНО
		home(modi);
		soob_for_oper=1;
		sbros=1;
		whole_finish(tipus);
		if(tipus!=0)
		{
			TRASSA[ukaz_trass--].object=0;
			while(ukaz_trass>=0)
			{
				fr3[TRASSA[ukaz_trass].object&0xFFF][10]=
				fr3[TRASSA[ukaz_trass].object&0xFFF][10]&0xfffe;
				TRASSA[ukaz_trass--].object=0;
			}
		}
		else
		{
			TRASSA0[ukaz_trass--].object=0;
			while(ukaz_trass>=0)
			{
				fr3[TRASSA0[ukaz_trass].object&0xFFF][10]=
				fr3[TRASSA0[ukaz_trass].object&0xFFF][10]&0xfffe;
				TRASSA0[ukaz_trass--].object=0;
			}
		}
		return;
	}
	if(half_marsh1!=0)
	{
		if(tipus!=0)nach0_marsh=half_marsh1;
		else nach0_marsh_oper=half_marsh1;
	}
	//проверить достижимость по топологии
	if(tipus!=0)
		ioy=may_be(nach_marsh,fr1[nach_marsh][1],tipus);
	else
		ioy=may_be(nach_marsh_oper,fr1[nach_marsh_oper][1],tipus);

	if(ioy==0)//если трасса найдена
	{
//		add(0,0,'0'); $$
		ukaz_trass=0;//встать в начало трассы
		while(1)//пройти по всем элементам найденного маршрута
		{
			if((tipus!=0)&&(TRASSA[ukaz_trass].object==0xFFFF))break;
			if((tipus==0)&&(TRASSA0[ukaz_trass].object==0xFFFF))break;

			if(tipus!=0)element=TRASSA[ukaz_trass].object&0xfff;
			else element=TRASSA0[ukaz_trass].object&0xfff;
			if(element>=kol_VO)break;
			if(fr3[element][8]!=0){ukaz_trass--;	break;}

			if(fr1[element][0]==1)
			plus_minus(tipus);//для стрелок установить признаки желаемого положен
			otvet=test_elem(element,3,tipus,ukaz_trass);
			if(otvet==1)
			{
				for(i=0;i<=100;i++)
				{
					if(tipus==1)
					{

						fr3[TRASSA[i].object&0xfff][10]=0;
						fr3[TRASSA[i].object&0xfff][8]=0;
						TRASSA_RIS[i].object=TRASSA[i].object&0xfff;
					}
					if(tipus==0)
					{
						fr3[TRASSA0[i].object&0xfff][10]=0;
						fr3[TRASSA0[i].object&0xfff][8]=0;
						TRASSA_RIS[i].object=TRASSA0[i].object&0xfff;
					}
				}
				ris_trassa(2);
				ZERO_TRASSA(2);
				goto not;//если есть причины отказ от маршрута
			}
			fr3[element][10]=fr3[element][10]|2;
			if(fr1[element][0]==5) //если это путь
			{
				if(tipus!=0)napravlenie=fr1[nach_marsh][1];
				else napravlenie=fr1[nach_marsh_oper][1];

				if(((tipus==1)&&(mane1==0))||((tipus==0)&&(mane==0)))kategoria=1; //поездной
				else kategoria=0; //маневровый

				maska=0;

				switch(napravlenie)//переключатель по направлению
				{
					case 0: //четное
									switch(kategoria)
									{
										case 0: maska=maska|0x40;break; //четный маневровый
										case 1: maska=maska|0x10;break; //четный поездной
										default: break;
									}
									break;

					case 1://нечетное
									switch(kategoria)
									{
										case 0: maska=maska|0x80;break; //нечетный маневровый
										case 1: maska=maska|0x20;break; //нечетный поездной
										default: break;
									}
									break;

					default: break;
				}
				fr3[element][10]=fr3[element][10]|maska;
			}

			ukaz_trass++;//перейти на следующий объект
			if(ukaz_trass>=100)return;
		}
		ukaz_trass++;


		for(i=0;i<skoko_stoek*3;i++)
		{
			if(tipus!=0)
			{
				if(MARSHRUT_ALL[i].NACH==nach_marsh)
				{
					ZERO_TRASSA(1);
					return;
				}
			}
			else
			{
				if(MARSHRUT_ALL[i].NACH==nach_marsh_oper)
				{
					ZERO_TRASSA(0);
					return;
				}
			}
		}
		for(i=0;i<skoko_stoek*3;i++)
		{
			if(MARSHRUT_ALL[i].NACH==0)
			{
//				add(0,0,'1'); $$
				if(tipus!=0)
				{
					MARSHRUT_ALL[i].NACH=nach_marsh;
					MARSHRUT_ALL[i].NACH0=half_marsh1;
					MARSHRUT_ALL[i].END=end_marsh;
				}
				else
				{
					MARSHRUT_ALL[i].NACH=nach_marsh_oper;
					MARSHRUT_ALL[i].NACH0=half_marsh1;
					MARSHRUT_ALL[i].END=end_marsh_oper;
				}
				break;
			}
		}
		if(i<(skoko_stoek*3))
		ZAPOLNI_MARSH(tipus,i);
		else
		{
//			add(0,0,'6'); $$
			if(tipus!=0){nach_marsh=0;end_marsh=0;}
			else {nach_marsh_oper=0;end_marsh_oper=0;}
			if(tipus==0)vozvr=0;
			w(273,999,"");
			if(tipus!=0)
			{
				for(jj=0;jj<100;jj++)
				{
					if(TRASSA[jj].object==0)break;
					if(TRASSA[jj].object==0xffff)break;
					fr3[TRASSA[jj].object&0xfff][10]=0;
				}
			}
			else
			{
				for(jj=0;jj<100;jj++)
				{
					if(TRASSA0[jj].object==0)break;
					if(TRASSA0[jj].object==0xffff)break;
					fr3[TRASSA0[jj].object&0xfff][10]=0;
				}
			}
			ZERO_TRASSA(tipus);
			return;
		}

	}
	else //если трасса не найдена
	{
not:
//		add(0,0,'2');$$
		ZERO_TRASSA(tipus);
		if(tipus==0)
		{
			nach_marsh_oper=0;
			nach0_marsh_oper=0;
			end_marsh_oper=0;
			END_OPER=0;
		}
		if(tipus==1)
		{
			nach_marsh=0;
			nach0_marsh=0;
			end_marsh=0;
			END=0;
		}
		//finish_startMM(tipus); //закончить работу с трассой
nn: if(ioy==1)//если "плохой" конец
		w(134,999,""); //ТОЧКА КОНЦА МАРШРУТА УКАЗАНА НЕВЕРНО
		home(modi);
		soob_for_oper=1;
		sbros=1;
		whole_finish(tipus);
	}
	return;
}
//------------------------------------------------------------------------
/*******************************************************************\
* Процедура проверки оттрассированного маршрута в маршрутном режиме *
* и проверка возможности установки стрелок в требуемое состояние,   *
* после подтверждения маршрута оператором                           *
\*******************************************************************/
void marsh2(int tipus)
{
	int otv,obekt2,obkt0,i,j,n_m,ukaz_trass0,n_mane;
	nom_func("165");
	if(tipus==0)
	{
		vozvr=0;//сбросить флаг нажатий точек задания маршрута
		n_mane=mane;
	}
	if(tipus==1)n_mane=mane1;
	End_vara=0;//сбросить конец вариантного маршрута
	ukaz_trass0=0;//указатель трассы поставить в начал

	while(1)//проход по всем элементам маршрута
	{
		if(tipus!=0)
		{
			if(TRASSA[ukaz_trass0].object==0xFFFF)
			{
//				add(0,0,'8'); $$
				break;
			}
			obkt0=TRASSA[ukaz_trass0].object&0xfff;
			n_m=nach_marsh;
		}
		else
		{
			if(TRASSA0[ukaz_trass0].object==0xFFFF)
			{
//				add(0,0,'8');$$
				break;
			}
			obkt0=TRASSA0[ukaz_trass0].object&0xfff;
			n_m=nach_marsh_oper;
		}
		if((obkt0>=kol_VO)||(obkt0<0))break;
		otv=test_elem(obkt0,3,tipus,ukaz_trass0);//провести тестирование для элемента

		if(fr1[obkt0][0]==3)//если СП
		{
			if(fr1[n_m][1]==0)//четный маршрут
			{ //если маневровый четный
				if(n_mane==1)fr3[obkt0][12]=fr3[obkt0][12]|1;
				//четный поездной
				else fr3[obkt0][12]=fr3[obkt0][12]|2;
			}
			else//нечетный маршрут
			{
				if(n_mane==1)fr3[obkt0][12]=fr3[obkt0][12]|4;
				//четный поездной
				else
					fr3[obkt0][12]=fr3[obkt0][12]|8;

			}
		}

		if(otv==1)goto final;//если есть причины отказ от маршрута

		if(otv==22)//если нужно перевести стрелку в другой стойке
		{
			obekt2=fr1[obkt0][3];//определить объект для другой стойки
			if(fr1[obekt2][12]!=0)
			{
				fr3[obekt2][7]=oper;//запомнить необходимый код команды
				for(i=0;i<5;i++)
				{ if(strelki[i][0]==0)
					{ strelki[i][0]=obekt2;
						if(oper=='I'){strelki[i][1]=1;strelki[i][2]=0;}
						if(oper=='J'){strelki[i][1]=0;strelki[i][2]=1;}
						break;
					}
				}
				TIMER_SOB(obekt2,22,obekt2);//зарядить таймер на ожидание выполнения
			}
		}
		ukaz_trass0++;//перейти к следующему элементу маршрута по трассе
		if(ukaz_trass0>=100)break;
	}
	for(i=0;i<(skoko_stoek*3);i++)if(MARSHRUT_ALL[i].NACH==n_m)break;
	if(i>=(skoko_stoek*3))
	{
		ZERO_TRASSA(tipus);
		return;
	}
	w(274,n_m,"");//ПРИНЯТ НА ИСПОЛНЕНИЕ МАРШРУТ ОТ
	//	add(0,0,'7');//$$
	zad_marsh(tipus);
final:
	return;
}
//--------------------------------------------------------------------------
/*********************************************************\
* Процедура проверки наличия активных объектов разделки и *
* проверки наличия объекта nom в числе активных           *
\*********************************************************/
int nashel(int nom)
{
  int iqc,kil=0;
	nom_func("180");
	//Пройти по всем объектам разделки
  for(iqc=0;iqc<WAZO;iqc++)
  { //если объект числится в активных
		if(nom==was[iqc])return(1);
    //если очередной объект активности свободен
		else if(was[iqc]==0) kil++;
  }
  //если все объекты разделки свободны
  if(kil==WAZO) return(0);
  //если есть активные объекты разделки
  else return(2);
}
//-------------------------------------------------------------------------
/*******************************************************\
* Процедура формирования и вызова выдачи команды        *
* включения/отключения немаршрутизированных перемещений *
\*******************************************************/
void nemarsh(int nom)
{
	nom_func("183");
	if(klaval==13)
	{if(fr3[nom][fr1[nom][11]]==1)w(fr1[nom][4],999,"");
    else
      if(fr3[nom][fr1[nom][11]]==0) w(fr1[nom][3],999,"");
    oppp=0;puti=0;
    form_kom_nemarsh(nom);
    home(modi);
    return;
  }
  else return;
}
//-------------------------------------------------------------------------
/*******************************************\
* Процедура подготовки к выдаче команды для *
* включения и выключения УКСПС              *
\*******************************************/
void nok(int nom)
{
	int bit,ob;

	nom_func("185");
//#ifdef NEW_KNOPKA
	if(klaval==13)
	{
		for(bit=0;bit<5;bit++)
		{
			if(fr1[nom][bit+3]==9999)continue;
			ob=(fr1[nom][bit+3]&0xff00)>>8;
			if(ob==0xff)continue;
			else break;
		}
		if(fr1[nom][0]==3)
		{
			if((fr3[nom][0]==1)&&(fr3[nom][bit]==0))
			{
				w(51,999,"");return;
			}
		}
		if(fr3[nom][bit]==0)vopros(1,9999,215,"?");//ОТКЛЮЧИТЬ УКСПС
		else
			if(fr1[nom][0]==2)
			{
				if(fr3[nom][bit]==1)vopros(1,9999,214,"?");//ВКЛЮЧИТЬ УКСПС
				else {home(modi);oppp=0;puti=0;return;}
			}
			else
			{
				w(229,999,""); //"Объект управления не активизирован"
				home(modi);
				oppp=0;
				puti=0;
				return;
			}
		zapretvybora=1;oppp=1;puti=1;
	}
	return;
//#else
/*
	if(klaval==13)
	{ if(fr3[nom][(fr1[nom][3]&0xf000)>>12]==0)
			w(109,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ОТКЛЮЧЕНИЕ УКСПС
		else
			if(fr3[nom][(fr1[nom][3]&0xf000)>>12]==1)
				w(108,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ВКЛЮЧЕНИЕ УКСПС
			else {home(modi);oppp=0;puti=0;return;}
		home(modi);
		oppp=0;
		puti=0;
		form_kom_nk_chk(nom);
	}
	return;
#endif
*/
}
//--------------------------------------------------------------------------
/******************************************************\
* Процедура выдачи исполнительной команды на включение *
* -отключение УКСПС не в группе разделки.              *
\******************************************************/
void noka(nom)
{ int ij,ob;

	nom_func("186");
#ifdef NEW_KNOPKA
	if(klaval==13)
	{ for(ij=0;ij<5;ij++)
		{
			if(fr1[nom][ij+3]==9999)continue;
			ob=(fr1[nom][ij+3]&0xff00)>>8;
			if(ob==0xff)continue;
			else break;
		}
		if(fr3[nom][ij]==0)
			w(109,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ОТКЛЮЧЕНИЕ УКСПС
		else
			if(fr3[nom][ij]==1)
				w(108,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ВКЛЮЧЕНИЕ УКСПС
			else {home(modi);oppp=0;puti=0;return;}
		home(modi);
		oppp=0;
		puti=0;
		form_kom_nk_chk(nom);
		return;
	}
	else return;

#else
	if(klaval==13)
	{ if(fr3[nom][(fr1[nom][3]&0xf000)>>12]==0)
			w(109,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ОТКЛЮЧЕНИЕ УКСПС
		else
			if(fr3[nom][(fr1[nom][3]&0xf000)>>12]==1)
				w(108,999,"");//ПОСЫЛАЕМ КОМАНДУ НА ВКЛЮЧЕНИЕ УКСПС
			else {home(modi);oppp=0;puti=0;return;}
		home(modi);
		oppp=0;
		puti=0;
		form_kom_nk_chk(nom);
		return;
	}
	else return;
#endif
}
//--------------------------------------------------------------------------
/********************************************\
* Процедура выдачи предварительной и исполни-*
* тельной команды в группе разделки          *
\********************************************/
void nokb(nom)
{ 
	nom_func("187");
	if(klaval==13)
  { if((fr3[nom][0]==0)&&(oppp==1))
    {
      w(11,999,"");//ВЫДАНА ПРЕДВАРИТЕЛЬНАЯ КОМАНДА
      TIMER_SOB(nom,30,nom);
      oppp=2;
    }
    else
			if((fr3[nom][0]==1)&&oppp==2)
      {
        w(109,999,"");//ВЫДАНА КОМАНДУ НА ВКЛЮЧЕНИЕ УКСПС
        oppp=0;
        home(modi);
      }
      else {home(modi);oppp=0;puti=0;return;}
    puti=1;
		if(oppp==2)first_razd((fr1[nom][3]&0xfff),0);
    else
			if(oppp==0)first_razd((fr1[nom][3]&0xfff),1);
    return;
  }
  else return;

}
//-------------------------------------------------------------------------
/**************************************************************\
* Процедура организации выдачи команд по вкл./откл. оповещения *
* и выдачи запретов/разрешений на работу монтерам.             *
\**************************************************************/
void opoveshe(int nom,int n)
{ int zm,b_zm,rm,b_rm;

	nom_func("201");
	rm=fr1[nom][2]&0xfff;              // rm - разрешение монтерам
	b_rm=(fr1[nom][2]&0xf000)>>12;

	zm=fr1[nom][7]&0xfff;              // zm - запрет монтерам
	b_zm=(fr1[nom][7]&0xf000)>>12;

	if (klaval==13)
	{
		oppp=0;puti=0;
		if(n==0)
		{
			//	ВЫДАНА КОМАНДА ВКЛЮЧИТЬ ОПОВЕЩЕНИЕ
			if((fr3[rm][b_rm]==0)&&(fr3[zm][b_zm]==0))
			{
				w(36,999,"");
				form_kom_knopka(rm,0);   //активизировать разрешение монтерам
			}
			else
				//ВЫДАНА КОМАНДА ОТКЛЮЧИТЬ ЗАПРЕТ МОНТЕРАМ
				if((fr3[rm][b_rm]==1)&&(fr3[zm][b_zm]==1))
				{
					w(30,999,"");
					form_kom_knopka(zm,0); //активизировать запрет монтерам
				}
				else {home(modi);return;}
		}
		if(n==1)
		{
			w(31,999,""); // ВЫДАНА КОМАНДА ВКЛЮЧИТЬ ЗАПРЕТ МОНТЕРАМ
			form_kom_knopka(zm,0);//если включается запрет монтерам
		}
		if(n==2)
		{
			w(162,999,""); //ВЫДАНА КОМАНДА ОТКЛЮЧИТЬ ОПОВЕЩЕНИЕ
			form_kom_knopka(rm,0);//если отключается оповещение
		}
		home(modi);
		return;
	}
	else return;
}
//------------------------------------------------------------------
//процедура организации диалога при открытии сигнала в раздельном режиме
//po_ma - признак типа светофора
void otkrs()
{
	int Rez,j1,ii;

	nom_func("208");
	if((fr1[point][0]!=2)||(fr1[point][2]>1)||(fr1[point][1]>1))
  {
    w(228,999,"!");
#ifdef MOUSE
		if(MOUSE_ON!=0)
    {
      clear_mouse();
      for(j1=0;j1<12;j1++)line_old_gor_kurs[j1]=0;
			for(j1=0;j1<40;j1++)line_old_vert_kurs[j1]=0;
    }
#endif
    soob_for_oper=1;
    zapretvybora=1;
    return;
  }
  if(vibeg==2)goto step2;
	if(vih_vopr==2)goto step2;//если выдано предупреждение, и запрошено подтверждение
	if((uprav==1)&&(i3>0))point=markery[i3][6];//запомнить выбранный объект
	if(punkt_N>0)danet=1;//goto step2;}
	else danet=0;//сбросить флаг ожидаемых ответов
 // если имеется признак отмены или открыт маневровый или поездной
	//сигнал закрывается
	if((ot==1)||(fr3[point][0]==1)||(fr3[point][1]==1)||(fr3[point][2]==1)||(fr3[point][3]==1))
	{ vopros(0,point,136,"");// ЗАКРЫТЬ СИГНАЛ?
		danet=2;//установить флаг ожидания ответа для закрытия сигнала
		vibeg=2;//установить признак задания вопроса для сигнала
		ot=1;
		zapretvybora=1;
		// если имеется признак отмены и закрыты поездной и маневровый
		if((ot==1)&&(fr3[point][0]==0)&&(fr3[point][1]==0))
		{ if(fr1[point][6]==2)
      {
        po_ma=1;// если совмещенный сигнал,то установить признак
      }
      if(fr1[point][6]==3)//если у сигнала есть 2 белых
      { point++;//идем вперед на специальный сигнал - для 2-го белого огня
        po_ma=5;//устанавливаем признак наличия 2-х белых
      }
    }
		return;
  }
	if((fr3[point][0]==0)&&(fr3[point][1]==0)) //если закрыты оба сигнала
	{ if(mane!=0)vopros(0,point,135," маневровый");// ОТКРЫТЬ СИГНАЛ?
		else vopros(0,point,135," поездной");// ОТКРЫТЬ СИГНАЛ?
		if(fr1[point][6]==2)po_ma=1; //если сигнал совмещенный
    if(fr1[point][6]==3)// если есть два белых
    { point++;//идем вперед на специальный сигнал - для 2-го белого огня
      po_ma=5;//устанавливаем признак наличия 2-х белых
    }
    danet=1;//устанавливаем флаг ожидания ответа для открытия сигнала
    vibeg=2;//установить признак задания вопроса для сигнала
		return;
  }
step2:
	if(klaval==13)//если нажата Enter
	{ if(vih_vopr==2)//если ранее выдавалось предупреждение для подтверждения
		{ t(0);        //убрать с экрана вопрос
      vih_vopr=0; //обнулить признак ожидания подтверждения при запретах
      goto step3; //перейти к действиям
    }
    if(vih_vopr==3)//если было предложено подтвердить
    { t(0);//очистить зону текстов
      vih_vopr=0;//сброс признака ожидания подтверждения при запретах
      goto step4;
    }
		if(((danet==3)||(danet==2))&&(ot==1))//если спрашивали о закрытии
		{ vibeg=0;//сброс флага ожидания подтверждения
      danet=0;
			zakrsi();//закрытие сигнала
			puti=0;
			zapretvybora=0;
		}
		else   // если спрашивали об открытии, то проверяем можно ли открывать
		{
			if((fr1[markery[modi][6]][2]==0)&&  //если маневровый сигнал и
			(fr1[markery[modi][6]][6]==0))// не совмещенный
			manevro=1;                    // то установить признак маневрового
			if(mane==1)manevro=1;
			if(manevro==0) oper='B';// установить команду по открываемому сигналу
			else oper='A';
			for(ukaz_trass=0;ukaz_trass<100;ukaz_trass++)
			TRASSA0[ukaz_trass].object=0xFFFF;
			ukaz_trass=0;//освободить trass[]
			point=markery[i3][6];
			otkrsi();//набрать структуру КК вдоль существующего маршрута
			if(tst_nal_sig()==1)//проверить наличие сигнала конца, и если его нет
			{
				if(PROV_SCB==1)
				{
					podtver();//запросить подтверждение выдачи команды на открытие в "никуда"
					vih_vopr=3;//установить флаг ожидания подтверждения для открытия сигнала
				}
				klaval=-1;//сброс клавиатурного символа
				return;
			}
step4:
			ukaz_trass=0;
step3:
			while((ukaz_trass<100)&&(TRASSA0[ukaz_trass].object!=0xFFFF))//продолжать пока для объектов есть память
			{ Rez=sogl_otkr(TRASSA0[ukaz_trass].object&0xfff);//проверить очередной элемент
				if(Rez>=10)
				{
					ZERO_TRASSA(0);
					return;
				}
				if(fr1[TRASSA0[ukaz_trass].object&0xfff][0]==2)//если вышли на сигнал
				{
					if((TRASSA0[ukaz_trass].object&0xfff)!=point)//если не сигнал начала
					{
						//если то же направление
						if(fr1[point][1]==(fr1[TRASSA0[ukaz_trass].object&0xfff][1]))
						{
							if(mane==1)//если маневровый маршрут
							{
								if(fr1[point][1]==0)//если четный маршрут
								//если есть конец четного маневрового
								if(((fr1[TRASSA0[ukaz_trass].object&0xfff][11])&4)==4)break;

								if(fr1[point][1]==1)//если нечетный маршрут
								//если есть конец нечетного маневрового
								if(((fr1[TRASSA0[ukaz_trass].object&0xfff][11])&1)==1)break;

							}
							else //если поездной
							{
								if(fr1[point][1]==0)//если четный маршрут
								//если есть конец четного поездного
								if(((fr1[TRASSA0[ukaz_trass].object&0xfff][11])&64)==64)break;

								if(fr1[point][1]==1)//если нечетный маршрут
								//если есть конец нечетного поездного
								if(((fr1[TRASSA0[ukaz_trass].object&0xfff][11])&16)==16)break;

							}
						}
					}
				}
				if(Rez==-1){vibeg=0;return;}
				if(Rez==2) goto fini;//если ограничения введены оператором - выйти
				if((Rez==1)||
				//если неисправна стрелка(сигнал,участок,путь),стрелка без контроля,
				//на пути башмаки, открыт враждебный сигнал или
				(Rez==3)||   //
				(Rez==4)||
				(Rez==5))//стрелка на макете
				{
					if(PROV_SCB==1)
					{
						podtver();//если все предупреждения кроме fr4 - запрос на выполнение
						vih_vopr=2;//установка флага выдачи предупреждения и ожидания ответа
					}
					else
					{
						soob_for_oper=1;
						menu_N=0;
						return;
					}
					if((Rez==1)&&(fr1[TRASSA0[ukaz_trass].object&0xfff][0]==3))ukaz_trass--;
					if((Rez==1)&&(fr1[TRASSA0[ukaz_trass].object&0xfff][0]==4))
					ukaz_trass--;
					if((Rez!=4)&&(Rez!=3)&&(Rez!=5))
					{
						ukaz_trass++;
						maka=0;
					}
					if(Rez==5)maka=5;//если стрелка на макете
					klaval=-1;
					return;
				}
				ukaz_trass++;//иначе перейти к следующему объекту
			}
			//если есть 2 белых сигнала на светофоре
			//для 2-го белого используется следующий объект
			menu_N=0;
			for(j1=0;j1<10;j1++)BEDA[j1]=0;
			if(fr1[point][6]==3)
			{ //для поездного используется основной объект
				if(manevro==0)w(132,point,"");//ПОСЫЛАЕМ КОМАНДУ НА ОТКРЫТИЕ СИГНАЛА
				if(manevro==1)w(132,point-1,"");// ПОСЫЛАЕМ КОМАНДУ НА ОТКРЫТИЕ СИГНАЛА
			}
			else
			{
				del_kursor(i3);
				okno_otkaz=0;
				w(132,point,"");// ПОСЫЛАЕМ КОМАНДУ НА ОТКРЫТИЕ СИГНАЛА
			}
		}
		if(oper!=0)
		{
			for(ii=0;ii<100;ii++)
			{
				if((TRASSA0[ii].object&0xfff)==0xfff)break;
				if((fr1[TRASSA0[ii].object&0xfff][0]==3)||
				(fr1[TRASSA0[ii].object&0xfff][0]==4))
				{
					if(fr3[TRASSA0[ii].object&0xfff][1]==1)break;
					if(fr3[TRASSA0[ii].object&0xfff][2]==1)break;
					if(fr3[TRASSA0[ii].object&0xfff][3]==1)break;
				}
				fr3[TRASSA0[ii].object&0xfff][10]=fr3[TRASSA0[ii].object&0xfff][10]|1;
				fr3[TRASSA0[ii].object&0xfff][8]=TRASSA0[ii+1].object&0xfff;
				if(fr1[TRASSA0[ii].object&0xfff][0]==1)//если объект - стрелка
				{
					if(fr3[TRASSA0[ii].object&0xfff][0]==0)//если стрелка в минусе
					if(fr3[TRASSA0[ii].object&0xfff][1]==1)
					fr3[TRASSA0[ii].object&0xfff][10]=fr3[TRASSA0[ii].object&0xfff][10]|0x8000;
				}
				if(fr1[TRASSA0[ii].object&0xfff][0]==5)//если объект - путь
				{
					if(mane==0)//если поездной
					{
						if(fr1[point][1]==0)//если четный
						fr3[TRASSA0[ii].object&0xfff][10]=fr3[TRASSA0[ii].object&0xfff][10]|0xa0;

						if(fr1[point][1]==1)//если нечетный
						fr3[TRASSA0[ii].object&0xfff][10]=fr3[TRASSA0[ii].object&0xfff][10]|0x50;
					}
					if(mane==1)//если маневровый
					{
						if(fr1[point][1]==0)//если четный
						fr3[TRASSA0[ii].object&0xfff][10]=fr3[TRASSA0[ii].object&0xfff][10]|0x20;

						if(fr1[point][1]==1)//если нечетный
						fr3[TRASSA0[ii].object&0xfff][10]=fr3[TRASSA0[ii].object&0xfff][10]|0x10;
					}
				}
			}
			ris_trassa(0);
			ZERO_TRASSA(0);
			fotksig(oper);//заполнение буфера выдачи команды
			oper=0;
		}
fini:
		if(danet==1)
		for(ukaz_trass=0;ukaz_trass<100;ukaz_trass++)
		TRASSA0[ukaz_trass].object=0xFFFF;//окончание работы для стрелок
		home(modi);//курсор - "ДОМОЙ!
		//сброс всех флагов
		basha=0;manevro=0;mane=0;manevr=0;vibeg=0;vih_vopr=0;
		maka=0;//обнуление признака выдачи предупреждения о макете на стрелке
		return;
	}
	else
	{
		if(klaval==32)//если сброс (нажатие пробела) после выбора сигнала
		{ vibeg=0;//сброс флагов задания вопросов
			vih_vopr=0;
			if(danet==1)//сброс массивов
			for(ukaz_trass=0;ukaz_trass<100;ukaz_trass++)
			TRASSA0[ukaz_trass].object=0xFFFF;//окончание работы для стрелок
			whole_finish(0);
			//если была активизирована отмена,сбросить активность отмены
			if(ot==1){vkl_kno(0,8);ot=0;}
			if(vrem==1){uprav=1;puti=0;}
			t(0);
			mane=0;manevr=0;manevro=0;basha=0;
			return;
		}
		else return;
	}
}
//------------------------------------------------------------------------
/***************************************************\
* Процедура подготовки (взведения) АРМа для выдачи  *
* команды  переключения комплектов ТУМС.            *
\***************************************************/
void perekl_komp(int nom)
{
	nom_func("218");
	if(((fr3[nom][0]==0)&&(fr3[nom][1]==0)&&(fr3[nom][2]==0))||
  ((fr3[nom][0]==1)&&(fr3[nom][1]==0)&&(fr3[nom][2]==0)))
  { //ПЕРЕКЛЮЧИТЬ КОМПЛЕКТ? 
    vopros(1,9999,113,"");oppp=1;puti=1;return;
  }
  else 
  { //ПЕРЕКЛЮЧЕНИЕ КОМПЛЕКТОВ НЕВОЗМОЖНО 
    w(40,999,"");oppp=0;return;
  }
}
//---------------------------------------------------------------------
/*********************************************************\
* Программа подготовки к выдаче команды перевода стрелки. *
* Включает в себя тестирование состояния стрелки и выдачу *
* предупреждений оператору, а также выбор кода команды.   *
\*********************************************************/
void perstr()
{
  int point1,ij,avtr;
	nom_func("219");
	point1=point;
#ifdef AVTOD
  avtr=prov_ele_avtod(point);
  if(avtr==1){w(245,999,"");return;}
  if(avtr==2){w(244,999,"");return;}
#endif
  if(vibeg==1) goto step2;
  if(vib_ma==1) goto ddo;// если переводится стрелка на макете
  if(vih_vopr==1)goto step2;//если ранее был задан вопрос для подтверждения
  osya=0;//обнуление счетчика выданных ненорм по объекту
  for(ij=0;ij<10;ij++)BEDA[ij]=0;
  hod=0;//сброс флага входа в проверку для анализа спаренных стрелок 
  if(Opred()<0)return;//поиск объекта информационных пакетов
          //для стрелки(имеет смысл для спаренной стрелки)
  if(fr4[point][1]==1)//Если стрелка на макете
  { w(99,markery[modi][6]," НА МАКЕТЕ, ПОСЫЛАТЬ КОМАНДУ?");
    vib_ma=1;//установка флага работы с макетом
    zapretvybora=1;
    return;
	}
ddo:
  //если стрелка без контроля
  if(net_kontro==2){fi_co1=0;se_co1=1;goto beda;}
  //если потеря контроля
  if(fr3[point][5]==1){net_kontro=1;return;}
  if(fr3[point][0]==fr3[point][1]==1){net_kontro=1;return;}
  else
  {
beda:
    if(((fr3[point][0]==1)&&(fr3[point][1]==0))||  //если стрелка в плюсе или
    ((net_kontro==2)&&((oper=='B')||  //стрелка без контроля и перевод в минус
    (oper=='F'))))//или стрелку без контроля на макете переводят в минус
    { if(net_kontro==2) vopros(0,point1,125,"");//без контр.СТРЕЛКУ ## В МИНУС?
      else vopros(0,markery[modi][6],125,"+");// с контролем СТРЕЛКУ ##+ В МИНУС?
      if(fr4[point][1]==0) oper='B';//если не на макете - команда простая
      else oper='F'; // если на макете - команда макетная
      net_kontro=0;//снять признак отсутствия контроля
    }
    else
    { //далее аналогично для перевода стрелки в плюс
      if(net_kontro==2) vopros(0,point1,124,"");
      else vopros(0,markery[modi][6],124,"-");// СТРЕЛКУ В ПЛЮС?
      if(fr4[point][1]==0) oper='A';
      else oper='E';
      net_kontro=0;
    }
    vibeg=1;
    zapretvybora=1;
    return;
  }
step2:
	vibeg=0;
  vib_ma=0;
	if((osya>=4)&&(osya<10))osya=0;
  else
  { //выполнить проверку возможности перевода
		osya=tst_strelok(point,1,0);
    if(osya==10){home(modi);finish_strelok();return;}
  }
  //если нет запрета на перевод стрелки
  if(osya==0)
  { // ПОСЫЛАЕМ КОМАНДУ НА ПЕРЕВОД СТРЕЛКИ
    okno_otkaz=0;
    if(AVARI==0){zapretvybora=0;w(123,point1,"");}
    else
    { //ОЖИДАЕТСЯ НАЖАТИЕ КНОПКИ СЧЕТЧИКА "ВСП.ПЕРЕВОД"
      w(55,999,"");AV_zdem=1;
    }
    for(ij=0;ij<10;ij++)BEDA[ij]=0;
    formkps(oper);
  }
  else
  if((osya>1)&&(PROV_SCB==1))
  { if(AVARI==0){vih_vopr=1;podtver();klaval=0;return;}
    else
    {if(osya<=2){vih_vopr=1;podtver();klaval=0;return;}
      else
      {home(modi);finish_strelok();klaval=0;return;}
    }
	}
	del_kursor(modi);
  show_kursor(modi);
  osya=0;vih_vopr=0;
	return;
}
//------------------------------------------------------------
/*****************************************************\
* Процедура выдачи команды на переключение комплектов.*
* nom - номер объекта переключения комплектов         *
\*****************************************************/
void prkl(int nom)
{
	nom_func("238");
	if (klaval==13)
	{ //ПОСЫЛАЕМ КОМАНДУ НА ПЕРЕКЛЮЧЕНИЕ КОМПЛЕКТОВ
		t(0);w(41,999,"");home(modi);
		oppp=0;puti=0;
		form_kom_prkl(nom);
		return;
	}
	else return;
}
//---------------------------------------------------------------
void smen_napr(int nom1)
{
	nom_func("309");
	if((nikuda==1)||(klo==1)||(help!=0)) return;
	if(fr3[fr1[nom1][9]][7]!=0)
	{
		w(237,999,"");return;//СМЕНА НАПРАВЛЕНИЯ НЕ ВОЗМОЖНА
	}
	if((fr3[nom1][2]==1)||(fr3[nom1][5]==1))
	{ oppp=1;
    #ifdef POLUAVTOMAT
      w(0,999,"");//Дать согласие на прием невозможно: занят перегон
    #else
      w(182,999,"");//СМЕНА НАПРАВЛЕНИЯ НЕВОЗМОЖНА: ЗАНЯТ ПЕРЕГОН
    #endif
    home(modi);oppp=0;
    return;
  }
  else
  if(fr3[nom1][1]==1)
  { oppp=1;
    w(170,999,"");//направление менять не надо
zz:
    home(modi);oppp=0;
    return;
  }
  else
  { if(oppp==0)vopros(17,markery[modi][6],117,"?"); //отправление
    oppp=1;puti=1;zapretvybora=1;
    return;
  }
}
//----------------------------------------------------
void smena(int nom)
{
	nom_func("310");
	if (klaval==13)
  {
    #ifdef POLUAVTOMAT 
    w(2,999,"");
    #else
    w(183,999,"");
    #endif
    home(modi); oppp=0;puti=0;
    form_kom_smen(nom);
  }
  return;
}
//----------------------------------------------------
void start_time_date()
{
	nom_func("319");
	rg.h.ah=0x2a;        // дата
  int86(0x21,&rg,&rg);
  dayy_=rg.h.dl;
  monn_=rg.h.dh;
  yearr_=rg.x.cx;
  rg.h.ah=0x2c;       // время
  int86(0x21,&rg,&rg);
  ho_ur=rg.h.ch;
  mi_n=rg.h.cl;
  se_c=rg.h.dh;
  return;
}
//------------------------------------------------------------------------
#ifdef POLUAVTOMAT
//------------------------------------------------------------------------
/*************************************************\
* Процедура выдачи предварительных вопросов       *
* оператору после нажатия им кнопки дачи согласия *
* на смену направления с соседней станции.        *
\*************************************************/
void kom_dso(int ndso_chdso)
{
  int npo_chpo,nfp_chfp,nfdp_chfdp;
  int bit_ndso_chdso,bit_npo_chpo,bit_nfp_chfp,bit_nfdp_chfdp;
  bit_ndso_chdso=fr1[ndso_chdso][3];
  npo_chpo=fr1[ndso_chdso][4]; bit_npo_chpo=fr1[ndso_chdso][5];
  nfp_chfp=fr1[ndso_chdso][6]; bit_nfp_chfp=fr1[ndso_chdso][7];
  nfdp_chfdp=fr1[ndso_chdso][8];bit_nfdp_chfdp=fr1[ndso_chdso][9];
  if(nikuda==1||klo==1||help!=0)return;

  //если перегон занят
  if(fr3[npo_chpo][bit_npo_chpo]==1)
	{
		w(0,999,""); //"дать согласие на прием невозможно - перегон занят"
		home(modi);
		return;
	}
  // если нет согласия
  if(fr3[ndso_chdso][bit_ndso_chdso]==0)
  {
		vopros(18,ndso_chdso,84,"?"); // ВЫДАТЬ СОГЛАСИЕ НА ПРИЕМ СО СТАНЦИИ?
		oppp=1;zapretvybora=1;
		return;
	}
  else
  if(fr3[ndso_chdso][bit_ndso_chdso]==1)
	{
		vopros(18,ndso_chdso,85,"?");	// СНЯТЬ СОГЛАСИЕ НА ПРИЕМ?
		oppp=1;zapretvybora=1;
		return;
	}
  return;
}
//-------------------------------------------------------------------
/*************************************************\
* Процедура формирования команд выдачи и снятия   *
* согласия на прием с соседней станции. Команда   *
* нужна для разрешения смены направления на пере- *
* гоне с соседней станции при полуавтоматике.     *
* ndso_chdso - объект в базе данных для воздейств.*
\*************************************************/
void k_dso(int ndso_chdso)
{
  unsigned char koda;
  int npo_chpo,nfp_chfp,nfdp_chfdp;
	int bit_ndso_chdso,bit_npo_chpo,bit_nfp_chfp,bit_nfdp_chfdp,n_bait;
	bit_ndso_chdso=fr1[ndso_chdso][3];// бит воздействия
	npo_chpo=fr1[ndso_chdso][4];      //объект
	nfp_chfp=fr1[ndso_chdso][6];      //объект фактического прибытия
	nfdp_chfdp=fr1[ndso_chdso][8];    //объект дачи фактического прибытия
	bit_npo_chpo=fr1[ndso_chdso][5];  //бит
	bit_nfp_chfp=fr1[ndso_chdso][7];  //бит фактического прибытия
	bit_nfdp_chfdp=fr1[ndso_chdso][9];//бит дачи фактического прибытия
	if(nikuda==1||klo==1||help!=0)return;
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(klaval==13)
  {
    if(fr3[ndso_chdso][bit_ndso_chdso]==0) //если согласие не выдавалось
		{
			vopros(17,ndso_chdso,86,"!");/* ВЫДАЕМ СОГЛАСИЕ НА ПРИЕМ НА СТАНЦИЮ##*/
			koda='A';
		}
		if(fr3[ndso_chdso][bit_ndso_chdso]==1)//если согласие уже выдано
		{
			vopros(17,ndso_chdso,87,"!");/* СНИМАЕМ СОГЛАСИЕ НА ПРИЕМ НА СТАНЦИЮ##*/
			koda='M';
		}
		home(modi);
		oppp=0;
		point=ndso_chdso;
		fotksig(koda);
		buf_ko[n_bait-2]=check_sum(buf_ko);
		pooo_com[ndso_chdso]=biostime(0,0L);
		com_bit[ndso_chdso]=koda&0xF;
		flagoutmsg=1;
	}
  return;
}
//-----------------------------------------------------------------------
/*****************************************************\
* Процедура предъявления оператору текстовых вопросов *
* при первичной инициализации кнопки фактической дачи *
* прибытия для полуавтоматической блокировки.         *
\*****************************************************/
void kom_fdp(int nfdp_chfdp)
{
	int ndso_chdso,npo_chpo,nfp_chfp;
  int bit_ndso_chdso,bit_npo_chpo,bit_nfp_chfp,bit_nfdp_chfdp;
  ndso_chdso=fr1[nfp_chfp][2];
  bit_ndso_chdso=fr1[ndso_chdso][3];
  npo_chpo=fr1[nfp_chfp][4];
  nfp_chfp=fr1[nfdp_chfdp][6];
  bit_npo_chpo=fr1[nfp_chfp][5];
  bit_nfp_chfp=fr1[nfp_chfp][7];
  bit_nfdp_chfdp=fr1[nfp_chfp][9];
  if(nikuda==1||klo==1||help!=0)return;

  if(fr3[nfdp_chfdp][bit_nfdp_chfdp]==0)//если нет дачи прибытия
  {
		vopros(17,nfdp_chfdp,161,"?");//"ДАЧА ПРИБЫТИЯ?"
		oppp=1;zapretvybora=1;
		return;
	}
  else
  if(fr3[nfdp_chfdp][bit_nfdp_chfdp]==1)

  {
		vopros(18,nfdp_chfdp,3,"");//"ПРИБЫТИЕ УЖЕ ВЫДАНО"
		oppp=0;home(modi);
		return;
	}
  return;
}
//---------------------------------------------------------------
/******************************************************\
* Процедура формирования команды для дачи фактического *
* прибытия для принимающей станции на полуавтоматике.  *
* nfdp_chfdp - объект в базе для воздействия.          *
\******************************************************/
void k_fdp(int nfdp_chfdp)
{
  unsigned char koda;
  int ndso_chdso,npo_chpo,nfp_chfp;
  int bit_ndso_chdso,bit_npo_chpo,bit_nfp_chfp,bit_nfdp_chfdp;
  ndso_chdso=fr1[nfp_chfp][2];
  bit_ndso_chdso=fr1[ndso_chdso][3];
  npo_chpo=fr1[nfdp_chfdp][4];
  nfp_chfp=fr1[nfdp_chfdp][6];
  bit_npo_chpo=fr1[nfp_chfp][5];
  bit_nfp_chfp=fr1[nfp_chfp][7];
  bit_nfdp_chfdp=fr1[nfp_chfp][9];
  if(nikuda==1||klo==1||help!=0)return;
  if(klaval==13)
  { //если фактическое прибытие еще не выдавалось
    if(fr3[nfdp_chfdp][bit_nfdp_chfdp]==0)
    {
      vopros(18,nfdp_chfdp,13,"!");//ПОСЫЛАЕМ КОМАНДУ ДАЧИ ПРИБЫТИЯ
      koda='A';
    }
    home(modi);
    oppp=0;
    point=nfdp_chfdp;
		fotksig(koda);
#ifdef NEW_FORMAT
		buf_ko[10]=check_sum(buf_ko);
#else
		buf_ko[9]=check_sum(buf_ko);
#endif
		pooo_com[nfdp_chfdp]=biostime(0,0L);
		com_bit[nfdp_chfdp]=koda&0xf;
		flagoutmsg=1;
  }
  return;
}
#endif
//---------------------------------------------------------
void vkl_avto()
{ //процедура реализации предварительного диалога при вкл./откл. автодействия
  int avt_plt,bt_avt;
	nom_func("394");
	if(markery[modi][6]!=9999)//если имеется пультовое автодействие
  {
    avt_plt=markery[modi][6]&0xfff;//номер объекта пультового автодействия
    bt_avt=(markery[modi][6]&0xf000)>>12;
    if(fr3[avt_plt][bt_avt]!=0){w(258,999,"");return;}
  }
  if(markery[modi][7]-777==0)//если нажата кнопка нечетного автодействия
    if(na==0) w(26,999,"");//если оно выключено"ВКЛЮЧИТЬ АВТОДЕЙСТВИЕ ПО 1П?"
    else w(28,999,"");//если включено "ВЫКЛЮЧИТЬ АВТОДЕЙСТВИЕ ПО 1П?"
  else
   if (markery[modi][7]-777==1)//если нажата кнопка четного автодействия
    if(cha==0) w(27,999,"");//если оно выключено"ВКЛЮЧИТЬ АВТОДЕЙСТВИЕ ПО 2П?"
    else w(29,999,"");//если включено "ВЫКЛЮЧИТЬ АВТОДЕЙСТВИЕ ПО 2П?"
  zapretvybora=1;oppp=1;puti=1; return;
}
//-------------------------------------------------
void vkl_dn(int nom)
{ int ob_au,ob_dn,ob_nn;
	ob_nn=fr1[nom][3];
	ob_dn=fr1[nom][5];
	ob_au=fr1[nom][9];
	nom_func("395");
	if(ob_au!=9999)//если есть кнопка "автомат"
  { if(nom==ob_au)//если нажата кнопка "автомат"
    { if(COD_DN>7)//если автоматический режим уже установлен
      //АВТОМАТИЧ.РЕЖИМ РАБОТЫ СИГНАЛОВ УЖЕ УСТАНОВЛЕН
      { w(202,999,"");oppp=0;puti=0;home(modi);return;}
      else vopros(1,9999,203,"?");//установить режим "автомат"?
    }
  }
  if((nom==ob_nn)||(nom==ob_dn))//если нажата кнопка режима
  { if((COD_DN==2)||(COD_DN==3)||(COD_DN==8))vopros(1,9999,204,"?");//включить дневной режим?
		else
      if((COD_DN==4)||(COD_DN==5)||(COD_DN==9))vopros(1,9999,205,"?");//включить ночной режим?
			else {w(209,999,"");
			oppp=0;puti=0;home(modi);return;}
	}
	if((nom!=ob_au)&&( nom!=ob_dn)&&( nom!=ob_nn))
	{oppp=0;puti=0;home(modi);return;}
	oppp=1;puti=1;return;
}
/*********************************************\
*        процедура включения кнопки           *
\*********************************************/
void vkl_knop(int nom)
{
	int bit,// бит включения объекта
	ob_blk, //объект блокирования
  bt_blk, //бит блокирования
  soo_blk,//сообщение о блокировании
	vopr1,//вопрос перед включением
	vopr2;//вопрос перед отключением
	nom_func("397");
  if((fr1[nom][2]==5)&&(na==1)) { w(245,999,""); return; }
  if((fr1[nom][2]==6)&&(cha==1)){ w(244,999,""); return; }
  if(fr1[nom][1]==53)
	{
		bit=fr1[nom][12]&0xf;vopr1=fr1[nom][4];vopr2=fr1[nom][5];
	}
	else
	{
		if((fr1[nom][1]==163)||
		((fr1[nom][1]>=155)&&(fr1[nom][1]<=158)))
		{
			for(bit=0;bit<5;bit++)
			if((fr1[nom][bit+3]&0xff00)!=0)break;
			vopr1=fr1[nom][8];
			vopr2=fr1[nom][9];
		}
		else
		if(fr1[nom][1]!=166)
		{
			bit=fr1[nom][11]&0xf;
			vopr1=fr1[nom][8];
			vopr2=fr1[nom][9];
			if((fr1[nom][2]<33)&&(fr1[nom][7]!=9999))
			{
				ob_blk=fr1[nom][7]&0xfff;
				bt_blk=(fr1[nom][7]&0xf000)>>12;
				soo_blk=fr1[nom][6]&0xfff;
				if(fr3[ob_blk][bt_blk]==0){w(soo_blk,999,"");return;}
				if(fr3[ob_blk][5]==1){w(soo_blk,999,"");return;}
			}
		}
		else
		{
			bit=(fr1[nom][3]&0xf000)>>12;
			vopr1=fr1[nom][6];
			vopr2=fr1[nom][7];
		}
	}
	if((fr1[nom][2]==33)&&(fr1[nom][1]==16))//если кнопка смены направления
	{ if(fr3[nom][bit]==0)//если объект пассивен
		{
			if(vopr1!=9999) //если предусмотрен вопрос на включение
			{ //задать вопрос о включении
				setfillstyle(1,14),setcolor(4);
				vopros(18,markery[modi][6],vopr1,"?");
			}
			else {oppp=0;puti=0;home(modi);return;}//если вопроса нет, то выйти
		}
		else// если объект не пассивен
		{
			if(fr3[nom][bit]==1)//если объект активен
			{
				if(vopr2!=9999)//если предусмотрен вопрос на отключение
				{ setfillstyle(1,14),setcolor(4);
					vopros(18,markery[modi][6],vopr2,"?");
				}
				else {oppp=0;puti=0;home(modi);return;}
			}
		}
	}
	else//для всех остальных кнопок
	{
		if(fr3[nom][bit]==0)//если объект пассивен
		{
			if(vopr1!=9999)//если есть текст на включение
			{
				setfillstyle(1,14),setcolor(4);
				if((fr1[nom][1]!=163)&&((fr1[nom][1]<155)||(fr1[nom][1]>158)))
				vopros(1,9999,vopr1,"?");
				else
				if((fr1[nom][12]&0xf)!=0)//если связь с массивом otopri
				{
					vopros(18,markery[modi][6],vopr1,"?");
				}
				else vopros(1,9999,vopr1,"?");
			}
			else {w(229,999,"");oppp=0;puti=0;home(modi);return;}
		}
		else //если объект активен
		{
			if(fr3[nom][bit]==1)
			{
				if((fr1[nom][1]!=163)&&((fr1[nom][1]<155)||(fr1[nom][1]>158))
				&&(fr1[nom][2]==2))//если не предусмотрено отключение
				{
					w(229,999,"");return;
				}
				if(vopr2!=9999)//если есть текст на отключение
				{
					setfillstyle(1,14),setcolor(4);
					if((fr1[nom][1]!=163)&&
					((fr1[nom][1]<155)||(fr1[nom][1]>158)))
					vopros(1,9999,vopr2,"");
					else
					{
						if((fr1[nom][12]&0xf)!=0)//если связь с массивом otopri
						{
							vopros(18,markery[modi][6],vopr2,"?");
						}
						else vopros(1,9999,vopr2,"");
					}
				}
				else {oppp=0;puti=0;home(modi);return;}
			}
		}
	}
	oppp=1;puti=1;return;
}
//--------------------------------------------------------------
void vkl_nemarsh(int nom)
{ int otvet;
	nom_func("398");
	if(fr1[nom][2]==33)
  { if(fr3[nom][fr1[nom][11]]==0)
      if(fr1[nom][8]!=9999){setfillstyle(1,14),setcolor(4);vopros(18,markery[modi][6],fr1[nom][8],"?");}
      else {oppp=0;puti=0;home(modi);return;}
    else
      if(fr3[nom][fr1[nom][11]]==1)
        if(fr1[nom][9]!=9999){setfillstyle(1,14),setcolor(4);vopros(18,markery[modi][6],fr1[nom][9],"?");}
        else{oppp=0;puti=0;home(modi);return;}
  }
  else
  { if(fr3[nom][fr1[nom][11]]==0)
      if(fr1[nom][8]!=9999){setfillstyle(1,14),setcolor(4);vopros(1,9999,fr1[nom][8],"?");}
      else {oppp=0;puti=0;home(modi);return;}
    else
      if(fr3[nom][fr1[nom][11]]==1)
        if(fr1[nom][9]!=9999){setfillstyle(1,14),setcolor(4);vopros(1,0,fr1[nom][9],"?");}
				else {oppp=0;puti=0;home(modi);return;}
  }
  if(fr3[nom][fr1[nom][11]]==0)
  {
		nach_marsh_oper=fr1[nom][2];
		end_marsh_oper=fr1[nom][7];
    END_OPER=end_marsh_oper;
    if(may_be(nach_marsh,fr1[nach_marsh][1],0)==0)
    {
			ukaz_trass=0;
			while(TRASSA0[ukaz_trass].object!=0xffff)
			{
				if(fr1[TRASSA0[ukaz_trass].object&0xfff][0]==1)plus_minus(0);
				fr3[TRASSA0[ukaz_trass].object&0xfff][10]=
				fr3[TRASSA0[ukaz_trass].object&0xfff][10]|1;
				otvet=test_elem(TRASSA0[ukaz_trass].object&0x7fff,2,0,ukaz_trass);
				if(otvet==1)goto not;
				ukaz_trass++;
			}
			ris_trassa(0);
			oppp=1;
			puti=1;
			return;
		}
		w(83,999,"");
not:
		whole_finish(0);
		return;
	}
	else
	{
		oppp=1;
		puti=1;
		return;
	}
}
//------------------------------------------------------------------
void vkl_opoveshe(int nom)
{ // объект для РМ, объект для ЗМ
	int bz1,bz2,bb;
	nom_func("400");
	bz1=fr1[nom][2];bz2=fr1[nom][7];bb=fr1[bz1][11];
  if((fr3[bz1][bb]==0)&&(fr3[bz2][bb]==0))
  //ВКЛЮЧИТЬ ОПОВЕЩЕНИЕ?
  { vopros(1,9999,38,"");zapretvybora=1;}
  else
  {
    if((fr3[bz1][bb]==1)&&(fr3[bz2][bb]==1))
    //ОТКЛЮЧИТЬ ЗАПРЕТ МОНТЕРАМ?
    { vopros(1,9999,39,"");zapretvybora=1;}
    else
      if((fr3[bz1][bb]==1)&&(fr3[bz2][bb]==0))
      {
        menu_N=4;
        punkt_N=1;
        klaval=-1;
        regs.x.ax=4;
        regs.x.cx=590;
        regs.x.dx=50;
        int86(0x33,&regs,&regs);
        X_m=590;
        Y_m=50;
				obnov_kur_mouse(590,50);
        menu1();
        return;
      }
      else
      {oppp=0;puti=0;home(modi);return;}
  }
  oppp=1;
  puti=1;
  return;
}
//-------------------------------------------------
void vopr_mar()
{
	int end_marsh1;
	nom_func("403");
	if((end_marsh_oper!=0)&&(half_marsh1==0))nach_marsh_oper=end_marsh_oper;
	end_marsh_oper=markery[modi][6];
	if(markery[modi][7]==7)end_marsh1=fr1[end_marsh_oper][2];
	else end_marsh1=end_marsh_oper;
	if((fr1[nach_marsh_oper][5]!=0)&&
	((fr1[nach_marsh_oper][5]&0x00ff)==end_marsh1)||
	(((fr1[nach_marsh_oper][5]&0xFF00)>>8)==end_marsh1))//если может быть вариант
	{
		if(fr1[end_marsh_oper][4]==10)
		{ if(fr1[nach_marsh_oper][1]==fr1[end_marsh_oper][1])
			{ menu_N=5;
				punkt_N=1;
				klaval=-1;
			}
		}
		else
		if(fr1[end_marsh_oper][4]==11)
		{ menu_N=5;
			punkt_N=1;
			klaval=-1;
		}
		Zdem_vibor_marshruta=1;
		return;
	}
	//если чисто вариантная точка
	if(fr1[end_marsh_oper][1]==2)
	{
		w(148,999,"");
		return;
	}
	if((end_marsh_oper!=0)&&(nach_marsh_oper!=0))voper();
	return;
}
//------------------------------------------------------
int ZAGO_MPSU(int Coc1)
{
	int BZ;
	int mask3y=0x08;//второй основной установить
  int mask2y=0x04;//первый основной установить
  int mask3n=0x77;//второй основной сбросить
  int mask2n=0x7b;//первый основной сбросить
	nom_func("425");
	BZ=Coc1;
  if(NOMER_ARMA==1)
  {//если основная ПЭВМ, установить основным первый
    if(STATUS==1){BZ=BZ|mask2y;BZ=BZ&mask3n;}
    if((STATUS==0)||(STATUS==2)){BZ=BZ&mask2n;BZ=BZ|mask3y;}//установить основным второй
  }
	if(NOMER_ARMA==2)
  {
		if(STATUS==1){BZ=BZ|mask3y;BZ=BZ&mask2n;}
    if(STATUS==0){BZ=BZ|mask2y;BZ=BZ&mask3n;}
    if(STATUS==2){BZ=BZ&mask2n;BZ=BZ&mask3n;}
  }
  if((TELEUP==1)&&(DU==1)&&(SEZON==0))BZ=BZ|16;
  if(((TELEUP==1)||(SEZON==1))&&(DU==0))BZ=BZ|32;
  return(BZ);
}
//-------------------------------------------------------------------------
/*************************************************************\
* Процедура вывода данных в буфер СПДЛП. Вызывается из main() *
* для основного АРМа оператора.                               *
\*************************************************************/
void ZAP_BUF_SPD()
{
	nom_func("433");
#ifdef SPDLP
	if(spdlp!=0)return;
	if(STATUS!=1)
	{
		BUF_OUT_SPD[0]=0x16;
		BUF_OUT_SPD[1]=0x5;
		BUF_OUT_SPD[2]=0x4;
		spdlp=1;
		ukaz_spd=3;
		ukaz_out_spd=0;
		return;
	}
	BUF_OUT_SPD[0]=0x16;
	BUF_OUT_SPD[1]=0x5;
	BUF_OUT_SPD[2]=0x80;
	BUF_OUT_SPD[3]=LEN_SPD;
	BUF_OUT_SPD[4]=adr_spdlp;
	BUF_OUT_SPD[5]=0x1c;
	BUF_OUT_SPD[6]=ho_ur;
	BUF_OUT_SPD[7]=mi_n;
	BUF_OUT_SPD[8]=se_c;
	ukaz_spd=9;
	while(ukaz_spd<75)
	{
		for(iz=0;iz<K64;iz++)//пройти по платам 64 контактным
		{
			BUF_OUT_SPD[ukaz_spd++]=0x80|iz;//записать имя платы
			for(jz=0;jz<8;jz++)//пройти по всем байтам платы
			{
				for(kz=0;kz<8;kz++) //пройти по всем битам байта
				{
					n_rpc=64*iz+8*jz+kz;
					if(fr3[byte_spdlp[n_rpc]][bt_spdlp[n_rpc]]==1)
						BUF_OUT_SPD[ukaz_spd]=BUF_OUT_SPD[ukaz_spd]|(0x80>>kz);
					else
						if(fr3[byte_spdlp[n_rpc]][bt_spdlp[n_rpc]]==0)
						BUF_OUT_SPD[ukaz_spd]=BUF_OUT_SPD[ukaz_spd]&(~(0x80>>kz));
				}
				ukaz_spd++;
			}
		}
		for(iz=0;iz<K32;iz++)
		{
			BUF_OUT_SPD[ukaz_spd++]=iz+K64;
			for(jz=0;jz<4;jz++)
			{
				for(kz=0;kz<8;kz++)
				{
					n_rpc=64*K64+8*jz+kz;
					if(fr3[byte_spdlp[n_rpc]][bt_spdlp[n_rpc]]==1)
						BUF_OUT_SPD[ukaz_spd]=BUF_OUT_SPD[ukaz_spd]|(0x80>>kz);
					else
						if(fr3[byte_spdlp[8*jz+kz]][bt_spdlp[8*jz+kz]]==0)
						BUF_OUT_SPD[ukaz_spd]=BUF_OUT_SPD[ukaz_spd]&(~(0x80>>kz));
				}
				ukaz_spd++;
			}
		}
		jz=0;
		for(iz=1;iz<ukaz_spd;iz++)jz=jz+BUF_OUT_SPD[iz];
		BUF_OUT_SPD[ukaz_spd++]=jz%256;
		BUF_OUT_SPD[ukaz_spd]=0x4;
		ukaz_out_spd=0;
		break;
	}
	spdlp=1;
#endif
	return;
}
//---------------------------===------------------------------------------
void OUT_SPD()
{
	unsigned char ts;
	nom_func("212");
	if(spdlp==0)return;
	inportb(BAZ_ADR6);
	ts=inportb(BAZ_ADR6+5);
	if((ts&0x20)==0x20)
	{

		outportb(BAZ_ADR6,BUF_OUT_SPD[ukaz_out_spd]);
		BUF_OUT_SPD[ukaz_out_spd]=0;
		ukaz_out_spd++;
	}
	else return;
	if(ukaz_out_spd>ukaz_spd)
	{
		spdlp=0;
		ukaz_spd=0;
		ukaz_out_spd=0;
		return;
	}
	return;
}
//-----------------------------------------------
void Zona_k()
{
	nom_func("436");
	if(klo==1||help!=0) return;
	if(nikuda==1) return;
	if(ZONA_KURSORA==ZONA_N)ZONA_KURSORA=ZONA_CH;
	else ZONA_KURSORA=ZONA_N;
	pat=i3;
	del_kursor(pat);
	i3=ZONA_KURSORA;
	show_kursor(i3);
	pat=i3;
  modi=i3;
  return;
}
//--------------------------------------------------------
void zp_zp(int nnn)
{
	int n_bait,koda=0;
	nom_func("438");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	prorisovka=0;
	NOM_NEXT=nnn+1;
	if((fr3[nnn][0]==1)&&(fr3[NOM_NEXT][1]==1))koda='M';
	else koda='A';
	point=nnn;
	if(form_kosig(koda)==1)
	{
		if(form_kodop(koda)==1)return;//если формирование команды не удалось - выход
	}
	buf_ko[n_bait-2]=check_sum(buf_ko,1);
	pooo_com[nnn]=biostime(0,0L);
	com_bit[nnn]=koda&0xF;
	TIMER_SOB(nnn,66,nnn);
	buf_ko[0]='('; buf_ko[10]=')';
	kom_v_bufer(fr1[nnn][13]-1);
	if((fr3[NOM_NEXT][1]==1)&&(koda=='M'))koda='N';
	else koda='B';
	point=NOM_NEXT;
	if(form_kosig(koda)==1)
	{
			if(form_kodop(koda)==1)return;//если формирование команды не удалось - выход
	}
	buf_ko[0]='(';
	buf_ko[n_bait-2]=check_sum(buf_ko,1);
	buf_ko[n_bait-1]=')';
	kom_v_bufer(fr1[NOM_NEXT][13]-1);
	flagoutmsg=0;zapretvybora=0;NOM_NEXT=0;puti=0;oppp=0; prorisovka=1;
	return;
}
//--------------------------------------------------------------
void voper()
{
  char mess[10]="";

	nom_func("402");
	t(0);
	setfillstyle(1,14);bar(476,1,635,27);moveto(478,8);
	setcolor(4);outtext(" МАРШРУТ ОТ ");
	strcpy(mess,pako[nach_marsh_oper]);outtext(mess);strcpy(mess,"");
	moveto(500,17);strcpy(mess,pako[end_marsh_oper]);
	if((fr1[nach_marsh_oper][1]==fr1[end_marsh_oper][1])||
	((mane==1)&&(fr1[end_marsh_oper][3]==7)))outtext(" ДО     , ДА ?  ");
	else outtext(" ЗА     , ДА ?  ");
	moveto(532,17);outtext(mess);setlinestyle(0,0,0);rectangle(478,4,633,26);
	vozvr=1;
	klaval=-1;
	poka=0;
	zapretvybora=1;
	return;
}
//----------------------------------------
//процедура выдачи вопросительного предложения в окно экрана
void vopros(int longo,int nomo,int N_v,unsigned char Sy[8])
{
//longo - вариант размера,цвета и расположения текста
  int CVE,CVEB,lp,lp1;
  char NAME[25]="",Nam[8]="";
	int xv,yv,exv,eyv;
	nom_func("404");
	t(0);
  switch(longo)
  {
		case 0: yv=1; eyv=16; break;
    case 1: yv=1; eyv=18; break;
    case 2: yv=1; eyv=18; break;
    case 3: yv=1; eyv=24; break;
    case 17:yv=0; eyv=28; break;
    case 18:yv=0; eyv=28; break;
    case 19:yv=1; eyv=28; break;
    default: return;
  }
  lseek(file_soob,N_v*63L,0L);
  read(file_soob,&soob.lex,60);
  for(lp=0;lp<60;lp++)soob.lex[lp]=~soob.lex[lp];
  while(soob.lex[lp]==0x20){soob.lex[lp]=0;lp--;}
	read(file_soob,&soob.COlo,1);
  CVE=soob.COlo;
  if(CVE==14)CVEB=4;
  if(CVE==2)CVEB=15;
  if(CVE==4)CVEB=15;
  if(longo==17)
	xv=639-max(10+strlen(soob.lex)*8,20+strlen(otopri[fr1[nomo][10]])*8);
	else xv=639-(7+strlen(soob.lex)+strlen(Sy))*7;
  exv=639;
  wind_w(xv,yv,exv,eyv,SOLID_FILL,CVE,CVEB);
  moveto(xv+8,yv+4);
  outtext(soob.lex);
  if(longo==17)
  { moveto(xv+1,yv+17);
    outtext(otopri[fr1[nomo][10]]);
    outtext(Sy);
    return;
  }
  if(longo==18)
  { moveto(xv+8,yv+17);
		if((fr1[nomo][1]!=163)&&
		((fr1[nomo][1]<155)||(fr1[nomo][1]>158)))
		outtext(otopri[fr1[nomo][7]]);
		else outtext(otopri[(fr1[nomo][12]&0xf)-1]);
		outtext(Sy);
		return;
	}
	if(longo==19)
	{ outtext(" ДЛЯ");
		moveto(xv+8,yv+17);
		outtext("ВСПОМОГАТ. ПЕРЕВОДА");
		return;
	}
	if(nomo==9999){outtext(Sy);return;}
	if(fr1[nomo][0]==1) //если стрелка
	{ lp=0;
		while(pako[nomo][lp]!=146)lp++;
		lp++;
		for(lp1=0;lp1<25;lp1++)NAME[lp1]=0;
		lp1=0;
		while(pako[nomo][lp]!=':'){NAME[lp1]=pako[nomo][lp];lp++;lp1++;}
	}
	else
	{ lp1=0;
		NAME[lp1++]=32;
		for(lp=0;lp<25;lp++)
		{ NAME[lp1]=0;
			if(pako[nomo][lp]!=32)NAME[lp1++]=pako[nomo][lp];
		}
		NAME[lp1]=0;
	}
  outtext(NAME);
  xv=getx(); yv=gety(); moveto(xv,yv);
  strcpy(Nam,Sy);outtext(Nam);
  return;
}
//------------------------------------------------------------------
void zad_marsh_avto(int Nmarsh)
{ //процедура задания маршрута
	int n_bait,vv=0,n_strel=0,ii,napr,stoyk;
	unsigned long pol_str=0l;
	nom_func("424");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	buf_ko[2]='b';//то же самое, если поездной маршрут в одной стойке
	vv=vibor(nach_avto);//получить код-индекс для задания начала
	buf_ko[1]=BAM;//заполнить заголовок
	if(vv!=9999)buf_ko[4]=vv; //заполнить код-индекс для сигнала начала
	else return;
	buf_ko[3]=podgr;//установить подгруппу начала
	vv=vibor(end_avto);//получить код-индекс для задания конца
	if(vv!=9999)  buf_ko[6]=vv; //заполнить код-индекс для сигнала конца
	else return;
	buf_ko[5]=podgr;//установить подгруппу конца
#ifdef NEW_FORMAT
	napr=fr1[nach_avto][1];
	stoyk=fr1[nach_avto][13]-1;
	for(ii=0;ii<K_EL;ii++)
	{
		if(fr1[Avto_el[Nmarsh].Elems[ii]][0]==1)//если стрелка
		{
			if(fr1[Avto_el[Nmarsh].Elems[ii]][7]==napr)//если стрелка определяющая
			{
				n_strel++;
			}
		}
	}
	buf_ko[7]=0x40|n_strel;
	buf_ko[8]=0x40|(pol_str&0x3f);
	buf_ko[9]=0x40|((pol_str&0xfc0)>>6);
	buf_ko[7]=buf_ko[7]|MYTHX_TEC[stoyk];
	TUMS_RABOT[stoyk]=0xF;
#else
	buf_ko[7]=124;//в оставшиеся байты записать заполнитель
	buf_ko[8]=124;
#endif
zc:
	buf_ko[0]='(';
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//дополнить команду контрольной суммой
	buf_ko[n_bait-1]=')';
	pooo_com[nach_avto]=biostime(0,0L);//зафиксировать время выдачи
	com_bit[nach_avto]=buf_ko[2]&0xF;
	flagoutmsg=1;//выставить флаг-требование на передачу в ТУМС
	return;
}
//----------------------------------------------------------------
void zad_marsh(int tipus)
{ //процедура задания маршрута
	int vv=0;
	nom_func("423");
	if(tipus!=0)
	{
		for(vv=0;vv<100;vv++)
		{
			fr3[TRASSA[vv].object&0xfff][8]=TRASSA[vv+1].object&0xfff;
			if(fr3[TRASSA[vv].object&0xfff][8]==0xfff)break;
		}
	}
	else
	{
		for(vv=0;vv<100;vv++)
		{
			if(fr3[TRASSA0[vv+1].object&0xfff][8]==0xfff)break;
			fr3[TRASSA0[vv].object&0xfff][8]=TRASSA0[vv+1].object&0xfff;

		}
	}
	TUMS_MARSH(tipus);
	return;
}

//-----------------------------------------------------------------
//формирования команды на закрытие сигнала
void zakrsi()
{ int stit=9999,ii,jj,tk,sl,napr,test_null=0,maska;
	int i_m,i_s,i_sig;
	nom_func("428");

#ifdef AVTOD
	for(i_m=0;i_m<(skoko_stoek*3);i_m++)
	{
		for(i_s=0;i_s<skoko_stoek;i_s++)
		{
			for(i_sig=0;i_sig<10;i_sig++)
			{
				if(MARSHRUT_ALL[i_m].SIG[i_s][i_sig]==point)
				{
//					add(i_m,8888,'5'); $$
					DeleteMarsh(i_m);
					ii=15;
					break;
				}
			}
			if(ii==15)break;
		}
		if(ii==15)break;
	}
	if((fr3[point][8]!=0)||(fr3[point][10]!=0))
	{
		i_m=point;
		while(1)
		{
			i_s=fr3[i_m][8];
			fr3[i_m][8]=0;
			fr3[i_m][10]=0;
			if(fr1[i_s][0]==2)
			{
				if((fr1[i_s][1]==fr1[point][1])&&(i_s!=point))
				{
					if((fr1[point][6]==0)&&(fr1[i_s][6]!=1))break;
					if((fr1[point][6]==1)&&(fr1[i_s][6]!=0))break;
					if((fr1[point][6]==2)&&(fr1[i_s][6]==1))break;
					if((fr1[point][6]==2)&&(fr1[i_s][6]==0))break;
				}
			}
			switch(fr1[i_m][0])
			{
				case 1: displaystrelka(i_m,0,0);break;
				case 3: zap_matr(i_m);cvet_matr();ris_sp_str();break;
				case 4: displaypathuch(i_m);break;
				case 5: displaypath(i_m,0);break;
				default: break;
			}
			i_m=i_s;
			if((i_m==0)&&(fr3[i_m][8]==0))break;
			if((i_m<0)||(i_m>=kol_VO))break;
		}
	}
	stit=prov_avtod1(point);//проверить вхождение в автодействие
	if(stit!=-1)//если сигнал входит в маршрут установленного автодействия
	{ if(stit<=1)
		{
			snyato_avto(1);//если в нечетное - снять нечетное
			zvuk_vkl(0,0);
		}
		else
			if((stit>1)&&(stit<=3))
			{
				snyato_avto(2);//если в четное - снять четное
				zvuk_vkl(0,0);
			}
		w(137,point,"автод.снято");//ПОСЫЛАЕМ КОМАНДУ НА ОТМЕНУ МАРШРУТА ОТ СИГНАЛА
	}
	else //если сигнал не входит в маршрут автодействия
#endif
	if (vrem==1)vrem=5;//установка признака выполнения закрытия сигнала

	if((ot==1)&&(fr3[point][0]==0)&& //если отмена и закрыт маневровый и
	(fr3[point][1]==0))//закрыт поездной
	{ if(fr1[point][6]!=2)//если сигнал не совмещенный
		{
			if(fr1[point][2]==1){oper='F';goto vyh1;}//если поездной
			else {oper='E';goto vyh1;}//если маневровый
		}
		if(fr1[point][1]==0)napr=3; //если четный сигнал
		else napr=2;

		if(punkt_N!=0){punkt_N=0;menu_N=0;goto vydat;}
		jj=point;
repit:
		stit=findspu1(jj,napr); //найти ближайшее СП за сигналом
		if((stit==-1)&&(jj<kol_VO))
		{
			if(napr==2)jj++;
			else jj--;
			goto repit;
		}
		if(stit==-1){w(44,point," 5");return;}//если СП не найдено Ошибка прог. №5
		if(napr==3)//если направление сигнала четное
		{
			if((fr3[stit][12]&1)==1)mane=1;//если замыкание четное маревровое
			else
				if((fr3[stit][12]&2)==2){mane=0;manevro=0;}//если замыкание четное поездное
		}
		else
		if(napr==2)//если направление сигнала нечетное
		{
			if((fr3[stit][12]&4)==4)mane=1;//если замыкание нечетное маревровое
			else
				if((fr3[stit][12]&8)==8){mane=0;manevro=0;}//если замыкание нечетное поездное
		}
		else
		//если нет типа замыкания и вообще нет замыкания
		if(((fr3[stit][12]&0xf)==0)&&(fr3[stit][1]==0)&&(fr3[stit][10]==0))
		{
			w(142,stit," (по типу маршрута)");
			oper=0;
			goto otm;
		}
		else
		{
			menu_N=1;
			punkt_N=1;
			klaval=-1;
			regs.x.ax=4; //курсор к меню
			regs.x.cx=590;
			regs.x.dx=40;
			int86(0x33,&regs,&regs);
			X_m=590;
			Y_m=40;
			obnov_kur_mouse(590,40);

			regs.x.ax=7; //границы мыши по горизонтали
			regs.x.cx=590;
			regs.x.dx=630;
			int86(0x33,&regs,&regs);

			regs.x.ax=8; //границы мыши по вертикали
			regs.x.cx=5;
			regs.x.dx=40;
			int86(0x33,&regs,&regs);
			menu1();
			return;
		}
vydat:
		if(fr1[point][6]==2) //если сигнал совмещенный
		{ if((manevro==1)||(mane==1)) oper='E';//если работа с маневровым сигналом
      else oper='F';//если работа с поездным
		}
    else//если сигнал не совмещенный
    if(fr1[point][2]==1)oper='F';//если поездной
    else oper='E';//если маневровый
    w(137,point,"");//ПОСЫЛАЕМ КОМАНДУ НА ОТМЕНУ МАРШРУТА ОТ СИГНАЛА
  }
  else
    if(fr3[point][0]==1)oper='E';//если открыт маневровый
		else oper='F';//если не открыт маневровый
vyh1:
	w(137,point,"");//ПОСЫЛАЕМ КОМАНДУ НА ОТМЕНУ МАРШРУТА ОТ СИГНАЛА
otm:
	if(ot==1)//если установлен признак "отмена"
	{
		vkl_kno(0,8);//кнопку "Отмена" на экране перекрасить в серый
		ot=0;//сбросить признак отмен
		jj=0;
		tk=point;
		maska=0;
		if(fr1[tk][0]==2)
		{
			if(fr1[tk][1]==0)maska=0x50;
			else maska=0xA0;
		}
		while(1)
		{
			TRASSA_RIS[jj++].object=tk;
			sl=fr3[tk][8]&0xfff;
			if((sl<0)||(sl>=kol_VO))break;
			fr3[tk][8]=0;
			fr3[tk][10]=fr3[tk][10]&(~maska);
			if((fr3[tk][10]&0xf0)==0)fr3[tk][10]=0;
			fr3[tk][12]=0;
			if(sl==0)test_null++;
			if(test_null>1)break;
			tk=sl;
			if(jj>=100)break;
		}
		ris_trassa(2);
		ZERO_TRASSA(2);
		jj=0;
	}
  clear_mouse();
  for(ii=0;ii<4;ii++){line_gor_kurs[4+2*ii]=0xff;line_gor_kurs[5+2*ii]=0xf8;}
  line_gor_kurs[0]=0xC;
  for(ii=4;ii<40;ii++)line_vert_kurs[ii]=0x80;
	line_vert_kurs[2]=8;
	return;
}
//--------------------------------------------------
//=============================================================================
/*******************************************************************\
*   TUMS_MARSH(i_m) - Модуль подготовки  маршрутных команд для ТУМС *
*  i_m - номер строки глобальных маршрутов с новым маршрутом        *
\*******************************************************************/
void TUMS_MARSH(int tipus)
{
	int ii=0,i_s=0,s_m,last_end,first_beg,jj,test,tip0,tip1;
	int i_m;
	unsigned int obkt0,obkt1;
	unsigned char tums_tek,tums_pred,adr_kom;
	unsigned char n_strel,kmnd,perevod_str;
	unsigned long pol_strel=0l;
	test=0;
	//если набрана трасса
	if(((TRASSA[0].object)&&(tipus!=0))||
	((TRASSA0[0].object)&&(tipus==0)))
	{
//		add(0,0,'5');//$$
		//если в трассе и строке одинаковые маршруты
		for(i_m=0;i_m<(skoko_stoek*3);i_m++)
		if(tipus!=0)
		{
			if(MARSHRUT_ALL[i_m].NACH==(TRASSA[0].object&0xFFF))
			{
				//получить команду и конец
				kmnd=MARSHRUT_ALL[i_m].KMND;
				END=MARSHRUT_ALL[i_m].END;
				break;
			}
		}
		else
		{
			if(MARSHRUT_ALL[i_m].NACH==(TRASSA0[0].object&0xFFF))
			{
				//получить команду и конец
				kmnd=MARSHRUT_ALL[i_m].KMND;
				END=MARSHRUT_ALL[i_m].END;
				break;
			}
		}
		if(i_m>=(skoko_stoek*3))
		{
			ZERO_TRASSA(tipus);
			return;
		}
		ii=0; //начать с первого
		last_end=-1,first_beg=-1;//установить начало поиска

		if(tipus!=0)tums_pred=TRASSA[0].stoyka;
		else tums_pred=TRASSA0[0].stoyka;
		tums_tek=tums_pred;            //взять ТУМС

		//считаем число стрелок, которые надо перевести
		for(i_s=0;i_s<skoko_stoek;i_s++)
		test=test+MARSHRUT_ALL[i_m].KOL_STR[i_s];

		//устанавливаем признак перевода стрелок
		if(test==0)perevod_str=0;
		else perevod_str=0xf;

		while(ii<100)//проход по всей трассе
		{
			if(tipus!=0)
			{
				tip0=TRASSA[ii].tip;
				obkt0=TRASSA[ii].object;
			}
			else
			{
				tip0=TRASSA0[ii].tip;
				obkt0=TRASSA0[ii].object;
			}
			//если сигнал или конец данных
			if((tip0==2)||(tip0==0)||(obkt0==0xFFFF))
			{
				if(tipus!=0)tums_tek=TRASSA[ii].stoyka;
				else tums_tek=TRASSA0[ii].stoyka;
				if(tums_tek!=tums_pred)//переход в другую стойку
				{
					if((first_beg&0x8000)==0x8000)//если реального начала нет
					{
						last_end=-1;first_beg=-1;tums_tek=tums_pred; //сброс начала и конца
					}
				}

				//переход в другую стойку или конец трассы
				if((tums_tek!=tums_pred)||(obkt0==0xFFFF))
				{
					//если есть реальное начало
					if((last_end!=-1)&&(first_beg!=last_end))//если есть конец и начало
					{
						i_s=tums_pred-1;
						//фиксируем вхождение стойки i_s в маршрут
						MARSHRUT_ALL[i_m].STOYKA[i_s]=1;

						if(perevod_str!=0) //если маршрут требует перевода стрелок
						{
							MARSHRUT_ALL[i_m].SOST=0x43;// маршрут предварительный
							if(MARSHRUT_ALL[i_m].KOL_STR[i_s]!=0) //если в этой стойке перевод
							{
								MARSHRUT_ALL[i_m].STOYKA[i_s]=2; //фиксируем вхождение с переводом
							}
							else goto NEXT; //если в данной стойке нет перевода -  идти далее
						}
						else //если стрелки стоят по маршруту
						{
							MARSHRUT_ALL[i_m].SOST=0x83; //маршрут исполнительный
						}

						 //пройти по строчкам локальной таблицы
						for(s_m=0;s_m<MARS_STOY;s_m++)
						//если маршрут в работе - прервать
						if((MARSHRUT_ST[i_s][s_m].NUM-100)==i_m)break;

						if(s_m>=MARS_STOY)//если этот маршрут не в работе
							//найти свободное место
							for(s_m=0;s_m<MARS_STOY;s_m++)
							if(MARSHRUT_ST[i_s][s_m].NUM==0)break; //если чисто - прервать

						//если нет свободной структуры для локального задания - завершить
						if(s_m>=MARS_STOY)goto out;

						//если строка свободна - начинаем писать команду
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[0]='(';
						//сформировать адрес
						switch(tums_pred)
						{
							case 1: adr_kom=ZAGO_MPSU(0x61);break;
							case 2: adr_kom=ZAGO_MPSU(0x62);break;
							default:return;
						}
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[1]=adr_kom;
						//готовим предварительную или исполнительную команду
						if(perevod_str==0)MARSHRUT_ST[i_s][s_m].NEXT_KOM[2]=kmnd;
						else MARSHRUT_ST[i_s][s_m].NEXT_KOM[2]=kmnd|8;
						//заполняем описание для начала маршрута
						if(tipus!=0)
						{
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[3]=TRASSA[first_beg].podgrup;
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[4]=TRASSA[first_beg].kod_bit;
							//заполняем описание для конца маршрута
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[5]=TRASSA[last_end].podgrup;
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[6]=TRASSA[last_end].kod_bit;
						}
						else
						{
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[3]=TRASSA0[first_beg].podgrup;
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[4]=TRASSA0[first_beg].kod_bit;
							//заполняем описание для конца маршрута
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[5]=TRASSA0[last_end].podgrup;
							MARSHRUT_ST[i_s][s_m].NEXT_KOM[6]=TRASSA0[last_end].kod_bit;
						}
						//готовим число и положение стрелок
						n_strel=0;
						pol_strel=0;
						for(jj=first_beg;jj<=last_end;jj++)
						{
							if(tipus!=0){tip1=TRASSA[jj].tip;obkt1=TRASSA[jj].object;}
							else {tip1=TRASSA0[jj].tip;obkt1=TRASSA0[jj].object;}

							if(tip1==1)//если стрелка
							{
								if((obkt1&0x4000)==0x4000)//если противошерстная
								{
									if((obkt1&0x2000)!=0x2000) //если нет глушения
									{
										if((obkt1&0x8000)==0x8000)//если в минусе
										{
											pol_strel=pol_strel|(1<<n_strel);
										}
										n_strel++;
									}
								}
							}
						}

						//заполняем число стрелок
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[7]=0x40|n_strel;
						//заполняем положение для первых 6 стрелок
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[8]=0x40|(pol_strel&0x3f);
						//заполняем положение для вторых 6 стрелок
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[9]=0x40|((pol_strel&0xFC0)>>6);
						//считаем контрольную сумму
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[10]=0;
						//закрываем пакет команды
						MARSHRUT_ST[i_s][s_m].NEXT_KOM[11]=')';
						add(i_s,6666,s_m);

						//устанавливаем состояние локального маршрута
						if(perevod_str!=0)MARSHRUT_ST[i_s][s_m].SOST=0x47;
						else MARSHRUT_ST[i_s][s_m].SOST=0x87;

						//запоминаем номер строки маршрута
						MARSHRUT_ST[i_s][s_m].NUM=i_m+100;


NEXT:
						first_beg=-1;
						last_end=-1;
					}
					else//если нет начала или конца
					{
						if(last_end==first_beg)first_beg=-1;
						if(last_end==-1)first_beg=-1;
					}
				}
				tums_pred=tums_tek; //совместить стойки

				//если сигнал может быть началом
				if((obkt0&0x8000)==0x8000)
				{
					if(first_beg)//если устанавливается команда на перевод стрелок
					{
						if(first_beg==-1)first_beg=ii;
					}
					else //если устанавливается команда на открытие сигнала
					{
						if(kmnd=='b') //поездной
						{
							if((obkt0&0x2000)==0x2000) //есть поездное
							{
								if(first_beg==-1)first_beg=ii; //если не было начала - взять
							}
							else//если нет поездного показания
								if(first_beg==-1)first_beg=0x8000|ii;//псевдоначало
						}
						else if(first_beg==-1)first_beg=ii;//для маневрового всегда начало
					}
				}//конец анализа сигнала для начала

				if((obkt0&0x4000)==0x4000)//если сигнал может быть концом
				{
					if(first_beg!=-1)
					{
						last_end=ii; //если было начало, то взять конец
						if(last_end==first_beg)last_end=-1;
					}
				}
			}
			if((tip0==0)&&(obkt0==0xffff))break;
			ii++;//идти далее
		}
out:
		ZERO_TRASSA(tipus);
	}

}
//---------------------------------------
void ZERO_TRASSA(nom)
{
	int ii=0;
	if(nom==1)
	{
		fr3[TRASSA[ii].object&0xfff][6]=0;
		TRASSA[ii].object=0;
		TRASSA[ii].tip=0;
		TRASSA[ii].stoyka=0;
		TRASSA[ii].kod_bit=0;
		TRASSA[ii++].podgrup=0;

		while(1)
		{
			if(TRASSA[ii].object==0xffff)break;
			if((TRASSA[ii].object&0xfff)>=kol_VO)break;
			if((TRASSA[ii].object==0)&&(TRASSA[ii].tip==0))break;
			fr3[TRASSA[ii].object&0xfff][6]=0;
			TRASSA[ii].object=0xffff;
			TRASSA[ii].tip=0;
			TRASSA[ii].stoyka=0;
			TRASSA[ii].podgrup=0;
			TRASSA[ii++].kod_bit=0;
			if(ii>=100)break;
		}
	}
	if(nom==0)
	{
		fr3[TRASSA0[ii].object&0xfff][6]=0;
		TRASSA0[ii].object=0;
		TRASSA0[ii].tip=0;
		TRASSA0[ii].stoyka=0;
		TRASSA0[ii].kod_bit=0;
		TRASSA0[ii++].podgrup=0;
		while(1)
		{
			if(TRASSA0[ii].object==0xffff)break;
			if((TRASSA0[ii].object&0xfff)>=kol_VO)break;
			if((TRASSA0[ii].object==0)&&(TRASSA0[ii].tip==0))break;
			fr3[TRASSA0[ii].object&0xfff][6]=0;
			TRASSA0[ii].object=0xffff;
			TRASSA0[ii].tip=0;
			TRASSA0[ii].stoyka=0;
			TRASSA0[ii].podgrup=0;
			TRASSA0[ii++].kod_bit=0;
			if(ii>=100)break;
		}
	}
	if(nom==2)
	{
		TRASSA_RIS[ii].object=0;
		TRASSA_RIS[ii].tip=0;
		TRASSA_RIS[ii].stoyka=0;
		TRASSA_RIS[ii].kod_bit=0;
		TRASSA_RIS[ii++].podgrup=0;
		while(1)
		{
			if(TRASSA_RIS[ii].object==0xffff)break;
			if((TRASSA_RIS[ii].object&0xfff)>=kol_VO)break;
			if((TRASSA_RIS[ii].object==0)&&(TRASSA_RIS[ii].tip==0))break;
			TRASSA_RIS[ii].object=0xffff;
			TRASSA_RIS[ii].tip=0;
			TRASSA_RIS[ii].stoyka=0;
			TRASSA_RIS[ii].kod_bit=0;
			TRASSA_RIS[ii++].podgrup=0;
			if(ii>=100)break;
		}
	}
}