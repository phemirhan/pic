
#include <16f877.h>     // Kullan�lacak denetleyicinin ba�l�k dosyas� tan�t�l�yor.

#device ADC=10  // 10 bitlik ADC kullan�laca�� belirtiliyor.

// Denetleyici konfig�rasyon ayarlar�
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD

#use delay (clock=2000000) // Gecikme fonksiyonu i�in kullan�lacak osilat�r frekans� belirtiliyor.

#use fast_io(a) //Port y�nlendirme komutlar� A portu i�in ge�erli

#define use_portb_lcd TRUE   // LCD B portuna ba�l�

#include <lcd.c>   // lcd.c dosyas� tan�t�l�yor
float isi;   // isi isimli de�i�kenin float cinsinde tan�mlanmas�
unsigned long int bilgi; // ��aretsiz 16 bitlik tam say� tipinde de�i�ken tan�mlan�yor
float voltaj,sicaklik;   // ondal�kl� tipte de�i�kenler tan�t�l�yor

//********** ANA PROGRAM FONKS�YONU*******

void main ( )
{
   isi =0; // isi de�i�kenini 0 yap�yor
   set_tris_d(0b11111000); // d portunu giri� ve ��k��a ayarl�yor d0, d1, d2 giri� di�erleride ��k�� 
   setup_psp(PSP_DISABLED);        // PSP birimi devre d���
   setup_timer_1(T1_DISABLED);     // T1 zamanlay�c�s� devre d���
   setup_timer_2(T2_DISABLED,0,1); // T2 zamanlay�c�s� devre d���
   setup_CCP1(CCP_OFF);            // CCP1 birimi devre d���
   setup_CCP2(CCP_OFF);            // CCP2 birimi devre d���

   set_tris_a(0x01);  // RA0 Giri� olarak y�nlendiriliyor
	
   setup_adc(adc_clock_div_32);   // ADC clock frekans� fosc/32
   setup_adc_ports(AN0); //RA0/AN0 giri�i analog

   lcd_init();   // LCD haz�r hale getiriliyor

   set_adc_channel(0);   // RA0/AN0 ucundaki sinyal A/D i�lemine tabi tutulacak
   delay_us(20);         // Kanal se�iminde sonra bu bekleme s�resi verilmelidir
   printf(lcd_putc,"\fIC ISI="); // LCD'ye yaz� yazd�r�l�yor
 
   while(1)   // sonsuz d�ng�
   {
		 
	 bilgi=read_adc();  // ADC sonucu okunuyor ve bilgi de�i�kenine aktar�l�yor
		if (input(PIN_D0)==1) // BUTON 1 BASILI MI ? 
		{
		 isi += 2 ; // isi ya +2 ekliyor
		 while(input(PIN_D0)==1); // butonun b�rak�lmas�n� bekliyoruz
		  delay_ms(25); // debounce s�resi, giri� g�r�lt�s�n� kald�rmas� i�in ekliyoruz
		  
		}
		if (input(PIN_D2)==1&& isi > 0) // BUTON 2 BASILI MI ? ve isi 0 dan b�y�k m�?
		{
		 isi -= 2 ; // isi dan 2 ��kart�yor
		 while(input(PIN_D2)==1); // butonun b�rak�lmas�n� bekliyoruz
		  delay_ms(25); // debounce s�resi, giri� g�r�lt�s�n� kald�rmas� i�in ekliyoruz
		}
		
     // voltaj=(0.0048828125*bilgi)*1000;   // Dijitale �evirme i�lemine u�rayan sinyalin mV olarak gerilimi hesaplan�yor
     //sicaklik=(voltaj/10)+2;    // Her 10mV'ta 1 derece artma
	  sicaklik=bilgi; // sicaklik=bilgi  
      lcd_gotoxy(8,1); // �mle� 1. sat�r 8.s�tunda
      printf(lcd_putc,"%3.0f'C",sicaklik); // LCD'ye s�cakl�k de�eri yazd�r�l�yor
	 
      // G�sterilecek ondal�kl� de�erin toplam basamak say�s� 5 olacak.
      // G�sterilecek de�erin ondal�kl� k�sm� ise 1 basamak olacak. %5.1f
      // komutu ile bu i�lemler yapt�r�lm��t�r.
	    lcd_gotoxy(1,2); // �mle� 1. sat�r 2.s�tunda
	  printf(lcd_putc,"AYAR. ISI="); // LCD'ye AYAR. ISI de�erini yazd�r�yor
	  lcd_gotoxy(12,2); // �mle� 12. sat�r 2.s�tunda
	 printf( lcd_putc,"%3.0f", isi); // LCD'ye s�cakl�k de�eri yazd�r�l�yor
	 if(sicaklik < isi) // isi sicakliktan b�y�k m�?
	 {
	  output_high(PIN_D7); // D7 pinini 1 yapar
	 }
	 else
	 {
		 output_low(PIN_D7); // D7 pinini 0 yapar
	 }
	   
      delay_ms(100); // 100 msn gecikme
   }
}


