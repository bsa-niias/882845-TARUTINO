#include"bla.h"
//--------------------------------------------------------------------
/********************************************\
* Процедура управления отображением стрелок  *
\********************************************/
void displaystrelka(int oi,int bt_,int povtor)
{
	int ui=0,xl,yl,N_sp,sp_s,sp_s1,zm_sp,ris_krest,i_m,i_s,s_m;
	nom_func("42");
	if(fr3[oi][5]==1)//если непарафазность
	{
		if(tst_fix(oi,5)){slom_interf(oi);fix(oi,5);}
	}
	if(fr3[oi][4]==1)//если есть потеря контроля
	{
		if(tst_fix(oi,4))//если нет реакции оператора и не было записи на диск
		{
#ifdef AVTOD
			if(na==1)//если установлено нечетное автодействие
			{
        //пройти по списку стрелок маршрута нечетного автодействия
				for(ui=0;ui<N_str;ui++)
				{
					if(N_strel[ui]==oi)//если данная стрелка в этом списке
					{
						snyato_avto(1); //снять нечетное автодействие
						w(149,oi,"");
						zvuk_vkl(0,0);
					}
				}
			}
			if(cha==1)//если установлено четное автодействие
			{
        //пройти по списку стрелок маршрута четного автодействия
				for(ui=0;ui<CH_str;ui++)
				{
					if(C_strel[ui]==oi)//если данная стрелка в этом списке
					{
						snyato_avto(2); //снять четное автодействие
						w(149,oi,"");
						zvuk_vkl(0,0);
					}
				}
			}
#endif
			if(povtor==0) //если состояние с новизной
			{
				for(ui=0;ui<20;ui++)chudo[ui]=0;
				ui=0;
				while(pako[oi][ui]!=':')chudo[ui]=pako[oi][ui++];
				w(127,999,chudo);//НЕ ИМЕЕТ КОНТРОЛЯ СТРЕЛКА
				strcat(chudo,"-без_к.");
				neisprav_na_disk('#',chudo);
				zvuk_vkl(3,0);
			}
			fix(oi,4);
		}
	}
	if(fr3[oi][4]==0)//если восстановлен контроль и первая стрелка
	{
		if(tst_fix(oi,4)&&(povtor==0))
		{
			for(ui=0;ui<20;ui++)chudo[ui]=0;
			ui=0;
			while(pako[oi][ui]!=':')chudo[ui]=pako[oi][ui++];
			w(227,999,chudo);//ВОССТАНОВЛЕН КОНТРОЛЬ СТРЕЛКИ
			strcat(chudo,"-есть_к.");
			neisprav_na_disk('#',chudo);
			zvuk_vkl(0,0);
		}
		fix(oi,4);
	}
	if(bt_==3)//если бит неустановки стрелки
	{
		if(prorisovka==0)
		{
			if(tst_fix(oi,bt_))//если нет реакции оператора и не было записи на диск
			{
				if(fr3[oi][3]==1)//если есть неустановка и первая стрелка
				{
					for(i_m=0;i_m<(skoko_stoek*3);i_m++)
					{
						for(i_s=0;i_s<skoko_stoek;i_s++)
						{
							for(s_m=0;s_m<20;s_m++)
							{
								if((MARSHRUT_ALL[i_m].STREL[i_s][s_m]&0xfff)==oi)
								DeleteMarsh(i_m);
							}
						}
					}
					w(99,oi," НЕ ПЕРЕВЕДЕНА");//СТРЕЛКА НЕ УСТАНОВЛЕНА
					for(ui=0;ui<20;ui++)chudo[ui]=0;
					ui=0;
					while(pako[oi][ui]!=':')chudo[ui]=pako[oi][ui++];
					strcat(chudo,"-не_пер.");
					neisprav_na_disk('#',chudo);
					zvuk_vkl(3,0);
					if(DISK!=0)return;
				}
			}
			fix(oi,bt_);
		}
	}
	//если не биты парафазности и положения, то закончить
	if((bt_!=5)&&(bt_>1))goto vyh;

	//если на экране выводится не путевое развитие, то закончить
	if((nikuda==1)||(klo==1)||(help!=0))goto vyh;

	if(fr2[oi][0]!=0)//если стрелка рисуется
	{
		if(fr1[oi][6]!=0)//если крестовая 
		{
			sp_s=(fr1[oi][6]&0xff00)>>8;  //получить объект для СП этой стрелки 
			ris_krest=fr1[oi][6]&0xff;
			sp_s1=(fr1[ris_krest&0xfff][6]&0xff00)>>8;
			zm_sp=fr3[sp_s][1]|fr3[sp_s1][1];//получить значение замыкания
			ris_krest=0;
			krest(sp_s,1,zm_sp);
		}
//		else //для обычных стрелок
		{
			zap_matr(oi);  //заполнить матрицу объединения СП-стрелки
			cvet_matr();   //заполнить матрицу цветов-толщины
			ris_sp_str();  //нарисовать сегмент СП-стрелки
		}
		N_sp=SP_STR[0][0];
		setcolor(8);
		if((fr1[oi][10]&8)==8)//если охранная стрелка плюсовая
		{
			if(((fr3[oi][0]!=1)||(fr3[oi][1]!=0))&&(fr3[N_sp][1]!=1))setcolor(12);
		}
		if((fr1[oi][10]&4)==4)//если охранная стрелка минусовая
		{
			if((fr3[oi][0]!=0)||(fr3[oi][1]!=1)&&( fr3[N_sp][1]!=1))setcolor(12);
		}
    //если охранная стрелка, без автовозврата но с индикацией
		if(((fr1[oi][10]&4)==4)||((fr1[oi][10]&8)==8))
		{
			//если ветвление вниз
			if(fr1[oi][9]==0)yl=fr2[oi][1]-10;

			//если ветвление вверх
			if(fr1[oi][9]==1)yl=fr2[oi][1]+10;

			//если вход на стрелку четный и четная сторона слева
			if((fr1[oi][7]==0)&&(OBRAT==1))xl=fr2[oi][0]-3;

			//если вход на стрелку нечетный и четная сторона справа
			if((fr1[oi][7]==1)&&(OBRAT==-1))xl=fr2[oi][0]+3;

			//если вход на стрелку четный и четная сторона справа
			if((fr1[oi][7]==0)&&(OBRAT==-1))xl=fr2[oi][0]-3;

			//если вход на стрелку четный и четная сторона слева
			if((fr1[oi][7]==1)&&(OBRAT==1))xl=fr2[oi][0]+3;
			for(ui=3;ui>0;ui--)circle(xl,yl,ui);
		}
	}
	else //если стрелка не рисуется, но положение отображают лампочки
	{
		setlinestyle(0,0,0);
		if(fr2[oi][12]!=0)
		{
			if(fr3[oi][5]==1) //если  непарафазность
      {
				setcolor(11);
				for(ui=4;ui>0;ui--)circle(fr2[oi][12],fr2[oi][13],ui);
				for(ui=4;ui>0;ui--)circle(fr2[oi][14],fr2[oi][15],ui);
      }
      else
      {
		  	if(fr3[oi][0]==1) //если стрелка в плюсе
				{
					if(fr3[oi][1]==0) //и стрелка не в минусе
					{
						setcolor(10);//зеленая лампа плюса
						for(ui=4;ui>0;ui--)circle(fr2[oi][12],fr2[oi][13],ui);
	          setcolor(8);//серая лампа минуса
						for(ui=4;ui>0;ui--)circle(fr2[oi][14],fr2[oi][15],ui);
    	    }
      		else //если стрелка и в плюсе и в минусе одновременно
					{
						setcolor(13); //фиолетовые лампы плюса и минуса
						for(ui=4;ui>0;ui--)circle(fr2[oi][12],fr2[oi][13],ui);
						for(ui=4;ui>0;ui--)circle(fr2[oi][14],fr2[oi][15],ui);
    	    }
	    	}
	  	  if(fr3[oi][0]==0) //если стрелка не в плюсе
				{
					if(fr3[oi][1]==1) //и стрелка в минусе
					{
						setcolor(8); //серая лампа плюса
						for(ui=4;ui>0;ui--)circle(fr2[oi][12],fr2[oi][13],ui);
	          setcolor(14);//желтая лампа минуса
						for(ui=4;ui>0;ui--)circle(fr2[oi][14],fr2[oi][15],ui);
      	  }
		      else //если стрелка не в плюсе и не в минусе
					{
						setcolor(13); //фиолетовые лампы плюса и минуса
						for(ui=4;ui>0;ui--)circle(fr2[oi][12],fr2[oi][13],ui);
						for(ui=4;ui>0;ui--)circle(fr2[oi][14],fr2[oi][15],ui);
		      }
  		  }
    	}
  	}
  }
vyh:
	return;
}
#ifdef AVTOVOZVRAT
//---------------------------------------------------------------------
/*******************************************************\
* Процедура проверки стрелки на наличие автовозврата    *
* и выдачи команды автовозврата при необходимости       *
* strf - объект для стрелки                             *
* uchf - объект для охранного участка СП                *
* mspf - объект для МСП                                 *
* m_b - номер бита для объекта МСП                      *
* param - признак объекта, по которому прошло изменение *
\*******************************************************/
void prov_av_str(int strf,int uchf,int mspf,int m_b,int param)
{
	int x,y,ra,wr,x_kir,y_kir,kod,lu,b_lu,MSP,B_MSP,ij,hod;
	nom_func("241");
	if((klo==1)||(help>0)||(nikuda==1))return;
	wr=0;hod=0;
proc:
	switch(param)
	{
		case 2: //если произошло изменение СП
						if(hod==0)
						{
							for(wr=wr;wr<Kol_avstr;wr++)// по всем стрелкам с автовозвратом
							if(coAV[wr].N_spu1==uchf)break;//если данное СП участвует-выйти
						}
						if(wr>=Kol_avstr)//если СП не найдено
						{
							hod=1;
							for(wr=0;wr<Kol_avstr;wr++)//поискать во втором СП
							if((coAV[wr].N_spu2==uchf)&&(coAV[wr].N_spu2!=0))break;
						}
						break;
		case 4: //если произошло изменение МСП
						for(wr=wr;wr<Kol_avstr;wr++)//поискать МСП
						{
							MSP=coAV[wr].MSP1&0xfff;
							B_MSP=(coAV[wr].MSP1&0xf000)>>12;
							if((MSP==mspf)&&(B_MSP==m_b)) break;
						}
						break;
	 default: return;
	}
	if(wr>=Kol_avstr)return;//если данная стрелка без автовозврата, выйти
	x=coAV[wr].Sx;// для стрелок с автовозвратом получить координаты лампочки
	y=coAV[wr].Sy;
	ra=coAV[wr].Sra;//взять радиус лампочки
	strf=coAV[wr].N_sst;//получить стрелку с автовозвратом
	lu=coAV[wr].LU&0xfff;//получить объект ЛУ
	b_lu=(coAV[wr].LU&0xf000)>>12;//получить бит ЛУ
kirp:
	//если стрелка парная и сбрасывающая - найти вторую

	kod=0;
	if((fr1[strf][10]&8)==8)
	{
//    if(fr1[strf][12]==0)strf=poisk_ras(fr1[strf][2],strf);
		kod=fr1[strf][7]+OBRAT*2;//получить код входа
		switch(kod)
		{
			case -2:  //в зависимости от ориентации получить координаты "кирпича"
			case 3:   if(fr2[strf][0]<fr2[strf][2])
								{
									x_kir=fr2[strf][0]; y_kir=fr2[strf][1];ij=1;break;
								}
								else
								{
									x_kir=fr2[strf][2]; y_kir=fr2[strf][3];ij=1; break;
								}
			case 2:
			case -1:  if(fr2[strf][0]<fr2[strf][2])
								{
									x_kir=fr2[strf][2]; y_kir=fr2[strf][3];ij=-1; break;
								}
								else
								{
									x_kir=fr2[strf][0]; y_kir=fr2[strf][1];ij=-1; break;
								}
			default:  break;
		}

	}
	//если стрелка в плюсе или в минусе,и соответствует охранному
	if((fr3[coAV[wr].N_sst][0]==coAV[wr].b0)&&(fr3[coAV[wr].N_sst][1]==coAV[wr].b1))
	{ //сбросить все флаги и изобразить красный кирпич в тупике
		coAV[wr].Fl_kom=0;
		setfillstyle(1,12);
		if(kod!=0)bar(x_kir-4*ij,y_kir-3,x_kir-1*ij,y_kir+3);
		setcolor(8);//установить серый цвет
	}
	else //если положение не охранное
	{ setfillstyle(1,7);
		if(kod!=0)bar(x_kir-4*ij,y_kir-3,x_kir-1*ij,y_kir+3);//стереть кирпич
		if((fr1[strf][10]&16)!=16)goto risun;//если нет автовозврата - рисовать
// если одно СП
		if(coAV[wr].N_spu2==0)
		{ //если СП замкнуто
			if(fr3[coAV[wr].N_spu1][1]==1)
			//взвести флаг команды и установить серый цвет
			{ coAV[wr].Fl_kom=1; setcolor(8);}
			else//если СП разомкнуто, то проверить МСП
			{ MSP=coAV[wr].MSP1&0xfff;B_MSP=(coAV[wr].MSP1&0xf000)>>12;
msp:    if(fr3[MSP][B_MSP]==1)//если МСП не готово установить желтый цвет
				{ setcolor(14);
					if(fr3[lu][b_lu]==1)coAV[wr].Fl_kom=0;//если ЛУ - сбросить флаг
				}
				else //если МСП готово
				{
					setcolor(12);
					if(coAV[wr].Fl_kom==1)
					{

						if(pooo[coAV[wr].N_sst]==0)
						{
							pooo[coAV[wr].N_sst]=biostime(0,0l);
						}
						else
						if((biostime(0,0l)-pooo[coAV[wr].N_sst])>18l)
						{
							coAV[wr].Fl_kom=2;
							pooo[coAV[wr].N_sst]=0;
							com_avtovozvr(wr);
							setcolor(12);
						}
					}
				}
			}
		}
//если есть второе СП
		else
		{ //если оба СП замкнуты
			if((fr3[coAV[wr].N_spu1][1]==1)&&(fr3[coAV[wr].N_spu2][1]==1))
			{coAV[wr].Fl_kom=1;setcolor(8);}
			else
				 //если замкнуто только второе
				if((fr3[coAV[wr].N_spu1][1]==0)&&(fr3[coAV[wr].N_spu2][1]==1))
				{coAV[wr].Fl_kom=1;setcolor(8);}
				else
					 //если замкнуто только первоое
					if((fr3[coAV[wr].N_spu1][1]==1)&&(fr3[coAV[wr].N_spu2][1]==0))
					{coAV[wr].Fl_kom=1;setcolor(8);}
					//если все разомкнулось
					else
					{
						MSP=coAV[wr].MSP1&0xfff;
						B_MSP=(coAV[wr].MSP1&0xf000)>>12;
						if(fr3[MSP][B_MSP]==1)//если МСП1 не готово
						{
							setcolor(14);//установить желтый цвет
							if(fr3[lu][b_lu]==1)coAV[wr].Fl_kom=0;//если ЛУ - сбросить флаг
						}
						else//если 1-ое МСП готово
						{
							MSP=coAV[wr].MSP2&0xfff;
							B_MSP=(coAV[wr].MSP2&0xf000)>>12;
							if((fr3[MSP][B_MSP]==1)&&(MSP!=0))//если МСП2 не готово
							{
								setcolor(14);//установить желтый цвет
								if(fr3[lu][b_lu]==1)coAV[wr].Fl_kom=0;//если ЛУ - сбросить флаг
							}
							else //если МСП готовы
							{
								setcolor(12);
								if(coAV[wr].Fl_kom==1)
								{
									if(pooo[coAV[wr].N_sst]==0)pooo[coAV[wr].N_sst]=biostime(0,0l);
									else
									if((biostime(0,0l)-pooo[coAV[wr].N_sst])>18l)
									{
										pooo[coAV[wr].N_sst]=0;
										coAV[wr].Fl_kom=2;
										com_avtovozvr(wr);
										setcolor(12);
									}
								}
							}
						}
					}
		}
	}
risun:
	if((fr3[strf][5]==1)||(fr3[coAV[wr].N_spu1][5]==1)||
	(fr3[coAV[wr].MSP1&0xfff][5]==1))setcolor(LIGHTCYAN);
	if(coAV[wr].N_spu2!=0)
	if(fr3[coAV[wr].N_spu2&0xfff][5]==1) setcolor(LIGHTCYAN);
	setlinestyle(0,0,0);
	if((fr1[strf][10]&16)==16)for(ij=0;ij<ra;ij++)circle(x,y,ij);
	if(wr<(Kol_avstr-1)){wr++;goto proc;}
	return;
}
#endif
//-------------------------------------------------------------------------
/**********************************************\
* Процедура поиска объекта за переходом или    *
* по минусу стрелки                            *
* nomer - номер объекта, указанный переходом   *
* otkuda - номер объекта, откуда идет переход  *
\**********************************************/
int poisk_ras(int nomer,int otkuda,int tipus,int uk_tras)
{
	int dvig,klp=9999,klop=9999,tstr;
	nom_func("231");
	klop=nomer;
	if(fr1[klop][0]==6) // если это переход
	{ //если ссылка туда откуда пришли, то записать в трассу.
		//это значит, что на переход вышли не по строке базы, а по ссылке
		//с другого перехода или со стрелки.В этом случае надо выйти на
		//строку базы.
		if(fr1[klop][1]==otkuda)
		{
			if(uprav!=0)
			{
				if(tipus!=0)
				{
					if(TRASSA[uk_tras].object==0xFFFF)
					{
						TRASSA[uk_tras].object=klop;
						TRASSA[uk_tras].tip=fr1[klop][0];
						TRASSA[uk_tras++].stoyka=fr1[klop][13];
					}
				}
				else
				{
					if(TRASSA0[uk_tras].object==0xFFFF)
					{
						TRASSA0[uk_tras].object=klop;
						TRASSA0[uk_tras].tip=fr1[klop][0];
						TRASSA0[uk_tras++].stoyka=fr1[klop][13];
					}
				}
aa:   	if(uk_tras!=0)
				{
					tstr=0;
					return(klop);
				}
			}
			klp=otkuda;
			//выявить направление движения
			if((fr1[klp][8]&0x8000)!=0)dvig=-1;
			else dvig=1;
			//продвинуться далее
			klop=klop+dvig;

			// если это ДЗ и делается поворот
			if((fr1[klop][0]==7)&&(FLAG_PO==1)&&(TEST_SPAR!=2))
			{ // если маршрутное управление, то запомнить элемент
				// в основной или вариантной трассе
				if(uprav==1)
				{ if(TEST_SPAR==0)
					{
						if(tipus!=0)
						{
							TRASSA[uk_tras].object=klop;
							TRASSA[uk_tras].tip=fr1[klop][0];
							TRASSA[uk_tras++].stoyka=fr1[klop][13];
						}
						else
						{
							TRASSA0[uk_tras].object=klop;
							TRASSA0[uk_tras].tip=fr1[klop][0];
							TRASSA0[uk_tras++].stoyka=fr1[klop][13];
						}
					}
				}
				//для раздельн. управления просто запомнить элемент в трассе
				else
				{
					if(tipus!=0)
					{
						TRASSA[uk_tras].object=klop;
						TRASSA[uk_tras].tip=fr1[klop][0];
						TRASSA[uk_tras++].stoyka=fr1[klop][13];
					}
					else
					{
						TRASSA0[uk_tras].object=klop;
						TRASSA0[uk_tras].tip=fr1[klop][0];
						TRASSA0[uk_tras++].stoyka=fr1[klop][13];
					}
				}
			}
			//если вышли на стрелку, сигнал, СП, УП или путь, то выйти с номером
			if((fr1[klop][0]>=1)&&(fr1[klop][0]<=5))return(klop);

			//если не переход (для ДЗ), то повторить поиск
			if(fr1[klop][0]!=6) goto aa;
			else
			{ //для перехода запомнить его в трассе и получить значение
				//следующего объекта, с этим объектом и выйти
				if(TEST_SPAR==0)
				{
					if(tipus!=0)
					{
						if(TRASSA[uk_tras].object==0xFFFF)
						{
							TRASSA[uk_tras].object=klop;
							TRASSA[uk_tras].tip=fr1[klop][0];
							TRASSA[uk_tras++].stoyka=fr1[klop][13];
						}
					}
					else
					{
						if(TRASSA0[uk_tras].object==0xFFFF)
						{
							TRASSA0[uk_tras].object=klop;
							TRASSA0[uk_tras].tip=fr1[klop][0];
							TRASSA0[uk_tras++].stoyka=fr1[klop][13];
						}
					}
				}
				klop=fr1[klop][1];
				return(klop);
			}
		}
		//если ссылка перехода не совпадает с объектом, то взять эту ссылку
		//и выйти с ней, это значит, что на переход вышли по строке базы, а
		//не по ссылке.
		else klop=fr1[klop][1];
	}
	return(klop);
}
#ifdef AVTOVOZVRAT
//----------------------------------------------------------------
/*************************************************\
* Процедура подготовки и выдачи команды на авто-  *
* возврат стрелки. Данная процедура вызывается из *
* обработки секции при ее размыкании или из обра- *
* ботки МСП, при ее завершении.                   *
\*************************************************/
void com_avtovozvr(int wr)
{
	int poH;
	if(coAV[wr].Fl_kom>1)                  //если флаг автовозврата взведен
  { AVARIIN=1;
    point=coAV[wr].N_sst;                //взять стрелку
		if(pooo[point]!=0l)return;
		if(coAV[wr].b0==1)poH=1;             //взять охранное положение
		else poH=-1;
		if((TELEUP==1)&&(STATUS==1)&&(DU==0))//если телеуправление наше
		{
			if(tst_str(point,poH,0,0)==0)          //если стрелку можно переводить
			{ if(poH==1) oper='A';             //выбрать код команды перевода стрелки
				else oper='B';
				coAV[wr].Fl_kom=0;
				formkps(oper);//сформировать команду для перевода
				prorisovka=0;
				w(123,point,"");//ПОСЫЛАЕМ КОМАНДУ НА ПЕРЕВОД СТРЕЛКИ
				okno_otkaz=1;
			}
		}
		AVARIIN=0;
		coAV[wr].Fl_kom=0;//сбросить флаг команды
	}
	return;
}
#endif
//-------------------------------------------------------
// процедура оценки четности или нечетности стрелки ob
// по имени стрелки
// возвращает 0 для четной стрелки
// возвращает 1 для нечетной стрелки
int tst_chet(int ob)
{
	int ns,jj;
  char strlk[10];
  for(ns=0;ns<10;ns++)strlk[ns]=0;
  ns=0;
  while(isdigit(pako[ob][ns])==0)ns++;
  jj=0;
  while(isdigit(pako[ob][ns])!=0)strlk[jj]=pako[ob][ns++];
  ns=atoi(strlk);
  if((ns%2)==0)return(0);
  else return(1);
}
