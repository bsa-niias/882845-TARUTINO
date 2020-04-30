#include "opred.h"
#include "bla.h"
//--------------------------------------
unsigned char ARROW[800];
unsigned long int BAZ_ADR1,
BAZ_ADR11,
BAZ_ADR2,
BAZ_ADR3,
BAZ_ADR4,
BAZ_ADR5,
BAZ_ADR6,
koef_uskor,
tormoz;
long FIR_time,
first_time,
VIRT_TIME_0,   //виртуальное время начала отсчета времени архива
VIRT_TIME_1,   //виртуальное время текущего отсчета времени архива
pooo[kol_VO],  //таймер событий
pooo_com[kol_VO], //таймер ожидания выполения команды
SEC_time,
second_time,
t_inf[skoko_stoek],//фиксатор времени выдачи информации в ДЦ
t_com[skoko_stoek],//фиксатор времени выдачи команды в стойку
TIME_OUT_PVM,
tii,
tiiq;
//----------------------------------------
struct dosdate_t dat;
struct so_so soob;
struct zvuk zvuk_sig={0l,0,0};
//-----------------------------------------------
int ANALIZ[skoko_stoek];
int ANALIZ_DC;
#ifdef KOL_SOO2
int old_aoar[skoko_stoek]={9999,9999};
#else
int old_aoar[skoko_stoek]={9999};
#endif
int AV_zdem;
int avap=0;
int AVARI=0;
int AVARIIN=0;  //признак выполнения автовозврата стрелки
int b_h1;
int b_m1;
int b_s1;
int BAIT_PRER=0;//номер байта,на котором прервана передача
int BAM=0;
int basha=0;
int bata=0;
int bata;
int BAZA;//хендл для файла контроля базы данных
int BEDA[10];
int beg[2]={-1,-1};
int bi=0;
int bil_bil;
int bit_pso=0;
#ifdef SPDLP
int bt_spdlp[N_BIT_SCB];
int byte_spdlp[N_BIT_SCB];
#endif
int cha=0;
int chas;    //число часов найденное для точки входа в архив
int chita=0;
int cikl_obnov;
int cikl_avto;
int COD_OGR;
int COD_OTPR[2];
int COD_PRIEM[2];
int COD_PS_NEW=0;
int COD_PS_OLD=0;
int COD_UCH_UDAL[2];
int CVETCVETOF=0;
int CVTF=4;
int cvv=7;
int danet;
int dayy_;
int DISK;
int down;
int DU=0;
int dv;
int DVA_MO=0;
int ELE=0;
int END;
int END_OPER;
int end_marsh;//конец текущего маршрута
int end_marsh_oper;//конец маршрута, набранный оператором
int end_avto;//конец текущего маршрута автодействия
int End_vara=0;
int FFFlag=1;
int f_ob1;
int fi_co1;
int fiki=0;
int file_arc;
int file_soob;//хендл для файла текстов сообщений
int fix_ts_sos[skoko_stoek];//индикатор фиксации состяния ТС соседа
int fix_tu_sos[skoko_stoek];//индикатор фиксации состояния ТУ соседа
int fix_rek_sos[skoko_stoek];//индикатор фиксации реконфиг. каналов соседа
int Fla_no_pp=0;
int flag_paro=0;
int FLAG_PO=0;
int flagoutmsg;
int flms=0;
int GraphDriver=VGA;
int GraphMode;
int gri=0;
int GUAV;
int half_marsh=0;
int half_marsh1;
int having;
int help;
int ho_ur;
int hod;
int hoz_poezd[2]={0,0};
int hudo=0;
int hudoo=0;
int i_s;
int i3;
int iqw;
int k;
int kadr[12][50];
int klaval;
int klo;
int ko;
int KOL_VYD_MARSH[skoko_stoek];
int konec[2];
int konec_dc;
int kot=0;
int KSK[skoko_stoek]={KS1};
int last_day=0;
int left_up_left[14];
int left_up_right[14];
int maka=0;
int maket;
int Makst=0;
int mane;
int mane1;
int manevr;
int manevr1;
int manevro;
int manevro1;
int MAR_GOT[skoko_stoek];
int marazm=0;
int marshrut[skoko_stoek][3];
int me_uk=0;
int menu_N;
int mi_n;
int min;  //число минут, найденное для точки входа в архив
int misto=1;
int mlc[16];
int mls[16];
int mlu[15];
int mlv[15];
int mly[16];
int mlz[16];
int MODE_KN=0;
int modi=-1;
int modi_new;
int monn_;
int mouse_down;
int MOUSE_ON;//признак обнаружения мыши при загрузке
int mouser;
int n;
int n_1;
int n_2;
int n_m=0;
int n_ob1=0;
int n_ob2=0;
int N_OGR=0;
int n_s=0;
int na=0;
int nach_marsh;
int nach0_marsh;
int nach_marsh_oper;
int nach0_marsh_oper;
int nach_avto;//начало текущего маршрута автодействия
int N_SOOB; //счетчик числа прочитанных событий из архива
int NE;
int NEISPRAVEN=1;
int net_kontro=0;
int nikuda;
int NM;
int NOM_NEXT=0;
int nomer;
int NOMER_ARMA=3;
int NOMER_FOR_FR4;
int number=5555;
int new_day=0;//флаг разрешения/запрета формирования нового файла
int OB_PRER=0;//объект,на котором прервана передача
int OB_NEW=0;
int OB_NEKVIT=0;
int obnov;
int okno_otkaz;
int old_me=0;
int old_modi;
int old_punkt;
int oper;
int oppp;
int opred;
int oshibka;
int osya=0;
int ot=0;
int otkaz_sos[skoko_stoek];   //флаг индикации отказа ТС соседней ПЭВМ
int otkaz_tu_sos[skoko_stoek];//флаг индикации отказа ТУ соседней ПЭВМ
unsigned int otl_soob;        //хранитель отложенного сообщения
int Otlog=0;
int OTV_KOM_0=0;
int OTV_KOM_1=0;
int otv_kom;
int otv_kom_oper;
int pat;
int pauza;     //пауза для торможения процедуры перерисовки экрана
int perekluch; //признак переключения экранов
int perezap;
int plo;
int plu=0;
int po_ma;
int podgr=0;
int podsvetka;
int point;
int pointer;
int poka;
int povt1;
int povt1;
int povt2;
int PRESS;
int priem_;
int PRIEM_FR4;
int PRKL=0;    //признак выполнения переключения комплектов
int prohod_1;
int prorisovka;
int PROV_SCB;
int prv[skoko_stoek];
int punkt_N;//пункт меню, на котором находится курсор
int pust_pvm=0;
unsigned int pusto;
int puti;
int qf;//хэндл для работы с журналом состояния УВК
int R;
int r;
int right_up_left[14];
int right_up_right[14];
int s1;
int s2;
int sbros;
int sbros1;
int schet=0;
int ScLk;//Индикатор состояния лампочки Scroll Lock на клавиатуре
int se_c;
int se_co1;
int sek; //число секунд,найденное для точки входа в архив
int SEZON=0;
int siv=0;
int soob_for_oper;
int speed_spdlp;
int spdlp;
int spa=0;
int STAT;
int STATUS_0=0;
int STATUS_1=0;
int STATUS=2;
int STACK;
int STOP=0;//признак остановки кадра для отладки каналов
int STOP_TUMS[2]={0,0};
int STOROG=0;
int str_i=20;
int strelki[5][3];
int t_koma[2];
int t_otpusk;
int t_pust[skoko_stoek];
int t_pust_dc;
int t_t=0;//флаг требования начала полной 10-минутной записи данных в архив
int test_tmp;
int TELEUP=0;
#ifdef KANALS
int TEST_KANAL;//флаг переключения АРМа в просмотр каналов
#endif
int TEST_N=0;
int TIME_ERROR_BBKP=0;
int ERROR_BBKP_0=0, ERROR_BBKP_1=0, ERROR_BBKP_2=0;
int ERROR_TUMS1_0=0,ERROR_TUMS1_1=0,ERROR_TUMS1_2=0;
int ERROR_TUMS2_0=0,ERROR_TUMS2_1=0,ERROR_TUMS2_2=0;
int timer_set;
int TORMOZ=0;
int tst;
int tst_tst=0;
int TUMS_RABOT[skoko_stoek];
int u;
int ukaz_ot_dc;
int ukaz_out_spd;
int ukaz_spd;
int ukaz_trass;//числовой указатель объекта в массиве trassa
int uprav=0;
int USER_FONT;
int V=0;
int vib_ma=0;
int vibeg;
int vih_vopr=0;
int vozvr;
int vrem=0;
int vspz;
int vtora;
int vvod_dc;
int vvod_set;
int vvod_tums;
int vysv;
int vysvv;
int vzvod=0;
int was[WAZO];
int X_DC=0;//коорд.X для просмотра конвертации данных ДЦ (в отладке)
int X_kan[2]={0,0};//коорд.X для просмотра принятых данных ТУМС1(2)(в отладке)
int X_m=10;
int X_menu;
int X_m_old;
int X_NAL=0;
int X_time;
int xc;
int Y_DC=100;//коорд.Y для просмотра конвертации данных ДЦ (в отладке)
int Y_kan[2]={20,60};//коорд.Y для просмотра принятых данных ТУМС1(2)(в отладке)
int Y_m=10;
int Y_m_isk;
int Y_m_old;
int Y_NAL=10;
int Y_txt=0;
int yc;
int yearr_;
int zabeg;
int zafix=0;
int ZAGRUZKA=0;
int ZAHOD=0;
int zakr=0;
int zalet;
int zapret[skoko_stoek];
int zapret_sos[skoko_stoek];
int zapret_otlad;
int zapretvybora;
int ZAPROS_FR4=0;
int zapusk;
int ZAVIS;
int zdem_ps_pso=0;
int Zdem_vibor_marshruta=0;
int Zdem_vibor_opov=0;
int Zdem=0;
int ZONA_KURSORA=-1;
int zzzzx;
int OTMENA;
int OGRAN;
int MAK;
int AVAR;
//------------------------------
int mas_lu[skoko_stoek][15],
    mas_lv[skoko_stoek][15],
		mas_ly[skoko_stoek][16], //массив фиксации ненорм разрядов модулей М201-1
		mas_lz[skoko_stoek][16], //массив фиксации ненорм разрядов модулей М201-2
    mas_lc[skoko_stoek][16], //массив фиксации ненорм разрядов модулей М201-3
    mas_ls[skoko_stoek][16], //массив фиксации ненорм разрядов модулей М201-4
    mas_lu[skoko_stoek][15],
		mas_lv[skoko_stoek][15],
        ly[skoko_stoek][4],
        lz[skoko_stoek][4],
        lc[skoko_stoek][4],
				ls[skoko_stoek][4],
        lu[skoko_stoek][3],
        lv[skoko_stoek][3];
