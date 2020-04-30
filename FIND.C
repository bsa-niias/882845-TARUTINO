#include "bla.h"
//-------------------------------------------------------------------
/*******************************************************\
*  Процедура поиска СП для стрелки по:                  *
* ga - номер объекта стрелки, для которой ищется СП     *
* tu - направление входа на стрелку 0-четный,1-нечетный *
\*******************************************************/
FILE *sfs;
int findspu1(int ga,int tu)
{
  int shag,strelk_=0,perehod=0,up=0;
  if(tu>1){tu=tu-2;up=1;}

	nom_func("59");
	if(tu==0)shag=1;//если вход на стрелку четный, шагать вперед
  else shag=-1; //если вход на стрелку нечетный, шагать назад
beg:
  ga=ga+shag; //перейти на следующий объект
  if(fr1[ga][0]==3)return(ga);//если вышли на СП - то дать его номер
  if((fr1[ga][0]==4)&&(up!=0))return(ga); //вышли на УП

   //если не стрелка, а что-то другое из путевого развития,
	if((fr1[ga][0]!=1)&&(fr1[ga][0]<6))
	return(-1); //то неудача
beg1:
  if(fr1[ga][0]==1)//если вышли на стрелку
  {
    if(perehod==1)
    {
      perehod=0;
      goto beg;
    }
		if((fr1[ga][7]==0)&&( shag==1))goto beg; //если вход на стрелку
		                                         //такой же,
		if((fr1[ga][7]==1)&&( shag==-1))goto beg;//то продолжать движение
    else//если вход встречный
    {
      //если переход по плюсу - прямо
      if(fr1[ga][1]==1)
      { //если есть стык по плюсу
        if(fr1[ga][4]==1)
        {  //если есть стык по минусу
          if(fr1[ga][5]==1)return(-1); //вернуть неудачу
          else //если нет стыка по минусу
          {
            strelk_=1;
            ga=fr1[ga][2]; //уйти на поиск СП за минусовой ветвью
            goto beg1;
          }
        }

      }
      else//переход по плюсу с отклонением
      {
        if(fr1[ga][4]==1)//если есть стык по плюсу
        {
          if(fr1[ga][5]==1)return(-1);//если есть стык по минусу - неудача
          else //если нет стыка по минусу
          {
            strelk_=1;
            ga=fr1[ga][2]; //уйти на поиск СП за минусовой ветвью
            goto beg1;
          }
        }
        else //если нет стыка по плюсу
        {
          strelk_=1;
          ga=fr1[ga][2]; //уйти на поиск СП за плюсовой ветвью
          goto beg1;
        }
      }
    }
  }
  if(fr1[ga][0]==6)//если вышли на переход
  {
    //если пришли к переходу со стороны стрелки
    if((strelk_==1)||(perehod==1))
		{
      strelk_=0;
      perehod=1;
      goto beg;
    }
    else
    {
      ga=fr1[ga][1];
      strelk_=0;
      perehod=1;
      goto beg1;
    }

  }
  else
  {
    strelk_=0;
    perehod=0;
    goto beg;
  }
}
//-----------------------------------------------------------
/***********************************\
*           p_li(int nom)           *
* Отображение станционного переезда *
* nom - номер объекта в базе        *
\***********************************/
void p_li(int nom)
{
	int gol=0,//переменная для радиуса "глаз" шлагбаума
	ob_iz, bit_iz,// номер 1 объекта и бита для извещения
	ob_iz1, bit_iz1,// номер 2 объекта и бита для извещения
	ob_zak, bit_zak,//номер объекта и бита для закрытия переезда
	cvt_iz, //цвет для извещения
	cvt_zak,//цвет для закрытия
	l_p,//длина переезда
	R_zg,X_zg,//радиус и координата Х лампы заграждения
	R_n,X_n, //радиус и координата X лампы неисправности
	R_a,X_a, //радиус и координата X лампы аварии
	Y,   //координата Y ламп
	bit_zg,//бит заграждения
	bit_n, //бит неисправности
	bit_a; //бит аварии

	if((klo==1)||(help!=0)||(nikuda==1))return;

	l_p=fr1[nom][6];

	if(fr1[nom][2]!=9999)//если есть объект 1го извещения
	{
		ob_iz =fr1[nom][2]&0xfff;
		bit_iz=(fr1[nom][2]&0xf000)>>12;
	}
	else bit_iz=0xf;

	if(fr1[nom][8]!=9999) //если есть объект 2го извещения
	{
		ob_iz1 =fr1[nom][8]&0xfff;
		bit_iz1=(fr1[nom][8]&0xf000)>>12;
	}
	else bit_iz1=0xf;

	if(fr1[nom][3]!=9999) // если есть объект закрытого состояния переезда
	{
		ob_zak=fr1[nom][3]&0xfff;
		bit_zak=(fr1[nom][3]&0xf000)>>12;
	}
	else bit_zak=0xf;

	Y=fr1[nom][12];

	bit_zg=(fr1[nom][7]&0xF000)>>12;
	R_zg=(fr1[nom][9]&0xF000)>>12;
	X_zg=fr1[nom][9]&0xFFF;

	bit_n=(fr1[nom][7]&0xF00)>>8;
	R_n=(fr1[nom][10]&0xF000)>>12;
	X_n=fr1[nom][10]&0xFFF;

	bit_a=(fr1[nom][7]&0xF0)>>4;
	R_a=(fr1[nom][11]&0xF000)>>12;
	X_a=fr1[nom][11]&0xFFF;

	if(fr3[nom][bit_zg]==1)//если есть заграждение
	{
		setcolor(12);
	}
	else setcolor(8);
  if(fr3[nom][5]==1)setcolor(11);
  setlinestyle(0,0,0);
	for(gol=0;gol<R_zg;gol++)circle(X_zg,Y,gol);

	if(fr3[nom][bit_n]==1)//если есть неисправность
	{
		if(tst_fix(nom,bit_n))
		{
			if(get_str(nom,bit_n)==0)neisprav_na_disk('@',chudo);
			w(169,999,chudo);
			zvuk_vkl(0,0);
			home(modi);
			fix(nom,bit_n);
		}
		setcolor(12);
	}
	else
	{
		setcolor(8);
    fix(nom,bit_n);
  }
	if(fr3[nom][5]==1)setcolor(11);
   setlinestyle(0,0,0);
	for(gol=0;gol<R_n;gol++)circle(X_n,Y,gol);

	if(fr3[nom][bit_a]==1)//если есть авария
	{
		if(tst_fix(nom,bit_a))
		{
			if(get_str(nom,bit_a)==0)neisprav_na_disk('@',chudo);
			w(175,999,chudo);
			zvuk_vkl(0,0);
			home(modi);
			fix(nom,bit_a);
		}
		setcolor(12);
	}
	else
	{
		setcolor(8);
    fix(nom,bit_a);
	}
	if(fr3[nom][5]==1)setcolor(11);
   setlinestyle(0,0,0);
	for(gol=0;gol<R_a;gol++)circle(X_a,Y,gol);



	cvt_iz=7;
	if(bit_iz!=0xf)
	{
		if(fr3[ob_iz][bit_iz]==1)cvt_iz=14;
		else cvt_iz=8;
	}
	if(bit_iz1!=0xf)
	{
		if(fr3[ob_iz1][bit_iz1]==1)cvt_iz=14;
		else if(cvt_iz==7)cvt_iz=8;
	}

	cvt_zak=7;
	if(bit_zak!=0xf)
	{
		if(fr3[ob_zak][bit_zak]==1){cvt_zak=12;cvt_iz=12;}
		else cvt_zak=7;
	}
	setfillstyle(1,cvt_zak);

	if(fr3[nom][5]==1)setfillstyle(1,11);
//	bar(fr1[nom][4]-6,fr1[nom][5]-4,fr1[nom][4]+2,fr1[nom][5]-3);
//	bar(fr1[nom][4]-6,fr1[nom][5]+l_p+3,fr1[nom][4]+2,fr1[nom][5]+l_p+4);
	if(fr3[nom][5]==1) setcolor(LIGHTCYAN);
	setcolor(8); setlinestyle(0,0,0);
	line(fr1[nom][4]-8,fr1[nom][5]-16,fr1[nom][4]-8,fr1[nom][5]-6);
	line(fr1[nom][4]-8-2,fr1[nom][5]-16,fr1[nom][4]-8+2,fr1[nom][5]-16);
	line(fr1[nom][4]-8-2,fr1[nom][5]-6-3,fr1[nom][4]-8+2,fr1[nom][5]-6-3);
	circle(fr1[nom][4]-8-2-3,fr1[nom][5]-6-3,3);
	circle(fr1[nom][4]-8+2+3,fr1[nom][5]-6-3,3);

	line(fr1[nom][4]+2,fr1[nom][5]+16+fr1[nom][6],fr1[nom][4]+2,fr1[nom][5]+6+fr1[nom][6]);
	line(fr1[nom][4]+2-2,fr1[nom][5]+16+fr1[nom][6],fr1[nom][4]+2+2,fr1[nom][5]+16+fr1[nom][6]);
	line(fr1[nom][4]+2-2,fr1[nom][5]+6+3+fr1[nom][6],fr1[nom][4]+2+2,fr1[nom][5]+6+3+fr1[nom][6]);
	circle(fr1[nom][4]+2-2-3,fr1[nom][5]+6+3+fr1[nom][6],3);
	circle(fr1[nom][4]+2+2+3,fr1[nom][5]+6+3+fr1[nom][6],3);
	setcolor(cvt_iz);
	if(fr3[nom][5]==1)setcolor(LIGHTCYAN);
	setlinestyle(0,0,0);
	for(gol=0;gol<3;gol++)
	{
		circle(fr1[nom][4]-13,fr1[nom][5]-9,gol);
		circle(fr1[nom][4]-3,fr1[nom][5]-9,gol);
		circle(fr1[nom][4]-3,fr1[nom][5]+9+fr1[nom][6],gol);
		circle(fr1[nom][4]+7,fr1[nom][5]+9+fr1[nom][6],gol);
	}
	return;
}
//----------------------------------------------------------
/**********************************************\
* Процедура прорисовки объектов из стойки WHO. *
\**********************************************/
void pict1(int WHO)
{
	int K;
	int pamjat;

	nom_func("220");
	//проход по всем основным элементам
	for(pamjat=0;pamjat<kol_VO;pamjat++)
	{
		//если объект не из другой стойки
		if((fr1[pamjat][13]==WHO)||(fr1[pamjat][13]>kol_OSN))
		switch(fr1[pamjat][0])
		{
			case 5: if((fr1[pamjat][1]!=1)||(fr1[pamjat][2]!=1))//если путь
							{
								K=pamjat;
								displaypath(K,0);displaypath(K,1);displaypath(K,2);
								displaypath(K,3);displaypath(K,4);
							}
							break;
			case 1: K=pamjat; // если стрелка
							displaystrelka(K,0,0);//прорисовка стрелки К
							break;
			case 2: if(fr1[pamjat][2]<2)
							{
								displaysignal(pamjat,0);
								displaysignal(pamjat,2);
								displaysignal(pamjat,1);
								displaysignal(pamjat,3);
							}
							if(fr1[pamjat][8]==666)
							displaysignal(pamjat,0);//если смена направления
							break;
			case 4: displaypathuch(pamjat);//если участок пути
							break;
			case 3: if(fr1[pamjat][1]<2)//если СП
							{
								zap_matr(pamjat);cvet_matr();ris_sp_str();
							}
			default:	break;

		}
	}

	//проход по всем основным элементам
	for(pamjat=kol_VO;pamjat<kol_VO;pamjat++)
	{
		switch(fr1[pamjat][0])
		{
			case 1: K=pamjat; // если стрелка
							displaystrelka(K,0,0);//прорисовка стрелки К
							break;
			case 3: if(fr1[pamjat][1]<2)//если СП
							{
								zap_matr(pamjat);cvet_matr();ris_sp_str();
							}
			default:	break;

		}
	}
	return;
}
/*********************************************************/
#ifdef kol_STY
void Styki()
{
  int i;

	nom_func("324");
	for(i=0;i<kol_STY;i++)
  {
      setlinestyle(0,0,0);setcolor(8);
			circle(styki[i][1],styki[i][2],3);
  }
	return;
}
#endif
//------------------------------------------------------------------------
/***************************************************\
*  Процедура изображения статуса ПЭВМ на мнемосхеме *
*  в виде стилизованной подставки монитора          *
\***************************************************/
void kvadrat()
{

	nom_func("139");
	if(STATUS==2){setfillstyle(1,8);bar(10,45,32,50);}
  else
    if(STATUS==0){setfillstyle(1,14);bar(10,45,32,50);}
    else { setfillstyle(1,10); bar(10,45,32,50);}
	return;
}
//---------------------------------------------------------
void whole_finish(int tipus)
{

	nom_func("413");
	End_vara=0;
	zapretvybora=0;povt1=0;povt2=0;poka=0;n_m=0;puti=0;
	vozvr=0;po_ma=0;
	siv=0;osya=0;
	ukaz_trass=0;
	if(tipus!=0)
	{
		END=0;
		mane1=0;manevro1=0;manevr1=0;
		nach_marsh=end_marsh=half_marsh1=half_marsh=nach0_marsh=0;
		while(TRASSA[ukaz_trass].object!=0xffff)
		{
			if(fr1[TRASSA[ukaz_trass].object&0xfff][0]==1)
			fr3[TRASSA[ukaz_trass].object&0xfff][6]=0;
			ukaz_trass++;
			if(ukaz_trass>=100)break;
		}
	}
	else
	{
		END_OPER=0;
		mane=0;manevro=0;manevr=0;
		nach_marsh_oper=end_marsh_oper=half_marsh1=half_marsh=nach0_marsh=0;
		while(TRASSA0[ukaz_trass].object!=0xffff)
		{
			if(fr1[TRASSA0[ukaz_trass].object&0xfff][0]==1)
			fr3[TRASSA0[ukaz_trass].object&0xfff][6]=0;
			ukaz_trass++;
			if(ukaz_trass>=100)break;
		}
	}
	return;
}
//----------------------------------------------------------
int test_sost_(int ob)
{
	int ej;

	nom_func("352");
	for(ej=9;ej<=10;ej++)
	if(fr3[ob][ej]==1) return(1);
	return(0);
}
//-------------------------------------------------------------------
/****************************************************\
* Процедура обработки данных о ненормах дешифратора  *
* ненорма фиксируется при трехкратном подтверждении. *
\****************************************************/
int do_it(int puk,unsigned char bnf[12])
{ int ccv1=0,yi;

	nom_func("44");
	if(bnf[3]==D_kont[0]) //если получены данные по K1X - объед.групп
	{ if(time_lu[puk]==0l)time_lu[puk]=biostime(0,0L); //зафиксировать время события
		for(yi=0;yi<=2;yi++) //
		{ lu[puk][yi]=bnf[yi+4];//обновить запакованные значения для байтов K1X
			ccv1=ccv1+obnov_obied(puk,yi,bnf);//раскидать распаковку полученных бит по массиву mas_lu
		}
		if(ccv1>0)slom_interf(4000+puk);
		return;
	}
	else
	if(bnf[3]==D_kont[1])//если получены данные по O1X - обрыв групп
	{ if(time_lv[puk]==0l)time_lv[puk]=biostime(0,0L);
		for(yi=0;yi<=2;yi++)
		{ lv[puk][yi]=bnf[yi+4];
			ccv1=ccv1+obnov_obryv(puk,yi,bnf);
		}
		if(ccv1>0)slom_interf(4010+puk);
		return;
	}
	else
	if(bnf[3]==D_kont[2])//если получены данные по потере 0 D1X - M201-1
	{ if(time_ly[puk]==0l)time_ly[puk]=biostime(0,0L);
		//запись данных о потере
		for(yi=0;yi<=3;yi++)
		{ ly[puk][yi]=ly[puk][yi]|(bnf[yi+4]&0xff);
			ccv1=ccv1+obnov_201_1(puk,yi,bnf,0);
		}
		if(ccv1>0)slom_interf(4020+puk);
		return;
	}
	else
	if(bnf[3]==D_kont[3]) //если получены данные по потере 0 D2X - M201-2
	{ if(time_lz[puk]==0l)time_lz[puk]=biostime(0,0L);
		for(yi=0;yi<=3;yi++)
		{ lz[puk][yi]=lz[puk][yi]|(bnf[yi+4]&0xff);
			ccv1=ccv1+obnov_201_2(puk,yi,bnf,0);
		}
		if(ccv1>0)slom_interf(4030+puk);
		return;
	}
	else
	if(bnf[3]==D_kont[4])//если получены данные по потере нуля D3X - M201-3
	{
		if(time_lc[puk]==0l)time_lc[puk]=biostime(0,0L);
		for(yi=0;yi<=3;yi++)
		{
			lc[puk][yi]=lc[puk][yi]|(bnf[yi+4]&0xff);
			ccv1=ccv1+obnov_201_3(puk,yi,bnf,0);
		}
		if(ccv1>0)slom_interf(4040+puk);
		return;
	}
	if(bnf[3]==D_kont[5])//если получены данные по потере нуля D4X - M201-4
	{
		if(time_ls[puk]==0l)time_ls[puk]=biostime(0,0L);
		for(yi=0;yi<=3;yi++)
		{
			ls[puk][yi]=ls[puk][yi]|(bnf[yi+4]&0xff);
			ccv1=ccv1+obnov_201_4(puk,yi,bnf,0);
		}
		if(ccv1>0)slom_interf(4050+puk);
		return;
	}
//----------------------------------проверка правильности единиц
	if(bnf[3]==D_kont[6])//если получены данные по потере единиц D3X - M201-1
	{
		if(time_ly[puk]==0l)time_ly[puk]=biostime(0,0L);
		for(yi=0;yi<=3;yi++)
		{
			ly[puk][yi]=ly[puk][yi]|((bnf[yi+4]<<8)&0xff00);
			ccv1=ccv1+obnov_201_1(puk,yi,bnf,1);
		}
		if(ccv1>0)slom_interf(4120+puk);
		return;
	}
	if(bnf[3]==D_kont[7])//если получены данные по потере единиц D3X - M201-2
	{
		if(time_lz[puk]==0l)time_lc[puk]=biostime(0,0L);
		for(yi=0;yi<=3;yi++)
		{
			lz[puk][yi]=lz[puk][yi]|((bnf[yi+4]<<8)&0xff00);
			ccv1=ccv1+obnov_201_2(puk,yi,bnf,1);
		}
		if(ccv1>0)slom_interf(4130+puk);
		return;
	}
	if(bnf[3]==D_kont[8])//если получены данные по потере единиц D3X - M201-3
	{
		if(time_lc[puk]==0l)time_lc[puk]=biostime(0,0L);
		for(yi=0;yi<=3;yi++)
		{
			lc[puk][yi]=lc[puk][yi]|((bnf[yi+4]<<8)&0xff00);
			ccv1=ccv1+obnov_201_3(puk,yi,bnf,1);
		}
	if(ccv1>0)
	slom_interf(4140+puk);
		return;
	}
	return;
}
//---------------------------------------------------------------------
/*****************************************************\
* Процедура обновления данных по объединению групп    *
* pek - номер канала, по которому получено сообщение  *
* ri - номер байта, для которого получено сообщение   *
* blum - буфер сообщения от ТУМС с возможной ненормой *
\*****************************************************/
int obnov_obied(int pek,int ri,unsigned char blum[12])
{
  int i,jk;
  unsigned char  iok;

	nom_func("192");
	jk=0;
	if(ri>2)return(0);       //если номер байта не для этой функции
  for(i=0;i<5;i++)         //пройти по битам
  { iok=blum[4+ri]&(1<<i); //получить состояние бита
    if(iok!=0)             //если бит установлен
    {
      mas_lu[pek][i+ri*5]++;           //увеличить счетчик ненорм
      if(mas_lu[pek][i+ri*5]==3)jk=1;  //если счетчик=3, установить признак

      //если ненорм более трех, то ограничиться значением 4
      if(mas_lu[pek][i+ri*5]>3)mas_lu[pek][i+ri*5]=4;
    }
    //если бит сброшен, то сбросить накопленный счетчик
    else mas_lu[pek][i+ri*5]=0;
  }
  return(jk);
}
//-------------------------------------------------------------------------
/****************************************************\
* Процедура фиксации ненорм  по обрыву групп         *
* pek- канал получения данных о ненорме              *
* ri- номер байта в числе информационных в сообщении *
* blum - буфер сообщения, полученного от УВК         *
\****************************************************/
obnov_obryv(int pek,int ri,unsigned char blum[12])
{
	int i,jk;
	unsigned char iov;

	nom_func("193");
	jk=0;
	if(ri>2)return(0);       //если байт не для этой функции
	for(i=0;i<5;i++)         //пройти по битам
	{ iov=blum[4+ri]&(1<<i); //определить состояние бита
		if(iov!=0)             //если бит установлен
		{
			//номера разрядов с ненормой могут быть
			//0-ой байт от 0 до 4
			//1-ый байт от 5 до 9
			//2-ой байт от 10 до 14
			mas_lv[pek][i+ri*5]++;//увеличить счетчик фиксации для этого разряда

			//если в течение 2-х секунд получено 3 ненормы по этому разряду
			//установить признак ненормы
			if(mas_lv[pek][i+ri*5]==3)jk=1;

			//если счетчик превысил число 3, то ограничиться четырьмя ненормами
			if(mas_lv[pek][i+ri*5]>3)
			{
				mas_lv[pek][i+ri*5]=4;
			}
		}
    //если бит сброшен - сбросить счет ненорм для этого разряда
		else mas_lv[pek][i+ri*5]=0;
	}
	return(jk);
}
//-------------------------------------------------------------------------
/****************************************************\
* Процедура фиксации ненорм  по модулю М201-1        *
* pek- канал получения данных о ненорме              *
* ri- номер байта в числе информационных в сообщении *
* blum - буфер сообщения, полученного от УВК         *
* tip - тип события 0-нет нуля 1- нет единицы        *
* комментарии см. в процедуре obnov_lc               *
\****************************************************/
obnov_201_1(int pek,int ri,unsigned char blum[12],int tip)
{ int i,jk,obek,N_razr;
	unsigned char iok;

	nom_func("194");
	if(ri>3)return(0);
	jk=0;
	for(i=0;i<4;i++)
	{ iok=blum[4+ri]&(1<<i);
		if(iok!=0)
		{
			if(tip==0)
			{
				mas_ly[pek][i+ri*4]++;
				if((mas_ly[pek][i+ri*4]&0xff)==3)jk=1;
				if((mas_ly[pek][i+ri*4]&0xff)>3)
				{
					mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]&0xff00;
					mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]|4;
				}
			}
			if(tip==1)
			{
				mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]+0x0100;
				if((mas_ly[pek][i+ri*4]&0xff00)==0x300)jk=1;
				if((mas_ly[pek][i+ri*4]&0xff00)>0x300)
				{
					mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]&0xff;
					mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]|0x400;
				}
			}
		}
		else
		{
			if(tip==0)mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]&0xff00;
			if(tip==1)mas_ly[pek][i+ri*4]=mas_ly[pek][i+ri*4]&0xff;
		}
	}
	if(tip==0) //если отсутствие нуля
	{ N_razr=0;
		for(obek=0;obek<16;obek++)
		{
			if((mas_ly[pek][obek]&0xff)>=3)N_razr=N_razr+1;
		}
		//вставка для ст. Новосергиевская
		if(N_razr>=4)
		{
			for(obek=0;obek<24;obek++)
			{
				fr3[GRUPPA1[pek][obek]][5]=1;
			}
		}
	}
	return(jk);
}
//-------------------------------------------------------------------------
/****************************************************\
* Процедура фиксации ненорм  по модулю М201-2        *
* pek- канал получения данных о ненорме              *
* ri- номер байта в числе информационных в сообщении *
* blum - буфер сообщения, полученного от УВК         *
* tip - тип события 0-нет нуля 1- нет единицы        *
* комментарии см. в процедуре obnov_lc               *
\****************************************************/
obnov_201_2(int pek,int ri,unsigned char blum[12],int tip)
{ int i,jk;
	unsigned char iok;

	nom_func("195");
	if(ri>3)return(0);
	jk=0;
	for(i=0;i<4;i++)
	{ iok=blum[4+ri]&(1<<i);
		if(iok!=0)
		{
			if(tip==0)
			{
				mas_lz[pek][i+ri*4]++;
				if((mas_lz[pek][i+ri*4]&0xff)==3)jk=1;
				if((mas_lz[pek][i+ri*4]&0xff)>3)
				{
					mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]&0xff00;
					mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]|4;
				}
			}
			if(tip==1)
			{
				mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]+0x0100;
				if((mas_lz[pek][i+ri*4]&0xff00)==0x300)jk=1;
				if((mas_lz[pek][i+ri*4]&0xff00)>0x300)
				{
					mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]&0xff;
					mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]|0x400;
				}
			}

		}
		else
		{
			if(tip==0)mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]&0xff00;
			if(tip==1)mas_lz[pek][i+ri*4]=mas_lz[pek][i+ri*4]&0xff;
		}
	}
	return(jk);
}
//--------------------------------------------------------------------------
/****************************************************\
* Процедура фиксации ненорм по модулю М201-3         *
* pek- канал получения данных о ненорме              *
* ri- номер байта в числе информационных в сообщении *
* blum - буфер сообщения, полученного от УВК         *
* tip - тип события 0-нет нуля 1- нет единицы        *
\****************************************************/
obnov_201_3(int pek,int ri,unsigned char blum[12],int tip)
{ int i,jk;
	unsigned char iok;

	nom_func("190");
	if(ri>3)return(0);
	jk=0;
	for(i=0;i<4;i++)           //проверка битов в указанном байте
	{ iok=blum[4+ri]&(1<<i);
		if(iok!=0)               //если бит равен единице
		{
			if(tip==0)             //если фиксируется отсутствие нуля
			{
        //увеличить счетчик ненорм по нулям для разряда i+ri*4
        //для нулевого байта разряды от 0 до 3
        //для первого байта разряды от 4 до 7
        //для второго байта разряды от 8 до 11
        //для третьего байта разряды от 12 до 15
				mas_lc[pek][i+ri*4]++;

				if((mas_lc[pek][i+ri*4]&0xff)==3)jk=1;  //если ненорм стало три
                                                //установить признак ненорм

				if((mas_lc[pek][i+ri*4]&0xff)>3)        //если ненорм больше трех
				{
          //сбросить счет ненорм нулей
					mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]&0xff00;

          //установить счетчик ненорм нулей в значение 4
					mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]|4;
				}
			}
			if(tip==1)         //если фиксируется отсутствие единицы
			{
        //увеличить счет ненорм по единицам
				mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]+0x0100;

        //если ненорм стало три, установить признак ненорм
				if((mas_lc[pek][i+ri*4]&0xff00)==0x300)jk=1;

        //если ненорм по единицам более трех
				if((mas_lc[pek][i+ri*4]&0xff00)>0x300)
				{
          //сбросить счет ненорм нулей
					mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]&0xff;

          //установить счет ненорм нулей в значение 4
					mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]|0x400;
				}
			}
		}
		else  //если бит равен нулю
		{
      //сбросить счет ненормовых нулей
			if(tip==0)mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]&0xff00;

      //сбросить счет ненормовых единиц
			if(tip==1)mas_lc[pek][i+ri*4]=mas_lc[pek][i+ri*4]&0xff;
		}
	}
	return(jk);
}
//-------------------------------------------------------------------------
/****************************************************\
* Процедура фиксации ненорм  по модулю М201-4        *
* pek- канал получения данных о ненорме              *
* ri- номер байта в числе информационных в сообщении *
* blum - буфер сообщения, полученного от УВК         *
* tip - тип события 0-нет нуля 1- нет единицы        *
* комментарии см. в процедуре obnov_lc               *
\****************************************************/
obnov_201_4(int pek,int ri,unsigned char blum[12],int tip)
{ int i,jk;
	unsigned char iok;

	nom_func("191");
	if(ri>3)return(0);
	jk=0;
	for(i=0;i<4;i++) //проверка битов в указанном байте
	{ iok=blum[4+ri]&(1<<i);
		if(iok!=0)
		{
			if(tip==0)
			{
				mas_ls[pek][i+ri*4]++;
				if((mas_ls[pek][i+ri*4]&0xff)==3)jk=1;
				if((mas_ls[pek][i+ri*4]&0xff)>3)
				{
					mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]&0xff00;
					mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]|4;
				}
			}
			if(tip==1)
			{
				mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]+0x0100;
				if((mas_ls[pek][i+ri*4]&0xff00)==0x300)jk=1;
				if((mas_ls[pek][i+ri*4]&0xff00)>0x300)
				{
					mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]&0xff;
					mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]|0x400;
				}
			}
		}
		else
		{
			if(tip==0)mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]&0xff00;
			if(tip==1)mas_ls[pek][i+ri*4]=mas_ls[pek][i+ri*4]&0xff;
		}
	}
	return(jk);
}
//-------------------------------------------------

