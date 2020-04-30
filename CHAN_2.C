#include"bla.h"
#include "opred.h"
#include <setjmp.h>
void interrupt far reading_char();
void interrupt far PRNTSCR();
void interrupt far CNTRLBREAK();
void interrupt far KEYBRD();
void interrupt far TIMER_TIC();
void (interrupt far *old_timer)();
void (interrupt far *s_time)();
void (interrupt far *old1)();
void (interrupt far *old2)();
void (interrupt far *old3)();
int i_C;
char text[3];
unsigned char test_sum;
//---------------------------------------------------------------------
/****************************************************\
* Процедура завершения выдачи команды в буфер вывода *
* выполняет обнуление буфера команд.                 *
\****************************************************/
void fin_kom()
{
	int uu=0,n_bait;
	nom_func("55");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	flagoutmsg=0;
	for(uu=0;uu<n_bait;uu++)buf_ko[uu]=0;
	return;
}
//---------------------------------------------------------------------
/***************************************************\
* программа монитор процессов получения/передачи,   *
* обработки и отображения информации по каналам     *
* обмена АРМа ДСП                                   *
\***************************************************/
void consentr()
{
	int TELO=0,st,tos=0,jj,i,n_bait;
	nom_func("28");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)goto m01; //если просмотр архива, то выход

	//просмотр буфера приема ТУМСов выделение ответов и сообщений
	ANALIZ_TUMS();
	OUT_DC(); //выдача данных в ДЦ
#ifndef WORK
	vidacha_BBKP(BAZ_ADR2);
#endif
m01:
#ifdef DISPETCHER
	ANALIZ_BBKP(); //анализ информации, принятой из ДЦ
#endif
	for(st=0;st<skoko_stoek;st++)//пройти по всем стойкам
	{
		if((REG_INFO[st][0]=='(')&& //если сообщение не сброшено

		((REG_INFO[st][n_bait-1]==')')||(REG_INFO[st][n_bait-1]=='+')))
		{
			prv[st]=1;//установить признак фиксации
			for(i=0;i<n_bait;i++)
			{
				bu[st][i]=REG_INFO[st][i];
				REG_INFO[st][i]=0;
			}
			obrabotka_inf(st);     //обработать принятые данные от ТУМС
			if(DISK!=0)goto m02;
			if(t_t==0)t_t=test_time(); //проверить наступление 10-минутного пикета
			if((t_t==1)&&(ZAGRUZKA==0))//если пора и нет загрузки
			{
/*				if(getpixel(580,450)==7)setcolor(12);
				else setcolor(7);
				outtextxy(580,450,"█");*/
				zafix++;//передвинуть счетчик
				if(prv[st]==1)//если есть что записать
				{
					SBROS_BIT_TEST_SOOB(st,bu[st][3]);//сбросить признак требования
					if(st==0)add(0,0,'п');// записать данные для 1-го
					if(st==1)add(1,0,'в');//или 2-го каналов
					prv[st]=0;//сбросить требование записи
					zafix=0;//обнулить счетчик до 16
				}
				if(TEST_SUM_SOOB()==0)//если все даные приняты
				{
					if(STATUS==0)add(0,0,'Д'); //записать статус АРМа
					if(STATUS==1)add(0,0,'Г');
					if(OTMENA==1)add(0,0,'О');//если нажата отмена
					else add(0,0,'о');
					if(OGRAN==1)add(0,0,'З');//если нажата ВСПЗ
					else add(0,0,'з');
					if(MAK==1)add(0,0,'М'); //если нажата макет
					else add(0,0,'м');
					if(AVAR==1)add(0,0,'Й');//если нажата аварийный перевод
					else add(0,0,'й'); //если не нажата
					if(uprav==1)add(0,0,'р');//если управление маршрутное
					else add(0,0,'Р'); //если раздельное
					jj=0;
					while(jj<kol_VO)//пройти по всем объектам
					{
						jj=upak_fr4(jj);//проверить имеющиеся ограничения
						if(jj<kol_VO)add(5,0,0xf);//записать ограничения
					}
					first_time=biostime(0,0);//сверить часы
					second_time=first_time;  //совместить часы
					zafix=0;
					t_t=0;
				}
			}
m02:
		}//конец работы с регистром информации
	}//конец прохода по стойкам
	if(DISK!=0)goto m03;
	test_mo();// проверка и переинициализация модема
	if(pusto==0) //если есть связь с ТУМС
	{
agm:
		//если половина сквозного маршрута выдана
		if(flagoutmsg==13)flagoutmsg=1;

		// если есть готовая команда для ТУМС
		if((flagoutmsg>0)&&(flagoutmsg<12))
		{
			//если АРМ не основной, то команды выдавать нельзя
			if((STATUS==0)||(STATUS==2))
			{
				w(140,999,"");
vs:     fin_kom();return;
			}
#ifdef DISPETCHER
			if(buf_ko_dc[0][1]!=0)
			{
				TELO=TEL1;
				st=0;//объект для РУ/ОРУ каналов стойки 1
				for(jj=0;jj<n_bait;jj++)
				{
					buf_ko[jj]=buf_ko_dc[0][jj];
					buf_ko_dc[0][jj]=0;
				}
			}
#endif
#ifdef KOL_SOO2
			if(buf_ko_dc[1][1]!=0) //если стойка вторая
			{
				TELO=TEL2;
				st=1;
				for(jj=0;jj<n_bait;jj++)
				{
					buf_ko[jj]=buf_ko_dc[1][jj];
					buf_ko_dc[1][jj]=0;
				}

			}
#endif

			if(buf_ko[1]!=0)
			{
				tos=buf_ko[1]&0x02; //выделить стойку
				if(tos==0) //если стойка первая
				{TELO=TEL1;st=0;}//объект для РУ/ОРУ каналов стойки 1
#ifdef KOL_SOO2
				if(tos==2){TELO=TEL2;st=1;} //если стойка вторая
#endif
			}
			if((otkaz_tu[st]==1)||   //если по каналу запрещено управлять
			(zapret[st]!=0))
			{
				w(54,999,"");//КОМАНДА НЕ ВЫПОЛНЕНА - НЕ РАБ. КАНАЛ ТУ ТУМС
				if(st==0);outtext(" 1");//вывести номер канала
				if(st==1);outtext(" 2");
				fin_kom();
				return;
			}

			else//если управление не запрещено и нет переключения комплектов
			{
				if(((fr3[TELO][2]==0)&&(fr3[TELO][3]==1))||(PRKL==1))
				//если комплекты данной стойки в норме(РУ-ОРУ)
				{
ag:       for(i_C=0;i_C<(n_bait-2);i_C++)bu[st][i_C]=buf_ko[i_C];//команду в буфер

					bu[st][0]='(';
					buf_ko[0]='(';
					bu[st][n_bait-1]=')';
					buf_ko[n_bait-1]=')';
					buf_ko[n_bait-2]=check_sum(buf_ko,1);//установить контрольную сумму
					flms=0;

					//отправить команду для регистрации в соседнюю ПЭВМ
					kom_v_bufer_pvm(1,buf_ko[0]);

					kom_v_bufer(st);  // отправить команду в стойку на исполнение
					add(st,0,'>');//записать команду на диск
					if(flagoutmsg==10)// если сквозной маршрут
					{
						//взять в буфер
						for(i_C=0;i_C<(n_bait-2);i_C++)buf_ko[i_C]=buf_ko_mm[i_C];

						if(flagoutmsg==10)// если сквозной маршрут ?????
							{flagoutmsg=1;  goto agm;}
						else  // если обычный сквозной маршрут
							{flagoutmsg=12;return;}
					}
					flagoutmsg=0;//сбросить флаг наличия команды для ТУМС

					//если была команда переключения комплектов, сбросить признак
					if(PRKL==1) PRKL=0;
				}
				else//если состояние датчиков отличается от состояния телеуправления
				{
					w(58,999,"");//ТЕЛЕУПРАВЛЕНИЕ ОТКЛЮЧЕНО-КОМАНДЫ НЕ ВОЗМОЖНЫ
					if(st==0)outtext(" УВК1");
					if(st==1)outtext(" УВК2");
					fin_kom();
					return;
				}
			}
		}
	}
m03:
	return;
}
//-----------------------------------------------------------------------
/*****************************************************\
* Процедура формирования выходного буфера обмена ПЭВМ *
\*****************************************************/
void FORM_BUF_PVM_OUT()
{
	int test=0,i,n_bait;
	nom_func("70");
	n_bait=11;
	if(DISK!=0)return; //для просмотра архива функция не выполняется

	//при загрузке и отсутствии телеуправления - не выполняется
	if((ZAGRUZKA==0)&&(TELEUP!=1))return;
	else
	{ //при отсутствии телеуправления, хоз.поездов и при не загрузке
		if((TELEUP==0)&&(hoz_poezd[0]==0)&&(hoz_poezd[1]==0)&&(ZAGRUZKA==0))
		{ //при отсутствие отказов
			if(okno_otkaz==0)t(0); //стереть окно сообщений
			TELEUP=1; //установить признак телеуправления
		}
	}

	// если нечего передавать
	if((MY_FR4==0)&&( MY_COM==0)&&( MY_INF==0)&&
	(KVIT_FR4==0)&&( KVIT_COM==0)&&( KVIT_INF==0))

	// если таймер запрета передачи включен
	{ if(PERED_PVM==1)
		{ if((biostime(0,0L)-TIME_OUT_PVM)>1L)// если прошло время тайм-аута
			{ outportb(BAZ_ADR4+1,3);// открыть передатчик
				PERED_PVM=2;// установить признак открытой передачи после тайм-аута
				outportb(BAZ_ADR4,'*');
			}
		}

		if(PERED_PVM==0)//если передача закрыта
		{ TIME_OUT_PVM=biostime(0,0L);//запустить таймер
			PERED_PVM=1;// установить признак закрытого передатчика с включенным таймером
		}
	}
	else// если надо что-то передать
	{
		//если надо передать квитанцию на команду, полученную от основного
		if(KVIT_COM==1)
		{ test++;
			for(i=0;i<3;i++)
			{ BUF_OUT_PVM[UKAZ_ZAP_OUT]=REG_KVIT_COM[i];
				REG_KVIT_COM[i]=0;
				if(UKAZ_ZAP_OUT==(MAX_SIZE_OUT-1))UKAZ_ZAP_OUT=0;
				else UKAZ_ZAP_OUT++;
			}
			KVIT_COM=0;
		}

		//если надо передать квитанцию на полученные ограничения
		if(KVIT_FR4==1)
		{ test++;
			for(i=0;i<3;i++)
			{ BUF_OUT_PVM[UKAZ_ZAP_OUT]=REG_KVIT_FR4[i];
				REG_KVIT_FR4[i]=0;
				if(UKAZ_ZAP_OUT==(MAX_SIZE_OUT-1))UKAZ_ZAP_OUT=0;
				else UKAZ_ZAP_OUT++;
				REG_KVIT_FR4[i]=0;
			}
			KVIT_FR4=0;
		}

		//если надо передать квитанцию на полученную информацию
		if(KVIT_INF==1)
		{ test++;
			for(i=0;i<3;i++)
			{ BUF_OUT_PVM[UKAZ_ZAP_OUT]=REG_KVIT_INF[i];
				REG_KVIT_INF[i]=0;
				if(UKAZ_ZAP_OUT==(MAX_SIZE_OUT-1))UKAZ_ZAP_OUT=0;
				else UKAZ_ZAP_OUT++;
			}
			KVIT_INF=0;
			BUF_OUT_PVM[UKAZ_ZAP_OUT]=0;
		}

		//если надо передать свою информацию
		if(MY_INF==1)
		{ //если передавал три раза, то сбросить обмен
			if(POVTOR_INF>3)sbros_obmen_pvm();
			else
			{ test++;
				for(i=0;i<4;i++)
				{ BUF_OUT_PVM[UKAZ_ZAP_OUT]=REG_INF[i];
					if(UKAZ_ZAP_OUT==(MAX_SIZE_OUT-1))UKAZ_ZAP_OUT=0;
					else UKAZ_ZAP_OUT++;
				}
				MY_INF=0;
				POVTOR_INF++;
			}
		}

		// если не основная ПЭВМ
		if((STATUS==0)||(STATUS==2))
		{ // если нет неосновной передачи
			if(test==0)
			{ MY_FR4=0;
				MY_COM=0;
				POVTOR_FR4=0;
				POVTOR_COM=0;
				return;
			}
			goto exit;
		}
		else// если основная
		{

			if(MY_FR4==1)//если есть новое для FR4 или нужен повтор
			{ if(POVTOR_FR4>3) sbros_obmen_pvm();
				else
				{ test++;
					for(i=0;i<6;i++)
					{ BUF_OUT_PVM[UKAZ_ZAP_OUT]=REG_FR4[i];
						if(UKAZ_ZAP_OUT==(MAX_SIZE_OUT-1))UKAZ_ZAP_OUT=0;
						else UKAZ_ZAP_OUT++;
					}
					MY_FR4=0;
					POVTOR_FR4++;
				}
			}
			//если есть команда для передачи в резервную ПЭВМ
			if(MY_COM==1)
			{ if(POVTOR_COM>4) sbros_obmen_pvm();
				else
				{ test++;
					for(i=0;i<n_bait;i++)
					{ BUF_OUT_PVM[UKAZ_ZAP_OUT]=REG_COM[i];
						if(UKAZ_ZAP_OUT==MAX_SIZE_OUT-1)UKAZ_ZAP_OUT=0;
						else UKAZ_ZAP_OUT++;
					}
					MY_COM=0;
					POVTOR_COM++;
				}
			}
exit:
			outportb(BAZ_ADR4+1,3);// открыть передачу
			PERED_PVM=2;
			outportb(BAZ_ADR4,'*');
			return;
		}
	}
	return;
}
//------------------------------------------------------
//***************************************
form_otv(int st)
{
  int gas1,jj;
  unsigned char text[7];
	nom_func("84");
	if(DISK!=0)return;
  //определить стойку по заголовку
  gas1=(REG_INFO[st][1]&3)-1;
  if(ZAGRUZKA==1)
    //объявить о перекрещивании каналов
    if(st!=gas1){Change_adresa();return;}
  if((zapret[gas1]&0x7fff)!=0)return;
	if(otv_kom_oper==0)
	{
		REG_OT[st][0]='(';
		REG_OT[st][5]=')';
	}
	else
	{
		REG_OT[st][0]='{';
		REG_OT[st][5]='}';
	}
  REG_OT[st][4]='P';
  REG_OT[st][1]=ZAGO_MPSU(REG_INFO[st][1]);
  REG_OT[st][2]=REG_INFO[st][2];
  REG_OT[st][3]=REG_INFO[st][3];
//  if(REG_INFO[st][10]!='+')sboy_tu[st]++;
//  if(sboy_tu[st]>5)sboy_tu[st]=6;
//  if(REG_INFO[st][10]==')')sboy_tu[st]--;
//  if(sboy_tu[st]<0)sboy_tu[st]=0;
  for(jj=0;jj<6;jj++)
  {
    if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
    BUF_OUT[st][ukaz_zap[st]]=REG_OT[st][jj];
    text[jj]=REG_OT[st][jj];
    REG_OT[st][jj]=0;
    ukaz_zap[st]++;
  }
  if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
  if(st==0)
  { outportb(BAZ_ADR1+1,3);
    outportb(BAZ_ADR1,0);
  }
  if(st==1)
  { outportb(BAZ_ADR11+1,3);
    outportb(BAZ_ADR11,0);
  }
	return;
}
//********************************
vidacha_pvm(int adre)
{
	nom_func("391");
	if(UKAZ_ZAP_OUT==UKAZ_VYVOD)// передано все?
	{
    if(PERED_PVM==2)// передача не запрещалась ?
    {
      PERED_PVM=0;// установить признак запрета передачи
      outportb(adre+1,1);// запретить прерывания на передачу

    }

	}
  else // передано не все
  {

//    if((MY_FR4>0)||(MY_COM>0)||(MY_INF>0)||(KVIT_FR4>0)||(KVIT_COM>0)||(KVIT_INF>0))
    outportb(adre,BUF_OUT_PVM[UKAZ_VYVOD]);// передать очередной символ
//     outportb(BAZ_ADR11,BUF_OUT_PVM[UKAZ_VYVOD]);
//     outportb(BAZ_ADR11,'0');
    BUF_OUT_PVM[UKAZ_VYVOD]=0;// очистить ячейку буфера переданного байта
    if(UKAZ_VYVOD==(MAX_SIZE_OUT-1))UKAZ_VYVOD=0;//передвинуть указатель передачи
    else UKAZ_VYVOD++;
  }
  return;
}
//--------------------------------------------------------------------
/**********************************************\
* Процедура сброса каналов обмена с ТУМС и ДЦ. *
* Освобождает все буфера,переменные и очереди. *
\**********************************************/