long time_ly[skoko_stoek]={0},
time_lz[skoko_stoek]={0},
time_lc[skoko_stoek]={0},
time_ls[skoko_stoek]={0},
time_lu[skoko_stoek]={0},
time_lv[skoko_stoek]={0};

//--------------------------переменные для организации обменов ТУМС-АРМ
int ukaz_vyd[skoko_stoek];//указатели очередного байта выдачи в буфере BUF_OUT в ТУМС
int ukaz_zap[skoko_stoek];//указатели очередного байта записи данных в буфере BUF_OUT в ТУМС
int ukaz_read[skoko_stoek];//указатели очередного байта выдачи в буфере BUF_OUT в ТУМС
int ukaz_priem[skoko_stoek];//указатели очередного байта записи данных в буфере BUF_OUT в ТУМС

int N_RAZ[skoko_stoek];//счетчик числа повторений выдачи команд в ТУМС
int sboy_ts[skoko_stoek]; //счетчики сбоев канала ТС для ТУМС
int sboy_tu[skoko_stoek]; //счетчики сбоев канала ТУ для ТУМС
int otkaz_ts[skoko_stoek];//индикаторы отказа канала ТС для ТУМС
int otkaz_tu[skoko_stoek];//индикаторы отказа канала управления ТУМС
//---------------------------переменные для организации обменов АРМ-ДЦ
int ukaz_vyd_dc;//указатели очередного байта выдачи в ДЦ
int ukaz_zap_dc;//указатель очередного;байта записи из ДЦ
int ukaz_read_dc;
int ukaz_priem_dc;
int ukaz_com_dc;//указатель на активный регистр приема команд ДЦ для передачи в ТУМС
int sboy_ts_dc;//счетчик сбоев канала ТС для ДЦ
int otkaz_ts_dc;//индикатор отказа канала ТС для ДЦ
int otkaz_tu_dc;//индикатор отказа канала управления ДЦ
int dc_not_plus;
int symb_1;
//-------------------------------------------------------
unsigned char password[8]="", //строковая переменная для ввода пароля
adr_spdlp,
str_baz[32],//строка для контроля целостности базы
BEG_TIM[20],
BUF_OUT_SPD[75],//буфер вывода для СПДЛП
chudo[20]="",
com_bit[kol_VO], //номер бита на который выдана команда
*NAME_STATION, //имя станции
NOM_FUNC[4],
END_TIM[20],
K64, //число плат СПДЛП на 64 контакта
K32, //число плат СПДЛП на 32 контакта
LEN_SPD, //длина информационного пакета СПДЛП
NAME_FILE[17],
OBMEN_PVM,
OBMEN_PVM_OLD,
STRoka[40]="",
sbis[2]="",
soobs[12]="",
//-------------------перемннные для обмена с ТУМС
SYMBOL[skoko_stoek], //пара приемных ячеек для приема очередного символа из порта ввода
BUF_OUT[skoko_stoek][SIZE_BUF_OUT], //кольцевые буферы вывода в ТУМС
BUF_IN[skoko_stoek][SIZE_BUF_PRIEM],//кольцевые буферы приема из ТУМС
REG_OT[skoko_stoek][6];  //массив регистров ответов для передачи в ТУМС

