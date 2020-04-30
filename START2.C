#include"bla.h"
void *fai;
//------------------------------------------------------------------------
/***************************************************************\
* Процедура полной прорисовки экрана по данным находящимся в    *
* памяти в массиве fr3,описаниям в базе станции fr1 и введенным *
* ограничениям fr4.                                             *
\***************************************************************/
void nach_zagruzka()
{
	int lo;
	modi=-1;pat=-1;i3=-1;
	nom_func("177");
	prorisovka=1;
	zagruzen();
	prorisovka=0;
	for(lo=0;lo<skoko_stoek;lo++){sboy_ts[lo]=0;otkaz_ts[lo]=0;}
	return;
}
//------------------------------------------------
void zagruzen()
{
	int i,lj,qi3;
	nom_func("426");
	setcolor(3);
	i3=-1;
	read_text1(0);
	for(qi3=0;qi3<kol_OB_UPR;qi3++)
	imena(qi3);
	pict1(1);
#ifdef KOL_SOO2
	pict1(2);
#endif
	for(lj=0;lj<kol_OB_UPR;lj++)
	if(markery[lj][7]==6||markery[lj][7]==7)init_markery(lj);
  setcolor(8);
  for(i=kol_OSN;i<kol_VO;i++)
  if(fr1[i][0]==2)
  {
    bata=0;
    displaysignal(i,0);
    displaysignal(i,1);
    displaysignal(i,2);
    displaysignal(i,3);
    displaysignal(i,4);
  }
  tel_vkl_vikl(TEL1);
#ifdef KOL_SOO2
  tel_vkl_vikl(TEL2);
#endif
  for(lj=Start_R;lj<Start_R+WAZO;lj++)
  {
    krasit_razd(lj,0);krasit_razd(lj,2);
  }
  krasit_knopki(Start_R+WAZO,2);
  knopka(KS1);
#ifdef KOL_SOO2
  knopka(KS2);
#endif
	vkl_kno(VSPZ,8);
#ifdef MAKET
	vkl_kno(MAKET,8);
#endif
  vkl_kno(AViA,8);
  if(ot==0)vkl_kno(0,8);
	else vkl_kno(0,14);
	if(uprav==0)
	{
		vkl_kno(1,LIGHTGREEN);
		vkl_kno(2,8);
		add(0,0,'Р');
	}
	else
	{
		vkl_kno(2,LIGHTGREEN);
		vkl_kno(1,8);
		add(0,0,'р');
	}
#ifdef AVTOD
  if(na==1)
  {
    vkl_kno(NANA,LIGHTGREEN);
    for(lj=0;lj<N_str;lj++)otkl_(N_strel[lj],1);
  }
  else vkl_kno(NANA,8);
  if(cha==1)
  {
    vkl_kno(CHACHA,LIGHTGREEN);
    for(lj=0;lj<CH_str;lj++)otkl_(C_strel[lj],0);
  }
  else  vkl_kno(CHACHA,8);
#endif
#ifdef kol_STY
  Styki();
#endif
  zapretvybora=0; i3=-1; pat=-1;
	if(DISK==0)knopka_OK();
  kvadrat();
	return;
}
/************************************************/
void out_k()
{
	nom_func("211");
// putimage(65,35,ukazat,COPY_PUT);
	return;
}
//----------------------------------------------------------------------
/*********************************************\
* Процедура вывода на экран имени указанного  *
* объекта, а также вывода подсветки курсора и *
* рамки, если объект выбран оператором        *
* ib - номер объекта управления в markery     *
\*********************************************/
void imena(int ib)
{
	int ix=0,iy=0,ele=0,ele1=0,ix1=0,iy1=0,lp,lp1,ijk,ijk1,n_str,n_str1,
	stroka_cvet,cvet_knop,cvet_knop1;
	char imya[20]="";
	int D=0,D1=0,NNBO,cod_bin=0;
	nom_func("103");

	//если выводится на экран помощь или сведения о ненормах СЦБ или УВК,
	//то ничего не делать
	if((klo==1)||(help!=0)||(nikuda==1))return;

	setcolor(8);//обычный серый шрифт
	NNBO=ib;//запомнить номер выбранного объекта управления
	if(markery[ib][7]==6)//если выбрана стрелка смены направления
	{
		ele=markery[ib][6];//получить объект управления
		ele=fr1[ele][9]; //объект для перегона
		lp1=fr1[ele][8];//сомножитель для смещения по Y
		lp=fr1[ele][9]; //получить обратную ссылку на объект управления
		if(fr1[lp][1]==0)
		{
#if OBRAT==1
			D=1;// если объект для приема с левой стороны
#else
			D=2;
#endif
		}
		else
		{
#if OBRAT==1
			D=2;
#else
			D=1;
#endif
		}
		// 1 - слева   2 - справа
		switch(D) // в зависимости от места расположения записать координаты
		{
			case 1: markery[ib][4]=left_up_left[0]+7;
							markery[ib][5]=left_up_left[1]+lp1*fr1[ele][4]+2;
							break;
			case 2: markery[ib][4]=right_up_left[0]+12;
							markery[ib][5]=right_up_left[1]+lp1*fr1[ele][4]+2;
							break;
		}
		return;
	}

  //если путь или участок пути
	if(markery[ib][7]==5||markery[ib][7]==4)
	{
		//если путь не закрывается - выход
		if(fr1[markery[ib][6]][8]==9999)return;

		ele=markery[ib][6];//получить номер объекта в базе

    //для управляемого объекта получить его имя
		if(fr1[ele][8]==1010)strcpy(imya,pako[ele]);

		//вычислить координаты
		ix1=(fr2[ele][0]+fr2[ele][2])/2-strlen(pako[ele])*4;
		iy1=fr2[ele][1]-10;

		//для закрытого - цвет красный
		if(fr4[ele][2]==1)setcolor(LIGHTRED);
		//иначе - цвет серый
		else setcolor(8);
	}
	else
	{
		// если сигнал
		if(markery[ib][7]==2)
		{
			ele=markery[ib][6];//получить номер в базе для объекта
			if(fr1[ele][8]!=666)//если это - не смена направления
			{
				strcpy(imya,pako[ele]);//скопировать имя строкой
				D=strlen(pako[ele]);//определить длину

				//переключатель цветов в зависимости от состояния сигнала
				//fr3[#][0] - маневровый огонь
				//fr3[#][1] - поездной огонь
        //fr3[#][2] - маневровый ВС
        //fr3[#][3] - поездной ВС
				switch(fr3[ele][0]*8+fr3[ele][1]*4+fr3[ele][3]*2+fr3[ele][2])
				{
					case 1: setcolor(15);break; //есть только маневровый ВС
					case 2: setcolor(14);break; //есть только поездной ВС
					case 3: setcolor(5);break;  //поездной и маневровый ВС вместе
					default: setcolor(8);break; //все остальное
				}
			}
			if(fr3[ele][5]==1)
			setcolor(11);
			ix=fr2[ele][0];iy=fr2[ele][1]+3;//взять координаты для текста
			if(fr1[ele][9]==1) //сигнал над путем
			{
				if(fr1[ele][1]==1)//сигнал нечетный
				{
#if(OBRAT==-1)
					goto a1; //учесть ориентацию на экране
#endif
a2:       ix1=ix+3;
					iy1=iy-14;//скорректировать координаты
				}
				else//аналогично для четного сигнала
				{
#if(OBRAT==-1)
					goto a2;
#endif
a1:       if(fr1[ele][3]==7)ix1=ix-(D*7+3);
					else ix1=ix-D*7;
					iy1=iy-14;
				}
			}
			else //аналогично для сигнала под путем
			if(fr1[ele][9]==0)
			{
				if(fr1[ele][1]==0)//сигнал четный
				{
#if(OBRAT==-1)
					goto a3;
#endif
a4:       if(fr1[ele][3]==7)ix1=ix-(D*7+3);
					else ix1=ix-(D*7+3);
					iy1=iy+2;
				}
				else//сигнал нечетный
				{
#if(OBRAT==-1)
					goto a4;
#endif
a3:       if(fr1[ele][3]==7)ix1=ix+3;
					else ix1=ix;
					iy1=iy+2;
				}
			}
		}
		else
		if(markery[ib][7]==1)//если это имя для стрелки
		{
			ele=markery[ib][6];//получить объект для стрелки
			lp=0;
			D=strlen(pako[ele]);//вычистить длину имени
			while(pako[ele][lp]!=':') lp++; //дойти до разделителя
			lp++;
			for(lp1=0;lp1<20;lp1++)imya[lp1]=0; //очистить имя
			lp1=0;
			while(lp<D)
			{
				imya[lp1]=pako[ele][lp];//переписать имя от разделителя
				lp++;
				lp1++;
			}
      D=strlen(imya);
			if(fr3[ele][0]==fr3[ele][1])setcolor(12);//если для стрелки нет контроля
			else
			if((fr3[ele][0]==1)&&(fr3[ele][1]==0))setcolor(LIGHTGREEN);//если в плюсе

      else
			if((fr3[ele][0]==0)&&(fr3[ele][1]==1))setcolor(14);//если в минусе
      if(fr3[ele][5]==1)setcolor(LIGHTCYAN);
      ix=fr2[ele][0];
      iy=fr2[ele][1];
      switch(fr1[ele][9])//переключатель по ветвлению
      { //вверх
				case 1: if(fr1[ele][7]==0) //вход четный
                {
#if(OBRAT==-1)
									goto ob2;//учесть ориентацию
#endif
ob1:              ix1=ix-(D*7);iy1=iy-11;//получить координаты
								}
								else
								{ // вход нечетный
#if(OBRAT==-1)
									goto ob1;
#endif
ob2:              ix1=ix;iy1=iy-11;
								}
								break;
				// вниз
				case 0: if(fr1[ele][7]==1) //вход нечетный
								{
#if(OBRAT==-1)
									goto ob4;
#endif
ob3:              ix1=ix;iy1=iy+5;
								}
								else //вход четный
								{
#if(OBRAT==-1)
									goto ob3;
#endif
ob4:              ix1=ix-(D*7)+2; iy1=iy+5;
								}
								break;
				default:  ix1=ix;iy1=iy;break;
			}
			if(fr1[ele][3]==2)/* вход по косой слева-сверху вправо-вниз */
			{
				ix1=ix+3;iy1=iy-11;
			}
		}
#ifdef NEW_KNOPKA
		else
		//если это кнопка для разделки
		if((markery[ib][7]&0xfff)>9&&(markery[ib][7]&0xfff)<100)
		{
			ele=markery[ib][6];
			for(ijk=0;ijk<5;ijk++)//пройти по всем битам объекта
			{
				if((fr1[ele][2+ijk]>199)&&(fr1[ele][2+ijk]<300))//если кнопка для разделки
				{
					D=strlen(pako[ele])-2;
					strncpy(imya,pako[ele],D);//скопировать имя строкой без РИ
					D=textwidth(imya);//определить длину в пикселах
					//переключатель цветов в зависимости от состояния сигнала
					//fr3[#][0] - маневровый огонь
					//fr3[#][1] - поездной огонь
					//fr3[#][2] - маневровый ВС
					//fr3[#][3] - поездной ВС
					switch(fr3[ele][0]+fr3[ele][2]*2+fr3[ele][5]*4)
					{

						case 0: cvet_knop=7;break; //исходное состояние
						case 1: cvet_knop=14;break; //есть только предварительная

						case 2:
						case 3: cvet_knop=12;break; //есть исполнительная

						default: cvet_knop=7;break;  //поездной и маневровый ВС вместе;break; //все остальное
					}
					if(fr3[ele][5]==1)cvet_knop=11;
					setfillstyle(1,cvet_knop);

					ix=fr2[ele][ijk*2];
					iy=fr2[ele][ijk*2+1];//взять координаты для текста
					if((getpixel(ix+1,iy+1)!=cvet_knop)||(cikl_obnov==0))
					{
						bar(ix,iy,ix+D,iy+9);
						setcolor(8);
						outtextxy(ix,iy+2,imya);
						setlinestyle(0,0,0);
						rectangle(ix-1,iy-1,ix+D+1,iy+10);
					}
					else
					{
						obnovi_obraz(ix,iy,D,imya,cvet_knop);
					}
				}
			}
		}
		else
		if(markery[ib][7]<9999) //если это не экранная кнопка
		{
			ele=markery[ib][6]; //получить объект для маркера управления
			if(fr1[ele][1]==163) //если объект содержит кнопку
			{
				for(ijk=0;ijk<5;ijk++)
				{
					if(fr1[ele][ijk+3]==9999)continue;
					if((fr1[ele][ijk+3]&0xff00)!=0)break;		//если бит для кнопки
				}
				ix=fr2[ele][2*ijk];
				iy=fr2[ele][2*ijk+1];
				n_str=fr1[ele][ijk+3]&0xff;
				D=textwidth(name_lamp[n_str]);
				stroka_cvet=fr1[ele][2]&0xff;
				if(fr3[ele][ijk]==0)cvet_knop=cvt[stroka_cvet][ijk*2];
				else cvet_knop=cvt[stroka_cvet][ijk*2+1];
				if(fr3[ele][5]==1)cvet_knop=11;
				setfillstyle(1,cvet_knop);
				if((getpixel(ix+1,iy+1)!=cvet_knop)||(cikl_obnov==0))
				{
					bar(ix,iy,ix+D,iy+10);
					setcolor(8);
					outtextxy(ix+1,iy+2,name_lamp[n_str]);
					setlinestyle(0,0,0);
					rectangle(ix-1,iy-1,ix+D+1,iy+10);
				}
				else
				{
					obnovi_obraz(ix,iy,D,name_lamp[n_str],cvet_knop);
				}
			}
			if(fr1[ele][1]==27)//если это УКСПС
			{
				for(ijk=0;ijk<5;ijk++)
				{
					if(fr1[ele][ijk+3]==9999)continue;
					if((fr1[ele][ijk+3]&0xff00)!=0)break;		//если бит для кнопки
				}
				ix=fr2[ele][2*ijk];
				iy=fr2[ele][2*ijk+1];
				n_str=fr1[ele][ijk+3]&0xff;
				D=textwidth(name_lamp[n_str]);
				if(fr3[ele][ijk]==0)cvet_knop=7;
				else cvet_knop=14;
				if(fr3[ele][5]==1)cvet_knop=11;
				setfillstyle(1,cvet_knop);
				if((getpixel(ix+1,iy+1)!=cvet_knop)||(cikl_obnov==0))
				{
					bar(ix,iy,ix+D,iy+10);
					setcolor(8);
					outtextxy(ix+1,iy+2,name_lamp[n_str]);
					setlinestyle(0,0,0);
					rectangle(ix,iy,ix+D+1,iy+10);
				}
				else
				{
					obnovi_obraz(ix,iy,D,name_lamp[n_str],cvet_knop);
				}

			}
			//если это объект двойной ответственной команды
			if((fr1[ele][1]==130)||(fr1[ele][1]==131))
			{
				ele=fr1[ele][2]; //объект предварительной команды и неответственной
				ele1=fr1[ele][3]; //объект исполнительной команды
				ijk=(fr1[ele][4]&0xff00)>>8; //бит предварительного воздействия
				ijk1=fr1[ele][4]&0xff; //бит исполнительного воздействия

//кнопка предварительного воздействия
				ix=fr2[ele][2*ijk];
				iy=fr2[ele][2*ijk+1];
				n_str=(fr1[ele][12]&0xff00)>>8;
				D=textwidth(name_lamp[n_str]);
//кнопка исполнительного воздействия
				ix1=fr2[ele1][2*ijk1];
				iy1=fr2[ele1][2*ijk1+1];
				n_str1=fr1[ele1][12]&0xff;
				D1=textwidth(name_lamp[n_str1]);

				cod_bin=fr3[ele1][ijk1]*2+fr3[ele][ijk];
				switch(cod_bin)
				{
					case 0:	cvet_knop1=cvet_knop=7;break; //все в исходном

					case 1: cvet_knop1=14;cvet_knop=7;break; //есть предварительная

					case 2: cvet_knop1=14;cvet_knop=12;break;//есть исполнительная
					case 3: cvet_knop1=12;cvet_knop=12; break; //есть обе
				}
				if(fr3[ele][5]==1)cvet_knop=11;
				if(fr3[ele1][5]==1)cvet_knop1=11;
				setfillstyle(1,cvet_knop);
				if((getpixel(ix+1,iy+1)!=cvet_knop)||(cikl_obnov==0))
				{
					bar(ix+1,iy+1,ix+D,iy+9);
					setcolor(8);
					outtextxy(ix+1,iy+2,name_lamp[n_str]);
					setlinestyle(0,0,0);
					rectangle(ix,iy,ix+D+1,iy+10);
				}
				else
				{
					obnovi_obraz(ix+1,iy,D,name_lamp[n_str],cvet_knop);
				}
				setfillstyle(1,cvet_knop1);
				if((getpixel(ix1+1,iy1+1)!=cvet_knop1)||(cikl_obnov==0))
				{
					bar(ix1+1,iy1+1,ix1+D1,iy1+9);
					setcolor(8);
					outtextxy(ix1+1,iy1+2,name_lamp[n_str1]);
					setlinestyle(0,0,0);
					rectangle(ix1,iy1,ix1+D1+1,iy1+10);
				}
				else
				{
					obnovi_obraz(ix1+1,iy1,D1,name_lamp[n_str1],cvet_knop1);
				}
			}
		}
#endif
	}
fin:
	if(markery[NNBO][7]<=6)//если не кнопки
	{
		moveto(ix1,iy1);
		if(fr1[ele][0]!=1)//если не стрелка
		{
			if((fr1[ele][0]==2)&&(fr1[ele][11]!=9999))//если сигнал
			{
				setfillstyle(SOLID_FILL,cvv); //стереть имя
				bar(ix1,iy1,ix1+D*7,iy1+7);
				moveto(ix1,iy1);
				outtext(imya); //написать имя
			}
			else outtext(imya);//если что-то другое - написать не стирая
		}
		else
		if(fr1[ele][0]==1) //если стрелка
		{
			outtext(imya);   //написать имя
			if((i3!=-1)&&(markery[i3][6]==ele))//если маркер на стрелке

			{
				Fla_no_pp=1;
				show_kursor(i3);//нарисовать курсор
				Fla_no_pp=0;
			}
		}
    //для всего
		if(markery[NNBO][7]!=7)//если не вариантная кнопка
    {
      markery[NNBO][4]=ix1;//заполнить координаты
			markery[NNBO][5]=iy1;
    }
	}
	return;
}
//--------------------------------------------------------------------
/*********************************************\
* Процедура инициализации объектов markery и  *
* занесения в таблицу координат литеров для   *
* объектов управления элементами путев.разв.  *
\*********************************************/
void init_markery(int ie)
{
  int ix=0,iy=0,ele=0,ix1=0,iy1=0,lp,lp1;
	int D=0;
  char imya[20]="";

	nom_func("107");
	if((nikuda==1)||(klo==1)||(help!=0))return;
  if(markery[ie][7]==7)//если вариантная точка
  {
    ix=markery[ie][4];
    iy=markery[ie][5];
    setfillstyle(1,8);
		bar(ix-3,iy-3,ix+3,iy+3);
    return;
  }
  if((markery[ie][7]==5)||(markery[ie][7]==4))
  {
    ele=markery[ie][6];
    markery[ie][4]=fr2[ele][2]+(fr2[ele][0]-fr2[ele][2])/2+7;
    markery[ie][5]=fr2[ele][1];
    return;
  }
	if(markery[ie][7]==6)
  {
    ele=markery[ie][6];
    ele=fr1[ele][9]; /* адрес перегона */
    lp1=fr1[ele][8];
    lp=fr1[ele][9];
    if(fr1[lp][1]==0)
    {
#if OBRAT==1
			D=1;
#else
			D=2;
#endif
    }
    else
    {
#if OBRAT==1
			D=2;
#else
			D=1;
#endif
    }
    /* 1 - слева   2 - справа */
    switch(D)
    {
      case 1: markery[ie][4]=left_up_left[0]+7;
              markery[ie][5]=left_up_left[1]+lp1*fr1[ele][4]+2;
              break;
      case 2: markery[ie][4]=right_up_left[0]+12;
              markery[ie][5]=right_up_left[1]+lp1*fr1[ele][4]+2;
              break;
    }
    return;
  }
  if(markery[ie][7]==2)
  {
    ele=markery[ie][6];
    D=strlen(pako[ele]);
    ix=fr2[ele][0];
    iy=fr2[ele][1]+3;
    if(fr1[ele][9]==1) // сигнал над путем
    {
      if(fr1[ele][1]==1)//сигнал нечетный
      {
#if(OBRAT==-1)
				goto a1; //если у нечетных сигналов имена слева
#endif
a2:     ix1=ix+9;
				iy1=iy-12;
			}
			else //сигнал под путем
			{
#if(OBRAT==-1)
				goto a2; //если у четных сигналов имена справа
#endif
a1:     ix1=ix-(D*7);
        iy1=iy-12;
      }
    }
    else
    if(fr1[ele][9]==0)
    {
      if(fr1[ele][1]==0)
      {
#if(OBRAT==-1)
				goto a3;
#endif
a4:     ix1=ix-(D*7);
				iy1=iy;
			}
			else
			{
#if(OBRAT==-1)
				goto a4;
#endif
a3:     ix1=ix+9;
        iy1=iy;
      }
    }
  }
  else
  if(markery[ie][7]==1)
  {
    ele=markery[ie][6];
    lp=0;
    D=strlen(pako[ele]);
    while(pako[ele][lp]!=':') lp++;
    lp++;
    for(lp1=0;lp1<20;lp1++)imya[lp1]=0;
    lp1=0;
    while(lp<D)
    {
      imya[lp1]=pako[ele][lp];
      lp++;
      lp1++;
    }
    D=strlen(imya);
    ix=fr2[ele][0];
    iy=fr2[ele][1];
    if(fr1[ele][9]==1) /*вверх*/
    if(fr1[ele][7]==0) /* вход четный */
    {
#if(OBRAT==-1)
			goto ob2;
#endif
ob1:  ix1=ix-(D*7)+6;
			iy1=iy-10;
		}
		else
		{ /* вход нечетный */
#if(OBRAT==-1)
			goto ob1;
#endif
ob2:  ix1=ix+7;
      iy1=iy-10;
    }
    else /*  вниз */
    {
      if(fr1[ele][7]==1) /* нечет */
      {
#if(OBRAT==-1)
				goto ob4;
#endif
ob3:    ix1=ix+7;
        iy1=iy+4;
      }
      else
      {
#if(OBRAT==-1)
				goto ob3;
#endif
ob4:    ix1=ix-(D*7)+6;
        iy1=iy+4;
      }
    }
  }
fin:
  markery[ie][4]=ix1+5*D;
  markery[ie][5]=iy1+7;
  return;
}
/***************************************************************\
*  Процедура чтения параметров настройки каналов связи ПЭВМ     *
*  из файла tranc.svs                                           *
*  fu - хэндл файла tranc.svs                                   *
\***************************************************************/
void formula(int fu)
{
	unsigned char *chislo="0000000000",prob=0;
	int in=0,inn=0,NB=0;
	unsigned int speed,speed11,speed_pvm,speed_bbkp;

	nom_func("87");
	for(inn=0;inn<10;inn++)chislo[inn]=0xf;
	inn=0;
	setcolor(8);
	len=(kol_VO+1)*6;//определение длины файла для ограничений
	//выполнение чтения файла tranc.svs
	in=0;
	lseek(fu,0,0l);
	while(chislo[in-1]!=0x20)read(fu,&chislo[in++],1);
	DVA_MO=atoi(chislo);
  while(prob!='\n')read(fu,&prob,1);//поиск конца строки
  if(DVA_MO!=0)//если установлен признак наличия модемов
  {
    clrscr();
    moveto(8,80);
    outtext("В данном случае первый параметр в файле tranc.svs должен быть равен 0");
    getch();
    exit(1);
  }

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  //считывание адреса для ТУМС-1
  BAZ_ADR1=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  speed=atoi(chislo);//считывание скорости для ТУМС-1
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
	//считывание адреса для ТУМС-2
  BAZ_ADR11=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  speed11=atoi(chislo);//считывание скорости для ТУМС-2
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
	chislo[in]=0;
  //считывание адреса для ББКП
  BAZ_ADR2=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  speed_bbkp=atoi(chislo);//считывание скорости для ББКП
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
	//считывание адреса для кнопки ОК
  BAZ_ADR3=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
	//считывание адреса для обмена ПЭВМ
  BAZ_ADR4=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  speed_pvm=atoi(chislo);//считывание скорости для обмена ПЭВМ
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
	//считывание адреса для СПДЛП
  BAZ_ADR6=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  speed_spdlp=atoi(chislo);//считывание скорости для СПДЛП
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
	//считывание адреса переключателя ОСН/РЕЗ
  BAZ_ADR5=(chislo[2]-48)*256+(chislo[3]-48)*16+chislo[4]-48;
  while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  NB=atoi(chislo);//считывание вектора прерывания
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
	while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  TEST_N=atoi(chislo);//считывание признака фиксации ЛС/ЗС
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  MODE_KN=atoi(chislo);//считывание флага способа включения кнопки ОК
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

  in=0;prob=0;
  while(chislo[in-1]!=32)read(fu,&chislo[in++],1);
  chislo[in]=0;
  PROV_SCB=atoi(chislo);//считывание флага проверки СЦБ
	while(prob!='\n')read(fu,&prob,1);//поиск конца строки

	//задание делителей для заданной скорости
	switch(speed)
	{
		case 300l:    ml_ba=0x80; st_ba=0x01; break;
		case 600l:    ml_ba=0xc0; st_ba=0x00; break;
		case 1200l:   ml_ba=0x60; st_ba=0x00; break;
		case 2400l:   ml_ba=0x30; st_ba=0x00; break;
		case 4800l:   ml_ba=0x18; st_ba=0x00; break;
		case 9600l:   ml_ba=0x0c; st_ba=0x00; break;
		case 19200l:  ml_ba=0x6;  st_ba=0x00; break;
		case 38400l:  ml_ba=0x3;  st_ba=0x00; break;
		case 57600l:  ml_ba=0x2;  st_ba=0x00; break;
		case 115200l: ml_ba=0x1;  st_ba=0x00; break;
		default:  clrscr();moveto(80,80);
							outtext("Неизвестная скорость обмена МПСУ1-ПЭВМ");
							getch();exit(1);
	}
#ifdef KOL_SOO2
	switch(speed11)
	{
		case 300:    ml_ba2=0x80; st_ba2=0x01; break;
		case 600:    ml_ba2=0xc0; st_ba2=0x00; break;
		case 1200:   ml_ba2=0x60; st_ba2=0x00; break;
		case 2400:   ml_ba2=0x30; st_ba2=0x00; break;
		case 4800:   ml_ba2=0x18; st_ba2=0x00; break;
		case 9600:   ml_ba2=0x0c; st_ba2=0x00; break;
		case 19200:  ml_ba=0x6;   st_ba=0x00;  break;
		case 38400:  ml_ba=0x3;   st_ba=0x00;  break;
		case 57600:  ml_ba=0x2;   st_ba=0x00;  break;
		case 115200: ml_ba=0x1;   st_ba=0x00;  break;
		default: clrscr();moveto(80,80);
						 outtext("Неизвестная скорость обмена МПСУ2-ПЭВМ");
						 getch();
						 exit(1);
	}
#endif
	switch(speed_pvm)
	{
		case 300l: ml_bap=0x80;st_bap=0x01;break;
		case 600l: ml_bap=0xc0;st_bap=0x00;break;
		case 1200l: ml_bap=0x60;st_bap=0x00;break;
		case 2400l: ml_bap=0x30;st_bap=0x00;break;
		case 4800l: ml_bap=0x18;st_bap=0x00;break;
		case 9600l: ml_bap=0x0c;st_bap=0x00;break;
		case 19200l:  ml_ba=0x6;st_ba=0x00;  break;
		case 38400l:  ml_ba=0x3;st_ba=0x00;  break;
		case 57600l:  ml_ba=0x2;st_ba=0x00;  break;
		case 115200l: ml_ba=0x1;st_ba=0x00;  break;
		default:   clrscr(); moveto(80,80);
							 outtext("Неизвестная скорость для обмена ПЭВМ-ПЭВМ");
							 getch();
							 exit(1);
	}
	switch(speed_bbkp)
	{
	 case 300l: ml_bab=0x80; st_bab=0x01;break;
	 case 600l: ml_bab=0xc0; st_bab=0x00;break;
	 case 1200l: ml_bab=0x60;st_bab=0x00;break;
	 case 2400l: ml_bab=0x30;st_bab=0x00;break;
	 case 4800l: ml_bab=0x18;st_bab=0x00;break;
	 case 9600l: ml_bab=0x0c;st_bab=0x00;break;
	 default: clrscr(); moveto(80,80);
						outtext("Неизвестная скорость для обмена ПЭВМ-ББКП");
						getch();
						exit(1);
	}
	switch(NB)
	{
	 case 3 : V=0x0b; break;
	 case 4 : V=0x0c; break;
	 case 5	: V=0x0d; break;
	 case 6	: V=0x0e; break;
	 case	7	: V=0x0f; break;
	 case	9	: V=0x71; break;
	 case	10 : V=0x72; break;
	 case 11 : V=0x73; break;
	 case 12 : V=0x74; break;
	 case 15 : V=0x77; break;
	 default: clrscr(); moveto(80,80);
						outtext("Неизвестный вектор прерывания");
						getch();
						exit(1);
	}
	return;
}
/******************************************/
void read_lex()
{
	int j=0,cvet=0,ddo=0;

	nom_func("268");
#ifdef lex_lex//если выполняется конвертация файла lex.lex
	fai=fopen("dat\\lex.lex","r");
	file_soob=creat("dat\\lex.bin",S_IREAD | S_IWRITE | O_BINARY);
	if(fai==NULL)
	{
		gotoxy(5,20);
		printf("Недостаточно данных для запуска программы, проверьте DAT");
		getch();
		exit(0);
	}
	while(1)
	{
		for(u=0;u<60;u++)soob.lex[u]=0x0;
		u=0;
		while(u!='#')
		{
			u=fgetc(fai);
			if(u=='@') goto end;
      if(u=='#') break;
      if(u==0x09) soob.lex[j]=32;
      else soob.lex[j]=u;
      j++;
    }
    j=0;
    u=0;
    fscanf(fai,"%d",&soob.COlo);
    for(u=0;u<60;u++)soob.lex[u]=~soob.lex[u];
    write(file_soob,soob.lex,60);
    cvet=soob.COlo;
    write(file_soob,&cvet,1);
    cvet=10;
    write(file_soob,&cvet,1);
oo:
    ddo=fgetc(fai);
    while(ddo!='\n') goto oo;
  }
end:
	if(fai!=NULL)fclose(fai);
	fai=NULL;
	if(file_soob>0)close(file_soob);
	file_soob=0;
#else
  file_soob=open("dat\\lex.bin",S_IREAD|O_BINARY);
#endif
	return;
}
//------------------------------------------------------------------------
/****************************************\
* Процедура проверки и сбора ненормовых  *
* событий по платам ввода-вывода УВК.    *
\****************************************/
void prov_lX()
{ long t_lu=0L,t_lv=0L,t_ly=0L,t_lz=0L,t_lc=0L;
  int vi=0,Nmp=0;

	nom_func("246");
	for(Nmp=0;Nmp<skoko_stoek;Nmp++)
  { if(time_lu[Nmp]!=0L)
    { t_lu=biostime(0,0L);
      if((t_lu-time_lu[Nmp])>=40L)
      { for(vi=0;vi<=14;vi++)if(mas_lu[Nmp][vi]<3)mas_lu[Nmp][vi]=0;
        for(vi=0;vi<=2;vi++)lu[Nmp][vi]='@';
        time_lu[Nmp]=0L;
      }
    }
    if(time_lv[Nmp]!=0L)
    { t_lv=biostime(0,0L);
      if((t_lv-time_lv[Nmp])>=40L)
      { for(vi=0;vi<=14;vi++)if(mas_lv[Nmp][vi]<3);mas_lv[Nmp][vi]=0;
        for(vi=0;vi<=2;vi++)lv[Nmp][vi]='@';
        time_lv[Nmp]=0L;
      }
    }
    if(time_ly[Nmp]!=0L)
    { t_ly=biostime(0,0L);
      if((t_ly-time_ly[Nmp])>=40L)
      { for(vi=0;vi<=15;vi++)if(mas_ly[Nmp][vi]<3)mas_ly[Nmp][vi]=0;
        for(vi=0;vi<=3;vi++)ly[Nmp][vi]='@';
        time_ly[Nmp]=0L;
      }
    }
    if(time_lz[Nmp]!=0L)
    { t_lz=biostime(0,0L);
      if((t_lz-time_lz[Nmp])>=40L)
      { for(vi=0;vi<=15;vi++)if(mas_lz[Nmp][vi]<3)mas_lz[Nmp][vi]=0;
        for(vi=0;vi<=3;vi++)lz[Nmp][vi]='@';
        time_lz[Nmp]=0L;
      }
    }
    if(time_lc[Nmp]!=0L)
    { t_lc=biostime(0,0L);
      if((t_lc-time_lc[Nmp])>=40L)
      { for(vi=0;vi<=15;vi++)if(mas_lc[Nmp][vi]<3)mas_lc[Nmp][vi]=0;
        for(vi=0;vi<=3;vi++)lc[Nmp][vi]='@';
        time_lc[Nmp]=0L;
      }
    }
  } /* for */
	return;
}
//-----------------------------------------------------------------------
void z1(int WHO)
{
  int ip,lj,TELL,KSS,KVVv,i33;

	nom_func("424");
	if(WHO==1) { TELL=TEL1; KSS=KS1; KVVv=KVV1;}
#ifdef KOL_SOO2
  else { TELL=TEL2; KSS=KS2; KVVv=KVV2;}
#endif
  pict1(WHO);
  setcolor(3);
  for(ip=kol_OSN;ip<kol_VO;ip++)
  if(fr1[ip][0]==2)
    if(fr1[ip][13]==WHO)
    {
      bata=0;
      displaysignal(ip,5);
		}
  tel_vkl_vikl(TELL);
  for(lj=Start_R;lj<Start_R+WAZO;lj++)
  {
    if(fr1[lj][13]==WHO)
    {
      krasit_razd(lj,0);
      krasit_razd(lj,2);
    }
  }
  krasit_knopki(Start_R+WAZO,2);
  for(i33=0;i33<kol_OB_UPR;i33++)imena(i33);
#ifdef kol_STY
  Styki();
#endif
  kvadrat();
  //if(zapret[WHO-1]==1||zapret_otv[WHO-1]==1)
  //{
	//  if(WHO==1)w(172,999," 1 ТУМС");
  //  if(WHO==2)w(172,999," 2 ТУМС");
  //}
  //else
  //  if(otkaz_tu[WHO-1]==1)
  //  {
  //    if(WHO==1)w(171,999," 1 ТУМС");
  //    if(WHO==2)w(171,999," 2 ТУМС");
  //  }
  knopka(KSS);
  kvv(KVVv,0);
  if(DISK==0)knopka_OK();
	/*i3 =-1;*/
	return;
}
/******************************************/
void read_spdlp(void *fai)
{
	//выполнение чтения файла spdlp.dat
#ifdef SPDLP
	inn=fscanf(fai,"%d",&byte_spdlp[0]);//чтение адреса пункта СПДЛП
	if(byte_spdlp[0]>255)
	{
		clrscr(); moveto(80,80);
		outtext("Адрес СПДЛП превышает допустимое значение");
		moveto(80,160);
		outtext("Для выхода из программы нажмите любую клавишу");
		getch();
		exit(1);
	}
	else   adr_spdlp=byte_spdlp[0];
	for(in1=0;in1<N_BIT_SCB;in1++)
	{
		fu=0;while(fu!='\n')fu=fgetc(fai);//поиск конца строки
		inn=fscanf(fai,"%d%d",&byte_spdlp[in1],&bt_spdlp[in1]);//чтение массива СПДЛП
		if(inn!=2)
		{
			clrscr(); moveto(80,80);
			outtext("Нарушена структура файла с данными для СПДЛП spdlp.dat.");
			moveto(80,160);
			outtext("Для выхода из программы нажмите любую клавишу");
			getch();
			exit(1);
		}
	}
		switch(speed_spdlp)
	{
		case 300l:    ml_bas=0x80; st_bas=0x01; break;
		case 600l:    ml_bas=0xc0; st_bas=0x00; break;
		case 1200l:   ml_bas=0x60; st_bas=0x00; break;
		case 2400l:   ml_bas=0x30; st_bas=0x00; break;
		case 4800l:   ml_bas=0x18; st_bas=0x00; break;
		case 9600l:   ml_bas=0x0c; st_bas=0x00; break;
		case 19200l:  ml_bas=0x6;  st_bas=0x00; break;
		case 38400l:  ml_bas=0x3;  st_bas=0x00; break;
		case 57600l:  ml_bas=0x2;  st_bas=0x00; break;
		case 115200l: ml_bas=0x1;  st_bas=0x00; break;
		default:  clrscr();moveto(80,80);
							outtext("Неизвестная скорость обмена c СПДЛП");
							getch();exit(1);
	}
	K64=N_BIT_SCB/64;
	K32=0;
	if((N_BIT_SCB-K64*64)>32)K64++;
	else
		if(N_BIT_SCB==K64*64)K32=0;
		else K32=1;
	LEN_SPD=K64*9+K32*5+5;
#endif
	return;
}