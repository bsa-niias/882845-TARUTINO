#include"bla.h"
//-----------------------------------------------------------------------
/*************************************************\
* Процедура обработки данных, для канала kanal,то *
*
\*************************************************/
void obrabotka_inf(int kanal)
{
	int kum=0,SMEZ=0,ST=0,jj,new_mythx,clrpix;
	unsigned char PODGR;
// nom_func("196");
	vysvv=9999;
//	if((pusto&3)!=0)return;
	kum=bu[kanal][1]&0x02;
	if(kum!=2) kum=1;
	if(otkaz_ts[kum-1]==1)return;
	if((STATUS==1)||(DISK!=0))
	{
		PODGR=bu[kanal][9];
		new_mythx=ANALIZ_MYTHX(PODGR,kanal);
	}
	if((bu[kanal][2]=='Q')&&(bu[kanal][3]=='D'))
	jj=0;
	switch(bu[kanal][2])
	{
		//если объект - стрелка
		case 'C': //if((pusto&3)!=0)return;
							if(kum==2){SMEZ=0;ST=STR1;}
							else {SMEZ=0;ST=0;}
							vysvv=strelok(bu[kanal],SMEZ,kum,ST);
							break;

		//если объект - сигнал
		case 'E': //if((pusto&3)!=0)return;
#ifdef KOL_SOO2
							if(kum==2){SMEZ=STR2;ST=SIG1;}
#endif
							if(kum==1){SMEZ=STR1;ST=0;}
							vysvv=signale(bu[kanal],SMEZ,kum,ST);
							break;

		//если дополнительный объект
		case 'Q': //if((pusto&3)!=0)return;
#ifdef KOL_SOO2
							if(kum==2){SMEZ=STR2+SIG2;ST=DOP1;}
#endif
							if(kum==1){SMEZ=STR1+SIG1;ST=0;}
							vysvv=signale(bu[kanal],SMEZ,kum,ST);
							break;

		//если СП или УП
		case 'F': //if((pusto&3)!=0)return;
#ifdef KOL_SOO2
							if(kum==2){SMEZ=STR2+SIG2+DOP2;ST=UCH1;}
#endif
							if(kum==1){SMEZ=STR1+SIG1+DOP1;ST=0;}
							vysvv=uchastic(bu[kanal],SMEZ,kum,ST);
							break;

		//если пути
		case 'I': //if((pusto&3)!=0)return;
#ifdef KOL_SOO2
							if(kum==2){SMEZ=STR2+SIG2+DOP2+UCH2;ST=PUT1;}
#endif
							if(kum==1){SMEZ=STR1+SIG1+DOP1+UCH1;ST=0;}
							vysvv=putec(bu[kanal],SMEZ,kum,ST);
							break;

		//если контроллер
		case 'L':
#ifdef KOL_SOO2
							if(kum==2){SMEZ=STR2+SIG2+DOP2+UCH2+PUT2;ST=UPR1;}
#endif
							if(kum==1){SMEZ=STR1+SIG1+DOP1+UCH1+PUT1;ST=0;}
							vysvv=kontroler(bu[kanal],SMEZ,kum,ST);
							break;

		case 'R': vysvv=diagnoze(bu[kanal],kanal);
							break;

		//если объект собственных нужд контроллера

		case 'J': for(jj=0;jj<9;jj++)
							{
								if(bu[kanal][3]==D_kont[jj])//если объект собственных нужд
								{
									do_it(kanal,bu[kanal]);
									vysvv=1;
									break;
								}
							}
							break;
		default:  vysvv=9999;
							break;
	}
	if((DISK==0)&&(vysvv!=9999))
	{
		fixation(kanal,vysvv);
//		clrpix=getpixel(600,450);
//		if(clrpix==7)setcolor(12);
//    else setcolor(7);
//		outtextxy(600,450,"█");
	}
	if((new_mythx!=0)&&(DISK==0))fixation(kanal,new_mythx);
	return;
}
//--------------------------------------------
/*******************************************\
* Процедура распаковки сообщений для путей. *
\*******************************************/

