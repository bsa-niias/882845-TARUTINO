#include"opred.h"
#include"nastroy.h"
#include <time.h>
#include <dos.h>
#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bios.h>
#include <sys\stat.h>
#include <dir.h>
#include <ctype.h>
#include <FCNTL.H>
#include <io.h>
//--------------------------------------
void add(int	a,int sob,unsigned	char	b); 	// определена в box.c
void add_oblom(int	a);               // определена в wr_ne.c
void add_word(char	a,char  b[20]);   // определена в wr_ne.c
int	all_null(int	a);                 // определена в station.c
int analiz(int a);                    // определена в osn_disk.c
void ANALIZ_BBKP(void);               // определена в chan_2.c
int analiz_com_dc(int a);             // определена в chan_2.c
void  Analiz_Glob_Marsh(void);        // определена в komm_2.c
void ANALIZ_KOM_TUMS(void);           // определена в chan_2.c
int ANALIZ_MYTHX(unsigned char PODGR,int ST); //определена в komm_2.c
void analiz_otprav(int a,int b);      //
void analiz_priem(int a,int b);       //
void analiz_time(void);               // определена в station.c
void ANALIZ_TUMS(void);               // определена в chan_2.c
void avto(void);                      // определена в avto.c
int	avto_marsh(int	a);               // определена в proverki.c
void barcolor(int	a,int	b);           // определена в datch2.c
int byla_com(unsigned char a[12]);    // определена в chan_2.c
int byla_inf_dc(unsigned char a[12]); // определена в chan_2.c
unsigned char
		CalculateCRC8(void *a,int b);     // определена в crccalc.c
