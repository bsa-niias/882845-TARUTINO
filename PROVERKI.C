#include"bla.h"
int ZA_SI,pusya;
//---------------------------------------------------
int test_time()
{
	nom_func("354");
	second_time=biostime(0,0);
	if((second_time-first_time)>=TIME_FIX)
	{ Init_TEST_SOOB(0);//сбросить ячейки 10-минутной новизны
		Init_TEST_SOOB(1);
		return(1);
	}
	else return(0);
}
/*************************************************************************\
* Процедура проверки возможности задания маршрута автодействия номер kkp  *           *
* Вызывается периодически при работе системы для выявления условий работы *
* автодействия.Если можно установить маршрут автодействия - возвращает 1, *
* если маршрут автодействия установить нельзя , то возвращает 0.          *
\*************************************************************************/
int avto_marsh(int kkp)
//
{ int ij,SEKC,test;
	nom_func("12");
	if(Avto_el[kkp].Flag == 1)goto end;					//если маршрут уже выдавался
	if(kkp>=3)for(ij=0;ij<4;ij++)Avto_el[ij].Flag=0;//если выдавались уже все маршруты
	nach_avto=Avto_el[kkp].Beg_s;                   //взять начало маршрута
	end_avto=Avto_el[kkp].End_s;                    //взять конец маршрута
	SEKC=Avto_el[kkp].N_sek;                        //взять защитный участок
	ij=0;
	while(Avto_el[kkp].Elems[ij]!=nach_avto)ij++;   //найти начало автомаршрута

	//если четный сигнал и четное автодействие
	if((fr1[nach_avto][1]==0)&&(cha==1))
	{
		//если сигнал начала еще не закрыт, то выйти
 		if((fr3[nach_avto][0]!=0)||(fr3[nach_avto][1]!=0)||
		(fr3[nach_avto][2]!=0)||(fr3[nach_avto][3]!=0))goto end;

		//если закрыт двигаться по трассе автодействия
		for(ij=0;ij<=K_EL;ij++)
		{

			test=test_elem(Avto_el[kkp].Elems[ij],kkp+10,1,0);

			if(test==1)goto end;                //если элемент не в норме - конец

			if(test==7)													//если надо включить РОН/РОЧ
			{ knopa(fr1[Avto_el[kkp].Elems[ij]][6]&0xfff);
				klaval=13;
				goto end;
			}

			if(Avto_el[kkp].Elems[ij]==SEKC)
			break;//защитная секция - выйти
		}
		if(MAR_GOT[fr1[nach_avto][13]-1]==64)goto end;//стойка занята - выйти
		return(1);//все в порядке - выход
	}

	//если нечетные сигнал и нечетное автодействие
	if((fr1[nach_avto][1]==1)&&(na==1))
	{
		//если сигнал начала не закрыт, то выйти
		if((fr3[nach_avto][0]!=0)||(fr3[nach_avto][1]!=0)||
		(fr3[nach_avto][2]!=0)||(fr3[nach_avto][3]!=0))goto end;

		//сигнал закрыт - двигаться по трассе автодействия
		for(ij=0;ij<=K_EL;ij++)
		{

			test=test_elem(Avto_el[kkp].Elems[ij],kkp+10,1,0);
			if(test==1)goto end;      //если элемент не в норме - конец
			if(test==7)//если надо включить РОН/РОЧ
			{
				knopa(fr1[Avto_el[kkp].Elems[ij]][6]&0xfff);
				klaval=13;
				goto end;
			}
			if(Avto_el[kkp].Elems[ij]==SEKC)
			break;//защитная секция - выйти
		}
		if(TUMS_RABOT[fr1[nach_avto][13]-1]!=0)goto end;
		return(1);//все в порядке - выйти
	}
end:
		return(0);           //нельзя включать автодействие
}
//---------------------------------------------------------------------
/*************************************************\
* Процедура поиска конца предполагаемого маршрута *
* возвращает объект за встречным сигналом.        *
\*************************************************/
int find_end(int tip)
{
	int mo,e_m,n_m;
	nom_func("58");
	//если концом маршрута является попутный сигнал, то оставить его
	if(tip!=0){e_m=end_marsh;n_m=nach_marsh;}
	else {e_m=end_marsh_oper;n_m=nach_marsh_oper;}
	mo=e_m;
	if(fr1[e_m][1]==fr1[n_m][1])return(e_m);
	else
	while(1) //бесконечный цикл
	{
		//если поиск вывел за пределы базы, то завершить.
		if((mo<0)||(mo>kol_VO))return(9999);

		//если вышли на сигнал
		if(fr1[mo][0]==2)
		{ //если сигнал встречный - уйти в конец
			if(fr1[mo][1]!=fr1[n_m][1])goto konec;
			else//если сигнал попутный
			{
				if(((mane==0)&&(tip==0))|| //если маршрут поездной
				((mane1==0)&&(tip==1)))
				{
					//если маршрут четный и у сигнала есть конец четного поездного
					if((fr1[n_m][1]==0)&&((fr1[mo][11]&64)==64))
					{
						if(fr1[n_m][1]==1)mo--;//вернуться на шаг
						else mo++;
						return(mo);
					}
					//если маршрут нечетный и у сигнала есть конец нечетного поездного
					if((fr1[n_m][1]==1)&&((fr1[mo][11]&16)==16))
					{
						if(fr1[n_m][1]==1) mo--;//вернуться на шаг
						else mo++;
						return(mo);
					}
				}
			}
		}
konec:
		// если вышли не на сигнал или сигнал встречный
		// или если маршрут маневровый
		// надо идти по маршруту вперед
		if(((mane!=0)&&(tip==0))||//если маршрут маневровый
		((mane1!=0)&&(tip==1)))
		{
			//а сигнал является поездным, то ехать можно только до него,
			//поэтому за ним ничего не ищем
			if((fr1[mo][0]==2)&&(fr1[mo][6]==1))return(mo);
		}
		if(fr1[n_m][1]==1) mo++;// если маршрут нечетный
																	 // продвинуться дальше по базе

		else mo--; //если четный - вернуться на шаг по базе

		if(((mane!=0)&&(tip==0))||//если маршрут маневровый
		((mane1!=0)&&(tip==1)))
		{
			//если вышли на реальный объект за сигналом
			if((fr1[mo][0]!=6)&&(fr1[mo][0]!=7))return(mo);
			if(fr1[mo][0]==6)mo=fr1[mo][1];
		}

		else  //если маршрут поездной

		//если вышли на путь и это не полуавтоматика
		if((fr1[mo][0]==5)&&(fr1[mo][7]<8888))return(mo);
	}
}
//---------------------------------------------------------------------
/**************************************************************\
* процедура формирования команды для индивидуальной выдачи на  *
* объекты, управляемые экранными кнопками                      *
* nnn - номер в базе объекта, на который воздействует команда  *
* inver - признак инверсии в команде (0-нет инверсии)          *
\**************************************************************/
void form_kom_knopka(int nnn,int inver )
{
	int koda=0,n_bait;
	nom_func("75");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	koda=form_koda(nnn,inver);//получение кода команды
	if(koda==9999) return;//если получение кода не состоялось - выход
	point=nnn;//установить номер точки для выдачи команды
	//сформировать команду для объекта типа сигнал
	if(form_kosig(koda)==1) //если команду сформировать не удалось
	{ //сформировать команду для дополнительного объекта
		if(form_kodop(koda)==1)return;//если формирование команды не удалось-выход
	}
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//добавить контрольную сумму
	pooo_com[nnn]=biostime(0,0L);//зафиксировать время выдачи
	com_bit[nnn]=koda&3;

  flagoutmsg=1;//установить флаг-требование на передачу в ТУМС
	return;
}
//-------------------------------------------------------------------------
/**********************************************************\
*  Процедура выдачи команды для объектов управления        *
*  немаршрутизированными перемещениями от экранных кнопок. *
*  nnn - базовый объект для выдачи команды                 *
\**********************************************************/
void form_kom_nemarsh(int nnn)
{
	int koda=0,nobi=0,n_bait;
	int nomer,podgr,i18,j18,SDVIG=0,FN=0,FIN=0,n=0;
	nom_func("76");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	nobi=fr1[nnn][11];
	switch(nobi)
	{
		case 0:   if(fr3[nnn][nobi]==0)koda='A';
							else koda='M';
							break;
		case 1:   if(fr3[nnn][nobi]==0)koda='B';
							else koda='N';
							break;
		default: return;
	}
	point=nnn;//установить номер точки для выдачи команды
	n=point;//зафиксировать объект выбранный оператором
	if(fr1[n][13]==1)//если объект из первой стойки
	{
		buf_ko[1]=ZAGO_MPSU(0x61);//заголовок
		SDVIG=STR1;FN=0;FIN=SIG1;//сдвиг и граница
	}
#ifdef KOL_SOO2
	else//для второй стойки - аналогично
	{
		buf_ko[1]=ZAGO_MPSU(0x62);
		SDVIG=STR2;FN=SIG1;FIN=FN+SIG2;
	}
#endif
	nomer=9999;
	//пройти по подгруппам + пройти по объектам подгруппы
	if((koda=='A')||(koda=='B')) //если включить немаршрутизированное движ.
	{ buf_ko[2]='d'; nomer=9999;
		n=fr1[nnn][2]; //взять первый сигнал из базового описания
		//пройти по подгруппам + пройти по объектам подгрупп
		for (i18=FN;i18<FIN;i18++)for(j18=0;j18<=4;j18++)
		if(spsig[i18][j18]==n) //если найден объект
		{ podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
			nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
			break;
		}
		if(nomer==9999)return;
		buf_ko[3]=podgr;
		buf_ko[4]='@'+nomer;
		n=fr1[nnn][7];//взять второй сигнал
		//пройти по подгруппам + пройти по объектам подгруппы
		nomer=9999;
		for (i18=FN;i18<FIN;i18++)for(j18=0;j18<=4;j18++)
		if(spsig[i18][j18]==n) //если найден объект
		{ podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
			nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
			break;
		}
		if(nomer==9999)return;
		buf_ko[5]=podgr;buf_ko[6]='@'+nomer;buf_ko[7]=124;buf_ko[8]=124;
	}
	if(koda=='N') //если отменить немаршрутизированные передвижения
	{ buf_ko[2]='E';//заполнить группу
		nomer=9999;
		for (i18=FN;i18<FIN;i18++)for(j18=0;j18<=4;j18++)
		if(spsig[i18][j18]==n) //если найден объект
		{ podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
			nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
			break;
		}
		if(nomer==9999)
		{
			buf_ko[2]='Q';//заполнить группу
			if(fr1[n][13]==1)//если объект из первой стойки
			{
				buf_ko[1]=ZAGO_MPSU(0x61);//заголовок
				SDVIG=STR1+SIG1;FN=0;FIN=DOP1;//сдвиг и граница
			}
#ifdef KOL_SOO2
			else//для второй стойки - аналогично
			{
				buf_ko[1]=ZAGO_MPSU(0x62);
				SDVIG=STR2+SIG2;FN=DOP1;FIN=FN+DOP2;
			}
#endif
#if DOP1>0
			for (i18=FN;i18<FIN;i18++)for(j18=0;j18<=4;j18++)
			if(spdop[i18][j18]==n) //если найден объект
			{ podgr=podgruppa[i18+SDVIG-FN];//зафиксировать подгруппу
				nomer=j18; //зафиксировать порядковый номер объекта в подгруппе
				break;
			}
#endif
		}
		if(nomer==9999)return;
		for (j18=0;j18<=4;j18++)
		{
			buf_ko[j18+4]=124;// для прочих вписать заполнитель
			if(j18==nomer)buf_ko[j18+4]=koda;//для найденного объекта - заполнить код
		}
		buf_ko[3]=podgr; // заполнить подгруппу
	}
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//добавить контрольную сумму
	pooo_com[nnn]=biostime(0,0L);//зафиксировать время выдачи
	com_bit[nnn]=koda&3;
	flagoutmsg=1;//установить флаг-требование на передачу в ТУМС
	return;
}
//--------------------------------------------------------------------
/******************************************************\
* Процедура формирования команды на отключение -       *
* включение УКСПС                                      *
* nnn - номер в базе для объекта управления УКСПС      *
\******************************************************/
void form_kom_nk_chk(int nnn)
{
	int koda=0,n_bait,nom_bit;
	nom_func("77");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	koda=form_koda(nnn,0); //получение кода команды
	point=nnn;
	if(form_kosig(koda)==1) //если не удалось сформировать в группе сигнала
	{
		if(form_kodop(koda)==1)//если не удалось сформировать в доп.об. - выход
		return;
	}
	buf_ko[n_bait-2]=check_sum(buf_ko,1);
	pooo_com[nnn]=biostime(0,0);
	com_bit[nnn]=koda&3;
	flagoutmsg=1;
	return;
}
//-----------------------------------------------------------------------
/**************************************************\
* Процедура формирования команды для переключения  *
* комплектов ТУМС. nno - объект воздействия в базе.*
\**************************************************/
void form_kom_prkl(int nno)
{
	int i11,n=0,n_bait;

	nom_func("78");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	n=nno;
	if(fr1[n][13]==1)///если переключается первая стойка
	{
		buf_ko[1]=ZAGO_MPSU(0x61);//сформировать заголовок
		slom_interf(11200);//зафиксировать на диске
		buf_ko[3]=kompl_1;//установить байт подгруппы
	}
#ifdef KOL_SOO2
	else//если переключается вторая стойка
	{
		buf_ko[1]=ZAGO_MPSU(0x62);//установить заголовок
		slom_interf(11201);//записать на диск
		buf_ko[3]=kompl_2;//записать байт подгруппы
	}
#endif
	buf_ko[2]='L';//установить код группы
	for (i11=7;i11<=n_bait-3;i11++)buf_ko[i11]=124;//записать заполнители
	buf_ko[5]='D';//записать код команды
	buf_ko[4]=124;//записать заполнители
	buf_ko[6]=124;//записать заполнители
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//дополнить команду контрольной суммой
	pooo[nno]=biostime(0,0L);//зафиксировать время
	flagoutmsg=1;//выставить флаг-требование на передачу в ТУМС
	PRKL=1;//установить признак выполнения переключения
	return;
}
//--------------------------------------------------------------------
/******************************************************\
* Процедура формирования команды для смены направления.*
* nnn - объект базы для выдачи команды смены напр.     *
\******************************************************/
void form_kom_smen(int nnn)
{
	int koda=0,n_bait;

	nom_func("80");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	//если бит датчика - первый
	if(fr1[nnn][11]==1) koda='B';  //если управление сменой направления в ДПС
	else
		//если бит датчика - нулевой
		if(fr1[nnn][11]==0) koda='A';//если управление сменой направления в ДМС
		else return;
	point=nnn; //задать объект

	//попытка сформировать команду в группе сигналов
	if(form_kosig(koda)==1)//если попытк неудачная
	{
		//попытка сформировать команду в группе доп.объектов
		if(form_kodop(koda)==1)return;//если формирование команды неудачное-выход
	}
	buf_ko[n_bait-2]=check_sum(buf_ko,1);//дополнить команду контрольной суммой
	pooo_com[nnn]=biostime(0,0L);// зафиксировать время выдачи
	com_bit[nnn]=koda&3;

	flagoutmsg=1;//выставить флаг - требование на передачу в ТУМС
	return;
}
//---------------------------------------------------------
#ifdef OGRAD
kom_ograd(int ob_sogl)
{ int ob_zapros,ob_ogr,bit_zapros,bit_sogl,bit_ogr,put;
	put=fr1[ob_sogl][4];
	ob_zapros=fr1[ob_sogl][2]&0xfff;
	bit_zapros=(fr1[ob_sogl][2]&0xf000)>>12;

	ob_ogr=fr1[ob_sogl][3]&0xfff;
	bit_ogr=(fr1[ob_sogl][3]&0xf000)>>12;

	bit_sogl=(fr1[ob_sogl][12]&0xf000)>>12;

	if(nikuda==1||klo==1||help!=0)return;

	if((fr3[ob_zapros][bit_zapros]==1)/* если есть запрос ограждения */
	&&(fr3[ob_ogr][bit_ogr]==0)/* и ограждение не установлено */
	&&(fr3[ob_sogl][bit_sogl]==0))/* и согласие не установлено */
	// ВЫДАТЬ СОГЛАСИЕ НА ОГРАЖДЕНИЕ ПУТЬ ##?
	{ vopros(1,put,105,"?");oppp=1;zapretvybora=1;return;}
	else
	if(fr3[ob_ogr][bit_ogr==1])
	// ОГРАЖДЕНИЕ УСТАНОВЛЕНО ПУТЬ ##
	{ w(141,put,"");oppp=0;zvuk_vkl(1,10);home(modi);return;}
	else
		if(fr3[ob_sogl][bit_sogl]==1)
		// СОГЛАСИЕ НА ОГРАЖДЕНИЕ ВЫДАНО,ПУТЬ ##
		{w(34,put,"");oppp=0;zvuk_vkl(1,10);home(modi);return;}
		else
			if(fr3[ob_zapros][bit_zapros]==0)
			// ЗАПРОСА НА ОГРАЖДЕНИЕ НЕ БЫЛО,ПУТЬ ##
			{ w(155,put,"");oppp=0;zvuk_vkl(1,10);home(modi);return;}
	return;
}
#endif
//-----------------------------------------------------------------------
/*********************************************\
*  Процедура задания вопроса о конце маршрута *
\*********************************************/
void koni()
{
	nom_func("130");
	vopros(1,9999,121,"");//УКАЖИТЕ КОНЕЦ МАРШРУТА
	zapretvybora=0;manevro=0;
	return;
}
//------------------------------------------------------------------------
/*************************************************************\
* Процедура трассировки маршрута для маршрутного управления   *
* процедура вызывается два раза, первый раз трассируется      *
* эталонный маршрут(основной), второй раз тот, который задан. *
* Это делается для последующего сравнения основного(эталон) с *
* маршрутом, заявленным как вариантный, если трассы совпадают,*
* то признак вариантности снимается,независимо от объявления  *
* его оператором как вариантного.                             *
\*************************************************************/
int may_be(int ako,int nap_dvi,int tipus)
{ int ii,pred,jj,kk,SDVIG,FN,FIN,kod_end,fin,finish=0;
	int UK_TRASS,n_mane,VARIANT;

	nom_func("166");
	VARIANT=0;
	//трассировка маршрута для маршрутного управления
	//предварительная очистка трассы
	//END=find_end();
	//очистить трассу
	if(tipus!=0)
	{
		for(ukaz_trass=0;ukaz_trass<100;ukaz_trass++)
		{
			TRASSA[ukaz_trass].object=0xFFFF;
		}
	}
	else
	{
		for(ukaz_trass=0;ukaz_trass<100;ukaz_trass++)
		{
			TRASSA0[ukaz_trass].object=0xFFFF;
		}

	}
	kod_end=0;
	switch(nap_dvi)
	{
		case 0: if(((mane==1)&&(tipus==0))||
						((mane1==1)&&(tipus==1)))kod_end=4;//четное, маневровый
						if(((mane==0)&&(tipus==0))||
						((mane1==0)&&(tipus==1)))kod_end=64;//четное, поездной
					 break;

		default: if(((mane==1)&&(tipus==0))||
						 ((mane1==1)&&(tipus==1))) kod_end=1;//нечетное, маневровый
						 if(((mane==0)&&(tipus==0))||
						 ((mane1==0)&&(tipus==1))) kod_end=16;  //нечетное, поездной
						 break;
	}
	ukaz_trass=0;//указатель объекта в начало трассы
	if(tipus!=0)half_marsh=nach0_marsh;
	else half_marsh=nach0_marsh_oper;
//	end_marsh_oper=END_OPER;
	if((half_marsh>0)&&(ZAHOD==0))//если задается вариантный маршрут
	{
		if(half_marsh>kol_OSN)half_marsh=fr1[half_marsh][2];
		if(tipus!=0)
		{
			nach0_marsh=end_marsh; //запомнить основной конец
			END=half_marsh; //вариантную точку назначить концом
			half_marsh1=half_marsh; //запомнить промежуточную точку
			NE=fra[END][0]; //взять номер блок-линии конца
			NM=fra[nach_marsh][0]; //взять номер блок-линии начала
		}
		else
		{
			nach0_marsh_oper=end_marsh_oper; //запомнить основной конец
			END_OPER=half_marsh; //вариантную точку назначить концом
			half_marsh1=half_marsh; //запомнить промежуточную точку
			NE=fra[END_OPER][0]; //взять номер блок-линии конца
			NM=fra[nach_marsh_oper][0]; //взять номер блок-линии начала
		}
		ZAHOD=1;
	}
/*	if(tipus==0)
	{
		END=END_OPER;
		if(half_marsh!=0)end_marsh_oper=END;
	}*/
	while(1)//"бесконечный" цикл трассировки
	{
		if((fr1[ako][0]==2)&&(fr1[ako][1]<2))//если вышли на сигнал
		{
			if(fr1[ako][13]==1){SDVIG=STR1;FN=0;FIN=SIG1;}
			if(fr1[ako][13]==2){SDVIG=STR2;FN=SIG1;FIN=FN+SIG2;}

			for(jj=FN;jj<FIN;jj++)
			{
				for(kk=0;kk<5;kk++)
				{
					if(spsig[jj][kk]==ako)
					{
						if(tipus!=0)
						{
							TRASSA[ukaz_trass].podgrup=podgruppa[jj+SDVIG-FN];
							TRASSA[ukaz_trass].kod_bit='@'+kk;
							break;
						}
						else
						{
							TRASSA0[ukaz_trass].podgrup=podgruppa[jj+SDVIG-FN];
							TRASSA0[ukaz_trass].kod_bit='@'+kk;
							break;
						}
					}
				}
				if(kk<5)break;
			}
			if(jj>=FIN)
			{
				test_tmp=0;
				return(1);
			}
		}
		if(half_marsh!=0)//если вариантный маршрут
		{
			if((fr1[ako][0]==2)&&(fr1[ako][1]==2))//если вышли на вариантную точку
			{
				VARIANT=1;
				if(tipus!=0)
				{
					END=nach0_marsh;//установить основной конец
					half_marsh=0;//очистить для дальнейшего движения по главному направлению
					NE=fra[END][0];//запомнить блок-линию основного конца
				}
				else
				{
					END_OPER=nach0_marsh_oper;//установить основной конец
					half_marsh=0;//очистить для дальнейшего движения по главному направлению
					NE=fra[END_OPER][0];//запомнить блок-линию основного конца
				}
			}
		}
		else VARIANT=1;
		if(VARIANT==1)
		if(((tipus!=0)&&(ako==END))||//если дошли до конца маршрута
		((tipus==0)&&(ako==END_OPER)))
		{
			if(tipus!=0)
			{
				TRASSA[ukaz_trass].object=ako;//запомнить элемент трассы
				TRASSA[ukaz_trass].stoyka=fr1[ako][13];
			}
			else
			{
				TRASSA0[ukaz_trass].object=ako;//запомнить элемент трассы
				TRASSA0[ukaz_trass].stoyka=fr1[ako][13];
			}
			if((fr1[ako][11]&kod_end)==kod_end)
			{
				if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object|0x4000;
				else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object|0x4000;
			}
			if(tipus!=0)TRASSA[ukaz_trass++].tip=fr1[ako][0];
			else TRASSA0[ukaz_trass++].tip=fr1[ako][0];
			//если конец не является промежуточным
			if(((tipus!=0)&&((END!=half_marsh)||(END==0)))||
			((tipus==0)&&((END_OPER!=half_marsh)||(END_OPER==0))))
			{ if(tst_sigogo(ako,tipus)==0)
				{
						fin=ukaz_trass;
						for(ukaz_trass=0;ukaz_trass<fin;ukaz_trass++)
						{
							if(tipus!=0)
							{
								if(TRASSA[ukaz_trass].object==0xffff)break;
								finish=finish+test_elem(TRASSA[ukaz_trass].object&0xfff,0,1,ukaz_trass);
							}
							else
							{
								if(TRASSA0[ukaz_trass].object==0xffff)break;
								finish=finish+test_elem(TRASSA0[ukaz_trass].object&0xfff,0,0,ukaz_trass);
							}
							if(finish!=0)
							{ if(DISK!=0)return(0);
								return(2);
							}
						}
						if(tipus!=0)
						{
							for(ii=fin;ii<100;ii++)
							{
								TRASSA[ii].object=0xffff;
								TRASSA[ii].stoyka=0;
							}
						}
						else
						{
							for(ii=fin;ii<100;ii++)
							{
								TRASSA0[ii].object=0xffff;
								TRASSA0[ii].stoyka=0;
							}
						}
						return(finish);//если тест пройден можно  задавать
				}
				else
				{
					test_tmp=1;
					return(1); //иначе нельзя
				}
			}
			else//если конец промежуточный
			{ ukaz_trass--;
				if(tipus!=0)
				{
					END=nach0_marsh;//установить основной конец
					half_marsh=0;//очистить для дальнейшего движения по главному направлению
					NE=fra[END][0];//запомнить блок-линию основного конца
				}
				else
				{
					END_OPER=nach0_marsh_oper;//установить основной конец
					half_marsh=0;//очистить для дальнейшего движения по главному направлению
					NE=fra[END_OPER][0];//запомнить блок-линию основного конца
				}
			}
		}
		if((fr1[ako][0]==7)||   //если попали на ДЗ или
		((fr1[ako][0]>1)&&(fr1[ako][0]<4)))//на сигнал или СП
		{
			if(tipus!=0)
			{
				TRASSA[ukaz_trass].object=ako;//зафиксировать в трассе и пройти далее
				TRASSA[ukaz_trass].tip=fr1[ako][0];
			}
			else
			{
				TRASSA0[ukaz_trass].object=ako;//зафиксировать в трассе и пройти далее
				TRASSA0[ukaz_trass].tip=fr1[ako][0];
			}

			if((fr1[ako][0]==2)&&(fr1[ako][1]<2))
			{
				n_mane=2;
				if(tipus==0)n_mane=mane;
				if(tipus==1)n_mane=mane1;
				if(nap_dvi==fr1[ako][1])//если сигнал данного направления
				switch(n_mane)
				{
					case 0: //если поездной четный
									if((fr1[ako][6]==1)||(fr1[ako][6]==2))
									{
										if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object|0x8000;
										else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object|0x8000;
									}
									break;
					case 1: if((fr1[ako][6]==0)||(fr1[ako][6]==2))
									{
										if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object|0x8000;
										else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object|0x8000;
									}
									break;
					default: break;
				}
				if((fr1[ako][11]&kod_end)==kod_end)
				{
					if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object|0x4000;
					else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object|0x4000;
				}
			}
			if(tipus!=0)TRASSA[ukaz_trass++].stoyka=fr1[ako][13];
			else TRASSA0[ukaz_trass++].stoyka=fr1[ako][13];
			pred=ako;
			if(nap_dvi==0) ako--;
			else ako++;
			if(ako<0)
			{
				test_tmp=3;
				return(1);
			}
		}
		else
		if(fr1[ako][0]==4)//если попали на УП
		{
			if(nap_dvi==1&&fr1[ako][4]==1)goto revers;//вернутьcя к послед. стрелке
			//если направление четное и есть граница станции в четном направлении

			if(nap_dvi==0&&fr1[ako][3]==1)goto revers;//вернуться к послед. стрелке
			if(tipus!=0)
			{
				TRASSA[ukaz_trass].object=ako;//зафиксировать в трассе и пройти далее
				TRASSA[ukaz_trass].tip=fr1[ako][0];
				TRASSA[ukaz_trass++].stoyka=fr1[ako][13];
			}
			else
			{
				TRASSA0[ukaz_trass].object=ako;//зафиксировать в трассе и пройти далее
				TRASSA0[ukaz_trass].tip=fr1[ako][0];
				TRASSA0[ukaz_trass++].stoyka=fr1[ako][13];
			}
			pred=ako;

			if(nap_dvi==0) ako--;
			else ako++;

			if(ako<0)
			{
				test_tmp=4;
				return(1);
			}
		}
		else
		if(fr1[ako][0]==5)//если попали на путь
		{
			if(tipus!=0)
			{
				TRASSA[ukaz_trass].object=ako;//взять следующий элемент в трассу
				TRASSA[ukaz_trass].tip=fr1[ako][0];
				TRASSA[ukaz_trass++].stoyka=fr1[ako][13];
			}
			else
			{
				TRASSA0[ukaz_trass].object=ako;//взять следующий элемент в трассу
				TRASSA0[ukaz_trass].tip=fr1[ako][0];
				TRASSA0[ukaz_trass++].stoyka=fr1[ako][13];
			}
			//если участок приближения
			if(fr1[ako][7]!=9999)goto revers;

			//если напр.нечетное и есть граница станции в нечетном направлении
			if(nap_dvi==1&&fr1[ako][1]==1)goto revers;//вернутьcя к послед. стрелке
			//если направление четное и есть граница станции в четном направлении
			if(nap_dvi==0&&fr1[ako][2]==1)goto revers;//вернуться к послед. стрелке
			if(fra[ako][0]==NE)//если находимся на блок-линии конца
			{
				pred=ako;
				if(nap_dvi==0) ako--;
				else ako++;

				if(ako==0)
				{
					test_tmp=5;
					return(1);
				}
				if(fra[ako][0]!=fra[pred][0])
				{
					test_tmp=6;
					return(1);
				}
			}
			else//если не находимся на блок-линии конца
			{
				if(uprav==1)//если управление маршрутное и...
				//начало и конец в разных ТУМС или один ТУМС
				if(((tipus!=0)&&(fr1[nach_marsh][13]!=fr1[end_marsh][13]))||
				((tipus==0)&&(fr1[nach_marsh_oper][13]!=fr1[end_marsh_oper][13]))||
				(skoko_stoek==1))
				{
					pred=ako;
					if(nap_dvi==0) ako--;//сделать шаг вперед
					else ako++;

					if(ako==0)
					{
						test_tmp=7;
						return(1);
					}

					if(fra[ako][0]!=fra[pred][0])
					{
						test_tmp=8;
						return(1);
					}
				}
				else //иначе возврат назад до стрелки
				{
revers:   if(tipus!=0)UK_TRASS=TRASSA[ukaz_trass-1].object&0xfff;
					else UK_TRASS=TRASSA0[ukaz_trass-1].object&0xfff;
					//найти пройденный элемент-стрелку
					while((fr1[UK_TRASS][0]!=1)||
					(fr3[UK_TRASS][6]!=1)||
					(nap_dvi!=fr1[UK_TRASS][7]))//с входом по маршруту
					{
						//если по стрелке было 2 прохода, то сброс счета проходов
						if(fr3[UK_TRASS][6]==2)fr3[UK_TRASS][6]=0;
						if(tipus!=0)
						{
							TRASSA[ukaz_trass].object=0xFFFF;//исключить из трассы
							TRASSA[ukaz_trass].stoyka=0;
							TRASSA[ukaz_trass].tip=0;
							TRASSA[ukaz_trass].podgrup=0;
							TRASSA[ukaz_trass].kod_bit=0;
						}
						else
						{
							TRASSA0[ukaz_trass].object=0xFFFF;//исключить из трассы
							TRASSA0[ukaz_trass].stoyka=0;
							TRASSA0[ukaz_trass].tip=0;
							TRASSA0[ukaz_trass].podgrup=0;
							TRASSA0[ukaz_trass].kod_bit=0;
						}
						ukaz_trass--;//вернуться на предыдущий

						if(ukaz_trass==0)
						{
							test_tmp=9;
							return(1);
						}

						if(tipus!=0)
						{
							UK_TRASS=TRASSA[ukaz_trass].object&0xfff;
							//если вышли в начало - нет маршрута
							if(UK_TRASS==nach_marsh)
							{
								test_tmp=10;
								return(1);
							}
						}
						else
						{
							UK_TRASS=TRASSA0[ukaz_trass].object&0xfff;
							//если вышли в начало - нет маршрута
							if(UK_TRASS==nach_marsh_oper)
							{
								test_tmp=11;
								return(1);
							}
						}
					}
					//запомнить объект, на котором остановились
					if(tipus!=0)ako=TRASSA[ukaz_trass].object&0xfff;
					else ako=TRASSA0[ukaz_trass].object&0xfff;
					if(ako==0xfff)
					{
						ukaz_trass--;
						if(tipus!=0)ako=TRASSA[ukaz_trass].object&0xfff;
						else ako=TRASSA0[ukaz_trass].object&0xfff;
					}
				}
			}
		}
		else//если попали не на путь
		{
			r=nap_dvi;
			if(tipus!=0)
			{
				TRASSA[ukaz_trass].object=ako;//взять следующий элемент в трассу
				TRASSA[ukaz_trass].tip=fr1[ako][0];
				TRASSA[ukaz_trass++].stoyka=fr1[ako][13];
			}
			else
			{
				TRASSA0[ukaz_trass].object=ako;//взять следующий элемент в трассу
				TRASSA0[ukaz_trass].tip=fr1[ako][0];
				TRASSA0[ukaz_trass++].stoyka=fr1[ako][13];
			}
			switch(fr1[ako][0])
			{         //если стрелка - пройти через нее
				case 0: if(nap_dvi==0) ako--;//пройти на следующий по направлению
								else ako++;
								break;

				case 1: ako=strelka_trass(ako,nap_dvi,tipus);
								break;
							 //если переход и на него вышли по строке
				case 6: if(tipus!=0)
								{
									if((fr1[TRASSA[ukaz_trass-2].object][0]!=6)&&
									(fra[TRASSA[ukaz_trass-2].object][0]==fra[ako][0]))//если вышли по строке
									ako=fr1[ako][1];//следующий элемент по переходу
									else //если вышли со стрелки или перехода (с другой строки)
									{ pred=ako;
										if(nap_dvi==0) ako--;//пройти на следующий по направлению
										else ako++;
										if(ako==0)
										{
											test_tmp=12;
											return(1);
										}
										if(fra[ako][0]!=fra[pred][0])
										{
											test_tmp=13;
											return(1);
										}
									}
								}
								else
								{
									if((fr1[TRASSA0[ukaz_trass-2].object][0]!=6)&&
									(fra[TRASSA0[ukaz_trass-2].object][0]==fra[ako][0]))//если вышли по строке
									ako=fr1[ako][1];//следующий элемент по переходу
									else //если вышли со стрелки или перехода (с другой строки)
									{ pred=ako;
										if(nap_dvi==0) ako--;//пройти на следующий по направлению
										else ako++;
										if(ako==0)
										{
											test_tmp=14;
											return(1);
										}
										if(fra[ako][0]!=fra[pred][0])
										{
											test_tmp=15;
											return(1);
										}
									}
								}
								break;

				default:
					{
						test_tmp=16;
						return(1); //если нет такого элемента - маршрут невозможен
					}
			}
			nap_dvi=r;
		}

		//если вышли за пределы базы или длина трассы слишком велика
		if((ako<0)||(ako>=kol_VO)||(ukaz_trass>=100))
		{
			if(tipus!=0)
			{
				for(ukaz_trass=ukaz_trass;ukaz_trass>=0;ukaz_trass--)
				{
					TRASSA[ukaz_trass].object=0;
					TRASSA[ukaz_trass].stoyka=0;
				}
			}
			else
			{
				for(ukaz_trass=ukaz_trass;ukaz_trass>=0;ukaz_trass--)
				{
					TRASSA0[ukaz_trass].object=0;
					TRASSA0[ukaz_trass].stoyka=0;
				}
			}
			test_tmp=17;
			return(1);//маршрут невозможен
		}
	}
}
//--------------------------------------------------
//процедура набора цепочки элементов при раздельном открытии сигнала
void otkrsi()
{
	int napa,Snext,moni,Spred,konec_trass;

	nom_func("209");
	konec_trass=0;
	napa=fr1[point][1];//направление маршрута
	if((manevro==1)||(mane==1)) moni=1;//признак того,что хотим открыть маневровый
	else moni=0;
	ZA_SI=0;
	Spred=point;//предыдущий и следующий элемент устанавливаем вместе
	Snext=point; //на текущую точку=выбранный объект
	TRASSA0[ukaz_trass++].object=Snext;//запомнить текущую точку
	if(napa==0) Snext--;//если направление четное, идем по базе назад
	else Snext++;//иначе вперед
	while(1)//бесконечный цикл
	{
		TRASSA0[ukaz_trass].object=Snext;//запоминаем следующий эл-т
		switch (fr1[Snext][0])//переключатель по типу объекта
		{
			case 5: if((mane==1)||(manevro==1)||(fr1[Snext][7]!=9999))return;
							else
							{
								if((fr1[Snext][1]==1)&&(fr1[Snext][2]==1))return;
								if((fr1[Snext][1]==1)&&(napa==1))return;
								if((fr1[Snext][2]==1)&&(napa==0))return;
								Spred=Snext;//совмещаем указатели пред./след.
								if(napa==0) Snext--;//если направление четное-идем назад
								else Snext++;//для нечетного вперед
								break;
							}
			case 0: //если резерв или ДЗ
			case 7: Spred=Snext;// если ДЗ, то совмещаем указатели пред./след.
							if(napa==0) Snext--;//если направление четное-идем назад
							else Snext++;//для нечетного вперед
							break;
			case 3: //if(ZA_SI==1)//если СП и был проход за сигнал
							//если открываем поездной и нет стрелки в пути
							//if((moni==0)&&(s_v_p==0)) return;//то выйти
							//если есть признак корыта
							Spred=Snext;//иначе перейти на следующий
							if(napa==0) Snext--;
							else Snext++;
							 break;
			case 4: if((fr1[Snext][3]==1)&&(fr1[Snext][4]==1))return;
							//если направление маршрута четное и УП на четной границе
							if((napa==0)&&(fr1[Snext][3]==1))return;
							if((napa!=0)&&(fr1[Snext][4]==1))return;
							Spred=Snext;//иначе перейти к следующему
							if(napa==0) Snext--;
							else Snext++;
							break;
strelo:
			case 1: if(fr3[Snext][0]==fr3[Snext][1]) return;//если стрелка без контроля-выйти
							//если пред. не стрелка и не переход
							if((fr1[Spred][0]!=1)&&(fr1[Spred][0]!=6))
							{
								Spred=Snext;
								// если вход на стрелку не соответствует направлению
								if(fr1[Snext][7]!=fr1[point][1])
								{
									if(napa==0) Snext--;//пройти на следующий элемент
									else Snext++;
								}
								//если вход на стрелку по направлению движения
								else
								{
									if(fr1[Snext][1]==1)//если по стрелке переход по плюсу прямой
									{
										if((fr3[Snext][0]==1)&&(fr3[Snext][1]==0))//стрелка в плюсе

											if(napa==0) Snext--;//перейти на следующий объект
											else Snext++;
										else //если стрелка в минусе
										{
											FLAG_PO=1;//установить флаг поворота
											TRASSA0[ukaz_trass++].object=Snext;
											Snext=poisk_ras(fr1[Snext][2],Snext,0,ukaz_trass);//найти объект за ответвлением
											FLAG_PO=0;//сбросить флаг поворота
										}
									}
									else //переход по отклонению прямой
									{
										if((fr3[Snext][0]==0)&&(fr3[Snext][1]==1))//стрелка в минусе
											if(napa==0) Snext--;//перейти на следующий объект
											else Snext++;
										else //если стрелка в плюсе
										{
											FLAG_PO=1;//установить флаг поворота
											TRASSA0[ukaz_trass++].object=Snext;
											Snext=poisk_ras(fr1[Snext][2],Snext,0,ukaz_trass);//найти объект за поворотом
											FLAG_PO=0;//сбросить флаг поворота
										}
									}
								}
							}
							else//если предшествует стрелка или переход
							{
								Spred=Snext;
								if(fr1[Snext][7]!=fr1[point][1])
								//если вход на стрелку не по направлению
								{
									if(napa==0) Snext--;//пройти на следующий объект
									else Snext++;
								}
								else//если вход на стрелку по направлению
								{
									if(fr1[Snext][1]==1)//если переход по плюсу прямой
									{
										if((fr3[Snext][0]==1)&&(fr3[Snext][1]==0))//если стрелка в плюсе

											if(napa==0) Snext--; //перейти на следующий объект
											else Snext++;
										else //если стрелка в минусе
										{
											FLAG_PO=1;//установить флаг поворота
											TRASSA0[ukaz_trass++].object=Snext;
											Snext=poisk_ras(fr1[Snext][2],Snext,0,ukaz_trass);//найти объект за поворотом
											FLAG_PO=0;//снять флаг поворота
										}
									}
									else//если прямой переход по минусу
									{
										if((fr3[Snext][0]==0)&&(fr3[Snext][1]==1))//если стрелка в минусе
											if(napa==0) Snext--;//пройти на следующий объект
											else Snext++;
										else//если стрелка в плюсе
										{
											FLAG_PO=1;//установить флаг поворота
											TRASSA0[ukaz_trass++].object=Snext;
											Snext=poisk_ras(fr1[Snext][2],Snext,0,ukaz_trass);//найти объект за поворотом
											FLAG_PO=0;//снять флаг поворота
										}
									}
								}
							}
							break;
preho:
			case 6: //pusya=ukaz_trass; //если попали на переход,запомнить текущий адрес КК
							if(uze(Snext,napa)==0)// если нет дублирования перехода
							{

								Spred=Snext;
								FLAG_PO=1;//установить флаг поворота
								Snext=poisk_ras(fr1[Snext][1],Snext,0,ukaz_trass);//найти объект за поворотом
								FLAG_PO=0;//снять флаг поворота
							}
							else //если переход уже зафиксирован
							{
								Spred=Snext;//выполнить переход на следующий объект
								if(napa==0) Snext--;
								else Snext++;
							}
							//ukaz_trass=pusya;//восстановить адрес КК
							//pusya=0;
							break;
			case 2:	if(Snext!=point)//если не сигнал начала
							{
								//если то же направление
								if(fr1[point][1]==fr1[Snext][1])
								{
									if(mane==1)//если маневровый маршрут
									{
										if(fr1[point][1]==0)//если четный маршрут
										//если есть конец четного маневрового
										if((fr1[Snext][11]&4)==4)
										{
											konec_trass=1;
											break;
										}

										if(fr1[point][1]==1)//если нечетный маршрут
										//если есть конец нечетного маневрового
										if((fr1[Snext][11]&1)==1)
										{
											konec_trass=1;
											break;
										}
									}
									else //если поездной
									{
										if(fr1[point][1]==0)//если четный маршрут
										//если есть конец четного поездного
										if((fr1[Snext][11]&64)==64)
										{
											konec_trass=1;
											break;
										}

										if(fr1[point][1]==1)//если нечетный маршрут
										//если есть конец нечетного поездного
										if((fr1[Snext][11]&16)==16)
										{
											konec_trass=1;
											break;
										}

									}
								}
							}
							Spred=Snext;//если попали на сигнал
							if(fr1[Snext][2]==5) goto ss;//для смены направления - переход
							if((moni==0)&&  //если открываем поездной и
							(fr1[Snext][2]==0))// попался маневровый
							{
ss:             if(napa==0) Snext--;//перейти на следующий объект
								else Snext++;
								break;
							}
							else
							if((moni==1)&& //если открываем маневровый и
							(fr1[Snext][2]==1)&&  //встретился поездной сигнал и
							(fr1[Snext][6]!=2))//сигнал не совмещенный
							{
								if(napa==0) Snext--;//идем дальше
								else Snext++;
								break;
							}
							if(fr1[point][1]==fr1[Snext][1])//если сигналы в одном напр.
							return;//выход
							else//если сигналы встречные
							{
								if(napa==0) Snext--;//идем дальше
								else Snext++;
								ZA_SI=1;//устанавливаем признак прохода за сигнал
								break;
							}
			default:  return;//если что-то непредвиденное
		}
		if(konec_trass!=0)break;
		if(TRASSA0[ukaz_trass].object!=0xFFFF)ukaz_trass++;
		if(ukaz_trass>=100)break;
	} /* while */
}
//-----------------------------------------------------------------------
/**********************************************************\
* Процедура анализа и запоминания требуемого положения     *
* стрелки при трассировке маршрута. Запоминание требуемого *
* положения стрелки происходит в fr3[ob][10]               *
\**********************************************************/
void plus_minus(int tipus)
{
	int obekt_pred,obekt_str,obekt_sled;

	nom_func("225");
	if(tipus!=0)
	{
		obekt_pred=TRASSA[ukaz_trass-1].object&0xFFF;
		obekt_str=TRASSA[ukaz_trass].object&0xFFF;
		obekt_sled=TRASSA[ukaz_trass+1].object&0xFFF;
	}
	else
	{
		obekt_pred=TRASSA0[ukaz_trass-1].object&0xFFF;
		obekt_str=TRASSA0[ukaz_trass].object&0xFFF;
		obekt_sled=TRASSA0[ukaz_trass+1].object&0xFFF;
	}
	// если  по прямой и для стрелки проход по нити плюсом
	if((fra[obekt_pred][0]==fra[obekt_sled][0])&&(fr1[obekt_str][1]==1))
	{
		if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object&0x7fff;
		else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object&0x7fff;
		fr3[obekt_str][10]=	fr3[obekt_str][10]&0x7fff;
	}
	else
	// если по прямой и проход для стрелки по нити минусом
	if((fra[obekt_pred][0]==fra[obekt_sled][0])&&(fr1[obekt_str][1]==0))
	{
		if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object|0x8000;
		else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object|0x8000;
		fr3[obekt_str][10]=	fr3[obekt_str][10]|0x8000;
	}
	else
	//если проход по отклонению и для стрелки проход по нити плюсом
	if((fra[obekt_pred][0]!=fra[obekt_sled][0])&&(fr1[obekt_str][1]==1))
	{
		if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object|0x8000;
		else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object|0x8000;
		fr3[obekt_str][10]=fr3[obekt_str][10]|0x8000;
	}
	else
	if((fra[obekt_pred][0]!=fra[obekt_sled][0])&&(fr1[obekt_str][1]==0))
	{
		// если для стрелки проход по нити плюсом
		if(tipus!=0)TRASSA[ukaz_trass].object=TRASSA[ukaz_trass].object&0x7fff;
		else TRASSA0[ukaz_trass].object=TRASSA0[ukaz_trass].object&0x7fff;
		fr3[obekt_str][10]=fr3[obekt_str][10]&0x7FFF;
	}
	fr3[obekt_str][6]=0;
	return;
}
//--------------------------------------------------------------
/*************************************************\
* Процедура выдачи опреатору текста запроса       *
* подтверждения команды при ненормах в работе СЦБ.*
\*************************************************/
void podtver()
{
	nom_func("228");
	setfillstyle(1,14);bar(495,30,639,43);
	moveto(498,33);setcolor(RED);outtext("Выдавать команду?");
	setlinestyle(0,0,0);rectangle(494,31,638,42);
	return;
}
//----------------------------------------------------------------------
/**********************************************\
* Процедура проверки правильности выбора конца *
* задаваемого маршрута.                        *
\**********************************************/
int prov_avail(tipus)
{
	unsigned int i,n_m,e_m,n0_m,E_N_D;
	int n_mane,n_manevro;
	if(tipus==0){n_mane=mane;n_manevro=manevro;}
	if(tipus==1){n_mane=mane1;n_manevro=manevro1;}

	nom_func("242");
	//nach_marsh - начало маршрута для простого маршрута или вариантная точка
	//end_marsh - сигнал конца маршрута
	//nach0_marsh - 0 для простого маршрута или начало для вариантного
	if(tipus!=0)
	{
		n_m=nach_marsh;e_m=end_marsh;n0_m=nach0_marsh;E_N_D=END;
	}
	else
	{
		n_m=nach_marsh_oper;e_m=end_marsh_oper;n0_m=nach0_marsh_oper;E_N_D=END_OPER;
	}
	if(fra[n_m][0]==fra[E_N_D][0])//если начало и конец в одной строке
	{ if((fr1[n_m][1]==1)&&(fra[n_m][1]>fra[E_N_D][1]))return(1);
		if((fr1[n_m][1]==0)&&(fra[n_m][1]<fra[E_N_D][1]))return(1);
	}
	if(n0_m!=0)
	if(fra[n0_m][0]==fra[E_N_D][0])//если начало и конец в одной строке
	{ if((fr1[n_m][1]==1)&&(fra[n0_m][1]>fra[E_N_D][1]))return(1);
		if((fr1[n_m][1]==0)&&(fra[n0_m][1]<fra[E_N_D][1]))return(1);
	}
	i=fr1[e_m][11];
	if(n0_m!=0)//если вариантный маршрут
	{ if((n_mane==1)||(fr1[n0_m][6]==3))//если маневровый или 2 белых
		{
			if(fr1[n_m][1]==0)//если четный маршрут
			{ //если есть конец четного маневрового
				if(((i&4)!=4)||((i&8)!=0x0))return(1);
				else return(0);
			}
			if(fr1[n_m][1]==1)//если нечетный маршрут
			{ //если есть конец нечетного маневрового
				if(((i&1)!=1)||((i&2)!=0x0))return(1);
				else return(0);
			}
		}
		if((n_mane==0)&&(fr1[n0_m][6]!=3))//если поездной
		{ if(fr1[n_m][1]==0)//если четный маршрут
			{ //если есть конец четного поездного
				if(((i&64)!=64)||((i&128)!=0x0))return(1);
				else return(0);
			}
			if(fr1[n_m][1]==1)//если нечетный маршрут
			{ //если есть конец нечетного поездного
				if(((i&16)!=16)||((i&32)!=0x0))return(1);
				else return(0);
			}
		}
	}
	if(n0_m==0)//если простой маршрут
	{ if((n_mane==1)||(fr1[n_m][6]==3))//если маневровый сигнал начала или 2 белых
		{ if(fr1[n_m][1]==0x0)//если четный маршрут
			{ //если есть конец четного маневрового
				if(((i&4)!=4)||((i&8)!=0))return(1);
				else return(0);
			}
			if(fr1[n_m][1]==1)//если нечетный маршрут
			{ //если есть конец нечетного маневрового
				if(((i&1)!=1)||((i&2)!=0x0))return(1);
				else return(0);
			}
		}
		if((n_mane==0)&&(fr1[n_m][6]!=3))//если поездной
		{ if(fr1[n_m][1]==0)//если четный маршрут
			{ //если есть конец четного поездного
				if(((i&64)!=64)||((i&128)!=0x0))return(1);
				else return(0);
			}
			if(fr1[n_m][1]==1)//если нечетный маршрут
			{ //если есть конец нечетного поездного
				if(((i&16)!=16)||((i&32)!=0x0))return(1);
				else return(0);
			}
		}
		if((n_mane==0)&&(n_manevro==1))//если поездной сигнал начала и маневровый маршрут
		{ if(fr1[n_m][1]==0)//если четный маршрут
			{ //если есть конец четного маневрового
				if(((i&4)!=4)||((i&8)!=0x0))return(1);
				else return(0);
			}
			if(fr1[n_m][1]==1)//если нечетный маршрут
			{ //если есть конец нечетного маневрового
				if(((i&1)!=1)||((i&2)!=0x0))return(1);
				else return(0);
			}
		}
	}
	return(1);
}
//--------------------------------------------------------------------
/************************************************\
* Процедура проверки занятости элемента          *
* дополнительной  зависимости по негабаритности. *
\************************************************/
int prov_negabarit(int nom,int avt)
{//проверка состояния негабаритного участка
	int nnom=0;

	nom_func("247");
	nnom=fr1[nom][2];
  if(fr3[nnom][0]==1)
  {
		if(avt==0)w(130,nnom,"");// ЗАНЯТ НЕГАБАРИТНЫЙ УЧАСТОК
		return(1);
	}
	else return(0);
}
//--------------------------------------------------------------------
/******************************************************\
* Процедура проверки занятости негабаритного участка с *
* учетом положения охранных стрелок.                   *
\******************************************************/
int prov_negabarit_ohr_pl_mi(int nom,int avt,int tipus,int uk_tras)
{
	int nnom=0,nnom1=0,pn1=0,pn2=0,sparen=0,stoika,k,n_mane;

	nom_func("248");
	if(tipus==0)n_mane=mane;
	if(tipus==1)n_mane=mane1;
	stoika=fr1[nach_marsh][13];//получить номер стойки
	nnom=fr1[nom][2]; //получить номер для СП
	nnom1=fr1[nom][3];//получить номер для стрелки
	pn1=fr1[nom][4]; //требуемое положение для плюсового контроля
	pn2=fr1[nom][5]; //требуемое положение для минусового контроля
ag:
	if(((fr3[nnom1][0]!=pn1)||(fr3[nnom1][1]!=pn2))// если стрелка стоит не так
	&&((n_mane==0)||(fr1[nom][1]==14)))//если в любых маршрутах
	{ //если раздельное управление,то выдать текст "НЕ ПО МАРШРУТУ СТРЕЛКА"
		if(uprav==0){for(k=0;k<10;k++)BEDA[k]=0;w(126,nnom1,"");return(1);}
		//если маршрутное управление и стрелка без контроля,то
		//выдать текст "НЕ ИМЕЕТ КОНТРОЛЯ СТРЕЛКА"
		if((uprav==1)&&(fr3[nnom1][0]==fr3[nnom1][1])){w(127,nnom1,"");return(1);}
	 //если отключена от управления,то выдать текст "ОТКЛ. ОТ УПРАВЛ. СТРЕЛКА"
		if(fr4[nnom1][0]==1){if(avt==0)w(67,nnom1,"");return(1);}
		if(fr4[nnom1][1]==1){if(avt==0)w(99,nnom1," ВКЛЮЧЕНА НА МАКЕТ"); return(1);}
		// ЗАНЯТ УЧАСТОК
		if(fr3[nnom][0]==1){if(avt==0)w(94,nnom,"");return(1);}
		// ЗАМКНУТ В МАРШРУТЕ УЧАСТОК
		if(fr3[nnom][1]==1){if(avt==0)w(96,nnom,"");return(1);}
		// ВКЛЮЧЕНО ИСКУССТВЕННОЕ РАЗМЫКАНИЕ
		if(fr3[nnom][2]==1){if(avt==0)w(98,nnom,"");return(1);}
		// НЕИСПРАВЕН УЧАСТО
    if(fr3[nnom][5]==1)
		{if(avt==0)w(104,nnom,"");
		return(-1);}
		if(tst_chet(nnom)==0)//если стрелка четная
		{
      if(fr3[RZ_CHET][RZ_CHET_BIT]==1)//если включено замыкание ручное четное
      {
        w(166,nnom1,"");
				return(1);
      }
		}
    else //если стрелка нечетная стойке
    {
      if(fr3[RZ_NECHET][RZ_NECHET_BIT]==1)//если замыкание ручное нечетное
      {
        w(166,nnom1,"");
				return(1);
      }
    }
    if(sparen==0)
		if(fr1[nnom1][13]!=fr1[nom][13])//если стрелка в другой стойке
		{ if(pn1==1)oper='I';//для перевода в плюс с реверсом
      if(pn1==0)oper='J';//аналогично для перевода в минус
      //если ранее не был запомнен маршрут
      if((marshrut[stoika-1][0]==0)||(marshrut[stoika-1][0]==nach_marsh))
      { marshrut[stoika-1][0]=nach_marsh;
        marshrut[stoika-1][1]=end_marsh;
        marshrut[stoika-1][2]=half_marsh;
        return(22);
			}
			//если не закончена работа с ранее заданным маршрутом
      //"ЕЩЕ НЕ ВЫПОЛНЕНА ПРЕДЫДУЩАЯ КОМАНДА"
      else {if(avt==0)w(51,999,"");return(1);}
    }
    if(sparen==0&&fr1[nnom1][12]!=9999)
    { sparen=1;
			TEST_SPAR=2;
			nnom1=poisk_ras(fr1[nnom1][2],nnom1,tipus,0);
			nnom=findspu1(nnom1,fr1[nnom1][7]);
			if(nnom==-1)return(0);
      goto ag;
    }
  }
  else//если стрелка по маршруту
  {
		if((n_mane==0)&&(fr1[nom][1]==15)&&(fr3[nnom1][5]==1))
		{
			w(72,nnom1,"");
			return(-1);
		}
		if((fr1[nom][1]==15)&&(fr1[nach_marsh][3]==7)&&(sparen==0))
    { // ЗАНЯТ УЧАСТОК
			if(fr3[nnom][0]==1){if(avt==0)w(94,nnom,"");return(1);}
      // ЗАМКНУТ В МАРШРУТЕ УЧАСТОК
      if(fr3[nnom][1]==1){if(avt==0)w(96,nnom,"");return(1);}
			// ВКЛЮЧЕНО ИСКУССТВЕННОЕ РАЗМЫКАНИЕ
      if(fr3[nnom][2]==1){if(avt==0)w(98,nnom,"");return(1);}
      // НЕИСПРАВЕН УЧАСТОК
      if(fr3[nnom][5]==1){if(avt==0)w(104,nnom,"");return(-1);}
    }
		if((sparen==0)&&(fr1[nnom1][12]!=9999))
		{ sparen=1;
			TEST_SPAR=2;
			nnom1=poisk_ras(fr1[nnom1][2],nnom1,tipus,uk_tras);
			if((fr1[nnom1][11]&256)!=0)return(0);
			nnom=findspu1(nnom1,fr1[nnom1][7]);
			if(nnom==-1)return(0);
			goto ag;
		}
	}
	//если поездной маршрут и д.з. только для поездных
	if(n_mane==0)
	{ if(nach_marsh!=0)
		if((fr1[nach_marsh][3]==7)||(fr1[nach0_marsh][3]==7))//если прием
		{ if((uprav==1)&&(fr1[nnom1][13]!=fr1[end_marsh][13]))//и стрелка в другой стойке
			{ if(sparen==0)
				if((fr3[nnom1][0]!=pn1)&&(fr3[nnom1][1]!=pn2))
				{ point=nnom1;
					if(pn1==1)oper='I';
					if(pn1==0)oper='J';
					if(marshrut[stoika][0]==0)
					{ marshrut[stoika-1][0]=nach_marsh;
						marshrut[stoika-1][1]=end_marsh;
						marshrut[stoika-1][2]=half_marsh;
					}
					return(22);
				}
			}
		}
	}
	return(0);
}
//-------------------------------------------------------------------
int prov_negabarit_pl_mi(int nom,int avt)
{
  int nnom=0,nnom1=0,pn1=0,pn2=0;

	nom_func("249");
	nnom=fr1[nom][2];//СП для охранной стрелки
  nnom1=fr1[nom][3];//охранная стрелка
	pn1=fr1[nom][4];//треб. состояние плюсового контроля для снятия контроля
	pn2=fr1[nom][5];//треб. состояние минусового контроля для снятия контроля

	if((fr3[nnom1][0]!=pn1&&fr3[nnom1][1]!=pn2)||  //если положение стрелки не то
	(fr3[nnom1][0]==fr3[nnom1][1]))//или стрелка без контроля
		if(fr3[nnom][0]==1)//если СП занято
		{
			if(avt==0)w(130,nnom,"");//ЗАНЯТ НЕГАБАРИТНЫЙ УЧАСТОК
			return(1);
		}
		else
/*
			if(fr3[nnom][1]==1) //если СП замкнуто
			{
				if(avt==0)w(96,nnom,"");//ЗАМКНУТ В МАРШРУТЕ УЧАСТОК
				return(1);
			}
			else
				if(fr3[nnom][2]==1)//если СП в разделке
				{
					if(avt==0)w(98,nnom,""); //ВКЛЮЧЕНО ИСКУССТВЕННОЕ РАЗМЫКАНИЕ
					return(1);
				}
				else
*/
					if(fr3[nnom][5]==1)//если по СП непарафазность
					{
						if(avt==0)w(104,nnom,"");//НЕИСПРАВЕН УЧАСТОК
						return(1);
					}
					else return(0);
	return(0);
}
//----------------------------------------------------------
int prov_negabarit_pl_mi2(int nom,int av)
{
	int nnom=0,nnom1=0,pn1=0,pn2=0;

	nom_func("250");
	nnom=fr1[nom][2];//получить номер СП для проверки
  nnom1=fr1[nom][3];//получить номер стрелки для проверки
  pn1=fr1[nom][4];//требуемое положение по плюсу
  pn2=fr1[nom][5];//требуемое положение по минусу
  if((fr3[nnom1][0]!=pn1&&//если плюсовой контроль не такой и
  fr3[nnom1][1]!=pn2)||  //минусовой контроль не такой или
  (fr3[nnom1][0]==fr3[nnom1][1]))//стрелка без контроля
  //если СП занят
  if(fr3[nnom][0]==1){if(av==0)w(130,nnom,"");return(1);}
  return(0);
}
//-----------------------------------------------------------
#ifdef OGRAD
int prov_ograd(int nom,int avto)//проверяет наличие ограждения пути
{
  int nnom=0,bt_=0,put=0;
	nnom=fr1[nom][2];//датчик запроса ограждения
  bt_=fr1[nom][3];
  put=fr1[nnom][8];//датчик ограждения
  if(fr3[nnom][bt_]==1)
  {
    if(avto==0)w(141,put,"");//ВКЛЮЧЕНО ОГРАЖДЕНИЕ, ПУТЬ...
    return(1);
  }
  else return(0);
}
#endif
//------------------------------------------------
void sbros_time_tip(int tip)
{
	int i,ob1;
	nom_func("294");
	for(i=0;i<25;i++)
	{
		if(TIMER_N[i][1]==tip)
		{
			ob1=TIMER_N[i][0]=0;
			if(pooo[ob1]!=2)pooo[ob1]=0L;
			TIMER_N[i][0]=0;
			TIMER_N[i][1]=0;
			TIMER_N[i][2]=0;
		}
	}
	return;
}
//--------------------------------------------------
int sogl_otkr(int NUMB)
{
  int okop=0,BBA=0;

	nom_func("314");
	switch(fr1[NUMB][0])
	{
		case 1: return(tst_str1(NUMB));
		case 2: if(fr1[NUMB][2]==5) return(0);
						return(tst_sig(NUMB,0,1));
		case 3: return(tst_spu(NUMB,0,0,0));
		case 4: okop=tst_uch(NUMB,0,1);return(okop);
		case 5: BBA=tst_put(NUMB,0,0);return(BBA);
		case 0: //резерв или точка перехода
		case 6: return(0);
		case 7: return(tst_dz(NUMB,0,0,0));
    default:return(1);
  }
}
//-------------------------------------------------
int strelka_trass(int aks,int nd,int tipus)
{//процедура трассировки маршрута через стрелку
	int Zahod,PREOB,E_N_D,n_m;

	nom_func("321");
	if(tipus!=0){E_N_D=END;n_m=nach_marsh;}
	else {E_N_D=END_OPER;n_m=nach_marsh_oper;}
	Zahod=0;
	if(fr3[aks][6]==0)Zahod=1;
	if(fr3[aks][6]==1)Zahod=2;
	PREOB=fr1[aks][10]&96;
	if(fr1[aks][7]==fr1[n_m][1])//если вход на стрелку по напр.маршрута
	{ if(tipus!=0)
		{
			if(TRASSA[ukaz_trass-1].object!=0)
			TRASSA[ukaz_trass-1].object=TRASSA[ukaz_trass-1].object|0x4000;
		}
		else
		{
			if(TRASSA0[ukaz_trass-1].object!=0)
			TRASSA0[ukaz_trass-1].object=TRASSA0[ukaz_trass-1].object|0x4000;
		}
		if(Zahod==1)//если 1-ый заход на стрелку
		{ fr3[aks][6]=1;
			if((PREOB&32)==32&&(fra[aks][0]!=fra[E_N_D][0]))
			//если стрелка имеет преобладание по минусу
			{
metka_a:
				FLAG_PO=1;//установить флаг отклонения маршрута от прямой
				TEST_SPAR=0;
				aks=poisk_ras(fr1[aks][2],aks,tipus,ukaz_trass);//найти объект за поворотом
				FLAG_PO=0;//сбросить флаг поворота
				return(aks);//вернуть номер объекта
			}
			else
			{
metka_b:
				if(nd==0) aks--; //перейти на следующий
				else aks++;
				return(aks);
			}
		}
		if(Zahod==2)//если второй заход
		{ fr3[aks][6]=2;
			//если стрелка имеет преобладание по минусу
			if((PREOB&32)==32)goto metka_b;
			else
				//если стрелка имеет преобладание по плюсу
				if((PREOB&64)==64)goto metka_a;
				else goto metka_a;// стрелка не имеет преобладания
		}
		else return(-1);//при попытке третьего захода - стрелка непроходима
	}
	else goto metka_b;//если вход на стрелку не совпадает с напр.движения
}
//-----------------------------------------
int t_dz(int no) //проверка ДЗ при включении автодействия
{ int nnnn1,bit1;

	nom_func("326");
	if(fr1[no][1]==7)//если проверка направления для автоблокировки
	{ nnnn1=fr1[no][4];
		if(fr3[nnnn1][1]==0)//если нет направления на отправление
		{ //НЕ УСТАНОВЛЕНО НАПРАВЛЕНИЕ ДВИЖЕНИЯ НА ОТПРАВЛЕНИЕ
			w(230,999,"");
			return(1);
		}
		else
		{
			if(fr1[no][6]!=9999)//если есть РОН/РОЧ
			{
				nnnn1=fr1[no][6]&0xfff;
				bit1=(fr1[no][6]&0xf000)>>12;
				if(fr3[nnnn1][bit1]!=1)
				{
					w(264,nnnn1," ");
					return(1);
				}
			}
		}
		return(0);
  }
#ifdef POLUAVTOMAT
  if(fr1[no][1]==5)//если проверка для полуавтоматики
  { nnnn1=fr1[no][3];//первый объект направления 
    bit1=fr1[no][4];//бит первого объекта 
    znach1=fr1[no][5];//значение бита для отправления 
    nnnn2=fr1[no][6];//второй объект
    bit2=fr1[no][7];//бит второго объекта 
    znach2=fr1[no][8];//значение бита для отправления 
    if((fr3[nnnn1][bit1]!=znach1)||(fr3[nnnn2][bit2]!=znach2))
    { w(230,999,"");//НЕ УСТАНОВЛЕНО НАПРАВЛЕНИЕ ДВИЖЕНИЯ НА ОТПРАВЛЕНИЕ
      return(1);
    }
    else return(0);
  }
  if(fr1[no][1]==6)//для полуавтоматики
  { nnnn1=fr1[no][3];//номер объекта перегона по отправлению 
    bit1=fr1[no][4];// номер бита
    znach1=fr1[no][5];//значение бита для свободности 
    if(fr3[nnnn1][bit1]!=znach1)
    { w(77,999,""); //ЗАНЯТ ПЕРЕГОН 
      return(1);
    }
    else return(0);
  }
#endif
  return(0);
}
//-------------------------------------------------------
int t_put(int no)//проверка пути при установке автодействия
{
	nom_func("328");
	if(fr4[no][2]==1)//если закрыт для движения
  { w(115,no,"");//"ДВИЖЕНИЕ ЗАПРЕЩЕНО ПУТЬ..."
    return(1);
  }
  if(fr3[no][5]==1)//если непарафазность
  { w(116,no,"");//"НЕИСПРАВЕН ПУТЬ..."
    return(1);
  }
  if(fr3[no][0]==1)//если занят 
  { w(106,no,"");//"ЗАНЯТ ПУТЬ..."
    return(1);
  }
  if((fr1[no][1]==1)||(fr1[no][2]==1))return(0);//если нет границ станции 
  if((fr3[no][1]==1)&&( markery[modi][7]-777==0)) return(0);//если НИ для нечетного 
  if((fr3[no][2]==1)&&( markery[modi][7]-777==1))return(0);//если ЧИ для четного 
  else //иначе вывести на экран текст 
  { w(143,no,"");//"НЕ ГОТОВ МАРШРУТ - ПУТЬ..."
    return(1);
  }
}
//---------------------------------------------------------
int t_sig(int no) //проверка сигнала при включении автодействия
{
	nom_func("329");
/*
	uksp=fr2[no][9]&0xfff;
	if(uksp!=0)
	{
		bt_uks=(fr2[no][9]&0xf000)>>12;
		if(fr3[uksp][bt_uks]==1){w(169,999," УКСПС");return(1);}
	}
	uksp=fr2[no][10]&0xfff;
	if(uksp!=0)
	{
		bt_uks=(fr2[no][10]&0xf000)>>12;
		if(fr3[uksp][bt_uks]==1){w(169,999," УКСПС");return(1);}
	}
*/
  if(fr3[no][5]==1)//если непарафазность
  { w(88,no,"");return(1);} //"НЕИСПРАВЕН СИГНАЛ..."
  if(fr1[no][2]==0)//если маневровый
  { if(fr3[no][0]==1)//если сигнал открыт
    {w(78,no,"");return(1);} //"ОТКРЫТ СИГНАЛ..."
    else return(0);
  }
  else//если не маневровый
  { if(markery[modi][7]-777==0)//если включается нечетное автодействие
    { if((fr1[no][1]==0)&&( fr3[no][1]==1))//если сигнал четный и открыт поездной
      { w(78,no,""); return(1);} //"ОТКРЫТ СИГНАЛ..."
			else return(0);
		}
		if(markery[modi][7]-777==1)//если включается четное автодействие
		{ if((fr1[no][1]==1)&&( fr3[no][1]==1))//если сигнал нечетный и открыт поездной
			{ w(78,no,"");return(1);} //"ОТКРЫТ СИГНАЛ..."
			else return(0);
		}
	}
	return(0);
}
//-------------------------------------------------------------
int t_spu(int no)//процедура проверки СП при установке автодействия
{
	nom_func("330");
	if(fr3[no][5]==1)//если непарафазность
	{ w(104,no,""); //вывести на экран текст "НЕИСПРАВЕН УЧАСТОК..."
		return(1);
	}
	//если только замкнут
	if((fr3[no][0]==0)&&(fr3[no][1]==1)&&(fr3[no][2]==0))return(0);
	if(fr3[no][0]==1)//если занят
	{ w(94,no,"");//"ЗАНЯТ УЧАСТОК..."
		return(1);
	}
	if(fr3[no][2]==1)//если в разделке
	{ w(98,no,"");//"ВКЛЮЧЕНО ИСКУССТВЕННОЕ РАЗМЫКАНИЕ..."
		return(1);
	}
	if(fr3[no][1]==0)//если не замкнут
	{ w(142,no,"");//"НЕ УСТАНОВЛЕНО ЗАМЫКАНИЕ ПО УЧАСТКУ..."
		return(1);
	}
	return(0);
}
//-----------------------------------------------------------
int t_uch(int no)//проверка участка пути при установке автодействия
{
	nom_func("332");
	if(fr4[no][2]==1)//если участок закрыт для движения
  { w(115,no,""); //"ДВИЖЕНИЕ ЗАПРЕЩЕНО ПУТЬ..."
    return(1);
  }
  if(fr3[no][5]==1)//если непарафазность
  { w(104,no,"");//"НЕИСПРАВЕН УЧАСТОК..."
    return(1);
  }
  //если только замкнут   
  if((fr3[no][0]==0)&&( fr3[no][1]==1)&&( fr3[no][2]==0))return(0);
  if(fr3[no][0]==1)//если занят 
  { w(94,no,"");//"ЗАНЯТ УЧАСТОК..."
    return(1);
  }
  if(fr3[no][2]==1)//если в разделке
  { w(98,no,"");//"ВКЛЮЧЕНО ИСКУССТВЕННОЕ РАЗМЫКАНИЕ..."
    return(1);
  }
  if(fr3[no][1]==0)//если не замкнут 
  { w(142,no,"");//"НЕ УСТАНОВЛЕНО ЗАМЫКАНИЕ ПО УЧАСТКУ..."
    return(1);
  }
	return(0);
}
//----------------------------------------------------------
int test_elem(int element,int fl,int tipus,int uk_tras)
{ //переключатель программ проверок элементов маршрута
// при удачной проверке возвращает 0, при неудаче возвращает 1
//  fl1=0 для обычных проверок
//	fl1=1 для повторного прохода по предварительно замкнутому маршруту
//  fl1=10+kkp для проверок перед включением автодействия
	int otvet,pologen;
	nom_func("339");
	otvet=1; //изначально предполагаем неудачу

	switch(fr1[element][0])
	{ //проверка стрелок (при норме возвращает 0)
		case 0: otvet=0;goto konec;
		case 1: if(fl==0)
						{
							otvet=0;
							break;
						}
						if(tipus==1)
						{
							if((TRASSA[uk_tras].object&0x8000)==0)pologen=1;
							else pologen=-1;
						}
						if(tipus==0)
						{
							if((TRASSA0[uk_tras].object&0x8000)==0)pologen=1;
							else pologen=-1;
						}
						if((fl<10)&&(tst_str(element,pologen,fl,tipus)==0)){otvet=0;goto konec;}
						if((fl>=10)&&(tst_str_av(element,1)==0)){otvet=0;goto konec;}
						break;
		//проверка сигналов
		case 2: if(fr1[element][2]==5){otvet=0;goto konec;}//смену направлен.не проверять
						//проверка открытости сигнала и его исправности
						otvet=tst_sig(element,fl,tipus);
						goto konec;
		// проверка СП
		case 3: otvet=tst_spu(element,fl,0,tipus);
						goto konec;
		// проверка УП
		case 4: otvet=tst_uch(element,fl,tipus);
						goto konec;
		//проверка пути
		case 5: if(fr1[element][1]==1&&fr1[element][2]==1)return(0);
						otvet=tst_put(element,fl,tipus);
						goto konec;
		case 6: return(0);
		case 7: otvet=tst_dz(element,fl,tipus,uk_tras);
						goto konec;
		default:otvet=1;goto konec;
	}
konec:
			return(otvet);
}
//----------------------------------------------------
int tst_dz(int D,int avto,int tipus,int uk_tras)
{ int nnnn1,nnnn2,a,c,ron_roch,b_ron_roch,n_mane,n_manevr,n_manevro;
	unsigned int b;

	nom_func("362");
	/* если это ДЗ - ограждение пути */
#ifdef OGRAD
	if(fr1[D][1]==77)return(prov_ograd(D,avto));
#endif
	if(tipus==0){n_mane=mane;n_manevr=manevr;n_manevro=manevro;}
	if(tipus==1){n_mane=mane1;n_manevr=manevr1;n_manevro=manevro1;}
	if(avto>10)avto=avto-10;
	if(fr1[D][1]==112)
	{ if(prov_negabarit_pl_mi(D,avto)==0)return(0);
		else return(1);
	}
	if(fr1[D][1]==1)return(prov_negabarit(D,avto));
  if(fr1[D][1]==101)return(prov_negabarit_pl_mi(D,avto));
	if((fr1[D][1]==14)||(fr1[D][1]==15))
	return(prov_negabarit_ohr_pl_mi(D,avto,tipus,uk_tras));
  if(fr1[D][1]==112)return(prov_negabarit_pl_mi2(D,avto));
#ifdef POLUAVTOMAT
  if(fr1[D][1]==5)
  { nnnn1=fr1[D][3]; bit1=fr1[D][4]; znach1=fr1[D][5];
    nnnn2=fr1[D][6]; bit2=fr1[D][7]; znach2=fr1[D][8];
		if((n_mane==0)&&(n_manevro==0)&&(n_manevr==0)&&
    ((fr3[nnnn1][bit1]!=znach1)||(fr3[nnnn2][bit2]!=znach2)))
    { //НЕ УСТАНОВЛЕНО НАПРАВЛЕНИЕ ДВИЖЕНИЯ НА ОТПРАВЛЕНИЕ
      if(avto==0)w(230,999,"");
      return(1);
    }
    return(0);
  }
	if(fr1[D][1]==6)
  { nnnn1=fr1[D][3]; bit1=fr1[D][4]; znach1=fr1[D][5];
    nnnn2=fr1[D][6]; bit2=fr1[D][7]; znach2=fr1[D][8];
		if((n_mane==0)&&(n_manevro==0)&&(n_manevr==0)&&
    ((fr3[nnnn1][bit1]!=znach1)||(fr3[nnnn2][bit2]!=znach2)))
    { if(avto==0)w(77,999,""); // ЗАНЯТ ПЕРЕГОН
      return(1);
    }
    return(0);
  }
#endif
  if(fr1[D][1]==7)//если проверка направления для 2-х путной автоблокировки
  {
		if((n_mane==1)||(n_manevro==1)||(n_manevr==1))return(0);
    nnnn1=fr1[D][4];//получить объект для смены направления перегона
    nnnn2=fr1[D][5];//получить объект для реле В
    a=fr3[nnnn2][1];//состояние реле В
		b=fr3[nnnn2][7];//состояние направления перегона с учетом переключения
		c=fr3[nnnn1][1];//установленное направление
		if((b!=0xffff)&&((b==0xf0f0)||((a==1)&&(c==1))))
		{
			ron_roch=fr1[D][6];
			if(ron_roch!=9999)//если есть РОН/РОЧ
			{
				ron_roch=fr1[D][6]&0xfff;
				b_ron_roch=(fr1[D][6]&0xf000)>>12;
				if(fr3[ron_roch][b_ron_roch]==0)//если нет РОН/РОЧ
				{
					if((avto!=1)&&(avto!=2))//если нет автодействия
					{
						get_str(ron_roch,b_ron_roch);
						w(264,999,chudo);
						return(1);
					}
					else return(7);
				}
			}
			return(0);
		}
		else
		{
    //НЕ УСТАНОВЛЕНО НАПРАВЛЕНИЕ ДВИЖЕНИЯ НА ОТПРАВЛЕНИЕ
      if(avto==0)w(230,999,"");
      return(1);
    }

  }
  if(fr1[D][1]==6)//если проверка направления для однопутной автоблокировки
  {
		if((n_mane==1)||(n_manevro==1)||(n_manevr==1))return(0);
		nnnn1=fr1[D][2]&0xfff;//объект для состояния перегона
    a=(fr1[D][2]&0xf000)>>12;//бит для состояния перегона
		nnnn2=fr1[D][3]&0xfff;//объект для направления перегона
    b=(fr1[D][3]&0xf000)>>12;//бит для направления перегона
    if((fr3[nnnn2][b]==0)&&(fr3[nnnn1][a]==1))//если прием и перегон занят
    {
      w(5,999," ПРИБЫВАЕТ ПОЕЗД");
      return(1);
    }
    else return(0);
  }
	return(0);
}
//----------------------------------------------------------------
//проверка наличия сигнала конца маршрута вдоль установленных стрелок
int tst_nal_sig()
{
	nom_func("365");
	ukaz_trass=1;//поставить в начало
	// выполнить пока не дойдем до конца
	while(TRASSA0[ukaz_trass].object!=0xFFFF)
	{ if(fr1[TRASSA0[ukaz_trass].object&0xfff][0]==2)// если в цепочке найден сигнал
		{ if(fr1[TRASSA0[ukaz_trass].object&0xfff][2]==5)goto fi; // если смена направления
			if((TRASSA0[ukaz_trass].object&0xfff)==markery[modi][6])goto fi; //если на этом сигнале маркер
			if(mane==1)manevro=1;
			switch(manevro)//переключатель по типу маршрута
			{//поездн.маршрут
				case 0: if(fr1[point][1]==0)//если четный маршрут
								{ //если у сигнала нет конца четного поездного - прервать
									if((fr1[TRASSA0[ukaz_trass].object&0xfff][11]&64)!=64)break;
									else return(0);
								}
								else//если нечетный маршрут
								{
									//если у сигнала нет конца нечетного поездного - прервать
									if((fr1[TRASSA0[ukaz_trass].object&0xfff][11]&16)!=16)break;
									else return(0);
								}
/*
								//если поездной сигнал или входной, то выход
								if((fr1[TRASSA0[ukaz_trass].object&0xfff][2]==1)||
								(fr1[TRASSA0[ukaz_trass].object&0xfff][3]==7))return(0);
								//если маршрут с 2-умя белыми-и есть конец для двух белых
								if((fr1[point][6]==3)&&
								(fr1[TRASSA0[ukaz_trass].object&0xfff][4]==3))return(0);
								break;
*/
				//маневр.маршрут
				case 1:
								if(fr1[point][1]==1)//если нечетный маршрут
								{
									//если у сигнала нет конца нечетного маневрового-прервать
									if((fr1[TRASSA0[ukaz_trass].object&0xfff][11]&1)!=1)break;
									else return(0);
								}
								else//если четный маршрут
								{
									//если у сигнала нет конца четного маневрового-прервать
									if((fr1[TRASSA0[ukaz_trass].object&0xfff][11]&4)!=4)break;
									else return(0);
								}
/*
								//если сигнал манавровый или
								if((fr1[TRASSA0[ukaz_trass].object&0xfff][2]==0)||
								//или сигнал совмещенный
								(fr1[TRASSA0[ukaz_trass].object&0xfff][6]==2)||
								//или сигнал выходной
								(fr1[TRASSA0[ukaz_trass].object&0xfff][3]==7))return(0);
								break;
								*/
				default: break;
			}
		}
fi:
		ukaz_trass++;//если не сигнал,то идти дальше
	}
	//если дошли до конца, а сигнала не было
	if((manevro==0)&&(mane==0))w(82,999,"");//ДЛЯ ПОЕЗДНОГО МАРШРУТА СТРЕЛКИ НЕ УСТАНОВЛЕНЫ
	else w(83,999,""); // МАРШРУТ НЕ СУЩЕСТВУЕТ
	return(1);
}
//----------------------------------