void read_text1(int QQ)
{
	int kodd=0;

	nom_func("276");
	sfs=fopen("dat\\text1.img","r");
	if(sfs==NULL) return;
aa:
	fscanf(sfs,"%d",&kodd);
	while(fgetc(sfs)!='\n');
	switch(kodd)
	{
		case 999: break;
		case 1: read_slova(QQ);goto aa;
		case 22: electri_platf(QQ); goto aa;
		case 2: ramki(0);goto aa;
		case 6: elo(QQ); goto aa;
		case 3: read_pereezd(QQ); goto aa;
		case 5: read_dzezli(QQ);  goto aa;
		case 4: read_smen(QQ); goto aa;
		case 7: figura(7); goto aa;
		case 8: figura(8); goto aa;
		case 9: ramki(1);goto aa;
		default:break;
	}
	if(sfs!=NULL)fclose(sfs);
	sfs=NULL;
	return;
}
/******************************************************************/
void read_slova(int SD)
{
	int symb=0;
	int x,y,sty,cv,gk;
	char slovo[60]="";
	char slo[60]="";

	nom_func("272");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
	while(symb!='#')
	{
		fscanf(sfs,"%d%d%d%d",&x,&y,&sty,&cv);
		x=x+SD;
		for(gk=0;gk<60;gk++)slovo[gk]=0;
		for(gk=0;gk<20;gk++)slo[gk]=0;
		fscanf(sfs,"%s",slo);
		for(gk=0;gk<strlen(slo);gk++)
		if(slo[gk]=='_') slovo[gk]=' ';
		else slovo[gk]=slo[gk];
		moveto(x,y);
		if(sty==17)settextstyle(USER_FONT,1,0);
		if(sty==1)setusercharsize(3,4,1,1);
		setcolor(cv);outtext(slovo);
		settextstyle(USER_FONT,0,0);
		setusercharsize(1,1,1,1);
		fgetc(sfs);
		symb=fgetc(sfs);
		if(symb!='@') break;
	}
	return;
}
//-------------------------------------------------------------
/*******************************************************\
*  Процедура изображения на плане станции платформы по  *
*  данным файла text1.img - рисуется однократно         *
\*******************************************************/
void electri_platf(int SD)
{
	int xkv,ykv,dxkv,dykv,xgol,ygol,sty,cv,dxp,dxm,dym,dyp,symb=0;

	nom_func("48");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
  while(symb!='#')
  {
		fscanf(sfs,"%d %d %d %d %d %d %d %d %d %d %d %d",
    &sty,&cv,&xkv,&ykv,&dxkv,&dykv,&xgol,&ygol,&dxm,&dxp,&dym,&dyp);
    xkv=xkv+SD;xgol=xgol+SD;
    setcolor(cv);
    setlinestyle(0,0,sty);
    rectangle(xkv,ykv,xkv+dxkv,ykv+dykv);
		circle(xgol,ygol,1);
		circle(xgol,ygol,0);
    line(xgol+dxm,ygol+dym,xgol+dxp,ygol+dyp);
		fgetc(sfs);
		symb=fgetc(sfs);
    if(symb!='@') break;
	}
	return;
}
//---------------------------------------------------------
/******************************************************\
* Процедура построения прямоугольных рамок или линий,  *
* заданных координатами, считанными из файла text1.img *
* для строк с ключом #2-рамки или #9-линии             *
\******************************************************/
void ramki(int lines)
{
	int xkv,ykv,dxkv,dykv,sty,cv,wi;
	int symb=0;
	char znak=0;

	nom_func("261");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
	while(symb!='#')
  {
		fscanf(sfs,"%d %d %d %d %d %d %d",&wi,&sty,&cv,&xkv,&ykv,&dxkv,&dykv);
    setlinestyle(wi,0,sty);
    setcolor(cv);
    if(lines==0)rectangle(xkv,ykv,xkv+dxkv,ykv+dykv);
    else line(xkv,ykv,xkv+dxkv,ykv+dykv);
		while(znak!=10)znak=fgetc(sfs);
		symb=fgetc(sfs);
    if(symb!='@') break;
  }
	return;
}
//--------------------------------------------------------
/*****************************************************\
* Процедура изображения любой прямой линии по данным  *
* из файла text1.img. Рисуется однократно при загрузке*
\*****************************************************/
void elo(int SD)
{
	int xkv,ykv,dxkv,dykv,sty,cv;
	int symb=0;

	nom_func("49");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
	while(symb!='#')
	{
		fscanf(sfs,"%d %d %d %d %d %d",&sty,&cv,&xkv,&ykv,&dxkv,&dykv);
		xkv=xkv+SD;
		setcolor(cv);
		setlinestyle(0,0,sty);
		line(xkv,ykv,dxkv,dykv);
		fgetc(sfs);
		symb=fgetc(sfs);
		if(symb!='@') break;
	}
	return;
}
/*****************************************************************/
void read_smen(int SD)
{
	int o,symb=0;

	nom_func("273");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
  while(symb!='#')
  {
	 for(o=0;o<=13;o++)fscanf(sfs,"%d ",&left_up_left[o]);
	 while(fgetc(sfs)!='@');
	 for(o=0;o<=13;o++)fscanf(sfs,"%d ",&left_up_right[o]);
	 while(fgetc(sfs)!='@');
	 for(o=0;o<=13;o++)fscanf(sfs,"%d ",&right_up_left[o]);
	 while(fgetc(sfs)!='@');
	 for(o=0;o<=13;o++)fscanf(sfs,"%d ",&right_up_right[o]);
	 fgetc(sfs);
	 symb=fgetc(sfs);
   if(symb!='@') break;
  }
  if(SD!=0)
  {
    for(o=0;o<13;o=o+2)
    {
      left_up_left[o]=left_up_left[o]+SD;
      left_up_right[o]=left_up_right[o]+SD;
      right_up_left[o]=right_up_left[o]+SD;
      right_up_right[o]=right_up_right[o]+SD;
    }
  }
	return;
}
/*********************************************/
void read_dzezli()
{
	int x1,y1,o,symb=0;

	nom_func("267");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
	while(symb!='#')
	{
		fscanf(sfs,"%d %d %d ",&o,&x1,&y1);
		fr3[o][6]=x1; fr3[o][9]=y1;
		symb=fgetc(sfs);
		if(symb!='@') break;
	}
	return;
}
//--------------------------------------
void read_pereezd()
{
int symb=0,o,x1p,y1p,dli;

	nom_func("271");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
	while(symb!='#')
	{
	 fscanf(sfs,"%d %d %d %d ",&o,&x1p,&y1p,&dli);
		//o- объект с информацией о переезде
		//x1p - координата x
		//y1p - координата y
		//dli - длина переезда
	 fr1[o][4]=x1p; fr1[o][5]=y1p; fr1[o][6]=dli;
	 fgetc(sfs);
	 symb=fgetc(sfs);
	 if(symb!='@') break;
	}
	return;
}
//---------------------------------------------------------
/********************************************************\
* Процедура изображения на экране контурной или сплошной *
* фигуры, заданной в файле text1.img. Изображается только*
* при выполнении загрузки или обновлении экрана по F5.   *
* tip=8 - контурная фигура, tip=7 - залитая фигура.      *
\********************************************************/
void figura(int tip)
{
	int poli[20],cv,symb=0,i;

	nom_func("54");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	symb=fgetc(sfs);
	while(symb!='#')
	{
		symb=fgetc(sfs);
		fscanf(sfs,"%d",&symb);
		for(i=0;i<2*symb;i++)fscanf(sfs,"%d",&poli[i]);
		fscanf(sfs,"%d",&cv);
		setcolor(cv);
		if(tip==8){setfillstyle(0,cv);drawpoly(symb,poli);}
		if(tip==7){setfillstyle(1,cv);fillpoly(symb,poli);}
		fgetc(sfs);
		symb=fgetc(sfs);
		if(symb!='@') break;
	}
	return;
}
//--------------------------------------------------------------------
/********************************************\
* Процедура поиска измененного, но не зафик- *
* сированного бита bt_ и имени соответствую- *
* щего объекта nm. При удачном поиске возвра-*
* щает 0. Имя объекта в переменной chudo.    *
\********************************************/
int get_str(int nm,int bt_)
{
	int g1,co_pro=0,h1,dlip=0,mn=0;
	char loo[40]="",chu[20]="";

	nom_func("99");
	if(bt_==5)return(1);
	strcpy(loo,pako[nm]); //скопировать строку имен
	dlip=strlen(loo);//получить длину всей строки
	g1=0;//установить указатель в начало строки
	while(co_pro<bt_)//двигаться по строке, пока пробелов меньше номера бита
	{ if(g1>dlip)return(1); //выход с неудачей, если строка кончилась
		if(loo[g1]==' ') co_pro++;//если найден пробел, добавить к счетчику
		g1++;
	}
	for(mn=0;mn<=19;mn++)chu[mn]=0;//очистить выходной массив
	strcpy(chudo,"");
	h1=0;
	while(1)//цикл заполнения выходного массива именем объекта
	{ //для последнего выход по размеру
		if(g1>dlip){strcpy(chudo,chu); break;}
		//для прочих выход по пробелу
		if(loo[g1]==' '){strcpy(chudo,chu);break;}
		//добавить одну букву и перейти к следующей
		chu[h1]=loo[g1];h1++;g1++;
	}
	return(0);
}
/***********************************************************************\
* Процедура обновления или изменения фона текстового кнопочного объекта *
*   x,y - координаты начала объекта на экране                           *
*   D - ширина объекта в пикселах                                       *
*   name_obj[15] - экранное имя объекта                                 *
*   new_cvt - требуемый цвет фона                                       *
\***********************************************************************/
void 	obnovi_obraz(int x,int y,int D,char name_obj[15],int new_cvt)
{
	int size_im,          //размерность образа
	l_pix,                //ширина образа в пикселах
	l_byte,               //ширина образа в байтах
	h_pix,                //высота образа в пикселах
	ii,jj,ll,kk,nn,mm,pp, //вспомогательные переменные
	old_cvt;              //цвет пикселов, взятых с экрана

	setcolor(8);
	outtextxy(x,y+2,name_obj);
	size_im=imagesize(x,y,x+D,y+12);
	if(size_im>800)return;
	getimage(x,y,x+D,y+12,ARROW);
	l_pix=ARROW[0]+1; //число пикселов для строки образа
	l_byte=l_pix%8;
	if(l_byte==0)l_byte=l_pix/8;
	else l_byte=l_pix/8+1; //число байт для строки образа
	h_pix=ARROW[2]; //число пикселов в колонке образа

	for(jj=0;jj<(h_pix-1);jj++) //пройти по строкам образа
	for(ii=0;ii<(l_byte);ii++) //пройти по байтам строки
	for(ll=0;ll<8;ll++)//пройти по битам байта
	{ old_cvt=0;
		for(kk=0;kk<4;kk++)//пройти по слоям цвета
		{
			nn=jj*l_byte*4+kk*l_byte+ii;
			if(ARROW[4+nn]&(0x80>>ll))old_cvt=old_cvt+(8>>kk);
		}
		if(old_cvt!=8)//если это не текст и не пустота
		{
			for(mm=0;mm<4;mm++)
			{
				pp=jj*l_byte*4+mm*l_byte+ii;
				ARROW[4+pp]=ARROW[4+pp]&(~(0x80>>ll)); //сбросить установленные
				if(new_cvt&(8>>mm))
				{
					ARROW[4+pp]=ARROW[4+pp]|(0x80>>ll);
				}
			}
		}
	}
	putimage(x,y,ARROW,0);
	return;
}
/*********************************************************\
* Процедура обновления или изменения фона текстовой лампы *
*   x,y - координаты начала текста объекта на экране      *
*   D - ширина текста объекта в пикселах                  *
*   name_obj[15] - экранное имя объекта                   *
*   new_cvt - требуемый цвет фона                         *
\*********************************************************/
void 	obnovi_obraz1(int x,int y,int D,char name_obj[15],int new_cvt)
{
	int size_im,          	 	//размерность образа
	l_pix,                	 	//ширина образа в пикселах
	l_byte,              		 	//ширина образа в байтах
	h_pix,                	 	//высота образа в пикселах
	ii,jj,ll,kk,nn,mm,pp,qq, 	//вспомогательные переменные
	old_cvt;              			//цвет пикселов, взятых с экрана

	setcolor(8);
	outtextxy(x,y+2,name_obj);
	size_im=imagesize(x-5,y,x+D+5,y+12);
	if(size_im>600)return;
	getimage(x-4,y,x+D+5,y+12,ARROW);
	l_pix=ARROW[0]+1; //число пикселов для строки образа
	l_byte=l_pix%8;
	if(l_byte==0)l_byte=l_pix/8;
	else l_byte=l_pix/8+1; //число байт для строки образа
	h_pix=ARROW[2]; //число пикселов в колонке образа
	for(jj=0;jj<(h_pix-1);jj++) //пройти по строкам образа
	{ for(ii=0;ii<(l_byte);ii++) //пройти по байтам строки
		for(ll=0;ll<8;ll++)//пройти по битам байта
		{ old_cvt=0;
			for(kk=0;kk<4;kk++)//пройти по слоям цвета
			{
				nn=jj*l_byte*4+kk*l_byte+ii;
				if(ARROW[4+nn]&(0x80>>ll))old_cvt=old_cvt+(8>>kk);
			}
			//jj - текущая строка образа
			//qq - текущая колонка образа
			qq=(ii<<3)+ll;
			if(((jj>0)&&(jj<5)&&(qq>=(5-jj))&&(qq<(5+D+jj)))||//если верхняя половина
			((jj>0)&&(jj>5)&&(qq>=(jj-5))&&(qq<(15+D-jj)))||//если нижняя половина
			(jj==5))//если середина
			{
				if(old_cvt!=8)//если это не текст и не пустота
				{
					for(mm=0;mm<4;mm++)//пройти по слоям цвета
					{
						pp=jj*l_byte*4+mm*l_byte+ii;
						ARROW[4+pp]=ARROW[4+pp]&(~(0x80>>ll)); //сбросить установленные
						if(new_cvt&(8>>mm))
						{
							ARROW[4+pp]=ARROW[4+pp]|(0x80>>ll);
						}
					}
				}
			}
		}
	}
	putimage(x-4,y,ARROW,0);
	return;
}