void card(void);                      // определена в read_f62.c
void Change_adresa(void);             // определена в zagr.c
int	check_sum(unsigned	char	a[12],int b);// определена в komm_2.c
int	chego_posl(int	a);               // определена в for_read.c
void clear_half(void);                // определена в r_f622.c
void clear_K(void);                   // определена в zagr.c
void clear_mouse(void);               // определена в osnov.c
void clscreen(void);                  // определена в box.c
void com_avtovozvr(int	a);           // определена в strelki.c
void COM_FROM_DC(void);               // определена в chan_2.c
void consentr(void);                  // определена в chan_2.c
int	cvet_matr(void);                  // определена в marsh2.c
void	del_kursor(int	a);             // определена в keyb.c
void DeleteMarsh(int a);              // определена в komm_2.c
void DelMarsh(int a);                 // определена в komm_2.c
void	den_noch(int	a);               // определена в datch2.c
int	der(void);												// определена в osn_disk.c
int diagnoze(unsigned	char a[12],int b);  //определена в box.c
void	disco(void);                    //определена в box.c
void	Disk_arc(void);                 //определена в osn_disk.c
void	dispdatch(int	a,int	b);         //определена в datch2.c
void	displaypath(int	a,int	b);       //определена в marsh2.c
void	displaypathuch(int	a);         //определена в marsh2.c
void	displaysignal(int	a,int	b);     //определена в for_read.c
void	displaystrelka(int	a,int	b,int	c); //определена в strelki.c
void	dn_au(int	a);                   //определена в station.c
int	do_it(int	a,unsigned	char	b[12]); //определена в find.c
void	dr(void);                       //определена в datch2.c
void	draw_mouse(void);               //определена в osnov.c
void	dzezl(int	a,int	b);             //определена в for_read.c
void	electri_platf(int	a);           //определена в find.c
void	elo(int	a);                     //определена в find.c
void	end_help(void);                 //определена в keyb.c
int est_com_ARM(int a);               //определена в chan_2.c
void	fider3dat(int	a,int	b);         //определена в datch2.c
void	figura(int	a);									//определена в find.c
void	fin_kom(void);									//определена в chan_2.c
void	FINAL(void);                    //определена в osn_disk.c
void	FINAL_(void);                   //определена в keyb.c
int	find_end(int a);                  //определена в proverki.c
int	find_obj(unsigned	char a,
unsigned char b,int c,int d);         //определена в box.c
int	findspu1(int	a,int	b);           //определена в find.c
void	finish_startMM(int a);           //определена в marsh2.c
void	finish_strelok(void);           //определена в keyb.c
void	first_razd(int	a,int	b);       //определена в station.c
void	first_second(int	a);           //определена в r_f622.c
void	fix(int	a,int	b);               //определена в datch2.c
void	fixation(int	a,int	b);         //определена в raspac.c
int fix_bad_com(int a);               //определена в chan_2.c
void fix_poter(int a,int b);          //определена в chan_2.c
void	fon_pic(void);                  //определена в r_f622.c
void	FORM_BUF_PVM_OUT(void);					//определена в chan_2.c
void	form_first_kom(int	a);         //определена в station.c
int	form_fr4(int	a);                 //определена в chan_2.c
int	form_koda(int	a,int	b);           //определена в komm_2.c
int	form_kodop(int	a);               //определена в komm_2.c
void form_kom_avtod(char a);					//определена в proverki.c
void form_kom_knopka(int	a,int	b);   //определена в proverki.c
void form_kom_nemarsh(int	a);         //определена в proverki.c
void form_kom_nk_chk(int	a);         //определена в proverki.c
void form_kom_prkl(int	a);           //определена в proverki.c
void form_kom_put(int	a,int	b);       //определена в komm_2.c
void form_kom_smen(int	a);           //определена в proverki.c
void form_kom_str(int a,unsigned	char b);//определена в komm_2.c
void form_kom_vsp(void);              //определена в proverki.c
int	form_kosig(int	a);               //определена в komm_2.c
void form_second_kom(int	a);         //определена в station.c
void formkps(int	a);                 //определена в proverki.c
void formula(int	a);                 //определена в start2.c
void fotksig(int	a);                 //определена в komm_2.c
void fraza(int	a);                   //определена в r_f622.c
void fraza2(int	a);                   //определена в r_f622.c
void get_int_vect(void);              //определена в chan_2.c
void get_p(int	a,int	b,int	c);       //определена в keyb.c
int	get_str(int	a,int	b);             //определена в find.c
void gri_razd(int	a);                 //определена в station.c
void h(void);                         //определена в helper.c
void home(int	a);                     //определена в keyb.c
void imena(int	a);                   //определена в start2.c
void in_port(unsigned int a);         //определена в chan_2.c
void in_port_BBKP(unsigned int	a);   //определена в chan_2.c
void init(void);                     //определена в chan_2.c
void init_markery(int	a);           //определена в start2.c
void Init_TEST_SOOB(int	a);         //определена в osnov.c
int	ischem(void);                     //определена в chan_2.c
void ismena(void);                   //определена в osn_disk.c
void ispr_pvm(int	a,int	b);         //определена в datch2.c
void k_dso(int a);										//определена в station.c
void k_fdp(int a);                    //определена в station.c
void k_per(int	a);                   //определена в station
void kanal(int	a);                   //определена в r_f622.c
void kanal_DC(void);                 //определена в datch2.c
void kanal_SPDLP(void);              //определена в datch2.c
void kartina(void);                  //определена в r_f622.c
void keyboard(void);                 //определена в keyb.c
int	keykey(void);                     //определена в osn_disk.c
void KM_put(int	a,int	b,int	c);     //определена в for_read.c
void knopa(int	a);                   //определена в station.c
void knopka(int	a);                 //определена в datch2.c
void knopka_OK(void);								//определена в station.c
void knopka_vkl_opove(int	a);				//определена в datch2.c
void knopka_vkl_vsego(int	a,int	b); //определена в datch2.c
void knopka_vkl_vsego1(int a,int b);//определена в datch2.c
void kom_dso(int a);                  //определена в station.c
void kom_fdp(int a);                  //определена в station.c
void kom_per(int	a);                 //определена в station.c
void kom_v_bufer(int	a);             //определена в chan_2.c
int	kom_v_bufer_pvm(int	a,unsigned	char	b);	//определена в proverki.c
void komplekt(int	a,int	b);           //определена в datch2.c
void koni(void);                      //определена в proverki.c
void kontr_perego(int	a,int	b);       //определена в for_read.c
int	kontroler(unsigned	char	a[12],  //определена в raspac.c
int	b,int	c,int	d);
int kotora(int a);                    //определена в wr_ne.c
void kps_osn(int	a);                 //определена в for_read.c
void kps_rez(int	a);                 //определена в for_read.c
void krasit_knopki(int	a,int	b);     //определена в datch2.c
void krasit_razd(int	a,int	b);       //определена в datch2.c
void krest(int	a,int	b,int	c);       //определена в raspac.c
void kvadrat(void);                   //определена в find.c
void kvv(int	a,int	b);               //определена в datch2.c
void lampa(int	a,int	b);             //определена в datch2.c
void lampa_3(int	a,int	b);           //определена в datch2.c
void lampa_3c(int	a,int	b,int	c);     //определена в datch2.c
void lampa_5(int a,int b);            //определена в datch2.c
void lampa_compl(int	a,int	b);       //определена в datch2.c
void lampa_fr2(int a,int b);          //определена в datch2.c
void lampa_konc(int	a,int	b);         //определена в datch2.c
void lampa_N(int a,int b,int c);      //определена в for_read.c
void lampa_text(int a,int b);					//определена в datch2.c
void lru_lko(int	a,int	b,int	c);     //определена в datch2.c
void make_avtod(void);                //определена в avto.c
void make_dn(void);                   //определена в vibor.c
void make_dso(void);                  //определена в vibor.c
void make_fdp(void);                  //определена в vibor.c
void make_gri(void);                  //определена в vibor.c
void make_knopka(void);               //определена в vibor.c
void make_knpk2(void);                //определена в vibor.c
void make_nemarsh(void);              //определена в vibor.c
void make_neotv(void);                //определена в vibor.c
void make_ograd(void);
void make_opoveshenie(void);          //определена в vibor.c
void make_otvetst(void);              //определена в vibor.c
void make_per_kompl(void);            //определена в vibor.c
void make_pereezd(void);              //определена в vibor.c
void make_put(void);                  //определена в vibor.c
void make_razdelka(void);             //определена в vibor.c
void make_signal(void);               //определена в vibor.c
void make_smena(void);                //определена в vibor.c
void make_vspom_otpr(void);           //определена в vibor.c
void make_vspom_priem(void);          //определена в vibor.c
void MARSH_GLOB_LOCAL(void);          //определена в komm_2.c
void MARSH_V_TUMS(int a);							//определена в komm_2.c
void marsh1(int a);										//определена в station.c
void marsh2(int a);                    //определена в station.c
int	may_be(int	a,int	b,int c);       //определена в proverki.c
void mem_mous_fon(void);              //определена в osnov.c
void menu(void);                      //определена в osnov.c
int	menu1(void);                      //определена в keyb.c
void menu_hoz_poezd(int a);           //определена в marsh2.c
int	mouse(void);                      //определена в osnov.c
void mouse_init(void);                //определена в osnov.c
void move_mouse(void);                //определена в osnov.c
void music1(void);                    //определена в osn_disk.c
void nach_zagruzka(void);             //определена в start2.c
void nach_zakr(int	a,int	b);         //определена в vibor.c
int	nashel(int	a);                   //определена в station.c 
void neisprav_na_disk(char a,char	b[20]); //определена в wr_ne.c
void nemar(int	a);                   //определена в datch2.c
void nemarsh(int	a);                 //определена в station.c
void neot(int	a);                     //определена в vibor.c
void nok(int	a);                     //определена в station.c
void noka(int	a);                     //определена в station.c
void nokb(int	a);                     //определена в station.c
void nom_func(char a[5]);             //определена в osnov.c
void obnov_kur_mouse(int	a,int	b);   //определена в keyb.c
int	obnov_201_1(int a,int b,unsigned	char c[12],int d);//определена в find.c
int	obnov_201_2(int a,int b,unsigned	char c[12],int d);//определена в find.c
int	obnov_201_3(int a,int b,unsigned	char c[12],int d);//определена в find.c
int	obnov_201_4(int a,int b,unsigned	char c[12],int d);//определена в find.c
int	obnov_obied(int a,int b,unsigned	char c[12]);      //определена в find.c
int	obnov_obryv(int a,int b,unsigned	char c[12]);//определена в find.c
void obnovi_obraz(int a,int b,int c,char name_lamp[15],int svebv);//определена в find.c
void obnovi_obraz1(int a,int b,int c,char name_lamp[15],int cvkn);//определена в find.c
void obrabotka_inf(int a);           //определена в raspac.c
void ochistka(void);								 //определена в chan_2.c
void ognev(int	a,int	b,int	c);      //определена в datch2.c
void OK(int	a,int	b);                //определена в zagr.c
void okrasklav(int	a);              //определена в vibor.c
void opoveshe(int	a,int	b);          //определена в station.c
int	Opred(void);                     //определена в komm_2.c
void otkaz_obkt(int a,int b,int c);  //определена в for_read.c
void otkl(int	a);                    //определена в vibor.c
void otkl_(int	a,int	b);            //определена в avto.c
void otkrs(void); 									 //определена в station.c
void otkrsi(void);									 //определена в proverki.c
void OUT_DC(void);									 //определена в chan_2.c	
void OUT_SPD(void);                  //определена в station.c
void out_time(void);                 //определена в box.c
void p_li(int	a);                    //определена в find.c 2
void Pam(int	a);                    //определена в datch2.c
int	parafaz(int	a);                  //определена в datch2.c
void perekl_komp(int	a);            //определена в station.c
void perstr(void);                   //определена в station.c
void pict_podsvetka(void);           //определена в datch2.c
void pict1(int	a);                  //определена в find.c
int pishu_com_dc(unsigned char a[12]); //определена в chan_2.c
int pishu_podt(int a);                 //определена в chan_2.c
int pishu_soob(int a);                 //определена в chan_2.c
void plus_minus(int a);                 //определена в proverki.c
void podtver(void);                    //определена в proverki.c
int	POISK(void);                       //определена в osn_disk.c
int poisk_obekt(unsigned char buf[12]);//определена в chan_2.c
int	poisk_ras(int	a,int	b,int c,int d);      //определена в strelki.c
void PovtorMarsh(int a);							 //определена в komm_2.c
void priem_otpr(int	a,int	b);          //определена в for_read.c
int	priem_pvm(int	a);                  //определена в chan_2.c
int PRINT_ALL(void);                  //определена в osn_disk.c
void Printf(int	a,int	b,int	c);				 //определена в osn_disk.c
void prkl(int	a);                      //определена в station.c
void prodol(void);                     //определена в osnov.c
void prodol1(void);                    //определена в osn_disk.c
void prov_av_str(int	a,int	b,int	c,int	d,int	e); //определена в strelki.c
int	prov_avail(int a);                  //определена в proverki.c
void prov_avto_si(int	a);              //определена в avto.c
int	prov_avtod1(int	a);                //определена в avto.c
int	prov_ele_avtod(int a);						 //определена в avto.c
void prov_lX(void);                    //определена в start2.c
int prov_negabarit(int a,int b);       //определена в avto.c
int prov_negabarit_ohr_pl_mi(int a,int b,int c,int d);//определена в proverki.c
int prov_negabarit_pl_mi(int a,int b); //определена в proverki.c
int prov_negabarit_pl_mi2(int a,int b);//определена в proverki.c
int	prov_si_avtod(int	a,int	b);        //определена в avto.c
void proverka_NA(void);                //определена в zagr.c
void proverka_STA(void);               //определена в zagr.c
void proverka_svyazi(void);            //определена в chan_2.c
void ps_pso(int	a,int	b);              //определена в for_read.c
void put_p(int a,int b,int c);         //определена в keyb.c
int	putec(unsigned	char	a[12],int	b,int	c,int	d); //определена в raspac.c
void puts1(char *a,unsigned char b,int c,int d);  //определена в osnov.c
void PVM(int	a,int	b);                //определена в r_f622.c
void rama(void);                       //определена в helper.c
void ramka(int	a,int	b,int	c,int	d);  //определена в osn_disk.c
void ramki(int	a);                    //определена в find.c
void rashifr(int	a,int	b);            //определена в read_f62.c
int	rasp_fr4(void);                    //определена в chan_2.c
int	razdelan(int	a,int	b);            //определена в marsh2.c
int	read_bl_box(void);                 //определена в osn_disk.c
void read_dzezli(int	a);              //определена в
void read_lex(void);                   //определена в
void read_neisprav(void);              //определена в
void read_passw(void);                 //определена в
void read_pereezd(int	a);              //определена в
void read_slova(int	a);                //определена в
void read_smen(int	a);                //определена в
void read_sode(int	a,int	b);          //определена в
void read_spdlp(void	*a);             //определена в
int	read_str(int a,unsigned	char b[50],unsigned	char c[12]);//определена в
unsigned char read_str1(int	a,unsigned char	b[50],
unsigned char	c[12]);                  //определена в
void read_t(void);                     //определена box.c
void read_text1(int	a);                //определена в
void reading(void);                    //определена в
void reset_arm(void);                  //определена в chan_2.c
void reset_int_vect(void);             //определена в
void Rezult(int	a,int	b,int	c);        //определена в
void Rezult_2(int	a,int	b,int	c);      //определена в
void ris_batarei(int	a,int	b);        //определена в
void ris_blk(int	a,int	b,int	c,int	d); //определена в
void ris_blok_uch(int	a,int	b);        //определена в
void ris_nemar(int	a);                //определена в
void ris_ogr_str(int	a,int	b);        //определена в
void ris_otv_knop(int	a);              //определена в datch2.c
void ris_otv_knop1(int a);						 //определена в datch2.c
void ris_smen_otv(int	a,int	b);        //определена в
void ris_sp_str(void);                 //определена в
void ris_trassa(int a);                //определена в
void sbros_all(void);                  //определена в
void SBROS_BIT_TEST_SOOB(int a,int	b); //определена в osnov.c
void sbros_obmen_pvm(void);            //определена в
void sbros_time(int	a,int	b);          //определена в
void sbros_time_tip(int	a);            //определена в
void sekci(int	a,int	b);              //определена в
void sekos(int	a,int	b);              //определена в
void set_dc(void);                     //определена в keyb.c
void set_int_vect(void);               //определена в
void set_timers(void);                 //определена в
void set_tums(void);                   //определена в vibor.c
void set_vvod(void);                   //определена в
void shapka(void);                     //определена в
void shl(void);                        //определена в
void show_kursor(int	a);              //определена в
int	signale(unsigned	char	a[12],int	b,int	c,int	d);//определена в
void sigo(int	a,int	b,int	c,int	d);   //определена в
void slom_interf(int	a);             //определена в
void slom_sign(int	a);               //определена в
void slom_uch(int	a,	char	b);       //определена в
void smen_napr(int	a);               //определена в
void smena(int	a);                   //определена в
void snyato_avto(int	a);             //определена в
void snyato_chet(void);               //определена в
void snyato_nechet(void);             //определена в
int	sogl_otkr(int	a);                 //определена в
void spisok_prikazov(void);           //определена в
int	starit(void);                     //определена в
void start_port(int a);               //определена в
void start_time_date(void);           //определена в
int	str_Makst(int	a);                 //определена в
int	strelka_trass(int	ako,int	b,int c);//определена в//определена в proverki.c
int	strelka_v_makete(void);           //определена в
int	strelok(unsigned char	a[12],int	b,int	c,int	d); //определена в
void Styki(void);                     //определена в
void t(int	a);                       //определена в
int	t_dz(int	a);                     //определена в
int	t_put(int	a);                     //определена в
int	t_sig(int	a);                     //определена в
int	t_spu(int	a);                     //определена в
int t_str(int a,int b);               //определена в avto.c
int	t_uch(int	a);                     //определена в
void tel_vkl_vikl(int	a);             //определена в
void tele(int	a);                     //определена в
void test_deshifr(int	a,int	b);       //определена в
int	test_elem(int	a,int	b,int c,int d); //определена в
void test_mo(void);                   //определена в
int	test_mp(void);                    //определена в
void test_not_ra(int	a);             //определена в
void test_otvetstv(void);             //определена в
void test_port_pvm(void);             //определена в
void test_prigol_otm(int	a);         //определена в
void test_ra_fini(int	a);             //определена в
int	TEST_SUM_SOOB(void);              //определена в
int	test_time(void);                  //определена в
int	test_time1(long	a);               //определена в
int test_virt_time(void);             //определена в osn_disk.c
void time20sec(int	a);               //определена в
int	TIMER_SOB(int	a,int	b,int	c);     //определена в for_read.c
int	tst_chet(int	a);                 //определена в
int	tst_dz(int	a,int	b,int c,int d); //определена в
int	tst_fix(int	a,int	b);             //определена в
int	tst_nal_sig(void);                //определена в
int	tst_pos(int	a);                   //определена в
int	tst_put(int	a,int	b,int c);       //определена в
int	tst_sig(int	a,int	b,int c);       //определена в proverki.c
int	tst_sigogo(int a,int b);          //определена в
int	tst_spu(int	a,int	b,int c,int d); //определена в marsh2.c
int	tst_str(int	a,int	b,int c,int d); //определена в marsh2.c
int	tst_str_av(int	a,int	b);         //определена в
int	tst_str1(int	a);                 //определена в
int	tst_strelok(int	a,int b,int c);    //определена в
int	tst_uch(int	a,int	b,int c);       //определена в
void tst_zvuk(void);                  //определена в
void tu_tc(void);                     //определена в
void TUMS_MARSH(int a);                //определена в station.c
void t_p_pvm(int a);                  //определена в
void uch_udal(int	a,int	b,int	c);     //определена в
int	uchastic(unsigned	char	a[12],int	b,int	c,int	d); //определена в
void ukazka(void);                    //определена в
void UKG(int	a);                     //определена в
void uksps(int	a,int b);             //определена в
int	upak_fr4(int	a);                 //определена в chan_2.c
int	uze(int	a,int	b);                 //определена в
void v_o_ma(int	a);                   //определена в
int	v_spiske(int	a);                 //определена в
int	vibor(int	a);                     //определена в
void vidacha(int	a);                 //определена в
void vidacha_BBKP(int	a);             //определена в
void vio_vi(void);                    //определена в
void vkl_(int	a,int	b);               //определена в
void vkl_avto(void);                  //определена в
void vkl_dn(int	a);                   //определена в
void vkl_kno(int	a,int	b);           //определена в
void vkl_knop(int	a);                 //определена в
void vkl_nemarsh(int	a);             //определена в
void vkl_neot(int	a);                 //определена в
void vkl_opoveshe(int	a);             //определена в
void vkl_otkl_maket(int	a);           //определена в
void voper(void);                     //определена в
void vopr_mar(void);                  //определена в
void vopros(int	a,int	b,int	c,unsigned char	d[8]); //определена в
void vuhod(void);                     //определена в
void vuhod1(void);                    //определена в
int	vyborklav(void);                  //определена в
void w(int	a,int	b,	char	cc[20]);  //определена в
unsigned long	int	wait(void);         //определена в
void what_is_new(void);               //определена в
void whole_finish(int a);             //определена в
void WIN_CARD(void);                  //определена в
void WIN_HELP(void);                  //определена в
void WIN_MAIN(void);                  //определена в
void WIN_MAIN(void);                  //определена в
void WIN_NEISPRAV(void);              //определена в
void WIN_OTV(void);                   //определена в
void WIN_RAZD(void);                  //определена в
void WIN_SOOB(void);                  //определена в
void WIN_VOPROS(void);                //определена в
unsigned long int wait(void);         //определена в
void wind_w(int	a,int	b,int	c,int	d,int	e,int	f,int	g);//определена в for_read.c
void work(void);                      //определена в
void z1(int	a);                       //определена в
void zad_marsh(int a);                 //определена в
void zad_marsh_avto(int a);           //определена в station.c
int	ZAGO_MPSU(int	a);                 //определена в
void zagruzen(void);                  //определена в
void zakroy(int	a);                   //определена в
void zakrsi(void);                    //определена в
void ZAMKNI_FR3(int a);								//определена в komm_2.c
int	zamknut(int	a,int	b,int c);       //определена в marsh2.c
int	zamknuto(int a,int	b,int c);     //определена в
int	zanyat(int	a,int	b,int c);       //определена в marsh2.c
int	zanyato(int	a,int	b,int c);       //определена в proverki.c
void ZAP_BUF_SPD(void);               //определена в
int	zap_matr(int	a);                 //определена в
void ZAPOLNI_MARSH(int a,int b);			//определена в komm_2.c
void ZERO_TRASSA(int a);               //определена в station.c
void Zona_k(void);                    //определена в
void zona_opov(int	a,int	b);         //определена в
void zp_zp(int	a);                   //определена в
void zvuk_vkl(int	a,	int	b);         //определена в
void ZZagruzka(void);                 //определена в