//массивы регистров сообщений из ТУМС
unsigned char
	bu[skoko_stoek][N_BYTE],
	REG_INFO[skoko_stoek][N_BYTE],
	REG_COM_TUMS[skoko_stoek][N_BYTE],  //регистры команд для передачи в ТУМС
	REG_PODT_TUMS[skoko_stoek][N_BYTE], //регистры подтверждений из ТУМС
	REG_COM[N_BYTE],                    //регистры команд в соседнюю ПЭВМ
	BUF_COM_PVM[N_BYTE+1],                //буфер приема команд из соседней ПЭВМ
	buf_ko[N_BYTE],                     //выходной буфер команд для передачи в ТУМС
	buf_ko_p[N_BYTE],
	buf_ko_mm[N_BYTE],
	buf_ko_vsp[N_BYTE+1],
	buf_zap_disk[N_BYTE],
	buf_ko_p2[N_BYTE],
	KOMANDA_TUMS[skoko_stoek][15],
	MARSH_VYDAN[skoko_stoek],
	MYTHX[skoko_stoek],
	MYTHX_TEC[skoko_stoek];
	#ifdef DISPETCHER
	unsigned char
		REG_INF_DC[KOL_SOO][N_BYTE+2],      //11 байт сообщение + 1 байт новизна+
		REG_COM_DC[N_BYTE],               //регистр команд ДЦ для передачи в ТУМС
		REG_RON[N_BYTE];                  //регистр команд для РОН/РОЧ
	#endif
