#include "bla.h"
#include "disk.h"
//---------------------------------------------------------------------
/*******************************************************************\
*    Основная процедура для организации работы с дисковым архивом   *
\*******************************************************************/
void Disk_arc()
{
	int fl,ohoho=0,iNt=0,ic,sddf,n_bait,ijk;
	char dob_str[10]="",alt;
#ifndef WORK
	int N_BAIT;
#endif

	nom_func("37");
	DISK=0xF;
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
#ifndef WORK
	//создание файла базы данных состояния объектов станции
	iNt=creat("dat\\fr3.bin",S_IWRITE|O_BINARY);

	//определение требуемого числа байт для хранения состояния объектов
	N_BAIT=kol_VO*13*2;

	//записть текущего состояния объектов станции на момент входа в архив
	write(iNt,fr3,N_BAIT);
	close(iNt);
#endif
	//удаление старых записей журналов-------------------
BB:
	iNt=remove("disk\\oblom1.fix");
	iNt=remove("disk\\oblom2.fix");
	iNt=remove("disk\\neispr1.fix");
	iNt=remove("disk\\neispr2.fix");
	iNt=remove("disk\\ogr.fr4");

	 //создание директория для архивных файлов (на случай, если его нет)
	iNt=mkdir("disk");

	for(iNt=0;iNt<kol_VO;iNt++)//сброс всех установленных ограничений
	{
		fr4[iNt][0]=0;
		fr4[iNt][1]=0;
		fr4[iNt][2]=0;
	}

	disco(); //запись файла со сброшенными ограничениями
	closegraph();
	//инициализация видеосистемы ------------------------
	GraphMode=VGAHI;
	errorcode = graphresult();
	initgraph(&GraphDriver,&GraphMode,"");
	errorcode = graphresult();
	GraphMode=VGAHI;
AA:

	regs.x.ax=0x1000;// функция установки палитры
	regs.h.bl=7;
	regs.h.bh=14;
	int86(0x10,&regs,&regs);

	setbkcolor(LIGHTGRAY);

	clscreen();
  USER_FONT= registerbgifont(small_font);
	settextstyle(USER_FONT,0,4);

  //загрузка консервативных данных
	fl=open("dat\\tranc.svs",O_RDONLY|O_BINARY);
	if(fl==-1)
	{
		clrscr(); moveto(80,80);
		outtext("Не открыт файл с исходными данными tranc.svs.");
		moveto(80,160);
		outtext("Для выхода из программы нажмите любую клавишу");
		getch();
		exit(1);
	}
	//считывание данных для имитации каналов обмена данными
	formula(fl);

	if(fl>0)close(fl); //закрытие файла

	fl=0;

	read_lex(); //инициализация текстовых констант

	flagoutmsg=0; //сброс индикатора команд

	perekluch=2;  //установить спец.значение признака переключения экранов

	otkaz_sos[0]=0; //сброс отказа ТС соседней ПЭВМ с ТУМС-1
	otkaz_sos[1]=0; //сброс отказа ТС соседней ПЭВМ с ТУМС-2
	pusto=0;        //сброс отказа каналов связи данной ПЭВМ

	for(iNt=0;iNt<kol_VO;iNt++)fr3[iNt][5]=0; //сброс непарафазностей

	//ввод времени,поиск точки входа
	// и "прокрутка" в памяти до точки входа
snova:
	sddf=starit();

	if(sddf<0) //если точка входа не найдена
	{
		sddf=0;
		FINAL();
		return;
	}

repit:  //точка повторного входа
	clscreen();
	for(iNt=0;iNt<kol_VO;iNt++)
	for(ic=0;ic<13;ic++)fr3[iNt][ic]=0; //сброс всех битов текущего состояния
	if((b_h1*60+b_m1)<=(bb_h*60+bb_mi))//если вышли на точку с меньшим временем
	{
		while((b_m1<bb_mi)||(b_h1<bb_h))//идти пока не достигнем заданной точки
		{
			if(read_bl_box()==0)goto fdf1;
			for(iNt=0;iNt<n_bait;iNt++){buf[0][iNt]=bu[0][iNt];buf[1][iNt]=bu[1][iNt];}
			work();
			proverka_svyazi();
		}

	}
	else
		{
povt: putch(7);
			setcolor(8);
			outtextxy(10,10,"На заданное время данных нет, выдать то, что есть ?(y/n)");
			alt=getch();
			if((alt=='n')||(alt=='N'))goto snova;
			else
			 if((alt!='y')&&(alt!='Y'))goto povt;
		}

povtorS:
	cikl_obnov=0;
	N_SOOB=0;
	ZAGRUZKA=0;
	clscreen();perekluch=1;
	nach_zagruzka();//прорисовка стартовой картинки
	menu();//вывод нижней части экрана с подсказками
	perekluch=0;
	for(ohoho=0;ohoho<20;ohoho++)data[ohoho]=0;
	first_time=biostime(0,0l); //получить текущее время
	tii=first_time;
	i3=-1;
met0:

	if((klo==0)&&(nikuda==0)&&(help==0))
	{
		regs.x.ax=0x1000;        //функция установки палитры
		regs.h.bl=7;
		regs.h.bh=14;
		int86(0x10,&regs,&regs);
		setbkcolor(7);
		setcolor(12);outtextxy(1,15,data);setcolor(4);outtextxy(2,16,data);
		if(pusto==0xff00)dr();
		if(pusto==1)for(ic=0;ic<kol_VO;ic++)fr3[ic][5]=1;
		prov_lX();
		tu_tc();
		kvadrat();
		knopka(KS1);
#ifdef KOL_SOO2
		knopka(KS2);
#endif
		xc=0;yc=0;
	}
kll:
	//если не был прочитан код отсутствия связи и просмотр любого журнала
	if((nikuda==1)||(klo==1)||(help==1))goto cikl;
	prodol1();
	if((klo==0)&&(nikuda==0)&&(help==0))
	{
		strcpy(data,"АРХИВ ");
		itoa(bb_d,dob_str,10);
		strcat(data,dob_str);
		strcat(data,"-");strcat(data,itoa(bb_m,dob_str,10));
		strcat(data,"-");strcat(data,itoa(bb_y,dob_str,10));
		setcolor(12);
		outtextxy(2,16,data);setcolor(4);
		outtextxy(1,15,data);
	}

	b_h0=b_h1;
	b_m0=b_m1;
	b_s0=b_s1;
	VIRT_TIME_0=biostime(0,0);
	VIRT_TIME_1=VIRT_TIME_0;
ssl:

	if(poshag==0)
	{
		if(test_virt_time())
		{
			VIRT_TIME_0=VIRT_TIME_1;
			b_s0=b_s0+1;
			if(b_s0==60)
			{
				b_s0=0;
				b_m0=b_m0+1;
				if(b_m0==60)
				{
					b_m0=0;
					b_h0=b_h0+1;
				}
				if(b_h0==24)
				{
					b_h0=0;
				}
			}
			out_time();//вывод текущего виртуального времени
		}
	}
	else
	{
		if(b_s1==15)
		b_s1=15;
		b_h0=b_h1;
		b_m0=b_m1;
		b_s0=b_s1;
		out_time();//вывод текущего виртуального времени
	}
	ho_ur=b_h0;
	mi_n=b_m0;
	se_c=b_s0;
	//bh_0,b_m0,b_s0 - текущее виртуальное время
	//bh_1,b_m1,b_s1 - архивное время последней считки
	//если виртуальное время подошло к архивному
	analiz_time();
	if(poshag==0)
	{
		while((b_h0*3600+b_m0*60+b_s0)>=(b_h1*3600+b_m1*60+b_s1))
		{
			ZAGRUZKA=1;
			ijk=PRINT_ALL(); //вывод на экран времени,стартового символа и текста сообщения
			if(ijk==1)goto povtorS;
			work(); //обработка архивной строки (команда или сообщение)
			proverka_svyazi();
fdf1:
			outportb(0x3c3,1); //включение изображения на экране
			opred_old=opred;
			//чтение следующей строки архива и вычисление задержки
			if(der()==-1){FINAL();goto AA;}//$$
			if((pusto&3)!=0)//если была потеря связи
			{ if(klo==1) kartina();
				else  if(nikuda==0&&help==0)  zagruzen();
				w(138,999,"");//ОТСУТСТВУЕТ СВЯЗЬ С МПСУ
				delay(2000);
			}
		}
		ZAGRUZKA=0;
	}
cikl:
	if(cikl_obnov>=kol_VO)cikl_obnov=0;
	if(N_SOOB>=KOL_SOO)
	{
		if((klo==0)&&(nikuda==0))
		{
			prorisovka=1;
			if(obnov<5)
			{
				switch(fr1[cikl_obnov][0])
				{
					case 1: displaystrelka(cikl_obnov,obnov,0);break;
					case 3: sekci(cikl_obnov,obnov);break;
					case 2: if(fr1[cikl_obnov][1]==13)komplekt(cikl_obnov,obnov);
									else  displaysignal(cikl_obnov,obnov);
									break;
					case 4: displaypathuch(cikl_obnov);break;
					case 5: displaypath(cikl_obnov,obnov);break;
					default:break;
				}
			}
			obnov++;
			if(obnov==5){obnov=0;cikl_obnov++;}
			prorisovka=0;
		}
	}
	if(kbhit()||(poshag==1))//если нажата клавиша
	{
		if(keykey()==-1){FINAL();return;}
		if(ismena_==1)//если изменена точка входа в архив
		{
			ismena_=0;
			FINAL();
			goto BB;
		}
		if(poshag==1)
		{
			if(klaval!=13)goto cikl;
			ijk=PRINT_ALL(); //вывод на экран времени,стартового символа и текста сообщения
			if(ijk==1)goto povtorS;
			work(); //обработка архивной строки (команда или сообщение)
			proverka_svyazi();
			outportb(0x3c3,1); //включение изображения на экране
			opred_old=opred;
			//чтение следующей строки архива и вычисление задержки
			if(der()==-1){FINAL();goto AA;}//$$
			if((pusto&3)!=0)//если была потеря связи
			{ if(klo==1) kartina();
				else  if(nikuda==0&&help==0)  zagruzen();
				w(138,999,"");//ОТСУТСТВУЕТ СВЯЗЬ С МПСУ
			}
		}
	}
	else
	{
		if(klo==1)
		card();
		else
		 if(nikuda==1)read_neisprav();
	}
	if((klo==0)&&(nikuda==0)&&(help==0))
	{
			tst_zvuk();
			goto ssl;
	}
fdf:
	opred=opred_new;
	b_h0=b_h1; b_m0=b_m1; b_s0=b_s1;
	for(iNt=0;iNt<n_bait;iNt++)buf[0][iNt]=bu[0][iNt];
fdf2:
	col=3;
	goto met0;
}
//----------------------------------------------------------------
int analiz(int gog)
{
	int jjj,i,sbros;

	nom_func("5");
	sbros=1;
	switch(gog)
	{ case 131: STATUS=1;slom_interf(7400);//"Г"
							if(perekluch!=2){ispr_pvm(0,0);ispr_pvm(0,NEISPRAVEN);}
							break;
		case 132: STATUS=0;slom_interf(7500);//"Д"
							if(perekluch!=2){ispr_pvm(0,0);ispr_pvm(0,NEISPRAVEN);}
							break;
		case 134: STATUS=2;slom_interf(7600); //Ж
							if(perekluch!=2){ispr_pvm(0,0);ispr_pvm(0,NEISPRAVEN);}
							break;
		case 'S': slom_interf(9000);
							w(174,999," ПЕРЕЗАГРУЗКА");
							pusto=0;
							for(i=0;i<kol_VO;i++)
							{
								fr3[i][0]=fr3[i][1]=fr3[i][2]=fr3[i][3]=fr3[i][4]=0;
								fr3[i][5]=1;
							}
							gog=0;
							cikl_obnov=0;
							return(1);

		case 142: vkl_kno(0,14);OTMENA=1;break; //О - кнопка отмена зеленая
		case 174: vkl_kno(0,8);OTMENA=0;break; //о - кнопка отмена серая

		case 135: vkl_kno(VSPZ,10);OGRAN=1;break; //З - кнопка ВСПЗ зеленая
		case 167: vkl_kno(VSPZ,8);OGRAN=0;break;  //з -кнопка ВСПЗ серая
#ifdef MAKET
		case 140: vkl_kno(MAKET,10);MAK=1;break; //М - кнопка МАКЕТ - зеленая
		case 172: vkl_kno(MAKET,8);MAK=0;break; //м - кнопка МАКЕТ - серая
#endif
		case 137:	vkl_kno(AViA,14);AVAR=1;break; // Й - кнопка авар.перевода желтая
		case 169: vkl_kno(AViA,8);AVAR=0;break;  //й - кнопка авар.перевода серая

		case 224:	vkl_kno(2,10);vkl_kno(1,8);    // р - кнопка раздельное серая
							uprav=1;break;                 //     маршрутное - зеленая

		case 144: vkl_kno(2,8);vkl_kno(1,10);    // р - кнопка раздельное зеленая
							uprav=0;break;                 //     маршрутное - серая

		case 175: otkaz_ts[0]=0;sbros=0;break;//"п"
		case 162: otkaz_ts[1]=0;sbros=0;break;//"в"
		case 15:  gog=gog;break;

		case 'A': otkaz_ts[0]=1;slom_interf(7000);pusto=pusto|1;
							for(jjj=0;jjj<kol_VO;jjj++)
							if(fr1[jjj][13]==1)fr3[jjj][5]=1;
							break;
		case 'a': otkaz_ts[1]=1;slom_interf(7001);pusto=pusto|2;
							for(jjj=0;jjj<kol_VO;jjj++)
							if(fr1[jjj][13]==1)fr3[jjj][5]=1;
							break;

		case 'B': otkaz_tu[0]=1;slom_interf(7010);break;
		case 'b': otkaz_tu[1]=1;slom_interf(7011);break;

		case 'C': zapret[0]=zapret[0]|0x8000;slom_interf(7020);break;
		case 'c': zapret[1]=zapret[1]|0x8000;slom_interf(7021);break;

		case 'E': otkaz_ts[0]=0;slom_interf(7050);pusto=pusto&0xfffe;break;
		case 'e': otkaz_ts[1]=0;slom_interf(7051);pusto=pusto&0xfffd;break;

		case 'F': otkaz_tu[0]=0;slom_interf(7060);break;
		case 'f': otkaz_tu[1]=0;slom_interf(7061);break;

		case 'D': zapret[0]=zapret[0]&0x7fff;slom_interf(7070);break;
		case 'd': zapret[1]=zapret[1]&0x7fff;slom_interf(7071);break;
		// отказ ТС соседней
		case 'Q': otkaz_sos[0]=1;slom_interf(7100);NEISPRAVEN=0;break;
		case 'q': otkaz_sos[1]=1;slom_interf(7101);NEISPRAVEN=0;break;
		// отказ ТУ соседней
		case 'R': slom_interf(7110);otkaz_tu_sos[0]=1;break;
		case 'r': slom_interf(7111);otkaz_tu_sos[1]=1;break;
		// восст ТС соседней */
		case 'U': slom_interf(7150);otkaz_sos[0]=0;break;
		case 'u': slom_interf(7151);otkaz_sos[1]=0;break;
		// восст ТУ соседней */
		case 'V': otkaz_tu_sos[0]=0;slom_interf(7160);break;
		case 'v': otkaz_tu_sos[1]=0;slom_interf(7161);break;

		case 'X':	slom_interf(7300);NEISPRAVEN=1;	break;
		case 'x': slom_interf(7200);NEISPRAVEN=0;break;

		case 'Y': otv_kom=1;knopka_OK();break;
		case 'y': otv_kom=0;knopka_OK();break;

		case 158: slom_interf(7701); break;//Ю
		case 238: slom_interf(7700); break;//ю

		default:  sbros=0;break;
	}
	if(sbros==1)for(i=0;i<12;i++)buf[0][i]=0;
	return(0);
}
//-------------------------------------------------------------
/**************************************************\
* Процедура определения типа очередного сообщения  *
* архива и вычисления реального и масштабного      *
* интервалов времени между текущим и следующим     *
* сообщениями                                      *
\**************************************************/
int der()
{
	int ii,n_bait;

	nom_func("35");
	b_u=box_uk; //запомнить хэндл для файла просматриваемого архива
	opred=read_bl_box(); //запомнить считанный символ начала информации
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	for(ii=0;ii<n_bait;ii++)
	{
		buf[0][ii]=bu[0][ii];
#if(skoko_stoek>1)
		buf[1][ii]=bu[1][ii];
#endif
	}
	if(opred==0) //если больше нет информации - закончить
	{ music1();
		ramka(15,15,51,18); moveto(16*7,16*7);
		outtext(" ДАННЫЕ ФАЙЛА АРХИВА ИСЧЕРПАНЫ! ");
		getch();
		return(-1);
	}
	if((opred!='>')&&(opred!='<')&&(opred!=0x2))flagoutmsg=0; //если сообщения
	else flagoutmsg=1;                          //если команды
	box_uk=b_u;
	return(0);
}
//-------------------------------------------------------------------
/*********************************\
* Процедура фиксации признака     *
* изменения точки входа в архив.  *                                            *
\*********************************/
void ismena()
{
	nom_func("111");
	ismena_=1; //формирование признака изменения точки входа в архив
	return;
}
//------------------------------------------------------------------------
/************************************************\
* Процедура обработки нажатия клавиш при шаговом *
* просмотре архивного файла системы.
\************************************************/
int keykey()
{
	int out;
	char dob_str[10]="";

	nom_func("119");
	klaval=getch();
	if(flag_paro!=0)//если выполняется считывание пароля
	{
		read_passw();
		return(0);
	}
  if(klaval==27)return(0); //если нажата клавиша Esc

  if(klaval==32) //если нажата клавиша пробел
	{
    //если работа в основном окне, то убрать строки сообщения с экрана
		if((nikuda==0)&&(help==0)&&(klo==0))t(0);
		okno_otkaz=0;
		return(0);
	}
vx:
	//если нажата управляющая клавиша
	if(klaval==0)
	{
		out=vyborklav();
		if((nikuda==0)&&(help==0)&&(klo==0))
		{
			strcpy(data,"АРХИВ ");
			itoa(bb_d,dob_str,10);
			strcat(data,dob_str);
			strcat(data,"-");strcat(data,itoa(bb_m,dob_str,10));
			strcat(data,"-");strcat(data,itoa(bb_y,dob_str,10));
			setcolor(12);
			outtextxy(2,16,data);setcolor(4);
			outtextxy(1,15,data);
		}
	}
  //если была нажата клавиша выхода из просмотра архива - конец
	if(out==-1)return(-1);
  //для других клавиш - продолжить
  return(0);
}
//------------------------------------------------------------------------
/*****************************************************\
* Процедура формирования выдачи музыкальной фразы при *
* исчерпании просматриваемых архивных записей         *
\*****************************************************/
void music1()
{
	int i;

	nom_func("176");
	for(i=0;i<4;i++)
	{
		sound(600);delay(50);
		nosound();sound(1000);
		delay(70);nosound();
	}
	return;
}
//--------------------------------------------------------------------
/***********************************************\
* Процедура поиска в архиве данных точки входа  *
* на указанное  в запросе оператора время.      *
\***********************************************/
int POISK()
{ long len_file,delta,delta0;
  int minut,ik;
  char tq[3]="",opr[10]="";

	nom_func("229");
	opred=0;
	len_file=filelength(box_uk);
	minut=bb_h*60+bb_mi-15;
	if(minut<0)minut=0;
	chas=minut/60;min=minut%60;delta0=len_file/2;delta=delta0;
	while(1)
	{ lseek(b_u,delta,0);
		while(opred!=13)read(b_u,&opred,1);
		read(b_u,&opred,1);
		for(ik=0;ik<10;ik++)opr[ik]=0;
		read(b_u,&opr,11);opred=0;
		for(ik=0;ik<2;ik++)tq[ik]=opr[ik];
    tq[ik]=0;chas=atoi(tq);
		for(ik=0;ik<2;ik++)tq[ik]=opr[ik+3];
    tq[ik]=0;min=atoi(tq);
    if(abs(chas*60+min-minut)<=1)
    { for(ik=0;ik<2;ik++)tq[ik]=opr[ik+7];
      tq[ik]=0;sek=atoi(tq);
			b_day=bb_d;b_mon=bb_m;
			b_h0=chas;b_m0=min;b_s0=sek;
      return(0);
    }
    delta0=delta0/2;
    if(chas*60+min<minut)delta=delta+delta0;
    if(chas*60+min>minut)delta=delta-delta0;
		if(delta0<2)
		{ delta=tell(b_u);
			if(((chas*60+min)<(minut+10))||(delta<40L))return(1);
			else return(2);//точка входа не найдена
		}
		strcpy(opr,"");
	}
}
//------------------------------------------------------------------
/*********************************************\
* Процедура вывода на экран архивного времени *
* и соответствующего сообщения из архива.     *
\*********************************************/
int PRINT_ALL(void)
{
	int gogi=0,i,i1,n_bait;
	char soobs[12]="",gog[2];

	nom_func("235");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	setfillstyle(1,cvv);setcolor(1);
	bar(35,26,105,34);
	bar(35,36,130,44);
	moveto(35,26);
	Printf(b_h1,b_m1,b_s1);//вывод на экран текущего архивного времени
//	ho_ur=b_h0;mi_n=b_m0;se_c=b_s0;//копирование времени для ведения журнала
	gog[0]=opred;gog[1]=0;
	moveto(35,36);
	if(gog[0]==0||gog[0]==13)gog[0]=32;
	outtext(gog);//вывод символа
	if((opred==175)||(opred==162)||(opred=='<')||(opred=='>')||(opred=='S')||
	(opred==15)||(opred=='=')||(opred==''))
	{
		soobs[n_bait]=0;
		if(opred=='=')
		flagoutmsg=1;
		for(i1=0;i1<n_bait;i1++)soobs[i1]=bu[0][i1];
		outtext(soobs);
	}
	if(gogi=='$')
	{moveto(5,26);outtext("ПЕРЕВОД ВРЕМЕНИ");delay(3000);}
	if(analiz(opred)==1)
	{ opred=0;
		return(1);
	}
	/*
	if(zapret[0]==1) zapret_TU(0);
	if(zapret[1]==1) zapret_TU(1);
	if(otkaz_tu[0]==1) otkaz_TU(0);
	if(otkaz_tu[1]==1) otkaz_TU(1);
	if(zapret[0]==0&&otkaz_tu[0]==0) razr_TU(0);
	if(zapret[1]==0&&otkaz_tu[1]==0) razr_TU(1);
	*/
	if(gogi=='S') return(1);
/*
	{
		if(fr4[kol_OSN][0]==1)
		{
			na=1;
			vkl_kno(NANA,10);
			for(i=0;i<N_str;i++)otkl_(N_strel[i],0);
		}
		else vkl_kno(NANA,8);
		if(fr4[kol_OSN][1]==1)
		{
			cha=1;
			vkl_kno(CHACHA,10);
			for(i=0;i<CH_str;i++)otkl_(C_strel[i],0);
		}
		else  vkl_kno(CHACHA,8);
	}*/
	if(klo==1)kartina();
	else
	{
		tu_tc();
		ispr_pvm(0,0);
		ispr_pvm(1,NEISPRAVEN);
	}
	return(0);
}
//----------------------------------------------------------------
void Printf(int g4,int g5,int g6)
{
	char AAq[5]="";

	nom_func("237");
	setcolor(1);
	itoa(g4,AAq,10);if(g4<10) outtext("0");outtext(AAq);outtext(":");
	itoa(g5,AAq,10);if(g5<10) outtext("0");outtext(AAq);outtext(":");
	itoa(g6,AAq,10);if(g6<10) outtext("0");outtext(AAq);outtext(" ");
	return;
}
//-----------------------------------------------------------------
/********************************************************\
* Процедура вывода на экран вида режима просмотра архива.*                     *
\********************************************************/
void prodol1()
{

	nom_func("240");
	if (vrem==5) uprav=1;
  setcolor(3);
  if(perekluch==1) poka=0;
  i3=-1;
  menu();
  setfillstyle(SOLID_FILL,cvv);
  setcolor(11);
  if(poshag==0)
	{bar(40,46,170,54); moveto(40,46); outtext("АВТОМАТИЧ. РЕЖИМ "); }
	else
	{bar(40,46,170,54); moveto(40,46);outtext("ПОШАГОВЫЙ  РЕЖИМ ");}
	return;
}
//---------------------------------------------------------------
/*******************************************\
* Процедура очистки прямоугольного участка  *
* экрана в заданных координатах и окантовки *
* этого участка рамкой двойных линий        *
\*******************************************/
void ramka(int x1,int y1,int x2,int y2)
{

	nom_func("260");

	setfillstyle(SOLID_FILL,8);
	bar(x1*7,y1*7,x2*7,y2*7);
	setcolor(WHITE);
	setlinestyle(0,0,0);
	rectangle(x1*7+2,y1*7+2,x2*7-2,y2*7-2);
	rectangle(x1*7+4,y1*7+4,x2*7-4,y2*7-4);
	if(x1<10)
	{
		setfillstyle(SOLID_FILL,8);
		bar(x1*7+30*7+10,y2*7-10,x1*7+30*7+20+115,y2*7);
		moveto(x1*7+30*7+20,y2*7-10);
		setcolor(11);
		outtext("'Esc' - выход");
	}
	return;
}
//--------------------------------------------------------------
/****************************************************\
* Процедура чтения строки записей черного ящика      *
\****************************************************/
int read_bl_box()
{
	int i,ik,n_bait;
	unsigned int ogr=0;
	unsigned char symbos,tq[3],opr[25];

	nom_func("265");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	read(b_u,&opred1,1);symbos=opred1;//прочитать один символ
oba:
	while(eof(b_u)!=1)//выполнять цикл, пока не достигнут конец файла
	{
povtor:
		//чтение времени
		if(symbos==13)
		{
			while(!isdigit(symbos))read(b_u,&symbos,1);
			lseek(b_u,-1,1);
			read(b_u,&opr,9);//если конец строки,прочитать 9 символов
		}
		else//если это не конец строки
		{ while(symbos!=13&&eof(b_u)==0)read(b_u,&symbos,1);//искать конец строки или конец файла
			while(!isdigit(symbos))read(b_u,&symbos,1);
			lseek(b_u,-1,1);
			read(b_u,&opr,9);//если конец строки,прочитать 9 символов
		}
		//чтение символа за временем
		read(b_u,&symbos,1);
		while(symbos==' ')read(b_u,&symbos,1);//поиск символа не пробела

		if(symbos==0x84){STATUS=0;goto povtor;} //'Д'
		if(symbos==0x83){STATUS=1;goto povtor;} //'Г'
		if((symbos=='>')||(symbos=='<'))
		flagoutmsg=1;
		else flagoutmsg=0;

		//перевести в целый формат параметры для времени
		if(opr[0]==32)for(ik=0;ik<8;ik++)opr[ik]=opr[ik+1];

		for(ik=0;ik<2;ik++)tq[ik]=opr[ik]; b_h1=atoi(tq);
		for(ik=0;ik<2;ik++)tq[ik]=opr[ik+3]; b_m1=atoi(tq);
		for(ik=0;ik<2;ik++)tq[ik]=opr[ik+6]; b_s1=atoi(tq);

		//анализ символа идентификатора
		//175="п" - информация из первой стойки
		//162="в" - информация из второй стойки
		//"<" - команда
		//">" - ограничение на объект
		//S - начало работы после перезагрузки программы
		if((symbos==175)||(symbos==162)||(symbos=='<')||(symbos=='>')||
		(symbos=='S')||(symbos=='=')||(symbos==''))
		{ read(b_u,&bu[0],n_bait);//чтение данных
			if(symbos!=0x2)
			{
				//если формат не правильный,то вернуться
				if(((bu[0][n_bait-1]!=')')&&(bu[0][n_bait-1]!='+'))&&
				((bu[0][n_bait-2]!=')')&&(bu[0][0]!='(')))
				goto povtor;
				if(symbos=='S')
				{ fr4[kol_OSN][0]=0; fr4[kol_OSN][1]=0; disco();
					na=0; cha=0;
#ifdef AVTOD
					vkl_kno(NANA,8);vkl_kno(CHACHA,8);
#endif
				}
//			pozic=lseek(b_u,0,1);//запомнить позицию
//далее чтение следующего времени
/*			read(b_u,&opr,2);
			read(b_u,&opr,8);
			//перевести в целый формат параметры для времени
			for(ik=0;ik<2;ik++)tq[ik]=opr[ik]; tq[ik]=0; b_h1=atoi(tq);
			for(ik=0;ik<2;ik++)tq[ik]=opr[ik+3]; tq[ik]=0; b_m1=atoi(tq);
			for(ik=0;ik<2;ik++)tq[ik]=opr[ik+6]; tq[ik]=0; b_s1=atoi(tq);
			lseek(b_u,pozic,0); //возврат на старую позицию
*/    }
			if(symbos==0x2)
			i=0;
			return(symbos);
		}
		else
			if(symbos==15)//если считан символ "$"
			{
				read(b_u,&opr,6);
				if(opr[2]=='#')
				return(symbos);
				i=((0xf0&(~opr[2]))>>4)*1000; //определение номера объекта
				i=i+(0xf&(~opr[2]))*100;
				i=i+((0xf0&(~opr[1]))>>4)*10;
				i=i+(0xf&(~opr[1]));

				fr4[i][0]=opr[3]&0x7f;
				if(fr1[i][0]==1)//если стрелка
				{
					if((fr4[i][0]&2)==2)
					{
						cha=1;
						vkl_kno(CHACHA,10);
					}
					if((fr4[i][0]&4)==4)
					{
						na=1;
						vkl_kno(NANA,10);
					}
				}
				ogr=(opr[4]&0x7f)<<8;
				ogr=ogr|(opr[3]&0x7f);
				if((ogr&1)==1)fr4[i][0]=fr4[i][0]|1;
				else fr4[i][0]=fr4[i][0]&0xfe;
				if((ogr&0x100)==0x100)fr4[i][2]=1;
				else fr4[i][2]=0;
				if((ogr&0x200)==0x200)fr4[i][1]=1;
				else fr4[i][1]=0;
			}
			else return(symbos);
	}
	return(0);//дошли до конца файла
}
//---------------------------------------------------------------------
/****************************************************\
* Процедура для ввода цифр числа, вводимого на экране*
* с позиции ik,ig и ограниченного значением Maxim    *
* Возвращает введенное число(при норме) или          *
* -1 - для ввода неправильных символов и Esc         *
* -2 - для ввода недопустимого кол-ва символов и для *
*      ввода числа большего, чем Maxim               *
* ik - координата X                                  *
* ig - координата Y                                  *
* cifr - число ожидаемых цифр                        *
* Maxim - максимальное значение для вводимого числа  *
\****************************************************/
int Scan(int ik,int ig,int cifr,int Maxim)
{ int I=0,iks=0,i;
	char sloo[8]="",sluo[8]="",ch[2]="";

	nom_func("295");
	I=0;iks=ik;moveto(iks,ig);
	while(ch[0]!=13)
	{ if(I>cifr) //если введено более заданного
		{
			setfillstyle(SOLID_FILL,8);
			bar(ik,ig,ik+cifr*8+8,ig+8);
			return(-1);
		}
		ch[0]=getch(); //прочитать символ с клавиатуры
		if(ch[0]==27)return(-2);//если нажата клавиша Esc

		if(ch[0]==0) //если нажата управляющая клавиша (расширенный код)
		{
			getch(); //прочитать символ приндалежности клавиши
			setfillstyle(SOLID_FILL,8); //стиль сплошной черный
			bar(ik,ig,ik+cifr*8+8,ig+8); //очистить строку
			return(-1);
		}
		if(ch[0]==13)  //если нажата Enter
		{
			sloo[I]='*';
			break;
		}
		if(ch[0]==8) //если нажата "Забой"
		{ if(I>0)
			{ iks=iks-8; //сместиться по горизонтали на 1 символ
				setfillstyle(SOLID_FILL,8); //установить стирание
				bar(iks,ig,iks+8,ig+8);    //стереть символ
				moveto(iks,ig);            //встать в позицию для ввода символа
				sloo[I]='*';
				I--; //уменьшить счетчик символов
			}
			goto enn;
		}
		if(!isdigit(ch[0])) //если введена не цифра
		{
			setfillstyle(SOLID_FILL,8);
			bar(ik,ig,ik+8*cifr+8,ig+8);
			return(-1);
		}
		moveto(iks,ig); //если введена цифра - изобразить ее
		outtext(ch);
		sloo[I]=ch[0];
		iks=iks+8;     //переместиться на следующий символ
		I++;           //увеличить счетчик символов
enn:;
	}
	if(I<0) //если стираем то, чего нет
	{
		setfillstyle(SOLID_FILL,8);
		bar(ik,ig,ik+8*cifr+8,ig+8);    //очистить весь ввод
		return(-1);
	}
	i=0;
	while(sloo[i]!='*')
	{
		sluo[i]=sloo[i];  //заполнить строку символами
		i++;
	}
	I=atol(sluo); //получить целое число для введенного значения
	if((I>Maxim)||(I<0))return(-2);//если слишком велико или менее нуля
	return(I); //вернуть полученне значение
}
//------------------------------------------------------
void spisok_prikazov()
{
	int bz1,bz2,bb,n,nn,nnn,i,spara=0,ura,uu,DOP,STR,UCH,PUT,SIG,ST;
	int tip=0,var_sig1,var_sig2,n_strel,naprav,TEK_STREL,i_pred;
	unsigned long POL_STREL;
	unsigned char kim,doba[40];;

	nom_func("316");
	/******** обычные сигналы ***************/
	if(opred==0x2)
	for(i=0;i<10;i++)buf[0][i]=buf[0][i+1];
	if(buf[0][1]=='Z')
	goto Z;
	kim=buf[0][1]&0x40;
	if(kim!=0x40) return;
	kim=buf[0][1]&0x02;
	if(kim!=2) kim=1;
signal:
	if((buf[0][2]=='E')||(buf[0][2]=='Q'))
	{
		if(buf[0][2]=='E')
		{
			if(kim==2){SIG=SIG2;ST=SIG1;SMI=STR2;}
			else{SIG=SIG1;ST=0;SMI=STR1;}
			for(i=SMI+ST;i<SIG+SMI+ST;i++)
			if(buf[0][3]==podgruppa[i-ST])nn=i-SMI;
			else if(i>=SIG+SMI+ST) return;
			for(i=4;i<=8;i++)
			if(buf[0][i]!=124&&buf[0][i]!='@')n=i-4;
			else if(i>8) return;
			nnn=spsig[nn][n];
		}
#if DOP1>0
		if(buf[0][2]=='Q')
		{
			if(kim==2){DOP=DOP2;ST=DOP1;SMI=STR2+SIG2;}
			else{DOP=DOP1;ST=0;SMI=STR1+SIG1;}
			for(i=SMI+ST;i<DOP+SMI+ST;i++)
			if(buf[0][3]==podgruppa[i-ST])nn=i-SMI;
			else if(i>=DOP+SMI+ST) return;
			for(i=4;i<=8;i++)
			if(buf[0][i]!=124&&buf[0][i]!='@')n=i-4;
			else if(i>8) return;
			nnn=spdop[nn][n];
		}
#endif

		if(fr1[nnn][13]==1)t_com[0]=biostime(0,0l);
		else t_com[1]=biostime(0,0l);
		if(nnn<kol_OSN&&fr1[nnn][8]!=666)
		{ if(buf[0][n+4]=='A'||buf[0][n+4]=='B')
			w(132,nnn,"");//выдана команда на открытие сигнала
			else
				if(buf[0][n+4]=='F'||buf[0][n+4]=='E')
				{
					vkl_kno(0,14);
					w(137,nnn,"");//выдана команда на отмену маршута от сигнала
				}
				else return;
			return;
		}
		else
		{
		/******** смена обычная ************/
			if(fr1[nnn][8]==666)
			{ if(buf[0][n+4]=='A'||buf[0][n+4]=='B')
				{w(2,999,otopri[fr1[nnn][10]]);}
				//ВЫДАНА КОМАНДА СОГЛАСИЯ НА ОТПРАВЛЕНИЕ
				return;
			}
			else
			{  /******** смена вспомогательная ************/
				 if(fr1[nnn][1]>=400&&fr1[nnn][1]<=600)
				 {  if(fr1[nnn][3]==1&&(buf[0][n+4]=='X'||buf[0][n+4]=='T'))
						{ if(buf[0][n+4]=='X')
							{
								strcpy(doba,"НА ПРИЕМ ");
								strcat(doba,otopri[fr1[nnn][11]]);
								w(11,999,doba);
							}
							else
							{
								strcpy(doba,"HA ОТПРАВЛ. ");
								strcat(doba,otopri[fr1[nnn][12]]);
								w(11,999,doba);
							}
							return;
						}
						if(fr1[nnn][3]==2)
						{ if(buf[0][n+4]=='X')
							{
								w(9,999,otopri[fr1[nnn][11]]);
							}
							else
								if(buf[0][n+4]=='T')
								{
									w(10,999,otopri[fr1[nnn][12]]);
								}
							return;
						}
					}
					else
					{ switch(fr1[nnn][1])
						{ case 17:  bz1=fr1[nnn][2]; /*rm*/
												bz2=fr1[nnn][7]; /*zm*/
												bb=fr1[bz1][11];
												if(buf[0][n+4]=='A')
												{
													if(fr3[bz1][bb]==0&&fr3[bz2][bb]==0) w(36,999,"");
													else w(31,999,"");
												}
												else
												{
													if(fr3[bz1][bb]==1&&fr3[bz2][bb]==1) w(30,999,"");
													else w(37,999,"");
												}
												return;

							case 53:
							case 163:
							case 155:
							case 156:
							case 157:
							case 158: knopa(nnn);break;
							case 16: /* любые кнопки */
												if(buf[0][n+4]=='A'||buf[0][n+4]=='B')
												{
													if(fr1[nnn][2]==33)
													{
														strcpy(doba,otopri[fr1[nnn][7]]);
														w(fr1[nnn][3],999,doba);
													}
													else
													 if(fr1[nnn][3]!=9999)w(fr1[nnn][3],999,"");
													 else w(fr1[nnn][4],999,"");
												}
												else
												{
													if(fr1[nnn][2]==33)
													{
														strcpy(doba,otopri[fr1[nnn][7]]);
														w(fr1[nnn][4],999,doba);
													}
													else w(fr1[nnn][4],999,"");
												}

												return;
							case 166: /* смена кнопка где есть один комплект */
												if(buf[0][n+4]=='A'||buf[0][n+4]=='B')
												{ if(fr3[fr1[nnn][2]][0]==1)w(2,999,otopri[fr1[nnn][3]]);
													else
														if(fr3[fr1[nnn][2]][1]==1)w(2,999,otopri[fr1[nnn][4]]);
												}
												return;
							case 66:  if(markery[fr1[nnn][12]][7]==333)
												{ if(buf[0][n+4]=='A') w(25,999,"");
													else w(24,999,"");
												}
												return;
							case 30:  if(buf[0][n+4]=='N'||buf[0][n+4]=='M')
												w(fr1[nnn][6],999,"");
												if(buf[0][n+4]=='T'||buf[0][n+4]=='S')
												w(11,nnn,"");
												return;

							case 31:  if(buf[0][n+4]=='T'||buf[0][n+4]=='S')
												w(fr1[nnn][6],999,"");
												return;
							default:  return;
						}
					}
				}
			}
		}
		//маршруты----------------------------------
		if(buf[0][2]=='a'||buf[0][2]=='b'||buf[0][2]=='i'||buf[0][2]=='j')
		{ if(kim==2){SIG=SIG2;ST=SIG1;SMI=STR2;}
			else {SIG=SIG1;ST=0;SMI=STR1;}
			n=buf[0][4]-64;
			for(i=SMI+ST;i<SIG+SMI+ST;i++)
			if(buf[0][3]==podgruppa[i-ST]) nnn=i-SMI;
			else if(i>=SIG+SMI+ST) return;
			nn=spsig[nnn][n];
			if(opred==0x2)
			{
				nach_marsh_oper=nn;
				n=buf[0][6]-64;
				for(i=SMI+ST;i<SIG+SMI+ST;i++)
				if(buf[0][5]==podgruppa[i-ST]) nnn=i-SMI;
				else if(i>=SIG+SMI+ST) return;
				nn=spsig[nnn][n];
				end_marsh_oper=nn;
				if(buf[0][2]=='a'||buf[0][2]=='i'){mane=1;mane1=1;}
				else {mane=0;mane1=0;}
				poka=1;
				var_sig1=fr1[nach_marsh_oper][5]&0xff;
				var_sig2=(fr1[nach_marsh_oper][5]&0xff00)>>8;
				half_marsh=0;
				half_marsh1=0;
				nach0_marsh_oper=0;
				if((var_sig1!=0)||(var_sig2!=0)) //если могут быть варианты
				{
					i=nach_marsh_oper;
					if(fr1[nach_marsh_oper][1]==1)naprav=1;
					else naprav=-1;
					n_strel=buf[0][7]&0xf;//число определяющих стрелок
					POL_STREL=buf[0][8]&0x3f;
					POL_STREL=POL_STREL|((buf[0][9]<<6)&0xfC0);
					TEK_STREL=0;
					while(1)
					{
						if(fr1[i][0]==1)//если стрелка
						{
							if(((fr1[i][7]==1)&&(naprav==1))||  //если стрелка определяющая
							((fr1[i][7]==0)&&(naprav==-1)))
							{
								if((POL_STREL&(1<<TEK_STREL))==0)//если стрелка нужна в плюсе
								{
								 i_pred=i;
								 i=i+naprav;
								}
								else
								{
									i_pred=i;
									i=fr1[i][2];
								}
								TEK_STREL=TEK_STREL+1;
								if(TEK_STREL>n_strel)break;
							}
							else
							{
								i_pred=i;
								i=i+naprav;
							}
						}
						else
							if(fr1[i][0]==6)
							{
								if((i_pred+naprav)!=i)
								{
									i_pred=i;
									i=i+naprav; //если пришли со стрелки
								}
								else
								{
									i_pred=i;
									i=fr1[i][1];
								}
							}
							else
							{
								if((i==var_sig1)||(i==var_sig2))
								{
									nach0_marsh_oper=i;
									break;
								}
								i_pred=i;
								i=i+naprav;
							}

					}
				}
				marsh1(0);
				marsh2(0);
				w(274,nach_marsh_oper,"");
				half_marsh=0;
				half_marsh1=0;
				nach0_marsh_oper=0;
				end_marsh_oper=0;
			}
			else
			{
				if((buf[0][2]=='a')||(buf[0][2]=='b'))
				w(132,nn,"-марш.");  //выдана команда на открытие сигнала
				else
				w(275,nn,"");  //выдана команда на перевод стрелок маршрута
				MYTHX_TEC[kim-1]=buf[0][7]&0xf0;
				pooo_com[nn]=biostime(0,0l);
				com_bit[nn]=buf[0][2]&3;//выделить номер бита
			}
			return;
		}
		/****** разделка + ГРИ *******************/
F:  if(buf[0][2]=='F')
		{ if(kim==2){UCH=UCH2;ST=UCH1;SMI=STR2+SIG2+DOP2;}
			else {UCH=UCH1;ST=0;SMI=STR1+SIG1+DOP1;}
			for(i=SMI+ST;i<UCH+SMI+ST;i++)
			if(buf[0][3]==podgruppa[i-ST]) nn=i-SMI;
			else  if(i>=UCH+SMI+ST) return;
			for(i=4;i<=8;i++)
				if(buf[0][i]!=124&&buf[0][i]!='@') n=i-4;
				else if(i>8) return;
			nnn=spspu[nn][n];
			if(buf[0][10]=='+')
			{ ura=buf[0][n+4]&0xC4;
				if(ura==0xC4)
					{fr4[nnn][2]=1;w(115,nnn,"");}
				else {fr4[nnn][2]=0;w(114,nnn,"");}
				displaypathuch(nnn);
				disco();
			}
			else
			{ if((markery[(fr1[nnn][5]&0xff00)>>8][7]&0xff)==100)//если это ГСП
				{ w(19,999,"");
					fr3[nnn][9]=9;
					return;
				}
				else
				if(fr1[nnn][1]==27)
				{ w(109,999,"");
					return;
				}
				else
				{ if((fr1[nnn][1]>200)||(fr1[nnn][2]>=200))
					{
						if(fr3[nnn][0]==0)w(11,nnn," НА ИСК. РАЗМ.");
						else w(18,nnn," ПО");
						fr3[nnn][9]=9;
						return;
					}
					else
					{
						if((buf[0][n+4]&0xC0)==0xC0)
						{
							ura=buf[0][n+4]&0x04;
							if(ura==0x04){fr4[nnn][2]=1;w(115,nnn,"");}
							else {fr4[nnn][2]=0;w(114,nnn,"");}
						}
						displaypathuch(nnn);
						disco();
					}
				}
			}
		}
		/********* стрелки *************************/
C:  if((buf[0][2]=='S')||(buf[0][2]=='C'))
		{ if(kim==2){STR=STR2;ST=STR1;SMI=0;}
			else {STR=STR1;ST=0;SMI=0;}
			for(i=SMI+ST;i<STR+SMI+ST;i++)
			if(buf[0][3]==podgruppa[i-ST]) nn=i;
			else if(i>=STR+SMI+ST) return;
			for(i=4;i<=8;i++)
			if(buf[0][i]!=124&&buf[0][i]!='@') n=i-4;
			else if(i>8) return;
			nnn=spstr[nn][n];
			if(fr1[nnn][12]!=9999)//если спаренная
			{
				TEST_SPAR=2;
				spara=poisk_ras(fr1[nnn][2],nnn,0,0);
			}
			if(buf[0][2]=='S')
			{
qwe:    ura=buf[0][n+4]&0x01;
				if(ura==1)
				{
					fr4[nnn][0]=1;//отключено от управления по ВСПЗ
					if(spara!=0)//если спаренная
					fr4[spara][0]=1;
				}
				else //если включено управление
				{
					fr4[nnn][0]=0;
					if(spara!=0)//если спаренная
					fr4[spara][0]=0;
				}
				ura=buf[0][n+4]&0x02;

				if(ura==0x02)
				{
					 fr4[nnn][1]=1;//макет
					if(spara!=0)//если спаренная
					fr4[spara][1]=1;
				}
				else
				{
					fr4[nnn][1]=0;
					if(spara!=0)//если спаренная
					fr4[spara][1]=0;
				}
				ura=buf[0][n+4]&0x04;
				if(ura==0x04)//закрыто для движения
				{
					if((buf[0][n+4]&0x08)==0)//если закрыта основная
					{
						fr4[nnn][2]=1; //закрыто для движения
					}
					else //закрыта спаренная
					{
						if(spara!=0)//если спаренная
						fr4[spara][2]=1;
					}
				}
				else
				{
					if((buf[0][n+4]&0x08)==0)//если открыта основная
					{
						fr4[nnn][2]=0; //открыта для движения
					}
					else //открыта спаренная
					{
						if(spara!=0)//если спаренная
						fr4[spara][2]=0;
					}
				}
				nach_zakr(nnn,1);
				if(spara!=0)nach_zakr(spara,1);
#ifdef MAKET
				vkl_kno(MAKET,8);
#endif
				disco();
			}
			else
			{ ura=buf[0][n+4]&0x01;
				if(ura==0x01){hop=3;w(123,nnn,"");hop=0;}
				else {hop=4;w(123,nnn,"");hop=0;}
			}
			return;
		}
I:  if(buf[0][2]=='I'&&buf[0][10]==')')
		{ if(kim==2){PUT=PUT2;ST=PUT1;SMI=STR2+SIG2+DOP2+UCH2;}
			else {PUT=PUT1;ST=0;SMI=STR1+SIG1+DOP1+UCH1;}
			for(i=SMI+ST;i<PUT+SMI+ST;i++)
			if(buf[0][3]==podgruppa[i-ST])nn=i-SMI;
			else
				if(i>=PUT+SMI+ST) return;
			for(i=4;i<=8;i++)
			if(buf[0][i]!=124&&buf[0][i]!='@')n=i-4;
			else
				if(i>8) return;
			nnn=spputi[nn][n];
			ura=buf[0][n+4]&0x04;
			if(ura==0x04){fr4[nnn][2]=1;w(115,nnn,"");}
			else {fr4[nnn][2]=0;w(114,nnn,"");}
			displaypath(nnn,0);
			disco();
		}
Z:
#ifdef AVTOD
		switch(buf[0][2])
		{

			case 'Y':

			case 'A': if(buf[0][3]=='@')
								{
									if(na==1)tip=4;//отключение нечетного
									else
										if(cha==1)tip=3;//отключение четного
										na=0;cha=0;
								}
								else
									if(buf[0][3]=='A')
									{
										if(na==0)tip=2;//включение нечетного
										else
											if(cha==1)tip=3;//отключение четного
										na=1;	cha=0;
									}
									else
										if(buf[0][3]=='B')
										{
											if(na==1)tip=4;//отключение нечетного
											else
												if(cha==0)tip=1;//включение четного
											na=0;	cha=1;
										}
										else
											if(buf[0][3]=='C')
											{
												if(na==0)tip=2;//включение нечетного
												else
													if(cha==0)tip=1;//вкллючение четного
												na=1;cha=1;
											}
								if(na==1) for(uu=0;uu<N_str;uu++)otkl_(N_strel[uu],1);
								else for(uu=0;uu<N_str;uu++)vkl_(N_strel[uu],1);
								if(cha==1) for(uu=0;uu<CH_str;uu++)otkl_(C_strel[uu],0);
								else for(uu=0;uu<CH_str;uu++)vkl_(C_strel[uu],0);

								if(buf[0][2]=='A')//включение
								{
									if(tip==1)w(90,999," ЧЕТНОЕ");//по 2 пути включение четного
									if(tip==2)w(91,999," НЕЧЕТНОЕ");//по 1 пути нечетное
								}
								if(buf[0][2]=='Y')//отключение
								{
									if(tip==3)w(265,999," ЧЕТНОЕ ПО 2П");
									if(tip==4)w(265,999," НЕЧЕТНОЕ ПО 1П");
								}
								if(na==1)vkl_kno(NANA,10);
								else vkl_kno(NANA,8);
								if(cha==1)vkl_kno(CHACHA,10);
								else vkl_kno(CHACHA,8);
								break;
				default:break;
		}
#endif
 L:
	if(buf[0][2]=='L')
	{ if(kim==2)
		{ if(buf[0][2]=='L'&&buf[0][3]=='K')
			if(buf[0][5]=='D') slom_interf(11201);
		}
		else
		{ if(buf[0][2]=='L'&&buf[0][3]=='V')
			if(buf[0][5]=='D') slom_interf(11200);
		}
		w(41,999,"");
	}
	return;
}
//-----------------------------------------------------------------------
/***************************************************\
* Процедура ввода информации о точке входа в архив  *
* и поиска точки входа в файле архива, а также      *
* ввода коэффициента ускорения просмотра данных     *
\***************************************************/
int starit()
{
	int xcx,d=0;
	char name_file[25],d_str[10];

	nom_func("317");
	for(d=0;d<25;d++)name_file[d]=0;
qwqw:
	dubl=0;cleardevice();ramka(1,9,79,28);
d2:
	if(d==-2)return(-2);
	moveto(20,80);
	outtext("Введите дату: число - ");
	d=Scan(170,80,2,31); //ввод номера дня, число не более 31

	if((d<1)||(d>31))//если введено неправильно,то пикнуть,стереть и повторить
	{
		if(d==-2)return(-2);
		putch(7);
		bar(170,80,190,90);
		d=0;
		goto d2;
	}
	bb_d=d; //запомнить введенный номер дня

d3:
	if(d==-2)return(-2); //если введено недопустимое - выйти

	moveto(20,88);
	outtext("              месяц - ");
	d=Scan(170,88,2,12); //ввод номера месяца, число не более 12
	if(d==-2)return(-2);
	if((d<1)||(d>12))//если введено неправильно,то пикнуть,стереть и повторить
	{
		putch(7);
		bar(170,88,200,98);
		d=0;
		goto d3;
	}
	bb_m=d;//запомнить введенный номер месяца

d4:
	if(d==-2)return(-2); //если введено недопустимое - выйти

	moveto(20,96);
	outtext("                год - ");
	d=Scan(170,96,4,3000); //ввод номера года, число не более 3000
	if(d==-2)return(-2);
	if(d<100)d=2000+d;   //если год менее 100, то исправить его

	if((d>2100)||(d<2000))  //если год за пределами, то повторить ввод
	{
		putch(7);
		bar(170,96,210,106);
		d=0;
		goto d4;
	}
	bb_y=d;//запомнить номер года

	//формирование имени файла архива
	strcpy(name_file,"result\\");
	itoa(bb_d,d_str,10);
	if(bb_d<10)strcat(name_file,"0");strcat(name_file,d_str);
	itoa(bb_m,d_str,10);
	if(bb_m<10)strcat(name_file,"0");strcat(name_file,d_str);
	strcat(name_file,".ogo");

again:
	box_uk=open(name_file,O_RDWR|O_BINARY); //попытка открытия файла
	na=_dos_getftime(box_uk,&mydate,&mytime);   //считывание даты создания файла
	mydate=(mydate>>9)+1980;                   //вычисление года файла

	//если год файла не совпадает, или архив не открылся, то повторить
	if((bb_y!=mydate)||box_uk<0)
	{
		moveto(20,165);setcolor(12);
		outtext("Информации на указанную дату нет, для продолжения нажмите <Enter>");
		if(getch()==27)return(-1);
		goto qwqw;
	}
d11:
	//переписать день,месяц и год
	dayy_=bb_d;
	monn_=bb_m;
	yearr_=bb_y;

dd:
	//ввесли коэффициент ускорения, не более 18
	moveto(20,104);
	outtext("Введите коэф.ускорения (1-10) ");
	moveto(230,104);outtext("10-максимум скорости,1-реальная");
	d=Scan(210,104,2,10);
	//если коэффициент ускорения неверный, то повторить
	if((d<1)||(d>18)){putch(7);bar(210,104,230,114);goto dd;}
	koef_uskor=d; //запомнить коэффициент ускорения


d5:
	moveto(20,112);outtext("Введите время: часы  - ");d=Scan(170,112,2,23);
	if(d<0){putch(7);bar(170,112,200,122);goto d5;}
	bb_h=d;
d6:
	if(d==-2)return(-2);
	moveto(20,120);outtext("             минуты - ");d=Scan(170,120,2,59);
	if(d<0){putch(7);bar(170,120,180,150);goto d6;}
	bb_mi=d;
  b_u=box_uk;
  if(POISK()>1)
  {
		putch(7);moveto(50,168); setcolor(LIGHTRED);
    outtext(" ДАННЫЕ НЕ НАЙДЕНЫ, ПОПРОБУЙТЕ ИЗМЕНИТЬ ВРЕМЯ, ЕЩЕ РАЗ? (y/n)");
wqwq:
    xcx=getch();
		if(xcx=='y'){if(box_uk>0)close(box_uk);box_uk=0;goto qwqw;}
    else
    {
      if(xcx=='n'){FINAL();return(-1);}
      else goto wqwq;
    }
  }
  else box_uk=b_u;
	read_bl_box();
	return(0);
}
//----------------------------------------------------------
int vyborklav()
{ int a;

	nom_func("406");
	if(klaval==0)
	{
		a=getch();
		t(0);
	}
	if(klaval!=0)return(0);
	if((nikuda==0)&&(klo==0)&&(help==0))
	{
a12:
		switch (a)
		{ case 68:  if(b_u>0)errorcode=close(b_u);
								b_u=0;
								FINAL();
								return(-1);;

			case 67:  if(podsvetka==0) podsvetka=1;//подсветка F9
								else podsvetka=0;
								pict_podsvetka();
								return(0);

			case 60 : return(0);//F2

hel:  case 61 : return(0);//F3

as:   case 62 : nosound();bq=0;bqq=0;return(0);//выключить звук F4

			case 63 : if((flag_paro==0)&&(nikuda==0)&&
								(klo==0)&&(help==0))//неисправности СЦБ F5
								{
									poshag=0;
									flag_paro=1;strcpy(password,"");
									w(118,999,"");//ВВЕДИТЕ ПАРОЛЬ
								}
								return(0);

as2:  case 64 : if((flag_paro==0)&&(nikuda==0)&&
								(klo==0)&&(help==0))//неисправности интерфейса F6
								{
									poshag=0;
									flag_paro=2;strcpy(password,"");
									w(118,999,"");//ВВЕДИТЕ ПАРОЛЬ
								}
								return(0);

shp:  case 65 : //изменение точки входа F7
								if((nikuda==1)||(klo==1))return(0);
								ismena();
								if (tst==2) uprav=1;
								else uprav=0;
								return(0);

shsh:  case 66: //переключение Пошаговый - Автоматический
								setfillstyle(SOLID_FILL,cvv);setcolor(11);
								if(poshag==1)
								{ poshag=0;bar(40,46,170,54);
									moveto(41,46);outtext("АВТОМАТИЧ. РЕЖИМ     ");
								}
								else
								{ poshag=1;bar(40,46,170,54);
									moveto(41,46);outtext("ПОШАГОВЫЙ  РЕЖИМ     ");
								}
								return(0);

			case 59 : return(0);

			default: return(0);
		}
	}
	else
	if(nikuda==1)
	{
//	 if(klaval!=0)return;
//		else klaval=getch();
		switch(a)
		{
			case  63: nikuda=0;
								if(tst==2)uprav=1;
								else uprav=0;
								if(n_1>0)close(n_1);n_1=0;
								if(n_2>0)close(n_2);n_2=0;
								end_help();
								sbros_all();
								BEG_TIM[0]=0;
								return(0);
			case  81: bil_bil=1;
								iqw=56;
								klaval=a;
								read_neisprav();
								return(0);
			case 73:	bil_bil=2;
								iqw=56;
								klaval=a;
								read_neisprav();
								return(0);
			default:  return(0);
		}
	}
	else
	if(klo==1)
	{
		switch (a)
		{
as3:  case 64 : klo=0;goga=0;
								if (tst==2) uprav=1;
								else uprav=0;
								perekluch=1;clscreen();setbkcolor(cvv);
								if(zabeg==0) zapretvybora=0;
								prorisovka=1;
								cikl_obnov=0;
							  zagruzen();
								prodol1();perekluch=0;klo=0;
								strcpy(data,"АРХИВ ");
								klaval=-1;
								return(0);
as4:  case 81 : bil_bil=1;
								klaval=a;
								card();
								return(0);

			case 73 : bil_bil=2;
								klaval=a;
								card();
								return(0);
			default:  return(0);
		}
	}
	return(-1);
}
//--------------------------------------------------------------
unsigned long int wait()
{
  unsigned long int t1=0l,t2=0l,tt=0l;

	nom_func("408");
	t1=(b_h1*3600l)+(b_m1*60l)+b_s1;
  t2=(b_h0*3600l)+(b_m0*60l)+b_s0;
	tt=(t1-t2)*18L;
  if(poshag==1)tt=0l;
  return(tt); // тиков между сообщениями
}
//-------------------------------------------------------------
void work()
{
	int ij,knl,n_bait;
	nom_func("423");

#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if((buf[0][1]=='E')&&(buf[0][2]=='Q')&&(buf[0][3]=='B'))
	ij=10;
	if(buf[0][0]==0)return;
	if((buf[0][1]&0x02)==2)knl=1;
	else knl=0;
	for(ij=0;ij<n_bait;ij++)bu[knl][ij]=buf[0][ij];
	if(flagoutmsg==0)
	{
		obrabotka_inf(knl);
		N_SOOB++;
		if(N_SOOB>=KOL_SOO)N_SOOB=KOL_SOO;
	}
	else
		{
			spisok_prikazov();
			flagoutmsg=0;
			for(ij=0;ij<n_bait;ij++)
			{
				bu[knl][ij]=0;
				buf[0][ij]=0;
			}
		}
	return;
}
//----------------------------------------------------------------------
/****************************************\
* Процедура завершения работы с архивом  *
* и возвращение к основной работе        *
\****************************************/
void FINAL()
{
#ifndef WORK
	int N_BAIT,hndl;
#endif                                          //из канала ТУМС

	nom_func("56");
	nosound();  								     //отключение звука
	if(box_uk>0)close(box_uk);       //закрытие файла архива
	if(b_u==box_uk)b_u=0;            //обнуление хэндла файла архива
	box_uk=0;
	if(file_soob>0)close(file_soob); //закрытие файла сообщений
	file_soob=0;
#ifndef WORK
	hndl=open("dat\\fr3.bin",S_IWRITE|O_BINARY);  //для отладочного вар-та
	N_BAIT=kol_VO*13*2;                           //восстановить состояние
	read(hndl,fr3,N_BAIT);                        //объектов, в рабочем
	close(hndl);                                  //варианте - закачка
#endif                                          //из канала ТУМС
	return;
}
//------------------------------------------------------------------------
/*********************************************************\
* Процедура отсчета виртуального времени просмотра архива,*
* возвращает 1, если прошла виртуальная секунда иначе 0.  *
\*********************************************************/
int test_virt_time(void)
{
	long delt;
	VIRT_TIME_1=biostime(0,0);
	delt=VIRT_TIME_1-VIRT_TIME_0;
	if((delt*koef_uskor)>18l)return(1);
	else return(0);
}