extern unsigned char ARROW[800];
extern unsigned long int
BAZ_ADR1,
BAZ_ADR11,
BAZ_ADR2,
BAZ_ADR3,
BAZ_ADR4,
BAZ_ADR5,
BAZ_ADR6,
koef_uskor,
tormoz;

extern long FIR_time,
first_time,
pooo[kol_VO],
pooo_com[kol_VO],
SEC_time,
second_time,
t_inf[skoko_stoek],//фиксатор времени выдачи информации в ДЦ
t_com[skoko_stoek],//фиксатор времени выдачи команды в стойку
t_com_dc[skoko_stoek],//фиксатор времени выдачи команды ДЦ в стойку
TIME_OUT_PVM,
tii,
tiiq,
time_ly[skoko_stoek],
time_lz[skoko_stoek],
time_lc[skoko_stoek],
time_ls[skoko_stoek],
time_lu[skoko_stoek],
time_lv[skoko_stoek],
VIRT_TIME_0,
VIRT_TIME_1;

extern int
		mas_lu[skoko_stoek][15],
		mas_lv[skoko_stoek][15],
		mas_ly[skoko_stoek][16],
		mas_lz[skoko_stoek][16],
		mas_lc[skoko_stoek][16],
		mas_ls[skoko_stoek][16],
		mas_lu[skoko_stoek][15],
		mas_lv[skoko_stoek][15],
				ly[skoko_stoek][4],
				lz[skoko_stoek][4],
				lc[skoko_stoek][4],
				ls[skoko_stoek][4],
				lu[skoko_stoek][3],
				lv[skoko_stoek][3];