void ochistka()
{
	int i,j;
	nom_func("197");

#ifdef DISPETCHER
	for(i=0;i<SIZE_BUF_OUT_DC;i++)BUF_OUT_DC[i]=0;
	for(i=0;i<SIZE_BUF_PRIEM_DC;i++)BUF_IN_DC[i]=0;
	ukaz_zap_dc=0;ukaz_vyd_dc=0;
#endif
	for(j=0;j<skoko_stoek;j++)
	{
		prv[j]=0;
		ukaz_zap[j]=0;ukaz_vyd[j]=0;
		ukaz_priem[j]=0;ukaz_read[j]=0;
		for(i=0;i<SIZE_BUF_OUT;i++)BUF_OUT[j][i]=0;
		for(i=0;i<SIZE_BUF_PRIEM;i++)BUF_IN[j][i]=0;
	}
	return;
}
//-------------------------------------------------------------
/********************************************\
*  Процедура приема символа из порта обмена  *
*  с ТУМС и запись его в буфер ввода ТУМСа.  *
*  если принят символ ")" или "+", то        *
*  увеличивается переменная ANALIZ[st],      *
*  а также запоминается позиция конца сообщ. *
*  при получении "+" сбрасываетя sboy_tu[st] *
\********************************************/
void in_port(unsigned int adr)
{
	int st;
	/*
	nom_func("104");
*/
  if(adr==BAZ_ADR1)st=0;
  else
    if(adr==BAZ_ADR11)st=1;
		else return;
  SYMBOL[st]=inportb(adr)&127;
  if(SYMBOL[st]==0)return;
  t_pust[st]=0;
	if((SYMBOL[st]==')')||(SYMBOL[st]=='+'))
  {
		ANALIZ[st]++;
    prorisovka=0;
    if(SYMBOL[st]=='+')sboy_tu[st]=0;
    konec[st]=ukaz_priem[st];
  }
  BUF_IN[st][ukaz_priem[st]++]=SYMBOL[st];
  if(ukaz_priem[st]>=SIZE_BUF_PRIEM)ukaz_priem[st]=0;
  return;
}
//------------------------------------------------------------------
/*********************************************\
* Процедура инициализации каналов связи АРМа  *
\*********************************************/
void init()
{
	nom_func("106");
	if(DISK!=0)return;
	disable();
//  old1=getvect(0x5);
		old2=getvect(0x1b);
		old3=getvect(0x9);
//	setvect(0x5,PRNTSCR);
		setvect(0x1b,CNTRLBREAK);
		setvect(0x9,KEYBRD);
	get_int_vect();
	set_int_vect();
//****************************
	s1=inportb(0x21);
	s2=0x00;
	outportb(0x21,s2); // маска прерываний 1 - маскировано
	s2=inportb(0x21);
	// 7 6 5 4 3 2 1 0
	// prn floppy винт com1 com2 аппар клавиат timer
	outportb(BAZ_ADR1+3,0x00);
	outportb(BAZ_ADR1+1,0x00);
	outportb(BAZ_ADR1+3,0x80); // регистр управления линией - 80Н для установки скорости
	outportb(BAZ_ADR1+1,st_ba); // старший байт делителя частоты
	outportb(BAZ_ADR1,ml_ba);// младший байт делителя частоты для скорости 2400
	outportb(BAZ_ADR1+3,0x0e);//N бит
	outportb(BAZ_ADR1+1,0x0); // запретить прерывания COM
	outportb(BAZ_ADR1+4,0x0b); // упр модемом
	outportb(BAZ_ADR1,0);
	//**********************
	if(BAZ_ADR11!=0)
	{
	// 7 6 5 4 3 2 1 0
	// prn floppy винт com1 com2 аппар клавиат timer
	outportb(BAZ_ADR11+3,0x00);
	outportb(BAZ_ADR11+1,0x00);
	outportb(BAZ_ADR11+3,0x80); // регистр управления линией - 80Н для установки скорости
	outportb(BAZ_ADR11+1,st_ba2); // старший байт делителя частоты
	outportb(BAZ_ADR11,ml_ba2);// младший байт делителя частоты для скорости 2400
	outportb(BAZ_ADR11+3,0x0e);//N бит
	outportb(BAZ_ADR11+1,0x0); // запретить прерывания COM
	outportb(BAZ_ADR11+4,0x0b); // упр модемом
	outportb(BAZ_ADR11,0);
	}
	 //**********************
	if(BAZ_ADR2!=0)
	{
		outportb(BAZ_ADR2+3,0x00);  //bbkp
		outportb(BAZ_ADR2+1,0x00);
		outportb(BAZ_ADR2+3,0x80);  // регистр управления линией - 80Н для установки скорости
		outportb(BAZ_ADR2+1,st_bab);// старший байт делителя частоты
		outportb(BAZ_ADR2,ml_bab);  // младший байт делителя частоты для скорости 2400
		outportb(BAZ_ADR2+3,0x0e);  //N бит
		outportb(BAZ_ADR2+1,0x00);  // запр прерывания COM
		outportb(BAZ_ADR2+2,0x0);  // запретить FIFO
		outportb(BAZ_ADR2,0);
 //**********************
	}
	 //**********************
	if(BAZ_ADR4!=0)
	{
	outportb(BAZ_ADR4+3,0x00);
	outportb(BAZ_ADR4+1,0x00);
	outportb(BAZ_ADR4+3,0x80);  // регистр управления линией - 80Н для установки скорости
	outportb(BAZ_ADR4+1,st_bap);// старший байт делителя частоты
	outportb(BAZ_ADR4,ml_bap);  // младший байт делителя частоты для скорости 2400
	outportb(BAZ_ADR4+3,0x0f);  //N бит
	outportb(BAZ_ADR4+1,0x03);  // разр прерывания COM
	outportb(BAZ_ADR4+4,0x0);  // упр модемом
	outportb(BAZ_ADR4+2,0x0); // запретить FIFO
	//**********************
	}
	//**********************
	if(BAZ_ADR6!=0)
	{
	outportb(BAZ_ADR6+3,0x00);
	outportb(BAZ_ADR6+1,0x00);
	outportb(BAZ_ADR6+3,0x80);  // регистр управления линией - 80Н для установки скорости
	outportb(BAZ_ADR6+1,st_bas);// старший байт делителя частоты
	outportb(BAZ_ADR6,ml_bas);  // младший байт делителя частоты для спдлп
	outportb(BAZ_ADR6+3,0x0f);  //8бит 2стоп без паритета
	outportb(BAZ_ADR6+1,0);    // запр прерывания COM
	inportb(BAZ_ADR6);
	//**********************
	}
	enable();
	return;
}
//---------------------------------------------------------------------
/*****************************************************\
* Процедура получения старого вектора прерывания для  *
* установленного вектора в многопортовой плате COM    *
\*****************************************************/
void get_int_vect()
{
 nom_func("97");
 if(DISK!=0)return;
 s_time=getvect(V);//запомнить старый вектор для коммуникационного прерывания
 old_timer=getvect(0x1c); //запомнить вектор для таймерного прерывания
 return;
}
//**********************************************************
void set_int_vect()
{
	unsigned int S01;
	unsigned char TST;
 nom_func("298");
 if(DISK!=0)return;
 TST=1;
 if(V>0x70)
 {
	TST=TST<<(V-0x71);
	S01=inportb(0xa1);
	outportb(0xa1,S01|TST);
 }
 else
 {
	TST=TST<<(V-8);
	S01=inportb(0x21);
	outportb(0x21,S01|TST);
 }
 setvect(V,reading_char);//установить вектор для коммуникации
 setvect(0x1c,TIMER_TIC);//установить вектор для таймерного прерывания
 return;
}
//*********************************************************
void reset_int_vect()
{
	nom_func("279");
	if(DISK!=0)return;
	disable();
//	setvect(0x5,old1);
	setvect(0x1b,old2);
	setvect(0x9,old3);
	setvect(0x1c,old_timer);
	if(BAZ_ADR2!=0) outportb(BAZ_ADR2+4,0);
	if(BAZ_ADR11!=0) outportb(BAZ_ADR11+4,0);
	if(BAZ_ADR4!=0) outportb(BAZ_ADR4+4,0);
	outportb(BAZ_ADR1+4,0);
	outportb(BAZ_ADR1+1,0); // разр прерывания COM
	if(BAZ_ADR11!=0) outportb(BAZ_ADR11+1,0); // разр прерывания COM
	if(BAZ_ADR2!=0) outportb(BAZ_ADR2+1,0); // разр прерывания COM
	if(BAZ_ADR4!=0) outportb(BAZ_ADR4+1,0); // разр прерывания COM
	outportb(0x21,0);
	outport(0x20,0x20);
	setvect(V,s_time);
	outportb(0x21,s1);
	enable();
	return;
}
//**************************************************************//
void interrupt far reading_char()
{
	int de1,de11,de2,de4,d5,test,err_bbkp,err_tums;
	if(DISK!=0)return;
//	asm cli
	outportb(0x20,0x20);
	if(STOROG>=60)
	{
		asm sti
		setcolor(12);
		outtextxy(X_NAL+30,Y_NAL,"??");
    sleep(2);
		reset_arm();
		outportb(0x20,0x20);// закрыть процедуру
		return;
	}
	de1=inportb(BAZ_ADR1+2);//читать прерывания 1-го ТУМС
	if(BAZ_ADR11!=0)de11=inportb(BAZ_ADR11+2); // читать прерывания 2-го ТУМС
	else de11=1;
	if(BAZ_ADR2!=0)de2=inportb(BAZ_ADR2+2);//читать прерывание ББКП
	if(BAZ_ADR4!=0)de4=inportb(BAZ_ADR4+2);//читать прерывания ПЭВМ
	if((BAZ_ADR4==0)&&(BAZ_ADR2==0)&&( BAZ_ADR11==0)) goto b;
a:
	err_tums=inportb(BAZ_ADR1+5)&0xe;
	if(err_tums!=0)
	{
		if((err_tums&2)==2)ERROR_TUMS1_0++;
		if((err_tums&4)==4)ERROR_TUMS1_1++;
		if((err_tums&8)==8)ERROR_TUMS1_2++;
	}

	d5=de4&6;//анализ ошибки четности  для обмена ПЭВМ
	if(d5==6)
	{
		inportb(BAZ_ADR4+5);// если ошибка четности
	}
	else
	{
		inportb(BAZ_ADR4+5);// сбросить все ошибки
		d5=de4&4;// анализ приема ПЭВМ - ПЭВМ
		if(d5==4)test=priem_pvm(BAZ_ADR4);
		else
		{
			d5=de4&2;// анализ передачи ПЭВМ-ПЭВМ
			if((d5==2)&&(PERED_PVM==2))vidacha_pvm(BAZ_ADR4);//если передатчик свободен
			else inportb(BAZ_ADR4+6);//сбрасываем прерывание от модема
		}
	}
	if(BAZ_ADR4!=0)de4=inportb(BAZ_ADR4+2);//проверить нет ли новых прерываний для ПЭВМ-ПЭВМ
	else de4=1;
	if(de4!=1) goto a;// если появились новые прерывания - обработать
b:
	d5=de1&6;//анализ ошибок четности для канала 1-ой стойки
	if(d5==6) inportb(BAZ_ADR1+5);//снять ошибку четности
	else
	{ inportb(BAZ_ADR1+5);//снять ошибку
		d5=de1&4;//проверить наличие приема от 1-ой стойки
		if(d5==4) in_port(BAZ_ADR1);//если что-то пришло, то принять
		else
		{
			d5=de1&2; //если нечего принимать, то проверить свободность передатчика
			if(d5==2)vidacha(BAZ_ADR1);//если можно передать, то выдать символ
			else inportb(BAZ_ADR1+6);// иначе сбросить ошибку линии
		}
	}
	de1=inportb(BAZ_ADR1+2); //прочитать нет ли нового прерывания 1-го ТУМС
	if(de1!=1)goto b;//если такое прерывание возникло, то вернуться
	if(BAZ_ADR11==0) goto b3;// если второй стойки нет, закончить
b2:
	err_tums=inportb(BAZ_ADR11+5)&0xe;
	if(err_tums!=0)
	{
		if((err_tums&2)==2)ERROR_TUMS2_0++;
		if((err_tums&4)==4)ERROR_TUMS2_1++;
		if((err_tums&8)==8)ERROR_TUMS2_2++;

	}

	d5=de11&6;//проверить ошибки четности второй стойки
	if(d5==6)inportb(BAZ_ADR11+5);//если есть, сбросить прерывание модема
	else
	{
		inportb(BAZ_ADR11+5);
		d5=de11&4;//проверить прием
		if(d5==4)in_port(BAZ_ADR11); //если есть прием, прочитать символ
		else//если нет приема
		{
			d5=de11&2;//проверить свободность передатчика
			if(d5==2)vidacha(BAZ_ADR11);//при свободном передатчике выдать символ
			else inportb(BAZ_ADR11+6);// иначе сбросить ошибку линии
		}
	}
	de11=inportb(BAZ_ADR11+2);//прочитать, нет ли нового прерывания от ТУМС 2
	if(de11!=1) goto b2;// если есть, вернуться
	if(BAZ_ADR2==0) goto fin;// если ББКП нет, закончить
b3:
	err_bbkp=inportb(BAZ_ADR2+5)&0xe;
	if(err_bbkp!=0)
	{
		if((err_bbkp&2)==2)ERROR_BBKP_0++;
		if((err_bbkp&4)==4)ERROR_BBKP_1++;
		if((err_bbkp&8)==8)ERROR_BBKP_2++;

	}
	d5=de2&6;//проверить ошибки четности ББКП
	if(d5==6) inportb(BAZ_ADR2+5);//если есть, сбросить прерывание модема
	else
	{
		inportb(BAZ_ADR2+5);
		d5=de2&4;//проверить прием
		if(d5==4) in_port_BBKP(BAZ_ADR2); //если есть прием, прочитать символ
		else//если нет приема
		{
			d5=de2&2;//проверить свободность передатчика
			if(d5==2)
			vidacha_BBKP(BAZ_ADR2);//при свободном передатчике выдать символ
			else inportb(BAZ_ADR2+6);// иначе сбросить ошибку линии
		}
	}
	de2=inportb(BAZ_ADR2+2);//прочитать, нет ли нового прерывания от BBKP
	if(de2!=1) goto b3;// если есть, вернуться
fin:
	de1=inportb(BAZ_ADR1+2);//прочитать 1-ый ТУМС
	if(BAZ_ADR11!=0) de11=inportb(BAZ_ADR11+2);//если есть, прочитать 2-ой ТУМС
	if(BAZ_ADR2!=0) de2=inportb(BAZ_ADR2+2);//если есть, прочитать ББКП
	if(BAZ_ADR4!=0) de4=inportb(BAZ_ADR4+2);//если есть, прочитать ПЭВМ
	if((BAZ_ADR2==0)&&(BAZ_ADR4==0)&&( BAZ_ADR11==0))// если только один ТУМС
	{
		if(de1==1)//если нет прерывания
		{
			outportb(0x20,0x20);// закрыть процедуру
			return;// и выйти
		}else goto b;// иначе вернуться
	}
	if((de1==1)&&( de2==1)&&( de4==1)&&( de11==1))// если по всем каналам ничего нет

	{
		outportb(0x20,0x20);// то закрыть процедуру
//		asm sti
		return;// и выйти
	}
	goto a;// иначе вернуться
}
//----------------------------------------------------------------
/************************************************\
* Процедура проверки значения переменных,        *
* характеризующих состояние связи по каналам АРМ.*
* При достижении пороговых значений пишет о      *
* ненорме в журнал и выдает сообщение на экран.  *
\************************************************/
void proverka_svyazi()
{
	int st,ic,jj;
	nom_func("254");
	if(DISK!=0)return;//при просмотре архива - выход сразу
	for(st=0;st<skoko_stoek;st++)//пройти по всем стойкам
	{
		//работа со сбоями ТС
		if(sboy_ts[st]>5)//если по ТС нет данных более 5 сек
		{
			if(otkaz_ts[st]==0)
			{ //установить признак отказа канала ТС
				otkaz_ts[st]=1;
				slom_interf(7000+st);//записать в журнал
				add(0,0,'A'|(32*st));//записать в архив
				OBMEN_PVM=OBMEN_PVM|(1<<st);
				if(st==0)w(173,999," 1 УВК");
				else w(173,999," 2 УВК");
				//если не было до этого потери связи
				//навязать непарафазность
				if((pusto&(1<<st))==0) //если ранее не фиксировалась потеря связи
				for(ic=0;ic<kol_VO;ic++)
				{
					if((fr1[ic][13]==(st+1))||(fr1[ic][13]==9999))
					{
						fr3[ic][5]=1;
						fr3[ic][10]=0;
						fr3[ic][8]=0;
						fr3[ic][11]=0;
						pooo[ic]=0l;
						pooo_com[ic]=0l;
					}
				}
#ifdef DISPETCHER
				if(st==0)
				for(ic=0;ic<KOL_SOO1;ic++)
				for(jj=4;jj<9;jj++)REG_INF_DC[ic][jj]=0x60;

				if(st==1)
				for(ic=0;ic<KOL_SOO2;ic++)
				for(jj=4;jj<9;jj++)REG_INF_DC[ic+KOL_SOO1][jj]=0x60;
#endif
				zvuk_vkl(3,0);//включить сигнал
			}
			sboy_ts[st]=6;//ограничить число сбоев
			pusto=pusto|(1<<st);//установить признак потери связи
			pusto=pusto|0x1000;
		}
		else
		{
			if(sboy_ts[st]>2)//если по ТС нет данных более 2 сек
			{
				if(otkaz_tu[st]==0)
				{
					otkaz_tu[st]=1;//установить признак отказа
					slom_interf(7010+st);//записать в журнал
					add(0,0,'B'|(32*st));//записать в архив
					OBMEN_PVM=OBMEN_PVM|(4<<st);//передать соседу
					//навязать запрет ТУ
					zapret[st]=zapret[st]|0x8000;
					slom_interf(7020+st);//записать в журнал
					add(0,0,'C'|(32*st));//записать в архив
					if(st==0)w(172,999," 1 УВК");  //Запрет ТУ
					else w(172,999," 2 УВК");
					zvuk_vkl(3,0);//включить сигнал
				}
			}
		}
		//работа со сбоями ТУ
		if(sboy_tu[st]>2)
		{
			if(otkaz_tu[st]==0)//если не было отказа ТУ
			{
				otkaz_tu[st]=1;
				slom_interf(7010+st);//записать в журнал
				add(0,0,'B'|(32*st));//записать в архив
				OBMEN_PVM=OBMEN_PVM|(4<<st);
				if(st==0)w(171,999," 1 УВК"); //Отказ ТУ
				else w(171,999," 2 УВК");
				zvuk_vkl(3,0);//включить сигнал
			}
			sboy_tu[st]=3;
		}
	}
	ic=0;
	for(st=0;st<skoko_stoek;st++)//пройти по всем стойкам
	{ //работа со сбоями ТС
		if(sboy_ts[st]==0)//если по ТС нет сбоя
		{
			if(otkaz_ts[st]!=0)
			{
				otkaz_ts[st]=0;
			//установить признак восстановления канала ТС
				slom_interf(7050+st);//записать в журнал
				add(0,0,'E'|(32*st));//записать в архив
			}
			OBMEN_PVM=OBMEN_PVM&(~(1<<st));//вернуть норму ТС
			OBMEN_PVM=OBMEN_PVM&(~(4<<st));//вернуть норму ТУ
			//если фиксировался отказ и нет запрета ТУ
			if((zapret[st]==0)&&(otkaz_tu[st]!=0)&&(sboy_tu[st]==0))
			{
				otkaz_tu[st]=0;//cнять признак отказа по потере ТС
				//восстановление ТУ
				slom_interf(7060+st);//записать в журнал
				add(0,0,'F'|(32*st));//записать в архив
				if(st==0)w(174,999," 1 УВК");
				else w(174,999," 2 УВК");
				zvuk_vkl(2,0);//включить сигнал
			}
			if(zapret[st]!=0)//если был запрет
			{
				zapret[st]=zapret[st]&0x7fff;
				if(zapret[st]==0)
				{
					slom_interf(7070+st); //внести в журнал
					add(0,0,'D'|(32*st));//записать в архив
					if(st==0)w(174,999," 1 УВК"); //восстановлено ТУ
					else w(174,999," 2 УВК");
					zvuk_vkl(2,0);//включить сигнал
				}
			}
		}
	}
	if(pust_pvm==3) // если прошло ровно 3 сек без обмена с соседней ПЭВМ
	{
		if(NEISPRAVEN==0)
		{
			slom_interf(7300);
			pust_pvm=4;
			NEISPRAVEN=1;
			if(ZAGRUZKA==0)ispr_pvm(1,NEISPRAVEN);
			add(0,0,'Х');
		}
	}
	else
		if(pust_pvm>3)
		{
			pust_pvm=4;// введение ограничения на число секунд
			if(NEISPRAVEN==0)
			{
				NEISPRAVEN=1;
				if(ZAGRUZKA==0)ispr_pvm(1,NEISPRAVEN);
			}
		}
	// сброс флага неисправности канала обмена ПЭВМ-ПЭВМ
	if(pust_pvm==0)
	{
		if(NEISPRAVEN==1)
		{
			slom_interf(7200);
			add(0,0,'х');
		}
		NEISPRAVEN=0;
	}
	return;
}
//****************************************************
void start_port(int a)
{
	nom_func("318");
	if(DISK!=0)return;
	ochistka();
	outportb(BAZ_ADR3+1,0x00); // запрет прерывания COM
	outportb(BAZ_ADR3+4,0x03); // установить DTR и RTS для КОК
	inportb(0x21);
	if(a==1)init();// установка векторов прерываний и настройка портов
	return;
}
//-----------------------------------------------------------
void test_deshifr(int k_ot,int bit_)
{
  if(k_ot!=0)k_ot=0;
  if(bit_!=0)bit_=0;
	nom_func("338");
  //if(STATUS!=1)
	return;
/*
	st=fr1[k_ot][13]-1;//определить индекс стойки
	strcpy(ST," УВК-");
	ST[5]=st+49;ST[6]=0;
	if(fr3[k_ot][bit_]==1) // если по данному биту 1 - сработало реле
	{ //если выполняется работа с маршрутом
		if(MAR_GOT[st]==0x40)return;
		//если выдавалась команда менее, чем за 20 сек
		vv=biostime(0,0l);
		vv=vv-t_com[st];
		if(vv<360l)return;
		// если реле сработало несанкционировано
		if((zapret[st]&0xfff)==0)//если запрет ранее не выдавался
		{ zapret[st]=zapret[st]|k_ot;//установить запрет ответов для канала ТУ
			w(172,k_ot,ST);
			slom_interf(7020+st);
			add(0,'C'|(32*st));
			zvuk_vkl(3,0);
	 }
	 fr3[k_ot][bit_+9]=1;
 }
 else
 { fr3[k_ot][bit_+9]=0;
   if((zapret[st]&0xfff)==0) return;
   k_ot=zapret[st]&0x7fff;
   if(test_sost_(k_ot)==1) return;
   zapret[st]=zapret[st]&0x8000;
	 if((otkaz_tu[st]==0)&&(zapret[st]==0))
   { w(174,999,ST);
		 slom_interf(7070+st);
     add(0,'D'|(32*st));
     zvuk_vkl(2,18);
   }
   return;
 }
 return;
 */
}
//------------------------------------------------
void test_otvetstv()
{
	int cl=0,cll=0;
	nom_func("345");
	if(DISK!=0)return;
	if(MODE_KN==1) return;
	cll=inportb(BAZ_ADR3+6);
	cl=cll&0x80;
	if(cl==0x80)
	{
		OTV_KOM_1++;
		if(OTV_KOM_1>3)OTV_KOM_1=3;
		if(OTV_KOM_1==3)
		{
			OTV_KOM_0=0;
			if(otv_kom_oper==1) return;
			else
			{
				hudo=0; otv_kom_oper=1;
				regs.x.ax=4;
				regs.x.cx=XOK;;
				regs.x.dx=YOK+15;
				int86(0x33,&regs,&regs);
			}
			return;
		}
	}
	else
	{
		cl=cll&0x20;
		if(cl==0x20)
		{
			OTV_KOM_0--;
			if(OTV_KOM_0<-3)OTV_KOM_0=-3;
			if(OTV_KOM_0==-3)
			{
				OTV_KOM_1=0;
				if((otv_kom_oper==0)&&(hudo==0))return;
				else
				{
					hudo=0;
					otv_kom_oper=0;
					zalet=0;
					vtora=0;
					poka=0;
					zapusk=0;
					zapretvybora=0;
				}
				return;
			}
		}
		else
		{
			if(hudo==1) return;
			otv_kom_oper=2;
			hudo=1;
		}
	}
	return;
}
//*********************************
void test_port_pvm()
{
	int cl=0,cll=0,kol_poo,ij;
	nom_func("347");
	if(DISK!=0)return;
	if(new_day==1)return;
#ifndef DEMONSTR
	cll=inportb(BAZ_ADR5+6);//прочитать порт статуса ПЭВМ
	cl=cll&0x80;
	if(cl==0x80)//если ПЭВМ основная
	{ cl=cll&0x20;
		if(cl==0)
		{
#ifndef IMIT
			if(STATUS==1){hudoo=0;return;}// и ранее тоже была основная, то выйти
			STATUS_1++;
			if(STATUS_1>3)STATUS_1=3;
			if(STATUS_1==3)
			{
				STATUS_0=0;
				if((STATUS==0)||(STATUS==2))// если ПЭВМ была резервной или неопределенной
				{ hudoo=0;
					slom_interf(7400);// зафиксировать в протоколе УВК переход в основной
					end_help();
					add(0,0,'Г');// зафиксировать в архиве переход в статус основной
					if((nikuda==0)&&(klo==0)&&(help==0))
					{
						setfillstyle(1,10);
						bar(10,45,32,50);
					}
				}
				for(kol_poo=0;kol_poo<kol_VO;kol_poo++)
				{
					pooo[kol_poo]=0;
					pooo_com[kol_poo]=0;
				}
				STATUS=1;
				menu();
			}
#else
	STATUS=1;
#endif
		}
	}
	else//если ПЭВМ резервная
	{
		cl=cll&0x20;
		if(cl==0x20)
		{
#ifndef IMIT
			STATUS_0--;
			if(STATUS_0<-3)STATUS_0=-3;
			if(STATUS_0==-3)
			{
				STATUS_1=0;
				if(STATUS==0){hudoo=0;return;}
				if((STATUS==1)||(STATUS==2))//если резервная
				{
					hudoo=0;
					snyato_nechet();
					snyato_chet();
					slom_interf(7500);
					add(0,0,'Д');
				}
				if((nikuda==0)&&(klo==0)&&(help==0))
				{setfillstyle(1,14);bar(10,45,32,50);}
				STATUS=0;
				menu();
				for(ij=0;ij<kol_VO;ij++)
				{
					fr3[ij][10]=0;fr3[ij][8]=0;
					if(fr1[ij][0]==1)displaystrelka(ij,0,0);
					if(fr1[ij][0]==3)sekci(ij,9999);
					if(fr1[ij][0]==4)displaypathuch(ij);
					if(fr1[ij][0]==5)displaypath(ij,10);
				}
				for(ij=0;ij<skoko_stoek*3;ij++)
				{
					if(MARSHRUT_ALL[ij].NACH!=0)DeleteMarsh(ij);
				}
			}
#else
	STATUS=1;
#endif
pro:
		}
		else//если статус неопределен
		{
#ifndef IMIT
			if((STATUS_1==0)&&(STATUS_0==0))
			{
				if(hudoo<1000)hudoo++;
				else hudoo=1001;
				if(hudoo==1000)
				{
					STATUS=2;
					sound(700);
					slom_interf(7600);
					add(0,0,'Ш');
					prorisovka=0;
					w(169,999," СТАТУС ПЭВМ");
					hudoo=1001;
				}
			}
#else
	STATUS=1;
#endif
		}
	}
#endif
	return;
}

