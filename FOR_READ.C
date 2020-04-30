#include"bla.h"
#include "opred.h"
//----------------------------------------------------
//формирование текстового сообщения numso из файла lex.lex
//для объекта p0 с добавлением слова dob и вывод на экран

void w(int numso,int p0,char dob[20])
{
	int i23,dxv,dyv=15,xv,yv,CVE,CVEB,lp,lp1;
	char NAME[20];

	nom_func("407");
#ifdef WORK
	if(prorisovka!=0)return;
#endif
	if(p0>999)yv=p0-999;
	else yv=0;
	if(numso<999)
	{
		lseek(file_soob,63L*numso,0L);
		read(file_soob,&soob.lex,60);
		read(file_soob,&soob.COlo,1);
		for(i23=0;i23<60;i23++)soob.lex[i23]=~soob.lex[i23];
		while(soob.lex[i23--]==0x20)soob.lex[i23]=0;
	}
	else
	{
		okno_otkaz=1;
		soob.lex[0]=0;
	}
	if(numso==239)okno_otkaz=0;
	if(p0>999)return;
	if(p0<999)
	{ if(fr1[p0][0]==1)
		{ lp=0;
			while(pako[p0][lp]!=146)lp++;
			lp++;
			for(lp1=0;lp1<20;lp1++)NAME[lp1]=0;
			strcat(NAME," Ст.");lp1=4;
			while(pako[p0][lp]!=':')NAME[lp1++]=pako[p0][lp++];
		}
		else
		{
			lp1=0;
			NAME[lp1++]=32;
			for(lp=0;lp<20;lp++)
			{ NAME[lp1]=0;
				if((pako[p0][lp]!=32)&&(pako[p0][lp]!='_'))NAME[lp1++]=pako[p0][lp];

			}
			NAME[lp1]=0;
		}
	}
	else strcpy(NAME,"");
	xv=639-(3+strlen(soob.lex)+strlen(NAME)+strlen(dob))*7;
	dxv=639-xv;
	CVE=soob.COlo;
//  if((CVE==4)&&(zapretvybora==1))return;
	if(CVE==14) CVEB=RED;
	else
		if(CVE==4)CVEB=WHITE;
		else
			if(CVE==2)CVEB=15;

	if(numso==239)dyv=dyv+10;
	if((numso==275)||(numso==132))
	{
		if(getpixel(xv,yv)!=7)yv=yv+15;
		if(yv>=30)yv=0;
	}
	else
	{
		if((CVE==4)&&(okno_otkaz==0)){t(0);okno_otkaz=1;}
		else
			if((okno_otkaz==1)&&(yv<30))
			{
				yv=yv+14;
				t(2);
			}
			else if(prorisovka==0)t(0);
	}
	wind_w(xv,yv,(xv+dxv),yv+dyv,SOLID_FILL,CVE,CVEB);
	moveto((xv+10),yv+5);
	outtext(soob.lex);outtext(NAME);outtext(dob);
	if(numso==239)
	{
		moveto((xv+10),yv+15);
		outtext("ЕГО СОСТОЯНИЯ НЕ ИЗМЕНИЛА");
	}

	return;
}
/*******************************************************/

