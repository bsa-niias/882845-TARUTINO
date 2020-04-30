#include "bla.h"
#include "opred.h"
//-----------------------------------------------
int str_Makst(int nom)
{
  int ro=0;
	nom_func("320");
	for(ro=0;ro<kol_OB_UPR;ro++)
  { if(markery[ro][6]==nom) return(ro);
    if(ro>=Ok) return(0);
  }
  return(0);
}
//------------------------------------------------------
int strelka_v_makete()
{
  int ro;
	nom_func("322");
	for(ro=0;ro<kol_OSN;ro++)if((fr4[ro][1]==1)&&(fr1[ro][0]==1))return(ro);
  return(0);
}
//-------------------------------------------------------
void vkl_otkl_maket(int nom)
{
  int nomer;
	nom_func("401");
	if(DISK!=0)return;
#ifdef MAKET
	if(markery[nom][7]!=1)
	{ w(17,999,"");//ЭТО НЕ СТРЕЛКА
		maket=0;vkl_kno(MAKET,8);home(modi);
		return;
	}
	nomer=markery[nom][6];
	if(fr4[nomer][1]==0) vopros(3,nomer,73,"?");//УСТАНОВИТЬ НА МАКЕТ СТР.
	else vopros(3,nomer,74,"?");//СНЯТЬ С МАКЕТА СТР.
	point=nomer;
	maket=2;
	vih_vopr=1;
#endif
	return;
}
//-------------------------------------------------------
void v_o_ma(int nom)
{
  int BUK,para;
  unsigned char BUKA;
	nom_func("386");
	if(DISK!=0)return;
  if(klaval==13)
  {
    //ТЕЛЕУПРАВЛЕНИЕ ОТКЛЮЧЕНО-КОМАНДЫ НЕ ВОЗМОЖНЫ
    if(TELEUP!=1){ w(58,999,"");return;}
    //ВКЛЮЧЕНО АВТОДЕЙСТВИЯ ПО 1П
    if(fr4[nom][0]&4){w(91,999,"");return;}
    if(fr1[nom][12]!=9999)
    { para=fr1[nom][12];
      //ВКЛЮЧЕНО АВТОДЕЙСТВИЯ ПО 1
      if(fr4[para][0]&4){w(91,999,"");return;}
    }
    //ВКЛЮЧЕНО АВТОДЕЙСТВИЯ ПО 2П
    if(fr4[nom][0]&2){w(90,999,"");return;}
    if(fr1[nom][12]!=9999)
    { para=fr1[nom][12];
      //ВКЛЮЧЕНО АВТОДЕЙСТВИЯ ПО  2П
      if(fr4[para][0]&2){w(90,999,"");return;}
		}
#ifdef MAKET
		//УСТАНАВЛИВАЕМ НА МАКЕТ СТРЕЛКУ
		if(fr4[nom][1]==0){fr4[nom][1]=1;w(112,nom,"");vkl_kno(MAKET,10);}
		//СНИМАЕМ С МАКЕТА СТРЕЛКУ
		else {fr4[nom][1]=0; w(75,nom,"");vkl_kno(MAKET,8);}
#endif
  }
  else return;
  BUK=fr4[nom][0];
  BUKA=fr4[nom][0]&1;
  if(fr4[nom][1]==1){BUK=BUK|0x200;BUKA=BUKA|2;}
  if(fr4[nom][2]==1)BUKA=BUKA|4;
  if(fr1[nom][12]==0)BUKA=BUKA|0x8;//установить признак неосновной
  ris_ogr_str(nom,BUK);
	okrasklav(nom);
  if(fr1[nom][12]!=9999)
  {
		TEST_SPAR=2;
		para=poisk_ras(fr1[nom][2],nom,0,0);
    fr4[para][1]=fr4[nom][1];
    ris_ogr_str(para,BUK);
		okrasklav(para);
  }
  BUKA=BUKA|0x80;
  form_kom_str(nom,BUKA);
  maket=0;
	return;
}
//---------------------------------------------------------------------------
/*******************************************************\
* Процедура наложения и снятия ограничений для стрелок. *
\*******************************************************/
void otkl(int nomer)
{ int BUK,x23,y23,para,kudo,avtr;
	unsigned char BUK1;
	nom_func("205");
	//для просмотра архива не действует
	if(DISK!=0)return;

	home(modi);  //вернуть курсор домой

	if((STATUS==0)||(STATUS==2)) //если АРМ не управляет
	{ w(140,999,""); //невозможна выдача команды с резервной ПЭВМ
vsi:
		vkl_kno(VSPZ,8);// погасить кнопку активизации ограничений
		return;
	}
	if(TELEUP!=1) goto vsi;

	t(0);//погасить сектор сообщений

	x23=fr2[nomer][0];y23=fr2[nomer][1]; //взять координаты начала объекта
	// найти парную стрелку
	TEST_SPAR=2;

	if(fr1[nomer][12]!=9999)para=poisk_ras(fr1[nomer][2],nomer,0,0);
	else para=0;

	if(punkt_N==3)// выбран нижний пункт меню:закрыть-открыть движение
	{
#ifdef AVTOD
		avtr=prov_ele_avtod(nomer);
		if(avtr==1){w(245,999,"");return;}
		if(avtr==2){w(244,999,"");return;}
#endif
		setlinestyle(0,0,3);
		if(fr4[nomer][2]==0)//если стрелка пока не закрыта для движения
		{ fr4[nomer][2]=1; // установить признак закрытия для движения
			setcolor(LIGHTRED);
		}
		else // если стрелка закрыта для движения
		{
			fr4[nomer][2]=0;// снять закрытие для основной стрелки
			setcolor(cvv);
		}
		if(fr1[nomer][7]==1)kudo=-1;// если вход нечетный
		else kudo=1;// если вход четный
		kudo=OBRAT*kudo; // учитываем ориентацию экрана
		x23=x23-2*kudo; // вычисляем точку оси X
		line(x23,y23-5,x23,y23-2);
		line(x23,y23+2,x23,y23+5);
	}
	else
	if(punkt_N==2)//выбран верхний пункт меню: включить-выключить управление
	{ setlinestyle(0,0,0);
		// если не выключена из управления, то выключить
		if((fr4[nomer][0]&1)==0)fr4[nomer][0]=fr4[nomer][0]|1;
		//если выключена из управления, то включить
		else fr4[nomer][0]=fr4[nomer][0]&0xfffe;

	}
	disco();
	BUK1=0;//код для передачи в соседнюю ПЭВМ
	BUK=fr4[nomer][0];//код для отрисовки
	if((BUK&1)==1)BUK1=1;
	if(fr4[nomer][1]==1){BUK=BUK|0x200;BUK1=BUK1|2;}
	else BUK=BUK&0xfdff;
	if(fr4[nomer][2]==1)BUK1=BUK1|4;
	if(fr1[nomer][12]==0)BUK1=BUK1|0x8;//если не основная
	ris_ogr_str(nomer,BUK);
	okrasklav(nomer);
	if(para>0)
	{ fr4[para][0]=fr4[nomer][0];
		fr4[para][1]=fr4[nomer][1];
		ris_ogr_str(para,BUK);
		okrasklav(para);
	}
	BUK1=BUK1|0x80;
	if(prorisovka==0)form_kom_str(nomer,BUK1);//отправить команду в соседнюю ПЭВМ
	return;
}
//--------------------------------------------------------------------------
/***********************************************\
* Процедура отображения ограничений на стрелки  *
* nomer -объект стрелки в базе                  *
* otkuda=0-закрытие для движения                *
* otkuda=1-остальное                            *
\***********************************************/
void nach_zakr(int nomer,int otkuda)
{
	int x13,y13,kudo,kuk=2,BUKA;
  struct linesettingstype  line_style;
	nom_func("178");
	if(ZAGRUZKA!=0)return;//во время загрузки не выполняется

  //при просмотре архивов и помощи не выполняется
  if((klo==1)||(help>0)||(nikuda==1))return;

  //если объект не стрелка не выполнять
  if(fr1[nomer][0]!=1) return;

  //координаты точки ветвления
  x13=fr2[nomer][0];
  y13=fr2[nomer][1];

  //считать данные по отключению от управлению
  BUKA=fr4[nomer][0];

  if(fr4[nomer][1]==1)BUKA=BUKA|0x200;//если макет
  else BUKA=BUKA&0xfdff;

  ris_ogr_str(nomer,BUKA);//отобразить ограничения

	getlinesettings(&line_style);
	//если закрыта для движения
	if(fr4[nomer][2]==1)
	{
		setcolor(LIGHTRED);
		setlinestyle(0,0,3);
		if(fr1[nomer][7]==1) kudo=-1;
		else kudo=1;
		kudo=OBRAT*kudo;
		x13=x13-kuk*kudo;
		line(x13,y13-5,x13,y13-2);
		line(x13,y13+2,x13,y13+5);
	}
	//если открыта для движения
	else
	{
		if(otkuda==1)
		{
			setcolor(cvv);
			setlinestyle(0,0,3);
			if(fr1[nomer][7]==1) kudo=-1;
			else kudo=1;
			kudo=OBRAT*kudo;
			x13=x13-kuk*kudo;
			line(x13,y13-5,x13,y13-2);
			line(x13,y13+2,x13,y13+5);
		}
	}
	okrasklav(nomer);
  setlinestyle(line_style.linestyle,line_style.upattern,line_style.thickness);
	return;
}
//------------------------------------------------------
int test_mp()
{
  int aa,n_,ab,objekt;
	nom_func("342");
	n_=nach_marsh_oper;
	if((n_==0)||(end_marsh_oper==0))return(0);
	objekt=markery[modi][6];
	//n_=markery[n_m][6];  //получить номера объектов
	aa=fr1[objekt][4];//получить характеристику сигнала

	if(End_vara==2)//если выбирался вариантный маршрут
	{
		//если сигнал конца-совмещенный или принадлежит сквозному маршруту
		if((fr1[objekt][6]==2)||(fr1[objekt][4]==2))return(0);

		 //если маршрут-маневровый и сигнал конца маршрута-маневровый
		if((mane==1)&&(fr1[objekt][2]==0))return(0);

		// если маршут-поездной и сигнал конца-поездной
		if((mane==0)&&(fr1[objekt][2]==1))return(0);

		return(1);// в других случаях для варианта  отказ
	}
	// если выбирался не вариантный маршрут, начало от 2-х белых и нет конца 2-х белых
	if((mane==0)&&(fr1[n_][6]==3)&&( fr1[objekt][4]!=3))return(1);
	if((mane==0)&&(fr1[objekt][6]==0))return(1);//маршрут-поездной,конец-маневровый
//  if((mane==1)&&(fr1[objekt][6]==1))return(1);//маршрут-маневровый,конец-поездной
	// если начало от поездного или совмещенного
	if(fr1[n_][6]>0)//если начало совмещенный или с двумя белыми
	{ if(fr1[objekt][4]==fr1[n_][6])return(0);//если конец соответствует началу

		// если стоим на вариантной точке
		if(((fr1[objekt][4]==10)||(fr1[objekt][4]==11))&&(End_vara==0))return(0);
		if((mane==1)&&(aa==0))return(0);//если маневровый маршрут и вариантов нет
		if((mane==0)&&(aa==1)&&(manevro==0))return(0);//если поездной и сигнал у стрелки в пути
		if((mane==1)&&(aa==1)&&(manevro==0))return(2); //если маневровый и сигнал у стрелки в пути
		if((mane==1)&&(aa==1)&&(manevro==1))return(0);//если маневровый сигнал и маневровый маршрут и сигнал у стрелки в пути
	}
	else // если начало от маневрового
	{
		if(fr1[objekt][6]==2) return(0);//если конец совмещенный
		if((fr1[n_][5]==objekt)&&(End_vara==0)) return(0); //если вариант куда надо
		if(aa==1)
		{
			ab=fr1[objekt][2];//получить признак сигнала конца
			// если конец поездной
			// если маршрут маневровый,начало маневровый и второй проход
			// если первый вход
			if(ab==1)
			{ if((mane==1)&&(manevro==1)&&(siv==1)){siv=0;return(0);}
				if(siv==0){siv=1;return(2);}
			}
		}
		if((manevro==1)&&(aa==0))return(0);

		if((manevro==0)&&(aa==1)&&( mane==0))return(0);

		if((aa==0)&&(mane==1))return(0);
	}
	return(0);
}
//-----------------------------------------------------------------
/***************************************\
* Процедура организации реакции системы *
* на выбор оператором сигнала как в раз-*
* дельном, так и в маршрутном режиме.   *
\***************************************/
void make_signal()
{ int mp;
	nom_func("161");
	if(maket==1)
	{
		return; //это не стрелка
	}
	mp=markery[modi][6];
	if(fr3[mp][12]!=0){w(263,mp,"");return;}
   if(DISK!=0)return;
#ifdef AVTOD
	mp=prov_si_avtod(markery[modi][6],0);
	if(mp==1){w(245,999,"");return;} //"Включено автодействие по 1П"
  if(mp==2){w(244,999,"");return;} //"Включено автодействие по 2П"
#endif
	//если стойка выполняет предыдущую команду
#ifndef NEW_FORMAT
	if(komanda2[fr1[markery[modi][6]][13]-1]!=0)
	{w(51,999,"");return;} //"еще не выполнена предыдущая команда
#endif
	//если выбранный сигнал является вторым
	if(vozvr==2)
	{
		if(End_vara==0)
		{ //если может быть вариант
			if((markery[modi][7]==7)||(Zdem_vibor_marshruta==0))vopr_mar();
			if(Zdem_vibor_marshruta==1)
			//переместить курсор к меню
			{ regs.x.ax=4;regs.x.cx=630; regs.x.dx=50; int86(0x33,&regs,&regs);
				menu_N=5;
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
		}
		//если нажата вариантная точка
		if((fr1[end_marsh_oper][1]==2)&&(nach0_marsh_oper==0))
		{
			nach0_marsh_oper=markery[i3][6];
			End_vara=2;
			vozvr=2;
			Zdem_vibor_marshruta=0;
			return;
		}
		end_marsh_oper=markery[i3][6];
		//если нет соответствия конца,начала и категории маршрута
		if(test_mp()!=0)
		{ w(134,999,"");//ТОЧКА КОНЦА МАРШРУТА УКАЗАНА НЕВЕРНО
			home(modi);//вернуть курсор
			soob_for_oper=1; sbros=1;
			whole_finish(0);
			return;
		}
		End_vara=0; povt2=1;
		STACK=_SP;
		marsh1(0);//подготовка операторской трассы
    STACK=_SP;
		return;
	}
	if(uprav==0)//если раздельное управление
	{ if(markery[modi][7]==7)return;//для вариантной точки
		if(vrem==0)
		{ point=markery[modi][6];
			if((fr1[point][6]!=1)&&(puti==0))//если не "поездной" и первый вход
			{ //если не чисто маневровый и закрыт
				if((fr1[point][6]>0)&&(fr3[point][0]==0)&&(fr3[point][1]==0)&&
				(fr3[point][2]==0)&&(fr3[point][3]==0))
				{ menu_N=1;//предложить выбор "поездной/маневровый"
					punkt_N=1;//предварительно выбрать отмену
					klaval=-1;
					//переместить курсор к меню
					regs.x.ax=4;//переместить курсор к меню
					regs.x.cx=590;
					regs.x.dx=50;
					int86(0x33,&regs,&regs);
					X_m=590;Y_m=50;
					obnov_kur_mouse(590,50);
					menu1();
					zapretvybora=0;vrem=1;
					return;
				}
				else//если маневровый
				{ if(fr1[point][6]==0)mane=1;
					else
						if(fr1[point][6]==1)mane=0;
						else
							if((fr3[point][0]==1)||(fr3[point][2]==1))mane=1;
							else
								if((fr3[point][1]==1)||(fr3[point][3]==1))mane=0;
					vrem=1; puti=1; zapretvybora=1;
          otkrs();
          vrem=0;
        }
      }
      else//если поездной
      { mane=0; vrem=1; puti=1; zapretvybora=1;
				otkrs();
				vrem=0;
			}
		}
		else return;
	}
	else//если маршрутное управление
	{
		end_marsh_oper=markery[modi][6];//назначить выбранный сигнал концом маршрута
		//если вариантная точка
		if((fr1[end_marsh_oper][0]==2)&&(fr1[end_marsh_oper][1]==2))
		{
			if(nach_marsh_oper==0)//если не было начала маршрута
			{
				end_marsh_oper=0;
				return;

			}
		}
		else mane=0;
		if(fr1[end_marsh_oper][6]!=1)//если не "поездной"
		{ if(fr1[end_marsh_oper][6]>0)//если конец не чисто маневровый
			{ //и выбраны первый или второй, то признак маневра скопировать
				if((povt2==1)||(vozvr==2))manevro=mane;
				else//иначе
				{ if(markery[modi][7]==7){home(modi);return;}
					point=end_marsh_oper;//запомнить точку выбранного сигнала
					po_ma=1;//назначить признак совмещенного сигнала
					menu_N=1;//предложить выбор "поездной/маневровый"
					punkt_N=1;  klaval=-1;
					//переместить курсор к меню
					regs.x.ax=4;//переместить курсор к меню
					regs.x.cx=590;
					regs.x.dx=50;
					int86(0x33,&regs,&regs);
					X_m=590;Y_m=50;
					obnov_kur_mouse(590,50);
					regs.x.ax=7;//задать границы для мыши по горизонтали
					regs.x.cx=590;
					regs.x.dx=630;
					int86(0x33,&regs,&regs);
					regs.x.ax=8;//задать границы для мыши по вертикали
					regs.x.cx=5;
					regs.x.dx=40;
					int86(0x33,&regs,&regs);
					menu1();
					zapretvybora=0;
					return;
				}
			}
			else mane=1;
		}
		else po_ma=0;//если ранее был признак "поездной/маневровый"

		vozvr=2;
		mp=test_mp();//проверить соответствие выбранной категории для сигналов ограничивающих
		if((povt2==1)&&(mp!=0))//если выбирается конец (а начало уже выбрано)
		{ w(134,999,"");//ТОЧКА КОНЦА МАРШРУТА УКАЗАНА НЕВЕРНО
			//вернуться в исходное
			home(modi);
			soob_for_oper=1;
			sbros=1;
			whole_finish(0);
			return;
		}
		//проверить возможность установки маршрута
		if((nach_marsh_oper==0)||(end_marsh_oper==0))koni();
		//если конец маршрута не указан-затребовать указание конца
	}

	return;
}
//-------------------------------------------------------------------------
/******************************************\
* Процедура управления закрытием-открытием *
* пути для движения                        *
\******************************************/
void make_put()
{ int nomer=0;
	int avtr=0;
	nom_func("159");
	if(DISK!=0)return;
#ifdef AVTOD
	if(maket==1)
	{
		return; //это не стрелка
	}
  avtr=prov_ele_avtod(nomer);
  if(avtr==1){w(245,999,"");return;} //"Включено автодействие по 1П"
  if(avtr==2){w(244,999,"");return;} //"Включено автодействие по 2П"
#endif
  if(uprav==1)poka=0;
  if(zakr==0)//если первое нажатие
  { puti=1;nomer=markery[modi][6];
    if(fr4[nomer][2]==0)vopros(3,nomer,68,"");//"ЗАКРЫТЬ ПУТЬ ДЛЯ ДВИЖЕНИЯ?"
    else vopros(3,nomer,69,"");               //"ОТКРЫТЬ ПУТЬ ДЛЯ ДВИЖЕНИЯ?"
    zapretvybora=1;
    zakr=1;
  }
  else
	if(zakr==1)//если получено подтверждение нажатия
  {
    zakroy(modi);
    zapretvybora=0;
    zakr=0;
    puti=0;
  }
    return;
}
/******************************************************************/
void zakroy(int nn)
{
  int zoz=0,zo=0,xc,yc,nomer,BUK=0xC0,BUKn=0xCf;
	nom_func("427");
	if(DISK!=0)return;
  if(klaval==13)
  //НЕВОЗМОЖНА ВЫДАЧА КОМАНДЫ С РЕЗЕРВНОЙ ПЭВМ
  { if((STATUS==0)||(STATUS==2)){w(140,999,"");goto end;}
    if(TELEUP!=1)goto end;
    t(0);nomer=markery[nn][6];
#ifdef AVTOD
    if(fr4[nomer][2]==0)
    { if(na==1)
      { for(zo=0;zo<2;zo++)for(zoz=0;zoz<K_EL;zoz++)
        //ВКЛЮЧЕНО АВТОДЕЙСТВИЯ ПО 1П
        if(nomer==Avto_el[zo].Elems[zoz]){w(91,999,"");goto end;}
      }
      if(cha==1)
      { for(zo=2;zo<4;zo++)for(zoz=0;zoz<K_EL;zoz++)
        //ВКЛЮЧЕНО АВТОДЕЙСТВИЕ ПО 2П
        if(nomer==Avto_el[zo].Elems[zoz]){w(90,999,"");goto end;}
      }
    }
#endif
    if(fr1[nomer][8]>=1010){xc=markery[nn][4];yc=markery[nn][5];}
    moveto(xc,yc);
    if(fr4[nomer][2]==0)
    { fr4[nomer][2]=1;
      setcolor(LIGHTRED);
      //settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
      outtextxy(xc,yc,pako[nomer]);
      w(115,nomer,"");//ДВИЖЕНИЕ ЗАПРЕЩЕНО ПУТЬ N#
      if(fr4[nomer][0]==1)BUK=BUK|0x01;
      if(fr4[nomer][1]==1)BUK=BUK|0x02;
      BUK=BUK|0x04;
    }
    else
    { fr4[nomer][2]=0;
      setcolor(8);
      //settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
      outtextxy(xc,yc,pako[nomer]);
      w(114,nomer,"");//ДВИЖЕНИЕ РАЗРЕШЕНО ПУТЬ N#
      if(fr4[nomer][0]==0)BUKn=BUKn&0xCe;
      if(fr4[nomer][1]==0)BUKn=BUKn&0xCd;
      BUK=BUKn&0xCb;
    }
    disco();
    form_kom_put(nomer,BUK);
    add(0,0,'<');
  }
  else return;
end:
  home(nn);
  return;
}
//-----------------------------------------------------------------------
/*****************************************\
* Процедура организации выдачи команд при *
* обычной смене направления               *
\*****************************************/
void make_smena()
{
	nom_func("162");
	if(DISK!=0)return;
	if(uprav==1)poka=0;
	if(oppp==0)smen_napr(fr1[markery[modi][6]][9]);
	else smena(markery[modi][6]);
	return;
}
//-------------------------------------------------------------------
/*********************************\
* Процедура активизации процесса  *
* искусственной разделки          *
\*********************************/
void make_razdelka()
{
	nom_func("160");
	if(DISK!=0)return;

	// для резервной машины нет выполнения
	if(STATUS!=1){w(140,999,"");soob_for_oper=1;return;}
	if(TELEUP!=1)return;//при отключенном телеуправлении нет выполнения
	if((fr3[GRI][2]==1)&&(fr1[point][3]==9999))//если вкл.выдержка и объект = СП
	{ //если включена выдержка на ИР
		if(fr3[GRI][2]==1)w(15,999,"");//КОМПЛЕКТ ИСК.РАЗМЫКАНИЯ ЗАНЯТ
		zalet=0;//сбросить флаг выдачи команды ИР
		was[(markery[i3][7]&0xfff)-10]=0;//обнулить для этого объекта ячейку разделки
		fr3[point][9]=0;//сбросить элемент памяти для объекта
		vtora=0; //сбросить признак ожидания выполнения предварительной команды
		home(modi);//курсор домой
		return;
	}
	if(uprav==1) poka=0;
	if((all_null(i3)==1)&&(vtora==0))
	{ w(51,999,"");//ЕЩЕ НЕ ВЫПОЛНЕНА ПРЕДЫДУЩАЯ КОМАНДА
		home(modi);return;
	}
	//если вопрос не задавался, данный объект имеет разделку и она не начиналась
	if((zalet==0)&&(nashel(modi)!=1)&&(fr3[point][0]==0)&&(fr3[point][2]==0))
	{ //задать вопрос о размыкании
		w(fr1[point][6],fr1[point][10],"?");//ИСКУССТВЕННОЕ РАЗМЫКАНИЕ ПО
		was[(markery[i3][7]&0xfff)-10]=i3;//зафиксировать объект
    zalet=1; //установить признак задания вопроса
    zapretvybora=1; //зафиксировать курсоры
    return;
  }
  else
  {
    zalet=0;
		was[(markery[i3][7]&0xfff)-10]=0;
    fr3[markery[i3][6]][9]=0;
    vtora=0;home(i3);return;
  }
}
//---------------------------------------------------------
/********************************\
* Процедура инициализации выдачи *
* ответственной команды          *
\********************************/
void make_otvetst()
{
	int isp,pred;//номера объекта исполнительного
	int bit_p,bit_i;//номера битов предварительной и исполнительной команд
	int cod_rele; //код состояния реле
	nom_func("156");
	if(DISK!=0)return;//при просмотре архива нет выполнения
	// для резервной машины нет выполнения
	if(STATUS!=1){w(140,999,"");soob_for_oper=1;return;}
	if(TELEUP!=1)return;//при отключенном телеуправлении нет выполнения
	pred=fr1[point][2];isp=fr1[point][3];
	bit_p=(fr1[point][4]&0xff00)>>8;
	bit_i=fr1[point][4]&0xff;
	if(uprav==1) poka=0;
	if(fr3[pred][5]==1)
	{
		w(272,pred," НЕ ОПРЕДЕЛЕННО");
		home(modi);return;
	}
	if(fr3[isp][5]==1)
	{
		w(272,isp," НЕ ОПРЕДЕЛЕННО");
		home(modi);return;
	}
	cod_rele=fr3[pred][bit_p]+fr3[isp][bit_i]*2;
	switch(cod_rele)
	{
		case 0:		if(vtora==0)
							{ w(229,999,"");//ОБЪЕКТ УПРАВЛЕНИЯ УЖЕ В ТРЕБУЕМОМ СОСТОЯНИИ
								home(modi);
								return;
							}
		case 2:	if(((fr3[pred][bit_p]+fr3[isp][bit_i])==1)&&(vtora==0))
						{ w(51,999,"");//ЕЩЕ НЕ ВЫПОЛНЕНА ПРЕДЫДУЩАЯ КОМАНДА
							home(modi);return;
						}
						else break;
		case 1: w(169,pred,"");break;
		default: break;
	}
	//если вопрос не задавался, данный объект имеет разделку и она не начиналась
	if((zalet==0)&&(fr3[pred][bit_p]==fr3[isp][bit_i]))
	{ //задать вопрос о размыкании
		w(fr1[point][5],999,"?");//отключить(включить) объект ?
		zapusk=1; //установить признак задания вопроса
		zapretvybora=1; //зафиксировать курсоры
		return;
	}
	else
	{ //если вопрос задавался
		zapusk=0;vtora=0;home(i3);return;
	}
}
//-------------------------------------------------------------------------
/********************************************************\
* Процедура подготовки и выдачи неответственных команд   *
* для ответственных объектов (напр. "закрыть переезд"    *
\********************************************************/
void make_neotv()
{
	nom_func("154");
	if(DISK!=0)return;
  poka=0;
  if(oppp==0)
  {
    vkl_neot(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    neot(markery[modi][6]);
    zapretvybora=0;
  }
	return;
}
//-------------------------------------------------------
/*******************************************************\
* Процедура активизации неответственного действия для   *
* ответственного объекта. (Пример: закрытие переезда)   *
* объект 2,30 из пары 2,30 - 2,31                       *
\*******************************************************/
void vkl_neot(int nom)
{
	nom_func("399");
	if(fr1[nom][6]!=9999)//если есть текст
	{
		setfillstyle(1,14),setcolor(4);
		vopros(1,9999,fr1[nom][5],"");
	}
	oppp=1;puti=1;return;
}
//------------------------------------------------------------------------
/********************************************\
* Процедура выдачи неответственных команд на *
* объект ответственных команд.               *
\********************************************/
void neot(int nom)
{
	unsigned int koda,n_bait;
	int isp,bit_isp,pred,bit_pred,cod;
	nom_func("184");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(klaval==13)
	{
		isp=fr1[nom][3];pred=fr1[nom][2];
		bit_pred=(fr1[nom][4]&0xff00)>>8;
		bit_isp=fr1[nom][4]&0xff;
		cod=fr3[isp][bit_isp]*2+fr3[pred][bit_pred];
		switch(cod)
		{
				case 0:
				case 2:	w(fr1[pred][6],999,"");
								point=pred;
								break;

				case 1: w(fr1[pred][6],999,"");
								point=isp;
								break;

				case 3:	w(229,nom,"!"); //"Объект управления уже в требуемом состоянии"
								break;


		}
		if(cod!=3)
		{
			koda='N';
			if(form_kosig(koda)==1)
			{
				//если формирование команды не удалось - выход
				if(form_kodop(koda)==1)return;
			}
			buf_ko[n_bait-2]=check_sum(buf_ko,1);//добавить контрольную сумму
			pooo_com[nom]=biostime(0,0L);//зафиксировать время выдачи
			com_bit[nom]=koda&0xF;
			flagoutmsg=1;//установить флаг-требование на передачу в ТУМС
		}
out:
		oppp=0;puti=0;
		home(modi);
		return;
	}
	else return;
}
//------------------------------------------------------------------
/********************************************\
* Процедура активизации управления объектом  *
* типа "переезд"(тип маркера = 333)          *
* Для старых вариантов - выдача двух команд  *
* подряд (исполнительная - без оператора)    *
\********************************************/
void make_pereezd()
{
	nom_func("158");
	if(DISK!=0)return;
	poka=0;
  if(oppp==0)
  {
    kom_per(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    k_per(markery[modi][6]);
    zapretvybora=0;
  }
	return;
}
/**********************************************/
void vkl_kno(int nom,int CCV)
{
	int to=0,too=0,D=0;
	int xc,yc;
	char klopo[6]="";
	nom_func("396");
	if((nikuda==1)||(klo==1)||(help>0))return;
	for(to=0;to<6;to++)klopo[to]=0;
	xc=markery[nom][4]-4;
	yc=markery[nom][5]-4;
	setfillstyle(SOLID_FILL,CCV);
	if((pusto&3)!=0)setfillstyle(SOLID_FILL,LIGHTCYAN);
#ifdef MAKET
	if(nom==MAKET)
	{
		setfillstyle(LTBKSLASH_FILL,CCV);
		bar(xc-17,yc,xc+25,yc+10);
		for(to=0;to<kol_OSN;to++)
		if((fr4[to][1]==1)&&(fr1[to][0]==1))
		{
			//settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
			setcolor(WHITE);
			too=2;
			while(pako[to][too]!=':')
			{
				klopo[too-2]=pako[to][too];
				too++;
			}
			D=strlen(klopo);
			D=D*4;
			outtextxy(xc+5-D,yc+2,klopo);
			break;
		}
	}
	else
#endif
	{
		if((markery[nom][6]>=9999)||(markery[nom][7]>776))
		{
			setfillstyle(SOLID_FILL,CCV);
#ifndef NEW_KNOPKA
			bar(xc,yc,xc+8,yc+8);
#else
			xc=markery[nom][4];
			yc=markery[nom][5];
			D=textwidth(name_lamp[markery[nom][6]-9999]);
			if(CCV==8)CCV=7;
			setfillstyle(SOLID_FILL,CCV);
			if((getpixel(xc+1,yc)!=CCV)||(cikl_obnov==0))
			{
				bar(xc-1,yc-1,xc+D+1,yc+10);
				setcolor(8);
				outtextxy(xc,yc+1,name_lamp[markery[nom][6]-9999]);
				setlinestyle(0,0,0);
				rectangle(xc-1,yc-1,xc+D+1,yc+10);
			}
			else
			{
				obnovi_obraz(xc,yc-1,D,name_lamp[markery[nom][6]-9999],CCV);
			}
		}
		else
		if((markery[nom][7]==777)||(markery[nom][7]==778)) //кнопка НАС или ЧАС
		{
			xc=markery[nom][4];	yc=markery[nom][5];
			D=textwidth("НАС");
			if(CCV==8)CCV=7;
			setfillstyle(SOLID_FILL,CCV);
			bar(xc-1,yc-1,xc+D+1,yc+10);
			setcolor(8);
			if(markery[nom][7]==777)outtextxy(xc,yc+1,"НАС");
			else outtextxy(xc,yc+1,"ЧАС");
			setlinestyle(0,0,0);
			rectangle(xc-1,yc-1,xc+D+1,yc+10);

#endif
		}
	}
	return;
}
//-------------------------------------------------------------------
/*****************************************************\
* Процедура управления включением-выключением объекта *
* кнопка + лампочка                                   *
\*****************************************************/
void make_knopka()
{
	nom_func("151");
	if(DISK!=0)return;
  poka=0;
  if(oppp==0) { vkl_knop(markery[modi][6]); zapretvybora=1; }
  else { knopa(markery[modi][6]); zapretvybora=0;}
	return;
}
//--------------------------------------------------------------------
/*******************************************************\
* Процедура управления включением-выключением объекта   *
* "одна кнопка - два объекта". Один объект - для включе-*
* ния. Второй объект для отключения. (тип 2,166)        *
\*******************************************************/
void make_knpk2()
{
	nom_func("152");
	if(DISK!=0)return;
  poka=0;
  if(oppp==0) { vkl_knop(markery[modi][6]); zapretvybora=1; }
  else { knopa(markery[modi][6]); zapretvybora=0;}
	return;
}
//------------------------------------------------------------------
/*****************************************************\
* Процедура управления включением-отключением объекта *
* управления немаршрутизированными перемещениями      *
* объекты типа 2,39                                   *
\*****************************************************/
void make_nemarsh()
{
	nom_func("153");
	if(DISK!=0)return;
  poka=0;
  if(oppp==0)
  {
    vkl_nemarsh(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    nemarsh(markery[modi][6]);
    zapretvybora=0;
  }
	return;
}
//-------------------------------------------------------------------------
/**********************************************************\
* Процедура управления переключением режимов день/ночь     *
* при нажатиях кнопки "день/ночь"                          *
\**********************************************************/
void make_dn()
{
	nom_func("149");
	if(DISK!=0)return;
  if (uprav==1) poka=0;
  if(oppp==0)//если первое нажатие
  {
    vkl_dn(markery[modi][6]);
    zapretvybora=1;
  }
  else //если второе нажатие
  {
    dn_au(markery[modi][6]);
    zapretvybora=0;
  }
	return;
}
//-----------------------------------------------------------------------
/***********************************************\
* Процедура включения-выключения объекта типа   *
* оповещение (2,17)                             *
\***********************************************/
void make_opoveshenie()
{
	nom_func("155");
	if(DISK!=0)return;
  if (uprav==1) poka=0;
  if (oppp==0)
  {
    vkl_opoveshe(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    opoveshe(markery[modi][6],0);
    zapretvybora=0;
  }
	return;
}
//------------------------------------------------------------------
/****************************************\
* Процедура активизации переключения     *
* комплектов МПСУ ТУМС (тип маркера=115) *
\****************************************/
void make_per_kompl()
{
	nom_func("157");
	if(DISK!=0)return;
  if (oppp==0)
  {
    perekl_komp(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    prkl(markery[modi][6]);
    zapretvybora=0;
  }
	return;
}
//--------------------------------------------
#ifdef OGRAD
make_ograd()
{
  if(DISK!=0)return;
  if(oppp==0)
  {
    kom_ograd(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    k_ograd(markery[modi][6]);
    zapretvybora=0;
	}
	return;
}
#endif
//--------------------------------------------------------------------------
/******************************************************\
* Процедура управления при нажатии кнопок разделки     *
* номер объекта, на котором находится система в point  *
\******************************************************/
void make_gri()
{
  int nn1,nn2;
	nom_func("150");
	if(DISK!=0)return;
  if(fr1[point][1]==301)
  {
    nn1=point;
    nn2=fr1[point][3];
    if((fr3[nn1][2]!=0)||(fr3[nn2][2]!=0))
		{ w(15,999,"");  return; } //"комплект иск.размык. занят"
  }
  if(gri==0)
  {
    if(uprav==1)poka=0;
    if(fr3[point][2]==0)
		{ w(20,999,""); //включить выдержку времени ?
      zapretvybora=1; gri=1; vih_vopr=1;
      return;
    }
    else w(15,999,""); //комплект искусственного размыкания занят
  }
  home(modi);
  zapretvybora=0;
  gri=0;
  return;
}
#ifdef POLUAVTOMAT
//----------------------------------------------------------------------
/*********************************************************\
* Процедура управления дачей согласия на отправление для  *
* перегона,оборудованного полуавтоматикой                 *
\*********************************************************/
void make_dso()
{
  if(DISK!=0)return;
  if(oppp==0)
  {
    kom_dso(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    k_dso(markery[modi][6]);
    zapretvybora=0;
  }
  return;
}
//---------------------------------------------------------
/*********************************************************\
* Процедура управления дачей фактического прибытия   для  *
* перегона,оборудованного полуавтоматикой                 *
\*********************************************************/
void make_fdp()
{
  if(DISK!=0)return;
  if(oppp==0)
  {
    kom_fdp(markery[modi][6]);
    zapretvybora=1;
  }
  else
  {
    k_fdp(markery[modi][6]);
    zapretvybora=0;
  }
  return;
}
#endif
/********************************************/
void ris_ogr_str(int nbn,int BUKv)
{ //процедура изображения на имени ограничений наложенных на стрелку
  int x2_3,y2_3,in,im,len_str,sp;
	char im_str[5];
	nom_func("286");
	in=0;im=0;
  while(pako[nbn][in]!=':')in++;//сместиться на имя стрелки
  in++;
  for(in=in;in<strlen(pako[nbn]);in++)im_str[im++]=pako[nbn][in];//взять имя
  im_str[im]=0;len_str=strlen(im_str);//получить длину имени
  x2_3=fr2[nbn][0];y2_3=fr2[nbn][1];//взять координаты вершины стрелки
  switch(fr1[nbn][9]) //переключение по направлению ветвления на экране
  { //ветвление вверх
    case 1: if(fr1[nbn][7]==0) // если вход четный
						{
#if(OBRAT==-1)
							goto ob2;
#endif
							//получить смещение для имени по X и по Y
ob1:          x2_3=x2_3-(len_str*7)-1; y2_3=y2_3-12;
						}
						else
						{ // вход нечетный
#if(OBRAT==-1)
							goto ob1;
#endif
ob2:          x2_3=x2_3-1;y2_3=y2_3-12;
						}
						break;
		// ветвление вниз
		case 0: if(fr1[nbn][7]==1) // нечет
						{
#if(OBRAT==-1)
							goto ob4;
#endif
ob3:          x2_3=x2_3-1;y2_3=y2_3+4;
						}
						else
						{
#if(OBRAT==-1)
							goto ob3;
#endif
ob4:
							x2_3=x2_3-(len_str*7);y2_3=y2_3+4;
						}
						break;
	default: y2_3=y2_3;break;
	}
	if(fr2[nbn][0]==0)return;
	switch(BUKv)//переключатель по наложенным ограничениям
	{
		case 1: setcolor(14);setlinestyle(0,0,0);//отключена от управления
						rectangle(x2_3,y2_3,x2_3+len_str*7+1,y2_3+8);//взять в рамку
						break;
		case 0x200:
						setcolor(12);
						setlinestyle(0,0,0);// на макете
						rectangle(x2_3,y2_3,x2_3+len_str*7+1,y2_3+8);//взять в рамку
						break;
		case 0x201:
						setcolor(12);setlinestyle(0,0,0);// отключена от управления и на макете
						line(x2_3,y2_3,x2_3+len_str*4,y2_3);//рамка - 2-х цветная
						line(x2_3+len_str*7+1,y2_3,x2_3+len_str*7+1,y2_3+4);
						line(x2_3+len_str*4,y2_3+8,x2_3+len_str*7+1,y2_3+8);
						line(x2_3,y2_3+4,x2_3,y2_3+8);
            setcolor(14);
            line(x2_3+len_str*4,y2_3,x2_3+len_str*7+1,y2_3);
            line(x2_3+len_str*7+1,y2_3+4,x2_3+len_str*7+1,y2_3+8);
            line(x2_3,y2_3+8,x2_3+len_str*4,y2_3+8);
            line(x2_3,y2_3,x2_3,y2_3+4);
            break;
   case 0:  setcolor(7);// нет ограничений
            setlinestyle(0,0,0);
            rectangle(x2_3,y2_3,x2_3+len_str*7+1,y2_3+8); //стереть рамку
            break;
   default: if((BUKv&0x67)==0)
            {
              if((fr1[nbn][11]&128)!=128)
              {
                sp=findspu1(nbn,fr1[nbn][7]);
                if((sp!=-1)&&(fr3[sp][1]==1))setcolor(7);
                else setcolor(8);
              }
              else setcolor(8);
            }
            else setcolor(8);
            setlinestyle(0,0,0);//отключена от управления
            rectangle(x2_3,y2_3,x2_3+len_str*7+1,y2_3+8);//взять в рамку
            break;
  }
	return;
}
//---------------------------------------------------------------------
/******************************************************\
* Процедура восстановления имени стрелок на экране при *
* установке и снятии ограничений.                      *
\******************************************************/
void okrasklav(int irs) /* одного из всех кнопок */
{
	int xz,zzc=9999;
	nom_func("200");
	//работает только для основного экрана
	if((klo==1)||(help!=0)||(nikuda==1)) return;

	for(xz=0;xz<kol_OB_UPR;xz++)//пройти по всем объектам управления

	if(irs==markery[xz][6])// если найдено управление для заданной стрелки
	{
		zzc=xz;//взять номер объекта управления для заданной стрелки
		break;
	}

	if(zzc==9999) return;//если нет управления для стрелки - выход

	imena(zzc);//нарисовать имя для объекта

	if(fr1[irs][10]==11)//если стрелка с автовозвратом
	{
		for(xz=0;xz<kol_OB_UPR;xz++)//пройти по объектам управления

		if((irs+9999)==markery[xz][6]) //если виртуальная стрелка
		{
			zzc=xz;//взять для нее номер объекта управления
			break;
		}
		if(zzc==9999) return;//если нет управления-выход
		imena(zzc);//нарисовать имя
	}
	return;
}
//----------------------------------------------------------
void set_timers()
{
	unsigned char vvod[2]={0,0};
	struct dostime_t tt;
	nom_func("299");
	if(DISK!=0)return;
  if(timer_set!=15)
  {
begin:
    setfillstyle(1,2);setcolor(15);bar(480,1,640,47);rectangle(481,2,639,46);
    outtextxy(494,4,"УСТАНОВКА ВРЕМЕНИ");
    outtextxy(484,16,"ЧАСЫ МИНУТЫ СЕКУНДЫ");
    outtextxy(600,30,"00");
    setfillstyle(1,7);bar(492,30,512,40);bar(540,30,560,40);
    outtextxy(493,30,"_");
    H_1=0xFF;H_0=0xFF;M_1=0xFF;M_0=0xFF;
    return;
  }
  else
  {
    vvod[0]=getch();
    if(vvod[0]==32)
    {
      timer_set=0;
      t(0);
      X_time=0;
      return;
    }
    if(vvod[0]==13)
    { if(((H_1*10+H_0)>23)||((M_1*10+M_0)>59))
      {
        if((H_1==0xFF)&&(H_0==0xFF)&&(M_1==0xFF)&&(M_0==0xFF))
        {
          t(0);
          X_time=0;
          timer_set=0;
          return;
        }
        putch(7);
        timer_set=15;
        t(0);
        X_time=0;
        goto begin;
      }
      else
      {
        rg.h.ch=H_1<<4;
        rg.h.ch=H_1|H_0;
        rg.h.cl=M_1<<4;
        rg.h.cl=rg.h.cl|M_0;
        rg.h.dh=0;
        rg.h.ah=0x2;
        int86(0x1A,&rg,&rg);
        tt.hour=H_1*10+H_0;
        tt.minute=M_1*10+M_0;
        tt.hsecond=0;
        tt.second=0;
        _dos_settime(&tt);
				add(0,0,'$');
        Init_TEST_SOOB(0);
#ifdef KOL_SOO2
        Init_TEST_SOOB(1);
#endif
        SEC_time=biostime(0,0L);
        FIR_time=SEC_time;
        second_time=FIR_time;
        first_time=second_time;
        //-TIME_FIX;
        X_time=0;
        timer_set=0;
        t(0);
        return;
      }
    }
    switch(X_time)
    {
      case 0: X_time=493;break;
      case 493: X_time=502;break;
      case 502: X_time=541;break;
      case 541: X_time=550;break;
      case 550: X_time=493;break;
    }

    if((vvod[0]>57)||(vvod[0]<48)){ putch(7); return;}
    else
    {
      setcolor(7);
      outtextxy(X_time,31,"█");
      setcolor(15);
      outtextxy(X_time,31,vvod);
      if(X_time==493)H_1=vvod[0]-48;
      if(X_time==502)H_0=vvod[0]-48;
      if(X_time==541)M_1=vvod[0]-48;
      if(X_time==550)M_0=vvod[0]-48;
      if(X_time==502)outtextxy(541,31,"_");
      if((X_time==493)||(X_time==541))outtextxy(X_time+9,31,"_");
    }
    if(vvod[0]==0)vvod[0]=getch();
  }
	return;
}
//-------------------------------------
#ifndef WORK
void set_vvod()
{
	unsigned char vvod[4];
	int OB,BIT,ZNACH,zapret;

	nom_func("300");
	if(DISK!=0)return;
begin:
	setfillstyle(1,2);setcolor(15);bar(480,1,640,47);rectangle(481,2,639,46);
	outtextxy(494,4,"УСТАНОВКА ДАННЫХ");
	outtextxy(484,16,"ОБЪЕКТ БИТ ЗНАЧЕНИЕ");
	gotoxy(30,2);
	setfillstyle(1,7);bar(492,30,522,40);bar(540,30,560,40);bar(580,30,600,40);
	gets(vvod);
	outtextxy(493,31,vvod);OB=atoi(vvod);
	if(OB>=kol_VO)goto begin;
	gets(vvod);outtextxy(541,31,vvod);BIT=atoi(vvod);
	if(BIT>11)goto begin;
	gets(vvod);outtextxy(581,31,vvod);ZNACH=atoi(vvod);
	if(ZNACH>1)goto begin;
	fr3[OB][BIT]=ZNACH;
	vvod_set=0;
	if(fr1[OB][0]==1)displaystrelka(OB,BIT,0);
	if(fr1[OB][0]==2)
	{
		if(fr1[OB][1]==11)kvv(OB,BIT);
		if(fr1[OB][1]==12)tel_vkl_vikl(OB);
		if(fr1[OB][1]==13)komplekt(OB,BIT);
		if(fr1[OB][1]==911)Pam(OB);
		if(fr1[OB][1]>1000)
		{
			if(fr1[OB][1]==9999)return;
			if(BIT<2)test_deshifr(OB,BIT);
			if((BIT>1)&&(BIT<5))dispdatch(OB,BIT);
		}
		else
			displaysignal(OB,BIT);
	}
	if(fr1[OB][0]==3)
	sekci(OB,BIT);
	if(fr1[OB][0]==4)displaypathuch(OB);
	if(fr1[OB][0]==5)displaypath(OB,BIT);
	zapretvybora=zapret_otlad;
	return;
}
//-----------------------------------------------------------------
/******************************************\
* Процедура прямого ввода сообщений ТУМСа  *
\******************************************/
void set_tums(void)
{
	char r_tums[11],a=0;
	int OB,i;

	nom_func("309");
	if(DISK!=0)return;
begin:
	for(i=0;i<11;i++)r_tums[i]=0;
	setfillstyle(1,2);setcolor(15);bar(480,1,640,47);rectangle(481,2,639,46);
	outtextxy(494,4,"СООБЩЕНИЕ ТУМС");
	outtextxy(484,16,"ТУМС  ЗНАЧЕНИЕ");
	gotoxy(30,2);

	//изображение окошек ввода
	setfillstyle(1,7);bar(492,30,522,40);bar(540,30,600,40);
	//ввод номера ТУМСа
	gets(r_tums);
	outtextxy(493,31,r_tums);
	OB=atoi(r_tums);
	if(OB>=skoko_stoek)goto begin;
	if(OB<0)goto begin;
	i=0;
	while(1)
	{
		a=getch();
		if(a==13)break;
		if(a==8)
		{
			if(i>0)
			{
				i--;
				r_tums[i]=0;
				goto txt;
			}
		}
		r_tums[i+1]=0;
		r_tums[i++]=a;
txt:
		bar(540,30,600,40);
		outtextxy(541,31,r_tums);
	}
/*
	gets(r_tums);
	outtextxy(541,31,r_tums);
*/
#ifdef NEW_FORMAT
	if(strlen(r_tums)>10)goto begin;
	r_tums[10]=check_sum(r_tums,1);
	r_tums[11]=')';
	for(i=0;i<12;i++)BUF_IN[OB][i]=r_tums[i];
	ANALIZ[OB]=1;
	konec[OB]=11;
#else
	if(strlen(r_tums)>9)goto begin;
	r_tums[9]=check_sum(r_tums);
	r_tums[10]=')';
	for(i=0;i<11;i++)BUF_IN[OB][i]=r_tums[i];
	ANALIZ[OB]=1;
	konec[OB]=10;
#endif
	vvod_tums=0;
	t(0);
	return;
}
//-----------------------------------------------------------------
/************************************\
* Процедура прямого ввода команд ДЦ  *
\************************************/
void set_dc(void)
{
	char r_dc[11],a=0;
	int OB,i;

	nom_func("309");
	if(DISK!=0)return;
#ifdef DISPETCHER
begin:
	setfillstyle(1,2);setcolor(15);bar(480,1,640,47);rectangle(481,2,639,46);
	outtextxy(494,4,"КОМАНДА ДЦ");
	gotoxy(30,2);

	//изображение окошек ввода
	setfillstyle(1,7);bar(540,30,600,40);
	i=0;
	while(1)
	{
		a=getch();
		if(a==13)break;
		if(a==8)
		{
			if(i>0)
			{
				i--;
				r_dc[i]=0;
				goto txt;
			}
		}
		r_dc[i+1]=0;
		r_dc[i++]=a;
txt:
		bar(540,30,600,40);
		outtextxy(541,31,r_dc);
	}
	if(strlen(r_dc)>10)goto begin;
	r_dc[10]=check_sum(r_dc,1);
	r_dc[11]=')';
	for(i=0;i<12;i++)BUF_IN_DC[i]=r_dc[i];
	ANALIZ_DC=1;
	konec[OB]=11;
	vvod_dc=0;
	t(0);
#endif
	return;
}

#endif

#ifdef VSP
//-----------------------------------------------------------------------
/*******************************************************\
* Процедура выдачи команд вспомогательного отправления  *
*                make_vspom_otpr()                      *
\*******************************************************/
void make_vspom_otpr()
{ if(DISK!=0)return;
	if(uprav==1) poka=0;
	//был только вопрос
	if((klaval==13)&&(zabeg==1)&&(fr3[point][9]==0)&&(vtora==0))
	{
		form_first_kom(modi);
		zabeg=2;
		zapretvybora=1;//выдать предварительную
		return;
	}
	else
	//была 1я команда и выполнена
	if((klaval==13)&&(zabeg==2)&&(fr3[point][9]==0)&&(vtora==1))
	{ w(233,999,"");//ВЫДАНА КОМАНДА ВСПОМ. ОТПРАВЛЕНИЯ
		form_second_kom(modi);
		zabeg=0;
		zapretvybora=0;
		fr3[point][9]=0;
		vtora=0;
		home(modi);
		return;
	}
	if((zabeg==0)&&(fr3[point][2]==0)&&(fr3[point][3]==0))//не было команды или не выполнена
	{ if(fr3[point][9]==9){home(modi);return;}//если была выдана первая - выход
		w(219,999,otopri[fr1[point][12]]);/* ВСПОМ. ОТПР.? */
		outtext("?");zabeg=1;zapretvybora=1;  return;
	}
	else
	if(zabeg!=0)
	{ if(fr3[point][9]==9)  return;
		if((zabeg==1)&&(fr3[point][9]==0)&&(vtora==1))
		{ if((fr3[point][2]==1)&&(fr3[point][3]==0))//если первая выполнена
			{ w(12,999,"");/* ПОСЫЛАТЬ ОКОНЧАТЕЛЬНУЮ КОМАНДУ? */
				zabeg=2;
				pooo_com[point]=biostime(0,0l);
				return;
			}
			else
			{zabeg=0;zapretvybora=0;fr3[point][9]=0;vtora=0;home(modi);return;}
		}
		else
		{zabeg=0;zapretvybora=0;fr3[point][9]=0;vtora=0;home(modi);return;}
	}
	else
	{zabeg=0;zapretvybora=0;fr3[point][9]=0;vtora=0;home(modi);return;}
}

//------------------------------------------
/*******************************************************\
* Процедура выдачи команд вспомогательного приема       *
*                make_vspom_priem()                     *
\*******************************************************/
void make_vspom_priem()
{
	if(DISK!=0)return;
	if(uprav==1)poka=0;
	if((klaval==13)&&(zabeg==1)&&(fr3[point][9]==0)&&(vtora==0))
	{
		form_first_kom(modi);
		zabeg=2;
		zapretvybora=1;
		return;
	}
	else
	if((klaval==13)&&(zabeg==2)&&(fr3[point][9]==0)&&(vtora==1))
	{
		w(232,999,"");//ВЫДАНА КОМАНДА ВСПОМ. ПРИЕМА
		form_second_kom(modi);zabeg=0;zapretvybora=0;fr3[point][9]=0;vtora=0;
		home(modi);
		return;
	}
	if((zabeg==0)&&(fr3[point][2]==0)&&(fr3[point][3]==0))
	{ if(fr3[point][2]!=0) return;
		if(fr3[point][9]==9){home(modi);return;}
		//ВСПОМОГАТЕЛЬНЫЙ ПРИЕМ
		w(218,999,otopri[fr1[point][11]]);outtext("?");zabeg=1;
		zapretvybora=1;return;
	}
  else
    if(zabeg!=0)
    { if(fr3[point][9]==9) return;
      if((zabeg==1)&&(fr3[point][9]==0)&&(vtora==1))
      { if((fr3[point][2]==0)&&(fr3[point][3]==1))
        //ПОСЫЛАТЬ ОКОНЧАТЕЛЬНУЮ КОМАНДУ?
				{zabeg=3;w(12,999,"");pooo_com[point]=biostime(0,0l);}
        else {zabeg=0;fr3[point][9]=0;vtora=0;home(modi);}
      }
      else {zabeg=0;fr3[point][9]=0;vtora=0;home(modi);}
    }
    else {zabeg=0;fr3[point][9]=0;vtora=0;home(modi);}
  return;
}
#endif
