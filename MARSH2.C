#include "bla.h"
//------------------------------------------------------------------
/**********************************************************\
* Сброс элементов маршрута, снятие предварительного замык. *
* прорисовка трассы маршрута без замыкания.                *
\**********************************************************/
void finish_startMM(int tipus)
{
	int jj,maska,element,i;

	nom_func("60");
	if(tipus!=0)jj=TRASSA[0].object&0xfff;
	else jj=TRASSA0[0].object&0xfff;

	if(fr1[jj][0]==2)//если сигнал
	{
		if(mane==0)//если поездной
		{
			if(fr1[jj][1]==0)maska=0xA0;//если четное направление
			else maska=0x50;//если нечетное
		}
		else
		{
			if(fr1[jj][1]==0)maska=0x20;//если четное направление
			else maska=0x10;//если нечетное
		}
	}
	for(jj=0;jj<100;jj++)
	{
		if(tipus!=0)
		{
			if(TRASSA[jj].object==0xffff)break;
			element=TRASSA[jj].object&0xfff;
			for(i=0;i<skoko_stoek*3;i++)
			if(MARSHRUT_ALL[i].NACH==element)
			DelMarsh(i);
		}
		else
		{
			if(TRASSA0[jj].object==0xffff)break;
			element=TRASSA0[jj].object&0xfff;
			for(i=0;i<skoko_stoek*3;i++)
			if(MARSHRUT_ALL[i].NACH==element)
			DelMarsh(i);
		}
		fr3[element][10]=0;
		fr3[element][6]=0;
	}
	ris_trassa(tipus);
	ZERO_TRASSA(tipus);
	for(jj=0;jj<(skoko_stoek*3);jj++)
	{
		if(tipus!=0)
		{
			if(fr3[nach_marsh][10]>1)continue;
			if(((MARSHRUT_ALL[jj].NACH&0xfff)==nach_marsh)&&
			((MARSHRUT_ALL[jj].END&0xfff)==point))break;
		}
		else
		{
			if(fr3[nach_marsh_oper][10]>1)continue;
			if(((MARSHRUT_ALL[jj].NACH&0xfff)==nach_marsh_oper)&&
			((MARSHRUT_ALL[jj].END&0xfff)==point))break;
		}
	}
	if(jj<skoko_stoek)DeleteMarsh(jj);
	if(tipus==0)
	{
		vozvr=0;
		nach_marsh_oper=0;
		end_marsh_oper=0;
		nach0_marsh_oper=0;
		END_OPER=0;
	}
	if(tipus==1)
	{
		nach_marsh=0;
		end_marsh=0;
		nach0_marsh=0;
		half_marsh=0;
		END=0;
	}
	povt1=0;
	return;
}
/******************************************************\
*  Проверка состояния стрелки.При удаче возвращает 0   *
*  при неудаче возвращает 1                            *
*  S - объект для стрелки                              *
*  polog - требуемое положение для стрелки в маршруте  *
*  polog=1 - стелка в плюсе; polog=-1 стрелка в минусе *
\******************************************************/
int tst_str(int S,int polog,int fl,int tipus)
{
	int U;

	nom_func("371");
	if(fr3[S][5]==1){ w(72,S,"");return(1);}//непарафазность = неисправна стрелка
	if(fr3[S][0]==fr3[S][1]){w(127,S,"");return(1);}//НЕ ИМЕЕТ КОНТРОЛЯ СТРЕЛКА
	if(fr4[S][2]==1){w(71,S,"");return(1);}//ЗАПРЕЩЕНО ДВИЖЕНИЕ ПО СТРЕЛКЕ
	if((fr4[S][0])!=0) //если ограничения по управлению
	{
		if((fr3[S][0]==0)&&(fr3[S][1]==1)&&(polog==-1))return(0);
		if((fr3[S][0]==1)&&(fr3[S][1]==0)&&(polog==1))return(0);
		w(67,S,"");//ОТКЛ. ОТ УПРАВЛ. СТРЕЛКА
		return(1);
	}
	if((fr4[S][0]&0xfff8)!=0) //если ограничения по управлению
	{
		if((fr3[S][0]==0)&&(fr3[S][1]==1)&&(polog==-1))return(0);
		if((fr3[S][0]==1)&&(fr3[S][1]==0)&&(polog==1))return(0);
		w(240,S,"");//СТРЕЛКА ЗАМКНУТА В МАРШРУТЕ
		return(1);
	}
	if(((tst_chet(S)==0)&&(fr3[RZ_CHET][RZ_CHET_BIT]==1))||
	((tst_chet(S)==1)&&(fr3[RZ_NECHET][RZ_NECHET_BIT]==1)))
	{
		if((fr3[S][0]==0)&&(fr3[S][1]==1)&&(polog==-1))return(0);
		if((fr3[S][0]==1)&&(fr3[S][1]==0)&&(polog==1))return(0);
		w(166,S,"");//ВКЛЮЧЕНО РУЧНОЕ ЗАМЫКАНИЕ СТРЕЛОК
		return(1);
	}
	if((fr4[S][1]==1)&&(fl!=0))
	{ if(tipus==0)
		{
			if((mane==0)&&((fr1[nach_marsh_oper][13]!=fr1[end_marsh_oper][13])||(AVARIIN==1)))
			goto figovo;
		}
		if(tipus==1)
		{
			if((mane1==0)&&((fr1[nach_marsh][13]!=fr1[end_marsh][13])||(AVARIIN==1)))
			goto figovo;
		}
		if((fr3[S][0]==0)&&(fr3[S][1]==1)&&(polog==-1))   return(0);
		if((fr3[S][0]==1)&&(fr3[S][1]==0)&&(polog==1)) return(0);
figovo:
		w(99,S," ВКЛЮЧЕНА НА МАКЕТ");
		return(1);
	}
	if((fr3[S][0]==0)&&(fr3[S][1]==1)&&(polog==-1))return(0); //если не требуется перевод
	if((fr3[S][0]==1)&&(fr3[S][1]==0)&&(polog==1))return(0); //если не требуется перевод
	U=findspu1(S,fr1[S][7]);
	if(U==-1)return(0); //если нет СП
	if(tst_spu(U,fl,1,tipus)==1) return(1);
	if(fr1[S][12]!=9999)
	{
		TEST_SPAR=2;
		U=poisk_ras(fr1[S][2],S,tipus,0);
		U=findspu1(U,fr1[U][7]);
		if(U==-1)
		return(0);
		if(tst_spu(U,fl,1,tipus)==1) return(1);
	}
	return(0);
}
//------------------------------------------------------
//Проверка занятости участка pu
//avto - признак работы в автодействии