#ifdef DISPETCHER
//-------------------перемннные для обмена с ДЦ
unsigned char
	buf_ko_dc[skoko_stoek][N_BYTE], //для передачи в ДЦ
	BUF_OUT_DC[SIZE_BUF_OUT_DC],        //кольцевой буфер выдачи данных для ДЦ
	BUF_IN_DC[SIZE_BUF_PRIEM_DC],    //кольцевой буфер приема из ДЦ

	//массивы регистров сообщений ТУМС для передачи в ДЦ
	REG_OT_DC[6],                 //регистр ответов ДЦ для передачи в ТУМС
	REG_PODT_DC[skoko_stoek][6]; //массив регистров подтверждений от ТУМС
#endif

//--------------------------------------
#ifdef MOUSE
	unsigned char
	line_gor_kurs[12],
	line_vert_kurs[40],
	line_old_gor_kurs[12],
	line_old_vert_kurs[40];
#endif
//------------------------------
unsigned char
	H_0,
	H_1,
	M_0,
	M_1;
//---------------------------------------------------------------------
unsigned int fr4[kol_VO][3];
unsigned int ukazat[138];//массив для слов описывающих изображение указателя-курсора
struct TRAS TRASSA[100],TRASSA0[100],TRASSA_RIS[100];
struct MARS_ALL MARSHRUT_ALL[skoko_stoek*3];
struct MARS_ST MARSHRUT_ST[skoko_stoek][MARS_STOY];
unsigned int ml_ba;
unsigned int ml_bas;
unsigned int st_ba;
unsigned int st_bas;
unsigned int ml_ba2;
unsigned int st_ba2;
unsigned int ml_bap;
unsigned int st_bap;
unsigned int ml_bab;
unsigned int st_bab;
unsigned int SP_STR[10][5];
unsigned int TEST_SPAR;
unsigned int COD_DN;
unsigned int len; /* счит kol_OSN * 2 * 3 */
//--------------------------------
int zvu_tabl[K_oo][10]=
{
0,  0,  0,  0,  0,  0,  0,  0 ,'$',0,  /*0*/
1,  1,  1,  1, '%','$','','',0,0,  /*1*/
0,  0,  0,  1,  0,  0,  0, '%',0,0,  /*2*/
0,  0,  0,  0, '*','*','*','*',0,0,  /*3*/
1,  1,  1,  1, '$','$', 0,  0 ,0,0,  /*4*/
0,  0,  0,  0, '%','%','%', 0 ,0,0,  /*5*/
0,  0,  0,  0, '%', 0 ,'%', 0 ,0,0,  /*6*/
0,  0,  0,  0, '%', 0 ,'%', 0 ,0,0,  /*7*/
0,  0,  0,  0,  0 , 0 , 0 , 0 ,0,0,  /*8*/
0,  0,  0,  0,  0 , 0 , 0 , 0 ,0,0,  /*9*/
0,  0,  0,  0, '@','@','@','@',0,0,  /*10*/
0,  1,  0,  0,  0 ,'@','@', 0 ,0,0,  /*11*/
0,  0,  0,  0,  0 , 0 , 0 , 0 ,0,0,  /*12*/
0,  0,  0,  0, '%','%','%','%',0,0,  /* неиспр пит*/
1,  1,  1,  0, '%','%','%', 0 ,0,0,  /*14*/
0,  0,  0,  0, '%','%','%', 0 ,0,0,  /*15*/
1,  1,  1,  1,  1, '%','%','%','%',0,  /*16*/
0,  0,  0,  0, '%','%','%', 0 ,0,0,  /*17*/
0,  0,  0,  0, '%','%', 0 , 0 ,0,0,  /*18*/
0,  0,  0,  0, '%','%','%', 0 ,0,0,  /*19*/
0,  1,  0,  0,  0, '%', 0,  0 ,0,0,  /*20*/
0,  0,  0,  0, '#', 0 , 0 , 0 ,0,0,  /*21*/
0,  1,  0,  0,  0 ,'@', 0 , 0 ,0,0,  /*22*/
1,  1,  0,  0, '','$', 0 , 0, 0,0,  /*23*/
0,  1,  1,  1,  0 , 0 ,'%', '%','%',0,/*24*/
1,  1,216,  0, '%','%','$', 0 ,0,0,  /*25*/
1,  1,  1,  1,  1 , '%','%','%','%','%',/*26*/
0,  1,  1,  0,  0 ,'$','$', 0 ,0,0,  /*27*/
0,  1,  1,  0,  0, '$','@','@','$',0,  //28
0,  1,  1,  0,  0, '@','@', 0 ,0,0,  //29
0,  0,  0,  0,  0,  0,  0,  0, 0,0,  //30
0,  0,  0,  0,  0,  0,  0,  0, 0,0,  //31
1,  1,  1,  0, '%','%','%', 0, 0,0,  //32
0,  1,  0,  1, '$','@','$','@',0,0,  //33
0,  0,  1,  1,  0,  0, '$','$',0,0, //34
0,  1,  1,  1, '$', '%','%','%',0,0,  //35
0,  0,  0,  0, '$','$', 0,  0,0,0,   //36
0,  0,  0,  0, '$', 0,  0,  0,0,0,   //37
0,  0,  0,  0, '$','%','%','%',0,0,  //38
1,  1,  0,  0,  0, '','', 0,0,0,   //39
1,  1,  1,  1, '@','@','@','@',0,0,  //40
0,  0,  0,  0, '@','@','@','@',0,0,  //41
};

