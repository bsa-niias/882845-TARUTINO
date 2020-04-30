#include"bla.h"
/***************************************/
#ifndef NEW_KNOPKA
void uksps(int nom,int bitik)//nom-номер объекта bitik-номер бита
{
	int ij,
	ob1_1=0,//первый датчик УКСПС 1-го перегона
	bt1_1=0,//бит первого датчика УКСПС 1-го перегона
	ob1_2=0,//второй датчик УКСПС 1-го перегона
	bt1_2=0,//бит второго датчика УКСПС 1-го перегона
	kz1=0, //датчик контроля линии 1-го перегона
	bt_kz1=0,//бит датчика контроля линии 1-го перегона
	otkl1=0,//отключатель УКСПС 1-го перегона
	bt_otkl1=0,//бит отключателя УКСПС 1-го перегона
	isk1=0,//исключатель УКСПС 1-го перегона
	bt_isk1=0, //бит исключателя УКСПС 1-го перегона
	ob2_1=0,//первый датчик УКСПС 2-го перегона
	bt2_1=0,//бит первого датчика УКСПС 2-го перегона
	ob2_2=0,//второй датчик УКСПС 2-го перегона
	bt2_2=0,//бит второго датчика УКСПС 2-го перегона
	kz2=0, //датчик контроля линии 2-го перегона
	bt_kz2=0,//бит датчика контроля линии 2-го перегона
	otkl2=0,//отключатель УКСПС 2-го перегона
	bt_otkl2=0,//бит отключателя УКСПС 2-го перегона
	isk2=0,//исключатель УКСПС 2-го перегона
	bt_isk2=0, //бит исключателя УКСПС 2-го перегона
	xp,//координата X для 1-го датчика УКСПС
	yp,//координата Y для 2-го датчика УКСПС
	dx,//смещение по X для 2-го датчика УКСПС относительно 1-го
	hod=0,cod=0,cod1=0,cod2=0,
	ob,bit1,bit2,kz=0,bt_kz,isk=0,bt_isk,otkl=0,bt_otkl,
	cv_da1,cv_da2,cv_kn,cv_lam,mark=0,otk_uksps=0,bt_otk;
	char kod[3];
	nom_func("383");
	if(fr1[nom][1]==28)//если получены данные по датчикам УКСПС
	{
		if(fr1[nom][2]!=9999)nom=fr1[nom][2]&0xfff;//если есть управление УКСПС
		else goto met0;//если нет управления, то перейти
	}
	 //объект управления УКСПС данного перегона
	if(fr1[nom][1]==27)//если получены данные по объекту управления
	{
		mark=fr1[nom][3]&0xfff; //номер объекта управления в markery
		ob1_1=fr1[nom][4]&0xfff;//это УКСПС_1 1-го перегона
		bt1_1=(fr1[nom][4]&0xf000)>>12;//это бит УКСПС_1 1-го перегона
		if(fr1[nom][5]!=9999)//если есть второй датчик
		{
			ob1_2=fr1[nom][5]&0xfff;//это УКСПС_2 1-го перегона
			bt1_2=(fr1[nom][5]&0xf000)>>12; //это бит УКСПС_2 1-го перегона
		}
		else ob1_2=9999;
		if(fr1[nom][7]!=9999)//если в объекте участвует второй перегон
		{
			ob2_1=fr1[nom][7]&0xfff;//это УКСПС_1 2-го перегона
			bt2_1=(fr1[nom][7]&0xf000)>>12; //это бит УКСПС_1 2-го перегона
			if(fr1[nom][8]!=9999)
			{
				ob2_2=fr1[nom][8]&0xfff;//это УКСПС_2 2-го перегона
				bt2_2=(fr1[nom][8]&0xf000)>>12;//это бит УКСПС_2 2-го перегона
			}
			else ob2_2=9999;
		}
	}
met0:
	if(ob1_1==0)//если нет объекта управления
	{
		ob1_1=fr1[nom][6]&0xfff;//первый датчик этого перегона
		bt1_1=(fr1[nom][6]&0xf000)>>12;//бит первого датчика УКСПС этого перегона
		if(fr1[nom][7]!=9999)//если есть второй датчик для этого перегона
		{
			ob1_2=fr1[nom][7]&0xfff;//второй датчик этого перегона
			bt1_2=(fr1[nom][7]&0xf000)>>12;//бит второго датчика УКСПС этого перегона
		}
		else ob1_2=9999;
	}
	if(fr1[ob1_1][9]!=9999)//если есть контроль линии первого перегона
	{
		kz1=fr1[ob1_1][9]&0xfff;//это датчик контроля линии 1-го перегона
		bt_kz1=(fr1[ob1_1][9]&0xf000)>>12;//это бит контроля линии 1-го перегона
	}
	if(fr1[ob1_1][10]!=9999)//если есть объект исключения первого перегона
	{
		isk1=fr1[ob1_1][10]&0xfff;//это датчик контроля линии 1-го перегона
		bt_isk1=(fr1[ob1_1][10]&0xf000)>>12;//это бит контроля линии 1-го перегона
	}
	if(ob2_1!=0)//если есть второй перегон для объекта управления
	{
		if(fr1[ob2_1][9]!=9999)//если есть контроль линии 2-го перегона
		{
			kz2=fr1[ob2_1][9]&0xfff;//это датчик контроля линии 2-го перегона
			bt_kz2=(fr1[ob2_1][9]&0xf000)>>12;//это бит контроля линии 2-го перегона
		}
		if(fr1[ob2_1][10]!=9999)//если есть объект исключения 2-го перегона
		{
			isk2=fr1[ob2_1][10]&0xfff;//это датчик контроля линии 2-го перегона
			bt_isk2=(fr1[nom][9]&0xf000)>>12;//это бит контроля линии 2-го перегона
		}
	}
	if(fr1[ob1_1][2]!=9999)//если есть управление
	{
			otkl1=fr1[ob1_1][2]&0xfff;
			bt_otkl1=(fr1[ob1_1][2]&0xf000)>>12;
	}
	if((ob2_1!=0)&&(fr1[ob2_1][2]!=9999))
	{
			otkl2=fr1[ob2_1][2]&0xfff;
			bt_otkl2=(fr1[ob2_1][2]&0xf000)>>12;
	}
	//uksps1*4+uksps2*2+isk
	if(ob1_2!=9999)
	cod1=fr3[ob1_1][bt1_1]*4+fr3[ob1_2][bt1_2]*2+fr3[isk1][bt_isk1];//1 перегон
	else
		cod1=fr3[ob1_1][bt1_1]*4+fr3[isk1][bt_isk1];//1 перегон

	if(ob2_2!=9999)
		cod2=fr3[ob2_1][bt2_1]*4+fr3[ob2_2][bt2_2]*2+fr3[isk2][bt_isk2];//2 перегон
	else
		cod2=fr3[ob2_1][bt2_1]*4+fr3[isk2][bt_isk2];//2 перегон
#ifdef AVTOD
	if(cod1>1)//если сработал УКСПС первого перегона
	{
		if(((OBRAT==1)&&(fr1[ob1_1][11]==0))||//если четн.прием и УКСПС слева
		((OBRAT==-1)&&(fr1[ob1_1][11]==1)))//или если четн.прием и УКСПС справа
		{
			if(cha==1)//если установлено четное автодействие
			{
				zvuk_vkl(0,0);
				w(169,999,"УКСПС ЧЕТ.АВТОД.СНЯТО!");
				snyato_chet();
			}
		}
		if(((OBRAT==-1)&&(fr1[ob1_1][11]==0))||//если нечетный прием и УКСПС слева
		((OBRAT==1)&&(fr1[ob1_1][11]==1)))//если нечетный прием и УКСПС справа
		{
			if(na==1)
			{
				zvuk_vkl(0,0);
				w(169,999,"УКСПС НЕЧЕТ.АВТОД.СНЯТО!");
				snyato_nechet();
			}
		}
	}
	#endif
	/*    for(ij=0;ij<3;ij++)cod[ij]=0;//$$отладка
	setcolor(7);
	outtextxy(600,10,"████");
	itoa(cod_nosk_chosk,kod,10);
	setcolor(8);
	outtextxy(600,10,kod);*/
met1:
	if(hod==0)
	{
		cod=cod1;
		ob=ob1_1;bit1=bt1_1;bit2=bt1_2;
		kz=kz1;bt_kz=bt_kz1;
		isk=isk1;bt_isk=bt_isk1;
		otkl=otkl1;
		bt_otkl=bt_otkl1;
	}
	else
	{
		cod=cod2;
		ob=ob2_1;bit1=bt2_1;bit2=bt2_2;
		kz=kz2;bt_kz=bt_kz2;
		isk=isk2;bt_isk=bt_isk2;
		otkl=otkl2;
		bt_otkl=bt_otkl2;
	}
	switch(cod)
	{
		case 0: cv_da1= 8;cv_da2= 8;goto ris;
		case 1: cv_da1= 8;cv_da2= 8;goto ris;
		case 4: cv_da1=12;cv_da2= 8;
						if(hod==0){otk_uksps=ob1_1;bt_otk=bt1_1;}
						else {otk_uksps=ob2_1;bt_otk=bt2_1;}
						goto ris;
		case 5: cv_da1=14;cv_da2= 8;goto ris;
		default: break;
	}
	if((fr1[ob][12]&0xff00)==0xff00)//если индикация участков зависимая
	{
		switch(cod)
		{
			case 2: cv_da1=7;cv_da2=12;
							if(hod==0){otk_uksps=ob1_2;bt_otk=bt1_2;}
							else {otk_uksps=ob2_2;bt_otk=bt2_2;}
							goto ris;
			case 3: cv_da1=7;cv_da2=14;break;
			case 6: cv_da1=7;cv_da2=12;
							otk_uksps=fr1[ob][7]&0xfff;
							bt_otk=(fr1[ob][7]&0xf000)>>12;
							break;
			case 7: cv_da1=7;cv_da2=14;break;
		}
		goto ris;
	}
	else //если индикация участков независимая
	{
		switch(cod)
		{
			case 2: cv_da1=8;cv_da2=12;
							if(hod==0){otk_uksps=ob1_2;bt_otk=bt1_2;}
							else {otk_uksps=ob2_2;bt_otk=bt2_2;}
							goto ris;
			case 3: cv_da1=8;cv_da2=14;break;
			case 6: cv_da1=12;cv_da2=12;
							otk_uksps=fr1[ob][7]&0xfff;
							bt_otk=bitik;
//							(fr1[ob][7]&0xf000)>>12;
							break;
			case 7: cv_da1=14;cv_da2=14;break;
		}
		goto ris;
	}
ris:
	if(otk_uksps==9999)
	{ if(hod==0){bit1=bt1_1;bit2=bt1_2;}
		else {bit1=bt2_1;bit2=bt2_2;}
		if(tst_fix(ob,bit1))
		{
			if(get_str(ob,bit1)==0) neisprav_na_disk('@',chudo);
			w(169,999,chudo);
			zvuk_vkl(0,0);
			home(modi);
			fix(ob,bit1);
		}
		if(tst_fix(ob,bit2))
		{
			if(get_str(ob,bit2)==0)neisprav_na_disk('@',chudo);
			w(169,999,chudo);
			zvuk_vkl(0,0);
			home(modi);
			fix(ob,bit2);
		}
	}
	else
	if(otk_uksps>0)
	{
		if(tst_fix(otk_uksps,bt_otk))
		{
 			if(get_str(otk_uksps,bt_otk)==0) neisprav_na_disk('@',chudo);
			w(fr1[nom][10+bt_otk],999,"");
			zvuk_vkl(0,0);
			home(modi);
			fix(otk_uksps,bt_otk);
		}
	}
	if(kz!=0)//если есть контроль линии
	{

		if(fr3[kz][bt_kz]==0)cv_lam=0;
		else cv_lam=12;
	}
	else cv_lam=0;
	if((klo==1)||(help>0)||(nikuda==1))return;
	if(fr3[ob][5]==1){cv_da1=LIGHTCYAN;cv_da2=LIGHTCYAN;}
	if((kz!=0)&&(fr3[kz][5]==1))cv_lam=LIGHTCYAN;
	setfillstyle(1,cv_da1);
	if(hod==0)
	{
		yp=fr1[ob1_1][4];// координата Y датчика 1
		xp=fr1[ob1_1][3];// координата X датчика 1
	}
	else
	{
		yp=fr1[ob2_1][4];// координата Y датчика 1
		xp=fr1[ob2_1][3];// координата X датчика 1
	}
	bar(xp,yp,xp+5,yp+5);
	if(fr1[ob1_1][5]!=0)//если есть второй датчик
	{
		if(hod==0)xp=fr1[ob1_1][3]+fr1[ob1_1][5];
		else xp=fr1[ob2_1][3]+fr1[ob2_1][5];
		setfillstyle(SOLID_FILL,cv_da2);
		bar(xp,yp,xp+5,yp+5);
	}
	if(mark!=0)
	{
		if(fr3[otkl][bt_otkl]==0)cv_kn=8;
		else
			if(fr3[otkl][bt_otkl]==1)cv_kn=14;
			else cv_kn=5;
		if(fr3[otkl][5]==1)cv_kn=11;
		setfillstyle(SOLID_FILL,cv_kn);
		// координаты кнопки
		xp=markery[mark][4];
		yp=markery[mark][5];
		bar(xp-4,yp-4,xp+4,yp+4);
		if(fr3[otkl][0]==1)
		{
			if(cv_kn!=11)
			{
				setfillstyle(1,14);
				bar(xp-4,yp-4,xp+4,yp);
				if(oppp==2)w(12,999,"");
			}
		}
		setlinestyle(0,0,0);
		if(cv_lam!=0)
		{

			if(tst_fix(kz,bt_kz))
			{
				if(get_str(kz,bt_kz)==0)
				{
					w(fr1[nom][10+bt_kz],999,"");
					zvuk_vkl(1,18);
					neisprav_na_disk('@',chudo);
				}
				fix(kz,bt_kz);
			}
			setcolor(cv_lam);
			for(ij=3;ij>=0;ij--)circle(xp,yp,ij);
		}
		else fix(kz,bt_kz);
		if((hod==0)&&(ob2_1!=0))
		{
			hod=2;
			goto met1;
		}
		else return;
	}
end:
	//если нет управления УКСПС
	if(fr3[nom][0]==1)cv_lam=12;
	if(fr3[nom][0]==0)cv_lam=8;
	if(fr3[nom][5]==1)cv_lam=11;
	setfillstyle(SOLID_FILL,cv_lam);
	if(cv_lam!=8)
	if(get_str(nom,0)==0)neisprav_na_disk('@',chudo);
	yp=fr1[nom][4];// координата Y датчика
	xp=fr1[nom][3];// координата X датчика
	bar(xp,yp,xp+5,yp+5);
	return;
}
#else
void uksps(int nom)//nom-номер объекта
{
	unsigned int maska,
	ij,
	jk,
	tip,
	D, //длина текстовой строки в пикселах
	//-----------------
	//первый перегон
	ob1=0,//первый датчик УКСПС (дальний)
	bt1=0,//бит первого датчика УКСПС

	ob2=0,//второй датчик УКСПС (ближний)
	bt2=0,//бит второго датчика УКСПС

	otkl=0,//отключатель УКСПС (принимает команды)
	btotkl=0,//бит отключателя УКСПС

	isk=0,//исключатель УКСПС
	btisk=0, //бит исключателя УКСПС
	//-------------------
	ob_svaz[5], //связанные объекты

	xp1,//координата X для датчика УКСПС-1
	yp1,//координата Y для датчика УКСПС-1

	xp2,//координата X для датчика УКСПС-2
	yp2,//координата Y для датчика УКСПС-2

	xkn, //координата X для кнопки отключения УКСПС
	ykn, //координата Y для кнопки отключения УКСПС
	str_kn, //текстовая строка с именем кнопки

	cvda1, //цвет первого (дальнего) датчика
	cvda2, //цвет второго (ближнего) датчика
	cvkn;  //цвет кнопки управления
	nom_func("383");
	maska=0x7000;
	for(ij=0;ij<5;ij++)
	{
		tip=fr1[nom][2]&maska;
		tip=tip>>(3*(4-ij));
		switch(tip)
		{
			case 0: break;
			case 1: ob1=nom;bt1=ij;break;
			case 2: ob2=nom;bt2=ij;break;
			case 3: otkl=nom;btotkl=ij;break;
			case 4:	isk=nom;btisk=ij;break;
			case 5: break;
			case 6: otkl=isk=nom;btotkl=btisk=ij;break;
			case 7: ob1=nom|0x8000;bt1=ij|0x8000;break;
		}
		if(fr1[nom][ij+8]!=9999)ob_svaz[ij]=fr1[nom][ij+8];
		else ob_svaz[ij]=0;
		maska=maska>>3;
	}
	for(jk=0;jk<5;jk++)
	{
		if(ob_svaz[jk]!=0)
		{
			nom=ob_svaz[jk];
			maska=0x7000;
			for(ij=0;ij<5;ij++)
			{
				tip=fr1[nom][2]&maska;
				tip=tip>>(3*(4-ij));
				switch(tip)
				{
					case 0: break;
					case 1: ob1=nom;bt1=ij;break;
					case 2: ob2=nom;bt2=ij;break;
					case 3: otkl=nom;btotkl=ij;break;
					case 4:	isk=nom;btisk=ij;break;
					case 5: break;
					case 6: otkl=isk=nom;btotkl=btisk=ij;break;
					case 7: ob1=nom|0x8000;bt1=ij|0x8000;break;
				}
				maska=maska>>3;
			}
		}
	}
	if(ob1!=0)
	{
		xp1=fr2[ob1&0xfff][2*(bt1&0xfff)];
		yp1=fr2[ob1&0xfff][2*(bt1&0xfff)+1];
	}
	if(ob2!=0)
	{
		xp2=fr2[ob2][2*bt2];
		yp2=fr2[ob2][2*bt2+1];
	}
/*
	if(kz!=0)
	{
		xlmp=fr2[kz][btkz*2];
		ylmp=fr2[kz][btkz*2+1];
		str_lamp=(fr1[kz][btkz+3]&0xff00)>>8;
	}
*/
	if(otkl!=0)
	{
		if(fr3[otkl][5]==1)cvkn=11;
		else
			if(fr3[otkl][btotkl]==1)cvkn=14;
			else cvkn=7;
		xkn=fr2[otkl][btotkl*2];
		ykn=fr2[otkl][btotkl*2+1];
		str_kn=fr1[otkl][btotkl+3]&0xff;
		D=textwidth(name_lamp[str_kn]);
		setfillstyle(1,cvkn);
		if((getpixel(xkn+1,ykn+1)!=cvkn)||(cikl_obnov==0))
		{
			bar(xkn,ykn,xkn+D+1,ykn+10);
			setcolor(8);
			setlinestyle(0,0,0);
			outtextxy(xkn+1,ykn+2,name_lamp[str_kn]);
			rectangle(xkn,ykn,xkn+D+1,ykn+10);
		}
		else
		{
			obnovi_obraz(xkn+1,ykn,D+1,name_lamp[str_kn],cvkn);
		}
	}
#ifdef AVTOD
	if((fr3[ob1][bt1]!=0)||(fr3[ob2][bt2]!=0))//если сработал любой УКСПС
	{
		if(((OBRAT==1)&&(xp1<320))||//если четн.прием и УКСПС слева
		((OBRAT==-1)&&(xp1>320)))//или если четн.прием и УКСПС справа
		{
			if(cha==1)//если установлено четное автодействие
			{
				zvuk_vkl(0,0);
				w(169,999,"УКСПС ЧЕТ.АВТОД.СНЯТО!");
				snyato_chet();
			}
		}
		if(((OBRAT==-1)&&(xp1<320))||//если нечетный прием и УКСПС слева
		((OBRAT==1)&&(xp1>320)))//если нечетный прием и УКСПС справа
		{
			if(na==1)
			{
				zvuk_vkl(0,0);
				w(169,999,"УКСПС НЕЧЕТ.АВТОД.СНЯТО!");
				snyato_nechet();
			}
		}
	}
	#endif
	if(fr3[ob1][bt1]==1)//если первый УКСПС сработал
	{
		if(tst_fix(ob1,bt1))//если ранее не фиксировался
		{
			if(get_str(ob1,bt1)==0) neisprav_na_disk('@',chudo);
			w(169,999,chudo);
			zvuk_vkl(0,0);
			home(modi);
			fix(ob1,bt1);
		}
	}
	if(fr3[ob2][bt2]==1)//если второй УКСПС сработал
	{
		if(tst_fix(ob2,bt2))
		{
			if(get_str(ob2,bt2)==0)neisprav_na_disk('@',chudo);
			w(283,999,chudo);
			zvuk_vkl(0,0);
			home(modi);
			fix(ob2,bt2);
		}
	}
	if(ob1<0xfff)//если индикация независимая
	{
		if(fr3[ob1][5]==1)cvda1=11; //если объект непарафазный
		else
		if(fr3[ob1][bt1]==1) //если сработал УКСПС
		{
			if(isk!=0)//если есть объект исключенияя
			{
				if(fr3[isk][btisk]==0)cvda1=12; //если не отключен УКСПС
				else cvda1=14;
			}
			else cvda1=12;
		}
		else cvda1=8;
		if(fr3[ob2][5]==1)cvda2=11; //если объект непарафазный
		else
		if(fr3[ob2][bt2]==1) //если сработал УКСПС
		{
			if(isk!=0)//если есть объект исключенияя
			{
				if(fr3[isk][btisk]==0)cvda2=12; //если не отключен УКСПС
				else cvda2=14;
			}
			else cvda2=12;
		}
		else cvda2=8;

	}
	else //если индикация зависимая
	{

	}
	if(ob1!=0)
	{
		setfillstyle(1,cvda1);
		bar(xp1,yp1,xp1+4,yp1+4);
		if(cvda1==7)setcolor(8);
		else setcolor(cvda1);
		setlinestyle(0,0,0);
		rectangle(xp1,yp1,xp1+4,yp1+4);
	}
	if(ob2!=0)
	{
		setfillstyle(1,cvda2);
		bar(xp2,yp2,xp2+4,yp2+4);
		if(cvda2==7)setcolor(8);
		else setcolor(cvda2);
		setlinestyle(0,0,0);
		rectangle(xp2,yp2,xp2+4,yp2+4);
	}
	return;
}