//----------------------------------------
extern struct so_so soob;
extern struct dosdate_t dat;
//----------------------------------------
extern int a_klav;
extern int ANALIZ[skoko_stoek];
extern int ANALIZ_DC;
extern int old_aoar[skoko_stoek];
extern int AV_zdem;
extern int avap;
extern int AVARI;
extern int AVARIIN;
extern int b_h1;
extern int b_m1;
extern int b_s1;
extern int BAIT_PRER;
extern int BAM;
extern int basha;
extern int bata;
extern int BAZA;
extern int BEDA[10];
extern int beg[2];
extern int bi;
extern int bil_bil;
extern int bit_pso;
extern int blokir[10][2];
#ifdef SPDLP
extern int bt_spdlp[N_BIT_SCB];
extern int byte_spdlp[N_BIT_SCB];
#endif
#ifdef AVTOD
extern int C_strel[CH_str];
#endif
extern int cha;
extern int chas;
extern int chasi;
extern int chit;
extern int chita;
extern int cikl_obnov;
extern int cikl_avto;
extern int COD_OGR;
extern int COD_OTPR[2];
extern int COD_PRIEM[2];
extern int COD_PS_NEW;
extern int COD_PS_OLD;
extern int COD_UCH_UDAL[2];
extern int CVETCVETOF;
extern int CVTF;
extern int cvv;
extern int da1;
extern int d_d;
extern int danet;
extern int dayy_;
extern int den;
extern int den1;
extern int DISK;
extern int down;
extern int DU;
extern int dv;
extern int DVA_MO;
extern int ELE;
extern int END;
extern int END_OPER;
extern int end_avto;
extern int end_marsh;
extern int end_marsh_oper;
extern int End_vara;
extern int FILTR[10];
extern int FFFlag;
extern int f_ob1;
extern int fi_co1;
extern int fiki;
extern int file_arc;
extern int file_neispr;
extern int file_oblom;
extern int file_soob;
extern int finish;
extern int fix_ts_sos[skoko_stoek];
extern int fix_tu_sos[skoko_stoek];
extern int fix_rek_sos[skoko_stoek];
extern int fl_file;
extern int fl_read;
extern int Fla_no_pp;
extern int flag_paro;
extern int FLAG_PO;
extern int flago;
extern int flagoutmsg;
extern int flms;
extern int flpass;
extern int flug;
extern int fr1[kol_VO][14];
extern int fr2[kol_VO][16];
extern unsigned int fr3[kol_VO][13];
extern int fra[kol_VO][2];
extern int g_g;
extern int god;
extern int god1;
extern int GraphDriver;
extern int GraphMode;
extern int gri;
extern int GRUPPA1[skoko_stoek][24];
extern int GUAV;
extern int half_marsh;
extern int half_marsh1;
extern int having;
extern int help;
extern int ho_ur;
extern int hod;
extern int hoz_poezd[2];
extern int hudo;
extern int hudoo;
extern int i_s;
extern int i3;
extern int iqw;
extern int j_jj;
extern int k;
extern int kadr[12][50];
extern int klaval;
extern int klo;
extern int ko;
extern int KOL_VYD_MARSH[skoko_stoek];
extern int kom_pereezd;
extern int konec[2];
extern int konec_dc;
extern int kot;
extern int KSK[skoko_stoek];
extern int last_day;
extern int left_up_left[14];
extern int left_up_right[14];
extern int m_m;
extern int maka;
extern int maket;
extern int Makst;
extern int mane;
extern int mane1;
extern int manevr;
extern int manevr1;
extern int manevro;
extern int manevro1;
extern int MAR_GOT[skoko_stoek];
extern int marazm;
extern int MAR_GOT[skoko_stoek];
extern int marshrut[skoko_stoek][3];
#ifdef VSP
extern int mark[VSP][2];
#endif
extern int markery[kol_OB_UPR][8];
extern int me_uk;
extern int menu_N;
extern int mes;
extern int mes1;
extern int mi_n;
extern int min;
extern int minu;
extern int misto;
extern int mlc[16];
extern int mls[16];
extern int mlu[15];
extern int mlv[15];
extern int mly[16];
extern int mlz[16];
extern int mo1;
extern int MODE_KN;
extern int modi;
extern int modi_new;
extern int monn_;
extern int mouse_down;
extern int MOUSE_ON;
extern int mouser;
extern int n;
extern int n_1;
extern int n_2;
extern int N_HELP;
extern int n_m;
extern int n_ob1;
extern int n_ob2;
extern int N_OGR;
extern int n_s;
#ifdef AVTOD
extern int N_strel[N_str];
#endif
extern int na;
extern int nach_avto;
extern int nach_marsh;
extern int nach0_marsh;
extern int nach_marsh_oper;
extern int nach0_marsh_oper;
extern int N_SOOB;
extern int NE;
extern int NEISPRAVEN;
extern int net_kontro;
extern int new_day;
extern int nikuda;
extern int NM;
extern int NOM_NEXT;
extern int nomer;
extern int NOMER_ARMA;
extern int NOMER_FOR_FR4;
extern int number;
extern int OB_PRER;
extern int OB_NEW;
extern int OB_NEKVIT;
extern int obnov;
extern int okno_otkaz;
extern int old_me;
extern int old_modi;
extern int old_punkt;
extern int oper;
extern int oppp;
extern int opred;
extern int oshibka;
extern int osya;
extern int ot;
extern int otkaz_sos[skoko_stoek];//флаг индикации отказа ТС соседней ПЭВМ
extern int otkaz_tu_sos[skoko_stoek];//флаг индикации отказа ТУ соседнейПЭВМ
extern unsigned int otl_soob;
extern int Otlog;
extern int OTV_KOM_0;
extern int OTV_KOM_1;
extern int otv_kom;
extern int otv_kom_oper;
extern int pat;
extern int pauza;
extern int perekluch;
extern int perezap;
extern int plo;
extern int plu;
extern int po_ma;
extern int podgr;
extern int podsvetka;
extern int point;
extern int pointer;
extern int poka;
extern int povt1;
extern int povt1;
extern int povt2;
extern int PRESS;
extern int priem_;
extern int PRIEM_FR4;
extern int PRKL;
extern int prohod_1;
extern int prorisovka;
extern int PROV_SCB;
extern int prv[skoko_stoek];
extern int punkt_N;
extern int pust_pvm;
extern unsigned int pusto;
extern int puti;
extern int qf;
extern int R;
extern int r;
extern int raz_raz;
extern int re;
extern int right_up_left[14];
extern int right_up_right[14];
extern int row;
extern int s1;
extern int s2;
extern int sbros;
extern int sbros1;
extern int schet;
extern int ScLk;
extern int se_c;
extern int se_co1;
extern int sek;
extern int seku;
extern int SEZON;
extern int speed_spdlp;
extern int symb_1;
extern int siv;
extern int soob_for_oper;
extern int spa;
extern int spdlp;
extern int spkont[UPR1+UPR2][5];
extern int spputi[PUT1+PUT2][5];
extern int spsig[SIG1+SIG2][5];
extern int spspu[UCH1+UCH2][5];
extern int spstr[STR1+STR2][5];
#if DOP1>0
extern int spdop[DOP1+DOP2][5];
#endif
extern int STACK;
extern int STAT;
extern int STATUS;
extern int STATUS_0;
extern int STATUS_1;
extern int STOP;
extern int STOP_TUMS[2];
extern int STOROG;
extern int str_i;
extern int strelki[5][3];
#ifdef kol_STY
extern int styki[kol_STY][3];
#endif
extern int t_koma[2];
extern int t_otpusk;
extern int t_pust[skoko_stoek];
extern int t_pust_dc;
extern int t_t;
extern int TELEUP;
extern int TEST_KANAL;
extern int TEST_N;
extern int test_tmp;
extern int the_kon;
extern int timer_set;
extern int TORMOZ;
extern int tst;
extern int tst_tst;
extern int TIME_ERROR_BBKP;
extern int ERROR_BBKP_0, ERROR_BBKP_1, ERROR_BBKP_2;
extern int ERROR_TUMS1_0,ERROR_TUMS1_1,ERROR_TUMS1_2;
extern int ERROR_TUMS2_0,ERROR_TUMS2_1,ERROR_TUMS2_2;
extern int TUMS_RABOT[skoko_stoek];
extern int u;
extern int ukaz_ot_dc;
extern int ukaz_out_spd;
extern int ukaz_spd;
extern int ukaz_trass;//числовой указатель объекта в массиве trassa
extern int uprav;
extern int USER_FONT;
extern int V;
extern int vib_ma;
extern int vibeg;
extern int vih_vopr;
extern int vozvr;
extern int vrem;
extern int vspz;
extern int vtora;
extern int vvod_dc;
extern int vvod_set;
extern int vvod_tums;
extern int vysv;
extern int vysvv;
extern int vzvod;
extern int was[WAZO];
extern int who_was;
extern int X_DC;
extern int X_kan[2];
extern int X_m;
extern int X_menu;
extern int X_m_old;
extern int X_NAL;
extern int X_time;
extern int xc;
extern int Y_DC;
extern int Y_kan[2];
extern int Y_m;
extern int Y_m_isk;
extern int Y_m_old;
extern int Y_NAL;
extern int Y_txt;
extern int yc;
extern int ye1;
extern int yearr_;
extern int zabeg;
extern int zafix;
extern int ZAGRUZKA;
extern int ZAHOD;
extern int zakr;
extern int zalet;
extern int zapr;
extern int zapret[skoko_stoek];
extern int zapret_sos[skoko_stoek];
extern int zapretvybora;
extern int zapret_otlad;
extern int ZAPROS_FR4;
extern int zapusk;
extern int ZAVIS;
extern int zdem_ps_pso;
extern int Zdem_vibor_marshruta;
extern int Zdem_vibor_opov;
extern int Zdem;
extern int ZONA_KURSORA;
extern int zvuk;
extern int zzzzx;
extern int OTMENA;
extern int OGRAN;
extern int MAK;
extern int AVAR;