int zanyat(int pu,int avto,int tipus)
{

	nom_func("431");
	if(fr3[pu][0]==0) return(0);//если свободен - выйти с разрешением

	 //если аварийный перевод стрелок и маркер на стрелке - разрешить
	if((AVARI==1)&&(markery[modi][7]==1))return(0);

	if((mane==1)||(manevro==1))//если маневровый маршрут
	{
		if(tipus==0)
		{
			if(pu==END_OPER)return(0);//если участок приема - разрешить
		}
	}
	if((mane1==1)||(manevro1==1))//если маневровый маршрут
	{
		if(tipus!=0)
		{
			if(pu==END)return(0);//если участок приема - разрешить
		}

	}
	//если не автодействие,то выдать на экран
	if(avto==0){w(94,pu,""); soob_for_oper=1;}//ЗАНЯТ УЧАСТОК

	return(1);
}
//---------------------------------------------------------
//Проверка замкнутости участка pu
//avt - признак анализа в автодействии 1-есть 0-нет
//perevod - требование перевода стрелки 1-есть 0-нет
int zamknut(int pu,int avt,int perevod)
{
	int MSP,BIT_MSP;

	nom_func("429");
	if((fr3[pu][10]!=0)&&(avt==0)&&(DISK==0))
	{
		w(238,pu,"");
		return(5);
	}
	if(fr3[pu][1]!=0)//участок замкнут
	{
		//ЗАМКНУТ В МАРШРУТЕ УЧАСТОК
		if((avt==0)||(avt==3)){w(96,pu,"");soob_for_oper=1; return(1);}
		else
		{
				if(avt>=10)avt=avt-10;
			//если замкнутая секция не перекрывная или конечный сигнал маршрута закрыт
			if((pu!=Avto_el[avt].N_sek)||(fr3[Avto_el[avt].End_s][1]==0))return(1);
		}

	}
	MSP=fr1[pu][11];// МСП
	BIT_MSP=fr1[pu][12];//бит МСП
#ifdef AVTOD
	if(avt==2)//если четное автодействие
	{ //если это секция автодействия
		if((pu==Avto_el[2].N_sek)||(pu==Avto_el[3].N_sek))
		{ if(fr3[pu][1]==0)//если СП разомкнута
			{ //если есть мсп
				if((MSP!=9999)&&(perevod!=0))
				{
					if(fr3[MSP][BIT_MSP]==0)return(0);// МСП в исходном
					else return(1);//МСП не в исходном
				}
				else return(0);
			}
			else return(0);
		}
	}
	if(avt==1)//если нечетное автодействие
	{ //если это секция автодействия
		if((pu==Avto_el[0].N_sek)||(pu==Avto_el[1].N_sek))
		{ if(fr3[pu][1]==0)//если СП разомкнута
			{ //если есть мсп
				if((MSP!=9999)&&(perevod!=0))
				{
					if(fr3[MSP][BIT_MSP]==0)return(0);//МСП в исходном
					else return(1);//МСП не в исходном
				}
				else return(0);
			}
			else return(0);
		}
	}
#endif
	if((avt==0)&&(AVARI==0))//если нет автодействия и аварийного перевода
	{
		if((MSP!=9999)&&(perevod!=0))//если есть мсп и нужен перевод
		{ if(fr3[MSP][BIT_MSP]==0)return(0);//если МСП в исходном
			else
			{
				if(get_str(MSP,BIT_MSP)==0)w(96,999,chudo);
				soob_for_oper=1;//ЗАМКНУТ В МАРШРУТЕ УЧАСТОК
				return(1);
			}
		}
		return(0);
	}
	return(0);
}
//--------------------------------------------------------------------
/*************************************************\
* Процедура проверки наличия признака разделки    *
* для объекта pu и выдача сообщения о его нали-   *
* чии, если флаг av установлен в состояние 1.     *
* Если признак разделка установлен - возвращает 1,*
* если не установлен - возвращает 0.              *
\*************************************************/
int razdelan(int pu,int av)
{

	nom_func("264");
	if(fr3[pu][2]==0) return(0);
	if(av!=1)
	{
		w(98,pu,"");/*ВКЛЮЧЕНО ИСКУССТВЕННОЕ РАЗМЫКАНИЕ */
		soob_for_oper=1;
	}
	return(1);
}
//-----------------------------------------------------
int tst_spu(int P,int avt,int perevod,int tipus)
{
	int k,tstr;//номер объекта в массиве BEDA
	unsigned int Test;

	nom_func("370");
	for(k=0;k<10;k++)if((BEDA[k]&0xfff)==P)break;
	if(k==10)for(k=0;k<10;k++)if(BEDA[k]==0)break;
	if(k==10)for(k=0;k<10;k++)BEDA[k]=0;
	if(fr3[P][5]==1){w(104,P,"");return(1);}//НЕИСПРАВЕН УЧАСТОК
	if((uprav==0)&&(avt<10))
	{ Test=P|0x1000;
		if((Test&BEDA[k])!=Test)
		if((osya<2)&&(zanyat(P,avt,tipus)==1))goto brak;
		Test=P|0x2000;
		if((Test&BEDA[k])!=Test)
		if(osya<3)
		{
			switch(zamknut(P,avt,perevod))
			{
				case 1:
				{
					if(AVARI==1)
					{
						home(modi);
						finish_strelok();
						klaval=0;
						return(10);
					}
					goto brak;
				}
				case 5:
				{
        	goto brak;
				}

			}
		}
		Test=P|0x4000;
		if((Test&BEDA[k])!=Test)
		if((osya<4)&&(razdelan(P,avt)==1))
		{ if(AVARI==1){home(modi);finish_strelok();klaval=0;return(10);}
			goto brak;
		}
		return(0);
	}
	else
	{ if(zanyat(P,avt,tipus)==1)return(1);

		tstr=zamknut(P,avt,perevod);
		return(tstr);
		if(razdelan(P,avt)==1)
		return(1);
		return(0);
	}
brak:
	BEDA[k]=BEDA[k]|Test;
	soob_for_oper=0;
	return(1);
}
//---------------------------------------------------
//процедура заполнения матрицы SP_STR[10][5]
int zap_matr(int object)
{
	int pereskok,ukaz_tek,ukaz_zap,tek_obkt,i,j,shag;

	nom_func("434");
	for(i=0;i<10;i++)for(j=0;j<5;j++)SP_STR[i][j]=0xFFFF;
  tek_obkt=object;
  ukaz_tek=0;
  ukaz_zap=0;
  if(fr1[tek_obkt][0]==3&&fr1[tek_obkt][1]<2)//если вошли с объектом типа СП
  {
metka_d:
    SP_STR[0][0]=tek_obkt;//запомнить номер объекта в 0-й строке
metka_c:
    if(fr1[tek_obkt][1]==0)//если для СП нет четного стыка 
    {
      if(SP_STR[0][2]==0xFFFF)//если для СП не записана четная ссылка
      {
        shag=-1;
k1:
        pereskok=0;
        tek_obkt=tek_obkt+shag;
k2:     if(fr1[tek_obkt][0]!=1)//если попали не на стрелку 
        {
          if(fr1[tek_obkt][0]!=7)//если попали не на ДЗ
          { //если попали не на переход 
            if(fr1[tek_obkt][0]!=6)
              if(fr1[tek_obkt][0]!=3)//если не СП
metka_a:      return(-1);
              else goto k1;//если СП
            else//если переход 
            {
              if(pereskok==0)//если вышли со строки
              {
                tek_obkt=fr1[tek_obkt][1];
                pereskok=1;
                goto k2;
              }
              else goto k1;//если вышли с перехода или стрелки 
						}
          }
          else goto k1;//если попали на ДЗ
        }
        else//если попали на стрелку 
        {
          i=v_spiske(tek_obkt);
          if(i==0)//если в матрице не записана
          {
            if(SP_STR[ukaz_zap][0]!=0xFFFF)ukaz_zap++;
            SP_STR[ukaz_zap][0]=tek_obkt;
          }
          else goto metka_a;
          if(shag==-1)SP_STR[0][2]=ukaz_zap;//если в четную сторону 
          else SP_STR[0][3]=ukaz_zap;//если в нечетную сторону 
          if((fr1[tek_obkt][7]==0)&&shag==1)//если вход на стрелку совпадает с шагом 
          {
            SP_STR[ukaz_zap][1]=0;
          }
          goto k3;
        }
      }
      else goto metka_b;//если заполнена ссылка для четной стороны СП
    }
    else//если есть четный стык
    {
      SP_STR[0][2]=0x0AAAA;
metka_b:
      if(fr1[SP_STR[0][0]][2]==0)//если для СП нет нечетного стыка 
      {
        if(SP_STR[0][3]==0xFFFF)//если не заполнена ссылка для нечетного стыка 
        {
          shag=1;
          goto k1;
        }
				else goto k3;
      }
      else//если у СП есть нечетный стык
      {
        SP_STR[0][3]=0xAAAA;
        if(SP_STR[0][2]==0xAAAA)return(0);//если СП без стрелок 
        goto k3;
      }
    }
  }
  else//если вошли не с типом СП
  {
    if(fr1[tek_obkt][0]==1)//если вошли с объектом типа стрелка 
    {
      SP_STR[1][0]=tek_obkt;
      ukaz_tek=1;
      ukaz_zap=2;
      goto metka_g;
    }
  }
k3:
  if(fr1[SP_STR[ukaz_tek][0]][0]==3)//если в текущей строке - объект СП
  {
    if(ukaz_tek!=0)goto metka_a;//если строка не нулевая - выход с отказом 
    else//если строка нулевая 
      if(SP_STR[ukaz_tek][2]==0xFFFF)goto metka_c;//если четная ссылка не заполнена 
      else
        if(SP_STR[ukaz_tek][3]==0xFFFF)goto metka_b;//если нечетная ссылка не заполнена 
        else
metka_f:
        ukaz_tek++;
        tek_obkt=SP_STR[ukaz_tek][0];
  }
  else goto k4;
  if(SP_STR[ukaz_tek][0]!=0xFFFF)//если текущая строка имеет объект
	{
k4:
    if(fr1[SP_STR[ukaz_tek][0]][0]==1)goto metka_g;//если этот объект-стрелка 
    else goto metka_a;//если не стрелка - выход с отказом 
  }
  else
  {
    if(ukaz_tek>1)return(0);//успешное завершение
    else goto metka_a;
  }
metka_g:
  if(SP_STR[ukaz_tek][1]==0xFFFF&&
  ((fr1[SP_STR[ukaz_tek][0]][11]&128)!=128))//если нет записи для входа на стрелку 
  {
    if(fr1[SP_STR[ukaz_tek][0]][7]==0)shag=1;//если вход на стрелку четный 
    else shag=-1;
k5: pereskok=0;
    tek_obkt=tek_obkt+shag;
k6: if(fr1[tek_obkt][0]!=1)//если попали не на стрелку 
    {
      if(fr1[tek_obkt][0]==7)goto k5;//если попали на ДЗ
      if(fr1[tek_obkt][0]==6)//если попали на переход 
      {
        if(pereskok!=0)goto k5;
        else
        {
          tek_obkt=fr1[tek_obkt][1];
          pereskok=1;
          goto k6;
        }
      }
      else//если не переход 
      {
        if(fr1[tek_obkt][0]==3&&fr1[tek_obkt][1]<2)//если вышли на СП
        {
					if((SP_STR[0][0]!=0xFFFF)&&(SP_STR[0][0]!=tek_obkt))goto metka_a;//если нулевая строка занята выход с отказом
          else SP_STR[ukaz_tek][1]=0;
k7:       if(shag==1)SP_STR[0][2]=ukaz_tek;
          else SP_STR[0][3]=ukaz_tek;
          ukaz_tek=0;
          goto metka_d;
        }
        else goto metka_a;
      }
    }
    else //если попали на стрелку 
    {
      i=v_spiske(tek_obkt);//проверить наличие в матрице найденного объекта 
      if(i==0)//если ранее не внесен
      {
        if(SP_STR[ukaz_zap][0]!=0xFFFF)ukaz_zap++;
        SP_STR[ukaz_zap][0]=tek_obkt;
        SP_STR[ukaz_tek][1]=ukaz_zap;
      }
      else SP_STR[ukaz_tek][1]=i;
      goto metka_e;
    }
  }
  else //если есть запись для входа на стрелку 
  {
metka_e:
    tek_obkt=SP_STR[ukaz_tek][0];
    if(SP_STR[ukaz_tek][2]==0xFFFF)//если нет записи для объекта по "+" ветви 
    { if(fr1[SP_STR[ukaz_tek][0]][4]==0)//если нет стыка по плюсу 
      { if(fr1[SP_STR[ukaz_tek][0]][1]==1)//если переход по плюсу прямой
        {
          if(fr1[SP_STR[ukaz_tek][0]][7]==0)shag=-1;//если вход на стрелку четный
          else shag=1;
k8:       pereskok=0;
          tek_obkt=tek_obkt+shag;
k9:       if(fr1[tek_obkt][0]!=1)//если вышли не на стрелку
          { if(fr1[tek_obkt][0]==7)goto k8;//если вышли на ДЗ
            if(fr1[tek_obkt][0]==6)
            { if(pereskok!=0)goto k8;
              else
              { tek_obkt=fr1[tek_obkt][1];
                pereskok=1;
                goto k9;
              }
            }
            if(fr1[tek_obkt][0]!=3)goto metka_a;
            else
              if((SP_STR[0][0]!=0xFFFF)&&(SP_STR[0][0]!=tek_obkt))goto metka_a;//если строка 0 занята
              else
              { SP_STR[ukaz_tek][2]=0;
                goto k7;
              }
          }
          else//если вышли на стрелку 
          {
            i=v_spiske(tek_obkt);
            if(i==0)//если ранее в матрицу не внесен
            {
              if(SP_STR[ukaz_zap][0]!=0xFFFF)ukaz_zap++;
              SP_STR[ukaz_zap][0]=tek_obkt;
              SP_STR[ukaz_tek][2]=ukaz_zap;
            }
            else SP_STR[ukaz_tek][2]=i;
            goto metka_h;
          }
        }
        else//если переход по плюсу отклоненный 
        {
          pereskok=1;
          tek_obkt=fr1[tek_obkt][2];
					goto k9;
        }
      }
      else//если есть стык по "+" ветви 
      {
        SP_STR[ukaz_tek][2]=0xAAAA;
        goto metka_h;
      }
    }
    else goto metka_h;
  }
metka_h:
  if(SP_STR[ukaz_tek][3]==0xFFFF)//если нет записи для "-" ветви 
  {
    if(fr1[SP_STR[ukaz_tek][0]][5]==0)//если нет стыка по минусу
    {
      if(fr1[SP_STR[ukaz_tek][0]][1]==0)//если переход по минусу прямой
      {
        if(fr1[SP_STR[ukaz_tek][0]][7]==0)shag=-1;//если вход четный
        else shag=1;
k10:    pereskok=0;
        tek_obkt=tek_obkt+shag;
k11:    if(fr1[tek_obkt][0]!=1)//если не вышли на стрелку 
        { if(fr1[tek_obkt][0]==7)goto k10;//если вышли на ДЗ
          if(fr1[tek_obkt][0]==6)
          { if(pereskok!=0)goto k10;
            else
            { tek_obkt=fr1[tek_obkt][1];
              pereskok=1;
              goto k11;
            }
          }
          else
          { if(fr1[tek_obkt][0]==3&&fr1[tek_obkt][1]<2)//если вышли на СП
            { if((SP_STR[0][0]!=0xFFFF)&&(SP_STR[0][0]!=tek_obkt))goto metka_a;//если нулевая строка занята
							else
              { SP_STR[ukaz_tek][3]=0;
                goto k7;
              }
            }
            else goto metka_a;
          }
        }
        else//если вышли на стрелку 
        { i=v_spiske(tek_obkt);
          if(i==0)
          { if(SP_STR[ukaz_zap][0]!=0xFFFF)ukaz_zap++;
            SP_STR[ukaz_zap][0]=tek_obkt;
            SP_STR[ukaz_tek][3]=ukaz_zap;
          }
          else SP_STR[ukaz_tek][3]=i;
          goto metka_f;
        }
      }
      else //если переход по минусу отклоненный 
      { if(fr1[SP_STR[ukaz_tek][0]][7]==0)shag=-1;//если вход четный
        else shag=1;
        pereskok=1;
        tek_obkt=fr1[SP_STR[ukaz_tek][0]][2];
        goto k11;
      }
    }
    else //если есть стык по минусу
    { SP_STR[ukaz_tek][3]=0xAAAA;
      goto metka_f;
    }
  }
  else goto metka_f; //если есть запись для минус ветви
}
//--------------------------------------------------------
int v_spiske(int ob)
{
	int i;

	nom_func("387");
	for(i=1;i<10;i++)
	if(SP_STR[i][0]==ob)return(i);
	return(0);
}
//------------------------------------------------------------------
/***********************************************************\
* процедура расстановки кодов цвета для матрицы стрелок-СП  *
\***********************************************************/
int cvet_matr()
{
	int prohod=0,cod,cvet_sp,SP,STREL,stroka_str,ukaz_strok=0,i,cvet_strel,
	str_plus,str_minus,avto_voz,msp;

	nom_func("29");
	SP=SP_STR[ukaz_strok][0];//получить номер объекта для СП
	cod=fr3[SP][0]+//занятость
	fr3[SP][1]*2+//замкнутость
	fr3[SP][2]*4;//разделка

	//если непарафазность СП
	if(fr3[SP][5]==1)
		cvet_sp=11;
	else
	{

		//если нет виртуального замыкания и подсветки положения стрелок
		if(((fr3[SP][12]&0xff0)==0)||(podsvetka!=0))
		switch(cod)
		{
			case 0: cvet_sp=8;break;//если в исходном
			case 1: cvet_sp=12;break;//если занят
			case 2: cvet_sp=46;break;//если замкнут -желтый + 32-толстый
			case 3: cvet_sp=44;break;//занят и замкнут - красный +32-толстый
			case 4: cvet_sp=104;break;//если в разделке - серый +64-пунктир +32-толстый
			case 5: cvet_sp=76;break;//занят и в разделке -красный+64-пунктир
			case 6: cvet_sp=110;break;//замкнут в разделке -желтый +64-пунктир+32-толстый
			case 7: cvet_sp=108;break;//занят,замкнут в разделке-красный+64-пунктир+32-толстый
		}
		else //если подсветка или виртуальное замыкание
		{
			switch(fr3[SP][12]&0xff0) //переключатель по виду замыкания
			{
				case 16: cvet_sp=6;break;   //коричневый
				case 32: cvet_sp=38;break;  //коричневый толстый
				case 64: cvet_sp=38;break;  //коричневый толстый
			}
		}
	}
	if((fr3[SP][7]==1)&&(cvet_sp!=11))
	cvet_sp=8; //если в корыте и не надо рисовать

	if((fr1[SP][11]!=9999)&&(cvet_sp==8))//если для СП есть МСП
	{
		msp=fr1[SP][11];
		if((fr1[msp][1]==19)||(fr1[msp][0]==3))

		if((fr3[msp][fr1[SP][12]]==1)&&(fr3[SP][7]==0))
		cvet_sp=37;
#ifdef AVTOVOZVRAT
		if(fr1[msp][1]==199)//если МСП для стрелки с автовозвратом
    {
      avto_voz=fr1[msp][2];//номер объекта автовозвратов для этого МСП


      if((fr3[coAV[avto_voz].N_sst][0]!=coAV[avto_voz].b0)||
      (fr3[coAV[avto_voz].N_sst][1]!=coAV[avto_voz].b1))
      if(fr3[fr1[SP][11]][fr1[SP][12]]==1)
      cvet_sp=37;
    }
    else
		if((fr3[fr1[SP][11]][fr1[SP][12]]==1)&&(fr3[SP][7]==0))
    cvet_sp=37;
#endif
  }
	if((podsvetka==1)&&(cvet_sp==8)&&(fr3[SP][7]==0))cvet_sp=33;//при подсветке-цвет синий,толстый
  SP_STR[ukaz_strok][4]=cvet_sp;
  if(SP_STR[ukaz_strok][2]!=0xAAAA)//если нет четного стыка
  {
    stroka_str=SP_STR[ukaz_strok][2];//установить цвет
    SP_STR[stroka_str][4]=cvet_sp;
  }
  if(SP_STR[ukaz_strok][3]!=0xAAAA)//если нет нечетного стыка
  {
    stroka_str=SP_STR[ukaz_strok][3];
    SP_STR[stroka_str][4]=cvet_sp;
  }
	if((SP_STR[ukaz_strok][2]==0xAAAA)&&(SP_STR[ukaz_strok][3]==0xAAAA))return(0);//если СП без стрелок
b_metka:
  ukaz_strok++;
c_metka:
  if(SP_STR[ukaz_strok][0]==0xFFFF)//если строка пустая
  {
    for(i=1;i<ukaz_strok;i++)
    if(SP_STR[i][4]==0xFFFF)
    {
      prohod++;
      if(prohod<5){ukaz_strok=0;goto b_metka;}
      else return(-1);
    }
    if(i==ukaz_strok)return(0);//если все цвета расставлены
  }
  else //если не пустая строка
  {
    if(SP_STR[ukaz_strok][4]==0xFFFF)goto b_metka;//если цвет для стрелки не установлен
    STREL=SP_STR[ukaz_strok][0];//взять объект для стрелки
    cvet_strel=SP_STR[ukaz_strok][4];//взять цвет для стрелки
    if(fr3[STREL][5]==1)//если стрелка непарафазная
    {
      SP_STR[ukaz_strok][4]=11;//установить для нее цвет циан
      cvet_strel=11;
      if(SP_STR[ukaz_strok][2]<0xAAAA)//если нет стыка по плюсу
      {
        str_plus=SP_STR[ukaz_strok][2];//взять объект для стрелки по "+" ветви
        SP_STR[str_plus][4]=cvet_strel;//установить цвет для стрелки за "+" ветвью
      }
      if(SP_STR[ukaz_strok][3]<0xAAAA)//если нет стыка по минусу
      {
        str_minus=SP_STR[ukaz_strok][3];//взять объект для стрелки по "-" ветви
        SP_STR[str_minus][4]=cvet_strel;//установить цвет для стрелки за "-" ветвью
      }
      goto b_metka;
		}
    else //если нет непарафазности
    {
      if(SP_STR[ukaz_strok][4]==11)//если на стрелку приходит непарафазная стрелка
      if((cvet_sp&12)!=12)SP_STR[ukaz_strok][4]=cvet_sp&0xFFDF;//снять толщину для незанятой
      else SP_STR[ukaz_strok][4]=cvet_sp;
      if(fr3[STREL][0]==1&&fr3[STREL][1]==0)//если стрелка в плюсе
      {
        if((SP_STR[ukaz_strok][2]<0xAAAA)&&//если нет стыка по плюсу и
        (SP_STR[ukaz_strok][1]!=0xFFFF))//и есть вход на стрелку
        {
          str_plus=SP_STR[ukaz_strok][2];//взять объект для стрелки по "+" ветви
          SP_STR[str_plus][4]=cvet_strel;//установить цвет для стрелки за "+" ветвью
        }
        if((SP_STR[ukaz_strok][3]<0xAAAA)&&//если нет стыка по минусу
        (SP_STR[ukaz_strok][1]!=0xFFFF))//и есть вход на стрелку
        {
          str_minus=SP_STR[ukaz_strok][3];//взять объект для стрелки по "-" ветви
          SP_STR[str_minus][4]=8;//установить цвет для стрелки за "-" ветвью
        }
        goto b_metka;
      }
      if(fr3[STREL][0]==0&&fr3[STREL][1]==1)//если стрелка в минусе
      {
        if(SP_STR[ukaz_strok][3]<0xAAAA)//если нет стыка по минусу
        {
          str_minus=SP_STR[ukaz_strok][3];//взять объект для стрелки по "-" ветви
          SP_STR[str_minus][4]=cvet_strel;//установить цвет для стрелки за "-" ветвью
        }
        if(SP_STR[ukaz_strok][2]<0xAAAA)//если нет стыка по плюсу
        {
          str_plus=SP_STR[ukaz_strok][2];//взять объект для стрелки по "+" ветви
          SP_STR[str_plus][4]=8;//установить цвет для стрелки за "+" ветвью
        }
        goto b_metka;
			}
      //далее то, что для стрелки без контроля
      SP_STR[ukaz_strok][4]=cvet_sp&0xFFDF;//снять толщину
      cvet_strel=SP_STR[ukaz_strok][4];
      if(SP_STR[ukaz_strok][2]<0xAAAA)//если нет стыка по плюсу
      {
        str_plus=SP_STR[ukaz_strok][2];//взять объект для стрелки по "+" ветви
        SP_STR[str_plus][4]=cvet_strel;//установить цвет для стрелки за "+" ветвью
      }
      if(SP_STR[ukaz_strok][3]<0xAAAA)//если нет стыка по минусу
      {
        str_minus=SP_STR[ukaz_strok][3];//взять объект для стрелки по "-" ветви
        SP_STR[str_minus][4]=cvet_strel;//установить цвет для стрелки за "-" ветвью
      }
      goto b_metka;
    }
  }
  return(-1);
}
/***********************************************************/
//процедура рисования стрелки и СП

