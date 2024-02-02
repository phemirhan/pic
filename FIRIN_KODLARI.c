
#include <16f877.h>     // Kullanýlacak denetleyicinin baþlýk dosyasý tanýtýlýyor.

#device ADC=10  // 10 bitlik ADC kullanýlacaðý belirtiliyor.

// Denetleyici konfigürasyon ayarlarý
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD

#use delay (clock=2000000) // Gecikme fonksiyonu için kullanýlacak osilatör frekansý belirtiliyor.

#use fast_io(a) //Port yönlendirme komutlarý A portu için geçerli

#define use_portb_lcd TRUE   // LCD B portuna baðlý

#include <lcd.c>   // lcd.c dosyasý tanýtýlýyor
float isi;   // isi isimli deðiþkenin float cinsinde tanýmlanmasý
unsigned long int bilgi; // Ýþaretsiz 16 bitlik tam sayý tipinde deðiþken tanýmlanýyor
float voltaj,sicaklik;   // ondalýklý tipte deðiþkenler tanýtýlýyor

//********** ANA PROGRAM FONKSÝYONU*******

void main ( )
{
   isi =0; // isi deðiþkenini 0 yapýyor
   set_tris_d(0b11111000); // d portunu giriþ ve çýkýþa ayarlýyor d0, d1, d2 giriþ diðerleride çýkýþ 
   setup_psp(PSP_DISABLED);        // PSP birimi devre dýþý
   setup_timer_1(T1_DISABLED);     // T1 zamanlayýcýsý devre dýþý
   setup_timer_2(T2_DISABLED,0,1); // T2 zamanlayýcýsý devre dýþý
   setup_CCP1(CCP_OFF);            // CCP1 birimi devre dýþý
   setup_CCP2(CCP_OFF);            // CCP2 birimi devre dýþý

   set_tris_a(0x01);  // RA0 Giriþ olarak yönlendiriliyor
	
   setup_adc(adc_clock_div_32);   // ADC clock frekansý fosc/32
   setup_adc_ports(AN0); //RA0/AN0 giriþi analog

   lcd_init();   // LCD hazýr hale getiriliyor

   set_adc_channel(0);   // RA0/AN0 ucundaki sinyal A/D iþlemine tabi tutulacak
   delay_us(20);         // Kanal seçiminde sonra bu bekleme süresi verilmelidir
   printf(lcd_putc,"\fIC ISI="); // LCD'ye yazý yazdýrýlýyor
 
   while(1)   // sonsuz döngü
   {
		 
	 bilgi=read_adc();  // ADC sonucu okunuyor ve bilgi deðiþkenine aktarýlýyor
		if (input(PIN_D0)==1) // BUTON 1 BASILI MI ? 
		{
		 isi += 2 ; // isi ya +2 ekliyor
		 while(input(PIN_D0)==1); // butonun býrakýlmasýný bekliyoruz
		  delay_ms(25); // debounce süresi, giriþ gürültüsünü kaldýrmasý için ekliyoruz
		  
		}
		if (input(PIN_D2)==1&& isi > 0) // BUTON 2 BASILI MI ? ve isi 0 dan büyük mü?
		{
		 isi -= 2 ; // isi dan 2 çýkartýyor
		 while(input(PIN_D2)==1); // butonun býrakýlmasýný bekliyoruz
		  delay_ms(25); // debounce süresi, giriþ gürültüsünü kaldýrmasý için ekliyoruz
		}
		
     // voltaj=(0.0048828125*bilgi)*1000;   // Dijitale çevirme iþlemine uðrayan sinyalin mV olarak gerilimi hesaplanýyor
     //sicaklik=(voltaj/10)+2;    // Her 10mV'ta 1 derece artma
	  sicaklik=bilgi; // sicaklik=bilgi  
      lcd_gotoxy(8,1); // Ýmleç 1. satýr 8.sütunda
      printf(lcd_putc,"%3.0f'C",sicaklik); // LCD'ye sýcaklýk deðeri yazdýrýlýyor
	 
      // Gösterilecek ondalýklý deðerin toplam basamak sayýsý 5 olacak.
      // Gösterilecek deðerin ondalýklý kýsmý ise 1 basamak olacak. %5.1f
      // komutu ile bu iþlemler yaptýrýlmýþtýr.
	    lcd_gotoxy(1,2); // Ýmleç 1. satýr 2.sütunda
	  printf(lcd_putc,"AYAR. ISI="); // LCD'ye AYAR. ISI deðerini yazdýrýyor
	  lcd_gotoxy(12,2); // Ýmleç 12. satýr 2.sütunda
	 printf( lcd_putc,"%3.0f", isi); // LCD'ye sýcaklýk deðeri yazdýrýlýyor
	 if(sicaklik < isi) // isi sicakliktan büyük mü?
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