//--------------------------
extern int ukaz_vyd[skoko_stoek];//указатели очередного байта выдачи в буфере BUF_OUT в ТУМС
extern int ukaz_zap[skoko_stoek];//указатели очередного байта записи данных в буфере BUF_OUT в ТУМС
extern int ukaz_read[skoko_stoek];//указатели очередного байта выдачи в буфере BUF_OUT в ТУМС
extern int ukaz_priem[skoko_stoek];//указатели очередного байта записи данных в буфере BUF_OUT в ТУМС
extern int N_RAZ[skoko_stoek];//счетчик числа повторений выдачи команд в ТУМС
extern int sboy_ts[skoko_stoek]; //счетчики сбоев канала ТС для ТУМС
extern int sboy_tu[skoko_stoek];//счетчики сбоев канала ТУ для ТУМС
extern int otkaz_ts[skoko_stoek];//индикаторы отказа канала ТС для ТУМС
extern int otkaz_tu[skoko_stoek];//индикаторы отказа канала управления ТУМС
//------------------------------------------------------
extern int ukaz_vyd_dc;//указатели очередного байта выдачи в ДЦ
extern int ukaz_zap_dc;//указатель очередного;байта записи из ДЦ 
extern int ukaz_read_dc;
extern int ukaz_priem_dc;
extern int ukaz_com_dc;//указатель на активный регистр приема команд ДЦ для передачи в ТУМС