#endif
//-----------------------------------------------------------------
/*********************************************************\
* изображение концентич.лампы для объекта nom и бита bit_ *
\*********************************************************/
void lampa_konc(int nom,int bit_)
{
  int goj,rad_min,rad_max,ZZ,CC,X,Y;
  unsigned char objkt_z;
	nom_func("141");
  if((klo==1)||(help!=0)||(nikuda==1))return;
  if(fr1[nom][bit_+5]==9999)return;
  X=fr1[nom][3];
  Y=fr1[nom][4];
  ZZ=(fr1[nom][2]&0xFF00)>>8;
  CC=fr1[nom][2]&0xff;
  //если бит установлени и нет переключения экранов
  if((fr3[nom][bit_]==1)&&(perekluch==0))
  { objkt_z=zvu_tabl[ZZ][bit_+4];//взять признак записи на диск
    if(objkt_z!=0)//если требуется запись
    { //если найдено название объекта и не было фиксации
      if((get_str(nom,bit_)==0)&&(tst_fix(nom,bit_)))
      {
        neisprav_na_disk(objkt_z,chudo);//записать в требуемую графу
        if(objkt_z!='$')w(169,999,chudo);//если не включение, а неисправность
      }
    }
  }
  fix(nom,bit_);//установить флаг фиксации состояния
  if(fr3[nom][bit_]==1)
      setcolor(cvt[CC][2*bit_+1]);
    else  setcolor(cvt[CC][2*bit_]);
  //если бит установлен и требуется звук
  if((fr3[nom][bit_]==1)&&(zvu_tabl[ZZ][bit_]!=0))
  {
    //если нет переключения экранов, то на 1 сек включить звук
    if(perekluch==0)zvuk_vkl(1,18);
  }
  setlinestyle(0,0,0);//тонкая линия
  /****************************************************************************/
  if(fr3[nom][5]==1)  setcolor(LIGHTCYAN);//для непарафазности цвет - циан
  rad_min=(fr1[nom][bit_+5]&0xf0)>>4;//взять радиус
  rad_max=fr1[nom][bit_+5]&0xf;//взять радиус
  //для лампочки нарисовать круг
  for(goj=rad_min;goj<rad_max;goj++)circle(X,Y,goj);
  return;
}

//----------------------------------------------------------------
/***********************************************************\
* Процедура отображения, фиксации неисправностей на диске,  *
* вывода предупреждения на экран и выдачи звукового сигнала *
* для датчиков состояния фидеров.                           *
\***********************************************************/
void fider3dat(int nom,int bit_)
{
	int goj,//радиус изображения лампы состояния фидера
	poly[12],
	F1,//цвет лампы первого фидера
	F2,//цвет лампы второго фидера
	cod_fider;//код состояния фидеров
	unsigned char imya[15];
	nom_func("53");
	//если передан не предусмотренный бит - выйти
	if(bit_>=fr1[nom][11])return;
  if((klo==1)||(help!=0)||(nikuda==1))return;
  //1вф*4+2ф*2+1ф - данные формирования кода состояния фидеров
	cod_fider=fr3[nom][2]*4+fr3[nom][1]*2+fr3[nom][0];
  switch(cod_fider)//в зависимости от состояния фидеров назначить цвета
	{
    case 0: F1=10;F2=14;break;
		case 1: F1=12; F2=14;break;
		case 2: F1=10;F2=12; break;
		case 3: F1=12; F2=12; break;
		case 4: F1=14;F2=10;break;
		case 5: F1=12; F2=10;break;
		case 6: F1=14;F2=12; break;
		case 7: F1=12; F2=12; break;
	}

	if((F1==12)||(F2==12))//если для одного из фидеров назначен красный
	{
    //если бит установлен и не фиксирован ранее
    if(tst_fix(nom,bit_)&&fr3[nom][bit_])
    {
      //если найдено название объекта для бита
			if(get_str(nom,bit_)==0)
			{ //запись на диск(раздел устройств питания)
				neisprav_na_disk('%',chudo);
				//вывод на экран текста
				if((F1==12)&&(bit_==0))w(289,999,"");
				if((F2==12)&&(bit_==1))w(293,999,"");

				//включить звук на 1 сек
				zvuk_vkl(1,18l);
			}
		}
	}
	//установить признак фиксации события
	fix(nom,bit_);
	if(fr1[nom][1]==25)
	{
		//если имеется непарафазность - цвет циан
		if(fr3[nom][5]==1)setcolor(11);
		else setcolor(F1); //иначе цвет 1-го фидера установить по коду
		setlinestyle(0,0,0); //назначить тонкую линию


		for(goj=1;goj<fr1[nom][11];goj++) //нарисовать лампу
		circle(fr1[nom][3+2*0],fr1[nom][4+2*0],goj);

		if(fr3[nom][5]==1)
		 setcolor(11); //если непарафазность цвет - "циан"
		else setcolor(F2);

		for(goj=1;goj<fr1[nom][11];goj++)circle(fr1[nom][3+2*1],fr1[nom][4+2*1],goj);
	}
	if(fr1[nom][1]==165)
	{
		if(fr3[nom][5]==1)setfillstyle(1,11);
		else setfillstyle(1,F1); //иначе цвет 1-го фидера установить по коду
		setlinestyle(0,0,0); //назначить тонкую линию
		strcpy(imya,name_lamp[fr1[nom][4]]);
		setcolor(8);
		poly[0]=fr2[nom][0];		//x0
		poly[1]=fr2[nom][1]-1;   //y0
		goj=textwidth(name_lamp[fr1[nom][4]]);
		poly[2]=poly[0]+goj;					//x1
		poly[3]=poly[1];              //y1
		poly[4]=poly[2]+5;            //x2
		poly[5]=poly[3]+5;            //y2
		poly[6]=poly[2];              //x3
		poly[7]=poly[5]+5;            //y3
		poly[8]=poly[0];              //x4
		poly[9]=poly[7];              //y4
		poly[10]=poly[0]-5;           //x5
		poly[11]=poly[5];             //y5
		fillpoly(6,poly);
		outtextxy(poly[0],poly[1]+2,name_lamp[fr1[nom][4]]);
		if(fr3[nom][5]==1)setfillstyle(1,11);
		else setfillstyle(1,F2); //иначе цвет 2-го фидера установить по коду
		setlinestyle(0,0,0); //назначить тонкую линию
		strcpy(imya,name_lamp[fr1[nom][5]]);
		setcolor(8);
		poly[0]=fr2[nom][2];		//x0
		poly[1]=fr2[nom][3]-1;   //y0
		goj=textwidth(name_lamp[fr1[nom][4]]);
		poly[2]=poly[0]+goj;					//x1
		poly[3]=poly[1];              //y1
		poly[4]=poly[2]+5;            //x2
		poly[5]=poly[3]+5;            //y2
		poly[6]=poly[2];              //x3
		poly[7]=poly[5]+5;            //y3
		poly[8]=poly[0];              //x4
		poly[9]=poly[7];              //y4
		poly[10]=poly[0]-5;           //x5
		poly[11]=poly[5];             //y5
		fillpoly(6,poly);
		outtextxy(poly[0],poly[1]+2,name_lamp[fr1[nom][5]]);
	}
	return;
}
/********************************************\
* Процедура изображения на экране текстовой  *
* лампы для объекта nom и бита bit_          *
* одновременно выполняется активация для     *
* датчиков вспомогательного перевода стрелок *
* и контроля состояния макета стрелок        *
\********************************************/
//-----------------------------------------------------------------
void lampa_text(int nom,int bit_)
{
	int x,y,goj,rad,ZZ,CC,n_bait,poly[14],D,cvkn;
	unsigned char objkt_z,imya[10];
	nom_func("450");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(fr1[nom][1]==165)
	{
		fider3dat(nom,bit_);
		return;
	}
	if((klo==1)||(help!=0)||(nikuda==1))return;
	//если для бита не определен текст лампы     
	if(fr1[nom][bit_+3]==0)return;
	ZZ=(fr1[nom][2]&0xFF00)>>8; //взять строку звуков и архивирования отказов
	CC=fr1[nom][2]&0xff;        //взять строку цветов

	//если объект связан с кнопкой аварийного перевода стрелок
	if(((nom==AVAch)&&(bit_==AV_bch))||((nom==AVAn)&&(bit_==AV_bn)))
	{ if(fr3[nom][bit_]==1)//если нажали кнопку аварийного перевода
		if(fr3[nom][5]==1){AV_zdem=0;AVARI=0;}//если объект непарафазен- выход
		else
			if(AVARI==1)//Если выполняется аварийный перевод стрелок
			{ if(fr3[nom][bit_]==1)// если нажата кнопка аварийного перевода
				{
					sbros_time(AVAch,AV_bch);//сбросить таймер для четной кнопки
					sbros_time(AVAn,AV_bn);//сбросить таймер для нечетной кнопки
#if(AVAch!=AVAn)||(AV_bch!=AV_bn)
					if((nom==AVAch)&&(bit_==AV_bch))//если четный
					{
						if(tst_chet(buf_ko_vsp[0])!=0)
						{
							w(268,999,"");//нажата не та кнопка
							zvuk_vkl(0,0);
							goto cont;
						}
					}
					if((nom==AVAn)&&(bit_==AV_bn))//если нечетный
					{
						if(tst_chet(buf_ko_vsp[0])!=1)
						{
							w(268,999,""); //нажата не та кнопка
							zvuk_vkl(0,0);
							goto cont;
						}
					}
#endif
					form_kom_vsp();//сформировать команду вспомогательного перевода
cont:
					AV_zdem=0; AVARI=0;//сбросить признаки ожидания аварийного перевода
					vkl_kno(AViA,8);//экранную кнопку аварийного перевода сбросить
					avap=0;//сбросить индикатор состояния процесса аварийного перевода
				}
				else
				{ AV_zdem=0;AVARI=0;
					vkl_kno(AViA,8);
					for(goj=0;goj<(n_bait-1);goj++)buf_ko_vsp[goj]=0;
					avap=0;
				}
			}
	}
	//если бит установлени и нет переключения экранов
	if((fr3[nom][bit_]==1)&&(perekluch==0))
	{ objkt_z=zvu_tabl[ZZ][bit_+4];//взять признак записи на диск
		if(objkt_z!=0)//если требуется запись
		{ //если найдено название объекта и не было фиксации
			if((get_str(nom,bit_)==0)&&tst_fix(nom,bit_))
			{
				neisprav_na_disk(objkt_z,chudo);//записать в требуемую графу
				if(fr1[nom][bit_+8]!=0)
				{ if(((fr1[nom][bit_+8]>=289)&&(fr1[nom][bit_+8]<=293))||(fr1[nom][bit_+8]==284))
					strcpy(chudo,"");
					w(fr1[nom][bit_+8],999,chudo);//если не включение, а неисправность
				}
			}
		}
	}

	if((klo==1)||(help!=0))return;//если помощь или просмотр состояния УВК
	if(fr1[nom][bit_+3]==0) return;//если нет координаты X для лампы
	//если это не объект для контроля макета или бит не нулевой
	if(((nom!=KM1)&&(nom!=KM2))||(bit_!=0))
	{ //если бит установлен то цвет для лампы взять из cvt
		if(fr3[nom][bit_]==1)
		{
			cvkn=cvt[CC][2*bit_+1];
			setfillstyle(1,cvkn);
			setcolor(cvkn);
		}
		//если бит сброшен-аналогично
		else
		{
			//setfillstyle(1,cvt[CC][2*bit_]);
			setfillstyle(1,0); //$$$$
			cvkn=7;
			//cvt[CC][2*bit_];
			setcolor(cvkn);
		}
	}
	else
	{ //если установлен бит контроля макета в обоих стойках,
		//то выбрать цвет для установленного бита из cvt
		setcolor(8);
		if((fr3[KM1][bit_]==1)&&(fr3[KM2][bit_]==1))cvkn=cvt[CC][2*bit_+1];
		else//аналогично для сброшенных битов КМ
			if((fr3[KM1][bit_]==0)&&(fr3[KM2][bit_]==0))cvkn=cvt[CC][2*bit_];
			else cvkn=13;//если значение КМ в стойках разное-фиолетовый цвет
				setfillstyle(1,cvkn);
	}
	//если бит установлен и требуется звук
	if((fr3[nom][bit_]==1)&&(zvu_tabl[ZZ][bit_]!=0)&&tst_fix(nom,bit_))
	{
		//если нет переключения экранов, то на 1 сек включить звук
		if(perekluch==0)zvuk_vkl(1,18);
	}
	setlinestyle(0,0,0);//тонкая линия
	/****************************************************************************/
	if(fr3[nom][5]!=0)
	{
		cvkn=11;
		setfillstyle(1,cvkn);//для непарафазности цвет - циан
		setcolor(cvkn);
		if(tst_fix(nom,5))slom_interf(nom);
		fix(nom,5);//установить флаг фиксации состояния
	}
	strcpy(imya,name_lamp[fr1[nom][bit_+3]]);
	D=textwidth(name_lamp[fr1[nom][bit_+3]]);
	if(fr1[nom][1]==162)//если простые лампы
	{
		poly[0]=fr2[nom][2*bit_];	   	//x0
		poly[1]=fr2[nom][2*bit_+1];   //y0
		poly[2]=poly[0]+D;						//x1
		poly[3]=poly[1];              //y1
		poly[4]=poly[2]+5;            //x2
		poly[5]=poly[3]+5;            //y2
		poly[6]=poly[2];              //x3
		poly[7]=poly[5]+5;            //y3
		poly[8]=poly[0];              //x4
		poly[9]=poly[7];              //y4
		poly[10]=poly[0]-5;           //x5
		poly[11]=poly[5];             //y5
		x=poly[0];y=poly[1];

		if((getpixel(poly[10]+1,poly[11])!=cvkn)||(cikl_obnov==0))
		{
			setfillstyle(1,cvkn);//для непарафазности цвет - циан
			fillpoly(6,poly);
			setcolor(8);
			outtextxy(x,y+2,name_lamp[fr1[nom][bit_+3]]);
		}
		else
		{
			setcolor(8);
			outtextxy(x,y+2,name_lamp[fr1[nom][bit_+3]]);
			obnovi_obraz1(x,y,D+1,name_lamp[fr1[nom][bit_+3]],cvkn);
		}
	}
	if(fr1[nom][1]==164) //если лампа комплексная
	{
		for(rad=0;rad<5;rad++)if(fr1[nom][rad+3]!=0)break;
		if(bit_==rad)//если первая половина
		{
			poly[0]=fr2[nom][2*bit_];			//x0
			poly[1]=fr2[nom][2*bit_+1]-1; //y0

			poly[2]=poly[0]+D/2;					//x1
			poly[3]=poly[1];              //y1

			poly[4]=poly[2]  ;            //x2
			poly[5]=poly[3]+10;           //y2

			poly[6]=poly[0];              //x3
			poly[7]=poly[5];              //y3

			poly[8]=poly[0]-5;            //x4
			poly[9]=poly[7]-5;            //y4
			setfillstyle(1,cvkn);//для непарафазности цвет - циан
			fillpoly(5,poly);
		}
		else//если вторая половина
		{
			poly[0]=fr2[nom][2*bit_]+D/2;			//x0
			poly[1]=fr2[nom][2*bit_+1]-1; 		//y0

			poly[2]=poly[0]+D/2;							//x1
			poly[3]=poly[1];                  //y1

			poly[4]=poly[2]+5;            //x2
			poly[5]=poly[3]+5;            //y2

			poly[6]=poly[2];              //x3
			poly[7]=poly[5]+5;            //y3

			poly[8]=poly[0];              //x4
			poly[9]=poly[7];              //y4
			setfillstyle(1,cvkn);//для непарафазности цвет - циан
			fillpoly(5,poly);
		}
	}
	setcolor(8);
	poly[12]=poly[0]=fr2[nom][2*bit_];			//x0
	poly[13]=poly[1]=fr2[nom][2*bit_+1];   //y0
	poly[2]=poly[0]+D;						//x1
	poly[3]=poly[1];              //y1
	poly[4]=poly[2]+5;            //x2
	poly[5]=poly[3]+5;            //y2
	poly[6]=poly[2];              //x3
	poly[7]=poly[5]+5;            //y3
	poly[8]=poly[0];              //x4
	poly[9]=poly[7];              //y4
	poly[10]=poly[0]-5;           //x5
	poly[11]=poly[5];             //y5
	drawpoly(7,poly);
	outtextxy(poly[0],poly[1]+2,name_lamp[fr1[nom][bit_+3]]);
	fix(nom,bit_);//установить флаг фиксации состояния
	return;

}

