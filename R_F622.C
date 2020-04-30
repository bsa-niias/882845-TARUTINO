#include"bla.h"
//---------------------------------------------
void vuhod()
{
//  nom_func("4051");
//settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
	setlinestyle(0,0,0);
	setcolor(8);
	line(8,464,630,464);
	line(8,233,8,464);
	line(13*8,233,13*8,464);
	line(23*8,233,23*8,464);
	line(50*8,233,50*8,464);
	line(630,233,630,464);
	setfillstyle(SOLID_FILL,cvv);
	bar(15*8,472,15*8+250,480);
	moveto(15*8,472);outtext("Выход - '");setcolor(MAGENTA);outtext("F6");
	setcolor(8);outtext("',вперед - '");setcolor(MAGENTA);outtext("PgDn");
	setcolor(8);outtext("',назад - '");setcolor(MAGENTA);outtext("PgUp");
	setcolor(8);outtext("'");
	return;
}
//-----------------------------------------------------------
void vuhod1()
{
//  nom_func("405");
	//settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
	setlinestyle(0,0,0);
	setcolor(8);
	line(8,464,630,464);
	setfillstyle(SOLID_FILL,cvv);
	bar(15*8,472,15*8+250,480);
	moveto(15*8,472);outtext("Выход - '");setcolor(MAGENTA);outtext("F5");
	setcolor(8);outtext("',вперед - '");setcolor(MAGENTA);outtext("PgDn");
	setcolor(8);outtext("',назад - '");setcolor(MAGENTA);outtext("PgUp");
	setcolor(8);outtext("'");
	return;
}
/*************************************************/
void shl()
{
//  nom_func("302");
	setlinestyle(0,0,0); setcolor(8); line(8,213,630,213);
	moveto(4*8,220);outtext("ДАТА");
	moveto(15*8,220);outtext("ВРЕМЯ");
	moveto(23*8,216);outtext(" ДИАГНОСТИКА АППАРАТУРЫ ТУМС");
	moveto(56*8,216);outtext("НЕИСПРАВНОСТИ");
	moveto(56*8,225);outtext("  ИНТЕРФЕЙСА");
	line(8,213,8,233);line(13*8,213,13*8,234);
	line(23*8,213,23*8,234);line(50*8,213,50*8,234);
	line(630,213,630,234);line(8,234,630,234);
	return;
}
//------------------------------------------------------------------
/*********************************************************\
* Очищает верхнюю часть экрана для отображения мнемосхемы *
\*********************************************************/
void clear_half()
{
//  nom_func("23");
	setfillstyle(SOLID_FILL,getbkcolor());
	bar(0,210,getmaxx(),getmaxy());
	return;
}
//------------------------------------------------------------------
/***************************************************\
* Процедура изображения фонового рисунка мнемосхемы *
* состояния каналов обмена УВК                      *
\***************************************************/
void fon_pic()
{
  char AAq[2]="";
#ifdef NALAD
  nom_func("69");
#endif
  /****     это фон для картинки     ************/
  setfillstyle(SOLID_FILL,getbkcolor());
  bar(32,5,getmaxx()-32,205);
  setfillstyle(CLOSE_DOT_FILL,YELLOW);
  bar(32,5,getmaxx()-32,205);
  setcolor(MAGENTA);
  setlinestyle(0,0,0);
  rectangle(36,7,getmaxx()-36,203);
  setlinestyle(0,0,3);
  rectangle(40,10,getmaxx()-40,200);

  setcolor(8);
  setlinestyle(0,0,0);
#ifdef KOL_SOO2
  rectangle(275,20,355,100);
  outtextxy(263,20,"Т");
  outtextxy(263,28,"У");
  outtextxy(263,36,"М");
  outtextxy(263,44,"С");
  outtextxy(263,52," ");
  outtextxy(263,58,"2");
  rectangle(282,25,347,50);
  rectangle(282,53,347,78);
#endif
  rectangle(275,118,355,195);
  outtextxy(263,150,"Т");
  outtextxy(263,158,"У");
  outtextxy(263,166,"М");
  outtextxy(263,174,"С");
  outtextxy(263,182," ");
  outtextxy(263,188,"1");
  rectangle(282,122,347,147);
  rectangle(282,150,347,175);
  setcolor(8);
  //settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
  moveto(468,70);
  outtext("ПЭВМ");
  if(NOMER_ARMA==1) outtext("2");
  else outtext("1");
  moveto(130,70);
  outtext("ПЭВМ");
  itoa(NOMER_ARMA,AAq,10);
  outtext(AAq);
  setcolor(8);
  setlinestyle(0,0,3);
  rectangle(467,87,507,133);
  rectangle(127,87,167,133);
	return;
}
/************************************************/
sost_komplect(int KKS)
{
  char slovo1[10],slovo2[10];
  int color1=0,color2=0,cod;
  int smeo=0;
#ifdef NALAD  
  nom_func("315");
#endif          
	if(KKS==KS1) smeo=97;
  else smeo=0;
  cod=fr3[KKS][0]+fr3[KKS][1]*2+fr3[KKS][2]*4;
  switch(cod)
  {
    case 0: color1=10;color2=14;
            strcpy(slovo1,"ИСПРAB");strcpy(slovo2,"ИСПРАВ");
            break;
    case 1: color2=10;color1=14;
            strcpy(slovo1,"ИСПРAB");strcpy(slovo2,"ИСПРАВ");
            break;
    case 2: color2=14;color1=12;
            strcpy(slovo1,"НЕИСПР");strcpy(slovo2,"ИСПРАВ");
            break;
    case 3: color2=10;color1=4;
            strcpy(slovo1,"НЕИСПР");strcpy(slovo2,"ИСПРАВ");
            break;
    case 4: color2=4;color1=10;
            strcpy(slovo1,"ИСПРАВ");strcpy(slovo2,"НЕИСПР");
            break;
    case 5: color2=12;color1=14;
            strcpy(slovo1,"ИСПРАВ");strcpy(slovo2,"НЕИСПР");
            break;
    case 6: color2=4;color1=12;
            strcpy(slovo1,"НЕИСПР");strcpy(slovo2,"НЕИСПР");
            break;
    case 7: color2=12;color1=4;
            strcpy(slovo1,"НЕИСПРАВ");strcpy(slovo2,"ИСПРАВ");
            break;
  }

x:
  if(((KKS==KS1)&&(otkaz_ts[0]==1))||((KKS==KS2)&&(otkaz_ts[1]==1))||(fr3[KKS][5]==1))color1=color2=3;
  setfillstyle(SOLID_FILL,color1);
  bar(283,26+smeo,346,49+smeo);
  if(((KKS==KS1)&&(otkaz_ts[0]==1))||((KKS==KS2)&&(otkaz_ts[1]==1))||(fr3[KKS][5]==1)) setcolor(3);
  else setcolor(8);
  outtextxy(290,38+smeo,slovo1);
  setfillstyle(SOLID_FILL,color2);
  bar(283,54+smeo,346,77+smeo);
  if(((KKS==KS1)&&(otkaz_ts[0]==1))||((KKS==KS2)&&(otkaz_ts[1]==1)))  setcolor(3);
  else setcolor(8);
  outtextxy(290,66+smeo,slovo2);
  setcolor(8);
  outtextxy(295,28+smeo,"ЭВМ 1");
  outtextxy(295,56+smeo,"ЭВМ 2");
	return;
}
//---------------------------------------------------------------------
/*****************************************************\
* Процедура отображения канала при просмотре          *
* состояния объектов УВК                              *
\*****************************************************/
void kanal(int kakoi)
{
  int st1=0,st2=0,cl1=0,cl11=0,cl2=0,cl22=0;
  char  right[2]={'\x10'},  // стрелка вправо ->
        left[2]={'\x11'};   // стрелка влево <-
#ifdef NALAD
  nom_func("114");
#endif
  if(kakoi==0)  /* свой */
  {
    setcolor(7); setlinestyle(3,0,0); //настройка для стирания
#ifdef KOL_SOO2
    line(200,75,275,75);     //для второй стойки стререть горизонталь
    line(200,100,200,75);    //для второй стереть вертикаль
    line(169,100,200,100);   //стереть горизонталь
#endif
    line(169,120,200,120);   //для первой стойки стереть горизонталь
    line(200,120,200,145);   //вертикаль
    line(200,145,275,145);   //горизонталь
    setcolor(8);
#ifdef KOL_SOO2
    if(otkaz_ts[1]==1) //если есть отказ своего ТС из второй стойки
		{
			st1=3;           // пунктир
			cl1=RED;         // красная
		}
    else // если нет отказа ТС
		{
			st1=0;           //обычная
			cl1=GREEN;       //зеленая
		}
    setlinestyle(st1,0,0); //стиль линии канала
    line(200,75,275,75);
    line(200,100,200,75);
    line(167,100,200,100);
    setcolor(cl1);
    outtextxy(215,72,left); //свой ТС для ТУМС-2 <-

    //если нет запретов и отказов своего ТУ для ТУМС-2 - цвет зеленый
    if((zapret[1]==0)&&(otkaz_tu[1]==0))cl2=GREEN;
    else cl2=RED; //иначе красный
    setcolor(cl2);

    outtextxy(250,72,right);  //стрелочка своего ТУ для ТУМС-2 ->
#endif
    if(otkaz_ts[0]==0) //если свой ТС от ТУМС-1 в норме
		{
			cl11=GREEN;
			st2=0;
		}
    else //если свой ТС от ТУМС-1 в ненорме
		{
			cl11=RED;
			st2=3;
		}
    setlinestyle(st2,0,0);
    line(167,120,200,120);
    line(200,120,200,145);
    line(200,145,275,145);
    setcolor(cl11);
    outtextxy(215,142,left); //свой ТС для ТУМС-1 <-

    if((zapret[0]==0)&&(otkaz_tu[0]==0))cl22=GREEN;
    else cl22=RED;
    setcolor(cl22);
    outtextxy(250,142,right);//свой ТУ для ТУМС-1
//#endif
  }
  else  //сосед
  {
    if(NEISPRAVEN==0) //если с соседом есть связь
    {
#ifdef KOL_SOO2
      if(otkaz_sos[1]==0) //если нет отказа ТС соседа от ТУМС-2
			{
				cl1=GREEN;
				st1=0;
			}
      else
			{
				cl1=RED;
				st1=3;
			}
#endif
      //если нет отказа ТС соседа от ТУМС-1
      if(otkaz_sos[0]==0){cl11=GREEN;st2=0;}
      else {cl11=RED;st2=3;}
    }
    else {cl1=cl11=8;st1=st2=3;} //если нет связи с соседом
    if(NEISPRAVEN==0) //если с соседом есть связь
    {
      if((otkaz_sos[0]==0)&&(otkaz_tu_sos[0]==0))cl22=GREEN;
      else cl22=RED;
#ifdef KOL_SOO2
      if((otkaz_sos[1]==0)&&(otkaz_tu_sos[1]==0))cl2=GREEN;
      else cl2=RED;
#endif
    }
    else cl2=cl22=8;
    setcolor(7);
    setlinestyle(3,0,0);
    line(355,145,430,145);
    line(430,145,430,120);
    line(430,120,466,120);
#ifdef KOL_SOO2
    line(355,75,430,75);
    line(430,75,430,100);
    line(430,100,466,100);
    setcolor(8);
    setlinestyle(st1,0,0);
    line(355,75,430,75);
    line(430,75,430,100);
    line(430,100,466,100);
    setcolor(cl1);
    outtextxy(410,72,right);
    setcolor(cl2);
    outtextxy(375,72,left);
#endif
    setcolor(8);
    setlinestyle(st2,0,0);
    line(355,145,430,145);
    line(430,145,430,120);
    line(430,120,466,120);
    setcolor(cl11);
    outtextxy(410,142,right); //ТС соседа -> от первого ТУМС
    setcolor(cl22);
    outtextxy(375,142,left);  // ТУ соседа <- к первому ТУМС
  }
	return;
}
//---------------------------------------------------------------
/**************************************\
* Процедура вывода на экран мнемосхемы *
* состояний ПЭВМ (своей и соседней).   *
* no - признак ПЭВМ 0-своя, 1-сосед    *
* is - код исправности (0-исправна)    *
\**************************************/
void PVM(int no,int is)
{
	char right[2]={'\x10'}, /* ts */
		 left[2]={'\x11'}; /* ty */
//  nom_func("258");
	if(no==0)	//если своя ПЭВМ
	{
		if(STATUS==0) setfillstyle(SOLID_FILL,YELLOW); //если резервная
		else
			if(STATUS==2) setfillstyle(SOLID_FILL,8); //если ШН
			else setfillstyle(SOLID_FILL,GREEN); //если основная
		bar(128,88,166,132);
	}
	else
	{
	// соседняя ПЭВМ
	 if(is==1) setfillstyle(SOLID_FILL,RED); //если неисправна
	 else
		if(STATUS==1) setfillstyle(SOLID_FILL,YELLOW); //если резервная
			else
			if(STATUS==2) setfillstyle(SOLID_FILL,8); //если ШН
				else setfillstyle(SOLID_FILL,GREEN);
		bar(468,88,506,132);
	}
	// канал ПЭВМ-ПЭ ВМ
	setcolor(8);
	if(is==1)  setlinestyle(3,0,0);
	else setlinestyle(0,0,0);
	line(167,110,464,110);
	// стрелки между ПЭВМ
	if(is==1) setcolor(RED);
	else setcolor(GREEN);
	outtextxy(250,107,left);
	setcolor(GREEN);
	outtextxy(377,107,right);
	return;
}
//------------------------------------------------------------------------
/********************************************************\
* Процедура изображения мнемосхемы состояния каналов УВК *
\********************************************************/
void kartina()
{
#ifdef NALAD
  nom_func("117");
#endif
	if((nikuda==1)||(help!=0)) return;
	if(klo==0) fon_pic();
	// состояние собственной ПЭВМ
	PVM(0,0);
  //состояние соседней ПЭВМ
	PVM(1,NEISPRAVEN);
	// состояние комплектов ТУМС-1
	sost_komplect(KS1);
#ifdef KOL_SOO2
  // состояние комплектов ТУМС-2
	sost_komplect(KS2);
#endif
	kanal(0); //каналы своей ПЭВМ
	kanal(1); //каналы соседней ПЭВМ
  //изображение состояния комплекта основного для ТУМС-1
	kps_osn(KVV1);
  //изображение состояния комплекта резервного для ТУМС-1
	kps_rez(KVV1);

	tel_vkl_vikl(TEL1); //состояние РУ-ОРУ ТУМС-1
  if(DISK!=0)tele(TEL1);
#ifdef KOL_SOO2
  //изображение состояния комплекта основного для ТУМС-2
  kps_osn(KVV2);
  //изображение состояния комплекта резервного для ТУМС-2
  kps_rez(KVV2);
  tel_vkl_vikl(TEL2); //состояние РУ-ОРУ ТУМС-2
  if(DISK!=0)tele(TEL2);
#endif
	return;
}
//------------------------------------------------------------------
/************************************************\
* Процедура записи в архив состояния УВК данных  *
* о состоянии первого и второго полукомплектов   *
* ТУМС.                                          *
* nom - номер объекта с данными о полукомплектах.*
\************************************************/
void first_second(int nom)
{
  int nnom,aoar;
#ifdef NALAD
  nom_func("63");
#endif
  if((nom==KS1)||(nom==KS2))//если объект соответствует назначению
  {
    nnom=0;
    if(nom==KS1)nnom=0;  //для стойки 1
    else nnom=1;         //для стойки 2
    if(klo==1) kartina();//если просмотр состояния УВК,дать мнемосхему

    //получить код состояния полукомплектов
    aoar=fr3[nom][0]+fr3[nom][1]*2+fr3[nom][2]*4;

    //если нет изменения - выйти
    if(aoar==old_aoar[nnom])return;

    //при наличии изменений работать по переключателю
    switch(aoar)
    {
      //исправное состояние первый- основной
      case 0: slom_interf(8000+nnom);break;//все исправно основной 1
      case 1: slom_interf(8010+nnom);break;//все исправно основной 2
      case 2: slom_interf(8020+nnom);break;//основной 1 неисправен 1
      case 3: slom_interf(8030+nnom);break;//основной 2 неисправен 1
      case 4: slom_interf(8040+nnom);break;//основной 1 неисправен 2
      case 5: slom_interf(8050+nnom);break;//основной 2 неисправен 2
      case 6: slom_interf(8060+nnom);break;//основной 1 неисправен 1 и 2
      case 7: slom_interf(8070+nnom);break;//основной 2 неисправен 1 и 2
    }
    old_aoar[nnom]=aoar;//запомнить текущее состояние,как старое
  }
	return;
}
//--------------------------------------------------------------------
/*******************************************************\
* Процедура выдачи текста на экран про просмотре файла  *
* состояния УВК oblom#.fix в зависимости от кода отказа *
* для кодов отказа 7000 - 7950                          *
* inde - код отказа прочитанный из файла                *
\*******************************************************/