extern int sboy_ts_dc;//счетчик сбоев канала ТС для ДЦ
extern int otkaz_ts_dc;//индикатор отказа канала ТС для ДЦ
extern int otkaz_tu_dc;//индикатор отказа канала управления ДЦ
extern int dc_not_plus;

//-------------------------------------------------------
extern unsigned char password[8],
adr_spdlp,
str_baz[32],
BEG_TIM[20],
BUF_OUT_SPD[75],
chudo[20],
com_bit[kol_VO],
END_TIM[20],
K64,
K32,
LEN_SPD,
NAME_FILE[17],
*NAME_STATION,
NOM_FUNC[4],
OBMEN_PVM,
OBMEN_PVM_OLD,
STRoka[40],
sbis[2],
D_kont[9],
otopri[kopo][50],
#ifdef KOL_SOO2
#if KOL_SOO1>KOL_SOO2
podgruppa[KOL_SOO1],
#else
podgruppa[KOL_SOO2],
#endif
#else
podgruppa[KOL_SOO1],
#endif
pako[kol_VO][32],
name_lamp[70][15], //наименование экранных ламп
//-------------------перемннные для обмена с ТУМС и ДЦ
SYMBOL[skoko_stoek],//массив приемных ячеек для записи очередного
										//символа из порта вводаBUF_OUT[skoko_stoek][SIZE_BUF_OUT],//кольцевые буферы вывода в ТУМС