int putec(unsigned char bufpu[11],int Smo, int kim,int sto)
{
  int kh=0,M,l;
	unsigned int tst_bit;
  unsigned char kop=0;
	int iv,jv,tester,reza=0,FIN=0,FN=0;
#ifdef NALAD
	nom_func("257");
#endif
	tester=9999;
#ifdef KOL_SOO2
	if(kim==2)//если стойка 2
	{
		FN=PUT1;
		FIN=FN+PUT2;
	}
#endif
	if(kim==1) //если стойка 1
	{
		FIN=PUT1;
		FN=0;
	}
	for(iv=FN;iv<FIN;iv++) //пройти по подгруппам
	if(bufpu[3]==podgruppa[iv+Smo-sto])//если вышли на подгруппу
	{
		tester=iv;
		break;
	}
	if(tester==9999) return(tester);//если подгруппа не найдена - вернуть 999
	for(jv=0;jv<5;jv++)//пройти по объектам подгруппы
	{
		vysv=0;
		kh=spputi[tester][jv]; //взять номер объекта
		if(kh==1111)continue; //если объект пустой - пропустить
		l=1;
		n=jv+4;
		for(M=0;M<=5;M++)//пройти по битам
		{
			kop=bufpu[n]&l; //получить значение бита
			if(kop==0)//если бит следует сбросить
			{
				if(fr3[kh][M]==0)goto nono;
				else// если ранее бит был установлен
        {
          if(fr1[kh][7]<9999)goto met0;//если участок приближения
          else
#ifdef PUTI_IN_2
					if(fr1[kh][13]!=9999)
          goto met0;
          else
          {
#if CHET==2
						if((FN>0)&&//если вторая стойка и
						(M==2)&&(fr1[kh][8]==1010))//бит ЧИ и приемо-отправочный
						 goto nono;//то уйти
						 if((FN==0)&&(M==1)&&(fr1[kh][8]==1010)&&(skoko_stoek==2))goto nono;// если НИ,а стойка 1
						 if(FN==0)tst_bit=1<<(M*2);//если первая стойка, то тест-бит нечетный
						 else tst_bit=2<<(M*2);//если вторая стойка, то тест-бит четный
						 tst_bit=~tst_bit;//инверсия тест-бита
# else
						 if((FN>0)&&//если вторая стойка и
						 (M==1)&&(fr1[kh][8]==1010))//бит НИ и приемо-отправочный
						 goto nono;//то уйти
						 if((FN==0)&&(M==2)&&(fr1[kh][8]==1010)&&(skoko_stoek==2))goto nono;// если НИ,а стойка 1
						 if(FN==0)tst_bit=2<<(M*2);//если первая стойка, то тест-бит нечетный
						 else tst_bit=1<<(M*2);//если вторая стойка, то тест-бит четный
						 tst_bit=~tst_bit;//инверсия тест-бита
#endif
          }
          fr3[kh][6]=fr3[kh][6]&tst_bit;//сбрасывание бита в слове увязки
          tst_bit=3<<(M*2);//формирование тест-бита на два разряда
          if((fr3[kh][6]&tst_bit)==0)
#endif
met0:     fr3[kh][M]=0;//если оба разряда увязки пустые сбросит бит в 0
					if((ZAGRUZKA==0)||(DISK!=0))
					{
						if(fr1[kh][0]==4)displaypathuch(kh);
						else displaypath(kh,M);//если не загрузка -нарисовать путь
					}
					vysv=1;//установить признаки получения изменения
					reza=1;
				}
			}
			else//если бит следует установить
			{
				if(fr3[kh][M]!=1) // если ранее был сброшен
				{
					if(fr1[kh][7]<9999)goto met1;
					else
#ifdef PUTI_IN_2
          if(fr1[kh][13]!=9999)goto met1;
					else
#if CHET==2
					 if((FN>0)&&(M==2)&&(fr1[kh][8]==1010)) goto nono;
					 if((FN==0)&&(M==1)&&(fr1[kh][8]==1010)&&(skoko_stoek==2)) goto nono;
					 if(FN==0)tst_bit=1<<(M*2);
					 else tst_bit=2<<(M*2);
					 fr3[kh][6]=fr3[kh][6]|tst_bit;
					 tst_bit=3<<(M*2);
#else
					 if((FN>0)&&(M==1)&&(fr1[kh][8]==1010)) goto nono;
					 if((FN==0)&&(M==2)&&(fr1[kh][8]==1010)&&(skoko_stoek==2)) goto nono;
					 if(FN==0)tst_bit=2<<(M*2);
					 else tst_bit=1<<(M*2);
					 fr3[kh][6]=fr3[kh][6]|tst_bit;
					 tst_bit=3<<(M*2);
#endif
					if((fr3[kh][6]&tst_bit)!=0)fr3[kh][M]=1;
#endif
met1:     fr3[kh][M]=1;
					fr3[kh][10]=0;
          if((ZAGRUZKA==0)||(DISK!=0))
					{
						if(fr1[kh][0]==4)displaypathuch(kh);
						else displaypath(kh,M);
					}
					vysv=1;
					reza=1;
				}
			}
nono: l=l*2;
		}
		if(vysv==0&&ZAGRUZKA==0)time20sec(kh);//если нет новых данных по объекту
Bbreak:;
  }
  return(reza);//вернуть признак появления новых данных по объекту
}
/***************************************************************/
int uchastic(unsigned char bufus[12],int Smo,int kim,int sto)
{
  int kh=0,Z=5,M,iv,jv,tester=9999,reza=0,FIN=0,FN=0,l;
  unsigned char kop=0;
#ifdef NALAD
  nom_func("379");
#endif
#ifdef KOL_SOO2
  if(kim==2){FIN=UCH1+UCH2;FN=UCH1;}
#endif
  if(kim==1){FIN=UCH1;FN=0;}
  for(iv=FN;iv<FIN;iv++)
	if(bufus[3]==podgruppa[iv+Smo-sto]){tester=iv;break;}
  if(tester==9999) return(tester);
  for(jv=0;jv<Z;jv++)
  { vysv=0;
    kh=spspu[tester][jv];
    if(kh==1111)continue;
    l=1;n=jv+4;
		for(M=0;M<=5;M++)
    { kop=bufus[n]&l;
      if(kop==0)//если бит надо сбросить
      {
        //если это данные по самому СП и СП завязано в кресте
        if((fr1[kh][1]<3)&&(fr1[kh][2]<3)&&((fr1[kh][4]&1)==1))
        {
          if(fr3[kh][M]!=0){vysv=1;reza=1;}
          krest(kh,M,0);
          l=l*2;
          continue;
        }
				if(fr3[kh][M]!=0)// а бит был установлен
				{
					fr3[kh][M]=0;
					pooo[kh]=0;
					if((com_bit[kh]&l)!=0)
					{
						pooo_com[kh]=0;
						com_bit[kh]=0;
					}
					if((ZAGRUZKA==0)||(DISK!=0))
					{
#ifdef POLUAVTOMAT
						if(fr1[kh][1]==52)analiz_priem(kh,M);
						else
#endif
						if(fr1[kh][0]==3)sekci(kh,M);
						if(fr1[kh][0]==4)displaypathuch(kh);
						if(fr1[kh][0]==2)
						displaysignal(kh,M);
					}
					vysv=1;
					reza=1;
				}
			}
			else//если бит надо установить
			{
				if(fr3[kh][10]!=0)
				fr3[kh][10]=0;
				if((fr1[kh][1]<3)&&(fr1[kh][2]<3)&&((fr1[kh][4]&1)==1))
				{
					if(fr3[kh][M]!=1){vysv=1;reza=1;}
					krest(kh,M,1);
					l=l*2;
					continue;
				}
				if(fr3[kh][M]!=1)//а бит был сброшен
				{ fr3[kh][M]=1;
					pooo[kh]=0;
					if((com_bit[kh]&l)!=0)
					{
						pooo_com[kh]=0; //таймер ожидания выполения команды
						com_bit[kh]=0;
					}
					if((ZAGRUZKA==0)||(DISK!=0))
          {
#ifdef POLUAVTOMAT
            if(fr1[kh][1]==52)analiz_priem(kh,M);
            else
#endif
              if(fr1[kh][0]==3)sekci(kh,M);
              if(fr1[kh][0]==4)displaypathuch(kh);
              if(fr1[kh][0]==2)displaysignal(kh,M);
					}
          vysv=1;
					reza=1;
        }
      }
      l=l*2;
		}
		if((vysv==0)&&(ZAGRUZKA==0))time20sec(kh);
  }
	return(reza);
}
/****************************************************************/
int strelok(unsigned char bufstr[12],int Smo,int kim,int sto)
{
	int vse=0,kh=0,para,M,iv,jv,tester=9999,reza=9999,FN=0,FIN=0,l,sp_s;
  unsigned char kop=0;
#ifdef NALAD
  nom_func("323");
#endif
  tester=9999;
#ifdef KOL_SOO2
	if(kim==2){FIN=STR1+STR2;FN=STR1;}
#endif
  if(kim==1){FIN=STR1;FN=0;}
	for(iv=FN;iv<FIN;iv++)
	if(bufstr[3]==podgruppa[iv+Smo-sto]){tester=iv; break;}
	if(tester==9999)return(tester);

	for (jv=0;jv<5;jv++) //пройти по всем возможным объектам сообщения
	{
		kh=spstr[tester][jv]; //взять номер очередного объекта
		if(kh==1111)continue; //если объект пустой пропустить
		vysv=0;
spar:
		l=1;//установить стартовое значение тест-бита
		n=jv+4;//взять номер байта сообщения с данными об объекте
		for(M=0;M<=5;M++)//пройти по всем битам объекта
		{ kop=bufstr[n]&l;//результат проверки бита
			if(kop==0)//если бит равен 0
			{ if(fr3[kh][M]!=0)//если ранее бит был не равен нулю
				{
					pooo_com[kh]=0l;
					com_bit[kh]=0;
					vysv=1;
					reza=1;
				}
				fr3[kh][M]=0;

				if(fr1[kh][6]!=0)//если стрелка в кресте
				{
					sp_s=(fr1[kh][6]&0xff00)>>8;  //получить объект для СП
					if((ZAGRUZKA==0)||(DISK!=0))
					{
						krest(sp_s,2,2);
						displaystrelka(fr1[kh][6]&0xff,M,vse);
					}
				}
//			else
					if((ZAGRUZKA==0)||(DISK!=0))displaystrelka(kh,M,vse);
			}
			else	//если бит не равен нулю
			{
				if(fr3[kh][M]!=1)//если раньше бит был не равен 1
				{
					pooo_com[kh]=0;
					com_bit[kh]=0;
					if(fr1[kh][6]!=0)
					{
						sp_s=(fr1[kh][6]&0xff00)>>8;  //получить объект для СП
						if((ZAGRUZKA==0)||(DISK!=0))
						{
							krest(sp_s,2,2);
							displaystrelka(fr1[kh][6]&0xff,M,vse);
						}
					}
					else
					if((ZAGRUZKA==0)||(DISK!=0))displaystrelka(kh,M,vse);
					fr3[kh][M]=1;
					vysv=1;
					reza=1;
				}
			}
			l=l*2;//перейти на слеюующий бит
		}

		if((vysv==0)&&(ZAGRUZKA==0))time20sec(kh);

		if (vse==1)vse=0;
		else
		if(fr1[kh][12]!=9999)
		{
			TEST_SPAR=2;
			para=poisk_ras(fr1[kh][2],kh,0,0);
			if(para!=9999)
			{
				for(M=0;M<6;M++)fr3[para][M]=fr3[kh][M];
				kh=para;
			}
			vse=1;
			goto spar;
		}
	}
	return(reza);
}
//-----------------------------------------------------------------------
/**********************************************\
* Процедура распаковки данных для контроллера  *
\**********************************************/
int kontroler(unsigned char bufde[12],int Smo,int kim,int sto)
{
	int vs=0,kh=0,Z=5,M,iv,jv,tester=9999,reza=9999,FN=0,FIN=0,l;
	unsigned char kop=0;

	nom_func("132");
	vysv=0;
#ifdef KOL_SOO2
	if(kim==2){FIN=UPR1+UPR2;FN=UPR1;}
#endif
	if(kim==1){FIN=UPR1;FN=0;}
	for(iv=FN;iv<FIN;iv++)if(bufde[3]==podgruppa[iv+Smo-sto]){tester=iv;break;}
	if(tester==9999) return(tester);
	for (jv=0;jv<Z;jv++)//пройти по всем объектам подгруппы
	{ kh=spkont[tester][jv];//взять очередной объект
		if(kh==1111) continue;//если объект не смотреть, то пропустить
		if(fr1[kh][1]==9999)goto BReak;//если объект пустой, то пропустить
		l=1; n=jv+4; //установить тест-бит в исходное,встать на байт первого объекта
		for(M=0;M<=5;M++)// пройти по битам объета
		{ kop=bufde[n]&l;//получить значение очередного бита
			if(kop==0)// если по объекту произошел сброс бита
			{ //если отпустило групповое или объектное
				if((fr1[kh][1]>=1000)&&(M<=1))
				{
					if((ZAGRUZKA==0)||(DISK!=0))
					{ if(fr3[kh][M]!=0)reza=1;//если до сих пор реле было под током
						fr3[kh][M+6]=fr3[kh][M];//запомнить ранее имевшееся значение для бита
					}
					fr3[kh][M]=0;//установить текущее значение
					//проверить корректность прохождения команды
					if((ZAGRUZKA==0)||(DISK!=0))
					test_deshifr(kh,M);
					goto kiv;
				}
			//далее для объектов не групповых и не объектных реле
				if(fr3[kh][M]!=0)//если объект был в состоянии 1
				{ fr3[kh][M]=0; //установить его в 0
					if((ZAGRUZKA==0)||(DISK!=0))
					{vysv=1;reza=1;}  //зафиксировать факт изменения
				}
			}
			else //если по объекту произошла установка бита
			{
				//если сработало групповое или объектное
				if((fr1[kh][1]>=1000)&&(M<=1))
				{
					if((ZAGRUZKA==0)||(DISK!=0))
					{ if(fr3[kh][M]!=1)reza=1;//зафиксировать факт изменения объекта
						fr3[kh][M+6]=fr3[kh][M];//запомнить предыдущее состояние бита
					}
					fr3[kh][M]=1;
					if((ZAGRUZKA==0)||(DISK!=0))
					{test_deshifr(kh,M);vysv=1;reza=1;}
					goto kiv;
				}
			//далее для объектов не групповых и не объектных реле
				if(fr3[kh][M]!=1)
				{ fr3[kh][M]=1;
					if((ZAGRUZKA==0)||(DISK!=0))
					{vysv=1;reza=1;}
				}
			}
kiv:
			if((ZAGRUZKA==0)||(DISK!=0))
			{ if((fr1[kh][1]==911)&&(vysv==1))//если проблемы с памятью
				{ Pam(kh);
					vysv=0;
					vs=1;
				}
				//при обнаружении ошибок работы выходного интерфейса
				if((fr1[kh][1]>=1000)&&(vysv==1)&&(M>1))
				{ dispdatch(kh,M);
					vysv=0;
					vs=1;
				}
				//для объекта KVV (ввода вывода)
				if((fr1[kh][1]==11)&&(vysv==1))
				{ kvv(kh,M);
					vysv=0;
				}
				//для объекта Ар из состава KS
				//если получено изменение по объекту KS (биты Р,Ао,Ар или непарафазность)
				if((fr1[kh][1]==13)&&(vysv==1)&&(M<=5))
				{komplekt(kh,M);vysv=0;vs=1;}
			}
			l=l*2;
		}
asa:
		if((vysv==1)&&(ZAGRUZKA==0))
		if(vs==1){vs=0; vysv=1; reza=1;}
		if((vysv==1)&&(ZAGRUZKA==0))
		{ if((fr1[kh][1]!=6)&&(fr3[kh][5]==1))slom_interf(kh);
			if(fr1[kh][1]==12)tel_vkl_vikl(kh);
		}
BReak:
		;//переход на следующий объект (если был не последний)
	}
it:
	return(reza);
}
//--------------------------------------------------------------
int signale(unsigned char bufsig[12],int Smo,int kim,int sto)
{
	int kh=0,M,jo,iv,jv,sled,tek,tester=9999,reza=9999,i_str,str,p,
	sp,kod,para,l,i_m;
	unsigned char kop=0;
	int FN=0,FIN=0;
	nom_func("304");
	tester=9999;
	if(bufsig[2]=='E')
	{
#ifdef KOL_SOO2
		if(kim==2){FIN=SIG1+SIG2;FN=SIG1;}
#endif
		if(kim==1){FIN=SIG1;FN=0;}
		for(iv=FN;iv<FIN;iv++)
		if(bufsig[3]==podgruppa[iv+Smo-sto]){tester=iv;break;}
		if(tester==9999)return(tester);
	}
	if(bufsig[2]=='Q')
	{
#ifdef KOL_SOO2
		if(kim==2){FIN=DOP1+DOP2;FN=DOP1;}
#endif
		if(kim==1){FIN=DOP1;FN=0;}
		for(iv=FN;iv<FIN;iv++)
		if(bufsig[3]==podgruppa[iv+Smo-sto]){tester=iv;break;}
		if(tester==9999)return(tester);
	}

	for(jv=0;jv<5;jv++)
	{ vysv=0;
		if(bufsig[2]=='E')kh=spsig[tester][jv];
#if DOP1>0
		if(bufsig[2]=='Q')kh=spdop[tester][jv];
#endif
		if(kh==1111)continue;
		l=32;
		n=jv+4;
		for(M=5;M>=0;M--)
		{ kop=bufsig[n]&l;
			if(kop==0)//если бит сбрасывается
			{
				if(fr3[kh][M]!=0)//если бит был установлен
				{ fr3[kh][M]=0;
					pooo[kh]=0;
					if((com_bit[kh]&l)!=0)
					{
						pooo_com[kh]=0;
						com_bit[kh]=0;
					}
					if((ZAGRUZKA==0)||(DISK!=0))
					if(fr1[kh][0]==2)displaysignal(kh,M);
					else
					{
						vysv=1;
						continue;
					}
					vysv=1;reza=1;
				}
			}
			else//если бит устанавливается
			{ if(fr3[kh][M]!=1)//если бит был сброшен
				{ fr3[kh][M]=1;
					pooo[kh]=0;
					if((com_bit[kh]&l)!=0)
					{
						pooo_com[kh]=0;
						com_bit[kh]=0;
					}

					if(M<4)//если устанавливается ВС или открывается сигнал
					{ if(fr3[kh][10]!=0)
						{ tek=kh;
							jo=0;
							while(1)
							{
								sled=fr3[tek][8]&0xfff;
								if((fr3[tek][10]==0)&&(fr3[tek][8]==0))break;
								TRASSA_RIS[jo++].object=tek;
								fr3[tek][8]=0;
								fr3[tek][10]=0;
								if(sled==0xfff)break;
								tek=sled;
								if(jo>=100)break;
							}
							ris_trassa(2);
							ZERO_TRASSA(2);
							for(i_m=0;i_m<=(3*skoko_stoek);i_m++)
							if(MARSHRUT_ALL[i_m].NACH==kh)
							{
								DeleteMarsh(i_m);
								break;
							}
						}
					}
					if((ZAGRUZKA==0)||(DISK!=0))
					{
						if(fr1[kh][0]==2)displaysignal(kh,M);
						else
						{
							vysv=1;
							continue;
						}
					}
					vysv=1;
					reza=1;
				 }
			}
			if((M<4)&&(ZAGRUZKA==0)&&(kh<kol_OSN))
			{ if(fr2[kh][8]!=0)sp=fr2[kh][8];//если есть СП,взять его номер
				if(fr2[kh][3]!=0)//если для сигнала есть стрелки в пути
				for(i_str=3;i_str<8;i_str++)
				{ if(fr2[kh][i_str]==0)break;//если очередной стрелки нет
					str=fr2[kh][i_str]&0xfff;//получить номер для стрелки
					p=(fr2[kh][i_str]&0xf000)>>12;//получить тип замыкания
					kod=fr3[kh][0]*8+fr3[kh][2]*2;//проверить маневровые
					kod=kod+fr3[kh][1]*4+fr3[kh][3];//добавить поездные
					switch(p)//переключение по типу замыкания
					{
						case 0: break;
						//четное поездное
						case 2: if(fr1[kh][1]==0)//если сигнал четный
										{
											//если поездной открыт или поездной ВС
											if((kod>11)||((kod>3)&&(kod<8))||(kod==1)||(kod==3))
											{
												if(fr3[sp][1]==1)//если СП замкнута
												fr4[str][0]=fr4[str][0]|8;
											}
											else//если поездной сигнал закрыт
											{
												if(fr3[sp][1]==0)//если СП разомкнута
												fr4[str][0]=fr4[str][0]&0xfff7;
												//если открылся маневровый
												if((kod<12)&&(kod>7))fr4[str][0]=fr4[str][0]&0xfff7;
											}
										}
										break;
							//четное любое
							case 3: if(fr1[kh][1]==0)//если сигнал четный
											{
												if(kod!=0)//если какой-либо сигнал открыт
												{
													if(fr3[sp][1]==1)//если СП замкнута
													fr4[str][0]=fr4[str][0]|8;
												}
												else //если все сигналы закрыты
												{
													if(fr3[sp][1]==0)//если СП разомкнута
													fr4[str][0]=fr4[str][0]&0xfff7;
												}
											}
											break;
							//нечетное поездное
							case 4: if(fr1[kh][1]==1)//если сигнал нечетный
											{
												if((kod>11)||((kod>3)&&(kod<8))||(kod==1)||(kod==3))
												{
													if(fr3[sp][1]==1)//если СП замкнута
													fr4[str][0]=fr4[str][0]|0x10;
												}
												else //если поездной сигнал закрыт
												{
													if(fr3[sp][1]==0)//если СП разомкнута
													fr4[str][0]=fr4[str][0]&0xffef;
													if((kod<12)&&(kod>7))fr4[str][0]=fr4[str][0]&0xffef;
												}
											}
											break;
							//нечетное любое
							case 5: if(fr1[kh][1]==1)//если сигнал нечетный
											{
												if(kod!=0)//если какой-либо сигнал открыт
												{
													if(fr3[sp][1]==1)//если СП замкнута
													fr4[str][0]=fr4[str][0]|0x10;
												}
												else //если все сигналы закрыты
												{
													if(fr3[sp][1]==0)//если СП разомкнута
													fr4[str][0]=fr4[str][0]&0xffef;
												}
											}
											break;
							//четное - нечетное поездные
							case 6:
												if((kod>11)||((kod>3)&&(kod<8))||(kod==1)||(kod==3))
												{
													if(fr3[sp][1]==1)//если СП замкнута
													fr4[str][0]=fr4[str][0]|0x18;
												}
												else //если поездной сигнал закрыт
												{
													if(fr3[sp][1]==0)//если СП разомкнута
													fr4[str][0]=fr4[str][0]&0xffe7;
													if((kod<12)&&(kod>7))fr4[str][0]=fr4[str][0]&0xffe7;
												}

												break;
						//четное -   нечетное любое
							case 7:   if(kod!=0)//если любой сигнал открыт
												{
													if(fr3[sp][1]==1)//если СП замкнута
													fr4[str][0]=fr4[str][0]|0x18;
												}
												else //если сигналы закрыты
												{
													if(fr3[sp][1]==0)//если СП разомкнута
													fr4[str][0]=fr4[str][0]&0xffe7;
												}
												break;
							default: w(44,sp," 4");break;
					}
					nach_zakr(str,0);
					if(fr1[str][12]!=9999)
					{ TEST_SPAR=2;
						para=poisk_ras(fr1[str][2],str,0,0);
						fr4[para][0]=fr4[str][0];
						nach_zakr(para,0);
					}
				}
			}
			l=l>>1;
		}
		if((vysv==0)&&(ZAGRUZKA==0))time20sec(kh);
Bre:;
	}
	return(reza);
}
//--------------------------------------------------------------------
/**************************************************\
* Процедура фиксации обновленных данных по каналам *
* knl, при указании необходимости need             *
\**************************************************/
void fixation(int knl, int need)
{
	nom_func("67");
	if(DISK!=0)return;//при просмотре архива ничего не делать
  if((need==1)||(ZAGRUZKA==1)) //при необходимости и загрузке
  { if((prv[0]==1)&&(knl==0))  //если новизна в первом ТУМСе
    {
			add(0,0,'п');                       //записать на диск
      SBROS_BIT_TEST_SOOB(0,bu[0][3]);  //учесть прием в массиве учета
      prv[0]=0;                         //снять новизну
    }
#ifdef KOL_SOO2                         //аналогично для второго ТУМС
    if((prv[1]==1)&&(knl==1))
    {
      add(1,0,'в');
      SBROS_BIT_TEST_SOOB(1,bu[1][3]);
      prv[1]=0;
    }
#endif
  }
  return;
}
//-----------------------------
//процедура вызывается при отсутствии изменений для текущего объекта
//предназначена для анализа получения изменений по объектам в заданный
//интервал времени после выдачи команды
void time20sec(int numba)
{
  long TIME_CC,vr;
  int vmv=0;

	nom_func("359");
	if((numba==AVAch)||(numba==AVAn))return;
	if((fr1[numba][0]==2)&&(numba<kol_OSN))TIME_CC=1400L;//для сигналов - 80 сек
	else TIME_CC=360L; //для других объектов - 20 сек

	if(pooo_com[numba]!=0l)//если для объекта включен таймер на команду
	{
		vmv=fr1[numba][13]-1;
		//коррекция с учетом возможной очереди объектов
		vr=biostime(0,0l);  //зафиксировать текущее время
		if((vr-pooo_com[numba])>=TIME_CC) //если прошло более заданного
		{
#ifdef KOL_SOO2
			if(flagoutmsg==12)//если задан сквозной маршрут
			{
				if((fr1[numba][0]==2)&&(fr1[numba][1]==1))//если нечетный маршрут

				{
					for(vmv=0;vmv<1;vmv++)//пройти по нечетным маршрутам
					if(Skvo[vmv].B2_s==numba)//если это сигнал начала второй половины
					{
						fin_kom();//сбросить команду
						break;
					}
				}
				else //если четный маршрут
				{
					for(vmv=1;vmv<2;vmv++) //пройти по четным маршрутам
					if(Skvo[vmv].B2_s==numba)//если это сигнал начала второй половины
					{
						fin_kom();//сбросить команду
						break;
					}
				}
			}
#endif
			t_com[vmv]=pooo_com[numba];
			//zvuk_vkl(1,30);//дать звуковой сигнал
			if(t_com[vmv]!=0)
			{
				t(0);
				w(239,numba,"");
				t_com[vmv]=0;
			}
			if(pooo_com[numba]!=0l)
			{
				pooo_com[numba]=0l;//сбросить фиксатор времени
				com_bit[numba]=0;
			}
			test_prigol_otm(numba);
			test_ra_fini(numba);
		}
		else test_not_ra(numba);
	}
	return;
}
//------------------------------------------------
void test_prigol_otm(int nemba)
{
	nom_func("349");
	//если вспомогательная смена направления
	if((fr1[nemba][0]==2)&&( fr1[nemba][1]>=400)&&( fr1[nemba][1]<=600))
  {
    fr3[nemba][9]=0;//сброс флага команды 
    vtora=0;//сброс признака для выдачи второй половины команды 
    if((nikuda==0)&&(klo==0)&&(help==0))
    {
      if((otv_kom==1)&&(i3==modi))home(modi);//курсор на место 
      t(0);//очистить строку интерактива 
    }
    zabeg=0;//сброс флагов
  }
  return;
}
//--------------------------------------------------------------
void test_not_ra(int nemba)
{
	nom_func("343");
	if((fr1[nemba][0]==3)||(fr1[nemba][0]==4))
  if((fr1[nemba][1]==301)||((fr1[nemba][1]>=200)&&(fr1[nemba][1]<=300)))
  fr3[nemba][9]=0;//сброс флага выдачи команды
  return;
}
/**********************************************************/
void test_ra_fini(int nemba)
{
	nom_func("351");
	if((fr1[nemba][0]==3)||(fr1[nemba][0]==4))//если УП или СП
  if((fr1[nemba][1]>=200)&&(fr1[nemba][1]<=300))//объект разделки (РИ)
  {
    fr3[nemba][9]=0;//сбросить метку команды
		was[(markery[fr1[nemba][5]][7]&0xfff)-10]=0;//сбросить флаг в массиве учета
    if((klo==0)&&(nikuda==0)&&(help==0))//если система в обычном состоянии
    {
      if((otv_kom==1)&&(i3==modi)&&(fr1[nemba][5]==modi))//если ОК и маркер на объекте
      {
        t(0); //стереть окно интерактива
        zalet=0;vtora=0;//сбросить флаги
        regs.h.ah=5;
        regs.h.ch=0x39;
        regs.h.cl=0x20;
        int86(0x16,&regs,&regs);
        w(169,nemba,"09");
        zvuk_vkl(1,9);
      }
    }
  }
  return;
}
//---------------------------------------------------------------------
/*****************************************************\
* Процедура изображения СП для крестовых стрелок      *
* ob - номер объекта СП в базе для крестовой стрелки  *
* BT - номер бита,для которого надо рисовать СП       *
* param - 2-не рисовать,но установить запреты         *
*           используется на этапе распаковки стрелок  *
*         1-бит установлен - рисовать                 *
*         0-бит сброшен - рисовать                    *
* если обе стрелки в плюсе - рисовать оба СП          *
* если обе стрелки в минусе - рисовать оба СП         *
* при разном положении - рисовать только минусовую    *
* запрет распространен на занятость,замкнутость,МСП и *
* разделку                                            *
\*****************************************************/
void krest(int ob,int BT,int param)
{
	int str_l, //левая стрелка СП
	str_p, //правая стрелка СП
	sp_l,//левая часть СП
	sp_p; //правая часть СП
	str_l=(fr1[ob][3]&0xff00)>>8;//взять левую стрелку
	str_p=fr1[ob][3]&0xff;//взять правую стрелку
	sp_l=(fr1[str_l][6]&0xff00)>>8;//взять сп для левой стрелки
	sp_p=(fr1[str_p][6]&0xff00)>>8;//взять сп для правой стрелки
	if((fr3[str_l][0]==1)&&(fr3[str_l][1]==0))//если левая в плюсе
	{
		if((fr3[str_p][0]==1)&&(fr3[str_p][1]==0))//если правая в плюсе
		{
			fr3[sp_l][7]=0;
			fr3[sp_p][7]=0;
		}
		if((fr3[str_p][0]==0)&&(fr3[str_p][1]==1))//если правая в минусе
		{
			fr3[sp_l][7]=1;//запретить рисовать левое МСП
			fr3[sp_p][7]=0;
		}
	}
	else
		if((fr3[str_l][0]==0)&&(fr3[str_l][1]==1))//если левая в минусе
		{
			if((fr3[str_p][0]==1)&&(fr3[str_p][1]==0))//если правая в плюсе
			{
				fr3[sp_l][7]=0;
				fr3[sp_p][7]=1;//запретить рисовать правое МСП
			}
			if((fr3[str_p][0]==0)&&(fr3[str_p][1]==1))//если правая в минусе
			{
				fr3[sp_l][7]=0;
				fr3[sp_p][7]=0;
			}
		}
		else
		{
			fr3[sp_l][7]=0;
			fr3[sp_p][7]=0;
		}
	if(param==2)return;
	if(param==0)//если получен 0
	{
		if((fr3[sp_l][BT]!=0)||(fr3[sp_p][BT]!=0))
		{
			vysv=1;  fr3[sp_l][BT]=0;   fr3[sp_p][BT]=0;
			goto ris;
		}
		return;
	}
	if(BT==5)
	{
		fr3[sp_l][5]=1;
		fr3[sp_p][5]=1;
		goto ris;
	}
	//если получена 1
	//и если для СП не было ранее получено данных
	//фиктивный СП при загрузке не получил значений
	if((fr3[str_l][0]==2)||(fr3[str_p][0]==2))
	{
		fr3[sp_l][BT]=1;
		fr3[sp_p][BT]=1;
		goto ris;
	}

	fr3[sp_l][BT]=1;  fr3[sp_p][BT]=1;
ris:
		if((ZAGRUZKA==0)&&(nikuda==0)&&(klo==0)&&(help==0))
		{
			sekci(sp_l,BT);
			sekci(sp_p,BT);
		}
	return;
}