unsigned int cvt[K_oo][10]=
{
	7, 7, 8,12,8,12,8,12, 0, 0, //0 без звука и без записей
 10,12, 0, 0,0, 0,0, 0, 0, 0, /*1*/
	7,10, 7, 7,7, 7,7, 7, 7,14, /*2*/
	0,15, 0,15,0,15,0,15, 0, 0, /*3*/
	7,12, 7,12,7,12,0, 0, 0, 0, //4 со звуком на все
	8,12, 8,12,8,12,7, 7, 0, 0, /*5*/
	7,12, 7,12,7,12,7,12, 7,12, //6 без звуков, с записью 0-го и 2-го
	8,14, 8,12,8,12,8,12, 0, 0, /*7*/
	7,14,10,15,7,14,7,14, 0, 0, /*8*/
	8,10, 8,10,8,10,8,10, 0, 0, /*9*/
	7,15, 7,15,7,15,7,15, 0, 0, /*10*/
0xc888,0xc888,0xcce8,0xcec8,0,0,0,0,0,0, //11
	8,12, 8,10,8,14,0, 0, 0, 0, /*12*/
	8,12, 8,12,8,12,8,12, 7,12, //13 без звука с записью всех
	8,14, 8,14,8,14,8,14, 0, 0, /*14*/
	8,12, 8,12,8,12,0, 0, 0, 0, //15 без звука с записью всех
	7,12, 7,12,7,12,7,12, 0, 0, //16 со звуком на все
 10,14, 8,12,8,12,8,12, 0, 0, /*17*/
 12, 7,10,10,8, 8,8, 8, 0, 0, /*18*/
	8,12, 8,12,8,14,0, 0, 0, 0, /*19*/
	8,12, 8,12,8,12,8,12, 8,12, //20 со звуками на 0-й и 2-ой
	8,12, 8,12,7, 7,7, 7, 0, 0, /*21*/
	7,12, 7,12,7,12,7,12, 0, 0, /*22*/
	7,14, 7,14,7,14,7,14, 0, 0, /*23*/
 14,10, 0, 0,0, 0,0, 0, 0, 0, /*24*/
	7,12, 7,12,7,12,0, 0, 7,12, /*25*/
	7,14, 8,14,7,14,7,14, 7,14, /*26*/
	7,12, 7,10,7,14,0, 0, 0, 0, /*27*/
	8,12, 7, 7,7, 7,7, 7, 0, 0, /*28*/
0x87,0xda,0x87,0xda,0x87,0xda,0x87,0xda,0,0,//29
	8,14,12,12,0, 0,0, 0, 0, 0, //30
	8,13, 7,10,0, 0,0, 0, 0, 0, //31
	7,10, 7,10,0, 0,0, 0, 0, 0, //32
 14,10,13,13,0, 0,0, 0, 0, 0, //33
	7,14, 8,14,0, 0,0, 0, 0, 0, //34
	8,12,14,12,0, 0,0, 0, 0, 0, //35
	7,15, 7,15,7,15,7,15, 0, 0, //36
	8,12, 8,10,8,14,0, 0, 0, 0, //37
	7,12, 7,12,7,12,7,14, 0, 0, //38
	8,14, 8,14,8,14,8,14, 8,14, //39
 10, 8, 8,12,8,12,0, 0, 0, 0, //40
0x8a88,0x8a88,0x88cc,0x888c,0xc888,0xdede,0, 0,0,0, //41
};