BUF_IN[skoko_stoek][SIZE_BUF_PRIEM],//кольцевые буферы приема из ТУМС
BUF_OUT[skoko_stoek][SIZE_BUF_OUT], //кольцевые буферы вывода в ТУМС
REG_INFO[skoko_stoek][N_BYTE],//массивы регистров сообщений из ТУМС
REG_COM_TUMS[skoko_stoek][N_BYTE],//массив регистров команд для передачи в ТУМС
REG_PODT_TUMS[skoko_stoek][N_BYTE],
REG_OT[skoko_stoek][6] //массив регистров ответов для передачи в ТУМС
#ifdef DISPETCHER
,
	REG_COM_DC[N_BYTE],//массив регистров команд ДЦ для передачи в ТУМС
	REG_RON[N_BYTE],
	REG_INF_DC[KOL_SOO][N_BYTE+2],//массивы регистров сообщений ТУМС для передчи в ДЦ
//массив регистров подтверждений из ТУМС
//-------------------перемннные для обмена с ДЦ
	BUF_OUT_DC[SIZE_BUF_OUT_DC],//кольцевой буфер выдачи данных для ДЦ
	BUF_IN_DC[SIZE_BUF_PRIEM_DC],//кольцевой буфер приема из ДЦ
	REG_OT_DC[6],//массив регистров ответов ДЦ для передачи в ТУМС
	REG_PODT_DC[skoko_stoek][6];//массив регистров подтверждений от ТУМС для передачи в ДЦ