//----------------------------------------------------------------
/********************************************\
* Процедура изображения на экране лампы для  *
* объекта nom и бита bit_                    *
* лампа типа круг или кольцо                 *
* одновременно выполняется активация для     *
* датчиков вспомогательного перевода стрелок *
* и контроля состояния макета стрелок        *
\********************************************/
void lampa(int nom,int bit_)
{
	int goj,rad,ZZ,CC,n_bait;
	unsigned char objkt_z;
	nom_func("141");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if((klo==1)||(help!=0)||(nikuda==1))return;
	//если для бита не определена коорд. X - выход
	if(fr1[nom][2*bit_+3]==9999)return;
	ZZ=(fr1[nom][2]&0xFF00)>>8; //взять строку звуков и архивирования отказов
	CC=fr1[nom][2]&0xff;        //взять строку цветов

	//если объект связан с кнопкой аварийного перевода стрелок
	if(((nom==AVAch)&&(bit_==AV_bch))||((nom==AVAn)&&(bit_==AV_bn)))
	{ if(fr3[nom][bit_]==1)//если нажали кнопку аварийного перевода
		if(fr3[nom][5]==1){AV_zdem=0;AVARI=0;}//если объект непарафазен- выход
		else
			if(AVARI==1)//Если выполняется аварийный перевод стрелок
			{ if(fr3[nom][bit_]==1)// если нажата кнопка аварийного перевода
				{
					sbros_time(AVAch,AV_bch);//сбросить таймер для четной кнопки
					sbros_time(AVAn,AV_bn);//сбросить таймер для нечетной кнопки
#if (AVAch!=AVAn)||(AV_bch!=AV_bn)
					if((nom==AVAch)&&(bit_==AV_bch))//если четный
					{
						if(tst_chet(buf_ko_vsp[0])!=0)
						{
							w(268,999,"");
							zvuk_vkl(0,0);
							goto cont;
						}
					}
					if((nom==AVAn)&&(bit_==AV_bn))//если нечетный
					{
						if(tst_chet(buf_ko_vsp[0])!=1)
						{
							w(268,999,"");
							zvuk_vkl(0,0);
							goto cont;
						}
					}
#endif
					form_kom_vsp();//сформировать команду вспомогательного перевода
cont:
					AV_zdem=0; AVARI=0;//сбросить признаки ожидания аварийного перевода
					vkl_kno(AViA,8);//экранную кнопку аварийного перевода сбросить
					avap=0;//сбросить индикатор состояния процесса аварийного перевода
				}
				else
				{ AV_zdem=0;AVARI=0;
					vkl_kno(AViA,8);
					for(goj=0;goj<(n_bait-1);goj++)buf_ko_vsp[goj]=0;
					avap=0;
				}
			}
	}
	//если бит установлени и нет переключения экранов
	if((fr3[nom][bit_]==1)&&(perekluch==0))
	{ objkt_z=zvu_tabl[ZZ][bit_+5];//взять признак записи на диск
		if(objkt_z!=0)//если требуется запись
		{ //если найдено название объекта и не было фиксации
			if((get_str(nom,bit_)==0)&&tst_fix(nom,bit_))
			{
				neisprav_na_disk(objkt_z,chudo);//записать в требуемую графу
				if(objkt_z=='')w(175,999,chudo);//авария переезда
				else
				if(objkt_z=='')w(298,999,chudo); //неисправность переезда
				else
				if(objkt_z!='$')w(169,999,chudo);//если не включение, а неисправность
			}
		}
	}

	if((klo==1)||(help!=0))return;//если помощь или просмотр состояния УВК
	if(fr1[nom][2*bit_+3]==9999) return;//если нет координаты X для лампы
	//если это не объект для контроля макета или бит не нулевой
	if(((nom!=KM1)&&(nom!=KM2))||(bit_!=0))
	{ //если бит установлен то цвет для лампы взять из cvt
		if(fr3[nom][bit_]==1)setcolor(cvt[CC][2*bit_+1]);
		//если бит сброшен-аналогично
		else  setcolor(cvt[CC][2*bit_]);
	}
	else
	{ //если установлен бит контроля макета в обоих стойках,
		//то выбрать цвет для установленного бита из cvt
		if((fr3[KM1][bit_]==1)&&(fr3[KM2][bit_]==1))setcolor(cvt[CC][2*bit_+1]);
		else//аналогично для сброшенных битов КМ
			if((fr3[KM1][bit_]==0)&&(fr3[KM2][bit_]==0))setcolor(cvt[CC][2*bit_]);
			else setcolor(13);//если значение КМ в стойках разное-фиолетовый цвет
	}
	//если бит установлен и требуется звук
	if((fr3[nom][bit_]==1)&&(zvu_tabl[ZZ][bit_]!=0)&&tst_fix(nom,bit_))
	{
		//если нет переключения экранов, то на 1 сек включить звук
		if(perekluch==0)zvuk_vkl(1,18);
	}
	setlinestyle(0,0,0);//тонкая линия
	/****************************************************************************/
	if(fr3[nom][5]==1)  setcolor(LIGHTCYAN);//для непарафазности цвет - циан
	rad=(fr1[nom][11]&(0xf000>>(bit_*4)))>>((3-bit_)*4);//взять радиус
	//для лампочки нарисовать круг
	if(fr1[nom][1]==22)for(goj=rad-1;goj>=0;goj--)circle(fr1[nom][3+2*bit_],fr1[nom][4+2*bit_],goj);
	//для кольца - нарисовать кольцо
	if(fr1[nom][1]==23)for(goj=rad-1;goj>=4;goj--)circle(fr1[nom][3+2*bit_],fr1[nom][4+2*bit_],goj);
	fix(nom,bit_);//установить флаг фиксации состояния
	return;
}
//----------------------------------------------------------------
/********************************************\
* Процедура изображения на экране лампы для  *
* объекта nom и бита bit_, где bit может <=4 *
* лампа типа круг                            *
* одновременно выполняется активация для     *
\********************************************/
void lampa_5(int nom,int bit_)
{
	int goj,rad,ZZ,CC;
	unsigned char objkt_z;
	nom_func("441");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	//если для бита не определена коорд. X - выход
	if(fr1[nom][2*bit_+3]==9999)return;
	ZZ=(fr1[nom][2]&0xFF00)>>8; //взять строку звуков и архивирования отказов
	CC=fr1[nom][2]&0xff;        //взять строку цветов
	//если объект связан с кнопкой аварийного перевода стрелок
	//если бит установлени и нет переключения экранов
	if((fr3[nom][bit_]==1)&&(perekluch==0))
	{ objkt_z=zvu_tabl[ZZ][bit_+4];//взять признак записи на диск
		if(objkt_z!=0)//если требуется запись
		{ //если найдено название объекта и не было фиксации
			if((get_str(nom,bit_)==0)&&tst_fix(nom,bit_))
			{
				neisprav_na_disk(objkt_z,chudo);//записать в требуемую графу
				if(objkt_z!='$')w(169,999,chudo);//если не включение, а неисправность
			}
		}
	}
	if((klo==1)||(help!=0))return;//если помощь или просмотр состояния УВК
	if(fr1[nom][2*bit_+3]==9999) return;//если нет координаты X для лампы
	//если бит установлен то цвет для лампы взять из cvt
	if(fr3[nom][bit_]==1)setcolor(cvt[CC][2*bit_+1]);
	//если бит сброшен-аналогично
	else  setcolor(cvt[CC][2*bit_]);
	//если бит установлен и требуется звук
	if((fr3[nom][bit_]==1)&&(zvu_tabl[ZZ][bit_]!=0)&&tst_fix(nom,bit_))
	{
		//если нет переключения экранов, то на 1 сек включить звук
		if(perekluch==0)zvuk_vkl(1,18);
	}
	setlinestyle(0,0,0);//тонкая линия
	/****************************************************************************/
	if(fr3[nom][5]==1)  setcolor(LIGHTCYAN);//для непарафазности цвет - циан

	rad=(fr1[nom][3+2*bit_]&0xf000)>>12;//взять радиус
	//для лампочки нарисовать круг
	for(goj=rad-1;goj>=0;goj--)circle(fr1[nom][3+2*bit_]&0xFFF,fr1[nom][4+2*bit_],goj);
	fix(nom,bit_);//установить флаг фиксации состояния
  return;
}
//-----------------------------------------------------------------
//------------------------------------------------------------------------
/*********************************************\
* Процедура изображения на экране цветного    *
* прямоугольника для объекта nom и бита bit_, *
* объект типа 2,177                           *
\*********************************************/
void barcolor(int nom,int bit_)
{
  int rad,ZZ,CC,xx,yy,cvt0,cvt1;
  unsigned char objkt_z;
	nom_func("141");
	if((klo==1)||(help!=0)||(nikuda==1))return;

  //если для бита не определена коорд. X - выйти без рисунка
  if(fr1[nom][2*bit_+3]==9999)return;

	ZZ=(fr1[nom][2]&0xFF00)>>8; //найти тип звуков и фиксации
	CC=fr1[nom][2]&0xff;        //найти вид цветового решения
	cvt0=cvt[CC][2*bit_];
	cvt1=cvt[CC][2*bit_+1];

	//если бит установлени и нет переключения экранов
	if((fr3[nom][bit_]==1)&&(perekluch==0))
	{ //взять признак записи на диск
		objkt_z=zvu_tabl[ZZ][bit_+5];
		if(objkt_z!=0)//если требуется запись
		{ //если найдено название объекта и не было фиксации
			if((get_str(nom,bit_)==0)&&(tst_fix(nom,bit_)))
			{ //записать в требуемую графу
				neisprav_na_disk(objkt_z,chudo);
				//если не включение, а неисправность - то сообщить на экран
				if(objkt_z!='$')w(169,999,chudo);
			}
		}
	}
	fix(nom,bit_);//установить флаг фиксации состояния

	//если бит установлен то цвет взять из cvt
	if(fr3[nom][bit_]==1){setfillstyle(1,cvt1);setcolor(cvt1);}

	 //если бит сброшен-аналогично
	else  {setfillstyle(1,cvt0);setcolor(cvt0);}

	//если бит установлен и требуется звук
	if((fr3[nom][bit_]==1)&&(zvu_tabl[ZZ][bit_]!=0))
	{
		//если нет переключения экранов, то на 1 сек включить звук
		if(perekluch==0)zvuk_vkl(1,18);
	}
	//для непарафазности цвет - циан
	if(fr3[nom][5]==1)setfillstyle(1,11);
	//взять размер
	rad=(fr1[nom][bit_*2+3]&0xf000)>>12;
	if(rad>0)
	{
		xx=fr1[nom][3+2*bit_]&0xfff;
		yy=fr1[nom][4+2*bit_];
		setlinestyle(0,0,0);
		bar(xx,yy-rad,xx+2,yy+rad);
	}
	return;
}
//------------------------------------------------------------------
/*******************************************************\
* Процедура для организации отображения лампы и кнопки  *
* для объектов типа "день-ночь"                         *
\*******************************************************/
void den_noch(int nom)
{
  int ob_nn,bit_nn,ob_dn,bit_dn,ob_dnk,bit_dnk,ob_au,bit_au,
  cod,x,y,cvet_au,cvet_ru;
	nom_func("34");
	if((klo==1)||(help!=0)||(nikuda==1))return;
  setlinestyle(0,0,0);
  ob_dn=fr1[nom][3];bit_nn=fr1[nom][4];
  ob_nn=fr1[nom][5];bit_dn=fr1[nom][6];
  ob_dnk=fr1[nom][7];bit_dnk=fr1[nom][8];
  ob_au=fr1[nom][9];bit_au=fr1[nom][10];
  if(fr3[nom][5]==1)setcolor(LIGHTCYAN);
  cod=fr3[ob_au][bit_au]*8+fr3[ob_dn][bit_dn]*4+fr3[ob_nn][bit_nn]*2+fr3[ob_dnk][bit_dnk];
  COD_DN=cod;
  if(cod!=0)
  setcolor(8);
  switch(cod)
  {
		case 0: cvet_au=8;cvet_ru=8;break;
		case 1: cvet_au=8;cvet_ru=8;break;
		case 2: cvet_au=8;cvet_ru=14;break;
		case 3: cvet_au=8;cvet_ru=14;break;
		case 4: cvet_au=8;cvet_ru=10;break;
		case 5: cvet_au=8;cvet_ru=10;break;
		case 6: cvet_au=8;cvet_ru=5;break;
		case 7: cvet_au=8;cvet_ru=5;break;
		case 8: cvet_au=10;cvet_ru=8;break;
		case 9: cvet_au=10;cvet_ru=8;break;
		case 10: cvet_au=10;cvet_ru=5;break;
		case 11: cvet_au=10;cvet_ru=5;break;
		case 12: cvet_au=10;cvet_ru=5;break;
		case 13: cvet_au=10;cvet_ru=5;break;
		case 14: cvet_au=10;cvet_ru=5;break;
		case 15: cvet_au=10;cvet_ru=5;break;
	}
	if(fr3[ob_au][5]==1)cvet_au=11;
	if(fr3[ob_dn][5]==1)cvet_ru=11;
	if(fr3[ob_nn][5]==1)cvet_ru=11;
	x=markery[fr1[ob_au][12]][4];
	y=markery[fr1[ob_au][12]][5];
	setfillstyle(1,cvet_au);setcolor(cvet_au);
	bar(x-4,y-4,x+4,y+4);//нарисовать кнопку АУ
	x=markery[fr1[ob_dn][12]][4];y=markery[fr1[ob_dn][12]][5];
	setfillstyle(1,cvet_ru);setcolor(cvet_ru);

	bar(x-4,y-4,x+4,y+4);//нарисовать кнопку dn/nn
	if(ob_dnk!=9999)lampa(ob_dnk,bit_dnk);
	return;
}
//-------------------------------------------------------------------------
/**********************************************************\
* Процедура отображения,записи на диск и вывода сообщений  *
* на экран для сложного объекта,состоящего из ряда(до 4-х) *
* ламп,каждая из которых сопряжена с другим объектом.      *
* Состояние которых выводится на одну лампу, цвет которой  *
* определяются состоянием объекта в целом, то есть, при    *
* изменении состояния любой части объекта меняется цвет    *
* лампы (применяется для ПНО)                              *
* nom - номер объекта                                      *
* bit_ - номер бита                                        *
* zps - признак записи на диск                             *
\**********************************************************/
void lampa_3c(int nom,int bit_,int zps)
{
	int goi=0,
	blokirovka=0,
	jjk,
	blok_obj, //блокирующий объект
	blok_bit, //блокирующий бит
	ob_cvt,//строка таблицы cvt
	ob_neispr,//строка таблицы zvu_tab
	ob_par[4],//парныe объект
	b_par[4],//биты парных объектов
	rd[4], //радиусы лампы
	cod[4], //коды состояния ламп
	x[4],//координаты X
	y[4],//координаты Y
	color[4];//цвета ламп

	nom_func("143");
	if(fr3[nom][bit_]!=0)
	goi=0;
	//если помощь,просмотр отказов или потеря обмена - выход
	if((klo==1)||(help!=0)||(nikuda==1))return;

	ob_cvt=fr1[nom][2]&0xff;//взять объект для цвета ламп
	ob_neispr=(fr1[nom][2]&0xff00)>>8;//взять объект для записи ненорм

	//парные объекты
	ob_par[0]=(fr1[nom][11]&0xff00)>>8;
	ob_par[1]=fr1[nom][11]&0xff;
	ob_par[2]=(fr1[nom][12]&0xff00)>>8;
	ob_par[3]=fr1[nom][12]&0xff;

	//определить текущиe коды состояния
	for(goi=0;goi<4;goi++)//пройти по битам
	{
		b_par[goi]=(fr1[nom][goi*2+3]&0xf000)>>12;
		x[goi]=fr1[nom][goi*2+3]&0xfff;
		rd[goi]=(fr1[nom][goi*2+4]&0xf000)>>12;
		y[goi]=fr1[nom][goi*2+4]&0xfff;

		cod[goi]=fr3[nom][goi]*2+fr3[ob_par[goi]][b_par[goi]];
		for(jjk=0;jjk<10;jjk++)
		{
			if((blokir[jjk][0]&0xfff)==nom)
			if(bit_==((blokir[jjk][0]&0xf000)>>12))
			{
				blok_obj=blokir[jjk][1]&0xfff;
				blok_bit=(blokir[jjk][1]&0xf000)>>12;
				if(fr3[blok_obj][blok_bit]==1)
				{
					cod[goi]=0;
					blokirovka=1;
					break;
				}
			}
		}
		switch(cod[goi])
		{ //все выключено
			case 0: color[goi]=cvt[ob_cvt][goi]&0xf;break;
			//включен сигнал - нет ПНО
			case 1: color[goi]=(cvt[ob_cvt][goi]&0xf0)>>4;break;
			//включено ПНО - нет сигнала
			case 2: color[goi]=(cvt[ob_cvt][goi]&0xf00)>>8;break;
			//все включено
			case 3: color[goi]=(cvt[ob_cvt][goi]&0xf000)>>12;break;
			default: break;
		}
	}
	setcolor(color[bit_]);
	setlinestyle(0,0,0);
	for(goi=0;goi<rd[bit_];goi++)circle(x[bit_],y[bit_],goi);
	circle(x[bit_],y[bit_],goi);
	if(color[bit_]==7)
	{setcolor(12);circle(x[bit_],y[bit_],goi);}
	if(zvu_tabl[ob_neispr][cod[bit_]+4]!=0)

	//если не было ранее фиксации и бит установлен
	//если требуется запись и код в допуске
	if((zvu_tabl[ob_neispr][cod[bit_]+4]!=0) && (cod[bit_]<4)&&
	(tst_fix(nom,bit_)|| tst_fix(ob_par[bit_],b_par[bit_])))
	{
		if(zps==0)//если вход не из таймера
		{
			 if(pooo[nom]==0l)TIMER_SOB(nom,21,bit_);
		}
		else//если вход из таймера
		{
			if((bit_==2)&&(fr3[nom][bit_]!=0))
			{
				if(get_str(nom,bit_)==0)neisprav_na_disk('$',chudo);
			}
			if((bit_<2)&&(fr3[nom][bit_]!=0))
			{
				//если найдено имя для бита объекта
				if(get_str(nom,bit_)==0)
				//выполнить запись в требуемую графу
				if(bit_==0)neisprav_na_disk(zvu_tabl[ob_neispr][cod[bit_]+4],chudo);
				else
					if(bit_==1)neisprav_na_disk('',chudo);
				//если требуется звук и вызов функции по новизне данных
				if(zvu_tabl[ob_neispr][cod[bit_]]!=0)
				{
					if((bit_==0)&&(zvu_tabl[ob_neispr][cod[bit_]+4]==''))
						w(175,999,chudo);//вывести сообщение на экран
					else
					if((bit_==1)&&(zvu_tabl[ob_neispr][6]==''))
						w(298,999,chudo);//вывести сообщение на экран
					else
						w(169,999,chudo);
					zvuk_vkl(3,0);//включить звук
				}
			}
			if(blokirovka==0)
			{
				fix(nom,bit_);
				fix(ob_par[bit_],b_par[bit_]);
			}
		}
	}
  else
  {
    if((zvu_tabl[ob_neispr][cod[bit_]+4]==0)&&(pooo[nom]!=0))
    sbros_time(nom,bit_);
		if(blokirovka==0)
		{
			fix(nom,bit_);
			fix(ob_par[bit_],b_par[bit_]);
		}
	}
	setlinestyle(0,0,0);//тонкая линия
	if(fr3[nom][5]==1)
	{
		setcolor(LIGHTCYAN);//для непарафазности - цвет циан
		for(goi=0;goi<3;goi++)circle(x[bit_],y[bit_],goi);
	}
	return;
}
//---------------------------------------------------------------------
/***********************************************\
* Процедура изображения для объекта типа 2,210  *
* (две многоцветные лампы из одного сообщения   *                                   *
\***********************************************/
void lampa_3(int nom,int bit_)
{
  int goj=0,cvet=7,ob_cvt,ob_neispr,ob_2v0,ob_2v1,cod,rad,x,y,b_20,b_21;

	nom_func("142");
	if((klo==1)||(help!=0)||(nikuda==1))return;
  if((fr1[nom][12]&(0xf000>>(bit_*4)))==0)return;
  if((prorisovka==0)&&(ZAGRUZKA==0))prorisovka=0;
  if(fr3[nom][5]==1)cvet=11;
  if(bit_!=5)//если любой бит, кроме бита непарафазности
  {
    ob_2v0=fr1[nom][4+bit_*4]&0xfff;      //объект для 2**0
    b_20=(fr1[nom][4+bit_*4]&0xf000)>>12; //бит для 2**0
    ob_2v1=fr1[nom][5+bit_*4]&0xfff;      //объект для 2**1
    b_21=(fr1[nom][5+bit_*4]&0xf000)>>12; //бит для 2**1
    ob_cvt=(fr1[nom][10+bit_]&0xff00)>>8; //объект таблицы цветов сvt
    ob_neispr=fr1[nom][10+bit_]&0xff;     //объект таблицы неисправностей для диска zvu_tab
    cod=fr3[ob_2v0][b_20]+fr3[ob_2v1][b_21]*2;//код состояния
  }
  else return;
  if(cvet!=11)//если нет непарафазности объекта
  {
    switch(cod)
    {
      case 0: cvet=cvt[ob_cvt][0];break;
      case 1: cvet=cvt[ob_cvt][1];break;
      case 2: cvet=cvt[ob_cvt][2];break;
      case 3: cvet=cvt[ob_cvt][3];break;
    }
  }
  //если требуется запись и код в допуске

  {
    if((zvu_tabl[ob_neispr][cod]!=0)&&(cod<4))
    { //если найдено имя для бита
      if(get_str(nom,bit_)==0)
      neisprav_na_disk(zvu_tabl[ob_neispr][cod+4],chudo);
     //если требуется звук и вызов функции по новизне состояния
      if((zvu_tabl[ob_neispr][cod]!=0)&&(prorisovka==0))
      {
        w(169,999,chudo); //"неиспраность" объекта chudo
        zvuk_vkl(3,0);
      }
    }
  }
rs:
  rad=(fr1[nom][12]&(0xf000>>(4*bit_)))>>(12-bit_*4);
  setlinestyle(0,0,0);
  setcolor(cvet);
  x=fr1[nom][2+bit_*4];y=fr1[nom][3+bit_*4];
	for(goj=rad;goj>=0;goj--)circle(x,y,goj);
  if(cvet==7)//если цвет не определен, то нарисовать окружность
  {
    cvet=8;setcolor(cvet);
		circle(x,y,rad);
  }
  return;
}
/************************************************************/
void tel_vkl_vikl(int iwr)
{
//отображает на экране символ установленного варианта управления
  int ir,cod,colorit,obj_2;
  char cod_asc[3];

	nom_func("334");
	for(ir=0;ir<=2;ir++)cod_asc[ir]=0;
  obj_2=fr1[iwr][5];
  if((fr3[iwr][0]!=fr3[obj_2][0])||
  (fr3[iwr][1]!=fr3[obj_2][1])||
  (fr3[iwr][2]!=fr3[obj_2][2])||
  (fr3[iwr][3]!=fr3[obj_2][3]))
  {
#ifdef IN_VNIIAS
  cod=fr3[iwr][0]*8+ // ВСУ
  fr3[iwr][1]*4+     // ДУ
  fr3[iwr][2]*2+     // РУ
  fr3[iwr][3];      // ОРУ
  if(klo==0)
  {
    itoa(cod,cod_asc,10);
    setcolor(7);
    outtextxy(40,31,"████");
    setcolor(8);
    outtextxy(40,31,cod_asc);
  }
#endif
    colorit=8;
    TELEUP=0;
    DU=0;
    SEZON=0;
    goto prodol;
  }
  cod=(fr3[iwr][0]&fr3[obj_2][0])*8+ // ВСУ 
  (fr3[iwr][1]&fr3[obj_2][1])*4+     // ДУ
  (fr3[iwr][2]&fr3[obj_2][2])*2+     // РУ
	(fr3[iwr][3]&fr3[obj_2][3]);      // ОРУ
#ifdef IMIT
	cod=1;
#endif
	switch(cod)
	{
#ifdef DISP_UPR
		case 1: colorit=14;strcpy(cod_asc,"РУ");
						if(TELEUP==0)
						{
							for(ir=0;ir<=skoko_stoek;ir++)t_com[ir]=biostime(0,0l);
							pooo[KVV1]=biostime(0,0l);
						}
						TELEUP=1;DU=0;SEZON=0;break; //резервное с АРМа
#else
		case 1: colorit=10;strcpy(cod_asc,"ОУ");
						if(TELEUP==0)
						{
							for(ir=0;ir<=skoko_stoek;ir++)
							t_com[ir]=biostime(0,0l);
							pooo[KVV1]=biostime(0,0l);
						}
						TELEUP=1;DU=0;SEZON=0;break; //резервное с АРМа
#endif
		case 2: colorit=12;strcpy(cod_asc,"АУ");TELEUP=0;DU=0;SEZON=0;break; //с пульта
		case 5: colorit=15;strcpy(cod_asc,"ДУ");
						if(TELEUP==0)
						{
							for(ir=0;ir<=skoko_stoek;ir++)
							t_com[ir]=biostime(0,0l);
							pooo[KVV1]=biostime(0,0l);
						}
						TELEUP=1;DU=1;SEZON=0;break; //диспетчерское
		case 9: colorit=10;strcpy(cod_asc,"СУ");
						if(TELEUP==0)
						for(ir=0;ir<=skoko_stoek;ir++)
						t_com[ir]=biostime(0,0l);
						TELEUP=1;DU=0;SEZON=1;break; //сезонное
	 default: colorit=8;TELEUP=0;DU=0;SEZON=0;break; //никакого
	}
prodol:
#ifndef TELEUPR
	TELEUP=1;
#endif
	if(klo==0)
	{
		if(fr3[iwr][5]==1)
		{
			colorit=11;
			if(tst_fix(iwr,5))slom_interf(iwr);
			fix(iwr,5);
		}
		if(help!=0) return;
		if(nikuda==1)return;//если просмотр неисправностей
		setcolor(8);setlinestyle(0,0,3);rectangle(12,30,30,40);
		line(21,40,21,45);line(15,45,27,45);
		setcolor(colorit);
		setfillstyle(1,colorit);bar(13,31,29,39);
		if(colorit!=11){setcolor(8);outtextxy(14,32,cod_asc);}
		if(prorisovka==0)
		{
			if((TELEUP==1)&&(SEZON==0)&&(DU==0))w(231,999,"");
			if(colorit==12)w(192,999,"");
			if(colorit==15)w(191,999,"");
			if((TELEUP==1)&&(SEZON==1)&&(DU==0))w(190,999,"");
      if(colorit==8||colorit==11)w(58,999,"");
#ifdef AVTOD
      if(DU==1)//если установили диспетчерское управление
      {
        if(na==1)//если установлено нечетное автодействие
        {
          snyato_nechet();
        }
        if(cha==1)//если установлено четное автодействие
        {
          snyato_chet();
        }
      }
 #endif
    }
  }
  if(klo==1)tele(iwr);
  return;
}
/*************************************************/
void tele(int iwr)
{
  int SME=0;

	nom_func("335");
	if(nikuda==1||help!=0) return;
  if(fr1[iwr][13]==2) SME=0;
  else SME=140;
  TELEUP=0;
  setcolor(8);
  setfillstyle(SOLID_FILL,7);
  bar(360,25+SME,560,43+SME);
  setfillstyle(CLOSE_DOT_FILL,YELLOW);
  bar(360,25+SME,560,43+SME);
  if(fr1[iwr][5]==1)return;
  moveto(360,25+SME);
  outtext("РУ  - ");
  setcolor(RED);
  if(fr3[iwr][2]==1) outtext("под током");
  else outtext("без тока");
  setcolor(8);
  moveto(360,35+SME);
  outtext("ОРУ - ");
  setcolor(RED);
  if(fr3[iwr][3]==1) outtext("под током");
  else outtext("без тока");
  if((fr3[iwr][1]==1)&&(fr3[iwr][2]==0))
  {
    setfillstyle(CLOSE_DOT_FILL,YELLOW);
    bar(360,45+SME,580,53+SME);
    setcolor(8);
    moveto(360,45+SME);
    outtext("включено дисп.управление");
    TELEUP=0;
   }
   else
   {
    setfillstyle(CLOSE_DOT_FILL,YELLOW);
    bar(360,45+SME,580,53+SME);
    if((fr3[iwr][2]==0)&&(fr3[iwr][3]==1))
    {
      setcolor(8);
      moveto(360,45+SME);
      outtext("включено управление с ПЭВМ");
      TELEUP=1;
    }
    else
    {
      setcolor(8);
      moveto(360,45+SME);
      outtext("управление с ПЭВМ отключено");
      TELEUP=0;
    }
  }
  return;
}
//----------------------------------------------------------------------
/****************************************************\
* Процедура организации обработки данных о состоянии *
* комплектов УВК (запись в архив и отображение)      *
\****************************************************/
void komplekt(int ir,int bit_)
{
  int chislo=0;

	nom_func("129");
	if(bit_==5) goto aa; //если бит непарафазности
  if(bit_>2)

    if((fr3[ir][3]==1)||(fr3[ir][4]==1))//если перезапуск или объединение
    {
			if(DISK==0)kom_v_bufer_pvm(0,OBMEN_PVM);
			chislo=chego_posl(ir); //сформировать код состояния для записи в архив
			if(chislo!=0) slom_interf(chislo);
			if(klo==1)kartina();
		}

	if(bit_<=2)
	{
aa:
		first_second(ir); //записать в архив код состояния комплекта

		if((klo==0)&&(nikuda==0)&&(help==0))
		knopka(ir);//изобразить кнопку переключения
		if(klo==1)kartina();//изобразить мнемосхему состояния УВК
  }
  return;
}
//----------------------------------------------------------------
/*************************************************************\
* Процедура организации изображения мнемосхемы состояния ПЭВМ *
* и каналов связи между ними.                                 *
* nom - код ПЭВМ 0-"своя", 1 - соседняя                       *
* ispr - код состояния 0-исправность, 1-ненорма               *
\*************************************************************/
void ispr_pvm(int nom,int ispr)
{
  char AAq[2]="";
	int colo1=0,x1,y1;

	nom_func("112");

  //если просмотр помощи или ненорм СЦБ -выйти
  if((help!=0)||(nikuda==1))return;

  kvadrat();//изображение статуса ПЭВМ (цветом "подставки")

  if(nom==0)// "своя" ПЭВМ
  {
	  //отображение при просмотре состояния интерфейсов УВК
    if(klo==1)
		{	PVM(nom,ispr);
			kanal(0); //канал свой
			return;
		}

    if(STATUS==1) colo1=LIGHTGREEN;//если основная - цвет зеленый
    else  if(STATUS==0) colo1=YELLOW;//если резервна - цвет желтый
          else colo1=8; //если ШН или другое - цвет серый
	}
  else//"соседняя" ПЭВМ
  {
    if(klo==1)
		{
			PVM(nom,ispr);
			kanal(1);  //канал соседа
			return;
	  }
    if(ispr==0)//если исправна
    { if(STATUS==1) colo1=YELLOW;//если сосед в резерве
      else
        if(STATUS==0) colo1=10;//если сосед основной
         else colo1=8;//если соседа нет
    }
		else colo1=12;
	}

	if(nom==0)//своя ПЭВМ
	{ x1=Xleft; y1=Yleft; setcolor(10); itoa(NOMER_ARMA,AAq,10);
		moveto(x1-12,y1-2); outtext(AAq);
	}
	//соседняя ПЭВМ
	else
	{ x1=Xright; y1=Yright; setcolor(8); moveto(x1+8,y1-2);
		if(NOMER_ARMA==1) outtext("2");
		else outtext("1");
	}
	setfillstyle(SOLID_FILL,colo1);bar(x1-4,y1-4,x1+4,y1+4);
	setcolor(8);setlinestyle(0,0,0);rectangle(x1-4,y1-4,x1+4,y1+4);
	line(x1,y1+4,x1,y1+6);line(x1-2,y1+6,x1+2,y1+6);
	return;
}
//--------------------------------------------------------------------
/**********************************************************\
* Процедура формирования изображения стрелочек канала ДЦ   *
* на мнемосхеме станции (зеленая для основной ПЭВМ, стереть*
* цветом фона для резервной ПЭВМ).                         *
\**********************************************************/
void kanal_DC()
{
  int x1,y1;
  char AAq[2];

	nom_func("115");
	x1=Xleft;y1=Yleft;
  if(STATUS==1)setcolor(10);
  else setcolor(7);
  moveto(x1-30,y1-2);
  AAq[0]=17;AAq[1]=0;outtext(AAq);
  moveto(x1-20,y1-2);AAq[0]=16;AAq[1]=0; outtext(AAq);
  x1=Xright; y1=Yright;
  if(STATUS==0)setcolor(10);
  else setcolor(7);
  moveto(x1+15,y1-2);AAq[0]=17;AAq[1]=0; outtext(AAq);
  moveto(x1+25,y1-2);AAq[0]=16;AAq[1]=0; outtext(AAq);
  return;
}
//--------------------------------------------------------------------
/**********************************************************\
* Процедура формирования изображения стрелочек канала СПДЛП*
* на мнемосхеме станции (зеленая для основной ПЭВМ, стереть*
* цветом фона для резервной ПЭВМ).                         *
\**********************************************************/
void kanal_SPDLP()
{
  int x1,y1;
  char AAq[2];

	nom_func("116");
	x1=Xleft;y1=Yleft;AAq[0]=31;AAq[1]=0;
  if(STATUS==1)setcolor(10);
  else setcolor(14);
  moveto(x1-13,y1+8);outtext(AAq);

  if(NEISPRAVEN==1)setcolor(8);
  else
    if(STATUS==1)setcolor(14);
    else setcolor(10);
  moveto(x1+46,y1+8);
  outtext(AAq);
  return;
}
//-----------------------------------------------------------------------
/***************************************************\
* Процедура изображения двухцветной стилизованной   *
* кнопки переключения комплектов ТУМС в зависимости *
* от состояния полукомплектов МПСУ ТУМС             *
* nom - номер в базе для описания АО/АР             *
\***************************************************/
void knopka(int nom)
{
  int color1=0,color2=0,x1,y1,KSSk=0,kod;

	nom_func("122");
	if((klo==1)||(help!=0)||(nikuda==1))return;
  KSSk=nom;
  kod=fr3[KSSk][0]+fr3[KSSk][1]*2+fr3[KSSk][2]*4;
  switch(kod)
  {
    case 0: color1=10;color2=14;break;
    case 1: color1=14;color2=10;break;
    case 2: color1=12;color2=14;break;
    case 3: color1=4;color2=10;break;
    case 4: color1=10;color2=4;break;
    case 5: color1=14;color2=12;break;
    case 6: color1=12;color2=4;break;
    case 7: color1=4;color2=12;break;
  }
x:
  if(fr3[KSSk][5]==1)  color1=color2=LIGHTCYAN;
  x1=markery[fr1[KSSk][12]][4];
  y1=markery[fr1[KSSk][12]][5];
  setfillstyle(SOLID_FILL,color1);
  bar(x1-8,y1-3,x1,y1+3);
  setfillstyle(SOLID_FILL,color2);
  bar(x1+1,y1-3,x1+9,y1+3);
  setcolor(8);
  return;
}
//--------------------------------------------------------------
/***************************************************\
* Процедура работы с датчиками выходного интерфейса,*
* при ненорме - запись информации в файл oblom#.fix *
\***************************************************/
void dispdatch(int nom,int bit_)
{
  ELE=nom;
	nom_func("38");
  if(bit_==2)
  { if(fr3[nom][bit_]==1)//если ненорма по первому реле данного типа
		{slom_interf(nom);return;}
	}
	if(bit_==3)
	{ if(fr3[nom][bit_]==1)//если ненорма по второму реле данного типа
		{slom_interf(nom);return;}
  }
  return;
}
/********************************************\
*        Процедура прорисовки сигнала        *
* void sigo(int sig,int tip,int obkt,int bt) *
* sig -  объект сигнала                      *
* tip -  тип вызова 0-обычный, 5-из таймера  *
* obkt - объект огневого реле                *
* bt -   обрабатываемый бит сигнала          *
\********************************************/
void sigo(int sig,int tip,int obkt,int bt)
{
	int x1,y1,i,cod_cvt,dug1=180,dug2=330,dug3=30,dug4=180;
	int mid=4,half=6,rad=3,gad=3,og,bit_og;
	int zapret_ris=0,eps=4,eps1=8;
	int cvt_sig,cvt_og,PNO,B_PNO;
	unsigned char dob[10];

	nom_func("305");
	strcpy(dob," огн.");
	if((klo==1)||(help!=0)||(nikuda==1))zapret_ris=1;
  setlinestyle(0,0,0);
  if(fr1[sig][2]!=0){half=half+1;eps=eps+1;eps1=eps1+1;}//для поездного

  x1=fr2[sig][0]; y1=fr2[sig][1];

  og=fr1[sig][7];if(og!=9999)bit_og=fr1[sig][8]; //для огневого реле

  if(fr2[sig][15]!=0)mid=1;
  switch(fr2[sig][2])
  {
    case 1: mid=-mid;rad=-rad;half=-half;
            dug1=0;dug2=150;dug3=210;dug4=360;
            eps=-eps;eps1=-eps1;
            break;
    case 2: half=-half;
            dug1=30;dug2=180;dug3=180;dug4=330;
            eps=-eps;eps1=-eps1;
            break;
		case 3: mid=-mid;rad=-rad;
						dug1=210;dug2=360;dug3=0;dug4=150;
						break;
		default: break;
	}
	if((nikuda==1)||(klo==1)||(help!=0))return;
	switch(tip)
	{
		//если рисуется сигнал
		case 0: ;
		case 5: if(parafaz(sig)<0)
						{cvt_sig=11;cvt_og=7;goto ris1;}
						cod_cvt=fr3[sig][1]*2+fr3[sig][0];
						switch(cod_cvt)
						{ //если сигнал закрыт
							case 0: if(fr1[sig][3]==7)cvt_sig=12;
											else cvt_sig=8;
											if((fr1[sig][10]>kol_OSN)&&(fr1[sig][10]!=9999))
											{
												PNO=fr1[sig][10]&0xfff;
												B_PNO=(fr1[sig][10]&0xf000)>>12;
												lampa_3c(PNO,B_PNO,0);// для ПНО/ПЧО
											}
											if(tst_fix(sig,0)||tst_fix(sig,1))pooo[sig]=0;
											fix(sig,0);
											if(fr1[sig][10]==9999)fix(sig,1);//если нет ПНО
											//если огневых не
											if(og==9999){cvt_og=cvt_sig; goto ris;}
											if(parafaz(og)<0){cvt_og=11;break;}
											if(fr3[og][bit_og]==1)//если огневое не в норме
											{ if(tst_fix(og,bit_og))
												{
													if(tip==5)//если таймер
													{ fix(og,bit_og);
														//огневое в сигнале-установить номер бита=0 для
														//нахождения имени сигнала
														if(og==fr1[og][7])bit_og=0;
														if(get_str(og,bit_og)==0)
														{ w(280,999,chudo);
															chudo[strlen(chudo)]=0;
															strcat(chudo,dob);
															neisprav_na_disk('@',chudo);
															zvuk_vkl(3,0);
															cvt_sig=7;cvt_og=12;goto ris;
														}
													}
													if(pooo[og]==0l)TIMER_SOB(og,18,bit_og);
													pooo_com[og]=0;
													cvt_og=cvt_sig;
												}
												else {cvt_sig=7;cvt_og=12;goto ris;}
											}
											else//если огневое в норме
											{
												if(tst_fix(og,bit_og))
												if(pooo[og]!=0l)sbros_time(og,bit_og);
												cvt_og=cvt_sig;
												fix(og,bit_og);
											}
											break;
							//если маневровый сигнал открыт
							case 1: cvt_sig=15;cvt_og=15;
											if(tst_fix(sig,0)||tst_fix(sig,1))pooo[sig]=0;
											if(pooo[og]!=0l)sbros_time(og,bit_og);
											fix(sig,0);
											if(fr1[sig][10]==9999)fix(sig,1);//если нет ПНО
											break;
							 //если поездной сигнал открыт
							case 2: cvt_sig=10;cvt_og=10;
											if(pooo[og]!=0l)sbros_time(og,bit_og);
											if((fr1[sig][10]>kol_OSN)&&(fr1[sig][10]!=9999))
											{
												PNO=fr1[sig][10]&0xfff;
												B_PNO=(fr1[sig][10]&0xf000)>>12;
												lampa_3c(PNO,B_PNO,0);// для ПНО/ПЧО
											}
											break;
							case 3: cvt_sig=13;cvt_og=13;
											if(tst_fix(og,bit_og))
											{
												w(88,sig,"");
												zvuk_vkl(1,18);
												fix(sig,0);fix(sig,1);
												if(pooo[og]!=0l)sbros_time(og,bit_og);

										 }
										 break;
							default: return;
						}
ris:        cod_cvt=fr3[sig][3]*2+fr3[sig][2];
						switch(cod_cvt)
						{
							case 0: break;
							case 1: break;
							case 2: break;
							case 3: if(tst_fix(sig,2)||tst_fix(sig,3))
											{
												w(88,sig,"");
												zvuk_vkl(1,18);
												fix(sig,2);fix(sig,3);
												if(pooo[og]!=0l)sbros_time(og,bit_og);

											}
											break;
						default: return;
					 }
					 if(tst_fix(sig,2)||tst_fix(sig,3))pooo[sig]=0;
ris1:      fix(obkt,5);
					 if(zapret_ris==0)
					 {  //setfillstyle(1,cvt_vs);
							//bar(fr2[sig][0]-1,fr2[sig][1],fr2[sig][0]+1,fr2[sig][1]+eps);
//              bar(fr2[sig][0],fr2[sig][1],fr2[sig][0]+mid/2,fr2[sig][1]+eps);
							setlinestyle(0,0,0);setcolor(cvt_sig);
							for(i=0;i<gad;i++)circle(x1+rad+mid,y1+half,i);
 //							if(DISK!=0)
							setcolor(cvt_og);circle(x1+rad+mid,y1+half,gad);
					 }
					 break;
		//если рисуется зСо
		case 1: //если непарафазность
						if(parafaz(obkt)<0)setcolor(11);
						else
						{ if(fr3[obkt][bt]==0) //если норма
							{fix(obkt,bt);setcolor(7);}
							else //если ненорма
							{ if(tst_fix(obkt,bt))//если не фиксирована
								{
									if(get_str(obkt,bt)==0)
									{ neisprav_na_disk('@',chudo);
										zvuk_vkl(1,18);
										w(169,999,chudo);
									}
									fix(obkt,bt);
								}
								setcolor(2);
							}
						}
						if(zapret_ris==0)
						{ arc(x1+rad+mid,y1+half,dug1,dug2,abs(gad)+2);//ЗСО
							arc(x1+rad+mid,y1+half,dug3,dug4,abs(gad)+2);//ЗСО
						}
						break;
		//если рисуется ВНП
		case 2: //если непарафазность
						if(parafaz(obkt)<0)setcolor(11);
						else
						{ if(fr3[obkt][bt]==0){fix(obkt,bt);setcolor(7);} //если норма
							else //если ненорма
							{ if(tst_fix(obkt,bt))
								{
									if(get_str(obkt,bt)==0)
									{ neisprav_na_disk('@',chudo);
										w(169,999,chudo);
										zvuk_vkl(1,18);
									}
								}
								fix(obkt,bt);
								setcolor(12);
							}
						}
						if(zapret_ris==0)
						{ arc(x1+rad+mid,y1+half,dug1,dug2,abs(gad)+3);//ВНП
							arc(x1+rad+mid,y1+half,dug1,dug2,abs(gad)+4);//ВНП
						}
						break;
		//если рисуется ЖСо
		case 3: if(parafaz(obkt)<0)setcolor(11);//если непарафазность
						else
						{//если норма
							if(fr3[obkt][bt]==0){fix(obkt,bt);setcolor(7);}
							else //если ненорма
							{ if(tst_fix(obkt,bt))
								{
									if(get_str(obkt,bt)==0)
									{
										neisprav_na_disk('@',chudo);
										get_str(sig,0);
										w(281,999,chudo);
										zvuk_vkl(1,18);
									}
								}
								fix(obkt,bt);
								setcolor(14);
							}
						}
						if(zapret_ris==0)
						{ arc(x1+rad+mid,y1+half,dug1,dug2,abs(gad)+1);//ЖСО
							arc(x1+rad+mid,y1+half,dug3,dug4,abs(gad)+1);//ЖСО
						}
						break;
		//если рисуется шкаф
		case 4: if(parafaz(obkt)<0)setfillstyle(1,11);//если непарафазность
						else
						{ if(fr3[obkt][bt]==0) //если норма
							{ fix(obkt,bt);
								setfillstyle(1,7);
							}
							else //если ненорма
							{ if(tst_fix(obkt,bt))
								{
									if(get_str(obkt,bt)==0)
									{
										neisprav_na_disk('@',chudo);
										get_str(sig,0);
										w(278,999,chudo);
										zvuk_vkl(1,36);
									}
								}
								fix(obkt,bt);
								setfillstyle(1,12);
							}
						}
						if(zapret_ris==0)bar(x1-2,y1+eps,x1+2,y1+eps1);
						break;
		default:break;
	}
	if(zapret_ris==0)
	{ setcolor(8);
		setlinestyle(0,0,0);line(x1,y1+eps,x1,y1+eps1);
		if(mid!=0)line(x1,y1+half,x1+mid,y1+half);
	}
	return;
}
//----------------------------------------------------------------------
/*********************************************\
* Процедура обработки данных по огневому реле *
\*********************************************/
void ognev(int oik,int bt,int tip)
{
	int sign,i;

	nom_func("198");
	if(fr1[oik][1]!=18)sign=oik;//если огневое в самом сигнале
	else
	{
		if(fr1[oik][4+bt]==9999)return; //если бит пустой, то выйти
		else sign=fr1[oik][4+bt];       //иначе номер объекта сигнала
	}
	//если огневой объект парафазен - изобразить этот сигнал
	if(fr3[oik][5]==0)sigo(sign,tip,oik,bt);
	else //если непарафазность
		//пройти по всем сигналам данного объекта
		for(i=0;i<fr1[oik][12];i++)
		{
			if(fr1[oik][4+i]==9999)break;
			else sign=fr1[oik][4+i];
			//изобразить все сигналы для этого огневого реле
			sigo(sign,tip,oik,i);
		}
	return;
}
//-----------------------------------------------------
//процедура обработки данных по датчикам,связанным с сигналом
//огневое реле, зСо, ЖСо, ВНП, авария шкафа
void ris_batarei(int nomer,int bit_)
{
	int sign,tip,bt_vnp,bt_a,maska;

	nom_func("282");
		//если огневое реле
	tip=0;
	if(fr1[nomer][1]==18)
	{
		if(fr1[nomer][3]==1){ognev(nomer,bit_,tip);return;}
		if(fr1[nomer][4+bit_]==9999) return;//если бит пустой, то выйти
		else sign=fr1[nomer][4+bit_];       //иначе номер объекта сигнала
		switch(fr1[nomer][3])
		{
			case 5: tip=1;break;//если зСо
			case 6: tip=3;break;//если ЖСо
			case 7: tip=2;break;//если ВНП
			case 8: bt_vnp=(fr1[nomer][2]&0xf0)>>4;
							bt_a=fr1[nomer][2]&0xf;
							tip=8;
							break;
			case 11:tip=4;break;//если авария шкафа
			default: return;
		}
		if(tip==8)
		{
			sigo(sign,2,nomer,bt_vnp);
			sigo(sign,4,nomer,bt_a);
		}
		else sigo(sign,tip,nomer,bit_);
	}
	else //для объектов 2,188
	{
		if(bit_>4)return;
		maska=0x7000>>(bit_*3);
		tip=(fr1[nomer][2]&maska)>>((4-bit_)*3);
		switch(tip)
		{
			case 0:  return;
			case 1:  tip=1;break;
			case 2:  tip=3;break;
			case 3:  tip=2;break;
			case 4:  tip=4;break;
			default: return;
		}
		sign=fr1[nomer][3+bit_];       //номер объекта сигнала
		sigo(sign,tip,nomer,bit_);
	}
	return;
}
//------------------------------------------------------------------
/*********************************************\
* Процедура изображения кнопок искусственной  *
* разделки                                    *
* nomer - номер в базе для объекта разделки   *
* bt - номер бита, который надо отобразить    *
\*********************************************/
void krasit_razd(int nomer,int bt)
{
	int nom,ob_was,color,x,y,xx,yy,cl1,cl2;

	nom_func("137");
	if(((vtora!=0)||(zalet!=0))&&(prorisovka==1))return;
	color=0;

	//если ГРИ или ИВ
	if((fr1[nomer][1]==301)||(fr1[nomer][1]==302)){sekci(nomer,bt);return;}

	//если непарафазность записать
	if((bt==5)&&(fr3[nomer][5]==1))
	{
		if(tst_fix(nomer,5))//если не фиксировалось
		{
			slom_interf(nomer);
			fix(nomer,5);
		}
	}
	if((nikuda==1)||(klo==1)||(help!=0))return;

	//если объект принадлежит разделке
	if(((fr1[nomer][1]>=200)&&(fr1[nomer][1]<=300))||
	((fr1[nomer][2]>=200)&&(fr1[nomer][2]<=300)))
	{ //если предв. и исп. команды нет
		if((fr3[nomer][0]==0)&&(fr3[nomer][2]==0))
		{
			color=8;
			ob_was=markery[fr1[nomer][5]][7]&0xfff;
			//если объект числитя в разделке
			if(was[ob_was-10]!=0)
			{ //снять флаг выполнения разделки
				was[ob_was-10]=0;
				//если маркер на этом объекте,вернуть его домой
				if(modi==fr1[nomer][5])
				{
					home(modi);t(0);
					//снять флаг выдачи команды в стойку
					fr3[nomer][9]=0;
					//снять признак выдачи предварительной команды
					vtora=0;
					//стереть окно интерактива
					t(0);
					//сбросить флаги
					zalet=0;vtora=0;
					//записать в буфер клавиатуры пробел
					regs.h.ah=5;
					regs.h.ch=0x39;
					regs.h.cl=0x20;
					pooo[nomer]=0l;
					int86(0x16,&regs,&regs);
					zvuk_vkl(1,9);
					w(169,nomer," 06");//"неисправность" объекта nomer "06"
				}
			}
		}
		else
		//если исполнена предварительная команда и окончательная команда
		if((fr3[nomer][0]==1)&&(fr3[nomer][2]==1))
		//установить красный цвет сбросить флаг выдачи команды в стойку
		{
			color=12;
			fr3[nomer][9]=0; fr3[nomer][8]=0;
			t(0); //стереть окно интерактива
			zalet=0;vtora=0;//сбросить флаги
			//записать в буфер клавиатуры "пробел"
			regs.h.ah=5;
			regs.h.ch=0x39;
			regs.h.cl=0x20;
			pooo[nomer]=0l;
			int86(0x16,&regs,&regs);
		}
		//если исполнено не все
		else
		{
			//если выполнена только предварительная команда
			if(fr3[nomer][0]==1)
			{ cl1=12;
				fr3[nomer][9]=0;//сбросить признак выдачи команды
				fr3[nomer][10]=0;
				fr3[nomer][8]=0;
				if((prorisovka==0)&&(STATUS==1)&&(DU==0))
				{
					w(12,999,"");//ПОСЫЛАТЬ ОКОНЧАТЕЛЬНУЮ КОМАНДУ?
					vtora=1;
					zapretvybora=1;
				}
			}
			//если не выполнена предварительная
			else
			{ cl1=8;
				if(fr3[nomer][9]==9)//если была выдана команда
				{ if(was[ob_was-10]!=0)//если секция помечена в разделке
					was[ob_was-10]=0;//снять метку о разделке
					//если маркер на этом объекте- вернуть домой
					if(modi==fr1[nomer][5]){home(modi);t(0);}
				}
			}
			//если идет искусственная разделка
			if(fr3[nomer][2]==1)
			{
				cl2=12;color=12;
				fr3[nomer][9]=0; fr3[nomer][10]=0; fr3[nomer][8]=0;
			}
			else cl2=8;
		}
	}
	//если объект не разделка,то выйти
	else  return;
	nom=fr1[nomer][5];//взять номер объекта для маркера
	// получить координаты рисования кнопки
#ifdef NEW_KNOPKA
	imena(nom);
#else
	nom=fr1[nomer][5];//взять номер объекта для маркера
	//получить координаты рисования для кнопки
	x=markery[nom][4]-4;y=markery[nom][5]-4;
	xx=markery[nom][4]+4;yy=markery[nom][5]+4;
	if(fr3[nomer][5]==1)//если непарафазность
	{setfillstyle(SOLID_FILL,LIGHTCYAN);bar(x,y,xx,yy);return;}
	if(color!=0){setfillstyle(SOLID_FILL,color);bar(x,y,xx,yy);return;}
	else
	{
		setfillstyle(SOLID_FILL,cl1);bar(x,y,xx,y+4);
		setfillstyle(SOLID_FILL,cl2);bar(x,y+4,xx,yy);
		if(vtora==0)home(-1);
		return;
	}
#endif
}
//---------------------------------------------------------------------
/************************************************\
* Процедура изображения кнопки ГРИ и выдержки ИВ *
* nomer - номер объекта  в базе данных.          *
* bt - бит,состояние которого надо отразить.     *
\************************************************/
void krasit_knopki(int nomer,int bt)
{
	int nn1,nn2=0,bb2=0,GRIV,color,x,y,xx,yy,nom;

	nom_func("136");
	//если получены данные по выдержке
  if(fr1[nomer][1]==302)//если ИВ
  {
    nn1=fr1[nomer][2];//взять объект для кнопки 1 ст
#ifdef KOL_SOO2
    nn2=fr1[nomer][3];//взять объект для кнопки 2 ст
#else
    nn2=nn1;
#endif
  }
  //если получены данные по кнопке
  if(fr1[nomer][1]==301)//получить номера для ГСП и для ГРИВ
  {
    if(prorisovka==0)pooo[nomer]=0l;
    nn1=nomer;//ГСП из этой стойки
    nn2=fr1[nomer][3];  //Кнопка из соседней стойки
    if(nn2==9999)nn2=nn1;
  }
  //взять бит для ИВ
  GRIV=0;//состояние 0
  if(nn1!=9999)//если есть кнопка ГСП
  { if(fr1[nn1][4]!=9999)//если есть ИВ
		{
      bb2=(fr1[nn1][4]&0xf000)>>12;  //получить бит для ИВ
      nomer=fr1[nn1][4]&0xfff;       //получить объект для ИВ
      GRIV=fr3[nomer][bb2];          //получить состояние выдержки
    }
  }
  // если бит непарафазности и есть непарафазность по ИВ
	if((bt==5)&&( fr3[nomer][5]==1))slom_interf(nomer);// ИВ

	//если есть управление и непарафазность по кнопке
	if((nn1!=9999)&&(fr3[nn1][5]==1))
	{
		//Если не было фиксации, записать в архив
		if(tst_fix(nn1,5))slom_interf(nn1);
    fix(nn1,5);//зафиксировать
  }
  //если в ТУМС-2 непарафазность кнопки
  if((nn2!=9999)&&(fr3[nn2][5]==1))
  {
    //Если не было фиксации, записать в архив
		if(tst_fix(nn2,5))slom_interf(nn2);
    fix(nn2,5); //зафиксировать
  }
  if((nikuda==1)||(klo==1)||(help!=0))return;

	//если кнопка или выдержка
  if((fr1[nomer][1]==301)||(fr1[nomer][1]==302))
  { fr3[nomer][8]=0;

    //если нет ГСП и ГРИВ равен 0, то цвет серый
    if((fr3[nn1][2]==0)&&(fr3[nn2][2]==0)&&( GRIV==0))color=8;
    else
		  //если нет ГСП и ГРИВ равен 1, то цвет желтый
      if((fr3[nn1][2]==0)&&(fr3[nn2][2]==0)&&(GRIV==1))color=14;
			else
				//если есть ГСП и ГРИВ равен 0, то цвет красный
        if((fr3[nn1][2]==1)&&(fr3[nn2][2]==1)&&(GRIV==0))
				color=12;
				else
					if(fr3[nn1][2]==fr3[nn2][2])color=14; // иначе цвет - желтый
					else color=13;
		nom=fr1[nn1][5];//номер объекта управления в markery
		//если непарафазность в ГРИ или непарафазность в ГРИВ
		//или нет связи вообще или нет связи с данной стойкой
		//то цвет - циан
		if((fr3[nn1][5]==1)||(fr3[nn2][5]==1)||(fr3[nomer][5]==1))color=11;

		if(nom!=9999)//если есть управление ГРИ от АРМа
		{ //получить координаты кнопки управления и
			//нарисовать кнопку полученным цветом
			x=markery[nom][4]-4; y=markery[nom][5]-4;
			xx=markery[nom][4]+4; yy=markery[nom][5]+4;
			setfillstyle(SOLID_FILL,color);bar(x,y,xx,yy);
		}
		return;
	}
	return;
}
//-----------------------------------------------------------------------
/*******************************************\
*  Процедура изображения разных кнопок      *
* nom - номер объекта с данными о кнопке    *
* bit_ - номер бита, который отображается   *
\*******************************************/
void knopka_vkl_vsego(int nom,int bit_)
{
  int x,y, 					//координаты кнопки
	p,  							//объект управления в markery
	b_lam, 						//бит для лампы
  X_lam=0, Y_lam=0, //координаты для лампы
  R_lam, 						//радиус лампы
	svebf,svebv, 			//строки таблиц cvt и zvu_tab или сами цвета
	cvtlmp,           //цвет лампы
	c_vt[4], 					//цвета на коды 0,1,2,3
	nm1,bt1, 					//объект включения и бит включения
	napr,							//объект направления
	ob_blk,bt_blk, 		//объект и бит блокирования кнопки
	cod,
	ii;

	nom_func("125");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	switch(fr1[nom][1])
	{ //если включение и выключение на разные объекты
		case 166: p=fr1[nom][2];
							nm1=fr1[nom][3]&0xfff;bt1=(fr1[nom][3]&0xf000)>>12;
							cod=fr3[nm1][bt1];
							svebf=(fr1[nom][5]&0xff00)>>8; //строка в CVT
							svebv=cvt[svebf][cod];  //цвет на состояние объекта вкл
							break;
		//если кнопка + лампочка
		case 53:  p=fr1[nom][7];
							svebf=(fr1[nom][6]&0xff00)>>8; //цвет для включения (цвет ненажатой)
							svebv=fr1[nom][6]&0xff;    //цвет для отключения (цвет нажатой)
							bit_= fr1[nom][12]&0xf;  //бит для кнопки
							b_lam=(fr1[nom][12]&0xf0)>>4;  //бит для лампочки
							X_lam=fr1[nom][9]&0xfff;
							Y_lam=fr1[nom][10];
							R_lam=(fr1[nom][9]&0xf000)>>12;
							break;
		 //прочие кнопки
		default:  if(fr1[nom][2]==0)fr3[nom][bit_]=0; //если кнопка нормализации
							p=fr1[nom][10]; //объект markery
							bit_=fr1[nom][11]>>12;  //номер бита объекта воздействия
							if(fr1[nom][2]==33) //если дача согласия смены направления
							{
								 napr = fr1[nom][11]&0xfff;
								 svebf=(fr1[nom][5]&0xff00)>>8;//цвет для включения(цвет ненажатой)
								 svebv=fr1[nom][5]&0x00ff;
								 if(fr3[napr][1]==1)cvtlmp=10;//цвет для выключения (цвет нажатой)
								 else cvtlmp=14;//цвет для выключения (цвет нажатой)
								 bit_=(fr1[nom][11]&0xf000)>>12;
								if(fr1[nom][6]!=9999) //если есть лампа
								{
									b_lam=(fr1[nom][12]&0xf000)>>12; //бит лампы
									X_lam=fr1[nom][6]&0xfff;
									Y_lam=fr1[nom][12]&0xfff;
									R_lam=(fr1[nom][6]&0xf000)>>12;
								}
							}
							else //для прочих кнопок
							{
								//получить цвета с учетом бита влияния
								//цвет кода 0 для включения, когда бит влияния равен 0
								c_vt[0]=(fr1[nom][5]&0xF000)>>12;
								//цвет кода 1 для выключения, когда бит влияния равен 0
								c_vt[1]=(fr1[nom][5]&0x0F00)>>8;
								//цвет кода 2 для включения, когда бит влияния равен 1
								c_vt[2]=(fr1[nom][5]&0x00F0)>>4;
								//цвет кода 3 для выключения, когда бит влияния равен 1
								c_vt[3]=(fr1[nom][5]&0x000F);

							}
							break;
	}
	x=markery[p][4];
	y=markery[p][5];
	if(fr3[nom][5]==1)setfillstyle(1,11);//для непарафазности
	else
		if(fr1[nom][1]==166)setfillstyle(1,svebv);
		//если кнопка для двух объектов - установить цвет включенной
		else
		{ //для прочих кнопок кроме дачи согласия и кнопки с лампой
			if((fr1[nom][7]!=9999)&&(fr1[nom][2]<33))//если есть объект влияния
			{
				ob_blk=fr1[nom][7]&0xfff; //получить объект влияния
				bt_blk=(fr1[nom][7]&0xf000)>>12; //получить бит влияния
				//получить код с учетом бита влияния
				cod=fr3[ob_blk][bt_blk]*2+fr3[nom][bit_];
				if(fr3[ob_blk][5]==1)cod=0; //если блокирующий объект непарафазен
				setfillstyle(1,c_vt[cod]);
			}
			else
			{ //если нет объекта влияния
				if(fr1[nom][2]<33){svebf=c_vt[2]; svebv=c_vt[3];}
				//если бит сброшен
				if(fr3[nom][bit_]==0)setfillstyle(1,svebf);
				else setfillstyle(1,svebv);
			}
		}

	bar(x-4,y-4,x+4,y+4);//нарисовать кнопку
	if(X_lam!=0) //если есть лампа
	{
		if(fr3[nom][b_lam]==1)setcolor(cvtlmp);
		else setcolor(8);
		if(fr3[nom][5]==1)setcolor(11);
		setlinestyle(0,0,0);
		for(ii=0;ii<R_lam;ii++)circle(X_lam,Y_lam,ii);
	}
	return;
}
//-------------------------------------------------------------------------
/*****************************************************\
*  Процедура изображения разных текстовых кнопок      *
* nom - номер объекта с данными о кнопке              *
* bit_ - номер бита, который отображается             *
\*****************************************************/
void knopka_vkl_vsego1(int nom,int bit_)
{
	int x,y, 					//координаты кнопки (лампы)
	svebv, 		       	//цвет кнопки (лампы)
	n_cvt,   					//номер строки цветовой таблицы
	n_txt,						//номер текстовой строки для бита
	D,                //ширина текста в пикселах
	napravl,          //объект смены направления для лампочки
										//получения или запроса согласия
	poly[12];         //массив для описания координат полигона

	nom_func("125");
	if(bit_>5)return;
	if(fr1[nom][bit_+3]==9999)return;//если бит не используется - выйти
	if((klo==1)||(help!=0)||(nikuda==1))return;
	//получить цвета с учетом бита влияния
	//цвет кода 0 для включения, когда бит влияния равен 0
	x=fr2[nom][bit_*2];
	y=fr2[nom][bit_*2+1];
	n_txt = fr1[nom][bit_+3]&0xff;
	D=textwidth(name_lamp[n_txt]);
	if(fr3[nom][5]==1)
	{
		svebv=11;
		setfillstyle(1,svebv);//для непарафазности
		if(tst_fix(nom,5))slom_interf(nom);
		fix(nom,5);//установить флаг фиксации состояния
	}
	else
	{
		n_cvt=fr1[nom][2]&0xff;
		if(fr3[nom][bit_]==0)svebv=cvt[n_cvt][bit_*2];
		else //если объект активен
		{
			if((fr1[nom][bit_+3]&0xff00)!=0)//если это кнопка
			{
				svebv=cvt[n_cvt][bit_*2+1];
			}
			else //если это лампа
			{
				if((fr1[nom][12]&0xfff0)!=0)//если есть объект смены направления
				{
					napravl=(fr1[nom][12]&0xfff0)>>4;
					if(fr3[napravl][1]==0)svebv=10;
					else svebv=14;
					if(fr3[napravl][5]==1)svebv=11;
				}
			}
		}
		setfillstyle(1,svebv);
	}
	if((fr1[nom][bit_+3]&0xff00)!=0) //если это кнопка
	{
		if((getpixel(x+1,y+1)!=svebv)||(cikl_obnov==0))
		{
			bar(x,y,x+D,y+9);
			setcolor(8);
			outtextxy(x,y+2,name_lamp[n_txt]);
			setlinestyle(0,0,0);
			rectangle(x-1,y-1,x+D+1,y+10);
		}
		else
		{
			//обновить или изменить содержание образа с текстом
			obnovi_obraz(x,y,D,name_lamp[n_txt],svebv);
		}
	}
	else
	{
		setlinestyle(0,0,0);
		poly[0]=x;					poly[1]=y;
		poly[2]=x+D;				poly[3]=poly[1];
		poly[4]=poly[2]+5;	poly[5]=poly[3]+5;
		poly[6]=poly[2];  	poly[7]=poly[3]+10;
		poly[8]=poly[0];  	poly[9]=poly[7];
		poly[10]=poly[0]-5; poly[11]=poly[5];
		setcolor(8);
		fillpoly(6,poly);
		outtextxy(x,y+2,name_lamp[n_txt]);
	}
	return;
}
//-------------------------------------------------------------------------