void ris_sp_str()
{
	int objekt,ob_dop,pol_dop,cvet,coor[16],ukaz_str=0,i,wid,tip,
	znak_vet,znak_vhod,dx=0,dy=0,dlinax=0,dlinay=0;

	nom_func("288");
	while(SP_STR[ukaz_str][0]!=0xFFFF)//пройти по всем строкам
	{
		objekt=SP_STR[ukaz_str][0];//получить объект
		if(fr3[SP_STR[0][0]][10]==0)fr3[objekt][10]=0;
		cvet=SP_STR[ukaz_str][4];//получить цвет
		if((cvet&32)==32)wid=3;//если толсая
		else wid=0;//если тонкая
		if((cvet&64)==64)tip=1;//если пунктирная
		else tip=0;//если сплошная
		for(i=0;i<16;i++)coor[i]=0;
		if(fr1[objekt][0]==1)for(i=0;i<16;i++)coor[i]=fr2[objekt][i];
		else for(i=0;i<12;i++)coor[i]=fr2[objekt][i];
		setlinestyle(0,1,3);setcolor(7);
		//стереть объект
		if(fr1[objekt][0]==1)//если объект - стрелка
		{
			if(fr1[objekt][9]==0)znak_vet=-1;//если ветвление вниз
			else znak_vet=1;//если ветвление вверх
			if(fr1[objekt][7]==0)znak_vhod=OBRAT;
			else znak_vhod=-1*OBRAT;
		}
		line(coor[0],coor[1],coor[2],coor[3]);

		if(coor[4]>0)line(coor[4],coor[5],coor[6],coor[7]);
		if(coor[8]>0)line(coor[8],coor[9],coor[10],coor[11]);
		if(coor[12]>0)line(coor[12],coor[13],coor[14],coor[15]);
		if(fr1[objekt][0]==3)//если это СП
		{ //установить тип линии
			setlinestyle(tip,1,wid);setcolor(cvet&0xF);

			if((fr3[objekt][10]!=0)&&(cvet==8))
			{
				setcolor(15);
				if((fr3[objekt][10]&0xf)==3)setlinestyle(tip,0,3);
			}
			//нарисовать СП
			line(coor[0],coor[1],coor[2],coor[3]);

			if(coor[4]>0)line(coor[4],coor[5],coor[6],coor[7]);
			if(coor[8]>0)line(coor[8],coor[9],coor[10],coor[11]);
			if(coor[12]>0)line(coor[12],coor[13],coor[14],coor[15]);
		}
		setcolor(7);
		setlinestyle(tip,0,3);
		if((fr1[objekt][0]==1)&&(coor[0]!=0))//если стрелка
		{
			//достереть последний хвостик
			if((coor[0]!=0)&&(coor[12]>0))line(coor[12],coor[13],coor[14],coor[15]);
			if((fr1[objekt][8]&0xfff)>1)//если стрелка сбрасывающая с общим СП
			{
				ob_dop=fr1[objekt][8]&0xFFF;
				pol_dop=(fr1[objekt][8]&0x1000)>>12;
				if(fr3[ob_dop][0]==pol_dop)
				{
					if(cvet!=11)cvet=8;
					wid=0;
					tip=0;
				}
				else
					i=0;
			}
			setcolor(7);
			//подготовка стирания палочек
			if(fr1[objekt][3]==1)//если движение по плюсу на экране - прямо
			{
				dx=2*znak_vhod;
				dy=-3*znak_vet;
				dlinax=-6*znak_vhod;
				dlinay=0;
			}
			else//если на экране по плюсу поворот
			{
				dx=3*znak_vhod;
				dy=0;
				dlinax=-1*znak_vhod;
				dlinay=4*znak_vet;
			}
			//стереть палочку ветви "плюс"
			setlinestyle(0,1,0);
			line(coor[0]+dx,coor[1]+dy,coor[0]+dx+dlinax,coor[1]+dy+dlinay);
			setlinestyle(0,1,3);
			//стереть палочку ветви "минус"
			if(fr1[objekt][3]==1)//если движение по плюсу на экране - прямо
			{
				dx=3*znak_vhod;
				dy=0;
				dlinax=-1*znak_vhod;
				dlinay=4*znak_vet;
			}
			else//если на экране по плюсу поворот
			{
				dx=2*znak_vhod;
				dy=-3*znak_vet;
				dlinax=-6*znak_vhod;
				dlinay=0;
			}
			setlinestyle(0,1,0);
			line(coor[8]+dx,coor[9]+dy,coor[8]+dx+dlinax,coor[9]+dy+dlinay);
			setlinestyle(0,1,3);
			//если стрелка в плюсе
			if((fr3[objekt][0]==1)&&(fr3[objekt][1]==0)&&(fr3[objekt][5]==0))
			//если стрелка в плюсе
			{
				if(cvet>0)setcolor(cvet&0xF);//установить цвет
				setlinestyle(tip,1,wid);//установить линию
				//подготовка изображения минуса
				if(fr1[objekt][3]==1)//если движение по плюсу на экране - прямо
				{
					dx=2*znak_vhod;
					dy=-3*znak_vet;
					dlinax=-6*znak_vhod;
					dlinay=0;
				}
				else//если на экране по плюсу поворот
				{
					dx=3*znak_vhod;
					dy=0;
					dlinax=-1*znak_vhod;
					dlinay=4*znak_vet;
				}
				//если объект в трассе по плюсу
				if(((fr3[objekt][10]&0xf)!=0)&&(cvet==8))setcolor(15);
				//если объект в трассе по минусу
				if((fr3[objekt][10]&0x800f)>0x8000)
				{
					setcolor(8);
					setlinestyle(tip,1,wid);//установить линию
				}
				//нарисовать плюсовую ветвь
				if(fr1[objekt][3]==1)
				{
					if(znak_vhod==1)line(coor[0]-1,coor[1],coor[2],coor[3]);
					if(znak_vhod==-1)line(coor[0]+1,coor[1],coor[2],coor[3]);
				}
				else
				{
					line(coor[0],coor[1],coor[2],coor[3]);
				}
				if(coor[4]!=0)line(coor[4],coor[5],coor[6],coor[7]);
				//нарисовать минусовую ветвь
				setcolor(8);
				if(((fr3[objekt][10]&0x800f)>0x8000)&&(cvet==8))
				{
					setcolor(15);
				}
				if((fr3[objekt][10]&0x8000)==0)
				{
					setcolor(8);
					setlinestyle(0,1,0);
				}
				line(coor[8]+dx,coor[9]+dy,coor[8]+dx+dlinax,coor[9]+dy+dlinay);
				line(coor[8]+dx,coor[9]+dy,coor[10],coor[11]);
				if((coor[0]!=0)&&(coor[12]!=0))line(coor[12],coor[13],coor[14],coor[15]);
			}
			//если стрелка в минусе
			if((fr3[objekt][0]==0)&&(fr3[objekt][1]==1)&&(fr3[objekt][5]==0))
			{
				if(cvet>0)setcolor(cvet&0xF);//установить цвет
				setlinestyle(tip,1,wid);//установить линию
				//подготовка изображения плюса
				if(fr1[objekt][3]==1)//если движение по плюсу на экране - прямо
				{
					dx=3*znak_vhod;
					dy=0;
					dlinax=-1*znak_vhod;
					dlinay=4*znak_vet;
				}
				else//если на экране по плюсу поворот
				{

					dx=2*znak_vhod;
					dy=-3*znak_vet;
					dlinax=-6*znak_vhod;
					dlinay=0;
				}
				//если стрелка по трассе в плюсе
				if((fr3[objekt][10]&0xf)>0)setcolor(8);

				//если стрелка по трассе в минусе
				if(((fr3[objekt][10]&0x800f)>0x8000)&&(cvet==8))setcolor(15);

				//нарисовать минусовую ветвь
				if(znak_vhod==1)line(coor[8],coor[9],coor[8]-1,coor[9]);
				if(znak_vhod==-1)line(coor[8],coor[9],coor[8]+1,coor[9]);
				line(coor[8],coor[9],coor[10],coor[11]);
				if(coor[12]!=0)line(coor[12],coor[13],coor[14],coor[15]);
				setcolor(8);setlinestyle(0,1,0);
				if((fr3[objekt][10]&0x800f)>0x8000)setcolor(8);
				else
				if(((fr3[objekt][10]&0x800f)>0)&&(cvet==8))setcolor(15);
				//нарисовать плюсовую ветвь
				line(coor[0]+dx,coor[1]+dy,coor[0]+dx+dlinax,coor[1]+dy+dlinay);
				line(coor[0]+dx,coor[1]+dy,coor[2],coor[3]);
				if(coor[4]!=0)line(coor[4],coor[5],coor[6],coor[7]);
			}
			//если стрелка без контроля
			if((fr3[objekt][0]==fr3[objekt][1])||(fr3[objekt][5]==1))
			{
				dx=2*znak_vhod;
				dy=3*znak_vet;
				dlinax=6*znak_vhod;
				dlinay=0;
				//стереть палочку ветви "минус"
				setlinestyle(0,1,0);
				line(coor[8]+dx,coor[9]+dy,coor[8]+dx+dlinax,coor[9]+dy+dlinay);
				//подготовка стирания ветви плюс
				dx=3*znak_vhod;
				dy=0;
				dlinax=-1*znak_vhod;
				dlinay=4*znak_vet;
				//стереть палочку ветви "плюс"
				setlinestyle(0,1,0);
				line(coor[0]+dx,coor[1]+dy,coor[0]+dx+dlinax,coor[1]+dy+dlinay);
				setcolor(cvet&0xF);//установить цвет
				wid=1;
				if((cvet&0xf)!=12)setcolor(8);
				setlinestyle(tip,1,wid);//установить линию
				//нарисовать плюс и минус
				//изображение плюсовой ветви
				//если стрелка по трассе в плюсе
				if((fr3[objekt][10]&0x8000)==0)
				{
					if(((fr3[objekt][10]&0xf)>0)&&(cvet==8))setcolor(15);
					else
					{
						setcolor(cvet&0xf);
						setlinestyle(0,1,wid);
					}
				}
				line(coor[0],coor[1],coor[2],coor[3]);
				if(coor[4]!=0)line(coor[4],coor[5],coor[6],coor[7]);

				//изображение минусовой ветви
				//если стрелка по трассе в минусе
				if((fr3[objekt][10]&0x8000)>0)
				{
					if(((fr3[objekt][10]&0x800f)>0x8000)&&(cvet==8))setcolor(15);
				}
				else setcolor(cvet&0xf);
				line(coor[8],coor[9],coor[10],coor[11]);
				if(coor[12]!=0)line(coor[12],coor[13],coor[14],coor[15]);
			}
			nach_zakr(objekt,0);;
		}
		ukaz_str++;
	}
	return;
}
//---------------------------------------------------------
void ris_trassa(int tipus)
{
	int ind;
	unsigned int obekt;

	nom_func("289");
	for(ind=0;ind<100;ind++)
	{
		if(tipus==1)obekt=TRASSA[ind].object;
		if(tipus==0)obekt=TRASSA0[ind].object;
		if(tipus==2)obekt=TRASSA_RIS[ind].object;
		if(obekt==0xFFFF)break;
		if(fr1[obekt&0xfff][0]==1)
		displaystrelka(obekt&0xFFF,0,0);
		if(fr1[obekt&0xfff][0]==3)sekci(obekt&0xfff,9999);
		if(fr1[obekt&0xfff][0]==4)displaypathuch(obekt&0xfff);
		if(fr1[obekt&0xfff][0]==5)
		displaypath(obekt&0xfff,0);
	}
	return;
}
//--------------------------------------------------------------------
/********************************************\
* Процедура отображения на экране путей для  *
* изменения состояния бита bot               *
\********************************************/
void displaypath(int oik,int bot)
{
	int x1,y1,x2,y2,x3,y3,y4,x4,xl,xr,xc,yc,nom,R,cvet,
	bit_npo_chpo,cod,napr,ob_km,bit_km;
	char cod_ni_chi=0;

	nom_func("39");

	if((klo==1)||(help!=0)||(nikuda==1))return;
  if(bot==5)
	if(fr3[oik][5]==1)slom_interf(oik);// если выявлена непарафазность

  //если бит ложной занятости
	if((bot==4)&&(TELEUP==1))
	{ //если информация о КМ не в информации о пути
		if((fr3[oik][4]==1)&&((fr1[oik][11]&0xfff)!=oik))
		{
			if(tst_fix(oik,4))slom_uch(oik,'З');// если получена ложная занятость
			fix(oik,4);
		}
	}

	//если бит ложной свободности
	if((bot==3)&&(TELEUP==1)&&((fr1[oik][11]&0xfff)!=oik))
	{ // если изменения в бите ложной свободности
	 if((fr3[oik][3]==1)&&(TEST_N==0))
	 {
		if(tst_fix(oik,3))slom_uch(oik,'С');//если игнорировать тест пути
		fix(oik,3);
	 }
	}
	//если изменения не в бите непарафазности сброс таймера
	if(bot!=5)pooo[oik]=0L;
	setlinestyle(0,0,3);// установить толстую линию
	//взять координаты
	x1=fr2[oik][0];y1=fr2[oik][1];x2=fr2[oik][2];y2=fr2[oik][3];
	x3=fr2[oik][4];y3=fr2[oik][5];x4=fr2[oik][6];y4=fr2[oik][7];
	setcolor(cvv);// установить цвет фона
	line(x1,y1,x2,y2);// стереть путь
	if(x3!=0) line(x3,y3,x4,y4);
	if(fr1[oik][7]<8888)// если это участок приближения автоблокировки
	{
		nom=fr1[oik][7];// взять номер для участка приближения
		uch_udal(nom,oik,0);// нарисовать участок приближения
	}

	//если участок в исходном
	if((fr3[oik][0]==0)&&(fr3[oik][1]==0)&&( fr3[oik][2]==0))
	{
		setlinestyle(0,0,0);//тонкая линия
		if((fr3[oik][10]&0xf)!=0)
		{
			setcolor(15);
			if((fr3[oik][10]&0xf)==3)
			{
				if(cvv==8)setlinestyle(0,0,3);
				else setlinestyle(0,0,0);
			}
		}
		else
		{
			setcolor(8); //черный цвет
			setlinestyle(0,0,1);
		}
	}
	else
		//свободен замкнут=желтый
		if((fr3[oik][0]==0)&&(fr3[oik][1]==1)) setcolor(14);
		else
			//свободен в разделке
			if((fr3[oik][0]==0)&&(fr3[oik][2]==1)) setcolor(14);
			else
				//занят и замкнут
				if((fr3[oik][0]==1)&&(fr3[oik][1]==1)) setcolor(LIGHTRED);
				else
					//занят в разделке
					if((fr3[oik][0]==1)&&(fr3[oik][2]==1)) setcolor(LIGHTRED);
					else
						// просто занят
						if(fr3[oik][0]==1) setcolor(LIGHTRED);
	if(fr3[oik][0]==1)//если занялся
	{
#ifdef POLUAVTOMAT //если есть полуавтоматика
		if((fr1[oik][7]>1)&&(fr1[oik][7]!=9999))// если участок удаления
    {
			if(fr1[oik][7]==8888)// если это участок приближения полуавтоматики
      {
        nom=fr1[oik][9];//датчик занятия перегона по приему НПО/ЧПО
        bit_npo_chpo=fr1[oik][10];//бит НПО/ЧПО
        znach=fr1[oik][11];//значения НПО/ЧПО бита для приема
        napr=fr1[oik][12];//направления для приема
        cod=fr3[nom][bit_npo_chpo]*2+fr3[oik][0];//совокупный код
        if(fr3[nom][bit_npo_chpo]==znach)//если перегон в заданном состоянии
          if(fr3[oik][0]==1)//если участок приближения занят
            if(cod!=COD_UCH_UDAL[napr]) //если было изменение кода
            {
              zvuk_vkl(2,18);//включить двухтональный сигнал на 1 сек
              COD_UCH_UDAL[napr]=cod;
            }
      }
    }
#endif
    setcolor(LIGHTRED);setlinestyle(0,0,3);//толстая красная линия
  }
	if(fr3[oik][0]==0)//если освободился
  {
    if((fr1[oik][7]>1)&&(fr1[oik][7]!=9999))// если участок удаления
    {
      if(fr1[oik][7]==8888)// если это участок приближения полуавтоматики
      {
        nom=fr1[oik][9];//датчик занятия перегона по приему НПО/ЧПО
        bit_npo_chpo=fr1[oik][10];//бит НПО/ЧПО
        napr=fr1[oik][12];//направления для приема
        cod=fr3[nom][bit_npo_chpo]*2+fr3[oik][0];//совокупный код
        COD_UCH_UDAL[napr]=cod;
      }
    }
  }
	if(fr3[oik][5]!=0)// если непарафазность
	{
    setcolor(LIGHTCYAN);
    setlinestyle(0,0,0);
  }
  if(fr1[oik][8]==1010)// если закрываемый путь
		if((fr1[oik][10]==9999)||(fr1[oik][7]==100))//если могут быть маршруты на путь
    line(x1,y1,x2,y2);// нарисовать путь
  if(x3!=0) line(x3,y3,x4,y4);
  //settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
  if((fr1[oik][9]!=9999)&&(fr1[oik][7]!=100)&&(fr1[oik][7]!=150))
  {
    R=fr1[oik][9];   // работа с элементом управления - шильдиком
    xc=markery[R][4];
    yc=markery[R][5];
  }
  if((fr4[oik][2]==1)&&(fr1[oik][9]!=9999)&&
  (fr1[oik][7]==9999))
  // если путь закрыт для движения
	{ setcolor(LIGHTRED);outtextxy(xc,yc,pako[oik]);}
  if((fr4[oik][2]==0)&&(fr1[oik][9]!=9999)&&
  (fr1[oik][7]==9999))
  // если путь открыт для движения
	{ if((i3==fr1[oik][9])||(modi==fr1[oik][9]))setcolor(15);//если маркер на объекте

    else   setcolor(8);
    outtextxy(xc,yc,pako[oik]);
  }
	setlinestyle(0,0,0);

	//если для пути есть четный КМ
	if((fr1[oik][11]!=9999)&&(fr1[oik][11]>1))
	{
		ob_km=fr1[oik][11]&0xFFF; //получить объект КМ
		bit_km=(fr1[oik][11]&0xF000)>>12; //получить бит КМ
		if(ob_km!=oik) KM_put(ob_km,bit_km,oik); //если КМ не в сообщении пути
		else//если КМ в сообщении пути
		{
			setlinestyle(0,0,0);
			if(fr3[ob_km][bit_km]==1)cvet=YELLOW;  //для замкнутого - желтый цвет
			else cvet=cvv;//иначе фон

			if(fr3[ob_km][5]==1)cvet=11;
			setcolor(cvet);
			xc=(fr2[oik][0]+fr2[oik][2])/2;
			yc=fr2[oik][1]+3;
			line(xc-3,yc,xc+3,yc);
		}
	}
	//если для пути есть нечетный КМ\
	if((fr1[oik][12]!=9999)&&(fr1[oik][12]>1))
	{
		ob_km=fr1[oik][12]&0xFFF;
		bit_km=(fr1[oik][12]&0xF000)>>12;
		if(ob_km!=oik)KM_put(ob_km,bit_km,oik);
	}

	if((fr1[oik][8]==1010)||(fr1[oik][8]==1011))
  { if(fr2[oik][0]<fr2[oik][2])
    { xl=fr2[oik][0]; //координата левого края пути
      xr=fr2[oik][2];
    }
    else
    {
			xl=fr2[oik][2];
			xr=fr2[oik][0];
		}
		yc=fr2[oik][1];
#if OBRAT==1   // если ЧЕТНЫЙ прием слева на экране
		cod_ni_chi=4;
#else
		cod_ni_chi=0;  //если ЧЕТНЫЙ прием справа на экране
#endif
		if(fr3[oik][1]==1)cod_ni_chi=cod_ni_chi|2;// если НИ под током
		else cod_ni_chi=cod_ni_chi&5;

		if(fr3[oik][2]==1)cod_ni_chi=cod_ni_chi|1;// ЧИ под током
		else cod_ni_chi=cod_ni_chi&6;
		if(fr3[oik][5]==1)cod_ni_chi=8;
		else
		switch(cod_ni_chi)
		{ //"ни" справа,или слева
			//"ни" без тока,"чи" без тока, парафазность в норме
			case 0:
			case 4: setcolor(7);line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
													line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
                          break;
      //"ни" справа,"ни" без тока,"чи" под током, парафазность в норме
      case 1:setcolor(7); line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
             if(fr3[ob_km][bit_km]!=1)setcolor(10);
             else setcolor(14);
             line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
                          break;
      //"ни" справа,"ни" под током,"чи" без тока, парафазность в норме
      case 2:if(fr3[ob_km][bit_km]!=1)setcolor(10);
             else setcolor(14);
             line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
             setcolor(7); line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
                          break;
      //"ни" справа,"ни" под током,"чи" под током, парафазность в норме
      case 3:
      case 7:
             if(fr3[ob_km][bit_km]!=1)setcolor(10);
             else setcolor(14);
             line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
             line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
             break;
      //"ни" слева,"ни" без тока,"чи" под током, парафазность в норме
      case 5: if(fr3[ob_km][bit_km]!=1)setcolor(10);
              else setcolor(14);
              line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
              setcolor(7); line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
                           break;
      //"ни" слева,"ни" под током,"чи" без тока, парафазность в норме
      case 6:setcolor(7);line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
             if(fr3[ob_km][bit_km]!=1)setcolor(10);
             else setcolor(14);
             line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
						 break;
      case 8:
			default: setcolor(11);line(xr-5,yc-1,xr-10,yc-3);line(xr-5,yc+1,xr-10,yc+3);
													 line(xl+5,yc-1,xl+10,yc-3);line(xl+5,yc+1,xl+10,yc+3);
													 break;
		}
	}
#ifdef kol_STY
	Styki();
#endif
	return;
}
//------------------------------------------------------------
/************************************************\
*	Процедура отображения участка пути УП          *
\************************************************/
void displaypathuch(int Kp)
{
	int f,r0,r1,r2;
	int Wide,Colo,Sty;

	nom_func("40");

	if((nikuda==1)||(klo==1)||(help!=0)) return;
	if(TELEUP==1)
	{
		if(fr3[Kp][4]==1) //если бит ложной занятости
		{
			if(tst_fix(Kp,4))slom_uch(Kp,'З');// если получена ложная занятость
			fix(Kp,4);
		}
		if(fr3[Kp][3]==1)//если получена ложная свободность
		{
			if(tst_fix(Kp,3))slom_uch(Kp,'C');
			fix(Kp,3);
		}
	}
	setlinestyle(0,0,3);setcolor(cvv);
	line(fr2[Kp][0],fr2[Kp][1],fr2[Kp][2],fr2[Kp][3]);
	if(fr2[Kp][4]!=0)line(fr2[Kp][4],fr2[Kp][5],fr2[Kp][6],fr2[Kp][7]);
  if(fr2[Kp][8]!=0)line(fr2[Kp][8],fr2[Kp][9],fr2[Kp][10],fr2[Kp][11]);
  r0=fr3[Kp][0];
	r1=fr3[Kp][1];
	r2=fr3[Kp][2];
  r1=r1*2;
	r2=r2*4;
	f=r0+r1+r2;//код состояния участка пути
  switch(f)
  {
    case 0: if(fr1[Kp][11]==9999){Wide=0;Sty=0;Colo=8;}
            else
             if(fr3[fr1[Kp][11]][fr1[Kp][12]]==1)
              {Wide=3;Sty=0;Colo=MAGENTA;}
              else{Wide=0;Sty=0;Colo=8;}
						break;
    case 1: Colo=12;Wide=0;Sty=0;break;
    case 2: Colo=14;Wide=3;Sty=0;break;
    case 3: Colo=LIGHTRED;Wide=3;Sty=0;break;
    case 4: Colo=8;Wide=3;Sty=1;break;
    case 5: Colo=12;Wide=3;Sty=1;break;
    case 6: Colo=14;Wide=3;Sty=1;break;
    case 7: Colo=12;Wide=3;Sty=1;break;
  }
  if(Colo==8)//если разомкнут УП
  {
		if((fr3[Kp][10]&0xf)!=0)
    {Colo=15;Wide=0;Sty=0;} fr3[Kp][12]=0;
  }
	else
	{
		if(fr3[Kp][10]!=0)
		tst_tst++;
		if(tst_tst>10)
		tst_tst=0;
		fr3[Kp][10]=0;
	}
  if(fr3[Kp][5]==1){Colo=11;Wide=0;Sty=0;}
fin:
  setcolor(Colo);setlinestyle(Sty,0,Wide);
  if(fr1[Kp][0]==4)
  { line(fr2[Kp][0],fr2[Kp][1],fr2[Kp][2],fr2[Kp][3]);
    if(fr2[Kp][4]!=0)line(fr2[Kp][4],fr2[Kp][5],fr2[Kp][6],fr2[Kp][7]);
    if(fr2[Kp][8]!=0)line(fr2[Kp][8],fr2[Kp][9],fr2[Kp][10],fr2[Kp][11]);
    if(fr1[Kp][9]!=9999)
    {
      R=fr1[Kp][9];   // работа с элементом управления - шильдиком
      xc=markery[R][4];
      yc=markery[R][5];
      if(fr4[Kp][2]==1)// если УП закрыт для движения
      {
        setcolor(LIGHTRED);
        outtextxy(xc,yc,pako[Kp]);
      }
      if(fr4[Kp][2]==0)// если открыт для движения
      {
				if((i3==fr1[Kp][9])|(modi==fr1[Kp][9]))setcolor(15);//если маркер на объекте
				else   setcolor(8);
				outtextxy(xc,yc,pako[Kp]);
			}
		}
		return;
	}
	return;
}
//-----------------------------------------------------------------
/**************************************/
void sekos(int nom,int bt_)
{

	nom_func("297");

// если этот объект - МСПД
  if((fr1[nom][1]==19)||(fr1[nom][1]==199))
  {
    sekci(fr1[nom][4+bt_],9999);
    if(fr1[nom][8+bt_]!=9999)sekci(fr1[nom][8+bt_],9999);
  }
#ifdef AVTOVOZVRAT
  prov_av_str(0,0,nom,bt_,4);
#endif
  return;
}
/***********************************\
* процедура изображения и анализа   *
*       состояния секции            *
*       sekci(int koh,int bot)      *
* koh - объект описания секции в БД *
* bot - анализируемый бит данных    *
\***********************************/
void sekci(int koh,int bot)
{
  int i_str,strel,ui,xl,yl,str,sig,bt_zam,p,kod,para,
	stl,stp,spl,spp,ijk;

	nom_func("296");
	if(fr1[koh][1]==27){uksps(koh,bot);return;}//если управление УКСПС в РИ
	if(bot==9999){goto opa;}//не указан конкретный бит-стереть СП
	if((fr1[koh][1]==301)||(fr1[koh][1]==302))//если это кнопка ГСП или ИВ
	{
		krasit_knopki(koh,bot);
		if(prorisovka==0)
#ifdef GRI  //если кнопка ГРИ управляется с АРМа
		//если нажата кнопка и
		if((fr3[koh][2]==1)&&(fr3[fr1[koh][11]][fr1[koh][12]]==0))
    neisprav_na_disk('$',pako[markery[GRI][6]]);
#endif
    return;
  }
  if((fr1[koh][1]>=200)&&(fr1[koh][1]<=300))//если это кнопки разделки
  {
    krasit_razd(koh,bot);
    return;
  }

	if((fr1[koh][1]<5)&&(fr1[koh][2]<5))
	{
		if((bot==5)&&(fr3[koh][5]==1)&&tst_fix(koh,5))
		{
			slom_interf(koh);//если непарафазность
			fix(koh,5);
		}
		if(fr1[koh][11]!=koh)//если МСП в другом сообщении
		{
			//если бит ложной занятости
			if(bot==4)
			{
				for(ijk=0;ijk<10;ijk++)if(FILTR[ijk]==koh)break;
				if(ijk==10)
				{
					if(tst_fix(koh,4)&&(fr3[koh][4]==1))
					{
						slom_uch(koh,'З');
					}
				}
				fix(koh,4);
				return;
			}
			if(bot==3)//если бит ложной свободности
			{
				for(ijk=0;ijk<10;ijk++)if(FILTR[ijk]==koh)break;
				if(ijk==10)
				{
					if(tst_fix(koh,3)&&(fr3[koh][3]==1))
					{
						slom_uch(koh,'C');
					}
				}
				fix(koh,3);
				return;
			}
		}
		else
		{
			if((bot!=5)&&(prorisovka==0))pooo[koh]=0L;//если не бит парафазности - сбросить таймер
		}
opa:
		if((klo==1)||(help!=0)||(nikuda==1))return;
		if((bot==1)&&(ZAGRUZKA==0))//если бит замыкания
		{
			if((fr3[koh][1]==0)&&(fr3[koh][12]>64))fr3[koh][12]=fr3[koh][12]&0x0ff0;//если разомкнулась

			if(fr3[koh][1]==1)fr3[koh][12]=fr3[koh][12]|0x8000;

			for(ui=7;ui<=11;ui++)//пройти по всем сигналам
			{
				if(ui<10)sig=fr1[koh][ui];
				if((ui==10)&&(fr1[koh][ui]==9999))break;
				if(ui==10)sig=fr1[koh][10]&0xff;
				if(ui==11)sig=(fr1[koh][10]&0xff00)>>8;
				if((sig==9999)||(sig==0xff))break;
				if(fr2[sig][3]==0)//если для сигнала нет стрелок в пути
				{
					w(44,sig," 2");
					break;
				}
				for(i_str=3;i_str<9;i_str++)
				{
					if(fr2[sig][i_str]==0)break;
					str=fr2[sig][i_str]&0xfff;//получить номер для стрелки
					p=(fr2[sig][i_str]&0xf000)>>12;//получить тип замыкания
					kod=fr3[sig][0]*8+fr3[sig][2]*2;//проверить маневровые
					kod=kod+fr3[sig][1]*4+fr3[sig][3];//добавить поездные
					switch(p)//переключение по типу замыкания
					{
						//четное поездное
            case 2: if(fr1[sig][1]==0)//если сигнал четный
										{
											//если поездной сигнал открыт
											if((kod>11)||((kod>3)&&(kod<8))||(kod==1)||(kod==3))
                      {
                        if(fr3[koh][1]==1)//если СП замкнута
                        fr4[str][0]=fr4[str][0]|8;
                      }
                      else//если поездной сигнал закрыт
                      {
                        if(fr3[koh][1]==0)//если СП разомкнута
												fr4[str][0]=fr4[str][0]&0xfff7;
												if((kod<12)&&(kod>7))fr4[str][0]=fr4[str][0]&0xfff7;
                      }
                    }
                    break;
            //четное любое
            case 3: if(fr1[sig][1]==0)//если сигнал четный
                    {
                      if(kod!=0)//если какой-либо сигнал открыт
                      {
                        if(fr3[koh][1]==1)//если СП замкнута
                        fr4[str][0]=fr4[str][0]|8;
                      }
                      else //если все сигналы закрыты
                      {
                        if(fr3[koh][1]==0)//если СП разомкнута
												fr4[str][0]=fr4[str][0]&0xfff7;
											}
										}
										break;
						//нечетное поездное
						case 4: if(fr1[sig][1]==1)//если сигнал нечетный
										{
											if((kod>11)||((kod>3)&&(kod<8))||(kod==1)||(kod==3))
											{
												if(fr3[koh][1]==1)//если СП замкнута
												fr4[str][0]=fr4[str][0]|0x10;
											}
											else //если поездной сигнал закрыт
											{
												if(fr3[koh][1]==0)//если СП разомкнута
												fr4[str][0]=fr4[str][0]&0xffef;
												if((kod<12)&&(kod>7))fr4[str][0]=fr4[str][0]&0xffef;
											}
										}
										break;
						//нечетное любое
						case 5: if(fr1[sig][1]==1)//если сигнал нечетный
										{
											if(kod!=0)//если какой-либо сигнал открыт
                      {
                        if(fr3[koh][1]==1)//если СП замкнута
                        fr4[str][0]=fr4[str][0]|0x10;
                      }
                      else //если все сигналы закрыты
                      {
                        if(fr3[koh][1]==0)//если СП разомкнута
                        fr4[str][0]=fr4[str][0]&0xffef;
                      }
                    }
                    break;
            //четное - нечетное поездные
						case 6:
											if((kod>11)||((kod>3)&&(kod<8))||(kod==1)||(kod==3))
                      {
                        if(fr3[koh][1]==1)//если СП замкнута
                        fr4[str][0]=fr4[str][0]|0x18;
                      }
                      else //если поездной сигнал закрыт
                      {
												if(fr3[koh][1]==0)//если СП разомкнута
												fr4[str][0]=fr4[str][0]&0xffe7;
												if((kod<12)&&(kod>7))fr4[str][0]=fr4[str][0]&0xffe7;
                      }
                      break;
            //четное - нечетное любое
            case 7:   if(kod!=0)//если любой сигнал открыт
                      {
                        if(fr3[koh][1]==1)//если СП замкнута
                        fr4[str][0]=fr4[str][0]|0x18;
                      }
                      else //если сигналы закрыты
                      {
                        if(fr3[koh][1]==0)//если СП разомкнута
                        fr4[str][0]=fr4[str][0]&0xffe7;
                      }
                      break;
            default: w(44,koh," 3");break;
          }
          nach_zakr(str,0);
          if(fr1[str][12]!=9999)//если стрелка спаренная
          {
            TEST_SPAR=2;
						para=poisk_ras(fr1[str][2],str,0,0);
            fr4[para][0]=fr4[str][0];
            nach_zakr(para,0);
          }
        }
      }
    }
#ifdef AVTOVOZVRAT
    if((bot==1)||(bot==9999))//если бит занятости участка
		prov_av_str(0,koh,0,0,2);//работа с автовозвратом стрелок
#endif
    for(i_str=5;i_str<7;i_str++)
    {
			if(fr1[koh][i_str]!=9999)//если есть охранная стрелка для СП
      {
        str=fr1[koh][i_str]&0xfff;//получить ходовую стрелку
        p=(fr1[koh][i_str]&0x3000)>>12;//положение ходовой для замыкания
        for(ui=0;ui<2;ui++)
        {

          strel=fr2[koh][2*i_str+2+ui]&0xfff;//получить охранную
          if(strel==0)break;
          bt_zam=(fr2[koh][2*i_str+2+ui]&0xf000)>>8;//получить бит замыкания
					if(fr3[koh][1]==1)//если СП замкнулось
					{ fr3[koh][10]=0;
						if(((p==0)&&(fr3[str][0]==1)&&(fr3[str][1]==0))//если нужный плюс
            ||((p==1)&&(fr3[str][0]==0)&&(fr3[str][1]==1)))//или нужный минус
            {
              fr4[strel][0]=fr4[strel][0]|bt_zam;//замкнуть охранную стрелку
              nach_zakr(strel,0);
              if(fr1[strel][12]!=9999)//если стрелка спаренная
              {
                TEST_SPAR=2;
								para=poisk_ras(fr1[strel][2],strel,0,0);
                fr4[para][0]=fr4[strel][0];
                nach_zakr(para,0);
              }
            }
            else
            {
              fr4[strel][0]=fr4[strel][0]&(~bt_zam);//разомкнуть охранную стрелку
              nach_zakr(strel,0);
              if(fr1[strel][12]!=9999)//если стрелка спаренная
              {
                TEST_SPAR=2;
								para=poisk_ras(fr1[strel][2],strel,0,0);
                fr4[para][0]=fr4[strel][0];
                nach_zakr(para,0);
							}
            }
          }
          if(fr3[koh][1]==0)//если СП разамкнулось
          {
            fr4[strel][0]=fr4[strel][0]&(~bt_zam);//замкнуть охранную стрелку
            nach_zakr(strel,0);
            if(fr1[strel][12]!=9999)//если стрелка спаренная
            {
							TEST_SPAR=2;
							para=poisk_ras(fr1[strel][2],strel,0,0);
              fr4[para][0]=fr4[strel][0];
              nach_zakr(para,0);
            }
          }
        }
			}
		}
		zap_matr(koh);
		cvet_matr();
		ris_sp_str();
		if((fr1[koh][4]&1)==1)//если крестовая
		{ stl=(fr1[koh][3]&0xff00)>>8;//левая стрелка
			stp=fr1[koh][3]&0xff;//правая
			spl=(fr1[stl][6]&0xff00)>>8;// сп левый
			spp=(fr1[stp][6]&0xff00)>>8;// сп правый
			if(spl==koh)spl=spp;
			{
				zap_matr(spl);
				cvet_matr();
				ris_sp_str();
			}
		}
		setcolor(8);

		for(i_str=1;i_str<10;i_str++)//пройти по всем стрелкам
		{
			if(SP_STR[i_str][0]==0xFFFF)break;//если нет стрелки
			strel=SP_STR[i_str][0];
			if(fr1[strel][10]==33)//если охранная стрелка плюсовая
				if(((fr3[strel][0]!=1)||(fr3[strel][1]!=0))&&( fr3[koh][1]!=1))setcolor(12);
			if(fr1[strel][10]==44)//если охранная стрелка минусовая
				if((fr3[strel][0]!=0)||(fr3[strel][1]!=1)&&(fr3[koh][1]!=1))setcolor(12);
			if((fr1[strel][10]==33)||(fr1[strel][10]==44))
			{
				if(fr1[strel][9]==0)yl=fr2[strel][1]+18;
				if(fr1[strel][9]==1)yl=fr2[strel][1]-18;
				xl=fr2[strel][0];
				for(ui=4;ui>0;ui--) circle(xl,yl,ui);
			}
		}
	}
final:
	return;
}
//----------------------------------------------------------------------
/******************************************************\
* Процедура вывода на экран меню для выбора оператором *
* состояния перегона при загрузке системы и отсутствии *
* ключа жезла. Позволяет отличить неисправность от на- *
* личия на перегоне хозяйственного поезда.             *
\******************************************************/
void menu_hoz_poezd(int npr)
{
	nom_func("170");

	setlinestyle(0,0,0);setcolor(8);rectangle(400,0,638,28);
	if(npr==0)
	{ setfillstyle(1,10);
		bar(402,1,637,12);moveto(402,4);setcolor(12);outtext(" ОТПР.ЧЕТН.ХОЗПОЕЗД    ");
		ukazka();
		setfillstyle(1,2);
		bar(402,14,637,27);moveto(402,16);setcolor(15);outtext(" НЕ БЫЛО ЧЕТН.ХОЗПОЕЗДА");
  }
  if(npr==1)
  { setfillstyle(1,10);
    bar(402,1,637,12);moveto(402,2);setcolor(12);outtext(" ОТПР.НЕЧЕТН.ХОЗПОЕЗД    ");
    ukazka();
    setfillstyle(1,2);setcolor(15);
    bar(402,14,637,27);moveto(402,16);outtext(" НЕ БЫЛО НЕЧЕТН.ХОЗПОЕЗДА");
  }
  me_uk=1;
  return;
}
//-------------------------------------------------------------------
