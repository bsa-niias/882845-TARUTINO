#include "bla.h"
main()
{
	int a,b,iNt,ic,errorcode,ii,fl;
	int ik,test_baz[15],n_bait;
	long time_start=0l,time_stop=0l,kol_cikl=0l;
	struct dosdate_t dat;
	unsigned char reg_vga[10],aa,lon_str[15];
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif

	regs.x.ax=0; //убрать изображение курсора
	int86(0x33,&regs,&regs);
	if(regs.x.ax==0)MOUSE_ON=0;
	else MOUSE_ON=0xF;
	GraphMode=VGAHI;
	//driv=installuserdriver("SVGA",NULL); //$$
	errorcode = graphresult();
	//GraphMode=SVGA;//$$
	initgraph(&GraphDriver,&GraphMode,"");
	//initgraph(&driv,&GraphMode,""); //$$
	errorcode = graphresult();
	//ТЕСТИРОВАНИЕ РЕГИСТРОВ VGA
	/*
	aa=inportb(0x3cc);
	itoa(aa,reg_vga,2);
	puts(reg_vga);putch(13);
	aa=inportb(0x3ca);
	itoa(aa,reg_vga,2);
	puts(reg_vga);putch(13);
	puts("graf.kontr");putch(13);
	for(ik=0;ik<9;ik++)
	{
		outportb(0x3ce,ik);
		aa=inportb(0x3cf);
		itoa(aa,reg_vga,2);
		puts(reg_vga);putch(13);putch(13);
	}
	*/
	//  puts("CRT");putch(13);
	for(ik=0;ik<5;ik++)
	{
		outportb(0x3c4,ik);
		aa=inportb(0x3c5);
		itoa(aa,reg_vga,16);
		puts(reg_vga);putch(13);putch(13);
	}
	USER_FONT= registerbgifont(small_font);
	settextstyle(USER_FONT,0,4);
povtor:
	for(iNt=0;iNt<skoko_stoek;iNt++)
	{
		time_lu[iNt]=0L;time_lv[iNt]=0L;time_ly[iNt]=0L;
		time_lz[iNt]=0L;time_lc[iNt]=0L;
		for(ii=0;ii<=14;ii++){mas_lu[iNt][ii]=0;mas_lv[iNt][ii]=0;}
		for(ii=0;ii<=2;ii++){lu[iNt][ii]='@';lv[iNt][ii]='@';}

		for(ii=0;ii<=15;ii++)
		{
			mas_ly[iNt][ii]=0;
			mas_lz[iNt][ii]=0;
			mas_lc[iNt][ii]=0;
			mas_ls[iNt][ii]=0;
		}
		for(ii=0;ii<=3;ii++)
		{
			ly[iNt][ii]='@';
			lz[iNt][ii]='@';
			lc[iNt][ii]='@';
			ls[iNt][ii]='@';
		}
	}
//СОЗДАНИЕ КОНТРОЛЬНОГО ФАЙЛА БАЗЫ ДАННЫХ
	BAZA=creat("dat\\baza.bin",S_IWRITE|O_BINARY);
	for(ic=0;ic<kol_VO;ic++)
	{
		for(iNt=0;iNt<14;iNt++)
		{
			str_baz[iNt*2]=(fr1[ic][iNt]&0xff00)>>8;
			str_baz[iNt*2+1]=fr1[ic][iNt]&0xff;
		}
		ik=CalculateCRC8(fr1[ic],14);
		str_baz[28]=(ik&0xff00)>>8;
		str_baz[29]=ik&0xff;
		ii=_write(BAZA,&str_baz,32);
	}
	if(BAZA>0)
	ii=close(BAZA);
	BAZA=0;

	//  for(iNt=0;iNt<skoko_stoek;iNt++){otkaz_ts[iNt]=1;otkaz_tu[iNt]=1;}

	regs.x.ax=0x1000;// функция установки палитры
	regs.h.bl=7;
	regs.h.bh=7;
	int86(0x10,&regs,&regs);
	setbkcolor(7);
	clscreen();
	//  setusercharsize(1,3, 1, 3);
	//  setcolor(8);
	//  outtext("█АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЭЮЯабвгдежзиклмнопрстуфхцчшщэюя");
	setbkcolor(cvv);
	obnov=0,cikl_obnov=0;otl_soob=0xffff;
		//очистка буферов ввода-вывода для обмена с внешними устройствами
#ifdef DISPETCHER
	for(ic=0;ic<SIZE_BUF_OUT_DC;ic++)BUF_OUT_DC[ic]=0;
	for(ic=0;ic<SIZE_BUF_PRIEM_DC;ic++)BUF_IN_DC[ic]=0;
#endif
	for(ic=0;ic<SIZE_BUF_OUT;ic++){BUF_OUT[0][ic]=0;BUF_OUT[1][ic]=0;}
	for(ic=0;ic<SIZE_BUF_PRIEM;ic++){BUF_IN[0][ic]=0;BUF_IN[1][ic]=0;}
	for(ic=0;ic<MAX_SIZE_OUT;ic++)BUF_OUT_PVM[ic]=0;
	for(ic=0;ic<MAX_SIZE_IN;ic++)BUF_IN_PVM[ic]=0;
	for(ic=0;ic<skoko_stoek;ic++)
	{
//    otkaz_ts[ic]=0;
//    sboy_ts[ic]=0;
		otkaz_tu[ic]=0;
		sboy_tu[ic]=0;
		old_aoar[ic]=7;
    t_com[ic]=0;
	}
//  OBMEN_PVM=0xf;
  UKAZ_ZAP_OUT=0;
  UKAZ_VYVOD=0;
	UKAZ_PRIEMA_PVM=0;
  N_OGR=0;
	nosound();NEISPRAVEN=1;
  //получение даты
  _dos_getdate(&dat);
  //формирование текстового формата и получение имени суточного архива
	strcpy(NAME_FILE,"RESULT//");
	if(dat.day<10){NAME_FILE[8]=0x30;NAME_FILE[9]=dat.day|0x30;}
	else {NAME_FILE[8]=(dat.day/10)|0x30;NAME_FILE[9]=(dat.day%10)|0x30;}
	if(dat.month<10){NAME_FILE[10]=0x30;NAME_FILE[11]=dat.month|0x30;}
	else {NAME_FILE[10]=(dat.month/10)|0x30;NAME_FILE[11]=(dat.month%10)|0x30;}
	strcat(NAME_FILE,".ogo");
	NAME_FILE[16]=0;
	//открытие или создание файла суточного архива
	file_arc=open(NAME_FILE,O_APPEND|O_RDWR,O_BINARY);
	if(file_arc<0)
	{ iNt=mkdir("result");
		file_arc=open(NAME_FILE,O_CREAT|O_TRUNC|O_APPEND|O_RDWR,S_IWRITE|O_BINARY);
	}
	if(file_arc<0)
	{ clscreen(); moveto(100,100); setcolor(14);
		outtext("Нарушение файловой структуры,работа невозможна");
		getch();
		exit(1);
	}
	if(file_arc>0)
	ii=close(file_arc);
	file_arc=0;
	read_t(); //формирование строки текущего времени TIME
	fl=open("dat\\tranc.svs",O_RDONLY|O_BINARY,S_IWRITE | S_IREAD);
	if(fl==-1)
	{
		clrscr(); moveto(80,80);
		outtext("Не открыт файл с исходными данными tranc.svs.");
		moveto(80,160);
		outtext("Для выхода из программы нажмите любую клавишу");
		getch();
		exit(1);
	}
	formula(fl);//считывание данных для организации каналов обмена данными
	if(fl>0)close(fl);
	fl=0;
#ifdef SPDLP
	fai=fopen("dat\\spdlp.dat","r");
	if(fai==NULL)
	{
		clrscr(); moveto(80,80);
		outtext("Нет файла с данными для СПДЛП spdlp.dat.");
		moveto(80,160);
		outtext("Для выхода из программы нажмите любую клавишу");
		getch();
		exit(1);
	}
	read_spdlp(fai);
	if(fai!=NULL)fclose(fai);
	fai=NULL;
#endif
	read_lex();//конвертация или открытие файла текстовых сообщений системы
	start_time_date();//получение даты и времени для ведения архива
	FIR_time=biostime(0,0L);SEC_time=FIR_time+1;
	flagoutmsg=0;//сброс флага наличия не выданной команды
	cha=0;na=0;
	first_time=biostime(0,0L);tii=first_time;
	slom_interf(9000);//записать код начала работы в файл oblom#.fix
	if(n_bait==11)strcpy(bu,"( НАЧАЛО  )");
	else strcpy(bu,"(  НАЧАЛО  )");
	add(0,0,'S');//зафиксировать начало работы в .ogo
#ifndef WORK
		STATUS=1;//##
#else
#ifdef IMIT
		STATUS=1;//##
#endif
	ZZagruzka(); //получить данные из стоек по всем сообщениям
#endif
	if(file_arc>0)close(file_arc);//закрыть файл архива
	file_arc=0;
	for(iNt=0;iNt<(n_bait-2);iNt++){bu[0][iNt]=0;bu[1][iNt]=0;}//очистить буфера ТУМС
	prohod_1=1;
dubl:
	clscreen();//очистить экран
	uprav=1;tst=2;
#ifdef SETKA1
	setcolor(15);
	for(iNt=0;iNt<=640;iNt=iNt+10)
	{ gotoxy(iNt,0);
		if(iNt%40==0)outtextxy(iNt-8,0,itoa(iNt,lstr,10));
		line(iNt,0,iNt,470);
		if(iNt%40==0)outtextxy(iNt-8,470,itoa(iNt,lstr,10));
	}
	for(iNt=20;iNt<=480;iNt=iNt+10)
	{ gotoxy(iNt,0);
		if(iNt%40==0)outtextxy(0,iNt,itoa(iNt,lstr,10));
		line(24,iNt,610,iNt);
		if(iNt%40==0)outtextxy(610,iNt,itoa(iNt,lstr,10));
	}
#endif
	nach_zagruzka();//выполнить полную прорисовку экрана
	setfillstyle(SOLID_FILL,cvv);
	menu();//изобразить меню для выбранного режима
	i3=-1;
#ifdef WORK
		start_port(0);//инициировать порты,установить вектора прерываний
#endif
	ZAGRUZKA=0;ZONA_KURSORA=ZONA_N;
	home(-1);//перевести курсор в исходное
	regs.h.ah=5;//сброс клавиатуры
	regs.h.ch=0;
	regs.h.cl=0;
	int86(0x16,&regs,&regs);
#ifdef MOUSE
	if(MOUSE_ON!=0)mouse_init();//инициализация мыши
#endif
	FIR_time=biostime(0,0L);SEC_time=FIR_time+1;
	OBMEN_PVM_OLD=OBMEN_PVM=0x80;
	kom_v_bufer_pvm(0,OBMEN_PVM);
	cikl_obnov=0;obnov=0;
	pauza=0;
	ZERO_TRASSA(0);
	ZERO_TRASSA(1);
met0://---------------------начало основного цикла
//	errorcode = graphresult();
//	if(errorcode != 0)
//	nom_func=146;
#ifdef IMIT
//STATUS=1;//##
	TELEUP=1;//##
#endif
#ifndef WORK
//STATUS=STAT;//##
	TELEUP=1;//##
#endif
	/*  n_cikl++;*/
	STOROG=0;
/*
	if(kol_cikl==0)time_start=biostime(0,0);
	kol_cikl++;
	if(kol_cikl==10000)
	{
		time_stop=biostime(0,0);
		time_start=time_stop-time_start;
		ltoa(time_start,lon_str,10);
		setcolor(7);
		outtextxy(100,400,"██████████████");
		setcolor(12);
		outtextxy(100,400,lon_str);
		kol_cikl=0;
	}
*/
	tst_zvuk();//проверка включения или выключения звука
	if((klo==0)&&(nikuda==0)&&(help==0))what_is_new();//проверка наличия принятых данных из соседней ПЭВМ
	//инициализация таймера-стророжа на выдержку 3 сек
	//outportb(0x443,3);
	pauza++;
	if((pauza<2)||(menu_N!=0))goto mtk;
	pauza=0;
	if(cikl_obnov>=kol_VO)cikl_obnov=0;
	if((obnov==0)&&(klo==0));
	{
		BAZA=open("dat\\baza.bin",O_RDONLY|O_BINARY);
		if(BAZA==-1)
		{
			w(44,999," База не открывается");
			getch();
			exit(0);
		}
		lseek(BAZA,32*cikl_obnov,0);
		read(BAZA,str_baz,32);
		for(ik=0;ik<15;ik++)
		{
			test_baz[ik]=(str_baz[ik*2]<<8)+str_baz[ik*2+1];
			if((test_baz[ik]!=fr1[cikl_obnov][ik])&&(ik!=14))
			{
				w(44,cikl_obnov," 6");
				getch();
			}
		}
		if(BAZA>0)close(BAZA);
		BAZA=0;
	}
	if((klo!=0)||(nikuda!=0)||(help!=0))goto mtk;
	prorisovka=1;
	if((markery[i3][6]==cikl_obnov)&&(i3!=0))goto out;
/*===================================================================
	itoa(cikl_obnov,lstr,10);
	setcolor(7);
	outtextxy(600,5,"████");
	setcolor(8);
	outtextxy(600,5,lstr);
=====================================================================*/
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
out:
	obnov++;
/* $$
	for(ic=0;ic<6;ic++)
	{
		setcolor(7);
		outtextxy(390,280+10*ic,"████");
		setcolor(8);
		outtextxy(390,280+10*ic,pako[MARSHRUT_ALL[ic].NACH]);
	}
	for(ic=0;ic<3;ic++)
	{
		setcolor(7);
		outtextxy(430,280+10*ic,"██████████████");
		setcolor(8);
		outtextxy(440,280+10*ic,MARSHRUT_ST[0][ic].NEXT_KOM);
		if(TUMS_RABOT[0]!=0)setcolor(12);
		else setcolor(10);
		outtextxy(430,280,"█");
	}
*/
/*
	if((ic==0)||(ic==7))
	ic=getpixel(613,128);
	if(ic==4)
	ic=0;

	else
		pauza=0;
	if((ic==0)||(ic==7))ic=getpixel(109,178	);
	else
		pauza=0;
	if((ic==0)||(ic==7))ic=getpixel(109,179);
	else
		pauza=0;
	if((ic!=0)&&(ic!=7))
	*/
	pauza=0;

	if(obnov==5)
	{
		obnov=0;
		cikl_obnov++;
#ifndef WORK
	time20sec(cikl_obnov);
#endif
	}
mtk:

	prorisovka=0;
	for(ik=1;ik<=skoko_stoek;ik++)
	{
		for(iNt=0;iNt<kol_VO;iNt++)
		if((pooo_com[iNt]!=0l)&&(fr1[iNt][13]==ik))break;

		if((iNt==kol_VO)&&(t_com[ik-1]<=biostime(0,0l))&&(t_com[ik-1]!=0))
		t_com[ik-1]=0l;
	}
	iNt=test_time1(18L);//проверка прохождения одной секунды
	if(iNt>0) //если прошла одна секунда
	{
 /*
		itoa(STOROG,reg_vga,10);
		setcolor(7);
		outtextxy(5,250,"████████");
		setcolor(15);
		outtextxy(5,250,reg_vga);
*/
		TIME_ERROR_BBKP++;
		if(TIME_ERROR_BBKP>10)
		{
			if(ERROR_BBKP_0!=0)
			{
				if(ERROR_BBKP_0>99)ERROR_BBKP_0=99;
				slom_interf(6000+ERROR_BBKP_0);
				ERROR_BBKP_0=0;
			}
			if(ERROR_BBKP_1!=0)
			{
				if(ERROR_BBKP_1>99)ERROR_BBKP_1=99;
				slom_interf(6100+ERROR_BBKP_1);
				ERROR_BBKP_1=0;
			}
			if(ERROR_BBKP_2!=0)
			{

				if(ERROR_BBKP_2>99)ERROR_BBKP_2=99;
				slom_interf(6200+ERROR_BBKP_2);
				ERROR_BBKP_2=0;

			}

			if(ERROR_TUMS1_0!=0)
			{
				if(ERROR_TUMS1_0>99)ERROR_TUMS1_0=99;
				slom_interf(1000+ERROR_TUMS1_0);
				ERROR_TUMS1_0=0;
			}

			if(ERROR_TUMS1_1!=0)
			{
				if(ERROR_TUMS1_1>99)ERROR_TUMS1_1=99;
				slom_interf(1100+ERROR_TUMS1_1);
				ERROR_TUMS1_1=0;
			}

			if(ERROR_TUMS1_2!=0)
			{
				if(ERROR_TUMS1_2>99)ERROR_TUMS1_2=99;
				slom_interf(1200+ERROR_TUMS1_2);
				ERROR_TUMS1_2=0;
			}

			if(ERROR_TUMS2_0!=0)
			{
				if(ERROR_TUMS2_0>99)ERROR_TUMS2_0=99;
				slom_interf(2000+ERROR_TUMS2_0);
				ERROR_TUMS2_0=0;
			}
			if(ERROR_TUMS2_1!=0)
			{
				if(ERROR_TUMS2_1>99)ERROR_TUMS2_1=99;
				slom_interf(2100+ERROR_TUMS2_1);
				ERROR_TUMS2_1=0;
			}
			if(ERROR_TUMS2_2!=0)
			{
				if(ERROR_TUMS2_2>99)ERROR_TUMS2_2=99;
				slom_interf(2200+ERROR_TUMS2_2);
				ERROR_TUMS2_2=0;
			}
			TIME_ERROR_BBKP=0;
		}
		Analiz_Glob_Marsh();//Распределить глобальные маршруты по стойкам
		MARSH_V_TUMS(0);
		MARSH_V_TUMS(1);
		MARSH_GLOB_LOCAL();
#ifdef WORK
		proverka_svyazi();//установка переменных контроля каналов обмена
		if(OBMEN_PVM!=OBMEN_PVM_OLD)
		kom_v_bufer_pvm(0,OBMEN_PVM);
		OBMEN_PVM_OLD=OBMEN_PVM;
#endif
		cikl_avto++;
		if(cikl_avto>5)cikl_avto=0;
		if(klo==1){kartina();goto dalee;}// если просмотр неисправностей УВК
#ifdef DOS
		if(peek(0x0040,0x001A)==peek(0x0040,0x001C)//если очередь клавиатуры пуста
		{
			ScLk=peek(0x0040,0x0017);//прочитать состояние индикаторов клавиатуры
			if((ScLk&16)==16)ScLk=ScLk&0xFFEF;//проинвертировать бит для ScLk;
			else ScLk=ScLk|16;
			poke(0x0040,0x0017,ScLk);
			while((inportb(0x64)&2)!=0);
			outportb(0x60,0xED);
			while((inportb(0x64)&1)==0);
			if(inportb(0x60)!=0xFA){w(234,999,"");sound(1200);}
			while((inportb(0x64)&2)!=0);
			outportb(0x60,((ScLk>>4)&7));
		}
#endif
#ifndef NEW_KNOPKA
		for(ii=0;ii<18;ii++)
		{ //если чисто экранная кнопка или кнопка вкл. автодействия
			if((markery[ii][6]==9999)||(markery[ii][7]>776))
			{
				if(ii==MAKET)continue;
				ic=getpixel(markery[ii][4],markery[ii][5]);
				a=getpixel(markery[ii][4]+1,markery[ii][5]+1);
				b=getpixel(markery[ii][4]-1,markery[ii][5]-1);
				if((a!=b)||(a!=ic))
				{
					if(b==ic)vkl_kno(ii,b);
					if(a==ic)vkl_kno(ii,a);
					if(a==b)vkl_kno(ii,a);
				}
				else vkl_kno(ii,a);
			}
		}
#else
		if(cha==1)vkl_kno(CHACHA,10);
		else vkl_kno(CHACHA,8);

		if(na==1)vkl_kno(NANA,10);
		else vkl_kno(NANA,8);

		if(uprav==1){vkl_kno(1,8);vkl_kno(2,10);}
		else {vkl_kno(1,10);vkl_kno(2,8);}

		if(ot==1)vkl_kno(0,14);
		else vkl_kno(0,8);

		if(avap==1)vkl_kno(AViA,14);
		else vkl_kno(AViA,8);

		if(vspz==1)vkl_kno(VSPZ,10);
		else vkl_kno(VSPZ,8);


#endif

		if((vih_vopr==0)&&(vibeg==0)&&(vozvr==0)&&(menu_N==0))
		{
			if(otl_soob!=0xffff)
			{
				t(0);
				w(otl_soob,999,"");
				otl_soob=0xffff;
			}
		}
dalee:
#ifdef AVTOD
		if((STATUS==1)&&(TELEUP==1)&&(cikl_avto==5))
		{
			for(ii=0;ii<4;ii++)
			if(avto_marsh(ii))//если маршрут можно устанавливать
			{
				zad_marsh_avto(ii); 	//задать маршрут автодействия
				Avto_el[ii].N_raz++; //увеличить счетчик числа выданных команд
				//Выдать на экран "Не установлен маршрут от сигнала...
				if(Avto_el[ii].N_raz==8)
				{
					zvuk_vkl(3,0);
					w(21,Avto_el[ii].Beg_s," Автод.");
				}
				if(Avto_el[ii].N_raz>9) //если выдавалось более 9 раз
				{
					if(ii<=1)snyato_avto(1); //если маршрут нечетного автодействия
					else snyato_avto(2); //если маршрут четного автодействия
					Avto_el[ii].N_raz=0;
					Avto_el[ii].Flag=0;
				}
			}
			else //если для маршрута есть препятствия
			{
				if(Avto_el[ii].N_raz>3)Avto_el[ii].N_raz=0;
			}
		}
		if(TELEUP==0)
		{
			if(cha==1)snyato_chet();
			if(na==1)snyato_nechet();
		}
#endif
//    setcolor(7);
//    outtextxy(100,10,"████");
//    setcolor(8);
//    itoa(trassa[0],lstr,10);
//    outtextxy(100,10,lstr);
#ifdef SPDLP
		ZAP_BUF_SPD();
#endif
		ANALIZ_KOM_TUMS();

#ifdef WORK
		pust_pvm++;//##
#endif
		//изображение своей ПЭВМ
		ispr_pvm(0,0);
		//изображение ПЭВМ соседа (NEISPRAVEN -признак отказа канала ПЭВМ-ПЭВМ)
		ispr_pvm(1,NEISPRAVEN);
		out_time();
#ifdef WORK
		for(a=0;a<skoko_stoek;a++){sboy_ts[a]++;sboy_tu[a]++;}
#endif
		if(flag_paro==0)
		{
			if((pusto&3)!=0)w(138,999,"");//ОТСУТСТВУЕТ СВЯЗЬ С МПСУ#
			else
				for(ii=0;ii<skoko_stoek;ii++)
				if(otkaz_ts[0]==1)w(144+ii,999,"");//отсутствует связь с МПСУ ii
		}
		if((nikuda==0)&&(klo==0)&&(help==0))
		analiz_time();
    FIR_time=SEC_time;
	}
	ic=0;
  for(ii=0;ii<skoko_stoek;ii++)
  {
    if((otkaz_ts[ii]==0)&&(sboy_ts[ii]<2))ic++;
  }
	if(ic==skoko_stoek) //если есть связь по обоим стойкам
	{
		if(pusto==0xff00) //если была нарушена связь,то перезагрузить станцию
		{

			clscreen();
			pusto=0;
//			inportb(0x43);
			if(file_soob>0)close(file_soob);
			file_soob=0;
//			goto povtor;
			reset_arm();
		}
	}
#ifdef SPDLP
	if(STATUS==1)OUT_SPD();
#endif
  tu_tc();
	consentr();
	FORM_BUF_PVM_OUT();
  test_otvetstv();
#ifdef WORK
  test_port_pvm();//##
#endif
#ifdef MOUSE
  if((MOUSE_ON!=0)&&(STATUS==1))
  {
		if((help!=0)||(klo!=0)||(nikuda!=0))goto klava;
		if((STATUS==1)&&(DU==0)&&(TELEUP==1))
    {
			modi_new=mouse();//найти новый объект
			if(menu_N!=0)goto move;
      if((modi_new<0)&&(perezap==1))//если ушли с ранее выбранного объекта
      { mouser=0; if(i3!=-1)del_kursor(i3);
        regs.x.ax=0x1A;//повышаем чувствительность
        regs.x.bx=70;regs.x.cx=70;//по горизонтали и вертикали - 100
        int86(0x33,&regs,&regs);
        obnov_kur_mouse(X_m,Y_m);
				home(old_modi);//восстановить старый вид имени
        setcolor(7);outtextxy(37,46,"██████████");
        perezap=2;
			}
      if((modi_new>=0)&&(perezap==0))//если вновь выделен объект
      {
        clear_mouse();//восстановить фон под последним местом курсора
				for(ik=0;ik<12;ik++)line_gor_kurs[ik]=0;//##
        for(ik=0;ik<40;ik++)line_vert_kurs[ik]=0;//##
        regs.x.ax=0x1A;//снижаем чувствительность
				regs.x.bx=50;regs.x.cx=50;//по горизонтали и вертикали - 2
				int86(0x33,&regs,&regs);
				if(i3>=0)del_kursor(i3);//удалить клавиатурный курсор со старой позиции
				if((old_modi>=0)&&(sbros1==0))del_kursor(old_modi);
				perezap=1;//вывесить флаг захвата объекта
				i3=modi_new;//свести все указатели вместе
				pat=i3;modi=i3;
				old_modi=modi_new;
				show_kursor(i3);//установить клавиатурный курсор на новую позицию
			}
			if(modi_new<0)mouser=0;
move:
			move_mouse();
			if(sbros1==1){t(0);sbros1=0;}
		}
	}
#endif
klava:
/*
	a=peek(0x0040,0x001e);
	if(a==27)
	{
		if(PRESS==a)
		{
				goto met0;
		}
	}
	PRESS=peek(0x0040,0x001e);
	*/
	if(kbhit())
	{ STACK=_SP;
		keyboard();
		STACK=_SP;
	}
	else
	{
		if(klo==1)
		{
			if(STATUS==1)
			{
				klo=0;
				goto dubl;
			}
			card();
		}
		 else
			if(nikuda==1)
			{
				if(STATUS==1)
				{
					nikuda=0;
					goto dubl;
				}
				read_neisprav();
			}
	}
#ifdef SETKA
	setcolor(15);
	for(iNt=0;iNt<=640;iNt=iNt+10)
	{ gotoxy(iNt,0);
		if(iNt%40==0)outtextxy(iNt-8,0,itoa(iNt,lstr,10));
		line(iNt,0,iNt,470);
		if(iNt%40==0)outtextxy(iNt-8,470,itoa(iNt,lstr,10));
	}
	for(iNt=20;iNt<=480;iNt=iNt+20)
	{ gotoxy(iNt,0);
		if(iNt%40==0)outtextxy(0,iNt,itoa(iNt,lstr,10));
		line(24,iNt,610,iNt);
		if(iNt%40==0)outtextxy(610,iNt,itoa(iNt,lstr,10));
	}
#endif
	prov_lX();
m1:
	if(DISK==0xf)
	{
		if(file_soob>0)close(file_soob);
		file_soob=0;
		Disk_arc();
		DISK=0;
		STATUS=2;
		NOMER_ARMA=3;
		if(file_soob>0)close(file_soob);
		file_soob=0;
		goto povtor;
	}
	else
	{
		if(pusto==0xff00)
		{
			pusto=0;
			clscreen();
//			inportb(0x43);
			if(file_soob>0)close(file_soob);
			file_soob=0;
//			goto povtor;
			reset_arm();
		}
		goto met0;
	}

	/*
	if(cikl<80000)goto met0;*/
} /* конец main */
//-----------------------------------------------------------------------
/***********************************************************\
* Процедура подготовки массива TEST_SOOB к контролю приема  *
* полноты принятых сообщений, т.е. соответствующие биты в   *
* этом масиве установить в 1,а биты сверх нормы сбросить.   *
* Размерность TEST_SOOB[Nst][7] - на каждую стойку 7 байт   *
* по 8 бит - всег 56 бит, что дает контроль получения 56-ти *
* сообщений.                                                *
\***********************************************************/
void Init_TEST_SOOB(int K)
{
  int N_bait,N_bit,N_SOOB,i;

	nom_func("108");
	if((K==1)&&(skoko_stoek==1))return;
  if(K==0)N_SOOB=KOL_SOO1;
#ifdef KOL_SOO2
  if(K==1)N_SOOB=KOL_SOO2;
#endif
  for(i=0;i<=6;i++)TEST_SOOB[K][i]=0xFF; //установить все биты

  // Пройти по лишним битам, для которых нет сообщений
  for(N_bait=N_SOOB/8;N_bait<7;N_bait++)
  {
		//если это последний байт, связанный с сообщениями
    if(N_bait==N_SOOB/8)
    //то пройти по битам не участвующим в сообщениях и сбросить в 0
    for(N_bit=N_SOOB%8;N_bit<8;N_bit++)
    TEST_SOOB[K][N_bait]=TEST_SOOB[K][N_bait]^(1<<N_bit);
    //для остальных байтов сверх требуемых установить нули
		else TEST_SOOB[K][N_bait]=0;
  }
  return;
}
/****************************************************\
* Процедура сброса тестового бита, контролирующего   *
* прием соответствующего сообщения.                  *
* K-номер стойки ТУМС, от которой принято сообщение. *
* N_SOOB - код номера сообщения.                     *
\****************************************************/
void SBROS_BIT_TEST_SOOB(int K,int N_SOOB)
{
  int N_bait,N_bit;

	nom_func("291");
	//если для второй стойки, а стоек только одна, то выйти сразу
  if((K==1)&&(skoko_stoek==1))return;

  N_SOOB=N_SOOB-48; //вычленить номер сообщения
	N_bait=N_SOOB/8;  //получить номер байта контроля приема
  N_bit=N_SOOB%8;   //получить номер бита контроля сообщения
  if(ZAGRUZKA==1)   //если выполняется загрузка данных
  {
    setcolor(1);    //цвет синий
    outtextxy(10*(N_SOOB+1)+1,305+K*20,"█"); //заполнить квадратик приема
  }
  //сброс бита, соответствующего принятому сообщению
  //для этого бита получается выражения 1 ^ 1 = 0
  TEST_SOOB[K][N_bait]=TEST_SOOB[K][N_bait]&((0x1<<N_bit)^0xFF);
  return;
}
/********************************************************/

int TEST_BIT_SOOB(int K, int N_SOOB)
{
  int N_bait,N_bit;

	nom_func("337");
	if((K==1)&&(skoko_stoek==1))return(-1);
  N_SOOB=N_SOOB-48;
  N_bait=N_SOOB/8;
  N_bit=N_SOOB%8;
  return(TEST_SOOB[K][N_bait]&(1<<N_bit));
}
/**************************************************************/
int TEST_SUM_SOOB()
{
  int sum,i;

	nom_func("353");
	sum=0;
  for(i=0;i<=6;i++)sum=sum+TEST_SOOB[0][i];
#ifdef KOL_SOO2
  for(i=0;i<=6;i++)sum=sum+TEST_SOOB[1][i];
#endif
  return(sum);

}

#ifdef MOUSE
//--------------------------------------------------------------------
/**********************************************************\
* Процедура инициализации мыши и переинициализации мыши    *
\**********************************************************/
void mouse_init()
{
  union REGS regs;
//  regs.x.ax=0; // установить драйвер мыши в исходное
//  int86(0x33,&regs,&regs);
//  ***********************************
  int ik;
  unsigned char kur[64];
  struct SREGS segregs;

	nom_func("174");
	if(MOUSE_ON==0)return;
	for(ik=0;ik<64;ik++)kur[ik]=0;

  regs.x.ax=9; // задать пустое изображение для курсора
  regs.x.bx=0;
  regs.x.cx=0;

  segregs.es = FP_SEG(kur);
  regs.x.dx = FP_OFF(kur);
  int86x(0x33, &regs, &regs, &segregs);

  regs.x.ax=2; //убрать изображение курсора
  int86(0x33,&regs,&regs);

  regs.x.ax=7; // задать пределы движения по горизонтали
  regs.x.cx=X_MIN;
  regs.x.dx=X_MAX;
  int86(0x33,&regs,&regs);
//  *************************************
  regs.x.ax=8;// задать пределы движения по вертикали
  regs.x.cx=Y_MIN;
  regs.x.dx=Y_MAX;
  int86(0x33,&regs,&regs);
//  *******************************************
  regs.x.cx=X_m; //установить мышь в исходное положение
  regs.x.dx=Y_m;
  regs.x.ax=4;
  int86(0x33,&regs,&regs);
//  *****************************************************
  setlinestyle(0,0,0);setcolor(15);
  line(X_m-6,Y_m,X_m+6,Y_m);line(X_m,Y_m-4,X_m,Y_m+4);
  getimage(X_m-6,Y_m,X_m+6,Y_m,line_gor_kurs);
  getimage(X_m,Y_m-4,X_m,Y_m+4,line_vert_kurs);
  setcolor(7);
  line(X_m-6,Y_m,X_m+6,Y_m);line(X_m,Y_m-4,X_m,Y_m+4);
  X_m_old=X_m;Y_m_old=Y_m;
	return;
}
//-----------------------------------------------------------------
/***************************************************\
* Процедура поиска нового объекта, для курсора мыши *
\***************************************************/
int mouse()
{
	int delta,N_m;
	char X_mous[4],Y_mous[4];

	nom_func("173");
	regs.x.bx=0; //выбираем левую клавишу
  regs.x.ax=5; // получить информацию о нажатии
  int86(0x33,&regs,&regs);

  if(regs.x.bx>=1) //если было нажатие
  {
		down=1;
    if(menu_N!=0)
    {
      regs.h.ah=5;
      regs.h.ch=0;
      regs.h.cl=13;// имитация нажатия Enter
      int86(0x16,&regs,&regs);
      return(-1);
    }
#ifdef MOUSE_NASTR
    itoa(regs.x.cx,X_mous,10);itoa(regs.x.dx,Y_mous,10);
    setcolor(7);outtextxy(300,240,"██████████");
    setcolor(8);outtextxy(300,240,X_mous);
    setcolor(7);outtextxy(300,250,"██████████");
    setcolor(8);outtextxy(300,250,Y_mous);
    getch();
#endif
  }
  regs.x.bx=1; //выбираем правую клавишу
  regs.x.ax=5; // получить информацию о нажатии
  int86(0x33,&regs,&regs);
  if((regs.x.bx>=1)&&(menu_N==0)) //если была нажата правая клавиша
  {
    down=0;
    //  ***********************************
    regs.x.ax=7; // задать пределы движения по горизонтали
    regs.x.cx=X_MIN;
    regs.x.dx=X_MAX;
    int86(0x33,&regs,&regs);
    //  *************************************
    regs.x.ax=8;// задать пределы движения по вертикали
    regs.x.cx=Y_MIN;
    regs.x.dx=Y_MAX;
		int86(0x33,&regs,&regs);
		nosound();
    //--------------------------------------------------
		regs.h.ah=5;
    regs.h.ch=0;
    regs.h.cl=32;// имитация нажатия Space
    int86(0x16,&regs,&regs);
  }
	regs.x.ax=6;//опрос счетчика отпускания кнопки
	int86(0x33,&regs,&regs);
  if(((regs.x.bx&1)==1)&&(down==1))
  {
		down=0;           //7777
    regs.h.ah=5;
    regs.h.ch=0;
    regs.h.cl=13;// имитация нажатия Enter
    int86(0x16,&regs,&regs);
    mouser=0xf;

  }
  if(zapretvybora>0)return(old_modi);
  regs.x.ax=3; // получить информацию о координатах
  int86(0x33,&regs,&regs);
  if((X_m!=regs.x.cx)||(Y_m!=regs.x.dx))// если позиция сместилась
  {
    X_m=regs.x.cx;
    Y_m=regs.x.dx;
    regs.x.ax=3; //получить информацию о позиции
    int86(0x33,&regs,&regs);

    X_m=regs.x.cx;
    Y_m=regs.x.dx;
    mouse_down=0;
    delta=mouse_all/2+mouse_all%2;
    N_m=delta;//начинаем с объекта в центре упорядоченной базы
rep0:
    if(mouse_mark[N_m][1]>X_m) //если этот объект правее места курсора
    {
			if(mouse_mark[N_m][1]-X_m<=5)// если объект в зоне Х-захвата
      {
        Y_m_isk=Y_m;
        goto poisk_Y;
      }
      if(N_m==0)return(modi);//если нет объекта,привязанного по x
      if(delta==1)return(-1);
      delta=delta/2+delta%2;
      N_m=N_m-delta;//смещаемся влево
      if(N_m<0)return(-1);
      goto rep0;
    }
    if(mouse_mark[N_m][1]<X_m) //если этот объект левее места курсора
    {
      if(X_m-mouse_mark[N_m][1]<=5)// если объект в зоне Х-захвата
      {
        Y_m_isk=Y_m;
        goto poisk_Y;
      }
      if(N_m>=mouse_all) return(-1);// если нет объекта,привязанного по x
      if(delta==1)return(-1);
      delta=delta/2+delta%2;
      N_m=N_m+delta;//смещаемся влево
      goto rep0;
    }
poisk_Y:
    while(abs(mouse_mark[N_m][1]-X_m)<=5) N_m--;// подняться вверх до начала
    N_m++;
    while(abs(mouse_mark[N_m][1]-X_m)<=5)
    {
      if(abs(mouse_mark[N_m][2]-Y_m)<=5) // если объект в зоне Y-захата
      {
        return(mouse_mark[N_m][0]);// вернуть номер выбранного объекта
      }
      N_m++;
		}
    return(-1);// новый объект не найден
  }
  else return(modi);
}
//-----------------------------------------------------
/****************************************\
* Процедура запоминания фона под старой  *
* позицией курсора мыши.                 *
\****************************************/
void mem_mous_fon()
{

	nom_func("167");
	getimage(X_m-6,Y_m,X_m+6,Y_m,line_old_gor_kurs);
  getimage(X_m,Y_m-4,X_m,Y_m+4,line_old_vert_kurs);
  return;
}
//-------------------------------------------------------
/**********************************************\
* Процедура отображения курсора мыши на экране *
\**********************************************/
void draw_mouse()
// нарисовать курсор
{
	nom_func("46");
	putimage(X_m-6,Y_m,line_gor_kurs,0);
  putimage(X_m,Y_m-4,line_vert_kurs,0);
  return;
}
//----------------------------------------------------------------
/********************************************\
* Процедура восстановления фона под мышью    *
* в старой позиции (откуда ушла мышь)        *
\********************************************/
void clear_mouse()

{
	nom_func("25");
	putimage(X_m_old-6,Y_m_old,line_old_gor_kurs,0);
	putimage(X_m_old,Y_m_old-4,line_old_vert_kurs,0);
	return;
}
//-----------------------------------------------------------------------
/*************************************************\
* Процедура переноса изображения курсора мыши из  *
* старой позиции X_m_old,Y_m_old в новую X_m,Y_m. *
\*************************************************/
void move_mouse()
{ int i;

	nom_func("175");
	if(MOUSE_ON==0)return;
  regs.x.ax=3; //получить информацию о координатах
  int86(0x33,&regs,&regs);
	X_m=regs.x.cx;
	Y_m=regs.x.dx;
	if(menu_N!=0)//если вывешено меню
	{
		if((X_m>X_menu)&&(Y_m<30))//если попали в зону меню
		{
			clear_mouse();
			for(i=0;i<12;i++)line_old_gor_kurs[i]=0;
			for(i=0;i<40;i++)line_old_vert_kurs[i]=0;
//      for(i=0;i<12;i++)line_gor_kurs[i]=0;
//      for(i=0;i<40;i++)line_vert_kurs[i]=0;
			if(Y_m<10)punkt_N=1;
			if((Y_m>10)&&(Y_m<20))punkt_N=2;
			if((Y_m>20)&&(Y_m<30))punkt_N=3;
			if(menu_N==1)down=0;
			menu1();
      down=0;
      return;
    }
  }
  if(((X_m!=X_m_old)||(Y_m!=Y_m_old)||(down==1))&&(perezap!=1)&&( mouser==0))// если в пустом поле
  {
		if(zapretvybora==1)return;
//    if(perezap==0)
//		if(sbros1!=1)
		clear_mouse();//восстановить картинку под старым местом курсора
		if(perezap==2)perezap=0;
		mem_mous_fon();//запомнить картинку под новым местом
		draw_mouse();//нарисовать курсор в новом месте
		X_m_old=X_m;
		Y_m_old=Y_m;
		old_modi=-1;
		pat=-1;
	}
	return;
}
#endif
//-----------------------------------------------------
void tst_zvuk()
{
	nom_func("376");
	if(STATUS!=1)
  {
		nosound();
    return;
  }
  switch(zvuk_sig.tip)
  {
    case 0: return;
    case 1: if((biostime(0,0l)-zvuk_sig.time_begin)<zvuk_sig.dlit)
            {
              sound(600);return;
            }
            else
            {
              nosound();
              return;
            }
    case 2: if((biostime(0,0l)-zvuk_sig.time_begin)<(zvuk_sig.dlit/3))
            {
              sound(400);return;
            }
            else
            {
              if((biostime(0,0l)-zvuk_sig.time_begin)<(2*zvuk_sig.dlit/3))
              {
                sound(600);return;
              }
              else
                if((biostime(0,0l)-zvuk_sig.time_begin)<zvuk_sig.dlit)
                {
                  sound(400);return;
                }
                else
                {
                  nosound();
                  return;
                }
            }
    case 3: return;            
    default:  return;
  }
}
//------------------------------------------------------------------
/************************************************\
* Процедура отрисовки экранных кнопок при выводе *
* основного экрана программы после загрузки или  *
* нахождения системы в других экранах АРМа.      *
\************************************************/
void prodol()
{
	nom_func("239");
	if (vrem==5) uprav=1;
	//settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
	setcolor(8);
	if (vrem==1&&uprav==0)
	{
		puti=0;
		vkl_kno(2,LIGHTGREEN);
		vkl_kno(0,14);
		add(0,0,'р');
		vrem=0;uprav=1;poka=0;tst=2;
	}
	else
	if (ot==1&&uprav==0)
	{
		vkl_kno(1,LIGHTGREEN);
		vkl_kno(0,14);
		add(0,0,'Р');
		tst=0;
	}
	if(ot!=1)
		if (uprav==0)
		{
			vkl_kno(1,LIGHTGREEN);
			add(0,0,'Р');
			tst=0;
		}
		else
			if (uprav==1)
			{
				vkl_kno(2,LIGHTGREEN);
				add(0,0,'р');
				tst=2;
			}
	if(perekluch==1&&ot==1&&tst==2)
	{
		vkl_kno(2,LIGHTGREEN);
		add(0,0,'р');
		ot=0;
	}
  else
		if(tst!=2)
		{
			vkl_kno(1,LIGHTGREEN);
			add(0,0,'Р');
		}
  if(perekluch==1)poka=0;
  i3=-1;
  menu();
	return;
}
//-----------------------------------------------------------
/**************************************************\
* Процедура вывода в нижнюю часть подсказки в виде *
* меню возможных действий оператора при работе на  *
* основной и резервной ПЭВМ.                       *
\**************************************************/
void menu()
{
	nom_func("168");
	setfillstyle(CLOSE_DOT_FILL,LIGHTCYAN);
  bar(1,460,640,480);setcolor(7);
  if(DISK==0)
  {
    if(STATUS==0)
    { outtextxy(3,462,"Esc");outtextxy(162,462,"ПРОБЕЛ");
      outtextxy(350,462,"F2");outtextxy(418,462,"F3");
      outtextxy(500,462,"F4");
      outtextxy(3,472,"F5");outtextxy(115,472,"F6");
      outtextxy(259,472,"F9");outtextxy(475,472,"F10");
      setcolor(8);
      outtextxy(2,461,"Esc");outtextxy(161,461,"ПРОБЕЛ");
      outtextxy(349,461,"F2");outtextxy(417,461,"F3");
      outtextxy(499,461,"F4");
      outtextxy(2,471,"F5");outtextxy(114,471,"F6");
      outtextxy(258,471,"F9");outtextxy(474,471,"F10");
      outtextxy(28,462,"-курсор на место");
      outtextxy(210,462,"-отказ от команды ");
      outtextxy(365,462,"-время");
      outtextxy(436,462,"-помощь");
      outtextxy(520,462,"-отключить звук");
      outtextxy(20,471,"-неиспр.СЦБ");
      outtextxy(130,471,"-неиспр.ап.УВК");
      outtextxy(274,471,"-подсв.положения стрелок");
      outtextxy(500,471,"-архив");
    }
    if(STATUS==1)
    {
      outtextxy(3,462,"Esc");outtextxy(162,462,"ПРОБЕЛ");
      outtextxy(350,462,"F1");
      outtextxy(3,472,"F4");
      outtextxy(146,472,"F5");outtextxy(290,472,"F9");
      setcolor(8);
      outtextxy(2,461,"Esc");outtextxy(161,461,"ПРОБЕЛ");
			outtextxy(349,461,"F1");
			outtextxy(2,471,"F4");
			outtextxy(145,471,"F5");outtextxy(289,471,"F9");
			outtextxy(28,462,"-курсор на место");
			outtextxy(210,462,"-отказ от команды ");
			outtextxy(362,462,"-разд/марш");
			outtextxy(20,472,"-отключить звук");
			outtextxy(161,472,"-обновить экран");
			outtextxy(305,472,"-подсв.положения стрелок");
		}
	}
	else
	{
		outtextxy(3,462,"F4");outtextxy(130,462,"F5");
		outtextxy(230,462,"F6");outtextxy(330,462,"F7");
		outtextxy(3,472,"F8");outtextxy(330,472,"F9");
		outtextxy(500,472,"F10");
		setcolor(BROWN);
		outtextxy(2,461,"F4");outtextxy(129,461,"F5");
		outtextxy(229,462,"F6");outtextxy(329,461,"F7");
		outtextxy(2,471,"F8");outtextxy(329,471,"F9");
		outtextxy(499,471,"F10");
		setcolor(8);
		outtextxy(18,462,"-отключить звук");outtextxy(142,462,"-неиспр.СЦБ");
		outtextxy(240,462,"-неиспр.ап.УВК");
		outtextxy(345,462,"-переход в архиве к другому моменту времени");
		outtextxy(18,471,"-переключение просмотра (автоматический/пошаговый)");
		outtextxy(345,471,"-подсв.положения стрелок");
		outtextxy(520,471,"-выход");
	}
	return;
}
//----------------------------------------------------------------------
/************************************************************\
* Отладочная функция для вывода на экран трассировки порядка *
* выдачи программных модулей.                                *
\************************************************************/
void nom_func(char proga[5])
{
	int cvets;

//	FILE* hDebugFile;
//	struct time ct;

//#ifdef NALAD_F
/*
   hDebugFile = fopen ("func.dbg", "a");
   if (hDebugFile != NULL)
   {
      gettime (&ct);
      fprintf (hDebugFile, "%02d:%02d:%02d : %s\n",
		 ct.ti_hour, ct.ti_min, ct.ti_sec, proga);
      fclose (hDebugFile);
   }
   else;
*/
//#endif

#ifndef NALAD
//cvets=getcolor();
//setcolor(cvets);
	return;
#endif
	cvets=getcolor();
	Y_NAL=Y_NAL+10;
	if(Y_NAL>=470)Y_NAL=10;
	setcolor(7);
	outtextxy(X_NAL,Y_NAL,"████");
	setcolor(8);
	outtextxy(X_NAL,Y_NAL,proga);
	setcolor(cvets);
}
//---------------------------------------------------------------
/*********************************************************\
* Программа прямого вывода текстовой строки в видеопамять *
* в текстовом режиме.                                     *
\*********************************************************/
void puts1(char *simb,unsigned char cvt,int X,int Y)
{
	int dx,simbol,i;
	unsigned char smb;
	i=0;
	Y=Y-1;
	X=X-1;
	while(simb[i]!=0)
	{
		smb=simb[i++];
		simbol=(cvt<<8)+smb;
		dx=Y*160+2*X;
		poke(0xB800,dx,simbol);
		X++;
	}
	return;
}