unsigned char TIME[11];



union REGS regs,rg;

unsigned int TIMER_N[25][3];// Блок из 25 таймеров по 3 слов
               // 1-номер объекта,
               // 3-тип объекта 
               // 4-номер подобъекта инициализации 
unsigned char TEST_SOOB[skoko_stoek][7];

//---------------------------------------
unsigned char
    IT_IS_COM_PVM=0,
    IT_IS_FR4=0,
    IT_IS_INF=0,
    PERED_PVM=2,
    REG_INF[4], BUF_INF[4],     REG_KVIT_INF[3],KVIT_INF,POVTOR_INF,
		REG_KVIT_COM[3],KVIT_COM,POVTOR_COM,
    REG_FR4[6], BUF_FR4[6],     REG_KVIT_FR4[3],KVIT_FR4,POVTOR_FR4;

int UKAZ_INF=0,END_INF=0,
    UKAZ_FR4=0,END_FR4=0,
    UKAZ_COM=0,END_COM=0,
    UKAZ_KVIT_INF=0,END_KVIT_INF=0,
    UKAZ_KVIT_FR4=0,END_KVIT_FR4=0,
    UKAZ_KVIT_COM=0,END_KVIT_COM=0,
    MY_INF,MY_COM,MY_FR4,UKAZ_ZAP_OUT,UKAZ_VYVOD, UKAZ_PRIEMA_PVM;
unsigned char BUF_OUT_PVM[MAX_SIZE_OUT];
unsigned char BUF_IN_PVM[MAX_SIZE_IN];
unsigned char D_kont[9]={'p','q','r','s','t','u','v','w','x'};
//-----------------------------
int SMI,errorcode;
int b_day,b_h0,b_m,b_m0,b_mon,b_mon0,b_mon1,
b_s,b_s0,b_second,b_u,bb_d,bb_h,bb_m,bb_mi,bb_y,box_uk,
bq,bqq,col,dodo,dubl,durdom,ee,end_,fing,flpass,goga,hop,ismena_,no,nome,old,
opred_new,opred1,opred_old,poshag,PRICHINA_SN_AVTO[2],
sss,zahod,zapr;
unsigned char
buf[skoko_stoek][N_BYTE],
data[30];
unsigned mydate,mytime,timer;
unsigned long int wait();