//--------------------------------------------------------
void test_mo()
{
	nom_func("341");
  if(DISK!=0)return;
  if((pusto&3)!=0)//если потеряна связь
  {
    tiiq=biostime(0,0L);//получить время
    if((tiiq-tii)>2020L) //если от пуск прошло более 2 минут
    {
      outportb(BAZ_ADR1+4,0xa);
			switch(skoko_stoek)
      {
        case 2: outportb(BAZ_ADR11+4,0xa);break;
        default: break;

      }
      tii=tiiq; //переопределить время
    }
    if((tiiq-tii)>9) //если от пуск прошло более 2 минут
    {
      outportb(BAZ_ADR1+4,0xb);
      switch(skoko_stoek)
      {
        case 2: outportb(BAZ_ADR11+4,0xb);break;
        default: break;
      }
		}
	}
	return;
}
//---------------------------------------
//процедура обработки символа, принятого от соседней ПЭВМ
int what_symb(unsigned char BO)
{
	int it;
	nom_func("411");
	if(DISK!=0)return(0);
	if(BUF_INF[2]!=BO)return(0xff); //если данный в байтах информации разные
	if((BO&0xC0)==0xC0)goto mfr4;  //если запрос на ограничения
	//Анализ данных по состоянию соседней ПЭВМ
	for(it=0;it<skoko_stoek;it++)
	{ if(((1<<it)&BO)!=0)
		{ otkaz_sos[it]=1;
			if(fix_ts_sos[it]==0)//если не было фиксации отказа ТС соседней ПЭВМ
			{ add(0,0,'Q'|(32*it));
				slom_interf(7100+it);
				fix_ts_sos[it]=1;
			}
		}
		else
		{ otkaz_sos[it]=0;
			if(fix_ts_sos[it]==1)//если не было фиксации восстановления ТС соседа
			{ add(0,0,'U'|(32*it));
				slom_interf(7150+it);
				fix_ts_sos[it]=0;
			}
		}
		if(((4<<it)&BO)!=0)
		{ otkaz_tu_sos[it]=1;
			if(fix_tu_sos[it]==0) //если не было фиксации отказа ТУ соседа
			{ add(0,0,'R'|(32*it));
				slom_interf(7110+it);
				fix_tu_sos[it]=1;
			}
		}
		else
		{ otkaz_tu_sos[it]=0;
			if(fix_tu_sos[it]==1)//если не было фиксации восстановления ТУ соседа
			{ add(0,0,'V'|(32*it));
				slom_interf(7160+it);
				fix_tu_sos[it]=0;
			}
		}
	}
	//
	if((BO&0x10)==0x10)//если реконфигурация 1-го канала соседа
	{ if(fix_rek_sos[0]==0)//если нет фиксации реконфигурации 1-го канала соседа
		{ fix_rek_sos[0]=1;
			add(0,0,'Ю');
			slom_interf(7940);
		}
	}
	else if(fix_rek_sos[0]==1)fix_rek_sos[0]=0;
#ifdef KOL_SOO2
	if((BO&0x20)==0x20)//если реконфигурация 2-го канала соседа
	if(fix_rek_sos[1]==0)
	{ fix_rek_sos[1]=1;
		add(0,0,'ю');
		slom_interf(7950);
	}
	else if(fix_rek_sos[1]==1)fix_rek_sos[1]=0;
#endif
	//если запрос об автодействии
	if(((BO&0xC0)==0x40)&&(STATUS==1))form_kom_avtod('A');
	//если запрос ограничений по fr4
mfr4:
	if((BO&0xC0)==0xC0)
	{ if((STATUS==1)&&(pusto==0))
		{ NOMER_FOR_FR4=form_fr4(0);
			ZAPROS_FR4=1;
			MY_FR4=1;
		}
	}
	add(0,0,BO);
	return(0);
}
//-----------------------------------------------------------------------
/*****************************************************\
* Процедура заполнения регистра передачи ограничений  *
* номером объекта и маской его ограничений, начиная с *
* объекта №os. При нахождении первого же существующего*
* ограничения выполняется функция и возвращается номер*
* следующего объекта, если нет ограничения,то сканиру-*
* ется список до наличия ограничения или до конца.    *
\*****************************************************/
int form_fr4(int os)
{
  unsigned char mask=0;
  int n_0,n_1,n_2,n_3,i,j;
	nom_func("72");
	//пройти от объекта os до конца
  for(i=os;i<kol_VO;i++)
  {
    j=(fr4[i][0]&7)+fr4[i][1]; //получить код ограничений для объекта
    j=j+fr4[i][2];
    if(j!=0)break; //если нет ограничений - прерваться
  }
  if(i==kol_VO) //если дошли до конца объектов
  {
    for(j=1;j<5;j++)REG_FR4[j]=0x23; //заполнить регистр "решетками" #
  }
  else //если не все объекты пройдены
  {
    n_3=i/1000; //получить цифру тысяч
    n_2=i%1000; //получить остаток
    n_1=n_2%100; //получить цифру сотен
    n_2=n_2/100; //получить остаток
    n_0=n_1%10;  //получить цифру единиц
    n_1=n_1/10;  //получить цифру десятков
    REG_FR4[2]=~((n_3<<4)|n_2); //заполнить номер объекта
    REG_FR4[1]=~((n_1<<4)|n_0);
    mask=0;
    //заполнить маску ограничений, наложенных на объект
    if((fr4[i][0]&1)==1)mask=mask|0x01;
    if(fr4[i][1]==1)mask=mask|0x02;
    if(fr4[i][2]==1)mask=mask|0x04;
    mask=mask|0x80;
    REG_FR4[3]=mask;
		REG_FR4[4]=0x80;
  }
  REG_FR4[0]=0x18;
  REG_FR4[5]=0x19;
  return(i+1);  //вернуть номер следующего объекта за переданным
}
//--------------------------------------------------------------
/***************************************************\
* Процедура упаковки ограничений для объекта os, или*
* следующего за ним, для которого есть ограничения. *
* Процедура возвращает номер объекта, для которого  *
* надо далее проверить наличие ограничений.         *
\***************************************************/
int upak_fr4(int os)
{
	unsigned int n_0,n_1,n_2,n_3,i,j,mask;
	nom_func("384");
	mask=0;
	for(i=os;i<kol_VO;i++) //пройти от заданного объекта до конца
	{
		j=fr4[i][0]+fr4[i][1]; //собрать все ограничения
		j=j+fr4[i][2];
		if(j!=0)break;         //если есть ограничения, то прервать цикл
	}
	//если дошли до конца объектов, то заполнить символами ####
	if(i==kol_VO)for(j=1;j<5;j++)REG_FR4[j]=0x23;
	else //если есть очередное ограничение
	{
		n_3=i/1000; n_2=i%1000; n_1=n_2%100;
		n_2=n_2/100;n_0=n_1%10; n_1=n_1/10;
		REG_FR4[2]=~((n_3<<4)|n_2);
		REG_FR4[1]=~((n_1<<4)|n_0);  //заполнить номер объекта
		mask=0;
		if(fr4[i][0]!=0)mask=fr4[i][0];
		if(fr4[i][1]==1)mask=mask|0x200;//макет
		if(fr4[i][2]==1)mask=mask|0x100;//ограждение
		REG_FR4[3]=(mask&0xff)|0x80;
		REG_FR4[4]=((mask&0xff00)>>8)|0x80;
	}
	REG_FR4[0]=0x18;
	REG_FR4[5]=0x19;
	return(i+1);
}
//------------------------------------------------------------
/*****************************\
* Процедура приема и анализа  *
* данных из канала ПЭВМ-ПЭВМ  *
\*****************************/
void what_is_new()
{
	int jj,n_bait;
	n_bait=11;
	nom_func("410");
	if(DISK!=0)return;
	// если получена квитанция на информацию
	if((END_KVIT_INF==1)&&(BUF_IN_PVM[UKAZ_KVIT_INF]==0x11))
	{ if((BUF_IN_PVM[UKAZ_KVIT_INF+2]!=0x10)||
		(BUF_IN_PVM[UKAZ_KVIT_INF+1]!=REG_INF[2]))MY_INF=1;
		else
		{ POVTOR_INF=0;
			for(jj=0;jj<4;jj++)REG_INF[jj]=0;
		}
		END_KVIT_INF=0;UKAZ_KVIT_INF=0;
	}
	// если получена квитанция на команду
	if((END_KVIT_COM==1)&&(BUF_IN_PVM[UKAZ_KVIT_COM]==0x24))
	{ if((BUF_IN_PVM[UKAZ_KVIT_COM+2]!=0x15)||
		(BUF_IN_PVM[UKAZ_KVIT_COM+1]!=REG_COM[n_bait-2]))MY_COM=1;
		else
		{ POVTOR_COM=0;
			MY_COM=0;
			for(jj=0;jj<n_bait;jj++)REG_COM[jj]=0;
		}
		END_KVIT_COM=0;UKAZ_KVIT_COM=0;
	}
	// если получена квитанция на ограничения
	if((END_KVIT_FR4==1)&&(BUF_IN_PVM[UKAZ_KVIT_FR4]==0x1a))
	{ if((BUF_IN_PVM[UKAZ_KVIT_FR4+2]!=0x1b)||  // если нет конца или
		(BUF_IN_PVM[UKAZ_KVIT_FR4+1]!=REG_FR4[4]))// нет контрольной суммы
		MY_FR4=1; // то повторить передачу
		else
		{ for(jj=0;jj<6;jj++)REG_FR4[jj]=0;
			if(BUF_IN_PVM[UKAZ_KVIT_FR4+1]=='#')ZAPROS_FR4=0;
			else  ZAPROS_FR4=1;
			MY_FR4=0;
			POVTOR_FR4=0;
		}
		END_KVIT_FR4=0;UKAZ_KVIT_FR4=0;
		if((ZAPROS_FR4!=0)&&(MY_FR4==0))
		{ NOMER_FOR_FR4=form_fr4(NOMER_FOR_FR4);
			add(5,0,0xf);
			MY_FR4=1;
		}
	}
	// если получена информация
	if((END_INF==1)&&(BUF_IN_PVM[UKAZ_INF]==0x1e))
	{ //считать данные
		for(jj=0;jj<4;jj++)BUF_INF[jj]=BUF_IN_PVM[UKAZ_INF+jj];
		// если нет знака закрытия или не совпадают информативные байты,послать запрос
		if((BUF_INF[3]!=0x1f)||(BUF_INF[1]!=BUF_INF[2]))
		{ REG_KVIT_INF[1]=0x13;
			IT_IS_INF=0;
		}
		else
		{ REG_KVIT_INF[1]=BUF_INF[1];// иначе сформировать штатную квитанцию
			IT_IS_INF=1;
		}
		REG_KVIT_INF[0]=0x11;REG_KVIT_INF[2]=0x10;
		KVIT_INF=1;END_INF=0;UKAZ_INF=0;
	}
	// если полученa команда
	if((END_COM==1)&&(BUF_IN_PVM[UKAZ_COM]==0x28))
	{ for(jj=0;jj<n_bait;jj++)BUF_COM_PVM[jj]=BUF_IN_PVM[UKAZ_COM+jj];
		if((BUF_COM_PVM[n_bait-1]!=0x29)||(BUF_COM_PVM[n_bait-2]!=check_sum(BUF_COM_PVM,0)))REG_KVIT_COM[1]=0x13;
		else
		{ REG_KVIT_COM[1]=BUF_COM_PVM[9];
			IT_IS_COM_PVM=1;
		}
		REG_KVIT_COM[0]=0x24;REG_KVIT_COM[2]=0x15;
		KVIT_COM=1;END_COM=0;UKAZ_COM=0;
	}
	// если получено ограничение по fr4
	if((END_FR4==1)&&(BUF_IN_PVM[UKAZ_FR4]==0x18))
	{ for(jj=0;jj<6;jj++)BUF_FR4[jj]=BUF_IN_PVM[UKAZ_FR4+jj];
		if(BUF_FR4[5]!=0x19)REG_KVIT_FR4[1]=0x13;
		else
		{ REG_KVIT_FR4[1]=BUF_FR4[4];
			IT_IS_FR4=1;
		}
		REG_KVIT_FR4[0]=0x1a;REG_KVIT_FR4[2]=0x1b;
		KVIT_FR4=1;END_FR4=0;UKAZ_FR4=0;
	}
	if(IT_IS_COM_PVM==1)
	{ if(ischem()!=0)kom_v_bufer_pvm(0,'Щ');
		IT_IS_COM_PVM=0;
		for(jj=0;jj<n_bait;jj++)BUF_COM_PVM[jj]=0;
	}
	if(IT_IS_FR4==1)
	{ if(rasp_fr4()==0)
		{ OBMEN_PVM=0x40;
			OBMEN_PVM_OLD=0x40;
			kom_v_bufer_pvm(0,0x40);
		}
		else N_OGR++;
		IT_IS_FR4=0;
		for(jj=0;jj<6;jj++)BUF_FR4[jj]=0;
	}
	if(IT_IS_INF==1)
	{ if(what_symb(BUF_INF[1])!=0)kom_v_bufer_pvm(0,'Щ');
		IT_IS_INF=0;
		for(jj=0;jj<4;jj++)BUF_INF[jj]=0;
	}
	return;
}
//---------------------------------------------------------------------
/***************************************************\
*  Процедура распаковки данных об ограничениях FR4, *
*  полученных по каналу от основной ПЭВМ, установка *
*  этих ограничений и фиксация в архиве             *
\***************************************************/
int rasp_fr4()
{
	int os=0,c0,c1,c2,c3;
	unsigned char ura=0;
	nom_func("263");
	if(DISK!=0)return(0);
	//если получена метка окончания передачи массива ограничений FR4 (###)
	if((BUF_FR4[1]=='#')&&(BUF_FR4[2]=='#')&&(BUF_FR4[3]=='#'))
	{
		PRIEM_FR4=0;
		return(0);
	}

	BUF_FR4[2]=~BUF_FR4[2];BUF_FR4[1]=~BUF_FR4[1];

	c0=BUF_FR4[1]&0xf;
	if((c0<0)||(c0>9))return(0xFF);

	c1=(BUF_FR4[1]>>4)&0xf;
	if((c1<0)||(c1>9))return(0xFF);

	c2=BUF_FR4[2]&0xf;
	if((c2<0)||(c2>9))return(0xFF);

	c3=(BUF_FR4[2]>>4)&0xf;
	if((c3<0)||(c3>9))return(0xFF);

	os=c0+c1*10+c2*100+c3*1000; //вычислить номер объекта
	if(os>=kol_VO)return(0xff);

	ura=BUF_FR4[3]&0x01; //получить код ограничений первого бита
	if(ura==1)fr4[os][0]=fr4[os][0]|1;
	else fr4[os][0]=fr4[os][0]&0xfe;

	ura=BUF_FR4[3]&0x02; //код ограничений второго бита
	if(ura==0x02)fr4[os][1]=1;
	else fr4[os][1]=0;

	ura=BUF_FR4[3]&0x04; //код ограничений третьего бита
	if(ura==0x04)fr4[os][2]=1;
	else fr4[os][2]=0;

	upak_fr4(os);  //упаковать ограничения для объекта os
								 //в регистр записи REG_FR4 для фиксации в архиве
	add(5,0,0xf);//фиксация принятого ограничения
	return(os);
}
//---------------------------------------------------------------------
/*****************************************\
* Процедура приема и анализа символа      *
* данных из порта обмена с соседней ПЭВМ. *
* adr - базовый адрес порта обмена с ПЭВМ.*
\*****************************************/
int priem_pvm(int adr)
{
	unsigned char simbol=0;
	int test;
	nom_func("233");
	// прочитать символ из порта ввода
	if(DISK!=0)return(0); //при просмотре архива - выход
	test=0;
	simbol=inportb(adr);
	if(simbol==0)return(0);
	if(simbol=='*')
	{
		// сброс счетчика секунд простоя обмена ПЭВМ-ПЭВМ
		pust_pvm=0;
		return(0);
	}
	else
	{
		BUF_IN_PVM[UKAZ_PRIEMA_PVM]=simbol;

	}
	switch(BUF_IN_PVM[UKAZ_PRIEMA_PVM])
	{
		case 0x1e: UKAZ_INF=UKAZ_PRIEMA_PVM;break;// - символ начала информации
		case 0x1f: END_INF=1;test=1;break;// - символ конца информации

		case 0x18: UKAZ_FR4=UKAZ_PRIEMA_PVM;break; //стр.вверх-символ начала FR4
		case 0x19: END_FR4=1;test=1;break;         //стр.вниз - символ конца FR4

		case 0x28: UKAZ_COM=UKAZ_PRIEMA_PVM;break;// ( - символ начала команды
		case 0x29: END_COM=1;test=1;break;        // ) - символ конца команды

		case 0x11: UKAZ_KVIT_INF=UKAZ_PRIEMA_PVM;break; //стр.влево - начало квит.
		case 0x10: END_KVIT_INF=1;test=1;break; //стр.вправо - конец квитанции инф.

		case 0x1a: UKAZ_KVIT_FR4=UKAZ_PRIEMA_PVM;break; //стр.вправо - нач.квит.FR4
		case 0x1b: END_KVIT_FR4=1;test=1;break;//стр.влево - конец квит. FR4

		case 0x24: UKAZ_KVIT_COM=UKAZ_PRIEMA_PVM;break;// $ - начало квит.команды
		case 0x15: END_KVIT_COM=1;test=1;break;        //  - конец квит. команды
		default:  break;
	}
	// если есть символ конца посылки и до конца буфера менее 10,
	//  то перейти на нулевую
	if((MAX_SIZE_IN<(UKAZ_PRIEMA_PVM+11))&&(test==1))UKAZ_PRIEMA_PVM=0;
	// если нет символа конца посылки, то перейти к следующей
	else UKAZ_PRIEMA_PVM++;
	return(1);

}
//------------------------------------------------------------------------
/*************************************************\
* Процедура поиска объекта, по которому получена  *
* команда из соседней ПЭВМ.                       *
* Фиксация команды на диске и формирование ограни-*
* чений.                                          *
* Данные из соседней ПЭВМ приняты в BUF_COM_PVM.  *
\*************************************************/
int ischem()
{
  int oo=0,uu=0,n_obek=9999,nnob=9999,Zna=0,Rezik=0,POD=9999,Sdvig=0,FN=0,FIN=0;
  int KJB=0,zapis=0;
	nom_func("110");
  if(DISK!=0)return(1);
  KJB=BUF_COM_PVM[1]&0x02;//определить для какой стойки
#ifdef KOL_SOO2
  if(KJB==2)//если для второй
	{
    if(BUF_COM_PVM[2]=='I')//если данные получены для путей
    {
      Sdvig=STR2+SIG2+DOP2+UCH2;FN=PUT1;FIN=FN+PUT2;
    }
    if(BUF_COM_PVM[2]=='F')//если данные для СП или для участков путей
		{
      Sdvig=STR2+SIG2+DOP2;FN=UCH1;FIN=FN+UCH2;
    }
    if((BUF_COM_PVM[2]=='C')||(BUF_COM_PVM[2]=='S'))//если данные для стрелок
    {
      Sdvig=0;FN=STR1;FIN=FN+STR2;
    }
  }
#endif
  if(KJB==0)//если для второй стойки
  {
    if(BUF_COM_PVM[2]=='I')//если принято для путей
		{
      Sdvig=STR1+SIG1+DOP1+UCH1;FN=0;FIN=PUT1;
    }
		if(BUF_COM_PVM[2]=='F') //если данные для СП или УП
    {
      Sdvig=STR1+SIG1+DOP1;FN=0;FIN=UCH1;
    }
    if((BUF_COM_PVM[2]=='C')||(BUF_COM_PVM[2]=='S'))//если данные для стрелки
    {
      Sdvig=0;FN=0;FIN=STR1;
    }
  }
  switch(BUF_COM_PVM[2])//переключение по полученным командам
	{
#ifdef AVTOD
		case 'A': //если автодействие
		case 'Y':	//если отключается
							if(BUF_COM_PVM[3]=='@'){na=0;cha=0;}
							else//если нечетное автодействие
								if(BUF_COM_PVM[3]=='A'){na=1;cha=0;}
								else
									if(BUF_COM_PVM[3]=='B'){na=0;cha=1;}
									else
										if(BUF_COM_PVM[3]=='C'){na=1;cha=1;}
							if(na==1)for(uu=0;uu<N_str;uu++)otkl_(N_strel[uu],1);
							else for(uu=0;uu<N_str;uu++)vkl_(N_strel[uu],1);
							if(cha==1)for(uu=0;uu<CH_str;uu++)otkl_(C_strel[uu],0);
							else for(uu=0;uu<CH_str;uu++) vkl_(C_strel[uu],0);
							if(ZAGRUZKA==0)
							{
								if(na==1)vkl_kno(NANA,10);//кнопку сделать зеленой
								else vkl_kno(NANA,8);//если не загрузка-кнопку сделать серой
								if(cha==1)vkl_kno(CHACHA,10);
								else  vkl_kno(CHACHA,8);
							}
							break;
#endif
    case 'I': for(oo=FN;oo<FIN;oo++)//если работа с путем,пройти по подгруппам
							{ if(BUF_COM_PVM[3]==podgruppa[oo+Sdvig-FN])//если найдена подгруппа
                {POD=oo;break;}//запомнить подгруппу
              }
              if(POD==9999)return(1);//если подгруппа не найдена - выход
              for(uu=0;uu<5;uu++)//пройти по байтам сообщения
              { if(BUF_COM_PVM[uu+4]!=124)//если в байте не заполнитель
                { n_obek=spputi[POD][uu];//получить номер объекта
                  Zna=BUF_COM_PVM[uu+4];//получить для объекта маску
                  goto fina;
                }
              }
fina:         if(n_obek==9999) return(1);//если объект не найден - выход
              Rezik=Zna&0x01;//проверить бит 0-й
              if(Rezik==0x01) fr4[n_obek][0]=1;//установлен-установить ограничение
							else fr4[n_obek][0]=0;//не установлен - снять ограничение
              Rezik=Zna&0x02;//проверить 1-й бит и выполнить то же самое
              if(Rezik==0x02) fr4[n_obek][1]=1;
              else fr4[n_obek][1]=0;
              Rezik=Zna&0x04;//повторить то же для 2-го бита
              if(Rezik==0x04) fr4[n_obek][2]=1;
              else fr4[n_obek][2]=0;
              if(ZAGRUZKA==0)displaypath(n_obek,0);//нарисовать путь
              break;
    case 'F': for(oo=FN;oo<FIN;oo++)//если СП или УП - пройти по группе
              { if(BUF_COM_PVM[3]==podgruppa[oo+Sdvig-FN])
                { POD=oo;break;} //если найдена подгруппа - запомнить
              }
              if(POD==9999) return(1);//если подгруппа не найдена - выход
							for(uu=0;uu<5;uu++)//пройти по байтам сообщения
              { if(BUF_COM_PVM[uu+4]!=124)//если в байте не заполнитель
                { n_obek=spspu[POD][uu];//запомнить объект
                  Zna=BUF_COM_PVM[uu+4];//читать установку/снятие ограничений
                  goto finak;
                }
              }
finak:        if(n_obek==9999) return(1);
              if(Zna==84){zapis=1;break;}
              Rezik=Zna&0x01;
              if(Rezik==0x01)fr4[n_obek][0]=1;
              else fr4[n_obek][0]=0;
              Rezik=Zna&0x02;
              if(Rezik==0x02)fr4[n_obek][1]=1;
              else fr4[n_obek][1]=0;
              Rezik=Zna&0x04;
              if(Rezik==0x04)fr4[n_obek][2]=1;
              else fr4[n_obek][2]=0;
              if(ZAGRUZKA==0)displaypathuch(n_obek);
              break;
    case 'S': for(oo=FN;oo<FIN;oo++)//аналогично для стрелок
							{ if(BUF_COM_PVM[3]==podgruppa[oo+Sdvig-FN])
                { POD=oo; break;}
              }
              if(POD==9999) return(1);
              for(uu=0;uu<5;uu++)
              { if(BUF_COM_PVM[uu+4]!=124)
                { n_obek=spstr[POD][uu];
                  Zna=BUF_COM_PVM[uu+4];
                  goto fena;
                }
              }
              //если ограничения на дополнительную
fena:         TEST_SPAR=2;
							if(fr1[n_obek][12]!=9999)nnob=poisk_ras(fr1[n_obek][2],n_obek,0,0);
              if(n_obek==9999)return(1);

              Rezik=Zna&0x01;//ограничения для управления
              if(Rezik==0x01)fr4[n_obek][0]=fr4[n_obek][0]|1;
              else fr4[n_obek][0]=fr4[n_obek][0]&0xfffe;

              Rezik=Zna&0x02;//макет
							if(Rezik==0x02)
              {
                fr4[n_obek][1]=1;
                if(nnob!=9999)fr4[nnob][1]=1;
              }
              else
              {
                fr4[n_obek][1]=0;
                if(nnob!=9999)fr4[nnob][1]=0;

							}
#ifdef MAKET
							if(ZAGRUZKA==0)vkl_kno(MAKET,8);
#endif
							Rezik=Zna&0x04;//запрет для проезда
              if(Rezik==0x04)//если принято ограждение
                if((Zna&0x8)==0)fr4[n_obek][2]=1; //если для основной стрелки
                else fr4[nnob][2]=1;//если для спаренной
              else//если принято снятие ограждения
                if((Zna&0x8)==0)fr4[n_obek][2]=0;//если для основной стрелки
                else fr4[nnob][2]=0;//если для спаренной стрелки

              if(nnob!=9999)//если спаренная стрелка
              {
                fr4[nnob][0]=fr4[n_obek][0];//совместить управление
                fr4[nnob][1]=fr4[n_obek][1]; //совместить макет
              }
              nach_zakr(n_obek,1);
              if(nnob!=9999)nach_zakr(nnob,1);
							break;
		case 'L': zapis=1;break;//для команд контролеру (перезапуск)
		case 'C': zapis=1;break;//для разовых команд на стрелку
		case 'E': zapis=1;break;//для сигналов - ничего не делать,кроме записи
		case 'Q': zapis=1;break;//для доп.объектов ничего не делать, кроме записи
		case 'i': zapis=1;break;//для маршрутов ничего не делать,кроме записи
		case 'j': zapis=1;break;//для маршрутов ничего не делать,кроме записи
		case 'a': zapis=1;break;//для маршрутов ничего не делать,кроме записи
		case 'd': zapis=1;break;//для РМ ничего не делать,кроме записи
		case 'b': zapis=1;break;
		default:  return(1);
	}
	if(zapis==0)//если признак записи сброшен
	{
		disco();//записать ограничения
		if(MY_COM==0)add(2,0,'>'); //если команды нет записать информацию
	}
	if(zapis==1)add(2,0,'<');//если есть признак записи - записать команду
	return(0);
}
//*************************************************************************
void sbros_obmen_pvm()
{
	int i,n_bait;
	nom_func("292");
	n_bait=11;
	if(DISK!=0)return;
	w(160,999,"");// не выполнена передача данных соседу
	UKAZ_ZAP_OUT=0; UKAZ_VYVOD=0;
	for(i=0;i<MAX_SIZE_OUT;i++)BUF_OUT_PVM[i]=0;// очистить буфер
	for(i=0;i<MAX_SIZE_IN;i++)BUF_IN_PVM[i]=0;// очистить буфер
	MY_COM=0;MY_INF=0;MY_FR4=0;POVTOR_COM=0;POVTOR_INF=0;POVTOR_FR4=0;
	UKAZ_INF=0;END_INF=0;UKAZ_FR4=0;END_FR4=0;UKAZ_COM=0;END_COM=0;
	UKAZ_KVIT_INF=0;END_KVIT_INF=0;UKAZ_KVIT_FR4=0;END_KVIT_FR4=0;
	UKAZ_KVIT_COM=0;END_KVIT_COM=0;
	IT_IS_COM_PVM=0;IT_IS_FR4=0;IT_IS_INF=0;
	UKAZ_PRIEMA_PVM=0;
	for(i=0;i<4;i++)
	{
		REG_INF[i]=0;BUF_INF[i]=0;
	}
	for(i=0;i<3;i++)
	{
		REG_KVIT_INF[i]=0;REG_KVIT_COM[i]=0;REG_KVIT_FR4[i]=0;
	}
	for(i=0;i<6;i++)
	{
		REG_FR4[i]=0;BUF_FR4[i]=0;
	}
	for(i=0;i<n_bait;i++)
	{
		REG_COM[i]=0;BUF_COM_PVM[i]=0;
	}
	KVIT_INF=0;KVIT_COM=0;KVIT_FR4=0;
	return;
}
//----------------------------------------------------------------
/*********************************************************\
* Процедура записи команды в регистр команд для выдачи    *
* в канал ТУМС. Вызывается из процедуры consentr в        *
* каждом цикле при наличии команды введенной оператором.  *
* При отсутствии свободных регистров команд-новая команда *
* теряется и формируется признак "отказ ТУ"               *
\*********************************************************/
void kom_v_bufer(int st)
{
	int i,n_bait;
	nom_func("127");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;
	//if(REG_COM_TUMS[st][0]!='(')//если в ренистре команд нет команды
#ifdef KOL_SOO2
	if((fr3[KM1][BitKM]!=0)||(fr3[KM2][BitKM]!=0)||
	(fr3[KM1][5]==1)||(fr3[KM1][5]==1))
#else
	//если макет установлен или непарафазен
	if((fr3[KM1][BitKM]!=0)||(fr3[KM1][5]==1))
#endif
	{
		if(strelka_v_makete()==0)
		{
			w(257,999,""); //"Не указана стрелка для включенного макета"
			goto met1;
		}
	}

	for(i=0;i<n_bait;i++)
	{
		REG_COM_TUMS[st][i]=buf_ko[i];//переписать команду в буфер команд ТУМСа
		buf_zap_disk[i]=buf_ko[i];
		buf_ko[i]=0;
	}

	if((REG_COM_TUMS[st][0]=='(')&&(REG_COM_TUMS[st][n_bait-1]==')'))
	{//если есть командa
		if(N_RAZ[st]>=3)
		{
met1: for(i=0;i<n_bait;i++)buf_ko[i]=REG_COM_TUMS[st][i]=0;
			N_RAZ[st]=0;
			flagoutmsg=0;
			return;
		}
		else N_RAZ[st]++;//записать число сделанных передач
		t_com[st]=biostime(0,0l);//запомнить время выдачи
//    whole_finish();

		for(i=0;i<n_bait;i++)//скопировать команду в буфер вывода
		{ if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//если до конца
			 BUF_OUT[st][ukaz_zap[st]++]=REG_COM_TUMS[st][i];//записать
		}

		if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//если до конца
		add(st,0,'>');
		if(flagoutmsg!=11)flagoutmsg=0;
		else flagoutmsg=10;
	}
	else return;
/********
		if((biostime(0,0l)-t_com[st])>18l)//если команда выдавалась
		{
			N_RAZ[st]++;//записать число сделанных передач
			t_com[st]=biostime(0,0l);//запомнить время выдачи
			for(i=0;i<11;i++)//скопировать команду в буфер вывода
			{ if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//если до конца
				BUF_OUT[st][ukaz_zap[st]++]=REG_COM_TUMS[st][i];//записать
				if(N_RAZ[st]>=3)REG_COM_TUMS[st][i]=0;
			}
			if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//если до конца
			if(N_RAZ>=3){N_RAZ[st]=0;t_com[st]=0;}
		}
***********/
	if(st==0)outportb(BAZ_ADR1+1,3);
	if(st==1)outportb(BAZ_ADR11+1,3);
	//for(i=0;i<11;i++)buf_ko[i]=0;//освободить промежуточный буфер команд
	otkaz_tu[st]=0;
	return;
}
//************************************************************
//процедура выдачи информации из буфера в линию
//вызывается из обработчика прерываний при свободности регистра вывода
void vidacha(int adr)
{
	int st;
/*
	nom_func("389");
*/
  if(DISK!=0)return;
  if(adr==BAZ_ADR1)st=0;
  else
    if(adr==BAZ_ADR11)st=1;
    else return;
  if((ukaz_vyd[st]==ukaz_zap[st])||(BUF_OUT[st][ukaz_vyd[st]]==0))
  { ukaz_zap[st]=0;
    ukaz_vyd[st]=0;
  }
  else
  { outportb(adr,BUF_OUT[st][ukaz_vyd[st]]);
    BUF_OUT[st][ukaz_vyd[st]]=0;
    ukaz_vyd[st]++;
    if(ukaz_vyd[st]>=SIZE_BUF_OUT)ukaz_vyd[st]=0;
  }
  return;
}
//***************************************
//процедура вывода в порт обмена данных из буфера вывода
//вызывается из обработчика прерывания при свободности регистра вывода
void vidacha_BBKP(int adr)
{
/*
	nom_func("390");
*/
#ifdef DISPETCHER
		
	if(STATUS==0)outportb(BAZ_ADR2,0);
	if(DISK!=0)return;
	if(adr!=BAZ_ADR2)return;
	else
	if((ukaz_vyd_dc==ukaz_zap_dc)||(BUF_OUT_DC[ukaz_vyd_dc]==0))
	{
		outportb(adr+1,1);//закрыть прерывания передачи
		return;
	}
	else
	{
		outportb(adr,BUF_OUT_DC[ukaz_vyd_dc]);
		BUF_OUT_DC[ukaz_vyd_dc]=0;
		ukaz_vyd_dc++;
		if(ukaz_vyd_dc>=SIZE_BUF_OUT_DC)ukaz_vyd_dc=0;
	}
#endif
	return;
}
//***************************************
//обработка полученных ответов ТУМС
//вызывается в каждом цикле из процедуры main
/*
otvety_TUMS()
{
  int st,ij,i;
  for(st=0;st<skoko_stoek;st++)//пройти по всем стойкам.
  {
    if(REG_OT[st][0]==0)break;//если ответ пустой-идти дальше.
    else//если не обработанный ответ.
    { if(REG_OT[st][4]=='P')//если ответ в норме.
      { if(DU==1)//если ДУ
        { if(est_com_dc(st,ij)==0)//если была найдена и сброшена команда ДЦ
metka_c:  REG_OT[st][4]='P';goto metka_a;//"хорошая" заготовка для ответа в ДЦ
          }
          else {otkaz_tu[st]=1;goto metka_b;}//"левое" подтверждение
        }
        else//если не ДЦ,
        { if(SEZON==1)//если сезонное управление
          { if(est_com_dc(st)==0)goto metka_c;//если была команда ДЦ
            else goto metka_d;// возможно это команда от АРМа
          }
          else goto metka_d;//если управление от АРМа
        }
      }
      else //если ответ не в норме
      { 
metka_b:
        REG_OT[st][4]='?';//Плохая заготовка для передачи в ДЦ
      }
metka_a:
      for(i=0;i<6;i++)
      {
				if(ukaz_zap_dc>SIZE_BUF_OUT_DC)ukaz_zap_dc=0;
				if(BUF_OUT_DC[ukaz_zap_dc]==0)
        {
					BUF_OUT_DC[ukaz_zap_dc]=REG_OT[st][i];//отправить ответ в ДЦ
          REG_OT[st][i]=0;//очистить регистр ответа стойки
          ukaz_zap_dc++;
        }
        else
        {
          sboy_ts_dc++;
          if(sboy_ts_dc>11){otkaz_ts_dc=1;sboy_ts_dc=0;}
          ukaz_zap_dc=ukaz_zap_dc++;
					if(ukaz_zap_dc>SIZE_BUF_OUT_DC)ukaz_zap_dc=0;
          break;
        }
      }
      outportb(BAZ_ADR2+1,3);
      outportb(BAZ_ADR2,0);
      break;//запустить передачу
metka_d:
      if(est_com_ARM(st,ij)==0)break;
      else
      { sboy_ts[st]++;
        if(sboy_ts[st]>11){otkaz_ts[st]=1;sboy_ts[st]=0;}
      }
    }//конец работы с регистром ответов
  }//конец работы со стойкой
}
********************************/
//процедура анализа наличия команды ДЦ для полученного ответа
//вызывается из процедуры otvety_TUMS в режимах ДЦ и Сезон
/***********
int est_com_dc()
{
  int i,tums;
  for(tums=0;tums<skoko_stoek;tums++)
  {
    if(REG_COM_DC[0]!=0)//если неподтверждена команда
    {
      if((REG_OT[tums][1]==REG_COM_DC[1])&&//если ответ годится

      (REG_OT[tums][2]==REG_COM_DC[2])&&  
      (REG_OT[tums][3]==REG_COM_DC[3]))
      {
        for(i=0;i<11;i++)REG_COM_DC[i]=0;//удалить команду
        t_com[tums]=0l;//сбросить метку времени
        N_RAZ[tums]=0;//сбросить счетчик количества повторов
        return(0);
      }
    }
    else//если нет неподтвержденной команды
    {
      for(i=0;i<6;i++)REG_OT[tums][i]=0;//очистить регистр ответа
      return(-1);
    }
  }
  return(0);
}
//--------------------------------------------------------------------
/**************************************\
* Процедура анализа наличия команды    *
* оператора для полученного ответа     *
* вызывается в режиме управления с АРМ *
\**************************************/
int est_com_ARM(int tums)
{
  int i;
	nom_func("51");
	if(REG_COM_TUMS[tums][0]!=0)//если неподтверждена команда
  {
    if((REG_OT[tums][1]==REG_COM_TUMS[tums][1])&&
    (REG_OT[tums][2]==REG_COM_TUMS[tums][2])&&
    (REG_OT[tums][3]==REG_COM_TUMS[tums][3]))
    {
      for(i=0;i<11;i++)REG_COM_TUMS[tums][i]=0;//удалить команду
      for(i=0;i<6;i++)REG_OT[tums][i]=0;//сбросить ответ
      t_com[tums]=0l;//сбросить метку времени
      N_RAZ[tums]=0;//сбросить счетчик количества повторов
      return(0);
    }
    else
    {
      for(i=0;i<6;i++)REG_OT[tums][i]=0;
      return(-1);
    }
  }
  return(0);
}
//******************************************************
//процедура анализа ответов ДЦ полученных АРМом
//вызывается из процедуры main в каждом цикле
/********
analiz_ot_dc()
{
  int ij,i,st;
  if(REG_OT_DC[0]==0)return;
  else//если в регистре лежит ответ
  {
    if(REG_OT_DC[4]=='P')
    {
      if(REG_OT_DC[3]>=KOL_SOO1)st=0;
      else st=0;
      if(est_inf_dc(ij)==0)//если информация передавалась в ДЦ
      {
        if(st==0)REG_OT_DC[3]=REG_OT_DC[3]-KOL_SOO1;
        for(i=0;i<6;i++)//переслать побайтно весь ответ
        {
          if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//определить указатель записи
          if(BUF_OUT[st][ukaz_zap[st]]==0)//если ячейка вывода свободна
          {
            BUF_OUT[st][ukaz_zap[st]]=REG_OT_DC[i];
            REG_OT_DC[i]=0;
            ukaz_zap[st]++;
          }
          else
          {
            ukaz_zap[st]++;
            dc_not_plus=1;
            goto metka_out;
          }
          otkaz_tu_dc=0;
        }
        if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;//определить указатель записи
        dc_not_plus=0;
        otkaz_tu_dc=0;otkaz_ts_dc=0;sboy_ts_dc=0;
metka_out:
        if(st==0)
        {
          outportb(BAZ_ADR1+1,3);
          outportb(BAZ_ADR1,0);
        }
        if(st==1)
        {
          outportb(BAZ_ADR11+1,3);
          outportb(BAZ_ADR11,0);
        }
      }
      else//если данная информация не передавалась в ДЦ
      {
        dc_not_plus++;
      }
    }
    else dc_not_plus++;
  }
}
*************/
//********************************
//процедура анализа наличия сообщения отосланного в ДЦ для полученного ответа
//вызывается из процедуры analiz_ot_dc
/**********
int est_inf_dc(int n_reg_ot)
{
  int i,ij;
  for(ij=0;ij<10;ij++)//пройти по всем регистрам cообщений ДЦ
  {
    if(REG_INF_DC[ij][0]!=0)//если найдено неподтвержденное сообщение
    {
      if((REG_OT_DC[1]==REG_INF_DC[ij][1])&&  
      (REG_OT_DC[2]==REG_INF_DC[ij][2])&&  
      (REG_OT_DC[3]==REG_INF_DC[ij][3]))
      {
        //удалить сообщение
        for(i=0;i<6;i++)REG_OT_DC[i]=0;
//        t_inf_dc[ij]=0l;//сбросить метку времени
        return(0);
      }
      else
      {
        for(i=0;i<6;i++)REG_OT_DC[i]=0;
        return(-1);
      }
    }
  }
  for(i=0;i<6;i++)REG_OT_DC[i]=0;
  return(-1);
}********/
//---------------------------------------------------------
/**********************************************\
*  процедура обработки команд, принятых из ДЦ  *
*  проверка из допустимости и раздача по ТУМС  *
*  вызывается из программы main в каждом цикле *
\**********************************************/
#ifdef DISPETCHER
void COM_FROM_DC()
{
	int ij,st,i,n_bait;
	nom_func("27");
	n_bait=11;
	if(ukaz_com_dc==0)return;//если не было команд
	ukaz_com_dc=n_bait-1;//считаем, что во всех регистрах есть команда
	for(ij=0;ij<(n_bait-1);ij++)//пройти по всем регистрам команд ДЦ
	{
		if((REG_COM_DC[0]=='(')&&
		(REG_COM_DC[n_bait-1]==')'))
		{
			if(BAZ_ADR2!=0)//если диспетчерское управление
			{
				st=analiz_com_dc(ij);//провести анализ команды
				if(st!=-1)//если команда допустима
				{
					goto metka_a;
				}
				else goto metka_b;
			}
			else//если не диспетчерское управление
			{
#ifdef SEZ_UPR
				if(SEZON==1)
				{
					N_obekt=psk_obect(REG_COM_DC[ij]);
					if((N_obekt!=ROCH1)&&(N_obekt!=RON1)
#ifdef KOL_SOO2
					&&(N_obekt!=ROCH2)&&(N_obekt!=RON2)
#endif
					)
					{
						goto metka_b;
					}
				}
				else
#endif
				goto metka_a;
			}
		}
		else
		{
			ukaz_com_dc--;
			goto konec_cikl;
		}
metka_a:
		for(i=0;i<n_bait;i++)
		{
			if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
			if(BUF_OUT[st][ukaz_zap[st]]==0)//если ячейка буфера свободна
			{
				BUF_OUT[st][ukaz_zap[st]]=REG_COM_DC[i];
				ukaz_zap[st]++;
			}
			else//если писать некуда
			{
				ukaz_zap[st]=ukaz_zap[st]++;
				if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
				goto mt;
			}
		}
		if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
//    t_com_dc[ij]=biostime(0,0l);
 mt:
		ukaz_com_dc--;
		if(st==0)
		{
			outportb(BAZ_ADR1+1,3);
		}
		if(st==1)
		{
			outportb(BAZ_ADR11+1,3);
		}
		goto konec_cikl;
metka_b:
		fix_bad_com(ij);
		for(i=0;i<n_bait;i++)REG_COM_DC[i]=0;
		ukaz_com_dc--;
konec_cikl:
	}
	return;
}
//--------------------------------------------------------------------------
/****************************************\
* Процедура организации анализа команды, *
* полученной от ДС на допустимость по    *
* формату и условиям СЦБ.                *
\****************************************/
int analiz_com_dc()
{
	nom_func("7");
	return(0);
}
/******************************************\
* Процедура фиксации плохой команды из ДЦ  *  $$$$$$$$$
\******************************************/
int fix_bad_com()
{
//	nom_func("65");
	return(0);
}
#endif
/*проверка подтверждений полученных из стойки для команд из АРМ
test_podt_arm()
{
	int st,ij,i;
	for(st=0;st<skoko_stoek;st++)
	{ if(REG_COM_TUMS[st][0]==0)break;
		else
		{ if((biostime(0,0l)-t_com[st])>18l)//если прошла 1 сек
			{ if(N_RAZ[st]<3)//если команду посылали менее 3 раз
				{ for(i=0;i<11;i++)
					{ if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
						if(BUF_OUT[st][ukaz_zap[st]]==0)
						{ BUF_OUT[st][ukaz_zap[st]+i]=REG_COM_TUMS[st][i];
							ukaz_zap[st]++;
						}
						else
						{ ukaz_zap[st]++;
							if(ukaz_zap[st]>=SIZE_BUF_OUT)ukaz_zap[st]=0;
							break;
						}
					}
					otkaz_tu[st]=0;
					if(st==0){outportb(BAZ_ADR1+1,3);outportb(BAZ_ADR1,0);}
					if(st==1){outportb(BAZ_ADR11+1,3);outportb(BAZ_ADR11,0);}
					N_RAZ[st]++;
				}
			}
		}
	}
	return;
}
****************************************/
test_podt_dc()
{
/*
	nom_func("346");
*/
 return;
}
//------------------------------------------------------------------------
/****************************************************************\
* Процедура проверки наличия команды в регистре REG_COM_TUMS[st] *
* для полученной квитанции в REG_, при наличии такой команды она *
* сбрасывается и исключается автоматический повтор команд        *
\****************************************************************/
int byla_com(unsigned char REG_[12])
{
	int st,tos,i;
	nom_func("14");
	tos=REG_[1]&0x02; //определение стойки по заголовку квитанции
	if(tos==0x02)st=1;
	else st=0;
	//проверка регистра команд стойки
	if(REG_COM_TUMS[st][0]=='(')//если в регистре есть команда
	{
		if((REG_COM_TUMS[st][1]==REG_[1])&&
		(REG_COM_TUMS[st][2]==REG_[2])&&
		(REG_COM_TUMS[st][3]==REG_[3]))
		{
#ifdef NEW_FORMAT
			for(i=0;i<12;i++)REG_COM_TUMS[st][i]=0;
#else
			for(i=0;i<11;i++)REG_COM_TUMS[st][i]=0;
#endif
			N_RAZ[st]=0;//сбросить счетчик количества повторов
			return(0);
		}
		else //если в регистре команд другая команда - то не сбрасывать
		{
			return(-1);
		}
	}
	return(-1); //если в регистре нет команды - то тоже неудача
}
//---------------------------------------------------------------------
/***************************************************\
* Процедура записи сообщения для ДЦ,анализ потери.  *
* Сообщения для ДЦ записываются сразу после приема  *
* от ТУМС. Эту процедуру вызывает ANALIZ_TUMS()     *
\***************************************************/
int pishu_soob(int tums)
{
	int soob,i,novizna,n_bait;
	unsigned char REG_[13];
	n_bait=11;
	nom_func("224");
#ifdef DISPETCHER
	if(STATUS!=1)return(0);
	//перезапись принятого из ТУМСа
	for(i=0;i<n_bait;i++)REG_[i]=REG_INFO[tums][i];

	if(REG_[3]=='y')return(0); //сообщения диагностики в ДЦ не отправлять

	REG_[n_bait+1]=0;novizna=0; //изначально считаем, что новизны нет

	if(tums==1)soob=REG_[3]-48+KOL_SOO1; //вычисляем сквозной номер сообщения
	else soob=REG_[3]-48;

	REG_[3]=soob+48; //присваиваем код номера сообщения

	REG_[n_bait-2]=check_sum(REG_,0); //пересчитываем контрольную сумму
	REG_[n_bait-1]=')';

	for(i=0;i<(n_bait-1);i++)//проходим по всем байтам сообщений
	{
		if(REG_INF_DC[soob][i]!=REG_[i])//если новизна в сообщении
		{
			if(((REG_INF_DC[soob][n_bait]&0x3)==0)&&(novizna==0))
			{
				novizna=2;
				fix_poter(2,soob);//если не передано
			}
			else
				if(((REG_INF_DC[soob][n_bait]&0x30)==0)&&( novizna==0))
				{
					novizna=1;
					fix_poter(1,soob);//если не квитировано
				}
				REG_INF_DC[soob][n_bait]=0xC0;//признак "новизна"
		}
		REG_INF_DC[soob][i]=REG_[i];//перезапись сообщения в массив ДЦ
	}
#endif
	return(0);
}
//------------------------------------------------- --------------
/******************************************\
* Запись полученного от ТУМС подтверждения *
* команды для ДЦ,анализ потери.            *
\******************************************/
int pishu_podt(int tums)
{
	int soob,i;
	unsigned char REG_[6];
	nom_func("441");
#ifdef DISPETCHER
	if(STATUS!=1)return(-1);
	for(i=0;i<6;i++)REG_[i]=REG_INFO[tums][i];
	if(tums==1)soob=REG_[3]-48+KOL_SOO1;
	else soob=REG_[3]-48;
	REG_[3]=soob+48;
	REG_[4]='P';
	for(i=0;i<6;i++)
	{
		if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;
		BUF_OUT_DC[ukaz_zap_dc++]=REG_[i];
	}
	outportb(BAZ_ADR2+1,3);
#endif
	return(0);
}