int tst_put(int P,int avto,int tipus)
{
	int naprav,kateg,n_mane,n_manevro;

	nom_func("367");
	if(tipus==0){n_mane=mane;n_manevro=manevro;}
	if(tipus==1){n_mane=mane1;n_manevro=manevro1;}
	if(uprav==0)
	{ if((fr1[P][1]==1)&&(fr1[P][2]==1)&&(n_mane==0)&&(n_manevro==0))
		{
//    if(avto=0)
		w(83,999,"!"); return(1);}//МАРШРУТ НЕ СУЩЕСТВУЕТ
	}
	if(fr4[P][2]==1)
	{
		if(avto==0)w(115,P,""); //ДВИЖЕНИЕ ЗАПРЕЩЕНО ПУТЬ
		if(uprav==1)return(1);
		else return(2);
	}
	if(fr3[P][5]==1){if(avto==0)w(116,P,"");return(1);}//НЕИСПРАВЕН ПУТЬ
	if(tipus==1)naprav=fr1[nach_marsh][1];
	if(tipus==0)naprav=fr1[nach_marsh_oper][1];
	if(n_mane==0)kateg=1;
	else kateg=0;
	switch(naprav)
	{
		case 0: if(kateg==1) //четное поездное
						{
							if(((fr3[P][10]&0xf0)!=0)&&(avto==0))
							{
								w(238,P,"");
								return(2);
							}
						}
						else //четное маневровое
						{
							if(((fr3[P][10]&0xf0)!=0)&&(avto==0))
							{
								w(238,P,"");
								return(2);
							}
						}
						break;
		case 1: if(kateg==1)	//нечетное поездное
						{
							if(((fr3[P][10]&0xf0)!=0)&&(avto==0))
							{
								w(238,P,"");
								return(2);
							}
						}
						else //нечетное маневровое
						{
							if(((fr3[P][10]&0xf0)!=0)&&(avto==0))
							{
								w(238,P,"");
								return(2);
							}
						}
						break;

		default: break;
	}
	if(zanyato(P,avto,tipus)==1) return(1);
	if(zamknuto(P,avto,tipus)==1) return(1);
	return(0);
}
//-------------------------------------------------------------
int tst_sig(int S,int av,int tipus)
{//проверка сигнала в трассе заданного маршрута
 //S - сигнал
 //av - признак автодействия (0-нет,1-нечетное,2-четное)
 //при норме возвращает 0, иначе 1.
	int gul;

	nom_func("368");
	//если управление маршрутное - взять начало
	if(fr1[S][1]==2)return(0);
	if(uprav==1)
	{
		if(tipus==0)gul=nach_marsh_oper;
		else gul=nach_marsh;
	}
	//для раздельного взять то, что хотим открыть
	else gul=point;

	if((av!=0)&&(av<3))gul=nach_avto; //для автодействия взять свое начало

	//если непарафазность-НЕИСПРАВЕН СИГНАЛ
	if(fr3[S][5]==1){if(av==0)w(88,S,"");return(1);}
	//если все закрыто
	if((fr3[S][0]==0)&&(fr3[S][1]==0)&&(fr3[S][2]==0)&&(fr3[S][3]==0))return(0);
	if(fr1[S][1]==fr1[gul][1])//если сигнал попутный
	{
		if(((tipus==0)&&(S!=end_marsh_oper))||//если не конец маршрута
		((tipus==1)&&(S!=end_marsh)))
		{
			if(av==0)w(78,S,"");//если сигнал не конец - ОТКРЫТ СИГНАЛ
			else return(0);
			return(1);
		}
	}
	else//если враждебный сигнал
	{
		if(av==0)w(128,S,"");//ОТКРЫТ ВРАЖДЕБНЫЙ СИГНАЛ
		return(1);
	}
	return(0);
}
//--------------------------------------------------------
int tst_sigogo(int sip,int tipus)
{ //процедура продолжения маршрута за встречный сигнал
	int n_m,mo=sip;

	nom_func("369");
	//если сигнал попутный - разрешить
	if(tipus!=0)n_m=nach_marsh;
	else n_m=nach_marsh_oper;
	if(fr1[sip][1]==fr1[n_m][1])return(0);
	while((fr1[mo][0]==1)||//пока стрелка или
	(fr1[mo][0]==2)||  //сигнал или
	(fr1[mo][0]==6)||  //переход или
	(fr1[mo][0]==7))//ДЗ
	{
		if((mo<0)||(mo>kol_OSN))return(1);//если вышли за пределы базы - нет маршрута
		if(fr1[n_m][1]==1)mo++;//продвинуться на следующий элемент маршрута
		else mo--;
		if(tipus!=0)TRASSA[ukaz_trass++].object=mo;
		else TRASSA0[ukaz_trass++].object=mo;
	}
	if(tipus!=0)END=mo;//если вышли на СП,УП или путь - назначить этот элемент концом
	else END_OPER=mo;
	return(0);//вернуть разрешение
}
//-------------------------------------------------
int tst_str_av(int S,int polog)
{
	nom_func("373");
	if(fr3[S][5]==1){w(72,S,""); return(1);} // неисправна стрелка
	if(fr3[S][0]==fr3[S][1]){w(127,S,"");return(1);}//НЕ ИМЕЕТ КОНТРОЛЯ СТРЕЛКА
  if(fr4[S][2]==1){w(71,S,"");return(1);}//ЗАПРЕЩЕНО ДВИЖЕНИЕ ПО СТРЕЛКЕ
  if(fr4[S][0]!=0)
  {
    if(fr3[S][0]==0&&fr3[S][1]==1&&polog==-1)return(0);
    if(fr3[S][0]==1&&fr3[S][1]==0&&polog==1)return(0);
		w(67,S,"");//ОТКЛ. ОТ УПРАВЛ. СТРЕЛКА
    return(1);
  }
  if(fr4[S][1]==1){w(99,S," ВКЛЮЧЕНА НА МАКЕТ");return(1);}
  if(fr3[S][0]==0&&fr3[S][1]==1&&polog==-1)return(0);
  if(fr3[S][0]==1&&fr3[S][1]==0&&polog==1)return(0);
  w(126,S,"");//НЕ ПО МАРШРУТУ СТРЕЛКА
  return(1);
}
//---------------------------------------------------
int tst_str1(int S)
{
	nom_func("372");
	 if(fr3[S][5]==1)
	 {
	 	w(72,S,"");
    vibeg=0;
		return(-1);
	 } // неисправна стрелка

	if((fr4[S][1]==1)&&(maka==0)){w(99,S," ВКЛЮЧЕНА НА МАКЕТ"); return(5);}
	if(fr4[S][2]==1){w(71,S,"");return(2);}//ЗАПРЕЩЕНО ДВИЖЕНИЕ ПО СТРЕЛКЕ
	if(fr3[S][0]==fr3[S][1])
		{w(127,S,"");return(1);}//НЕ ИМЕЕТ КОНТРОЛЯ СТРЕЛКА

	if(fr1[point][1]==fr1[S][7])return(0);//если вход на стрелку совпал с напр.

	if((fr1[TRASSA0[ukaz_trass-1].object&0xfff][0]==1)||//если перед стрелкой стрелка или...
	(fr1[TRASSA0[ukaz_trass-1].object&0xfff][0]==6)||   //если перед стрелкой переход или ...
	(fr1[TRASSA0[ukaz_trass-1].object&0xfff][0]==7))// если перед стрелкой ДЗ
	{ //если по + прямо и стрелка в плюсе или ...
		if(((fr1[S][1]==1)&&(fr3[S][0]==1)&&(fr3[S][1]==0))||//или
		//если прямо по - и стрелка в минусе
		((fr1[S][1]==0)&&(fr3[S][0]==0)&&(fr3[S][1]==1)))
		{ // если при заходе на стрелку другая нить
			if(fra[TRASSA0[ukaz_trass-1].object&0xfff][0]!=fra[S][0])
			{ w(126,S,"");//НЕ ПО МАРШРУТУ СТРЕЛКА
				return(1);
			} else return(0);
		}
		else
		{ // если по плюсу прямо и стрелка в минусе
			if(((fr1[S][1]==1)&&(fr3[S][0]==0)&&(fr3[S][1]==1))|| //или
			//если по минусу прямо, а стрелка в плюсе
			((fr1[S][1]==0)&&(fr3[S][0]==1)&&(fr3[S][1]==0)))
			{
				// если одинаковые нити
				if(fra[TRASSA0[ukaz_trass-1].object&0xfff][0]==fra[S][0])
				{ w(126,S,"");//НЕ ПО МАРШРУТУ СТРЕЛКА
					return(1);
				} else return(0);
			}
		}
	}
	else//если переход по плюсу прямой и стрекла в минусе
		if(((fr1[S][1]==1)&&(fr3[S][0]==0)&&(fr3[S][1]==1))||
		((fr1[S][1]==0)&&(fr3[S][0]==1)&&(fr3[S][1]==0)))
		{ w(126,S,"");//НЕ ПО МАРШРУТУ СТРЕЛКА
			return(1);
		}
	return(0);
}
//------------------------------------------------------
int tst_strelok(int S,int tipus, int uk_tras)
//проверка возможности перевода стрелки
{ int U,UU,tstr,ii,jj,kk;

	nom_func("374");
	if(((tst_chet(S)==0)&&(fr3[RZ_CHET][RZ_CHET_BIT]==1))||  //если стрелка замкнута
	((tst_chet(S)==1)&&(fr3[RZ_NECHET][RZ_NECHET_BIT]==1)))//ручным замыканием
	//ВКЛЮЧЕНО РУЧНОЕ ЗАМЫКАНИЕ СТРЕЛОК
	{w(166,S,"");return(1);}
	if(((fr1[S][13]==CHET)&&(fr3[AVAch][AV_bch]==1))||//если нажата вспом.перевод четный
	((fr1[S][13]==NECHET)&&(fr3[AVAn][AV_bn]==1)))//если нажата вспом.перевод нечетный
	//Нажата кнопка вспомогательного перевода
	{w(111,S,"");return(1);}
	//если стрелка отключена от управления-"ОТКЛ. ОТ УПРАВЛ. СТРЕЛКА"
	if(((fr4[S][0]&7)!=0)&&(DU==0)){w(67,S,"");return(1);}
	if(((fr4[S][0]&0xf8)!=0)&&(osya==0))
	{
		w(240,999,"");
		if(uprav==0)return(2);
		else return(1);
	}
	if((hod==1)&&(uprav==0))goto sparka;
	U=findspu1(S,fr1[S][7]);//найти СП
	if(U==-1)
	{
		hod=1;
		if(uprav==1)goto spr2;
		else goto sparka;
	}
	UU=tst_spu(U,0,1,tipus);//проверить СП
	if((osya==2)&&(UU==5)&&(uprav!=0))
	{
		for(ii=0;ii<skoko_stoek*3;ii++)		//пройти по всем маршрутам
		for(kk=0;kk<skoko_stoek;kk++) 		//пройти по всем стойкам
		for(jj=0;jj<20;jj++)          		//пройти по всем стрелкам
		{
			if(S==(MARSHRUT_ALL[ii].STREL[kk][jj]&0xFFF))
			{
				DeleteMarsh(ii);
			}
		}
		return(0);
	}
	if((uprav==0)||(UU==5))//если управление раздельное или предварит. замык
	{
		if(UU==10) return(10);
		if((UU==1)||(UU==5))
			if(osya==0){osya=2;return(2);}
			else {osya++;return(osya);}

		if(fr1[S][12]!=9999)//если стрелка спаренная
		{ osya=0;
			hod=1;
sparka:
			TEST_SPAR=2;
			U=poisk_ras(fr1[S][2],S,tipus,uk_tras);//найти парную
			if((fr1[U][11]&256)!=256)
			{
				U=findspu1(U,fr1[U][7]);//найти СП для парной стрелкой
				UU=tst_spu(U,0,1,tipus);//проверить состояние СП
				if(UU==10) return(10);
				if((UU==1)||(UU==5))
				if(osya==0){osya=2;return(2);}
				else {osya++; return(osya);}
			}
			else return(0);
		}
	}
	else//если управление маршрутное
	{ tstr=UU;
		if(tstr==1)return(1);//проверить СП
		if(fr1[S][12]!=9999)//если стрелка спаренная
		{
spr2:	TEST_SPAR=2;
			U=poisk_ras(fr1[S][2],S,tipus,uk_tras);
			if((fr1[U][11]&256)==256)return(0);
			U=findspu1(U,fr1[U][7]);
			if(tst_spu(U,0,1,tipus)==1) return(1);
			return(0);
		}
	}
	return(0);
}
//-------------------------------------------------------------
int tst_uch(int P,int avto,int tipus)
{
	int Test,n_mane;

	nom_func("375");
	if(tipus==0)n_mane=mane;
	if(tipus==1)n_mane=mane1;
	//НЕИСПРАВЕН УЧАСТО  //ДВИЖЕНИЕ ЗАПРЕЩЕНО ПУТЬ
	if(fr4[P][2]==1)
	{
	 w(115,P,"");soob_for_oper=1;
	 if(uprav==1)return(1);
	 else return(2);
	}
	for(k=0;k<10;k++)if((BEDA[k]&0xfff)==P)break;
	if(k==10)for(k=0;k<10;k++)if(BEDA[k]==0)break;
	if(k==10)for(k=0;k<10;k++)BEDA[k]=0;
	if(fr3[P][5]==1){w(104,P,"");return(1);}//НЕИСПРАВЕН УЧАСТОК
	if((uprav==0)&&(avto==0))
	{
		if(n_mane==0)
		{
			Test=P|0x1000;
			if((Test&BEDA[k])!=Test)
			if((osya<2)&&(zanyat(P,avto,tipus)==1))goto brak;
		}
		Test=P|0x2000;
		if((Test&BEDA[k])!=Test)
		if((osya<3)&&(zamknut(P,avto,0)==1))
		{ if(AVARI==1){home(modi);finish_strelok();klaval=0;return(10);}
			goto brak;
		}
		Test=P|0x4000;
		if((Test&BEDA[k])!=Test)
		if((osya<4)&&(razdelan(P,avto)==1))
		{ if(AVARI==1){home(modi);finish_strelok();klaval=0;return(10);}
			goto brak;
		}
		return(0);
	}
	else
	{ if(zanyat(P,avto,tipus)==1) return(1);
		if(zamknut(P,avto,0)==1) return(1);
		if(razdelan(P,avto)==1) return(1);
		return(0);
	}
brak:
  BEDA[k]=BEDA[k]|Test;
  soob_for_oper=0;
  return(1);
}
//----------------------------------------------------
int zamknuto(int pu,int avt,int tipus)
{
	int chkm,nkm,bit_ch,bit_n;

	nom_func("430");
	if((fr3[pu][10]==2)&&(avt==0))
	{
		if(mane==0)
		{
			w(238,pu,"");return(1);
		}
	}
	if((fr3[pu][1]==0)&&(fr3[pu][2]==0))return(0);//если путь разомкнут
  else//если путь замкнут
		if(((tipus==0)&&(manevro==0)&&(mane==0))||//если поездной
		((tipus==1)&&(manevro1==0)&&(mane1==0)))
    {
      if(avt==0)w(217,pu,"");//ЗАДАН МАРШРУТ НА ПУТЬ
      return(1);
    }
    else//если маневровый маршрут
		{
      chkm=fr1[pu][11]&0xFFF;//объект для ЧКМ
      nkm=fr1[pu][12]&0xFFF;//объект для НКM
      bit_ch=(fr1[pu][11]&0xF000)>>12;//
      bit_n=(fr1[pu][12]&0xF000)>>12;
      if((fr3[pu][1]==1)||(fr3[pu][2]==1))//если ЧИ или НИ под током
      {
        if((fr3[chkm][bit_ch]==0)&&(fr3[nkm][bit_n]==0))
        {
          if(avt==0)w(8,pu,"");//ЗАДАН ПОЕЗДНОЙ МАРШРУТ НА ПУТЬ
          return(1);
        }
      }
      return(0);
		}
}
//----------------------------------------
int zanyato(int pu,int avt,int tipus)
{ int op=pu,E_N_D,n_mane,n_manevro,N_M,E_M;

	nom_func("432");
	if(tipus!=0)
	{
		E_N_D=END;
		n_mane=mane1;
		n_manevro=manevro1;
		N_M=nach_marsh;
		E_M=end_marsh;
	}
	else
	{
		E_N_D=END_OPER;
		n_mane=mane;
		n_manevro=manevro;
		N_M=nach_marsh_oper;
		E_M=end_marsh_oper;
	}
	if(fr3[pu][0]==0) return(0);//если путь свободен
	if((n_mane==1)||(n_manevro==1))//если задается маневровый маршрут
	{ if(pu==E_N_D)return(0);//если путь является концом маршрута

		if(fr1[N_M][1]==fr1[E_M][1])//если сигналы попутные
		{ if(fr1[N_M][1]==1) op=pu+1;//если маршрут нечетный - вперед по базе
			else op=pu-1;//если маршрут четный по базе назад

			if(op==E_N_D) return(0);//если ближайший сигнал - конец - можно
			else
			{
				w(106,pu,"");//ЗАНЯТ ПУТЬ
				return(1);
			}
		}
		else//если хотят ехать за сигнал ограничивающий путь
		{ if(avt==0)w(106,pu,"");//НЕВОЗМОЖЕН МАНЕВРОВЫЙ МАРШРУТ ЧЕРЕЗ ПУТЬ
			return(1);
		}
	}
	else//если поездной
	{ if(fr1[pu][0]==4)w(94 ,pu,"");//если хотят на занятый УП - ЗАНЯТ УЧАСТОК
		else
			if((fr1[pu][1]==0)&&(fr1[pu][2]==0))//если путь приемо-отправочный
			{
				if(avt==0)w(106,pu,"");//ЗАНЯТ ПУТЬ
			}
			else if(avt==0)w(107,pu,"");//ЗАНЯТ УЧАСТОК УДАЛЕНИЯ
		return(1);
	}
//	return(0);
}
//-----------------------------------------------------------------------
/**********************************************\
* Процедура формирования команды на установку  *
* автодействия для передачи в резервную ПЭВМ   *
\**********************************************/
void form_kom_avtod(char COD)
{

	int i11,n_bait;
	unsigned char kod;

	nom_func("74");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return; //для просмотра архива ничего не делать
	mane=0;
	manevro=0;
	//'@'-все откл.; 'A'-вкл.нечет.(откл. чет.)
	//'B'-вкл.чет.(откл.нечет.);'C'-вкл.четн. и нечет.
	kod='@'+cha*2+na; //формирование кода по состоянию признаков автодействия
	if(buf_ko_p[0]!=0)//если первый буфер команд не пустой
	{
		buf_ko_p2[0]='(';//начать заполнение второго буфера
		buf_ko_p2[1]='Z';//начать заполнение второго буфера
		buf_ko_p2[2]=COD; //автодействие 'A'-включение, 'a'-выключение
		buf_ko_p2[3]=kod;//записать в буфер код (включение/отключение

		//в свободные байты - заполнитель
		for (i11=4;i11<(n_bait-2);i11++)buf_ko_p2[i11]=124;

		//получить и записать контрольную сумму
		buf_ko_p2[9]=check_sum(buf_ko_p2,0);
		buf_ko_p2[10]=')';
		flms=2;
	}
	else//если первый буфер пустой - то все то же самое для первого буфера
	{
		buf_ko_p[0]='(';//начать заполнение второго буфера
		buf_ko_p[1]='Z';
		buf_ko_p[2]=COD;
		buf_ko_p[3]=kod;
		for (i11=4;i11<9;i11++)buf_ko_p[i11]=124;
		buf_ko_p[9]=check_sum(buf_ko_p,0);//получить и записать контрольную сумму
		buf_ko_p[10]=')';
		//установить признак-требование передачи сообщения в соседнюю ПЭВМ
		flms=1;
	}
	kom_v_bufer_pvm(1,0);
	return;
}
//------------------------------------------------------------------------
/******************************************************\
* Процедура формирования команды в стойку для перевода *
* стрелок. kod - код команды перевода стрелки.         *
\******************************************************/
void formkps(int kod)
{
	int podgr,i18,j18,n=0,SDVIG=0,FN=0,FIN=0,kkol=0,n_bait;
	unsigned int nomer;

	nom_func("86");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return; //если просмотр архива, то ничего не делать

	if(buf_ko[2]==0) //если буфер чист
	{
		buf_ko[2]='C';

		//найти стрелку, входящую в сообщения (имеет смысл для спаренных)
		if(Opred()<0)return;
		n=point;

		if(fr1[n][13]==1){buf_ko[1]=ZAGO_MPSU(0x61);SDVIG=0;FN=0;FIN=STR1;}
#ifdef KOL_SOO2
		else {buf_ko[1]=ZAGO_MPSU(0x62);SDVIG=0;FN=STR1;FIN=FN+STR2;}
#endif
		nomer=0xAAAA;
		for(i18=FN;i18<FIN;i18++)for(j18=0;j18<5;j18++)
		if(spstr[i18][j18]==n){podgr=podgruppa[i18+SDVIG-FN];nomer=j18;break;}
		if(nomer==0xAAAA) return;
		buf_ko[3]=podgr;
		kkol=kod;

		//если стрелка на макете, изменить код
		if(fr4[point][1]==1)kkol=kkol|0x04;//если стрелка на макете

		//если аварийный перевод, добавить признак в код команды
		if(AVARI==1)kkol=kkol|0x10;
		else
			//если автовозврат стрелки, добавить признак
			if((AVARIIN==1)&&(fr4[point][1]==0))kkol=kkol|0x08;

		//найти байт в сообщении для вставки кода команды
		for (j18=0;j18<5;j18++)
		if(j18==nomer)buf_ko[j18+4]=kkol;
		else  buf_ko[j18+4]=124;
		buf_ko[0]='(';
		if(n_bait==12)buf_ko[9]=124;
		buf_ko[n_bait-2]=check_sum(buf_ko,1);
		buf_ko[n_bait-1]=')';
		//если аварийный перевод стрелки, то команду сохранить во всп.буфере,
		//а основной буфер команд обнулить (ожидается нажатие кнопки на пульте)
		if(AVARI==1)
		{
			for(i18=1;i18<n_bait-1;i18++){buf_ko_vsp[i18]=buf_ko[i18];buf_ko[i18]=0;}
			buf_ko_vsp[0]=point;

			//оцениваем четность или нечетность стрелки и заряжаем таймер на
			//ожидание нажатия четной или нечетной кнопки аварийного перевода
			if(tst_chet(point)==0)TIMER_SOB(AVAch,1,AV_bch);
			else TIMER_SOB(AVAn,1,AV_bn);
			return;
		}
		flagoutmsg=1;

		pooo_com[point]=biostime(0,0l);
		com_bit[point]=kod&3;
	}
	else //если в буфере, что-то есть, то заполняем buf_ko_mm
	{
		buf_ko_mm[2]='C';
		if(Opred()<0)return;//найти стрелку, входящую в сообщения (имеет смысл для спаренных)
		n=point;
		if(fr1[n][13]==1){buf_ko_mm[1]=ZAGO_MPSU(0x61);SDVIG=0;FN=0;FIN=STR1;}
#ifdef KOL_SOO2
		else {buf_ko_mm[1]=ZAGO_MPSU(0x62);SDVIG=0;FN=STR1;FIN=FN+STR2;}
#endif
		nomer=0xAAAA;
		for(i18=FN;i18<FIN;i18++)for(j18=0;j18<5;j18++)
		if(spstr[i18][j18]==n){podgr=podgruppa[i18+SDVIG-FN];nomer=j18;break;}
		if(nomer==0xAAAA) return;
		buf_ko_mm[3]=podgr;
		kkol=kod;
		if(fr4[point][1]==1)kkol=kkol|0x04;//если стрелка на макете
		if(AVARI==1)kkol=kkol|0x10;//если аварийный перевод
		else
			if((AVARIIN==1)&&(fr4[point][1]==0))kkol=kkol|0x08;
		for (j18=0;j18<5;j18++)
		if(j18==nomer)buf_ko_mm[j18+4]=kkol;
		else  buf_ko_mm[j18+4]=124;
		buf_ko_mm[0]='(';
		buf_ko_mm[n_bait-2]=check_sum(buf_ko_mm,1);
		buf_ko_mm[n_bait-1]=')';
		flagoutmsg=11;

		pooo_com[point]=biostime(0,0l);
		com_bit[point]=kod&3;
	}
	return;
}
//---------------------------------------------------------------------
/*************************************\
* Процедура формирования команды для  *
* вспомогательного перевода стрелки   *
\*************************************/
void form_kom_vsp()
{
	int n_bait,i18=0;

	nom_func("82");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;
	if(AVARI==1)//если установлен признак аварийного перевода стрелки
	{
		for(i18=0;i18<(n_bait-2);i18++)
		{
			//перезаписать команду в исполнительный буфер
			buf_ko[i18+1]=buf_ko_vsp[i18+1];
			//вспомогательный буфер освободить
			buf_ko_vsp[i18+1]=0;
		}
		flagoutmsg=1;//установить флаг-требование на пересылку команды в ТУМС
		w(123,buf_ko_vsp[0],"");//"ПОСЫЛАЕМ КОМАНДУ НА ПЕРЕВОД СТРЕЛКИ"
		buf_ko_vsp[0]=0;
	}
	return;
}
//-----------------------------------------------------------------------
/******************************************************\
* Процедура заполнения выходного буфера для передачи в *
* соседнюю ПЭВМ копий команд ТУМСа и ограничений FR4.  *
\******************************************************/
int kom_v_bufer_pvm(int tip_oper, unsigned char WCHAR)
{
	int i,n_bait;

	nom_func("128");
	n_bait=11;
	if(DISK!=0)return(-1);

	if(tip_oper==0)// если информация
	{
		REG_INF[0]=0x1e;
		REG_INF[1]=WCHAR;REG_INF[2]=WCHAR;
		REG_INF[3]=0x1f;
		MY_INF=1;
		return(0);
	}
	if(tip_oper==1)// если команда
	{ // если в работе две команды для ТУМС (автодействие)
		if(flms==2)
		{
			for(i=0;i<n_bait;i++)REG_COM[i]=buf_ko_p2[i];
			REG_COM[9]=check_sum(REG_COM,0);
			REG_COM[10]=')';
		}
		else
			//если требуется передача в резервную ПЭВМ (ограничения)
			if(flms==1)for(i=0;i<n_bait;i++)REG_COM[i]=buf_ko_p[i];
			// для команд в стойки ТУМС
			else
			{
				for(i=0;i<n_bait;i++)REG_COM[i]=buf_ko[i];
				REG_COM[9]=check_sum(REG_COM,0);
				REG_COM[10]=')';
			}
		if(pust_pvm>1)return(-1);//если потеряна связь с соседней ПЭВМ
		MY_COM=1;
		flms=0;
		return(1);
	}
	if(tip_oper==2)// если команда из таблицы маршрутов
	{
		for(i=0;i<n_bait;i++)REG_COM[i]=REG_COM_TUMS[WCHAR][i];
		REG_COM[9]=check_sum(REG_COM,0);
		REG_COM[10]=')';
		if(pust_pvm>1)return(-1);//если потеряна связь с соседней ПЭВМ
		MY_COM=1;
		flms=0;
		return(1);
	}
	return(-1);
}
//--------------------------------------------------------------
int uze(int noper,int naps)
{
	int j;
	nom_func("385");
	j=ukaz_trass;
	j--;
	if(fr1[TRASSA0[j].object&0xfff][0]==6)return(-1);
	while(j!=0)
	{ if((TRASSA0[j].object&0xfff)==noper)
    { if(naps==1) return(1);
      else
        if(noper!=0) return(1);
    }
    j--;
  }
  return(0);
}