#else
;
#endif
//--------------------------------------
//---------------------------------------
extern unsigned char
		IT_IS_COM_PVM,
		IT_IS_FR4,
		IT_IS_INF,
		PERED_PVM,
		REG_INF[4], BUF_INF[4],     REG_KVIT_INF[3],KVIT_INF,POVTOR_INF,
		REG_COM[N_BYTE],
		BUF_COM_PVM[N_BYTE+1],
		REG_KVIT_COM[3],KVIT_COM,POVTOR_COM,
		REG_FR4[6], BUF_FR4[6],     REG_KVIT_FR4[3],KVIT_FR4,POVTOR_FR4,
		soobs[12];

extern int UKAZ_INF,END_INF,
		UKAZ_FR4,END_FR4,
		UKAZ_COM,END_COM,
		UKAZ_KVIT_INF,END_KVIT_INF,
		UKAZ_KVIT_FR4,END_KVIT_FR4,
		UKAZ_KVIT_COM,END_KVIT_COM,
		MY_INF,MY_COM,MY_FR4,UKAZ_ZAP_OUT,UKAZ_VYVOD, UKAZ_PRIEMA_PVM;
extern unsigned char BUF_OUT_PVM[MAX_SIZE_OUT];
extern unsigned char BUF_IN_PVM[MAX_SIZE_IN];
extern unsigned char TIME[11];
//-----------------------------

#ifdef MOUSE
	extern int mouse_mark[mouse_all][3];
	extern unsigned char
	line_gor_kurs[12],
	line_vert_kurs[40],
	line_old_gor_kurs[12],
	line_old_vert_kurs[40];
#endif

//---------------------------------------------------------------------
extern unsigned char H_0,H_1,M_0,M_1;
extern unsigned int
fr4[kol_VO][3],
ukazat[138],//массив для слов описывающих изображение указателя-курсора trassa[100],//массив слов описания трассы для задаваемого маршрута
ml_ba,
ml_bas,
st_ba,
st_bas,
ml_ba2,
st_ba2,
ml_bap,
st_bap,
ml_bab,
st_bab,
SP_STR[10][5],
COD_DN,
len,
TEST_SPAR,
TIMER_N[25][3];
extern unsigned char TEST_SOOB[skoko_stoek][7];
//--------------------------------
extern int zvu_tabl[K_oo][10];
extern unsigned int cvt[K_oo][10];
extern unsigned char
buf[skoko_stoek][N_BYTE],
buf_ko[N_BYTE],
buf_ko_dc[skoko_stoek][N_BYTE],
buf_ko_p[N_BYTE],
buf_ko_mm[N_BYTE],
buf_ko_vsp[N_BYTE+1],
buf_zap_disk[N_BYTE],
bu[skoko_stoek][N_BYTE],
buf_ko_p2[N_BYTE],
KOMANDA_TUMS[skoko_stoek][15],
MARSH_VYDAN[skoko_stoek],
MYTHX[skoko_stoek],
MYTHX_TEC[skoko_stoek];
/**********************/
extern struct  so_so
{
	char  lex[60];
	int COlo;
};
//--------------------------------------
extern struct elem_marsh
{
	int N_sek;
	int Beg_s;
	int End_s;
	int Flag;
	int N_raz;
	int Elems[K_EL];
}Avto_el[4];
//-----------------------------------------
extern struct skvoz_marsh
{
 int B1_s,E1_s,B2_s,E2_s;
} Skvo[4];

extern struct skv_m
{
 int B,E;
} Skv[4];
extern struct inversia
{
	int obj, bit;
}invers[10];
//------------------------------
#ifdef AVTOVOZVRAT
extern struct avst_
{
  int  N_sst,
      b0,
      b1,
      N_spu1,
      N_spu2,
			MSP1,
			MSP2,
			Sx,
			Sy,
			Sra,
			LU,
			Fl_kom;
} coAV[Kol_avstr];
#endif

extern struct zvuk
{
	unsigned long time_begin;
	int dlit;
	int tip;
}zvuk_sig;
extern struct TRAS
{
	unsigned int object;
	unsigned char tip;
	unsigned char stoyka;
	unsigned char podgrup;
	unsigned char kod_bit;
}TRASSA[100],TRASSA0[100],TRASSA_RIS[100];

extern struct MARS_ALL
{
	char KMND;
	int NACH;
	int NACH0;
	int END;
	int NSTR;
	unsigned long POL_STR;
	int KOL_STR[skoko_stoek];
	int STREL[skoko_stoek][20];
	int SIG[skoko_stoek][10];
	int SP_UP[skoko_stoek][10];
	unsigned char SOST;
	int STOYKA[skoko_stoek];
}MARSHRUT_ALL[skoko_stoek*3];

extern struct MARS_ST
{
	char NEXT_KOM[14];
	char NUM;
	unsigned char SOST;
	time_t T_VYD;
	long T_MAX;
}MARSHRUT_ST[skoko_stoek][MARS_STOY];

extern void *f22,*saus,*qfr,*wew,*fq,*fai,*f_o;
extern union REGS regs,rg;
//extern FILE *qf,*fq;

extern int
SMI,
errorcode,
b_day,
b_h0,
b_m,
b_m0,
b_mon,
b_mon0,
b_mon1,
b_s,
b_s0,
b_second,
b_u,
bb_d,
bb_h,
bb_m,
bb_mi,
bb_y,
box_uk,
bq,
bqq,
col,
dodo,
dubl,
durdom,
ee,
end_,
fing,
flpass,
goga,
hop,
ismena_,
no,
nome,
old,
opred_new,
opred1,
opred_old,
poshag,
PRICHINA_SN_AVTO[2],
sss,
zahod,
zapr;

extern unsigned char
data[30];

extern unsigned date, timer;