//-------------------------------------------------------------
/**********************************************\
* Процедура фиксации потерь данных на основе   *
* отсутствия квитанций или отсутствия передачи *
* pot - признак потери 1-не передано;          *  $$$$$$$$$
*                      2-не квитировано        *
* sob - номер потерянного сообщения            *
\**********************************************/
void fix_poter(int pot,int sob)
{
	nom_func("66");
	if(pot!=0)pot=0;
	if(sob!=0)sob=0;
	return;
}

//----------------------------------------------------------------------
/**********************************************************\
* Процедура заполнения буфера вывода в ДЦ очередного       *
* сообщения, полученного из ТУМС. Вызывается из consentr() *
* после анализа сообщений, полученных из ТУМСов            *
\**********************************************************/
void OUT_DC()
{
#ifdef DISPETCHER
	int i,j,n_bait;
	nom_func("210");
	n_bait=11;
	if(STATUS!=1)return;//для неосновного АРМа не выполняется
	//если объект, на котором была прервана предыдущая передача последний, то
	//начать сначала
	if(OB_PRER>=KOL_SOO)OB_PRER=0;
	if(OB_NEKVIT>=KOL_SOO)OB_NEKVIT=0;
	if(OB_NEW>=KOL_SOO)OB_NEW=0;

	if(ukaz_zap_dc>ukaz_vyd_dc)return; //ЕСЛИ ВЫДАЧА ОТСТАЛА ОТ ЗАПИСИ
	else
	{
		ukaz_zap_dc=0;
		ukaz_vyd_dc=0;
	}
	//пройти по всем донесениям с поиском новизны
	for(i=OB_NEW;i<KOL_SOO;i++)
	{
		//если по сообщению непереданная новизна
		if(REG_INF_DC[i][n_bait]==0xC0)
		{ //если не пустое сообщение нет квитанции
			if(REG_INF_DC[i][0]!=0)
			{
				if(dc_not_plus==0)REG_INF_DC[i][n_bait-1]='+';
				if(dc_not_plus==1)REG_INF_DC[i][n_bait-1]=')';
				//если был отказ по квитанциям ДЦ
				if(dc_not_plus==2)REG_INF_DC[i][n_bait-1]='-';

				for(j=0;j<n_bait;j++)
				{ if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;
					BUF_OUT_DC[ukaz_zap_dc++]=REG_INF_DC[i][j];
				}
				if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;

				//заменить признак новизны на признак "выдано"
				REG_INF_DC[i][n_bait]=0x3;

				//переключиться на следующий объект
				OB_NEW=i+1;
				if(OB_NEW>=KOL_SOO)OB_NEW=0;
				outportb(BAZ_ADR2+1,3);//открыть передачу в ДЦ
				return;
			}
			else continue;
		}
	}
	if(OB_NEW>=KOL_SOO)OB_NEW=0;

	//если нет новизны
	//пройти по всем донесениям с поиском неквитирования
	for(i=OB_NEKVIT;i<KOL_SOO;i++)
	{
		//если по сообщению нет квитанции
		if(REG_INF_DC[i][n_bait]==3)
		{
			//если не пустое сообщение, то выдать повторно
			if(REG_INF_DC[i][0]!=0)
			{
				if(dc_not_plus==0)REG_INF_DC[i][n_bait-1]='+';
				if(dc_not_plus==1)REG_INF_DC[i][n_bait-1]=')';
				//если был отказ по квитанциям ДЦ
				if(dc_not_plus==2)REG_INF_DC[i][n_bait-1]='-';
				for(j=0;j<n_bait;j++)
				{ if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;
					BUF_OUT_DC[ukaz_zap_dc++]=REG_INF_DC[i][j];
				}
				if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;

				//сбросить признаки новизны и выдачи
				REG_INF_DC[i][n_bait]=0;
				OB_NEKVIT=i+1;
				if(OB_NEKVIT>=KOL_SOO)OB_NEKVIT=0;
				outportb(BAZ_ADR2+1,3);//открыть передачу в ДЦ
				return;
			}
			else continue;
		}
	}
	if(OB_NEKVIT>=KOL_SOO)OB_NEKVIT=0;
	//пройти по всем донесениям
	for(i=OB_PRER;i<KOL_SOO;i++)
	{ //если не пустое сообщение
		if(REG_INF_DC[i][0]!=0)
		{
			if(dc_not_plus==0)REG_INF_DC[i][n_bait-1]='+';
			if(dc_not_plus==1)REG_INF_DC[i][n_bait-1]=')';
			//если был отказ по квитанциям ДЦ
			if(dc_not_plus==2)REG_INF_DC[i][n_bait-1]='-';

			for(j=0;j<n_bait;j++)
			{ if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;
				BUF_OUT_DC[ukaz_zap_dc++]=REG_INF_DC[i][j];
			}
			if(ukaz_zap_dc>=SIZE_BUF_OUT_DC)ukaz_zap_dc=0;

			//установить признак "выдано"
			REG_INF_DC[i][n_bait]=0;
			OB_PRER=i+1;
			if(OB_PRER>=KOL_SOO)OB_PRER=0;
			outportb(BAZ_ADR2+1,3);//открыть передачу в ДЦ
			return;
		}
		else continue;
	}
	if(OB_PRER>=KOL_SOO)OB_PRER=0;
	return;
#else
	return;
#endif
}
//-----------------------------------------------------------------
/**********************************************************\
* Процедура анализа буферов приема стоек и определение     *
* наличия в буферах законченной последовательности символов*
\**********************************************************/
void ANALIZ_TUMS()
{
	int st,i,ij,podt_com,k,n_bait;
	nom_func("10");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;
	for(st=0;st<skoko_stoek;st++)//пройти по стойкам
	{ if(ANALIZ[st]>0)//если из стойки пришло сообщение или ответ
		{ ij=0; k=konec[st];ANALIZ[st]=0;
			while((BUF_IN[st][k]!='(')&&(BUF_IN[st][k]!=0))
			{ k--; ij++;if(k<0)k=SIZE_BUF_PRIEM-1;}
			if(BUF_IN[st][k]=='(')
			{ for(i=0;i<=n_bait;i++)
				{ REG_INFO[st][i]=BUF_IN[st][k];
					BUF_IN[st][k]=0;
					k++;if(k>=SIZE_BUF_PRIEM)k=0;
					if((REG_INFO[st][i]=='+')||(REG_INFO[st][i]==')'))break;
				}
				if(i==5)//если принят ответ на команду
				{ podt_com=byla_com(REG_INFO[st]);//проверить,была ли команда ?
					if(podt_com==0)sboy_ts[st]=0;
					pishu_podt(st);
				}
				if(i==(n_bait-1))//если принято сообщение
				{ test_sum=check_sum(REG_INFO[st],1);//проверить контрольную сумму
					if(test_sum!=REG_INFO[st][n_bait-2])//к.сумма не совпадает
					{for(k=0;k<n_bait;k++)REG_INFO[st][k]=0;}//сбросить сообщениe
					else
					{
						sboy_ts[st]=0;
						otkaz_ts[st]=0;
						pusto=pusto&(~(1<<st));
						if(((pusto&0x10ff)==0x1000)&&(ZAGRUZKA==0))
						pusto=0xff00;
//           else pusto=0;
						if((STATUS==1)||(STATUS==0))form_otv(st);//отправить ответ
						dc_not_plus=pishu_soob(st);//запись сообщения для ДЦ,анализ потери
					}
				}
			}
		}
	}
	return;
}
//-----------------------------------------------------------------
/**************************************************\
* Проверка и запись команды от ДЦ в буфер обмена с *
* ТУМС.                                            *
\**************************************************/
int pishu_com_dc(unsigned char REG_[12])
{
	int handle,nom_kom,i,ost,vverh,vniz,shag,shag0,st,kol_kom,strt,fin,iv,
	obk,obkt,n_bait,tip_ob,ob_sp,ob_parn,n_shag;
	long len_f;
	unsigned char buf_[12];
	nom_func("223");
	REG_[9]='|';
	REG_[10]=check_sum(REG_,1);
	REG_[11]=')';
	if(STATUS!=1)return(-1);
	handle = open("dat\\COM_DC.BIN",O_RDONLY|O_BINARY);
	if(handle==-1)
	{
		w(256,999,"");
		return(-1);
	}
	len_f=filelength(handle);//длина файла
#ifdef NEW_FORMAT
	nom_kom=len_f/14;//число команд в файле
	n_bait=12;
#else
	nom_kom=len_f/13;//число команд в файле
	n_bait=11;
#endif
	kol_kom=nom_kom;
	shag=nom_kom/2;ost=nom_kom%2;//найти половину
	if(ost!=0)shag++;nom_kom=shag;
snova:
	vverh=0;vniz=0;
	lseek(handle,(nom_kom-1)*(n_bait+2),0);//выйти на команду
	read(handle,buf_,n_bait);
	for(i=0;i<n_bait;i++)
	{
		if(REG_[i]<buf_[i]){vverh=1;vniz=0;break;}
		if(REG_[i]>buf_[i]){vverh=0;vniz=1;break;}
	}
	if((vverh==0)&&(vniz==0))//если найдена команда
	{
		st=REG_[1]&2;
		if(st==2)st=1;
		else st=0;
		obkt=9999;
		if(REG_[2]=='E') //если команда на сигнал
		{
			tip_ob=2;
			if(st==0){strt=STR1;fin=SIG1+STR1;}
			else {strt=STR2;fin=STR2+SIG2;}
			obk=9999;

			for(iv=strt;iv<fin;iv++)
			if(REG_[3]==podgruppa[iv])
			{obk=iv;break;} //obk=номер принятой строки сообщений в стойке

			if(obk==9999)return(-1);

			if(st==0)strt=STR1; //начало поиска сигнала 1-й стойки = конец стрелок
			else strt=STR2-SIG1;//начало поиска сигнала 2-ой стойки

			for(iv=0;iv<5;iv++)
			{
				if(REG_[iv+4]==124)continue;
				obkt=spsig[obk-strt][iv];
				if(obkt==1111)return(-1);
				else break;
			}
		}
		if(REG_[2]=='Q') //если команда на доп.объект
		{
			tip_ob=9;
			if(st==0)
			{
				strt=STR1+SIG1;
				fin=SIG1+STR1+DOP1;
			}
			else
			{
				strt=STR2+SIG2;
				fin=STR2+SIG2+DOP2;
			}
			obk=9999;
			for(iv=strt;iv<fin;iv++)
			if(REG_[3]==podgruppa[iv])
			{
				obk=iv;
				break;
			}
			if(obk==9999)return(-1);

			if(st==0)strt=obk-STR1-SIG1;
			else strt=obk-STR2-SIG2+DOP1;

			for(iv=0;iv<5;iv++)
			{
				if(REG_[iv+4]==124)continue;
				obkt=spdop[strt][iv];
				if(obkt==1111)return(-1);
				else break;
			}
		}
		if(REG_[2]=='C') //если команда на стрелку
		{
			tip_ob=1;
			if(st==0){strt=0;fin=STR1;}
			else {strt=0;fin=STR2;}
			obk=9999;
			for(iv=strt;iv<fin;iv++)if(REG_[3]==podgruppa[iv]){obk=iv;break;}
			if(obk==9999)return(-1);
			if(st==0)strt=0;
			else strt=STR1;
			for(iv=0;iv<5;iv++)
			{
				if(REG_[iv+4]==124)continue;
				obkt=spstr[obk+strt][iv];
				if(obkt==1111)return(-1);
				else break;
			}
		}

#ifdef DISPETCHER
		if(obkt!=9999)
		switch(tip_ob)
		{
			//если выдаются команды на стрелку
			case 1: ob_sp=findspu1(obkt,fr1[obkt][7]);
							if(ob_sp>0)
							{
								if((fr3[ob_sp][0]!=0)||(fr3[ob_sp][1]!=0)||(fr3[ob_sp][2]!=0))
								for(i=0;i<n_bait;i++)REG_[i]=0;
							}
							if(fr1[obkt][12]!=9999)//если стрелка спаренная
							{
								ob_parn=poisk_ras(fr1[obkt][2],obkt,1,0);
								ob_sp=findspu1(ob_parn,fr1[ob_parn][7]);
								if(ob_sp>0)
								{
									if((fr3[ob_sp][0]!=0)||(fr3[ob_sp][1]!=0)||(fr3[ob_sp][2]!=0))
									for(i=0;i<n_bait;i++)REG_[i]=0;
								}
							}
							break;
			//если выдаются команды на РОН/РОЧ
			case 2:	if((fr1[obkt][1]==16)&&(fr1[obkt][2]==5))//если РОН для нечетного отправления
							{
								if((REG_[iv+4]=='M')&&(na==1))//если РОН отключается и неч.автодействие
								{
									w(265,999,"(нечетное)");
									zvuk_vkl(0,0);
									snyato_nechet();
									TIMER_SOB(obkt,11,obkt);
									for(i=0;i<n_bait;i++){REG_RON[i]=REG_[i];REG_[i]=0;}
									if(handle>0)close(handle);
									handle=0;
									return(0);
								}
							}
							if((fr1[obkt][1]==16)&&(fr1[obkt][2]==6))//если РОЧ для четного отправления
							{
								if((REG_[iv+4]=='M')&&(cha==1))//если РОН отключается и чет.автодействие
								{
									w(265,999,"(четное)");
									zvuk_vkl(0,0);
									snyato_chet();
									TIMER_SOB(obkt,11,obkt);
									for(i=0;i<n_bait;i++){REG_RON[i]=REG_[i];REG_[i]=0;}
									if(handle>0)close(handle);
									handle=0;
									return(0);
								}
							}
							break;
			//если выдаются команды на РОН/РОЧ
			case 9: if(fr1[obkt][1]==155)//если РОН для нечетного отправления
							{
								if((REG_[iv+4]=='M')&&(na==1))//если РОН отключается и неч.автодействие
								{
									w(265,999,"(нечетное)");
									zvuk_vkl(0,0);
									snyato_nechet();
									TIMER_SOB(obkt,11,obkt);
									for(i=0;i<n_bait;i++){REG_RON[i]=REG_[i];REG_[i]=0;}
									if(handle>0)close(handle);
									handle=0;
									return(0);
								}
							}
							if(fr1[obkt][1]==156)//если РОЧ для четного отправления
							{
								if((REG_[iv+4]=='M')&&(cha==1))//если РОН отключается и чет.автодействие
								{
									w(265,999,"(четное)");
									zvuk_vkl(0,0);
									snyato_chet();
									TIMER_SOB(obkt,11,obkt);
									for(i=0;i<n_bait;i++){REG_RON[i]=REG_[i];REG_[i]=0;}
									if(handle>0)close(handle);
									handle=0;
									return(0);
								}
							}
							break;

			default:	break;
		}
		if(REG_[0]!=0)
		{
			for(i=0;i<n_bait;i++)
			{
				buf_ko_dc[st][i]=REG_[i];
				REG_[i]=0;
			}
			flagoutmsg=1;
		}
		if(handle>0)close(handle);
		handle=0;
		return(0);
#endif
	}
	//если не найдена команда
	if(((nom_kom==kol_kom)&&(vverh==0))||(vverh==vniz))
	{
		if(handle>0)close(handle);
		handle=0;
		return(-1);
	}
	shag0=shag/2;ost=shag%2;

	if(ost!=0)shag=shag0+1;
	else shag=shag0;
	if(vverh==1)nom_kom=nom_kom-shag;
	if(vniz==1)nom_kom=nom_kom+shag;
	if(nom_kom<0)nom_kom=1;
	if(nom_kom>kol_kom)nom_kom=kol_kom;
	goto snova;
}
//--------------------
form_otv_dc(unsigned char REG_[11])
{
	nom_func("85");
	return;
}
//------------------------------------------------------------------------
/********************************************************\
*	Процедура проверки наличия сообщения, на которое была  *
* получена квитанция от ДЦ (соответстие квитанции)       *
\********************************************************/
int byla_inf_dc(unsigned char REG_[12])
{
	int soob,n_bait;
	nom_func("15");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return(-1);
#ifdef DISPETCHER
	soob=REG_[3]-48;
	if((soob<0)||(soob>=KOL_SOO))return(-1);
	REG_INF_DC[soob][n_bait]=REG_INF_DC[soob][n_bait]|0x30;
#endif
	return(0);
}
/*****************************************************\
*   Процедура проверки наличия команды для стойки в   *
*   регистре команд и контроля прохождения 1 сек от   *
*   момента предыдущей выдачи данной команды.         *
*   Через 1 сек. не сброшенная команда выдается вновь *
\*****************************************************/
void ANALIZ_KOM_TUMS()
{
	int st,i,n_bait;
	unsigned long int t_tek;
	nom_func("8");
#ifdef NEW_FORMAT
	n_bait=12;
#else
	n_bait=11;
#endif
	if(DISK!=0)return;  //при просмотре архива выход без действий
	t_tek=biostime(0,0l); //взять текущее время

	for(st=0;st<skoko_stoek;st++)  //пройти во всем стойкам
	{
		if(REG_COM_TUMS[st][0]=='(') //если в регистре команд стойки есть начало
		{
			if(t_tek<9)t_com[st]=0l;  //если начало суток, то сбросить время команд

			//если прошло более 1 сек, нет ДУ и не нажата кнопка ОК
			if((t_tek-t_com[st]>18l)&&(DU==0)&&(otv_kom==0))
			{
				//переписать команду из регистра команд стойки в буфер команд
				for(i=0;i<n_bait;i++)buf_ko[i]=REG_COM_TUMS[st][i];

				kom_v_bufer(st); //выдать команду в буфер вывода
			}
		}
	}
	return;
}
//---------------------------------------------------------------------
/*******************************************************\
* Процедура приема символа из порта ББКП и размещения   *
* его в буфер приема ДЦ.                                *
* Если принят символ скобки закрытия, то получает прира-*
* щение переменная ANALIZ_DC                            *
\*******************************************************/
void in_port_BBKP(unsigned int adr)
{
  unsigned char SYM_DC;
#ifdef DISPETCHER
  SYM_DC=inportb(adr)&127;
  if(SYM_DC==0)return;
  t_pust_dc=0;
	if(SYM_DC==')')
  ANALIZ_DC++;
  BUF_IN_DC[ukaz_priem_dc++]=SYM_DC;
	konec_dc=ukaz_priem_dc;
	if(ukaz_priem_dc>=SIZE_BUF_PRIEM_DC)ukaz_priem_dc=0;
#endif
	return;
}
//-------------------------------------------------------------------------
/******************************************************************\
* Процедура анализа информации, поступающей из ББКП (команда или   *
* квитанция).Для команды выполняется проверка контрольной суммы и  *
* сравнение со списком допустимых команд. При удачной команде, она *
* транслируется в ТУМС. Для квитанции выполняется сравнение с пос- *
* ледним сообщением, переданным в ДЦ. При удаче сброс повтора, при *
* неудаче фиксируется сбой канала ДЦ                               *
\******************************************************************/
void ANALIZ_BBKP()
{
	int i,ij,podt_inf_dc,k,n_bait;
	unsigned char REG_COM_DC_OUT[12];
	nom_func("6");
	if(DISK!=0)return;
	n_bait=11;
#ifdef DISPETCHER
povtor:
	if(ANALIZ_DC>0)//если из ДЦ пришла команда или ответ
	{ ij=0;k=konec_dc;
		ANALIZ_DC=0;
		while(BUF_IN_DC[k]!='(')
		{k--;
		 ij++;
		 if(ij>SIZE_BUF_PRIEM_DC)break;
		 if(k<0)k=SIZE_BUF_PRIEM_DC-1;
		}
		if(BUF_IN_DC[k]=='(')
		{ for(i=0;i<=n_bait;i++)
			{ REG_COM_DC[i]=BUF_IN_DC[k];
				BUF_IN_DC[k]=0;k++;
				if(k>=SIZE_BUF_PRIEM_DC)k=0;
				if(REG_COM_DC[i]==')')break;
			}
			if(i==5)//если принят ответ на сообщение
			{
				podt_inf_dc=byla_inf_dc(REG_COM_DC);//проверить,была ли информация ?
				if(podt_inf_dc==-1)otkaz_ts_dc=1; //если команды не было-отказ ТС
			}
			if(i==10)//если принята команда
			{
				test_sum=check_sum(REG_COM_DC,0);//проверить контрольную сумму
				if(test_sum!=REG_COM_DC[9])//к.сумма не совпадает
				{
					for(k=0;k<11;k++)
					{
						REG_COM_DC[k]=0;//сбросить команду
						return;

					}
				}
				else
				{
					for(k=0;k<11;k++)
					{
						REG_COM_DC_OUT[k]=REG_COM_DC[k];//сбросить команду

					}
				}
				form_otv_dc(REG_COM_DC);//отправить ответ
				dc_not_plus=pishu_com_dc(REG_COM_DC_OUT);//записать команду ДЦ
			}
		}
	}
#endif
	return;
}
//==========================================================
void interrupt far PRNTSCR()
{
	outportb(0x20,0x20);
}
//==========================================================
void interrupt far CNTRLBREAK()
{
  outportb(0x20,0x20);
}
//==========================================================
void interrupt far KEYBRD()
{
  unsigned char value;
  value = peek(0x0040, 0x0017);
  if(value==0)goto LOO1; //если не нажата CTRL
  poke(0x0040,0x0017,0);
LOO1:
  value = peek(0x0040, 0x0018);
	if(value==0)goto LOO; //если не нажата CTRL
  poke(0x0040,0x0018,0);
LOO:
	old3();
  return;
}
//==================================
void interrupt far TIMER_TIC()
{

	if(ZAGRUZKA==0)STOROG++;
	else STOROG=0;
	if(STOROG>30)
	{
		outportb(BAZ_ADR1+1,3);//разрешить прерывания по 1-му каналу
		outportb(BAZ_ADR1,0);//отправить 0-байт в канал
	}
	old_timer();
	return;
}
//====================================
void reset_arm()
{
	int value;
	jmp_buf jumper;
	value=setjmp(jumper);
	poke(0x40,0x72,0x1234);
	jumper[0].j_cs=0xffff;
	jumper[0].j_ip=0;
	longjmp(jumper,1);
	return;
}