void ris_nemar(int nom)
{
	int x,y,p,svebf,svebv,bit_,isp,ob_rm,b_rm,ob_mi,b_mi,ob_ot,b_ot,cd,c_l,ii;

	nom_func("285");
	if((klo==1)||(help!=0)||(nikuda==1))return;
	p=fr1[nom][10];//объект управления в markery
	svebv=fr1[nom][6];//цвет для включения
  svebf=fr1[nom][5];//цвет для выключения
  bit_=fr1[nom][11];//номер бита
  isp=fr1[nom][12];//объект исполнения
  ob_rm=fr1[isp][3]&0xfff;//объект РМ исполнительный
	b_rm=(fr1[isp][3]&0xf000)>>12;//бит РМ исполнительный
  ob_mi=fr1[isp][4]&0xfff;//объект МИ
  b_mi=(fr1[isp][4]&0xf000)>>12;//бит МИ
  ob_ot=fr1[isp][5]&0xfff;//объект ОТ
  b_ot=(fr1[isp][5]&0xf000)>>12;//бит ОТ
	cd=fr3[ob_ot][b_ot]*4+fr3[ob_mi][b_mi]*2+fr3[ob_rm][b_rm];
  if((fr3[ob_ot][5]==1)||(fr3[ob_mi][5]==1)||(fr3[ob_rm][5]==1))cd=8;
  x=markery[p][4];
  y=markery[p][5];
  if(fr3[nom][5]==1)setfillstyle(SOLID_FILL,LIGHTCYAN);//для непарафазности
  else
    if(fr3[nom][bit_]==0)//если бит сброшен
    setfillstyle(SOLID_FILL,svebf);
    else setfillstyle(SOLID_FILL,svebv);
  bar(x-4,y-4,x+4,y+4);
  switch(cd)
  {
    case 0: c_l=8; break;
    case 1: c_l=15;break;
		case 2: c_l=12;break;
		case 3: c_l=10;break;
    case 4: c_l=13;break;
    case 5: c_l=13;break;
    case 6: c_l=14;break;
    case 7: c_l=13;break;
    case 8: c_l=3;break;
  }
  setlinestyle(0,0,0);
  setcolor(c_l);
	for(ii=3;ii>0;ii--)circle(x,y,ii);
  return;
}
//---------------------------------------------------------------------
/******************************************\
* Процедура подготовки отображения трассы  *
* немаршрутизированных передвижений        *
* и выдача на экран                        *
\******************************************/
void nemar(int ob)
{
  int ob_up,ob_rm,bt_rm,ob_mi,bt_mi,ob_ot,bt_ot,ij,cd,
  sig_n,sig_k;

  ob_up=fr1[ob][2]&0xfff; //объект для управления (вкл/откл) немар.передвиж.

  ob_rm=fr1[ob][3]&0xfff;
  bt_rm=(fr1[ob][3]&0xf000)>>12;

  ob_mi=fr1[ob][4]&0xfff;
  bt_mi=(fr1[ob][4]&0xf000)>>12;

  ob_ot=fr1[ob][5]&0xfff;
  bt_ot=(fr1[ob][5]&0xf000)>>12;

  sig_n=fr1[ob_up][2];
  sig_k=fr1[ob_up][7];

	nom_func("182");
	//вычислить код состояния
  cd=fr3[ob_ot][bt_ot]*4+fr3[ob_mi][bt_mi]*2+fr3[ob_rm][bt_rm];

  //если свободно, разомкнуто и не в разделке
  if(cd==0){fr3[sig_n][12]=0;fr3[sig_k][12]=0;}

  if((cd==1)||(cd==3)||(cd==7)){fr3[sig_n][12]=15;fr3[sig_k][12]=15;}
  if((fr3[ob_ot][5]==1)||(fr3[ob_rm][5]==1)||(fr3[ob_mi][5]==1))cd=8;
  for(ij=6;ij<13;ij++)
  {
    if(fr1[ob][ij]==9999)break;

    fr3[fr1[ob][ij]][12]=fr3[fr1[ob][ij]][12]&0xf;//взять младшие биты
    switch(cd)
    {
      case 1: fr3[fr1[ob][ij]][12]=fr3[fr1[ob][ij]][12]|16;break;
      case 3: fr3[fr1[ob][ij]][12]=fr3[fr1[ob][ij]][12]|32;break;
      case 7: fr3[fr1[ob][ij]][12]=fr3[fr1[ob][ij]][12]|64;break;
    }
    sekci(fr1[ob][ij],0);//изобразить секцию
  }
  ris_nemar(ob_up);
  return;
}
//---------------------------------------------------------------------
/****************************************************\
* Поцедура изображения кнопок управления оповещением *
\****************************************************/
void knopka_vkl_opove(int nom)
{
  int rm,     // номер объекта для РМ
  b_rm,       // номер бита для РМ
  zm,         // номер объекта для ЗМ
  b_zm,       // номер бита для РМ
  x,y,p,svebf,treug[6];

	nom_func("124");
	if((klo==1)||(help!=0)||(nikuda==1))return;
  rm=fr1[nom][2]&0xfff;
  b_rm=(fr1[nom][2]&0xf000)>>12;
  zm=fr1[nom][7]&0xfff;
  b_zm=(fr1[nom][7]&0xf000)>>12;
  p=fr1[nom][10]; //номер объекта управления в markery
  x=markery[p][4]-4;y=markery[p][5];
  if(fr3[nom][5]==1)svebf=11;
  else
  {   //установить цвета для сочетаний РМ-ЗМ
    switch(fr3[rm][b_rm]*2+fr3[zm][b_zm])
    {
      case 0: svebf=8;break;
      case 1: svebf=13;break;
      case 2: svebf=14;break;
      case 3: svebf=12;break;
    }
  }
  setcolor(svebf); setfillstyle(1,svebf);
  setlinestyle(0,0,0);
  treug[0]=x+2;treug[1]=y;treug[2]=x+8;treug[3]=y-4;treug[4]=x+8;treug[5]=y+4;
  fillpoly(3,treug);
  return;
}
//-------------------------------------------------------
void slom_uch(int nomer,char vid)
{
  int o,i;
  char dobav[3];

	nom_func("308");
	if(pako[nomer][0]==0)return;
  for(o=0;o<20;o++)chudo[o]=0;
  dobav[0]='_';dobav[1]=vid;dobav[2]=0; 
  o=0;
  while(pako[nomer][o]==' ')o++;
  i=0;
  while((pako[nomer][o]!=' ')&&(pako[nomer][o]!=0))chudo[i++]=pako[nomer][o++];
  strcat(chudo,dobav);
	neisprav_na_disk('&',chudo);
  return;
}
/**********************************************/
void slom_sign(int nomer)
{
  int o;

	nom_func("307");
	for(o=0;o<20;o++)chudo[o]=0;
	strcpy(chudo,pako[nomer]);
	if(fr3[nomer][0]==1)strcat(chudo,"_М");
  if(fr3[nomer][1]==1)strcat(chudo,"_П");
  neisprav_na_disk('*',chudo);
  w(236,999,chudo);
  zvuk_vkl(0,0);
  return;
}
//--------------------------------------------------------------------
/***********************************************\
* Процедура переинициализации картины при входе *
* в просмотр архива                             *
\***********************************************/
void dr()
{

	nom_func("45");
	if((nikuda==1)||(help>0))goto fin; //если не основной экран - выход

  if(klo==1){kartina();goto fin; } //если картина каналов - выход
  if (marazm==1){uprav=1;marazm=0;}

  //анализ и установка режима управления объектами
  if (tst==2) uprav=1;
  else uprav=0;

  GraphMode=VGAHI;perekluch=1;

  if((otkaz_ts[0]==0)&&(otkaz_ts[1]==0))
	{
		clscreen();
    if(zabeg==0)zapretvybora=0;
  	zagruzen();
		prodol();
	}
  else
  {
    if(otkaz_ts[0]==0) z1(1);
    if(otkaz_ts[1]==0) z1(2);
  }
fin:
  perekluch=0;pusto=0;
  if(otkaz_ts[0]==0){outportb(BAZ_ADR1+1,3);outportb(BAZ_ADR1,0);}
  if(otkaz_ts[1]==0){outportb(BAZ_ADR11+1,3);outportb(BAZ_ADR11,0);}
  return;
}
//------------------------------------------------------------------------
/****************************************************\
* Процедура изображения квадратика в правом нижнем   *
* углу экрана меняющего свой цвет от мультивибратора *
* и вывод состояния каналов для просмотра УВК        *
* nom-объект КВВ в базе                              *
* бит-бит,требующий отображения                      *
\****************************************************/
void kvv(int nom,int bit_)
{
  int KVx,Krz,SME=0;

	nom_func("140");
	if(nom==KVV1){KVx=KVV1;Krz=KVV1;SME=625;}
  else
  {
#ifdef KOL_SOO2
		KVx=KVV2;Krz=KVV2;SME=630;
#endif
  }
  if(bit_==0)//если бит мультивибратора
  {
    if((klo==0)&&(nikuda==0)&&(help==0))
    { if(fr3[nom][5]==1)setfillstyle(SOLID_FILL,11);
      else setfillstyle(SOLID_FILL,2);
      if(fr3[nom][bit_]==0)setfillstyle(SOLID_FILL,8);
      bar(SME,450,SME+5,455);
    }
    return;
  }
  if(fr3[nom][1]==1)
  {
		if(tst_fix(nom,1))slom_interf(4070+fr1[nom][13]-1);
    fix(nom,1);
    return;
  }
  if(bit_==2){if(klo==1)kps_osn(KVx);    return;}
  if(bit_==3){if(klo==1)kps_rez(Krz);    return;}
  return;
}
//------------------------------------------------------------------------
/**************************************************************\
* Процедура записи в журнал неисправностей сообщения о ненорме *
* контрольной суммы программы ТУМСа.                           *
\**************************************************************/
void Pam(int nom)
{

	nom_func("215");
	if(tst_fix(nom,0))
  {
		if(fr3[nom][0]==1){slom_interf(8299+fr1[nom][13]);}
    fix(nom,0);
  }
	if(tst_fix(nom,1))
	{
		if(fr3[nom][1]==1){slom_interf(8399+fr1[nom][13]);}
		fix(nom,1);
	}
	if(tst_fix(nom,2))
	{
		if(fr3[nom][2]==1)
		{
			otv_kom=1;
			knopka_OK();
		}
		if(fr3[nom][2]==0)
		{
			otv_kom=0;
			knopka_OK();
		}
		fix(nom,2);
	}
  return;
}
//-----------------------------------------------------------------
/************************************************\
* Процедура установки цвета подсветки на все СП. *
\************************************************/
void pict_podsvetka()
{ int K,pamjat=0;

	nom_func("221");
	//пройти по всем основным элементам
  for (pamjat=0;pamjat<kol_VO;pamjat++)
  {
    if((fr1[pamjat][0]==3)&&(fr1[pamjat][1]<3)&&(fr1[pamjat][2]<3))
    {K=pamjat;zap_matr(K);cvet_matr();ris_sp_str();}
  }
  return;
}
//---------------------------------------------------------
#ifdef VSP
void ris_smen_otv(int nomer,int bit_)
{
	int nom,first,color,xc,yc,oho,j13;
	if((klo==1)||(help!=0)||(nikuda==1))return;
	if (fr1[nomer][3]==2)first=2;
	else
		if(fr1[nomer][3]==1) first=1;
	if(bit_==2)
	{ if (fr3[nomer][2]==1)color=12;
		else color=8;
		nom=fr1[nomer][6];
	}
	else
	if(bit_==3)
	{ if(fr3[nomer][3]==1) color=12;
		else color=8;
		nom=fr1[nomer][5];
	}
	else return;
	xc=markery[nom][4];
	yc=markery[nom][5];
	oho=9999;
	for(j13=0;j13<VSP;j13++)
	if(mark[j13][0]==nom){oho=j13;break;}
	if(oho==9999) return;
	if(fr3[nomer][5]==1)
	{ setfillstyle(SOLID_FILL,LIGHTCYAN);
		if(first==1)bar(xc-4,yc-4,xc+4,yc);
		if(first==2)bar(xc-4,yc,xc+4,yc+4);
		return;
	}
	setfillstyle(SOLID_FILL,color);
	if(color==12)
		if(first==1)
		{ bar(xc-4,yc-4,xc+4,yc);
			if((mark[oho][1]==1)&&(priem_==1)&&(fr3[nomer][9]!=0))
			{
				w(12,999,"");
				zapretvybora=1;
			}
			if((mark[oho][1]==-1)&&(priem_==-1)&&(fr3[nomer][9]!=0))
			{
				w(12,999,"");
				zapretvybora=1;
			}
			fr3[nomer][9]=0;
		}
		else
		{ bar(xc-4,yc,xc+4,yc+4);
			fr3[nomer][7]=0;
		}
	else
		if(first==1)
		{ bar(xc-4,yc-4,xc+4,yc);
			if((mark[oho][1]==1)&&(priem_==1))fr3[nomer][9]=0;
			if((mark[oho][1]==-1)&&(priem_==-1))fr3[nomer][9]=0;
			fr3[nomer][9]=0;
		}
		else bar(xc-4,yc,xc+4,yc+4);
	return;
}
#endif
//------------------------------------------------
#ifdef OGRAD
void k_ograd(int ob_sogl)
{ int ob_zapros,ob_ogr,bit_zapros,bit_sogl,bit_ogr,put,n_bait;
	int sp_chet,sp_nchet,st_chet,st_nchet,otvod_chet,otvod_nchet;
	unsigned char koda;
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;
	put=fr1[ob_sogl][4];
	ob_zapros=(fr1[ob_sogl][2]&0xfff);
	bit_zapros=(fr1[ob_zapros][2]&0xf000)>>12;

	ob_ogr=fr1[ob_sogl][3]&0xfff;
	bit_ogr=(fr1[ob_ogr][3]&0xf000)>>12;

	bit_sogl=(fr1[ob_sogl][12]&0xf000)>>12;

	sp_chet=fr1[ob_sogl][6];
	st_chet=fr1[ob_sogl][7];
	otvod_chet=fr1[ob_sogl][8];

	sp_nchet=fr1[ob_sogl][9];
	st_nchet=fr1[ob_sogl][10];
	otvod_nchet=fr1[ob_sogl][11];

	if(klaval==13)
	{ if(may_be_ograd(put)==1){home(modi);return;}
		// ВЫДАЕМ СОГЛАСИЕ НА ОГРАЖДЕНИЕ,ПУТЬ ##
		w(156,put,""); home(modi);oppp=0;
		koda='A';
		point=ob_sogl;
		fotksig(koda);
		if(n_bait==12)buf_ko[9]=124;
		buf_ko[n_bait-2]=check_sum(buf_ko);
		pooo[ob_sogl]=biostime(0,0L);
		flagoutmsg=1;
	}
}
#endif
//----------------------------------------------------------------
/******************************************************\
/* Процедура проверки парафазности объекта и сброса    *
*  таймеров при непарафазности. Возвращает 0 при норме *
*  парафазности возвращает -1 при ненорме.             *
\******************************************************/
int parafaz(int ob)
{

	nom_func("216");
	if(fr3[ob][5]==1)//при непарафазности
	{
		if(tst_fix(ob,5))
		{
			slom_interf(ob);
			fix(ob,5);
			if(pooo[ob]!=0l)sbros_time(ob,5);
			if(pooo_com[ob]!=0l)pooo_com[ob]=0l;
		}
		return(-1);
	}
	else
	{
		if(tst_fix(ob,5))fix(ob,5);
		return(0);
	}
}
//-----------------------------------------------------------------------
/****************************************************\
* Процедура отображения, вывода на экран сообщения   *
* и записи на диск состояния объекта управления типа *
* лампа, зависящая от двух объектов из разных сообще-*
* ний стойки ТУМС                                    *
\****************************************************/
void lampa_compl(int nom,int bit_)
{
  int goj=0,ob_svyaz,ob_sn,bit_sn;

	nom_func("144");
	//если нет координаты X для 1-ой лампы объекта, то выйти
  //если система находитс в просмотре помощи, неисправностей или нет связи
  if((klo==1)||(help!=0)||(nikuda==1))return;
  if((fr1[nom][2*bit_+3]==9999)&&(fr1[nom][2*bit_+4]==9999))return;
  //если бит установлен и нет переключения экранов
  if((fr3[nom][bit_]==1)&&(perekluch==0))
  //если надо фиксировать данное событие
  if(zvu_tabl[fr1[nom][2]][bit_+4]!=0)
  //если найдено имя для бита
  if(get_str(nom,bit_)==0) neisprav_na_disk(zvu_tabl[fr1[nom][2]][bit_+4],chudo);
  ob_svyaz=fr1[nom][11];//определить объект связи
  ob_sn=fr1[ob_svyaz][bit_*2];//определить смежный объект
  bit_sn=fr1[ob_svyaz][bit_*2+1];//определить бит смежного объекта
	if(fr3[nom][bit_]==1)// если основной датчик сработал
  {
    if(fr3[ob_sn][bit_sn]==0)
    setcolor(fr1[ob_svyaz][10]);//взять цвет для 0-вого значения смежного бита
    if(fr3[ob_sn][bit_sn]==1)
    setcolor(fr1[ob_svyaz][11]);//взять цвет для 1-ного значения смежного бита
    setlinestyle(0,0,0);
    //если нужен звук и нет переключения экранов
    if((zvu_tabl[fr1[nom][2]][bit_]!=0)&&(perekluch==0))zvuk_vkl(1,18);
  }
  else // если основной датчик сбросился
  {
    if(fr3[ob_sn][bit_sn]==0)
    setcolor(fr1[ob_svyaz][8]);//взять цвет для 0-го значения смежного бита
    if(fr3[ob_sn][bit_sn]==1)
    setcolor(fr1[ob_svyaz][9]);//взять цвет для 1-го значения смежного бита
    setlinestyle(0,0,0);
    //если надо звук и нет переключения экрана
    if((zvu_tabl[fr1[nom][2]][bit_]!=0)&&(perekluch==0))zvuk_vkl(1,18);
  }
  //если непарафазность или нет связи
  if(fr3[nom][5]==1)setcolor(LIGHTCYAN);
  for(goj=fr1[ob_svyaz][12]-1;goj>=0;goj--)
	circle(fr1[nom][3+2*bit_],fr1[nom][4+2*bit_],goj);
  return;
}
#ifdef OGRAD
//------------------------------------------------------
//Процедура проверки возможности установки ограждения пути
int may_be_ograd(int put_ogr)
{ int element,napr,vhod_s_v=0,strelka;
  element=put_ogr;
  napr=1; //начинаем движение в четную сторону
start:
  if(napr==1)element++;
	if(napr==0)element--;
	strelka=0;
	while(fr1[element][0]!=5)
	{ if(napr==1&&vhod_s_v==0)element++;
		if(napr==0&&vhod_s_v==0)element--;
		if(fr1[element][0]==1)//если вышли на стрелку
		{ strelka++;
			if(fr1[element][7]==napr)//если вход на стрелку совпадает с движением
			{ if(fr3[element][0]==1&&fr3[element][1]==0)//если стрелка в плюсе
				{ if(fr1[element][1]==0)// если переход по плюсу c отклонением
					{
					 element=fr1[element][2];
					}
				}
			}
			else//вход на стрелку не совпадает с направлением
			{ if(vhod_s_v==1)//если входим с отклонения
				{ if(fr3[element][0]==1&&fr3[element][1]==0//если стрелка в плюсе
					&&fr1[element][1]==1)// если переход по плюсу прямой
					{ vhod_s_v=0;
						if(strelka==1)break;
					}
					if(fr3[element][0]==0&&fr3[element][1]==1//если стрелка в минусе
					&&fr1[element][1]==0)// если переход по минусу прямой
					{ vhod_s_v=0;
            if(strelka==1)break;
          }
        }
        if(vhod_s_v==0)//если входим по прямой
        { if(fr3[element][0]==1&&fr3[element][1]==0//если стрелка в плюсе 
          &&fr1[element][1]==0)// если переход по плюсу отклоненный 
          { vhod_s_v=0;
            if(strelka==1)break;
          }
          if(fr3[element][0]==0&&fr3[element][1]==1//если стрелка в минусе
					&&fr1[element][1]==1)// если переход по минусу отклоненный
          { vhod_s_v=0;
            if(strelka==1)break;
          }
        }
      }
    }
    if(fr1[element][0]==3)//если вышли на СП за стрелкой
    { if((fr3[element][1]&1)==1)//если СП замкнут
      { w(96,element,"");
        home(modi);
        return(1);
      }
      else break;
    }
    else
    if(fr1[element][0]==6)//если переход 
    { element=fr1[element][1];
      vhod_s_v=1;
    }
    else vhod_s_v=0;
  }
  if(napr==1)
  { napr=0;
		element=put_ogr;
    goto start;
  }
  else return(0);
}
#endif
//-----------------------------------------------------------
//процедура проверки новизны для объекта "ob" по биту "bt"
//для записей ненорм в дисковый архив
//контроль ведется по слову fr3[ob][11]
int tst_fix(int ob,int bt)
{

//	nom_func("364");
	if((fr3[ob][bt]==1)&&((fr3[ob][11]&(1<<bt))!=0))return(0);
	if((fr3[ob][bt]==0)&&((fr3[ob][11]&(1<<bt))==0))return(0);
	return(1);
}
//----------------------------------------------------------
//процедура фиксации восприятия состояния объекта при записи
//на диск и выдаче текстовых сообщений-предупреждений
void fix(int ob,int bt)
{
//	nom_func("64");
	if(fr3[ob][bt]==1)fr3[ob][11]=fr3[ob][11]|(1<<bt);
	if(fr3[ob][bt]==0)fr3[ob][11]=fr3[ob][11]&(~(1<<bt));
	return;
}
/************************************************************************\
* процедура изображения кнопки выдачи ответственных команд (не разделка) *
\************************************************************************/
void ris_otv_knop(int ob)
{
	unsigned int mrk1,//объект управления предварительный
	mrk2,//объект управления исполнительный
	pred,//предварительный объект
	isp,//исполнительный объект
	b_p,//бит предварительного объекта
	b_i,//бит исполнительного объекта
	x,//координата x
	y,//координата y
	ob_dob,//дополнительный(определяющий) объект
	b_d,//бит дополнительного объекта
	b_lam, //бит для лампы
	r_lam, //радиус лампы
	X_lam, //координата X для лампы
	Y_lam, //координата Y для лампы
	invers,//признак инверсии цветов
	cvt,//цвет исполнительного объекта
	cvt1,//цвет предварительного объекта
	inv,
	inv1,
	i;

	nom_func("287");
	pred=fr1[ob][2];//получить предварительный
	isp=fr1[ob][3];//получить исполнительный
	invers=fr1[ob][10]&0xf000;//получить признак инверсии
	if(invers!=0)//если нужна инверсия
	{
		cvt=(fr1[pred][11]&0xff00)>>8;
		cvt1=(fr1[isp][11]&0xff00)>>8;
		inv=fr1[pred][11]&0xff;
		inv1=fr1[isp][11]&0xff;
	}
	else//если нет инверсии
	{
		inv=(fr1[pred][11]&0xff00)>>8;
		inv1=(fr1[isp][11]&0xff00)>>8;
		cvt=fr1[pred][11]&0xff;
		cvt1=fr1[isp][11]&0xff;
	}
	mrk1=(fr1[ob][7]&0xff00)>>8;
	mrk2=fr1[ob][7]&0xff;
	x=markery[mrk2][4];y=markery[mrk2][5];//получить координаты исполнительного
	if(fr3[ob][5]==1)
	{
		setfillstyle(1,11);
		bar(x-4,y-4,x+4,y+4);
		goto a;
	}
	b_p=(fr1[ob][4]&0xff00)>>8;
	b_i=fr1[ob][4]&0xff;
	if(fr3[pred][b_p]==1)setfillstyle(1,cvt);
	else setfillstyle(1,inv);
	bar(x-4,y-4,x+4,y);

	if(fr3[isp][b_i]==1)setfillstyle(1,cvt);
	else setfillstyle(1,inv);
	bar(x-4,y,x+4,y+4);
a:
	x=markery[mrk1][4];//получить координаты предварительного
	y=markery[mrk1][5];
	if(fr3[pred][5]==1)
	{
		setfillstyle(1,11);
		bar(x-4,y-4,x+4,y+4);
		goto final;
	}
	if(fr1[ob][9]!=9999)displaysignal(fr1[ob][9]&0xfff,0);
	if((fr1[pred][9]<9999)&&(ob==pred))
	{
		ob_dob=fr1[pred][9]&0xfff;
		b_d=(fr1[pred][9]&0xf000)>>12;
		if(fr3[ob_dob][b_d]==1)setfillstyle(1,cvt);
		else setfillstyle(1,inv);
		bar(x-4,y-4,x+4,y+4);
		goto final;
	}
	if((fr3[isp][b_i]==1)&&(fr3[pred][b_p]==1))setfillstyle(1,cvt1);
	else setfillstyle(1,inv1);
	bar(x-4,y-4,x+4,y+4);
final:
	if(fr1[ob][8]!=9999)//если есть лампа
	{
		b_lam=(fr1[ob][8]&0xf000)>>12;
		r_lam=(fr1[ob][12]&0xf000)>>12;
		X_lam=fr1[ob][8]&0xfff;
		Y_lam=fr1[ob][12]&0xfff;
		if(fr3[ob][b_lam]==1)setcolor(fr1[ob][10]&0xf);
		else setcolor((fr1[ob][10]&0xf0)>>4);
		if(fr3[ob][5]==1)setcolor(11);
		setlinestyle(0,0,0);
		for(i=r_lam;i<6;i++)circle(X_lam,Y_lam,i);
		if(fr3[ob][5]==1)return;
	}
	if((fr3[pred][b_p]==0)&&(zapusk==2))
	{
		if((STATUS==1)&&(tst_fix(pred,b_p)))
		{
			w(12,999,""); vtora=1;zapretvybora=1; zapusk++;
			fix(pred,b_p);
		}
	}
	else
	{
		fix(pred,b_p);
		fix(isp,b_i);
	}
}
//----------------------------------------------
//отображение "ворот" для УКГ
void UKG(int ob)
{
	int bt_kg, //бит для контроля габарита
	bt_okg, //бит отключателя контроля габарита
	upr,//объект управления отключателем
	bt_upr, //бит управления отключателем
	x,  //x-координата размещения "ворот"
	y,  //y-координата размещения "ворот"
	xt, //x-координата для вывода текста
  yt, //y-координата для вывода текста
  cvt, //цвет "ворот"
  cod; //код состояния

	nom_func("382");
	bt_kg=fr1[ob][2];
  bt_okg=fr1[ob][3];
  upr=fr1[ob][4]&0xfff;
  bt_upr=(fr1[ob][4]&0xf000)>>12;
  cod=fr3[ob][bt_kg]+fr3[ob][bt_okg]*2+fr3[upr][bt_upr]*4;
  switch(cod)
  {
    case 0:cvt=8;break;
    case 1:cvt=14;break;
    case 3:cvt=13;break;
    case 4:cvt=8;break;
    case 7:cvt=12;break;
    default: cvt=13;break;
  }
  x=fr1[ob][5];
  y=fr1[ob][6];
  xt=fr1[ob][8];
  yt=fr1[ob][9];
  if(fr3[ob][5]==1)cvt=11;
  setcolor(cvt);
  if(fr1[ob][7]==1)//если въезд справа
  {
    setlinestyle(0,0,1);
    line(x,y+2,x+3,y+5);line(x,y+1,x+3,y+4);
    line(x+3,y+5,x+6,y+5);line(x+3,y+4,x+6,y+4);
    line(x,y-2,x+3,y-5); line(x,y-1,x+3,y-4);
    line(x+3,y-5,x+6,y-5);line(x+3,y-4,x+6,y-4);
  }
  if(fr1[ob][7]==0)//если въезд слева
  {
    setlinestyle(0,0,1);
    line(x,y+2,x-3,y+5);line(x,y+1,x-3,y+4);
    line(x-3,y+5,x-6,y+5);line(x-3,y+4,x-6,y+4);
    line(x,y-2,x-3,y-5); line(x,y-1,x-3,y-4);
    line(x-3,y-5,x-6,y-5);line(x-3,y-4,x-6,y-4);
  }
  if(((biostime(0,0)-pooo[ob])>18)&&(pooo[ob]!=0))
  {
    setcolor(7);
    outtextxy(xt,yt,"███████████████████████");
    if((biostime(0,0)-pooo[ob])>36)pooo[ob]=biostime(0,0);
  }
  if(((cod==7)||(cod==0))&&(pooo[ob]==0))pooo[ob]=biostime(0,0);
  if(cod==0)
  {
    if(tst_fix(ob,bt_kg))
    {
      w(255,999,"");
      if(STATUS==1)sound(700);
    }
    setcolor(4);
    if((biostime(0,0)-pooo[ob])<18)
    {
      outtextxy(xt,yt,"НЕОБХОДИМО ВКЛЮЧИТЬ УКГ");
    }
  }
  else
    if(cod==7)
    {
      if(tst_fix(ob,bt_kg))
      {
        if(get_str(ob,bt_kg)==0)neisprav_na_disk('@',chudo);
        w(254,999,"");
        if(STATUS==1)sound(700);
      }
      setcolor(4);
      if((biostime(0,0)-pooo[ob])<18)
      {
        outtextxy(xt,yt,"СРАБОТАЛО УКГ");
      }
    }
    else
      {
        if(tst_fix(ob,bt_kg))nosound();
        pooo[ob]=0;
        setcolor(7);
        outtextxy(xt,yt,"███████████████████████");
      }
   fix(ob,bt_kg);
   fix(ob,bt_okg);
  return;
}
//--------------------------------------------------------------------------
/**************************************************************************\
* Процедура отображения входного сигнала соседней станции                  *
* param - тип входа в данную функцию                                       *
* param=0 - вход из объекта типа LRU(датчик сигнала) или смены направления *                                      *
* param=1 - вход из объекта LKO (датчик огневого реле)                     *
* param=5 - вход из анализа таймера-по истечению времени                   *
* ob - номер объекта в базе                                                *
* bt - номер бита в объекте                                                *
\**************************************************************************/
void lru_lko(int ob,int bt,int param)
{
	int ob_ru, // датчик сигнала
	bt_ru,     // бит сигнала
	ob_ko,     // датчик огневого
	bt_ko,     // бит огневого
	ob_zs,		 // датчик ЗС
	bt_zs,     // бит ЗС
	ob_nv,     // объект управления сменой направления
	ob_sn,     // объект смены направления
	ob_sopr,   // сопряженный объект
	bt_sopr,   // сопряженный бит
	cod,       // код состояния
	color_cod, // цвет
	X,         // координата X лампы
	Y,         // координата Y лампы
	i;
	unsigned char  neisp;     // литера неисправности
	if(ZAGRUZKA!=0)return;
	ob_ru=ob_ko=ob_zs=ob_nv=ob_sn=0;
	if(fr1[ob][bt+2]==9999)return;//если объекта для данного бита нет - выйти

	ob_sopr=ob;bt_sopr=bt;

povt:
	while((ob_sopr!=ob_ru)&&(ob_sopr!=ob_ko)&&(ob_sopr!=ob_zs)&&(ob_sopr!=0))
	{
		switch((fr1[ob_sopr][bt_sopr+2]&0xc000)>>14)
		{
			case 0:	//если это объект РУ
							if(ob_ru!=0){ob_sopr=0;break;}
							ob_ru=ob_sopr;
							bt_ru=bt_sopr;
							break;
			case 1:	//если это объект KO
							if(ob_ko!=0){ob_sopr=0;break;}
							ob_ko=ob_sopr;
							bt_ko=bt_sopr;
							break;
			case 2: //если это объект ЗС
							if(ob_zs!=0){ob_sopr=0;break;}
							ob_zs=ob_sopr;
							bt_zs=bt_sopr;
							break;
			default: break;
		}
		if(ob_sopr==0)break;
		ob_sopr=fr1[ob_sopr][bt_sopr+2]&0xfff;
	}
	ob_nv=fr1[ob][bt+7];
	ob_sn=fr1[ob_nv][9];
	cod=fr3[ob_sn][1]*8+fr3[ob_ru][bt_ru]*4;
	if(ob_ko!=0)cod=cod+fr3[ob_ko][bt_ko]*2;
	if(ob_zs!=0)cod=cod+fr3[ob_zs][bt_zs];
	if(cod>=8)cod=cod-8;
	else cod=cod+8;

	if(pusto!=0)goto ris;//исключение анализа при отсутствии связи
	switch(cod)
	{ //если соседняя станция на приеме
		//если все в норме
		case 0:
		case 1:	color_cod=4;break;
		//если огневое не в норме - сигнал закрыт-запустить таймер
		//ждем отпускания огневого или открытия сигнала
		case 2:
		case 3: if(tst_fix(ob,bt)&&(pooo[ob]==0))
						TIMER_SOB(ob_ru,3,bt_ru);
						if(param!=5)return;
						color_cod=7;
						break;
		case 4:
		case 6: if(ob_zs==0)color_cod=10;
						else color_cod=14;
						break;


		case 5:
		case 7: color_cod=10;
						break;
		//если соседняя станция на отправлении
		default: if(ob_ru!=0)fr3[ob_ru][bt_ru]=0;
						 if(ob_ko!=0)fr3[ob_ko][bt_ko]=0;
						 if(ob_zs!=0)fr3[ob_zs][bt_zs]=0;
							color_cod=8;break;
	}
	sbros_time(ob_ru,bt_ru);

	if((cod<8)&&(tst_fix(ob_ru,bt_ru)||tst_fix(ob_ko,bt_ko)))
	{ neisp='@';
		if((cod==2)||(cod==3))//если требуется вывод на экран сообщения
		{
			if(neisp!=0)
			{

		//		zvuk_vkl(0,0);
		//		if(get_str(ob_ko,bt_ko)==0)neisprav_na_disk(neisp,chudo);
		//		w(88,999,chudo);
				pooo[ob_ru]=0;
				pooo_com[ob_ru]=0;
			}
		}
	}
	fix(ob_ru,bt_ru);
	fix(ob_ko,bt_ko);
ris:
	X=fr1[ob_sn][6];
	if(X>500)//если объект справа на экране
	{
		X=X+fr1[ob_sn][5]-5;
		Y=fr1[ob_sn][7]+7;
	}
	if((fr3[ob][5]==1)||(fr3[ob_sn][5]==1))setcolor(11);
	else setcolor(color_cod);
	setlinestyle(0,0,0);
	for(i=1;i<=2;i++)circle(X,Y,i);
	if(color_cod==7)setcolor(12);
	circle(X,Y,3);
	return;
}
//===========================================
void zona_opov(int obi,int bit_)
{
  int RRM,VS,VO,BIT_VO,cod,cvet,jl,ckl;
  unsigned int RAD,X,Y;

	nom_func("437");
	if(fr1[obi][1]==15)//если это объект VS
  {
    RRM=obi;
    VS=fr1[obi][10];
  }
  else
  {
    if(fr1[obi][1]==14)//если это объект RRM
    {
      VS=obi;
      RRM=fr1[obi][10];
    }
    else return;
  }
  VO=fr1[obi][11]&0xFFF;
  BIT_VO=(fr1[obi][11]&0xf000)>>12;
  if(bit_==5)
  {
    for(ckl=0;ckl<4;ckl++)
    {
      cod=fr3[VO][BIT_VO]*4+fr3[RRM][ckl]*2+fr3[VS][ckl];
      switch(cod)
      {
        case 0: cvet=8;break;
				case 1: cvet=14;break;
        case 2: cvet=13;break;
        case 3: cvet=13;break;
        case 4: cvet=12;break;
        case 5: cvet=14;break;
        case 6: cvet=15;break;
        case 7: cvet=13;break;
      }
      if((fr3[VS][5]==1)||(fr3[RRM][5]==1)||(fr3[VO][5]==1))cvet=11;
			setcolor(cvet);setlinestyle(0,0,0);
			RAD=fr1[obi][2+ckl*2]>>12;
      X=fr1[obi][2+ckl*2]&0xfff;
      Y=fr1[obi][3+ckl*2];
      if(Y==9999)continue;
			for(jl=RAD;jl>0;jl--)circle(X,Y,jl);
endik:
    }
    return;
  }
  cod=fr3[VO][BIT_VO]*4+fr3[RRM][bit_]*2+fr3[VS][bit_];
  switch(cod)
  {
    case 0: cvet=8;break;
    case 1: cvet=14;break;
    case 2: cvet=13;break;
    case 3: cvet=13;break;
    case 4: cvet=12;break;
    case 5: cvet=14;break;
    case 6: cvet=15;break;
    case 7: cvet=13;break;
  }
	if((fr3[VS][5]==1)||(fr3[RRM][5]==1)||(fr3[VO][5]==1))cvet=11;
  setcolor(cvet);setlinestyle(0,0,0);
  RAD=fr1[obi][2+bit_*2]>>12;
  X=fr1[obi][2+bit_*2]&0xfff;
  Y=fr1[obi][3+bit_*2];
  if(Y==9999)return;
	for(jl=RAD;jl>0;jl--)circle(X,Y,jl);
  return;
}
//=======================================================================
void lampa_fr2(int obj,int bt)
{
	int cvt1,cvt0,x,y,rad,zv_sig,zap_disk,ii,invers;

	nom_func("441");
	if((fr1[obj][bt+4]&0x3000)==0x2000)invers=1;
	else invers=0;
	cvt0=(fr1[obj][bt+4]&0xf00)>>8; //цвет для нулевого значения бита
	cvt1=(fr1[obj][bt+4]&0xf0)>>4;  //цвет для единичного значения бита
	x=fr2[obj][bt*2]&0xfff; //x-координата лампы
	y=fr2[obj][bt*2+1];     //y-координата лампы
	rad=(fr2[obj][bt*2]&0xf000)>>12; //радиус лампы
	zv_sig=(fr1[obj][bt+4]&0xc)>>2; //признак выдачи звука
	zap_disk=fr1[obj][bt+4]&0x3; //признак записи на диск
	setlinestyle(0,0,0);
	if(fr3[obj][5]==1)//если объект непарафазен
	{
		setcolor(11);
		for(ii=0;ii<rad;ii++)circle(x,y,ii);
		return;
	}
	if(((fr3[obj][bt]==1)&&(invers==0))|| //если неинверсный бит установлен или
	((fr3[obj][bt]==0)&&(invers==1)))
	{
		if(tst_fix(obj,bt))//если не было фиксации
		{
			if(zap_disk==3)
			{
				if(get_str(obj,bt)==0)neisprav_na_disk('%',chudo);
				w(169,999,chudo);
			}
			if(zv_sig!=0)zvuk_vkl(0,0);

		}
		setcolor(cvt1);
		for(ii=0;ii<rad;ii++)circle(x,y,ii);
		return;
	}
	else //если неинверсный бит сброшен или инверсный установлен
	{
		setcolor(cvt0);
		for(ii=0;ii<rad;ii++)circle(x,y,ii);
		return;
	}
}
/**************************************************\
* процедура отработки действий по текстовой кнопке *
* ответственных команд (не разделка)               *
\**************************************************/
void ris_otv_knop1(int ob)
{
	unsigned int pred,//предварительный объект
	isp,//исполнительный объект
	b_p,//бит предварительного объекта
	b_i;//бит исполнительного объекта

	nom_func("287");
	pred=fr1[ob][2];//получить предварительный
	isp=fr1[ob][3];//получить исполнительный
	if(fr3[ob][5]==1)goto a;
	b_p=(fr1[ob][4]&0xff00)>>8;
	b_i=fr1[ob][4]&0xff;
a:
	if(fr3[pred][5]==1)goto final;
final:
	if(fr3[ob][5]==1)return;
	if((fr3[pred][b_p]==0)&&(zapusk==2))
	{
		if((STATUS==1)&&(tst_fix(pred,b_p)))
		{
			w(12,999,""); vtora=1;zapretvybora=1; zapusk++;
			fix(pred,b_p);
		}
	}
	else
	{
		fix(pred,b_p);
		fix(isp,b_i);
	}
}