void wind_w(int x1,int y1,int x2,int y2,int sty,int col,int color)
{

	nom_func("422");
	setfillstyle(sty,col);
	setcolor(color);
  bar(x1,y1,x2,y2);
  setlinestyle(0,0,0);setcolor(8);
  rectangle(x1,y1,x2,y2);setcolor(color);
	return;
}
/***********************************\
* Процедура гашения окна сообщений  *
* kod - вариант гашения             *
* 0 - всего окна                    *
* 1 - двух длинных строк            *
* 2 - второй длинной строки         *
\***********************************/
void t(int kod)
{
	int poly[12]={170,0,639,0,639,50,460,50,445,32,170,32};;

	nom_func("325");
//	if(DISK==0){
	poly[0]=170;poly[10]=170;
//	}
//	else {poly[0]=125;poly[10]=125;}
	if(flag_paro!=0) return;
	if((klo==1)||(help>0)||(nikuda==1))return;
#ifdef WORK
	if(prorisovka==1)return;
#endif
#ifdef KANALS
	if(TEST_KANAL==1)return;
#endif
	setfillstyle(1,7);
	setcolor(7);
	if(kod==0)fillpoly(6,poly);
	if(kod==1)bar(180,0,639,25);
	if(kod==2)bar(180,14,639,32);
	return;
}
//--------------------------------------------------------------
/************************************************************\
* Процедура управления отображением сигналов и объектов,     *
* передаваемых в группе сигналов.                            *
\************************************************************/
void displaysignal(int oi,int bt_)
{
	int lj,vmv=0,test,ou;

	nom_func("41");
	if(fr1[oi][1]==9999)goto kon;
	if(fr1[oi][0]==3){sekci(oi,bt_);goto kon;}
	if((fr1[oi][1]==161)&&(fr1[oi][2]>199)&&(fr1[oi][2]<300))
	krasit_razd(oi,bt_);
	if((fr3[oi][bt_]==0)&&(fr1[oi][0]!=2))goto kon;
	//если перекрытие сигнала
	if((fr1[oi][2]<2)&&(fr1[oi][1]<2))//если сигнал
	if((bt_==4)&&((fr3[oi][12]&0xf)==0)&&(TELEUP==1)&&(fr1[oi][7]!=oi))
	//если не фиксировался отказ
	{ if(tst_fix(oi,4)&&(oi<kol_OSN))
			{
				if(fr3[oi][4]==1)
				{
					if(fr1[oi][7]!=oi)
					{
						slom_sign(oi);
						zvuk_vkl(0,0);
#ifdef AVTOD
						if((na==1)||(cha==1))prov_avto_si(oi);
#endif
					}
          else
          {
            if((fr3[oi][0]==0)&&(fr3[oi][1]==0))
            {
            	get_str(oi,0);
          		zvuk_vkl(3,0);
            	strcat(chudo,"_ОГН");
            	neisprav_na_disk('@',chudo);
            	w(169,999,chudo);
            }
					}

				}
				fix(oi,4);
      }
    fix(oi,4);
  }
	if((klo==1)||(help!=0)||(nikuda==1))goto kon;
  if((fr1[oi][2]<2)&&(fr1[oi][1]<2))//если сигнал
  { //если открылся маневровый поездной или сработал любой ВС
    if((fr3[oi][0]==1)||(fr3[oi][1]==1)||(fr3[oi][2]==1)||(fr3[oi][3]==1)||
		(fr3[oi][5]==1))//если сигнал непарафазен
		{
#ifdef AVTOD
      if(prov_si_avtod(oi,1)==1)
      {
        zvuk_vkl(0,0);
        w(128,oi," НЕЧЕТ.АВТОД.СНЯТО!");
				snyato_nechet();
      }
			if(prov_si_avtod(oi,1)==2)
      {
				zvuk_vkl(0,0);
        w(128,oi," ЧЕТ.АВТОД.СНЯТО!");
        snyato_chet();
      }
#endif
      test=fr1[oi][1];
      if(test==0)test=3;
      else test=2;
      ou=findspu1(oi,test);//найти СП по ходу сигнала
      if(ou!=-1)
      {
        if(fr3[ou][1]==1)//если есть замыкание СП
        {
					if((fr3[oi][0]==1)||(fr3[oi][2]==1)) //если открыт маневровый
					{
						if(fr1[oi][1]==0)fr3[ou][12]=fr3[ou][12]|1;//если четный
						if(fr1[oi][1]==1)fr3[ou][12]=fr3[ou][12]|4;//если нечетный
					}
					if((fr3[oi][1]==1)||(fr3[oi][3]==1))//если открыт поездной
					{
						if(fr1[oi][1]==0)fr3[ou][12]=fr3[ou][12]|2;//если четный
            if(fr1[oi][1]==1)fr3[ou][12]=fr3[ou][12]|8;//если нечетный
					}
        }
      }
    }
    sigo(oi,0,oi,bt_);
    ou=9999;
    for(test=0;test<kol_OB_UPR;test++)
    if(markery[test][6]==oi){ou=test;break;}
    if((ou!=9999)&&(ou!=modi))imena(ou);//переписать имя
	}
	if((oi<kol_OSN)&&(fr1[oi][1]!=2))
	{ if(fr1[oi][8]==666)/* если смена направления */
    {
      bt_=fr1[oi][11];
      oi=fr1[oi][9];
      goto smen;
    }
    if((bt_==1)&&(fr3[oi][0]==0)&&(fr3[oi][1]==1))//если открыт поездной
    {
#ifdef KOL_SOO2
      if(STATUS==1)
      {
        if(flagoutmsg==12)
        {
          if(fr1[oi][1]==1)
					{
            for(vmv=0;vmv<1;vmv++)
            if(Skvo[vmv].B2_s==oi) flagoutmsg=13;
          }
          else
          {
            for(vmv=1;vmv<2;vmv++)
            if(Skvo[vmv].B2_s==oi) flagoutmsg=13;
					}
        }
      }
#endif
    }
    bata=0;
//    if(fr1[oi][10]==9999)fix(oi,bt_);      ??????????????
		goto kon;
  }
  if(fr1[oi][1]==60)//если это объект переключения комплектов смены направления
  {
		bt_=0;
    ou=fr1[oi][bt_+3];
		oi=fr1[oi][bt_+2];
    bt_=1;
    priem_otpr(oi,bt_);
    priem_otpr(ou,bt_);
    kontr_perego(oi,2);
    kontr_perego(ou,2);
  }
  /**********дальние и направления*************/
smen:
  if((fr1[oi][1]>=0x70)&&(fr1[oi][1]<=0x7f)&&((bt_<=3)||(bt_==5)))
  {
    switch(bt_)
		{
      case 0: uch_udal(oi,fr1[oi][10],bt_); break;
      case 1: priem_otpr(oi,bt_);break;
      case 2: kontr_perego(oi,bt_);break;
      case 3: dzezl(oi,bt_);
#ifdef POLUAVTOMAT
              analiz_otprav(fr1[oi][11],fr1[oi][12]);
#endif
							break;
			case 5: uch_udal(oi,fr1[oi][10],0);
							priem_otpr(oi,1);
							kontr_perego(oi,2);
							dzezl(oi,3);
							break;
			default:break;
		}
		fix(oi,bt_);
		goto kon;
	}
/******************************************/
#ifdef VSP
	if((fr1[oi][1]>=400)&&(fr1[oi][1]<=600))
	{ if(bt_==5)
		{
			ris_smen_otv(oi,2);
			ris_smen_otv(oi,3);
		}
		else
		if((bt_==2)||(bt_==3))ris_smen_otv(oi,bt_);
		if(bt_==5)//если бит непарафазности
		{
			if((fr3[oi][5]==1)&&tst_fix(oi,5))
			{
				slom_interf(oi);
			}
		}
		fix(oi,bt_);
		goto kon;
	}
#endif
	if((fr1[oi][1]==19)||(fr1[oi][1]==199))
	{
		if((fr1[oi][bt_+4]&0xf000)==0)//если это для МСП
		{
			if(bt_==5)for(lj=0;lj<fr1[oi][12];lj++) sekos(oi,lj);
			else
				if(bt_<fr1[oi][12]) sekos(oi,bt_);
		}
		if((fr1[oi][bt_+4]&0x3000)!=0)//если это для лампы
		{
			lampa_fr2(oi,bt_);
		}
		fix(oi,bt_);
		goto kon;
	}
/*****************************************************************/
	if(fr1[oi][1]==9)
	{
		if(bt_==5)for(lj=0;lj<fr1[oi][12];lj++) KM_put(oi,lj,oi);
			else
			if(bt_<fr1[oi][12]) KM_put(oi,bt_,oi);
    fix(oi,bt_);
		goto kon;
  }
/********************************************************/
  if(fr1[oi][1]==10)
	{
    if(bt_==5)
    for(lj=0;lj<fr1[oi][12];lj++)
    ris_blok_uch(oi,lj);
		else
    if(bt_<fr1[oi][7]) ris_blok_uch(oi,bt_);
	}
/***************************************************************/
	if((fr1[oi][1]==18)||(fr1[oi][1]==188))
	{
		if(bt_==5)for(lj=0;lj<fr1[oi][12];lj++)ris_batarei(oi,lj);
		else
			if(bt_<fr1[oi][12]) ris_batarei(oi,bt_);
//    fix(oi,bt_);
		goto kon;
	}
//-------------------------------------------------------------
	if((fr1[oi][1]==30)||(fr1[oi][1]==31))
	{
		ris_otv_knop(oi);
		goto kon;
	}
//----------------------------------------
	if((fr1[oi][1]==130)||(fr1[oi][1]==131))
	{
		ris_otv_knop1(oi);
		imena(fr1[oi][7]&0xff);
		goto kon;
	}

//-------------------------------------------------------
  if(fr1[oi][1]==302)
  {
		if((bt_==2)||(bt_==5))krasit_knopki(oi,bt_);
    fix(oi,bt_);
		goto kon;
  }
/*************************************************************************/
  if((fr1[oi][1]==27)||(fr1[oi][1]==28))
  {
		uksps(oi,bt_);
		fix(oi,bt_);
		goto kon;
  }
 /************************************************************************/
  if(fr1[oi][1]==21)//если объект типа ПНО
	{
    if(bt_==5)for(lj=0;lj<4;lj++)lampa_3c(oi,lj,0);
		else
      if(bt_<4)lampa_3c(oi,bt_,0);
		goto kon;
  }
  //-------------------------------------------------------------
  if(fr1[oi][1]==210)
	{
		if(bt_==5)for(lj=0;lj<5;lj++)lampa_3(oi,lj);
		else if(bt_<5)lampa_3(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
	//-------------------------------------------------------------
	if((fr1[oi][1]==22)||(fr1[oi][1]==23))//если объект типа лампа или окантовка
	{
		if(bt_==5)for(lj=0;lj<fr1[oi][12];lj++)lampa(oi,lj);
		else
			if(bt_<fr1[oi][12]) lampa(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
	//------------------------------------------------------
	if(fr1[oi][1]==24) //объект типа "лампы концентрические
	{
		if(bt_==5)for(lj=0;lj<5;lj++)lampa_konc(oi,lj);
		else if(bt_<5)lampa_konc(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
	//-------------------------------------------------------------
		//если объект - текстовая лампа
	if((fr1[oi][1]>=162)&&(fr1[oi][1]<=165)&&(fr1[oi][1]!=163))
	{
		if(bt_==5)for(lj=0;lj<5;lj++)lampa_text(oi,lj);
		else
			if(bt_<5) lampa_text(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}

	//-------------------------------------------------------------
	if(fr1[oi][1]==101)//если объект типа 5 ламп в сообщении
	{
		if(bt_==5)for(lj=0;lj<5;lj++)lampa_5(oi,lj);
		else
			if(bt_<5) lampa_5(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
 //--------------------------------------------------------------
	if(fr1[oi][1]==38)//если объект типа переключение день/ночь
	{
		den_noch(oi);
    fix(oi,bt_);
		goto kon;
  }
  //--------------------------------------------------
  if(fr1[oi][1]==39)//если объект типа "переключение немаршрутизированных"
  {
		ris_nemar(oi);
    fix(oi,bt_);
		goto kon;
	}
  //---------------------------------------------------------------
  if(fr1[oi][1]==26)//если объект типа "исполнение немаршрутизированных"
	{
    nemar(oi);
    fix(oi,bt_);
		goto kon;
  }

  //-----------------------------------------------------
	if(fr1[oi][1]==220)
	/*если объект типа N ламп на 2^N состояний */
	{
		lampa_N(oi,bt_,0);
		goto kon;
  }
  if(fr1[oi][1]==5000)
  {
    if(bt_==5)for(lj=0;lj<fr1[oi][12];lj++)lampa_compl(oi,lj);
    else
      if(bt_<fr1[oi][12])lampa_compl(oi,bt_);
    fix(oi,bt_);
		goto kon;
  }
  /*******************************************************************/
  if(fr1[oi][1]==25)/*если эта лампа для фидера */
  {
    if(bt_==5)for(lj=0;lj<fr1[oi][12];lj++)fider3dat(oi,lj);
    else
      if(bt_<fr1[oi][12]) fider3dat(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
/*********************************************************************/
  if((fr1[oi][1]==14)||(fr1[oi][1]==15))/* если это зона оповещения */
  {
    zona_opov(oi,bt_);
    fix(oi,bt_);
		goto kon;
	}

/*********************************************************************/
  if((fr1[oi][1]==16)||(fr1[oi][1]==166)||(fr1[oi][1]==53))/* если это кнопка */
  {
		if(bt_==5) knopka_vkl_vsego(oi,fr1[oi][11]);
			else
			if(bt_<fr1[oi][12]) knopka_vkl_vsego(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
//--------------------------------------------------------------------
	if((fr1[oi][1]==163)||((fr1[oi][1]>=155)&&(fr1[oi][1]<=158)))
	{
		if(bt_==5)knopka_vkl_vsego1(oi,0);
		else knopka_vkl_vsego1(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
  /********************************************************************/
  if(fr1[oi][1]==17)/* если это кнопка для оповещения */
  {
    if(bt_==5) knopka_vkl_opove(oi);
      else
			if(bt_<fr1[oi][12]) knopka_vkl_opove(oi);
    fix(oi,bt_);
		goto kon;
  }
//------------------------------------------------------
  if(fr1[oi][1]==177)//если объект типа прямоугольник
  {
		if(bt_==5)for(lj=0;lj<5;lj++)barcolor(oi,lj);
    else
			barcolor(oi,bt_);
    fix(oi,bt_);
		goto kon;
  }


  //------------------------------------------------------
  if(fr1[oi][1]==66)// если переезд
  {
    p_li(oi);
    fix(oi,bt_);
		goto kon;
  }
  //------------------------------------------------------
  if(fr1[oi][1]==77)//если контроль габаритов
  {
    UKG(oi);
    fix(oi,bt_);
		goto kon;
  }
  //------------------------------------------------------
#ifdef OGRAD
  if(fr1[oi][1]==45)/*если запрос или ограждение */
  {
    if(bt_!=5)analiz_ogr(oi,bt_);
    else
    {
      analiz_ogr(oi,fr1[oi][3]);
      anal_sogl(fr1[oi][6],fr1[oi][7]);
    }
		fix(oi,bt_);
		goto kon;
	}
	if(fr1[oi][1]==46)/* если согласие */
  {
		if(bt_==5)anal_sogl(oi,fr1[oi][12]);/* работа с датчиком согласия */
    else anal_sogl(oi,bt_);
		fix(oi,bt_);
		goto kon;
	}
#endif
/***********************************************************************/
#ifdef POLUAVTOMAT
	if(fr1[oi][1]==51)/*если объект= "отправление" для полуавтоматики */
	{
		analiz_otprav(oi,bt_);
		fix(oi,bt_);
		goto kon;
  }
  if(fr1[oi][1]==52)/*если объект= "прием" для полуавтоматики */
  {
    analiz_priem(oi,bt_);
    fix(oi,bt_);
		goto kon;
  }
#endif

	if(fr1[oi][1]==2)//вариантная точка
	{
		setcolor(8);
		setfillstyle(1,8);
		bar(markery[fr1[oi][2]][4]-4,markery[fr1[oi][2]][5]-4,
		markery[fr1[oi][2]][4]+4,markery[fr1[oi][2]][5]+4);
	}

/************************************************************************/
  if((fr1[oi][1]==37)||(fr1[oi][1]==40))/* если пригласительные или реле ПСО */
  {
		ps_pso(oi,bt_);
    COD_PS_OLD=COD_PS_NEW;
    fix(oi,bt_);
		goto kon;
	}
	if((fr1[oi][1]==67)||(fr1[oi][1]==68))
	{
		if(fr3[oi][5]==1)for(lj=0;lj<4;lj++)lru_lko(oi,lj,fr1[oi][1]-67);
		else
		if(bt_<5)lru_lko(oi,bt_,fr1[oi][1]-67);
		goto kon;
	}
kon:
	return;
}
//---------------------------------------------------
void tu_tc()
{
  char  right[2]={'\x10'}, /* ts */
        left[2]={'\x11'}; /* ty */

	nom_func("377");
	if((klo==1)||(help!=0)) return;
  if(nikuda==1) return;
  /********************  TC  **********************/
  settextstyle(USER_FONT,VERT_DIR,0);
#ifdef KOL_SOO2
  //канал своего приема от второй стойки
  if((otkaz_ts[1]==0)&&(sboy_ts[1]<2)) setcolor(LIGHTGREEN);
	else setcolor(12);
	outtextxy(Xleft-3,Yleft+7,right);

	//канал приема соседа от второй
	if(NEISPRAVEN==0)
		if(otkaz_sos[1]==0) setcolor(LIGHTGREEN);
		else setcolor(12);
	else setcolor(8);
	outtextxy(Xright-3,Yright+7,right);
#endif

	//канал своего приема от первой стойки
	if((otkaz_ts[0]==0)&&(sboy_ts[0]<2)) setcolor(LIGHTGREEN);
	else setcolor(12);
	outtextxy(Xleft-3,Yleft-12,left);

	//канал приема соседа от первой
	if(NEISPRAVEN==0)
		if(otkaz_sos[0]==0) setcolor(LIGHTGREEN);
		else setcolor(12);
	else setcolor(8);
	outtextxy(Xright-3,Yright-12,left);
/********************  TУ  **********************/
#ifdef KOL_SOO2
	//канал передачи свой во вторую стойку
	if((otkaz_tu[1]==0)&&(zapret[1]==0)&&(otkaz_tu[1]==0))setcolor(LIGHTGREEN);
	else setcolor(12);
	outtextxy(Xleft-3,Yleft+17,left);

	//канал передачи соседа во вторую стойку
	if(NEISPRAVEN==0)
		if((zapret_sos[1]==0)&&(otkaz_tu_sos[1]==0))setcolor(LIGHTGREEN);
		else setcolor(12);
	else setcolor(8);
	outtextxy(Xright-3,Yright+17,left);
#endif

	//канал передачи свой в первую стойку
	if((otkaz_tu[0]==0)&&(zapret[0]==0)&&(otkaz_tu[0]==0))setcolor(LIGHTGREEN);
	else setcolor(12);
	outtextxy(Xleft-3,Yleft-22,right);

	//канал передачи соседа в первую стойку
	if(NEISPRAVEN==0)
		if((zapret_sos[0]==0)&&(otkaz_tu_sos[0]==0))setcolor(LIGHTGREEN);
		else setcolor(12);
	else setcolor(8);
	outtextxy(Xright-3,Yright+-22,right);

//канал между ПЭВМ
	if(NEISPRAVEN==1) setcolor(12);
	else setcolor(LIGHTGREEN);
	settextstyle(USER_FONT,HORIZ_DIR,0);
	moveto(Xleft+10,Yleft-3);
	outtext(left);
	setcolor(LIGHTGREEN);
	moveto(Xright-15,Yleft-3);
	outtext(right);
#ifdef DISP_UPR
	kanal_DC();
#endif

#ifdef SPDLP
	kanal_SPDLP();
#endif
	return;
}
//-----------------------------------------------------------------------
/*******************************************\
* Процедура отображения на экране датчиков  *
* состояния КМ путей                        *
* nom - номер объекта в базе с датчиками КМ *
* bit_ - номер бита объекта КМ              *
* nom_vyz - объект пути, для которого КМ    *
\*******************************************/
void KM_put(int nom,int bit_,int nom_vyz)
{
	int NNm=0,xc,yc,cvet,bit_ch=15,bit_n=15,napr;

	nom_func("120");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	//если процедура вызывается из обработки пути,
	//то nom - объект КМ,а nom_vyz - объект пути
	//если вызов из обработки КМ, то и nom и nom_vyz - объект КМ
	NNm=fr1[nom][3+bit_]&0xFFF; //в любом случае берем номер пути из КМ
	if(nom_vyz!=nom)//если вызов из пути
	{
		if(NNm!=nom_vyz)return; //если ссылки в базе не совпали, то выйти
	}
	if((fr1[nom][3+bit_]&0xf000)==0)//если четный КМ
	bit_ch=bit_;
	if((fr1[nom][3+bit_]&0xf000)==0x1000)//если нечетный КМ
	bit_n=bit_;//бит для нечетного КМ
	if((fr1[nom][3+bit_]&0xf000)==0x2000)//если обобщенный КМ
	bit_n=bit_ch=bit_;

	setlinestyle(0,0,0);

	if(fr3[nom][bit_]==1)cvet=YELLOW;  //для замкнутого - желтый цвет
	else cvet=cvv;//иначе фон

	if(fr3[nom][5]==1)cvet=11;
	setcolor(cvet);

	//если датчик КМ один для обоих направлений
	if(bit_n==bit_ch)
	{
		xc=(fr2[NNm][0]+fr2[NNm][2])/2+8;
		yc=fr2[NNm][1]+3;
		line(xc-3,yc,xc+3,yc);
		return;
	}
	yc=fr2[NNm][1];//координата Y

	if(bit_ch==15)goto nechet; //если нечетные КМ

	if(bit_==bit_ch) //если в сообщении четный КМ
	{
#if OBRAT==-1 //четный прием справа(четный КМ смотрит влево)
		{
chch:
			if(fr2[NNm][0]<fr2[NNm][2])xc=fr2[NNm][0]+10;
			else xc=fr2[NNm][2]+10;
			napr=1;
#else//четный КМ смотрит вправо
nn:   if(fr2[NNm][0]<fr2[NNm][2])xc=fr2[NNm][2]-10;
			else xc=fr2[NNm][0]-10;
			napr=0;
#endif
	}

	if(bit_n==15)//если четные КМ
	goto chet;
nechet:
	if(bit_==bit_n) /* нечетный */
	{
#if OBRAT==-1   //нечетный КМ  смотрит вправо
		if(fr2[NNm][0]>fr2[NNm][2])xc=fr2[NNm][0]-10;
		else  xc=fr2[NNm][2]-10;
		napr=0;
#else  //нечетный КМ смотрит влево
		if(fr2[NNm][0]<fr2[NNm][2])xc=fr2[NNm][0]+10;
		else xc=fr2[NNm][2]+10;
		napr=1;
#endif
	}
chet:
	if(fr3[nom][5]==1)setcolor(LIGHTCYAN);
	if(napr==1){line(xc,yc-1,xc+5,yc-3);line(xc,yc+1,xc+5,yc+3);}
	if(napr==0){line(xc,yc-1,xc-5,yc-3);line(xc,yc+1,xc-5,yc+3);}
	return;
}
//------------------------------------------------------------------
/*****************************************************\
* Процедура изображения стрелочек смены направления,  *
* схемы переключения комплектов смены направления и
* связанных с направлением движения перегонных точек. *
* hto - объект контроля перегона в базе               *
* b_ - бит объекта для которого надо нарисовать.      *
\*****************************************************/
void priem_otpr(int hto,int b_)
{
	int nj=0,ito,msdv,cstr,c_nv,mesto,ob_pod,bit_pod_my,bit_pod_sos;
	int dso,bit_dso; //объект - лампа дачи согласия на смену направления
	int l_u_l[14],l_u_r[14],r_u_l[14],r_u_r[14],cod_podkl,prf;

	nom_func("232");
	if((fr1[hto][1]&(1<<b_))==0)return;
	msdv=fr1[hto][8]*fr1[hto][4];
	for(ito=0;ito<14;ito++)
	{ l_u_l[ito]=left_up_left[ito];
		l_u_r[ito]=left_up_right[ito];
		r_u_l[ito]=right_up_left[ito];
		r_u_r[ito]=right_up_right[ito];
	}
#ifndef NEW_KNOPKA
	nj=fr1[hto][9];//номер объекта для управления сменой направления
	dso=fr1[nj][6]&0xfff;
	bit_dso=(fr1[nj][6]&0xf000)>>12;
#else
	knopka_vkl_vsego1(dso,bit_dso);
#endif
	//для полуавтоматики
	if(nj==9999)if(fr1[hto][5]>0)mesto=2;
							else mesto=1;
	else//для автоблокировки
	{ if(fr1[nj][1]==0)//если объект в четной горловине
		{
#if	OBRAT==1
			mesto=1;
#else
			mesto=2;
#endif
		}
		else
			if(fr1[nj][1]==1)//если объект в нечетной горловине
			{
#if OBRAT==1
				mesto=2;
#else
				mesto=1;
#endif
			}
			else return;
	}
	//проверка наличия объекта переключения комплектов
	if(fr1[nj][3]==9999)goto prodol; //если нет комплекта - перейти
	else ob_pod=fr1[nj][3];//иначе взять номер объекта подключения
	//если объект подключения комплекта непарафазен
	if(parafaz(ob_pod)==-1)
	//цвет стрелки "циан", цвет НВ(ЧВ) -"циан", уйти на рисование
	{cstr=11;c_nv=11;fr3[nj][7]=0xFFFF;goto ris;}
	//для парафазного объекта
	bit_pod_my=fr1[nj][4];//получить бит подключения к данному пути
	bit_pod_sos=fr1[nj][5];//получить бит подключения к соседнему пути
	//вычислить код подключения
	cod_podkl=fr3[ob_pod][bit_pod_sos]*2+fr3[ob_pod][bit_pod_my];
	fr3[nj][7]=0;
	switch(cod_podkl)
	{
		case 0: cstr=8;c_nv=8;
						if(fr1[hto][2]==0)fr3[nj][7]=0xffff;
						if(fr1[hto][2]==1)fr3[nj][7]=0xf0f0;
						break;
		case 1: cstr=0;c_nv=0;break;
		case 2: cstr=8;c_nv=8;fr3[nj][7]=0xffff;break;
		case 3: cstr=13;c_nv=13;fr3[nj][7]=0xffff;break;
	}
	if(cstr!=0)goto ris;
prodol:
	prf=0;
	if(parafaz(hto)==-1)prf=2;
	if(parafaz(nj)==-1)
	prf=prf+1;
	switch(prf)
	{
		case 0: cstr=0; c_nv=0; break;
		case 1: cstr=0; c_nv=11;break;
		case 2: cstr=11;c_nv=0; fr3[nj][7]=0xffff;break;
		case 3: cstr=11;c_nv=11;fr3[nj][7]=0xffff;break;
	}
	if(cstr==0)
	{
		fr3[nj][7]=0;
		if(fr3[hto][b_]==1)cstr=LIGHTGREEN;
		else cstr=YELLOW;
	}
ris:
	setlinestyle(0,0,0);
	setcolor(cstr);
	/* 1 - слева   2 - справа */
	switch(mesto)
	{
		case 1: for(ito=1;ito<14;ito=ito+2)
						{
							l_u_l[ito]=left_up_left[ito]+msdv;
							l_u_r[ito]=left_up_right[ito]+msdv;
						}
						if(((fr3[nj][1]==1)||(fr3[nj][7]==0xf0f0))&&
						(fr3[nj][7]!=0xffff))//если В на отправлении
						{
							if(c_nv!=11)c_nv=10;
							setfillstyle(1,7);
							bar(l_u_r[0]-4,l_u_r[1],l_u_r[0],l_u_r[13]);
							setfillstyle(1,c_nv);
							bar(l_u_l[0],l_u_l[1],l_u_l[0]+4,l_u_l[13]);
						}
						else //если В на приеме
						{
							if(c_nv!=11)c_nv=14;
							setfillstyle(1,7);
							bar(l_u_l[0],l_u_l[1],l_u_l[0]+4,l_u_l[13]);
							setfillstyle(1,c_nv);
							bar(l_u_r[0]-4,l_u_r[1],l_u_r[0],l_u_r[13]);
						}
						if(((fr3[hto][b_]==1)||(fr3[nj][7]==0xf0f0))&&
						(fr3[nj][7]!=0xffff))//если СН на отправлении
						{
							setfillstyle(SOLID_FILL,7);
							bar(l_u_r[0],l_u_r[5],l_u_r[6],l_u_r[9]);
							setfillstyle(SOLID_FILL,cstr);
							fillpoly(7,l_u_l);
						}
						else //если СН на приеме
						{
							setfillstyle(SOLID_FILL,7);
							bar(l_u_l[6],l_u_l[5],l_u_l[0],l_u_l[9]);
							setfillstyle(SOLID_FILL,cstr);
							fillpoly(7,l_u_r);
						}
						break;
		case 2: for(ito=1;ito<14;ito=ito+2)
						{
							r_u_l[ito]=right_up_left[ito]+msdv;
							r_u_r[ito]=right_up_right[ito]+msdv;
						}
						if(((fr3[nj][1]==1)||(fr3[nj][7]==0xf0f0))&&
						(fr3[nj][7]!=0xffff))//если В на отправлении
						{
							if(c_nv!=11)c_nv=10;
							setfillstyle(1,7);
							bar(r_u_l[0],r_u_l[1],r_u_l[0]+4,r_u_l[13]);
							setfillstyle(1,c_nv);
							bar(r_u_r[0]-4,r_u_r[1],r_u_r[0],r_u_r[13]);
						}
						else   //если В на приеме
						{
							if(c_nv!=11)c_nv=14;
							setfillstyle(1,7);
							bar(r_u_r[0]-4,r_u_r[1],r_u_r[0],r_u_r[13]);
							setfillstyle(1,c_nv);
							bar(r_u_l[0]-4,r_u_l[1],r_u_l[0]+4,r_u_l[13]);
						}
						if(((fr3[hto][b_]==1)||(fr3[nj][7]==0xf0f0))&&
						(fr3[nj][7]!=0xffff))//если СН на отправлении
						{
							setfillstyle(SOLID_FILL,7);
							bar(r_u_l[6],r_u_l[5],r_u_l[0],r_u_l[9]);
							setfillstyle(SOLID_FILL,cstr);
							fillpoly(7,r_u_r);
						}
						else    //если СН на приеме
						{
							setfillstyle(SOLID_FILL,7);
							bar(r_u_r[0],r_u_r[5],r_u_r[6],r_u_r[9]);
							setfillstyle(SOLID_FILL,cstr);
							fillpoly(7,r_u_l);
						}
						break;
	}
	if(fr1[hto][11]!=9999)ris_blok_uch(fr1[hto][11],5);
	if(fr1[hto][12]!=9999)ris_blok_uch(fr1[hto][12],5);
	return;
}
//----------------------------------------------------------------------
/***********************************************************\
* Анализ состояния объекта KS #v, для формирования записи о *
* ненорме в архив состояний УВК oblom#.fix                  *
\***********************************************************/
int chego_posl(int v)
{
	//если перезапуск и объединение

	nom_func("19");
	if((fr3[v][3]==1)&&(fr3[v][4]==1))return(9399+fr1[v][13]);
	//если перезапуск без объединения
	if((fr3[v][3]==1)&&(fr3[v][4]==0))return(9299+fr1[v][13]);
	//если объединение без перезапуска
	if((fr3[v][3]==0)&&(fr3[v][4]==1))return(9199+fr1[v][13]);
	else return(0);
}
/*********************************************************/
void uch_udal(int nuch2,int nuch1,int b_)
{
	//изображение второго участка приближения
	//nuch1 - второй участок приближения
	//nuch2 - объект типа 0x7k для первого участка
	// b_ - бит для первого участка в объекте nuch2
	int smesh=1,ito=0,x1_1,y1_1,x1_2,y1_2,x2,y2,cod,cvt1,cvt2,smen_n;
	nom_func("378");
	smesh=smesh<<b_; //установка тест бита на нужную позицию
	ito=fr1[nuch2][1]&smesh;//проверка значения бита
	if(ito!=smesh) return; //бит сброшен - участок не наблюдается - выход
//-----------------------------------------------------------
	x2=fr1[nuch2][6] ;y2=fr1[nuch2][7];//координаты начала участка 1ИП
	x1_1=fr2[nuch1][0]; x1_2=fr2[nuch1][2];//координаты 2-го участка удаления
	y1_1=fr2[nuch1][1]; y1_2=fr2[nuch1][3];
	//стирание
	setlinestyle(0,0,3); //бит наблюдается - цвет фона, толстая
	setcolor(7);
	line(x1_1,y1_1,x1_2,y1_2); //2-й участок приближения
	line(x2,y2,x2+fr1[nuch2][5],y2); //1-й участок приближения
	smen_n=fr1[nuch2][9];//номер объекта для датчика смены направления (НВ/ЧВ...)
	if(fr1[smen_n][3]!=9999)
	{
//    kap_rem=fr1[smen_n][3];//объект контроля подключения комплекта для кап.рем.
//    bit_my=fr1[smen_n][4]; //бит подключения к этому пути
//    bit_sos=fr1[smen_n][5];//бит подключения к соседнему пути
		// направление(NSN)* 4 + занятость 2ип * 2 + занятость 1ип
		if(fr3[smen_n][7]==0xffff)
			fr3[nuch2][1]=0;
		if(fr3[smen_n][7]==0xf0f0)
			fr3[nuch2][1]=1;
	}
	cod=4*fr3[nuch2][1]+2*fr3[nuch2][0]+fr3[nuch1][0];
	switch(cod)
	{
		case 0: cvt1=8; cvt2=8; break;
		case 1: cvt1=12;cvt2=8; break;
		case 2: cvt1=8; cvt2=12;break;
		case 3: cvt1=12;cvt2=12;break;
		case 4: cvt1=8; cvt2=8; break;
		case 5: cvt1=12;cvt2=7; break;
		case 6: cvt1=8; cvt2=12;break;
		case 7: cvt1=12;cvt2=7; break;
	}
	if(fr3[nuch1][5]!=0)cvt1=11;
	if(fr3[nuch2][5]!=0)cvt2=11;
	setcolor(cvt1);
	if(cvt1==8)setlinestyle(0,0,0);
	line(x1_1,y1_1,x1_2,y1_2); //1-й участок приближения
	if(cvt2==8)setlinestyle(0,0,0);
	else setlinestyle(0,0,3);
	setcolor(cvt2);
	line(x2,y2,x2+fr1[nuch2][5],y2); //2-ой участок приближения
	if(tst_fix(nuch2,b_)||tst_fix(nuch1,b_))
	{
		if(cod==2)
			zvuk_vkl(2,9);//прибытие 2 участок
		if(cod==3)
			zvuk_vkl(2,15);//прибытие 1 участок
		fix(nuch2,b_);
		fix(nuch1,b_);
	}
#ifdef POLUAVTOMAT
	if(fr1[nuch1][7]==8888)//для полуавтоматики
	{
		if(fr3[nuch2][b_]==1)//если участок занят
		{
			setcolor(LIGHTRED);setlinestyle(0,0,3);
			ind=fr1[nuch2][9]; //получить объект увязки
			if(fr1[ind][0]==5)//если это участок приближения (полуавтоматика)
			{
				if((fr3[ind][0]==0)&&(perekluch==0))zvuk_vkl(2,9);//прибытие
			}
		}
		else //если участок освободился
		{
			setcolor(8);
			setlinestyle(0,0,0);
		}
		if(fr3[nuch2][5]==1) setcolor(LIGHTCYAN);
		x2=fr1[nuch2][6];
		y2=fr1[nuch2][7];
		line(x2,y2,x2+fr1[nuch2][5],y2);
	}
#endif
	return;
}
//--------------------------------------------------------------------
/**************************************************\
* Процедура отображения на экране ключей-жезлов    *
* o - объект с информацией о ключе-жезле           *
* b_ - бит жезла в объекте                         *
\**************************************************/
void dzezl(int o,int b_)
{
	int cvet,x,y,smesh=1,ito;

	nom_func("47");
	smesh=smesh<<b_;             //установить единицу в позицию бита жезла
	ito=fr1[o][1]&smesh;         //получить принадлежность бита объекту
	if(ito!=smesh) return;       //если бит не соответствует - выйти
	if (fr3[o][3]==1)cvet=12;   //если бит установлен - красный цвет
	else if(fr3[o][3]==0)cvet=8; //иначе - серый
	if(fr3[o][5]==1)cvet=LIGHTCYAN;//если непарафазность - цвет циан
aa:
  //далее изображение ключа - жезла
	x=fr3[o][6];
	y=fr3[o][9];
	setlinestyle(0,0,0);
	setcolor(cvet);
	circle(x,y,1);circle(x,y,2);circle(x,y,3);circle(x,y,4);
	setlinestyle(0,0,3);line(x,y,x,y+13);

	if(cvet==12)       //если красный
	if(tst_fix(o,b_))   //и еще не фиксировался, то записать на диск
	{                   //и выдать на экран
		if(get_str(o,b_)==0)neisprav_na_disk(0x3,chudo);
		w(320,999,chudo);
	}
	fix(o,b_); //запомнить факт фиксации
	return;
}
//----------------------------------------------------------------------
/***************************************************\
* Процедура обработки данных о состоянии перегона.  *
\***************************************************/
void kontr_perego(int hto,int b_)
{
	int msdv,ckon,ito,nj,mesto=0;
	int smesh=1,ob_podkl,bit_podkl;

	nom_func("131");
	smesh=smesh<<b_;				//сместить указатель на бит
	ito=fr1[hto][1]&smesh;	//проверить наличие этого бита в объекте контроля
	if(ito!=smesh) return; 	//если нет в объекте - выход
	msdv=fr1[hto][8];       //сомножитель для смещения по Y

	if(fr3[hto][b_]==1)
	 ckon=12;              //если перегон занят - цвет красный
	else ckon=YELLOW;       //если свободен - цвет желтый

	if(fr3[hto][5]==1)
	 ckon=LIGHTCYAN;        //если непарафазность - цвет циан

	nj=fr1[hto][9];         //объект смены направления
	if(nj==0)               //если нет объекта для смены направления
		if(fr1[hto][5]>0) mesto=2; //если на экране участок справа
		else mesto=1;             //если участок на экране слева
	else                    //если есть объект смены направления
		if(fr1[nj][1]==0)     //если объект в четной горловине
#if	OBRAT==1 //если прием с четной стороны
			mesto=1;
#else
			mesto=2;
#endif
		else
			if(fr1[nj][1]==1) //если объект смены направления в нечетной горловине
#if OBRAT==1  //если прием с нечетной стороны
				mesto=2;
#else
				mesto=1;
#endif
			else return;
	if(fr1[nj][12]!=9999)  //если есть объект РУ вх.сигнала соседней станции
	{
		ob_podkl=fr1[nj][12]>>4;   //взять объект РУ вх.сигнала соседней станции
		bit_podkl=fr1[nj][12]&0xf; //взять бит РУ вх.сигнала соседней станции
		if(fr3[ob_podkl][bit_podkl]==0)ckon=7; //если бит РУ сброшен
	}
	/* 1 - слева   2 - справа */
	switch(mesto)
	{
		case 1: setfillstyle(SOLID_FILL,ckon);
						bar(left_up_left[0]+7,
						left_up_left[1]+msdv*fr1[hto][4],
						left_up_left[0]+13,
						left_up_left[13]+msdv*fr1[hto][4]);
						break;
		case 2: setfillstyle(SOLID_FILL,ckon);
						bar(right_up_left[0]+7,right_up_left[1]+msdv*fr1[hto][4],right_up_left[0]+13,right_up_left[13]+msdv*fr1[hto][4]);
						break;
	}
	return;
}
//----------------------------------------------------------------------
/*****************************************************\
* Процедура изображения состояния основного комплекта *
* ТУМС в мнемосхеме состояния УВК                     *
* nom - номер в базе для объекта состояния комплекта. *
\*****************************************************/
void kps_osn(int nom)
{
	int smeo=0;

	nom_func("134");
	if((nikuda==1)||(help!=0)) return;
#ifdef KOL_SOO2
	if(nom==KVV2)smeo=0; //для ТУМС-2
	else
#endif
	smeo=97;
	setcolor(8); //цвет контура и буквы "О"
	setlinestyle(0,0,0);
	rectangle(292,80+smeo,308,92+smeo);//контур
	if(fr3[nom][5]==1) //при непарафазности
	{
		setfillstyle(SOLID_FILL,3); //цвет заливки циан
		bar(293,81+smeo,307,91+smeo); //заливка контура
		outtextxy(297,83+smeo,"O");
		return;
	}
	if(fr3[nom][2]==0)
	{
		setfillstyle(SOLID_FILL,GREEN); //при норме - цвет заливки зеленый
		bar(293,81+smeo,307,91+smeo);
	}
	else
	{
		setfillstyle(SOLID_FILL,RED); //при ненорме - цвет заливки красный
		bar(293,81+smeo,307,91+smeo);
	}
	outtextxy(297,83+smeo,"O");
	return;
}
//----------------------------------------------------------------------
/******************************************************\
* Процедура изображения состояния резервного комплекта *
* ТУМС в мнемосхеме состояния УВК                      *
* nom - номер в базе для объекта состояния комплекта.  *
\******************************************************/
void kps_rez(int nom)
{
	int smeo=0;

	nom_func("135");
	if(klo!=1)return;
	if((nikuda==1)||(help!=0))return;
#ifdef KOL_SOO2
	if(nom==K_rez2) smeo=0;
	else
#endif
	smeo=97;
	setcolor(8); //цвет контура и буквы "Р"
	setlinestyle(0,0,0);
	rectangle(325,80+smeo,340,92+smeo);
	if(fr3[nom][5]==1) //если непарафазность
	{
		setfillstyle(SOLID_FILL,3); //цвет заливки - циан
		bar(326,81+smeo,339,91+smeo);
		return;
	}
	if(fr3[nom][3]==0) //если норма
	{
		setfillstyle(SOLID_FILL,GREEN); //цвет заливки - зеленый
		bar(326,81+smeo,339,91+smeo);
	}
	else //при ненорме
	{
		setfillstyle(SOLID_FILL,RED); //цвет заливки красный
		bar(326,81+smeo,339,91+smeo);
	}
	outtextxy(330,83+smeo,"P");
	return;
}
/********************************************/
#ifdef OGRAD
/*----------------анализ ограждения путей и согласия ограждения-------*/
analiz_ogr(int obekt,int bit_)
{
	int putik,knopik,cvet_ogr,cvet_knop,X,Y,t_0,cod;
	/*putik - номер объекта пути, для которого выполняется ограждение */
	/*knopik - номер объекта в markery, который является кнопкой согласия */
	/*obekt -номер объекта для контроля за состоянием реле в системе ограждения*/
	/*cvetik - цвет объекта для текущей ситуации */
	int ob_ogr,bit_ogr,ob_sogl,bit_sogl,ob_zapros,bit_zapros;
	ob_ogr=fr1[obekt][3]&0xfff;
	bit_ogr=(fr1[obekt][3]&0xf000)>>12;
	ob_zapros=fr1[obekt][2]&0xfff;
	bit_zapros=(fr1[obekt][2]&0xf000)>>12;
	ob_sogl=fr1[obekt][12]&0xfff;
	bit_sogl=(fr1[obekt][12]&0xf000)>>12;
	putik=fr1[obekt][4];
	cod=fr3[ob_ogr][bit_ogr]+fr3[ob_sogl][bit_sogl]*2+fr3[ob_zapros][bit_zapros]*4;
	COD_OGR=fr3[putik][9];
	if((nikuda==1)||(klo==1)||(help!=0))return;
	fr3[putik][9]=cod;
	switch(cod)
	{
		case 0: if(COD_OGR==4)w(35,putik,"");/* запрос на ограждение снят,путь №№*/
						else
						{
							if(COD_OGR==7)w(153,putik,"");/* ограждение снято - путь №№*/
							else
							if(COD_OGR!=0)w(89,putik,"");/* ограждение в исходном - путь №№*/
						}
						sbros_time(putik,ob_ogr);
						break;
		case 4: if(COD_OGR==0)w(152,putik,"");// получен запрос на ограждение, путь №№
						else
						if(COD_OGR==7)otkaz_obkt(ob_ogr,bit_ogr,1);
						sbros_time(putik,ob_ogr);
						break;
		case 7: if(COD_OGR!=7)w(141,putik,"");// включено ограждение путь ##
						sbros_time(putik,ob_ogr);
						break;
		default: if(COD_OGR!=cod)t_0=TIMER_SOB(putik,46,ob_ogr);
						 if(biostime(0,0l)-pooo[putik]>40l)// если время вышло
						 {
							switch(cod)
							{
								case 1: obekt=ob_ogr;bit_=bit_ogr;break;
								case 2: obekt=ob_sogl;
												bit_=bit_sogl;
												break;
								case 3: obekt=ob_zapros;bit_=bit_zapros;break;
								case 4: obekt=ob_sogl;bit_=bit_sogl;break;
								case 5: obekt=ob_sogl;bit_=bit_sogl;break;
								case 6: obekt=ob_zapros;bit_=bit_zapros;break;
								default:  break;
							}
							otkaz_obkt(obekt,bit_,1);
							sbros_time(putik,ob_ogr);
							break;
						 }
						 else return;
	}
	//-------------------------------------------------------------------
	switch(cod)
	{
		case 0: cvet_knop=8;cvet_ogr=7;break;// нет ограждения, согласия и запросов
		case 1: cvet_knop=8;cvet_ogr=13;break;// есть только ограждение
		case 2: cvet_knop=5;cvet_ogr=7;break;// есть только согласие
		case 3: cvet_knop=5;cvet_ogr=13;break;// согласие и ограждение
		case 4: cvet_knop=8;cvet_ogr=14;break;// запрос
		case 5: cvet_knop=8;cvet_ogr=13;break;// запрос и ограждение
		case 6: cvet_knop=5;cvet_ogr=13;break;// запрос и согласие
		case 7: cvet_knop=4;cvet_ogr=12;break;// запрос,согласие и ограждение
	}
	if(fr3[obekt][5]==1) // если выявлена непарафазность
	{
		cvet_ogr=11;
		if(prorisovka==0)slom_interf(obekt);
	}
	if(fr3[ob_sogl][5]==1) // если выявлена непарафазность
	{
		cvet_knop=3;
		if(prorisovka==1)slom_interf(ob_sogl);
	}
ris:
	setcolor(cvet_ogr);
	setlinestyle(0,0,0);
	putik=fr1[obekt][4];// объект для пути
	knopik=fr1[ob_ogr][5];// объект управления для дачи согласия
	X=fr2[putik][0];   // координата для левой части пути
	Y=fr2[putik][1]-3;
	line(X,Y,X,Y+6);  // нарисовать ограждение пути с левой стороны
	line(X+1,Y,X+1,Y+6);
	X=fr2[putik][2];   // координата для правой стороны пути
	line(X,Y,X,Y+6);  // нарисовать ограждение с правой стороны
	line(X+1,Y,X+1,Y+6);
	X=markery[knopik][4];// координаты кнопки дачи согласия
	Y=markery[knopik][5];
		// нарисовать окантовку кнопки
	rectangle(X-5,Y-5,X+5,Y+5);
	setfillstyle(1,cvet_knop);
	bar(X-4,Y-4,X+4,Y+4);
	return;
}
#endif
//-----------------------------------------------------------------------
/************************************\
* Процедура вывода на экран и записи *
* в архив ненорм по объектам СЦБ     *
\************************************/
void otkaz_obkt(int obkt,int bit,int ekran)
{
	int nom_soob,ob_txt;

	nom_func("203");
	//не действует во время прорисовки
	if(prorisovka==1)return;
	//если объект связан с ограждением
	if((fr1[obkt][1]==46)||(fr1[obkt][1]==45))
	{
		nom_soob=154;//"Неисправность системы ограждения"
		if(fr1[obkt][1]==46)ob_txt=fr1[obkt][4];
		if(fr1[obkt][1]==45)ob_txt=fr1[obkt][8];

		//если требуется вывод на экран - дать текст на экране
		if(ekran==1)w(nom_soob,ob_txt,"");

		//если найдено имя объекта для бита - записать на диск
		if(get_str(obkt,bit)==0)neisprav_na_disk('@',chudo);
	}

	//если объект связан с полуавтоматикой
	if((fr1[obkt][1]==51)||(fr1[obkt][1]==52))
	{
		nom_soob=81; // "Неисправность системы полуавтоматики
		if(get_str(obkt,bit)==0)neisprav_na_disk('@',chudo);
		if(ekran==1)
		{
			w(nom_soob,999,"");
			outtext(" ");
			outtext(chudo);
		}
	}
	zvuk_vkl(1,10);
	w(169,obkt,"04"); //"Неисправность"
	return;
}
//----------------------------------------------------------
#ifdef OGRAD
anal_sogl(int obekt,int bit_)
{
	int putik,knopik,cvetik,X,Y,t_0;
	//putik - номер объекта пути, для которого выполняется ограждение
	//knopik - номер объекта в markery, который является кнопкой согласия
	//obekt -номер объекта для контроля за состоянием реле в системе ограждения
	//cvetik - цвет объекта для текущей ситуации
	int ob_ogr,bit_ogr,ob_sogl,bit_sogl,ob_zapros,bit_zapros;
	ob_ogr=fr1[obekt][3]&0xfff;
	bit_ogr=(fr1[ob_ogr][3]&0xf000)>>12;
	ob_zapros=fr1[obekt][2]&0xfff;
	bit_zapros=(fr1[obekt][2]&0xf000)>>12;
	ob_sogl=fr1[obekt][12]&0xfff;
	bit_sogl=(fr1[obekt][12]&0xf000)>>12;
	putik=fr1[obekt][4];// объект для пути
	knopik=fr1[obekt][5];// объект управления для дачи согласия
	X=markery[knopik][4];// координаты кнопки дачи согласия
	Y=markery[knopik][5];
	if(fr3[obekt][5]==1) // если выявлена непарафазность
	{
		if(prorisovka==0)slom_interf(obekt);
		setfillstyle(1,3);
		bar(X-5,Y-5,X+5,Y+5);
	}
	analiz_ogr(ob_ogr,bit_ogr);
	return;
}
#endif
//---------------------------------------------------------------
int TIMER_SOB(int ob,int tip,int ob_ini)
{
	int N_time=25,i;

	nom_func("361");
	if(ZAGRUZKA!=0)return(0);
	for(i=0;i<25;i++)
	{
		if((N_time==25)&&(TIMER_N[i][0]==0))N_time=i;
		else
		if(TIMER_N[i][0]==ob)break;
	}
	if(TIMER_N[i][0]==ob)//если найден таймер для этого объекта
	{
		if(TIMER_N[i][1]!=20)
		{
			if((biostime(0,0L)-pooo[ob])>35L) // если время вышло
			{
				if(pooo[ob]!=2)pooo[ob]=0L;
				TIMER_N[i][0]=0;
				return(-1);
			}
			else return(0);
		}
		else
		{
			if((biostime(0,0L)-pooo[ob])>600L) // если время вышло
			{
				if(pooo[ob]!=2)pooo[ob]=0L;
				TIMER_N[i][0]=0;
				return(-1);
			}
			else return(0);
		}
	}
	if(N_time<25)
	{
		TIMER_N[N_time][0]=ob;    // запомнить объект
		TIMER_N[N_time][1]=tip;   // запомнить тип объекта
		TIMER_N[N_time][2]=ob_ini;// запомнить инициирующий объект
		pooo[ob]=biostime(0,0L);  // зафиксировать время запуска таймера
		if((tip!=21)&&(tip!=20)&&(tip!=3))pooo_com[ob]=pooo[ob];
		return(0);
	}
	return(-1);// выйти, если нет свободных таймеров
}
//----------------------------------------------------------
void sbros_time(int ob1,int ob2)
{
	int i;

	nom_func("293");
	for(i=0;i<25;i++)
	{
    if((TIMER_N[i][0]==ob1)&&(TIMER_N[i][2]==ob2))
    {
      TIMER_N[i][0]=0;
      TIMER_N[i][1]=0;
      TIMER_N[i][2]=0;
			if(pooo[ob1]!=0)pooo[ob1]=0L;
		}
  }
	return;
}
/*******************************************************/
#ifdef POLUAVTOMAT
/*----------------изображение стрелки отправления-------*/
void analiz_otprav(int obekt,int bit_)
{
	int nop_chop,nlp_chlp,nln_chln,cvet_otpr,cvet_per,kj,X,Y,t_0,cod,j;
	/*nop_chop - номер объекта,в котором передаются данные по реле НОП-ЧОП */
	/*nlp_chlp - номер объекта,в котором передаются данные по реле НЛП-ЧЛП */
  /*nln_chln - номер объекта,в котором передаются данные по реле НЛН-ЧЛН */
  /*kj - номер объекта для ключа-жезла */
  /*napr - направление отправления для данного объекта 0-четное, 1-нечетное */
  int bit_nop_chop,bit_nlp_chlp,bit_nln_chln,bit_kj,napr,otprav[6],per[8];
  if((bit_==5)&&(fr3[obekt][5]!=1))bit_=0;
  nop_chop=fr1[obekt][2];bit_nop_chop=fr1[obekt][3];
  nlp_chlp=fr1[obekt][4];bit_nlp_chlp=fr1[obekt][5];
  nln_chln=fr1[obekt][6];bit_nln_chln=fr1[obekt][7];
  kj=fr1[obekt][8];bit_kj=fr1[obekt][9];
  cod=fr3[kj][bit_kj]*8+
  fr3[nlp_chlp][bit_nlp_chlp]*4+
  fr3[nln_chln][bit_nln_chln]*2+
  fr3[nop_chop][bit_nop_chop];
  if((bit_==5)&&(fr3[obekt][5]!=1))bit_=0;
  napr=fr1[obekt][13]-1;
  if((nikuda==1)||(klo==1)||(help!=0))return;
  switch(cod)
  {
    case 0: sbros_time(nop_chop,nop_chop);//нет согласия на отправление,исходное
            cvet_otpr=8;
            cvet_per=8;
            COD_OTPR[napr]=cod;
            hoz_poezd[napr]=0;
            break;

    case 1: if((COD_OTPR[napr]==0)||  // перегон занят по отправлению
            (COD_OTPR[napr]==9)||  
            (COD_OTPR[napr]==5)||  
            (COD_OTPR[napr]==1)||  
            (COD_OTPR[napr]==3))
            {
              sbros_time(nop_chop,nop_chop);
              cvet_otpr=8;
              cvet_per=4;
						}
            else
            {
							otkaz_obkt(nop_chop,bit_nop_chop,1);
              cvet_otpr=8;
              cvet_per=4;
            }
            COD_OTPR[napr]=cod;
            break;

    case 2: if(COD_OTPR[napr]!=2)//неисправность ПАБ 
            {
              sbros_time(nop_chop,nop_chop);
							t_0=TIMER_SOB(nop_chop,51,nop_chop);
            }
            else
            {
              if((biostime(0,0l)-pooo[nop_chop])>70l)
              {
                otkaz_obkt(nln_chln,bit_nln_chln,0);
                sbros_time(nop_chop,nop_chop);
              }
            }
            cvet_otpr=13;
            cvet_per=8;
            COD_OTPR[napr]=cod;
            break;

    case 3: if(COD_OTPR[napr]!=3)// неисправность ПАБ 
            {
              sbros_time(nop_chop,nop_chop);
              cvet_otpr=8;
							t_0=TIMER_SOB(nop_chop,51,nop_chop);
            }
            else
						{
              if((biostime(0,0l)-pooo[nop_chop])>70l)
              {
								otkaz_obkt(nln_chln,bit_nln_chln,0);
                cvet_otpr=13;
                sbros_time(nop_chop,nop_chop);
              }
              else cvet_otpr=8;
            }
            cvet_per=4;
            COD_OTPR[napr]=cod;
            break;

    case 4: sbros_time(nop_chop,nop_chop);//нет согласия на отправление,исходное
            cvet_otpr=8;
            cvet_per=8;
            COD_OTPR[napr]=cod;
            break;

    case 5: if((COD_OTPR[napr]==6)||  // отправлен поезд по сигналу
            (COD_OTPR[napr]==4)||  
            (COD_OTPR[napr]==7)||  
            (COD_OTPR[napr]==5)||  
            (COD_OTPR[napr]==13)||  
            (COD_OTPR[napr]==1))
            {
//              vopros(18,obekt,77,"");// отправлен поезд на станцию
              zvuk_vkl(1,10);
              sbros_time(nop_chop,nop_chop);
            }
            else
            {
              otkaz_obkt(nop_chop,bit_nop_chop,1);
              sbros_time(nop_chop,nop_chop);
            }
						cvet_otpr=8;
            cvet_per=4;
            COD_OTPR[napr]=cod;
						break;

    case 6: if((COD_OTPR[napr]==2)||  // получено согласие на отправление
            (COD_OTPR[napr]==4)||  
            (COD_OTPR[napr]==14)||  
            (COD_OTPR[napr]==6)||  
            (COD_OTPR[napr]==0))sbros_time(nop_chop,nop_chop);
            else
            {
              otkaz_obkt(nop_chop,bit_nop_chop,1);
              sbros_time(nop_chop,nop_chop);
            }
            cvet_otpr=10;
            cvet_per=8;
            COD_OTPR[napr]=cod;
            break;

    case 7: if((COD_OTPR[napr]==0)&&(hoz_poezd[napr]==0))//если первое включение
            {
                hoz_poezd[napr]=2;
                menu_hoz_poezd(napr); //выдать запрос о хозпоезде 
								t_0=TIMER_SOB(nop_chop,51,nop_chop);//запустить таймер
                COD_OTPR[napr]=cod;
                return;
            }
            if((COD_OTPR[napr]==7)&&(hoz_poezd[napr]==0))//если то же
            {                                       //и нет хозпоезда 
              if(biostime(0,0l)-pooo[nop_chop]>70l)//если время вышло 
              {
                otkaz_obkt(nln_chln,bit_nln_chln,0);//отказ НЛП/ЧЛП 
                sbros_time(nop_chop,nop_chop);
                COD_OTPR[napr]=cod;
								cvet_otpr=13;
                cvet_per=4;
                break;
							}
              else return;//  если время не вышло - подождать 
            }
            if((COD_OTPR[napr]==15)&&(hoz_poezd[napr]==1))//если то же
            {                                       //и хозпоезд
                sbros_time(nop_chop,nop_chop);
                COD_OTPR[napr]=cod;
                cvet_otpr=14;
                cvet_per=4;
                break;
            }
            if((COD_OTPR[napr]==7)&&(hoz_poezd[napr]==1))//если то же
            {                                       //и хозпоезд
              if(biostime(0,0l)-pooo[nop_chop]>70l)//если время вышло
              {
                sbros_time(nop_chop,nop_chop);
                COD_OTPR[napr]=cod;
                cvet_otpr=14;
                cvet_per=4;
                break;
              }
              else return;//  если время не вышло - подождать 
            }
            if((COD_OTPR[napr]!=0)&& 
            (hoz_poezd[napr]!=1)&& 
            (pooo[nop_chop]==0))//если не включение и нет хозпоезда 
            {
							t_0=TIMER_SOB(nop_chop,51,nop_chop); //перезапустить таймер
              return;
            }
            if((COD_OTPR[napr]==5)||(COD_OTPR[napr]==3))
            {
							if((biostime(0,0l)-pooo[nop_chop])>70l)//если время вышло
              {
                otkaz_obkt(nln_chln,bit_nln_chln,0);//отказ НЛН/ЧЛН 
								sbros_time(nop_chop,nop_chop);
                COD_OTPR[napr]=cod;
                cvet_otpr=13;
                cvet_per=4;
                break;
              }
            }
            if(COD_OTPR[napr]==6)
            {
              if((biostime(0,0l)-pooo[nop_chop])>70l)//если время вышло 
              {
                otkaz_obkt(nln_chln,bit_nln_chln,0);//отказ НЛН/ЧЛН 
                sbros_time(nop_chop,nop_chop);
                cvet_otpr=13;
                cvet_per=4;
                COD_OTPR[napr]=cod;
                break;
              }
            }
            if((COD_OTPR[napr]==7)&&(hoz_poezd==0))
            {
              cvet_otpr=13;
              cvet_per=4;
              COD_OTPR[napr]=cod;
              break;
            }

    case 8: cvet_otpr=8;
            cvet_per=8;
            COD_OTPR[napr]=cod;
            break;

		case 9: cvet_otpr=8;
            cvet_per=4;
            COD_OTPR[napr]=cod;
						break;

    case 10:
            cvet_otpr=5;
            cvet_per=8;
            otkaz_obkt(nln_chln,bit_nln_chln,0);
            if(get_str(kj,bit_nln_chln)==0)neisprav_na_disk('@',chudo);
            COD_OTPR[napr]=cod;
            break;


    case 11:cvet_otpr=14;
            cvet_per=4;
            COD_OTPR[napr]=cod;
            break;

    case 13:              // хоз.поезд на перегоне,сняли согласие
            cvet_otpr=8;
						cvet_per=4;
						COD_OTPR[napr]=cod;
						break;

		case 14: cvet_otpr=14;
						 cvet_per=8;
						 COD_OTPR[napr]=cod;
						 break;

		case 15: cvet_otpr=14;
						 cvet_per=4;
						 hoz_poezd[napr]=1;
						 COD_OTPR[napr]=cod;
						 break;

		default: if(pooo[nop_chop]==0l)t_0=TIMER_SOB(nop_chop,51,nop_chop);
						 if(biostime(0,0l)-pooo[nop_chop]>70l)
						 {
								otkaz_obkt(obekt,bit_,0);
								sbros_time(nop_chop,nop_chop);
								break;
						 }
						 COD_OTPR[napr]=cod;
						 cvet_otpr=5;
						 cvet_per=13;
						 break;
	}
/**-------------------------------------------------------------------**/
	if(fr3[obekt][5]==1) /* если выявлена непарафазность */
	{
		cvet_otpr=11; cvet_per=11;
		slom_interf(obekt);
	}
	if(fr1[obekt][13]==0)/* если ТУМС-1 */
	{
		for(j=0;j<6;j++)otprav[j]=left_up_left[j+4];
		for(j=0;j<4;j++)per[j]=left_up_left[j];
		for(j=0;j<4;j++)per[j+4]=left_up_left[j+10];
	}
	if(fr1[obekt][13]==1) /* если ТУМС-2 */
	{
		for(j=0;j<6;j++)otprav[j]=right_up_right[j+4];
		for(j=0;j<4;j++)per[j]=right_up_right[j];
		for(j=0;j<4;j++)per[j+4]=right_up_right[j+10];
	}
	setcolor(cvet_otpr);
	setfillstyle(1,cvet_otpr);
	setlinestyle(0,0,0);
	fillpoly(3,otprav);
	setcolor(cvet_per);
	setfillstyle(1,cvet_per);
	fillpoly(4,per);
	return;
}
/***************************************************************************/
void analiz_priem(int obekt,int bit_)
{
	int ndso_chdso,npo_chpo,nfp_chfp,nfdp_chfdp,cvet_priem,X,Y,t_0,cod,j;
	int bit_ndso_chdso,bit_npo_chpo,bit_nfp_chfp,bit_nfdp_chfdp,priem[6],per[8];
	int cvet_knop_ds,cvet_knop_dp,cvet_per,napr;
  if((bit_==5)&&(fr3[obekt][5]!=1))bit_=0;
  ndso_chdso=fr1[obekt][2];bit_ndso_chdso=fr1[obekt][3];
  npo_chpo=fr1[obekt][4];bit_npo_chpo=fr1[obekt][5];
  nfp_chfp=fr1[obekt][6];bit_nfp_chfp=fr1[obekt][7];
  nfdp_chfdp=fr1[obekt][8];bit_nfdp_chfdp=fr1[obekt][9];
  cod=fr3[nfp_chfp][bit_nfp_chfp]*8+fr3[nfdp_chfdp][bit_nfdp_chfdp]*4+
  fr3[ndso_chdso][bit_ndso_chdso]*2+fr3[npo_chpo][bit_npo_chpo];
  napr=fr1[ndso_chdso][13]-1;
  if((nikuda==1)||(klo==1)||(help!=0))return;
  switch(cod)
  {
    case 0: {
              if(COD_PRIEM[napr]!=0)
              COD_PRIEM[napr]=COD_PRIEM[napr];
              sbros_time(ndso_chdso,ndso_chdso); /* исходное состояние приема */
              pooo[ndso_chdso]=0l;
              cvet_priem= 8;cvet_per=8;
              cvet_knop_ds=8;cvet_knop_dp=8;
              break;
            }
    case 1: if(COD_PRIEM[napr]==2)
            {
              if(biostime(0,0l)-pooo[ndso_chdso]<35l)
              {
                sbros_time(ndso_chdso,ndso_chdso);
                otkaz_obkt(npo_chpo,bit_,1);
                pooo[ndso_chdso]=0l;
              }
              else
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
//                vopros(18,obekt,14,"");
              }
            }
            if(COD_PRIEM[napr]==3)
            {
              sbros_time(ndso_chdso,ndso_chdso);
              pooo[ndso_chdso]=0l;
//              vopros(18,obekt,14,"");
            }
            if(COD_PRIEM[napr]==0)
            {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
								t_0=TIMER_SOB(ndso_chdso,52,ndso_chdso);//перезарядить таймер
            }
            if((COD_PRIEM[napr]==1)&&(pooo[ndso_chdso]!=0l))//если то же состояние

            { if((biostime(0,0l)-pooo[ndso_chdso])>70l)
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
                otkaz_obkt(npo_chpo,bit_npo_chpo,1);
                zvuk_vkl(0,0);
              }
            }
            cvet_priem=8;cvet_per=4;
            cvet_knop_ds=8;cvet_knop_dp=8;
            break;
    case 2: if((COD_PRIEM[napr]==0)&&(pooo[ndso_chdso]==0l))
            {
							t_0=TIMER_SOB(ndso_chdso,52,obekt);
            }
            cvet_priem=14;cvet_per=8;       // выдано согласие
            cvet_knop_ds=14;cvet_knop_dp=8;
            COD_PRIEM[napr]=cod;
            break;
    case 3: if(COD_PRIEM[napr]==2)//если было выдано согласие на прием
            {
              if(biostime(0,0l)-pooo[ndso_chdso]<18l)//если прошло менее 2 сек
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
                otkaz_obkt(npo_chpo,bit_,1);
              }
              else //если прошло более 2 сек
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
								t_0=TIMER_SOB(ndso_chdso,52,ndso_chdso);//перезарядить таймер
              }
            }
            else
            {
              if((COD_PRIEM[napr]==3)&&(pooo[ndso_chdso]!=0l))//если то же состояние

              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
                otkaz_obkt(ndso_chdso,bit_,1);
              }
            }
            cvet_priem=14;cvet_per=4;
            cvet_knop_ds=14;cvet_knop_dp=8;
            zvuk_vkl(1,18);
            break;
    case 4: if(COD_PRIEM[napr]==12)
            {
              if((biostime(0,0L)-pooo[ndso_chdso])<35L)
              {
                otkaz_obkt(nfp_chfp,0,1);
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
              }
              else //если прошло более 2 сек
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
              }
            }
            cvet_priem=8;cvet_per=8;
            cvet_knop_ds=8;cvet_knop_dp=14;
            break;
    case 9: if((COD_PRIEM[napr]!=1)&&(COD_PRIEM[napr]!=9))otkaz_obkt(nfp_chfp,bit_,1);
            if(COD_PRIEM[napr]==1)
            {
              sbros_time(ndso_chdso,ndso_chdso);
              pooo[ndso_chdso]=0l;
            }
            if(COD_PRIEM[napr]==9)//если то же состояние
            {
              sbros_time(ndso_chdso,ndso_chdso);
              pooo[ndso_chdso]=0l;
            }

            cvet_priem=8;cvet_per=14;
            cvet_knop_ds=8;cvet_knop_dp=8;
            break;
    case 12:if(COD_PRIEM[napr]==13)
            {
              sbros_time(ndso_chdso,ndso_chdso);
              pooo[ndso_chdso]=0l;
							t_0=TIMER_SOB(ndso_chdso,52,ndso_chdso);
            }
            else
            {
              if((COD_PRIEM[napr]==12)&&(pooo[ndso_chdso]!=0l))//если то же состояние

              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
              }
              else
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
								t_0=TIMER_SOB(ndso_chdso,52,ndso_chdso);
              }
            }
            cvet_priem=8;cvet_per=14;
            cvet_knop_ds=8;cvet_knop_dp=14;
            break;
    case 13:if(COD_PRIEM[napr]==9)
            {
              sbros_time(ndso_chdso,ndso_chdso);
              pooo[ndso_chdso]=0l;
							t_0=TIMER_SOB(ndso_chdso,52,ndso_chdso);
            }
            else
            {
              if((COD_PRIEM[napr]==13)&&(pooo[ndso_chdso]!=0l))//если то же состояние

              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
                otkaz_obkt(npo_chpo,bit_,1);
              }
              else
              {
                sbros_time(ndso_chdso,ndso_chdso);
                pooo[ndso_chdso]=0l;
              }
            }
            cvet_priem=8;cvet_per=14;
            cvet_knop_ds=8;cvet_knop_dp=14;
            break;
    default:
              sbros_time(ndso_chdso,ndso_chdso);
              cvet_priem=5;cvet_per=13;
              cvet_knop_ds=5;cvet_knop_dp=5;
              break;
  }
  COD_PRIEM[napr]=cod;
  if(fr3[obekt][5]==1) /* если выявлена непарафазность */
  {
    cvet_priem=11;
    cvet_per=11;
		slom_interf(obekt);
  }
  setcolor(cvet_priem);setfillstyle(1,cvet_priem);
  if(fr1[npo_chpo][13]==1)/* если ТУМС-2 */
  {
    for(j=0;j<6;j++)priem[j]=left_up_right[j+4];
    for(j=0;j<4;j++)per[j]=left_up_right[j];
    for(j=0;j<4;j++)per[j+4]=left_up_right[j+10];
  }
  if(fr1[npo_chpo][13]==0) /* если ТУМС-1 */
  {
    for(j=0;j<6;j++)priem[j]=right_up_left[j+4];
    for(j=0;j<4;j++)per[j]=right_up_left[j];
    for(j=0;j<4;j++)per[j+4]=right_up_left[j+10];
  }
  setcolor(cvet_priem);
  setfillstyle(1,cvet_priem);
  setlinestyle(0,0,0);
  fillpoly(3,priem);
  setcolor(cvet_per);
  setfillstyle(1,cvet_per);
  fillpoly(4,per);
  X=markery[fr1[ndso_chdso][12]][4];
  Y=markery[fr1[ndso_chdso][12]][5];
  setfillstyle(1,cvet_knop_ds);
  bar(X-5,Y-5,X+5,Y+5);
  X=markery[fr1[nfdp_chfdp][12]][4];
  Y=markery[fr1[nfdp_chfdp][12]][5];
  setfillstyle(1,cvet_knop_dp);
  bar(X-5,Y-5,X+5,Y+5);

}
#endif
//---------------------------------------------------------------------
/***********************************************************\
* Процедура отображения ламп, переданных в одном сообщении, *
* цвета которых зависят от состояния всех ламп в комплексе  *
* максимальные возможности отображения: до 3-х ламп на      *
* 8 состояний (цветов) каждая                               *
* param - признак вызова функции 0-обычный, 1-из таймера    *
\***********************************************************/
void lampa_N(int obj,int bit_,int param)
{
	int cod,x[5],y[5],D,i,j;
	int pole[14];
	unsigned int tester,sdvig,cvet[3];
	char NAME_OBJ[30];

	nom_func("145");
	if(fr1[obj][1]==20)
	{
		for(i=0;i<30;i++)NAME_OBJ[i]=0;
		//получить код состояния объекта
		cod=fr3[obj][0]+fr3[obj][1]*2+fr3[obj][2]*4;
		//получить координаты и цвета для каждой лампы для данного состояния
		for(i=0;i<fr1[obj][12];i++)
		{
			x[i]=fr1[obj][2*i+3];
			if(x[i]==9999)continue;//если бит без данных
			y[i]=fr1[obj][2*i+4];
			sdvig=12-(cod%4)*4;
			tester=0xf<<sdvig;
			cvet[i]=cvt[fr1[obj][2]][cod/4+i*2]&tester;
			cvet[i]=cvet[i]>>sdvig;
			setcolor(cvet[i]);setlinestyle(0,0,0);
			if(fr3[obj][5]==1)setcolor(11);
			for(j=1;j<=fr1[obj][11];j++)circle(x[i],y[i],j);
		}
	}
	else
	if(fr1[obj][1]==220)
	{
		//получить код состояния объекта
		cod=fr3[obj][0]+fr3[obj][1]*2+fr3[obj][2]*4;
		//получить координаты и цвета для каждой лампы для данного состояния
		for(i=0;i<fr1[obj][12];i++)
		{
			x[i]=fr2[obj][2*i];
			if(x[i]==0)continue;//если бит без данных
			y[i]=fr2[obj][2*i+1];
			sdvig=12-(cod%4)*4;
			tester=0xf<<sdvig;
			cvet[i]=cvt[fr1[obj][2]][cod/4+i*2]&tester;
			cvet[i]=cvet[i]>>sdvig;
			if(cvet[i]==8)cvet[i]=7;
			if(fr3[obj][5]==1)cvet[i]=11;
			setfillstyle(1,cvet[i]);
			setlinestyle(0,0,0);
			D=textwidth(name_lamp[fr1[obj][i+3]]);
			pole[0]=x[i];					pole[1]=y[i];
			pole[2]=pole[0]+D+1;	pole[3]=pole[1];
			pole[4]=pole[2]+5;    pole[5]=pole[3]+5;
			pole[6]=pole[2];			pole[7]=pole[5]+5;
			pole[8]=pole[0];			pole[9]=pole[7];
			pole[10]=pole[0]-5;		pole[11]=pole[9]-5;
			pole[12]=pole[0];			pole[13]=pole[1];
			if((getpixel(x[i]+1,y[i]+1)!=cvet[i])||(cikl_obnov==0))
			{
				fillpoly(6,pole);
				setcolor(8);
				outtextxy(x[i],y[i]+2,name_lamp[fr1[obj][i+3]]);
				drawpoly(7,pole);
			}
			else
			{
				setcolor(8);
				outtextxy(x[i],y[i]+2,name_lamp[fr1[obj][i+3]]);
				obnovi_obraz1(x[i],y[i],D,name_lamp[fr1[obj][i+3]],cvet[i]);
			}
		}
	}
//формирование донесения для полученных кодов(применительно к переездам)
	if((cod==0)&&tst_fix(obj,bit_))
	{
		if(param==0)//если вход не из таймера
		{
			if(pooo[obj]==0)TIMER_SOB(obj,20,bit_);
		}
		else
		{

			if((get_str(obj,bit_)==0)&&(prorisovka==0))
			{
				neisprav_na_disk('',chudo);
				get_str(obj,0);
				w(175,999,chudo);
			}
			fix(obj,bit_);
			pooo[obj]=0;
		}
	}
	else
	if(((cod==5)||(cod==7))&&tst_fix(obj,bit_))
	{
		if(param==0)//если вход не из таймера
		{
			if(pooo[obj]==0)TIMER_SOB(obj,20,bit_);
		}
		else
		{
			if((get_str(obj,bit_)==0)&&(prorisovka==0))
			{
				get_str(obj,0);
				if(cod==5)neisprav_na_disk('',chudo);
				if(cod==7)neisprav_na_disk('',chudo);
				if((cod==5)||(cod==7))w(298,999,chudo);
			}
			fix(obj,bit_);
			pooo[obj]=0;
		}
	}
	else
	{
		fix(obj,bit_);
	}
	return;
}
//-------------------------------------------------
void ris_blok_uch(int nom,int bit_)
{
	int i,napr,ob_napr,bt_napr,cvtp_o;

	nom_func("284");
	ob_napr=fr1[nom][2];    // объект направления перегона
	bt_napr=fr1[nom][3]>>8; //бит направления
	napr=fr1[nom][3]&0x3; //направления для контрольных точек
	//если направление перегона не для точки
	if(napr==3)napr=fr3[ob_napr][bt_napr];
	if(napr==1)i=0;
	if((napr!=2)&&(fr3[ob_napr][bt_napr]!=napr))return;
	if((bit_!=5)&&(bit_>=fr1[nom][7]))return;
	cvtp_o=fr1[nom][8];//список цветов приема/отправления
	if(bit_==5)for(i=0;i<fr1[nom][7];i++)ris_blk(nom,i,cvtp_o,napr);
  else ris_blk(nom,bit_,cvtp_o,napr);
  return;
}
//-----------------------
void ris_blk(int nm,int ii,int cvtp_o,int npr)
{
  int bit_,cvet1,ij,rad,n_t,d_x,znak;
  char im[4];//имена для перегонных точек 
  for(ij=0;ij<4;ij++)im[ij]=0;//очистить имена для перегонных точек

	nom_func("283");
	bit_=ii;
  d_x=fr1[nm][6]&0xff;
  znak=fr1[nm][6]&0xf00;
  if(znak==0xf00)znak=-1;
  else znak=1;
  d_x=d_x*znak;
  rad=(fr1[nm][6]&0xf000)>>12;
	if(fr1[nm][9+bit_]!=-1)
  {
    if((npr==2)||(npr==1))n_t=(fr1[nm][9+bit_]&0xff00)>>8;
    else
    n_t=fr1[nm][9+bit_]&0xff;
    itoa(n_t,im,10);//заполнить имена точек
    if((npr==0)||(npr==2))//если прием или не ориентированная точка
    { //если точка свободна
      if(fr3[nm][bit_]==0)cvet1=cvt[cvtp_o][2*bit_];
      //если точка занята
      else
        cvet1=cvt[cvtp_o][2*bit_+1];
    }
    else //если отправление
    { //если точка свободна
      if(fr3[nm][bit_]==0)cvet1=cvt[cvtp_o][2*bit_];
      //если точка занята
      else cvet1=cvt[cvtp_o][2*bit_+1];
    }
    if(fr3[nm][5]==1)cvet1=11;
  }
  setcolor(cvet1);
  setlinestyle(0,0,0);
	for(ii=rad;ii>=1;ii--)circle(fr1[nm][4]+bit_*d_x,fr1[nm][5],ii);
  moveto(fr1[nm][4]+d_x*bit_-6,fr1[nm][5]-14);
  setcolor(7);outtext("██");
  moveto(fr1[nm][4]+d_x*bit_-6,fr1[nm][5]-14);
  setcolor(cvet1);outtext(im);
	return;
}
//----------------------------------------------------------------
/************************************************\
* Процедура обработки информации по данным,      *
* полученным для пригласительных сигналов и ПСО. *
\************************************************/
void ps_pso(int ob,int tip)
{
	int pso,ps,prf,sig_test,PSO,ips,isig,cvt_s,cvt_ob,N_o=0,i,N_vkl=0,N_vkl1=0;

	nom_func("255");
	if(fr1[ob][1]==37)pso=fr1[ob][2];//если это пригласительный-получить ПСО
	else pso=ob; //иначе это сам ПСО
	if(fr3[pso][0]==1)test_tmp=1;//если ПСО сработал
  for(ips=0;ips<fr1[pso][12];ips++) //пройти по объектам пригласительных
	{ ps=0xFFF&fr1[pso][ips+2];//получить объект с пригласительными сигналами
    for(isig=0;isig<fr1[ps][12];isig++)//пройти по пригласительным сигналам
    { //подсчет включенных пригласительных
      if((0xF000&fr1[pso][ips+2])>0)N_vkl=N_vkl+fr3[ps][isig];//для сигналов ПСО
      if((0xF000&fr1[pso][ips+2])==0)N_vkl1=N_vkl1+fr3[ps][isig];//для входных
    }
	}
	for(ips=0;ips<fr1[pso][12];ips++)//пройти по объектам пригласительных
	{
		if(parafaz(pso)<0)prf=1;//получить признак парафазности ПСО
		else prf=0;
		if(fr3[pso][0]==1)
		PSO=1;//если ПСО включен, то установить признак
		else PSO=0;
		ps=0xFFF&fr1[pso][ips+2];//получить объект пригласительных
		if(parafaz(ps)<0)prf=prf+2; //добавить парафазность пригласительного
		for(isig=0;isig<fr1[ps][12];isig++)//пройти по сигналам
		{
			if(fr1[ps][2*isig+3]!=9999)//если очередной сигнал в объекте есть
			{
				switch(prf)
				{ //парафазность в норме для обоих
					case 0: if(fr3[ps][isig]==1)//если сигнал открыт
									{
										if((0xF000&fr1[pso][ips+2])>0)sig_test=1;//если сигнал для ПСО
										else {sig_test=3;break;}
									}
									else//если сигнал закрыт
									{
										sig_test=0;
										if((0xF000&fr1[pso][ips+2])==0)break;//если сигнал для ПСО
									}
									sig_test=2*PSO+sig_test;
									break;
					//ПСО не парафазен
					case 1: cvt_s=7;
									if((0xF000&fr1[pso][ips+2])!=0)cvt_ob=11;
									else cvt_ob=7;
									break;
					//ПС не парафазен
					case 2: cvt_s=11;
									if((0xF000&fr1[pso][ips+2])!=0)cvt_ob=7;
									else cvt_ob=11;
									break;
					//не парафазны и ПСО и ПС
					case 3: cvt_s=11;cvt_ob=11;break;
				}
				if(prf!=0)goto cikl;//при наличии непарафазности пропустить анализ
				switch(sig_test)
				{
					case 0: cvt_s=7;cvt_ob=7;  //сигнал закрыт ПСО=0
									if(N_vkl==0&&PSO==0)//если все выключено
									{
										if(pooo[pso]!=0l)sbros_time(pso,pso);
										fix(pso,0);
										if((0xF000&fr1[pso][ips+2])>0)fix(ps,isig);
									}
									if((N_vkl1==0)&&((0xF000&fr1[pso][ips+2])==0))fix(ps,isig);

									break;
					case 1: cvt_ob=15; //сигнал открыт ПСО=0
									if(tip==5)//если вход из таймерного анализа
									{
										if(get_str(ps,isig)==0)neisprav_na_disk('@',chudo);
										chudo[strlen(chudo)-2]=0;
										w(282,999,chudo);
										zvuk_vkl(3,0);
										cvt_s=7;
										fix(ps,isig);
										fix(pso,0);
									}
									else//если вход не из таймера
									{
										if(tst_fix(ps,isig))//если событие пригласительного новое
										{
											cvt_s=15;
											if(pooo[pso]==0l)
											{
												TIMER_SOB(pso,2,pso);//запустить таймер
												pooo_com[pso]=0;
											}

										}
										else
											if(tst_fix(pso,0))//если событие ПСо новое
											{
												if(pooo[pso]==0l)
												{
													TIMER_SOB(pso,2,pso);
													pooo_com[pso]=0;
												}
												cvt_s=15;
											}
											else cvt_s=7;
									}
									N_o++;
									break;
					case 2: cvt_ob=7;cvt_s=7;//сигнал закрыт ПСО=1
									if(tip==5)//если вход из таймерного анализа
									{
										if(get_str(pso,0)==0)neisprav_na_disk('@',chudo);
										w(279,999,"");
										zvuk_vkl(3,0);
										fix(ps,isig);
										fix(pso,0);
										pooo[pso]=0l;
										tip=0;
									}
									else
									if(tst_fix(pso,0))//если новое событие ПСо
									{
										if(pooo[pso]==0l)
										{
											TIMER_SOB(pso,2,pso);
											pooo_com[pso]=0;
										}
//										fix(pso,0);
									}
									if(tst_fix(ps,isig))//если новае событие пригласительного
									{
										if(pooo[pso]==0l)
										{
											TIMER_SOB(pso,2,pso);
											pooo_com[pso]=0;
										}
//										fix(ps,isig);
									}
									break;
					case 3: cvt_s=15;cvt_ob=15;//сигнал открыт ПСО=1
									if(tst_fix(pso,0)||tst_fix(ps,isig))//если из анализа таймера
									{
										if(get_str(ps,isig)==0)strcat(chudo,"-откр.");
										neisprav_na_disk('$',chudo);
										if(pooo[pso]!=0l)sbros_time(pso,pso);
										fix(ps,isig);fix(pso,0);//зафиксировать событие
									}
									N_o++;
									break;
				}
cikl:
				setlinestyle(0,0,0);setcolor(cvt_ob);
				circle(fr1[ps][isig*2+3],fr1[ps][isig*2+4],fr1[ps][11]);
				setcolor(cvt_s);
				for(i=1;i<fr1[ps][11];i++)circle(fr1[ps][isig*2+3],fr1[ps][isig*2+4],i);
			}
		}
	}
	if(N_o>1)//если открыто более одного ПС
	{
		fr3[pso][6]=1;
		if(tst_fix(pso,6))
		{
			neisprav_na_disk('@',"ПС_откр.>2");
			w(235,999,"");
			zvuk_vkl(3,18);
		}
	}
	else fr3[pso][6]=0;
	fix(pso,6);
	return;
}
//--------------------------