void fraza2(int inde)
{
#ifdef NALAD
  nom_func("90");
#endif
  switch(inde)
  {
    case 7000: outtext("отказ ТС-1"); break;
    case 7001: outtext("отказ ТС-2"); break;
    case 7010: outtext("отказ ТУ-1"); break;
    case 7011: outtext("отказ ТУ-2"); break;
    case 7020: outtext("запрет ТУ-1"); break;
    case 7021: outtext("запрет ТУ-2"); break;

    case 7050: outtext("восстановление ТС-1"); break;
    case 7051: outtext("восстановление ТС-2"); break;
    case 7060: outtext("восстановление ТУ-1"); break;
    case 7061: outtext("восстановление ТУ-2"); break;
    case 7070: outtext("разрешение ТУ-1"); break;
    case 7071: outtext("разрешение ТУ-2"); break;

    case 7100: outtext("отказ ТС-1 соседней ПЭВМ"); break;
    case 7101: outtext("отказ ТС-2 соседней ПЭВМ"); break;
    case 7110: outtext("отказ ТУ-1 соседней ПЭВМ"); break;
    case 7111: outtext("отказ ТУ-2 соседней ПЭВМ"); break;

    case 7150: outtext("восст.ТС-1 соседней ПЭВМ"); break;
		case 7151: outtext("восст.ТС-2 соседней ПЭВМ"); break;
    case 7160: outtext("восст.ТУ-1 соседней ПЭВМ"); break;
		case 7161: outtext("восст.ТУ-2 соседней ПЭВМ"); break;
    case 7170: outtext("разреш.ТУ-1 соседней ПЭВМ"); break;
    case 7171: outtext("разреш.ТУ-2 соседней ПЭВМ"); break;
    case 7200: outtext("восст. связь ПЭВМ1=ПЭВМ2"); break;
    case 7300: outtext("нет связи ПЭВМ1-ПЭВМ2");break;
    case 7400: outtext("данная ПЭВМ-основная");break;
    case 7500: outtext("данная ПЭВМ-резервная");break;
    case 7600: outtext("не определен статус ПЭВМ "); break;
    case 7700: outtext("выдан запрос в основ.ПЭВМ");break;
    case 7800: outtext("отказано в передаче ПЭВМ");break;
    case 7900: outtext("получен отказ от ПЭВМ");break;
    case 7940: outtext("реконфиг.1-го кан. соседней ПЭВМ");break;
    case 7950: outtext("реконфиг.2-го кан. соседней ПЭВМ");break;
    default:  break;
  }
	return;
}
//---------------------------------------------------------------------
/*******************************************************\
* Процедура выдачи текста на экран про просмотре файла  *
* состояния УВК oblom#.fix в зависимости от кода отказа *
* для кодов отказа 8000 - 8070                          *
* inde - код отказа прочитанный из файла                *
\*******************************************************/
void fraza(int inde)
{
#ifdef NALAD
	nom_func("89");
#endif
	switch(inde)
	{
		case 8000: outtext("1й и 2й исправны,осн.МПСУ1");break;
		case 8010: outtext("1й и 2й исправны,осн.МПСУ2");break;
		case 8020: outtext("отказ МПСУ1, МПСУ1-осн.");break;
		case 8030: outtext("отказ МПСУ1, МПСУ2-осн.");  break;
		case 8040: outtext("отказ МПСУ2, МПСУ1-осн");break;
		case 8050: outtext("отказ МПСУ2, МПСУ2-осн.");break;
		case 8060: outtext("отказ МПСУ1 и МПСУ2,МПСУ1-осн.");break;
		case 8070: outtext("отказ МПСУ1 и МПСУ2,МПСУ2-осн."); break;
	}
	return;
}
//-------------------------------------------------------
void read_sode(int HNDL,int numbe)
{
	unsigned char STr[17],bukv,kotor[2];
	int j;
#ifdef NALAD
	nom_func("274");
#endif
	kotor[1]=0;
	if((numbe>=4000)&&(numbe<4010))//объединение групп
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел
		for(plo=0;plo<=14;plo++)
		{
			read(HNDL,&mlu[plo],1);
			if(mlu[plo]==13)break;
		}
	}
	if((numbe>=4010)&&(numbe<4020))//обрыв групп
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел
		for(plu=0;plu<=14;plu++)
		{
			read(HNDL,&mlv[plu],1);
			if(mlv[plu]==13)break;
		}
	}
	if((numbe>=4020)&&(numbe<4030))//потеря 0 201.1
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел

		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mly[plu],1);
			if(mly[plu]==13)break;
		}
	}
	if((numbe>=4030)&&(numbe<4040))//потеря 0 201.2
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел

		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mlz[plu],1);
			if(mlz[plu]==13)break;
		}
	}
	if((numbe>=4040)&&(numbe<4050))//потеря 0 201.3
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел

		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mlc[plu],1);
			if(mlc[plu]==13)break;
		}
	}
	if((numbe>=4050)&&(numbe<4060))//потеря 0 201.4
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел

		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mls[plu],1);
			if(mls[plu]==13)break;
		}
	}
	if((numbe>=4060)&&(numbe<4080))
	{
		for(j=0;j<16;j++)
		{
			read(HNDL,&bukv,1);
			STr[j]=bukv;
			if(((STr[j]==32)&&(j>1))||(STr[j]==13))break;
		}
		if(j==10)kot=0;
		else {STr[j]=0;kot=atoi(STr);}
	}
	if((numbe>=4120)&&(numbe<4130))//потеря 1 М201.1
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел

		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mly[plu],1);
			if(mly[plu]==13)break;
		}
	}
	if((numbe>=4130)&&(numbe<4140))//потеря 1 М201.2
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел

		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mlz[plu],1);
			if(mlz[plu]==13)break;
		}
	}
	if((numbe>=4140)&&(numbe<4150))//потеря 1 М201.3
	{
		read(HNDL,&kotor[0],1); //номер полукомплекта
		read(HNDL,&bukv,1); //пропустить пробел
		for(plu=0;plu<=15;plu++)
		{
			read(HNDL,&mlc[plu],1);
			if(mlc[plu]==13)break;
		}
	}
	kot=atoi(kotor);
	return;
}
//-------------------------------------------------------------------
