#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct takim_bilgileri
{
    int   takim_kodu;
    char  takim_adi[21];
    int   galibiyet_say;
    int   beraberlik_say;
    int   maglubiyet_say;
    int   attigi_gol;
    int   yedigi_gol;
};

struct oyuncular
{
    int   forma_numarasi;
    char  ad_soyad[21];
    int   attigi_gol;
};

struct skorlar
{
    int   hafta_no;
    int   ev_sahibi_kod;
    int   misafir_kod;
    int   ev_sahibi_gol_sayisi;
    int   misafir_gol_sayisi;
};

typedef struct takim_bilgileri takim_tipi;
typedef struct oyuncular oyuncu_tipi;
typedef struct skorlar skor_tipi;

int menu();
int hataKontrol(int ,int );
void yeniMacEkle(FILE *);
void takimDurumuVeFutbolcuListele(FILE *);
void takimDurumuVeMaclariListele(FILE *);
void puanCetveli(FILE *);
void takimRekorlari(FILE *);
void deplasmanRekoru(FILE *);
void haftalikMacSonuclari(FILE *);
void futbolcuRekoru(FILE *);
void ligDisindanTransfer(FILE *);
void ligIcindenTransfer(FILE *);
void ligDisinaTransfer(FILE *);
takim_tipi takimAra(FILE *);
oyuncu_tipi oyuncuAra(FILE *,int );

/////////////////////
// Main fonksiyonu //
/////////////////////
/**
  * Main fonksionu takimlar dosyasini acar ve hata kontrolu yapar. Eger dosya acilmasinda hata varsa program sonlanir.
  * Dosya dogru acilirsa menu'yu cagirir ve girilen degere gore kullanicinin istedigi fonksiyonu cagirir
  * Kullanici programdan cikmak icin 12 girmelidir
  */
int main()
{
    int secim;
    FILE *takim_ptr = fopen("takimlar.dat","rb+");/* Takimlar dosyasi rb+ modunda aciliyor. Binary modda hem reading hem de writing yapilacak */
    system("color 3f");
    if(takim_ptr == NULL)/* Eger acilamazsa hata yazdiriyoruz ve program sonlaniyor */
    {
        printf("Takimlar dosyasi acilamadi!\n");
    }
    else
    {
        do
        {
            switch(secim = menu())/* Takimlar dosyasi acildigi icin menu islemlerine geciliyor */
            {
            /* Acilan takimlar dosyasi butun alt fonksionlara parametre olarak gonderiliyor */
                case 1:yeniMacEkle(takim_ptr);break;
                case 2:takimDurumuVeFutbolcuListele(takim_ptr);break;
                case 3:takimDurumuVeMaclariListele(takim_ptr);break;
                case 4:puanCetveli(takim_ptr);break;
                case 5:takimRekorlari(takim_ptr);break;
                case 6:deplasmanRekoru(takim_ptr);break;
                case 7:haftalikMacSonuclari(takim_ptr);break;
                case 8:futbolcuRekoru(takim_ptr);break;
                case 9:ligDisindanTransfer(takim_ptr);break;
                case 10:ligIcindenTransfer(takim_ptr);break;
                case 11:ligDisinaTransfer(takim_ptr);break;
                case 12: break;
            }
        }while(secim != 12);/* Secim degeri 12 ise programdan cikiliyor */
    }
    fclose(takim_ptr);/* Takimlar dosyasi kapatiliyor */
    return 0;/* Program sonlaniyor */
}
/////////////////////
// Menu fonksiyonu //
/////////////////////
/**
  * Menuyu listeleyip kullaninin bir secenek secmesini isteyen fonksyion.
  * Parametre olarak bir sey almaz. Hata kontolu yaptýktan sonra kullanicinin girdigi degeri dondurur
  */
int menu()
{
    system("cls");
    printf("\n-------------------------------------------------------------------------------\n");
    printf("|01|                           Yeni mac skoru ekle                         |01|\n");
    printf("|02|                 Bir takimi ve futbolcularini listele                  |02|\n");
    printf("|03|                   Bir takimi ve maclarini listele                     |03|\n");
    printf("|04|                        Puan cetvelini listele                         |04|\n");
    printf("|05|         En cok gol atan ve en cok gol yiyen takimlari listele         |05|\n");
    printf("|06|             Deplasmanda en cok gol atan takimi listele                |06|\n");
    printf("|07|                Bir haftanin mac skorlarini listele                    |07|\n");
    printf("|08|             Ligde en cok gol atan futbolcunun listele                 |08|\n");
    printf("|09|            Lig disindan yeni bir futbolcunu transfer et               |09|\n");
    printf("|10|                 Lig icinden bir futbolcu transfer et                  |10|\n");
    printf("|11|                   Lig disina futbolcu transfer et                     |11|\n");
    printf("|12|                                 Cikis                                 |12|\n");
    printf("-------------------------------------------------------------------------------\n\n");
    printf("   Secim : ");

    return hataKontrol(1,12);
}
/////////////////////////////
// Hata kontrol fonksiyonu //
/////////////////////////////
/**
  * Kullanicinin girdigi deger, istenilen aralikta mi diye bakar ve degilse dogru oluncaya kadar deger girmesini saglar.
  * Parametre olarak alt ve ust limitleri alir. Hata kontolu yaptýktan sonra kullanicinin girdigi degeri dondurur
  */
int hataKontrol(int alt,int ust)
{
    int say;
    do
    {
        scanf("%d",&say);
        if(say < alt || say > ust)
        {
            printf("\nHATA! %d ile %d arasinda bir deger girisi yapin!\n\n",alt,ust);
        }
    }
    while(say < alt || say > ust);

    return say;
}
/////////////////////////////////////////////////////
// (MENU 1) - Kayitlara yeni mac ekleme fonksiyonu //
/////////////////////////////////////////////////////
/**
  * Kayitlara yeni bir mac eklenmesini saglayan fonksiyon. Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Futbolcular ve Skorlar dosyalari acilir. Kullanicidan macla ilgili bilgiler girilmesi istenir. Ancak butun girilen degerler icin hata kontolu yapilir
  */
void yeniMacEkle(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb+");/* Futbolcular dosyasi rb+ modunda aciliyor. Binary modda hem okuma hem yazma yapilacak */
    FILE *skor_ptr = fopen("skorlar.txt","a+");/* Skorlar dosyasi a+ modunda aciliyor. Update yapilacak */

    skor_tipi bir_skor;
    oyuncu_tipi golcu_oyuncu;
    takim_tipi ev_sahibi_takim,misafir_takim;

    int sayac,gol;
    char cikis;

    system("cls");

    if(futbolcu_ptr == NULL)/* Futbolcular dosyai acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else if(skor_ptr == NULL)/* Skorlar dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nSkorlar dosyasi acilamadi!\n");
    }
    else
    {
        printf("Yeni bir mac ekleniyor...\n");
        do/* Dosyalar acildiktan sonra kullanicinin isterse tekrar takrar mac ekleyebilmesini saglayan donguye giriliyor */
        {
            printf("Hafta numarasini giriniz.\n");/* Oynanan macin hafta numarasini aliniyor */
            bir_skor.hafta_no = hataKontrol(1,99);

            printf("\nEv sahibi takimin kodunu giriniz.\n");/* Ev sahibi takim aliniyor */
            ev_sahibi_takim = takimAra(takim_ptr);

            printf("Misafir takimin kodunu giriniz.\n");/* Misafir takim aliniyor */
            misafir_takim = takimAra(takim_ptr);

            bir_skor.ev_sahibi_kod = ev_sahibi_takim.takim_kodu;
            bir_skor.misafir_kod = misafir_takim.takim_kodu;

            printf("\nEv sahibi takimin attigi gol sayisini giriniz.\n");/* Ev sahibi takimin attigi gol sayisi aliniyor */
            bir_skor.ev_sahibi_gol_sayisi = hataKontrol(0,99);

            printf("Misafir takimin attigi gol sayisini giriniz.\n");/* Misafir takimin attigi gol sayisi aliniyor */
            bir_skor.misafir_gol_sayisi = hataKontrol(0,99);
            /* Mac sonucu elde edildi, skorlar dosyasina yazdiriliyor */
            fprintf(skor_ptr,"%d %d %d %d %d\n",bir_skor.hafta_no,bir_skor.ev_sahibi_kod,bir_skor.misafir_kod,
                    bir_skor.ev_sahibi_gol_sayisi,bir_skor.misafir_gol_sayisi);

            sayac = 0;/* Sayac, takimda atilan gol sayini tutacak, toplam gol sayisindan fazla olmasina gore donguden cikilacak */
            while(sayac < bir_skor.ev_sahibi_gol_sayisi)/* Donguden, sayac toplam gol sayisindan fazla olunca cikilacak */
            {
                printf ("\nEv sahibi takimdan gol atan oyuncunun forma numarasini giriniz.\n");
                golcu_oyuncu = oyuncuAra(futbolcu_ptr,bir_skor.ev_sahibi_kod);

                /* Golu atan oyuncu alindiktan sonra futbolcunun macta attigi gol sayisi isteniyor */
                if(bir_skor.ev_sahibi_gol_sayisi-sayac == 1)/* Zaten futbolcunun atabilecegi gol sayisi 1 ise sorulmadan geciliyor */
                {
                    gol = 1;
                }
                else
                {
                    printf ("Futbolcunun attigi gol sayisini giriniz.\n");
                    gol = hataKontrol(1,bir_skor.ev_sahibi_gol_sayisi-sayac);/* Kullanici - futbolcunun attigi gol sayisi olarak - en fazla ,(macta attiklari toplam gol sayisi - suana kadar attileri gol sayisi) girebilir */
                }
                golcu_oyuncu.attigi_gol+=gol;/* Futbolcunun attigi gol sayisi arttiliyor */

                fseek(futbolcu_ptr,(bir_skor.ev_sahibi_kod*100+golcu_oyuncu.forma_numarasi-1) * sizeof(oyuncu_tipi),SEEK_SET);
                fwrite(&golcu_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

                sayac += gol;/* Futbolcularin attiklari gol sayisini tutan sayac da arttiriliyor */
            }

            /* Misafir takima geciliyor */
            sayac = 0;/* Sayac, takimda atilan gol sayini tutacak, toplam gol sayisindan fazla olmasina gore donguden cikilacak */
            while(sayac < bir_skor.misafir_gol_sayisi)/* Donguden, atilan gol sayisi toplam gol sayisindan fazla olunca cikilacak */
            {
                printf ("\nMisafir takimdan gol atan oyuncunun forma numarasini giriniz.\n");
                golcu_oyuncu = oyuncuAra(futbolcu_ptr,bir_skor.misafir_kod);
                /* Golu atan oyuncu alindiktan sonra futbolcunun macta attigi gol sayisi isteniyor */
                if(bir_skor.misafir_gol_sayisi-sayac == 1)/* Zaten futbolcunun atabilecegi gol sayisi 1 ise sorulmadan geciliyor */
                {
                    gol = 1;
                }
                else
                {
                    printf ("Futbolcunun attigi gol sayisini giriniz.\n");
                    gol = hataKontrol(1,bir_skor.misafir_gol_sayisi-sayac); /* Kullanici - futbolcunun attigi gol sayisi olarak - en fazla ,(macta attiklari toplam gol sayisi - suana kadar attileri gol sayisi) girebilir */
                }
                golcu_oyuncu.attigi_gol+=gol;/* Futbolcunun attigi gol sayisi arttiliyor */

                fseek(futbolcu_ptr,(bir_skor.misafir_kod*100+golcu_oyuncu.forma_numarasi-1) * sizeof(oyuncu_tipi),SEEK_SET);
                fwrite(&golcu_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

                sayac += gol;/* Futbolcularin attiklari gol sayisini tutan sayac da arttiriliyor */
            }
            /* Macla ilgili bilgiler alindi. Simdi guncellemelere geciliyor */
            /* Mac sonucu(G-B-M) belirleniyor */
            if(bir_skor.ev_sahibi_gol_sayisi > bir_skor.misafir_gol_sayisi) /* Ev sahibi takimin gol sayisi fazla ise ev sahibi takim galip oluyor */
            {
                ev_sahibi_takim.galibiyet_say++;
                misafir_takim.maglubiyet_say++;
            }
            else if(bir_skor.ev_sahibi_gol_sayisi < bir_skor.misafir_gol_sayisi)/* Degilse ve misafir takimin gol sayisi fazla ise ev sahibi takim galip oluyor */
            {
                ev_sahibi_takim.maglubiyet_say++;
                misafir_takim.galibiyet_say++;
            }
            else/* Ikisi de degilse, goller esittir, mac berabere bitmistir */
            {
                ev_sahibi_takim.beraberlik_say++;
                misafir_takim.beraberlik_say++;
            }
            /* Takimlarin attiklari ve yedikleri gol sayilari guncelleniyor */
            ev_sahibi_takim.attigi_gol += bir_skor.ev_sahibi_gol_sayisi;
            misafir_takim.attigi_gol += bir_skor.misafir_gol_sayisi;
            ev_sahibi_takim.yedigi_gol += bir_skor.misafir_gol_sayisi;
            misafir_takim.yedigi_gol += bir_skor.ev_sahibi_gol_sayisi;

            /* Kayitlar guncelleniyor */
            fseek(takim_ptr,(bir_skor.ev_sahibi_kod-1) * sizeof(takim_tipi),SEEK_SET);
            fwrite(&ev_sahibi_takim,sizeof(takim_tipi),1,takim_ptr);

            fseek(takim_ptr,(bir_skor.misafir_kod-1) * sizeof(takim_tipi),SEEK_SET);
            fwrite(&misafir_takim,sizeof(takim_tipi),1,takim_ptr);

            printf("\n\n%s (%d) - (%d) %s maci basariyla sisteme eklenmistir!\n",ev_sahibi_takim.takim_adi,bir_skor.ev_sahibi_gol_sayisi,
                   bir_skor.misafir_gol_sayisi,misafir_takim.takim_adi);

            printf("\nYeni bir mac skoru eklemek ister misiniz?(e/E-h/H)\n");
            do/* Yeni bir mac eklenecegi sorgulaniyor.  */
            {
                fflush(stdin);
                cikis = getchar();
                if(cikis != 'e' && cikis != 'E' && cikis != 'h' && cikis != 'H')
                {
                    printf("Lutfen dogru bir karakter giriniz!\n");
                }
            }
            while(cikis != 'e' && cikis != 'E' && cikis != 'h' && cikis != 'H');

        }
        while(cikis != 'h' && cikis != 'H');/* Kullanici hayir derse fonksiyon sonlaniyor */
    }
    fclose(futbolcu_ptr);/* Futbolcular dosyasi kapatiliyor */
    fclose(skor_ptr);/* Skorlar dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
/////////////////////////////////////////////////////////////
// (MENU 2) - Takim ve futbolcularini listeleme fonksiyonu //
/////////////////////////////////////////////////////////////
/**
  * Adi verilen takimin bilgilerinin yazdirilmesi ve oyuncularinin listelenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Futbolcular dosyasi acilir. Takimin adi alinir. Ancak hata kontrolu yapilir.
  * Futbolcular dosyasindan oyuncular okunur ve ekrana yazdirilir
  */
void takimDurumuVeFutbolcuListele(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb");/* Futbolcular dosyasi rb modunda aciliyor. Binary modda sadece okuma yapilacak */

    takim_tipi  eslesen_takim;
    oyuncu_tipi bir_oyuncu;

    int i,flag = 0;
    char str[21],upp_str[21];

    system("cls");

    if(futbolcu_ptr == NULL)/* Futbolcular dosyai acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else
    {
        printf("Bir takim ve futbolculari sorgulaniyor...\n");
        printf("Bilgilerini gormek isteginiz takimin adini giriniz.\n");/* Takimin adi isteniyor */
        do
        {
            *str = "";
            fflush(stdin);
            gets(str);
            for(i=0; str[i] != '\0'; i++)/* Takim isimleri, dosyada buyuk harflerle tutuldugundan, kullanicinin girdigi string buyuk harflere cevriliyor */
            {
                upp_str[i] = toupper(str[i]);
            }
            upp_str[i] = '\0';

            rewind(takim_ptr);/* Takimlar dosyasinin en basina geliniyor */
            for(i=0; i<100; i++)/* Dosyanin sonuna kadar okuma yapiliyor */
            {
                fread(&eslesen_takim,sizeof(takim_tipi),1,takim_ptr);
                if (strcmp(eslesen_takim.takim_adi,upp_str) == 0)/* Eger ki yapilan okuma sirasinda aradigimiz takim rastgelirse donguden cikiliyor */
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)/* Aranan takim bulunamamissa yoksa hata veriliyor */
            {
                printf("HATA! Boyle bir takim bulunmamaktadir!\n");
            }
        }
        while(flag == 0);/* Donguden, kullacini, dosyada bulunnan bir takim girinceye kadar cikilmiyor */

        /* Takim okunduktan sonra takim bilgileri yazdiriliyor */
        printf("\n\n\tTakim Adi            OO GG BB MM AA YY AVR Puan\n");
        printf("\t-------------------- -- -- -- -- -- -- --- ----\n");
        printf("\t%-20s %2d %2d %2d %2d %2d %2d %3d %3d\n\n",eslesen_takim.takim_adi,eslesen_takim.maglubiyet_say+eslesen_takim.beraberlik_say+eslesen_takim.galibiyet_say,
               eslesen_takim.galibiyet_say,eslesen_takim.beraberlik_say,eslesen_takim.maglubiyet_say,eslesen_takim.attigi_gol,eslesen_takim.yedigi_gol,
               eslesen_takim.attigi_gol-eslesen_takim.yedigi_gol,eslesen_takim.galibiyet_say*3+eslesen_takim.beraberlik_say);

        printf("\t\tForma No Ad Soyad             AA\n");
        printf("\t\t-------- -------------------- --\n");

        fseek(futbolcu_ptr,(eslesen_takim.takim_kodu*100)*sizeof(oyuncu_tipi),SEEK_SET);/* Futbolcular dosyasinda, o takimin oyuncularinin bulundugu yerin basina geliniyor  */
        for(i=0; i<100; i++)/* Takimin sahip olabilecegi futbolcu sayisi kadar, futbolcular dosyasindan okuma yapiliyor */
        {
            fread(&bir_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);
            if(bir_oyuncu.forma_numarasi != -1)/* Eger ki oyuncunun forma numarasi -1 degilse boyle bir futbolcu vardir. Yazdiriliyor */
            {
                printf("\t\t%5d    %-20s %2d\n",bir_oyuncu.forma_numarasi,bir_oyuncu.ad_soyad,bir_oyuncu.attigi_gol);
            }
        }
    }
    fclose(futbolcu_ptr);/* Futbolcular dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
////////////////////////////////////////////////////////
// (MENU 3) - Takim ve maclarini listeleme fonksiyonu //
////////////////////////////////////////////////////////
/**
  * Kodu verilen takimin bilgilerinin yazdirilmesi ve oyadigi maclarin listelenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Futbolcular ve Skorlar dosyasi acilir. Takimin kodu alinir. Hata kontrolu yapilir
  * Skorlar dosyasindan maclar okunur ve ekrana yazdirilir
  */
void takimDurumuVeMaclariListele(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb+");/* Futbolcular dosyasi rb modunda aciliyor. Binary modda sadece okuma yapilacak */
    FILE *skor_ptr = fopen("skorlar.txt","r");/* Skorlar dosyasi r modunda aciliyor. Sadece okuma yapilacak */

    takim_tipi  bir_takim,eslesen_takim;
    skor_tipi bir_skor;

    system("cls");

    if(futbolcu_ptr == NULL)/* Futbolcular dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else if(skor_ptr == NULL)/* Skorlar dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nSkorlar dosyasi acilamadi!\n");
    }
    else
    {
        printf("Bir takim ve maclari sorgulaniyor...\n");
        printf("Bilgilerini gormek isteginiz takimin kodunu giriniz.\n");/* Takim kodu isteniyor */

        eslesen_takim = takimAra(takim_ptr);
        /* Takim alindi, bilgileri yazdiriyor */
        printf("\n\n\tTakim Adi            OO GG BB MM AA YY AVR Puan\n");
        printf("\t-------------------- -- -- -- -- -- -- --- ----\n");
        printf("\t%-20s %2d %2d %2d %2d %2d %2d %3d %3d\n\n",eslesen_takim.takim_adi,eslesen_takim.maglubiyet_say+eslesen_takim.beraberlik_say+eslesen_takim.galibiyet_say,
               eslesen_takim.galibiyet_say,eslesen_takim.beraberlik_say,eslesen_takim.maglubiyet_say,eslesen_takim.attigi_gol,eslesen_takim.yedigi_gol,
               eslesen_takim.attigi_gol-eslesen_takim.yedigi_gol,eslesen_takim.galibiyet_say*3+eslesen_takim.beraberlik_say);

        printf("\n\t             Ev Sahibi                 Misafir\n");
        printf("\tHafta Takim Adi            AA  AA Takim Adi\n");
        printf("\t----- -------------------- --  -- --------------------\n");

        /* Skorlar doyasindan okuma yapiliyor. Maclara tek tek bakiliyor. Eger ki aranan takim koduna rastlanirsa, bu maclar yazdiriliyor */
        fscanf(skor_ptr,"%d %d %d %d %d",&bir_skor.hafta_no,&bir_skor.ev_sahibi_kod,&bir_skor.misafir_kod,&bir_skor.ev_sahibi_gol_sayisi,&bir_skor.misafir_gol_sayisi);
        while(!feof(skor_ptr))
        {
            if(bir_skor.ev_sahibi_kod == eslesen_takim.takim_kodu)/* Eger ki aranan takim, ev sahibi ise, ev sahibi olarak yazdiriliyor */
            {
                fseek(takim_ptr,(bir_skor.misafir_kod-1)*sizeof(takim_tipi),SEEK_SET);/* Misafir ekip okunuyor */
                fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);
                printf("\t%3d   %-20s %2d  %2d %20s\n",bir_skor.hafta_no,eslesen_takim.takim_adi,bir_skor.ev_sahibi_gol_sayisi,bir_skor.misafir_gol_sayisi,bir_takim.takim_adi);
            }
            if(bir_skor.misafir_kod == eslesen_takim.takim_kodu)/* Eger ki aranan takim, misafir ise, misafir olarak yazdiriliyor */
            {
                fseek(takim_ptr,(bir_skor.ev_sahibi_kod-1)*sizeof(takim_tipi),SEEK_SET);/* Ev sahibi ekip okunuyor */
                fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);
                printf("\t%3d   %-20s %2d  %2d %20s\n",bir_skor.hafta_no,bir_takim.takim_adi,bir_skor.ev_sahibi_gol_sayisi,bir_skor.misafir_gol_sayisi,eslesen_takim.takim_adi);
            }
            fscanf(skor_ptr,"%d %d %d %d %d",&bir_skor.hafta_no,&bir_skor.ev_sahibi_kod,&bir_skor.misafir_kod,&bir_skor.ev_sahibi_gol_sayisi,&bir_skor.misafir_gol_sayisi);
        }
    }
    fclose(futbolcu_ptr);/* Futbolcular dosyasi kapatiliyor */
    fclose(skor_ptr);/* Skorlar dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
////////////////////////////////////////////////////
// (MENU 4) - Puan cetvelini listeleme fonksiyonu //
////////////////////////////////////////////////////
/**
  * Puan cetvelinin listelenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Takimlar dosyasindan takimlar okunur, puan sirasina gore (esitlik durumunda averaja bakilir) yazdirilir
  */
void puanCetveli(FILE *takim_ptr)
{
    takim_tipi bir_takim,gecici;
    takim_tipi takimlar[100];/* Takimlarin tutulacagi takim tipi dizisi olusturuluyor */

    int i,tur;
    int sira = 0;/* Takimin listede tutuldugu sirayi tutar. Son olarak aldigi deger toplam takim sayisina esittir */

    system("cls");

    rewind(takim_ptr);/* Takimlar dosyasinin basina gelinir */
    for(i=0; i<100; i++)/* Takimlar dosyasi sonuna kadar okunur */
    {
        fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);
        if(bir_takim.takim_kodu != -1)/* Eger ki takim kodu -1 degilse boyle bir takim vardir. Diziye ekleniyor */
        {
            takimlar[sira] = bir_takim;
            sira++;
        }
    }
    /* Takimlari puanlarina gore siralamak icin bubble-sort algoritmasi kullaniliyor */
    for(tur=1; tur<sira; tur++)
    {
        for(i=0; i<sira-tur; i++)
        {
            /* Puani buyuk takim one geciriliyor */
            if(takimlar[i].galibiyet_say*3+takimlar[i].beraberlik_say < takimlar[i+1].galibiyet_say*3+takimlar[i+1].beraberlik_say)
            {
                gecici = takimlar[i];
                takimlar[i] = takimlar[i+1];
                takimlar[i+1] = gecici;
            }
            /* Puanlar esitse averajlara bakiliyor, fazla olan one geciriliyor */
            else if(takimlar[i].galibiyet_say*3+takimlar[i].beraberlik_say == takimlar[i+1].galibiyet_say*3+takimlar[i+1].beraberlik_say)
            {
                if(takimlar[i+1].attigi_gol-takimlar[i+1].yedigi_gol > takimlar[i].attigi_gol-takimlar[i].yedigi_gol)
                {
                    gecici = takimlar[i];
                    takimlar[i] = takimlar[i+1];
                    takimlar[i+1] = gecici;
                }
            }
        }
    }
    /* Sort islemi tamamlaniyor. Cetvel listeleniyor */
    printf("Puan Cetveli listeleniyor...\n\n");
    printf("\tSira Takim Adi            OO GG BB MM AA YY AVR Puan\n");
    printf("\t---- -------------------- -- -- -- -- -- -- --- ----\n");
    for(i=0; i<sira; i++)
    {
        printf ("\t%3d  %-20s %2d %2d %2d %2d %2d %2d %3d %3d\n",i+1,takimlar[i].takim_adi,takimlar[i].maglubiyet_say+takimlar[i].beraberlik_say+takimlar[i].galibiyet_say,
                takimlar[i].galibiyet_say,takimlar[i].beraberlik_say,takimlar[i].maglubiyet_say,takimlar[i].attigi_gol,takimlar[i].yedigi_gol,takimlar[i].attigi_gol-takimlar[i].yedigi_gol,takimlar[i].galibiyet_say*3+takimlar[i].beraberlik_say);
    }

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
///////////////////////////////////////////////////////////////////////////////////
// (MENU 5) - En cok gol atan ve en cok gol yiyen takimlari listeleme fonksiyonu //
///////////////////////////////////////////////////////////////////////////////////
/**
  * En cok gol atan ve en cok gol yiyen ikitakimin listelenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Takimlar dosyasindan takimlar okunur, en cok gol atan ve en cok gol yiyen takimlar belirlenir ve ekrana yazdirilir
  */
void takimRekorlari(FILE *takim_ptr)
{
    takim_tipi bir_takim;
    takim_tipi golcu_takim;/* En cok gol atan takim */
    takim_tipi yenik_takim;/* En cok gol yiyen takim */

    int i;

    golcu_takim.attigi_gol = 0;/* Atilan gol sayisini basta sifirlanior */
    yenik_takim.yedigi_gol = 0;/* Yenilen gol sayisini basta sifirlanior */

    system("cls");
    rewind(takim_ptr);/* Takimlar dosyasinin basina geliniyor */
    for(i=0; i<100; i++)/* Takimlar dosyasi sonuna kadar okunuyor */
    {
        fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);
        if(bir_takim.attigi_gol > golcu_takim.attigi_gol)/* Okunan takimin attigi gol sayisi en cok atilan gol sayisindan coksa o takim En cok gol atan takim kabul ediliyor */
        {
            golcu_takim = bir_takim;
        }
        if(bir_takim.yedigi_gol > yenik_takim.yedigi_gol)/* Okunan takimin yedigi gol sayisi en cok yenilen gol sayisindan coksa o takim En cok gol yiyen takim kabul ediliyor */
        {
            yenik_takim = bir_takim;
        }
    }
    /* Donguden cikildiginda, en cok gol atan ve en cok gol yiyen takimlar yazdiriliyor */
    printf("En cok gol atan ve en cok gol yiyen takimlar listeleniyor...\n\n");
    printf("\tTakim Adi            AA  YY \n");
    printf("\t-------------------- --  -- \n");
    printf("\t%-20s %2d  %2d\n",golcu_takim.takim_adi,golcu_takim.attigi_gol,golcu_takim.yedigi_gol);
    printf("\t%-20s %2d  %2d\n",yenik_takim.takim_adi,yenik_takim.attigi_gol,yenik_takim.yedigi_gol);

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
////////////////////////////////////////////////////////////////////////
// (MENU 6) - Deplasmanda en cok gol atan takimi listeleme fonksiyonu //
////////////////////////////////////////////////////////////////////////
/**
  * Deplasmanda en cok gol atan takimin goruntulenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Skorlar dosyasindan maclar okunur. Takimlari attiklari goller hesaplanir ve deplasmanda en cok gol atan takim yazdirilir.
  */
void deplasmanRekoru(FILE *takim_ptr)
{
    FILE *skor_ptr = fopen("skorlar.txt","r");/* Skorlar dosyasi r modunda aciliyor. Sadece okuma yapilacak */

    takim_tipi bir_takim;
    skor_tipi bir_skor;

    int misafir_gol_say[100] = {0};/* Takimlarin deplasmada attiklari gol sayilarini tutacak olan dizi. Basta tum takimlar 0 gol atmis olarak kabul ediliyor */
    int i,n;
    int maks=0;

    system("cls");

    if(skor_ptr == NULL)/* Skorlar dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nSkorlar dosyasi acilamadi!\n");
    }
    else
    {
        /* Skorlar dosyasindan okuma yapiliyor */
        fscanf(skor_ptr,"%d %d %d %d %d",&bir_skor.hafta_no,&bir_skor.ev_sahibi_kod,&bir_skor.misafir_kod,&bir_skor.ev_sahibi_gol_sayisi,&bir_skor.misafir_gol_sayisi);
        while(!feof(skor_ptr))
        {
            misafir_gol_say[bir_skor.misafir_kod] += bir_skor.misafir_gol_sayisi;/* Takimlarin deplasmanda attiklari gol sayilari arttiriliyor */
            fscanf(skor_ptr,"%d %d %d %d %d",&bir_skor.hafta_no,&bir_skor.ev_sahibi_kod,&bir_skor.misafir_kod,&bir_skor.ev_sahibi_gol_sayisi,&bir_skor.misafir_gol_sayisi);
        }

        for(i=0; i<100; i++)/* Deplasmanda en cok gol atan takim araniyor */
        {
            if(misafir_gol_say[i] > maks)
            {
                maks = misafir_gol_say[i];
                n = i;/* n, deplasmanda en cok gol atan takimin kodu olmus oluyor */
            }
        }

        fseek(takim_ptr,(n-1)*sizeof(takim_tipi),SEEK_SET);/* Deplasmanda en cok gol atan takim, takimlar dosyasindan okunuyor */
        fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);
        /* Bu takim yazdiriliyor */
        printf("Deplasmda en cok gol atan takim goruntuleniyor...\n\n");
        printf("\tTakim Adi            Deplasmanda AA\n");
        printf("\t-------------------- --------------\n");
        printf("\t%-20s %8d\n",bir_takim.takim_adi,maks);
    }
    fclose(skor_ptr);/* Skorlar dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
/////////////////////////////////////////////////////////////////
// (MENU 7) - Bir haftada oynanan maclari listeleme fonksiyonu //
/////////////////////////////////////////////////////////////////
/**
  * Istenilen haftada oynanan maclarin listelenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Skorlar dosyasindan maclar okunur. Istenilen haftayla ile ilgili olan maclar okunur ve yazdirilir.
  */
void haftalikMacSonuclari(FILE *takim_ptr)
{
    FILE *skor_ptr = fopen("skorlar.txt","r");/* Skorlar dosyasi r modunda aciliyor. Sadece okuma yapilacak */

    takim_tipi ev_sahibi_takim,konuk_takim;
    skor_tipi bir_skor;

    int hafta;
    int takim_say=0;/* O hafta mac yapan takim sayisi */
    int toplam=0;/* O hafta atilan toplam gol sayisini tutacak olan degisken */

    system("cls");

    if(skor_ptr == NULL)/* Skorlar dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nSkorlar dosyasi acilamadi!\n");
    }
    else
    {
        printf("Belirtilen haftadaki mac sonuclari yazdiriliyor...\n");
        printf("Hafta no giriniz.\n");/* Hafta no aliniyor */
        hafta = hataKontrol(1,99);

        printf("\n\t\t%2d. Hafta Mac Skorlari: \n",hafta);
        printf("\t       Ev Sahibi                 Misafir\n");
        printf("\tTakim Adi            AA  AA Takim Adi \n");
        printf("\t-------------------- --  -- --------------------\n");
        /* Skorlar dosyasindan maclar okunuyor. */
        fscanf(skor_ptr,"%d %d %d %d %d",&bir_skor.hafta_no,&bir_skor.ev_sahibi_kod,&bir_skor.misafir_kod,&bir_skor.ev_sahibi_gol_sayisi,&bir_skor.misafir_gol_sayisi);
        while(!feof(skor_ptr))
        {
            if(bir_skor.hafta_no == hafta)/* Eger ki okunan macin hafta no'su istenilen hafta no'su ise takimlar okunup yazdiriliyor */
            {
                fseek(takim_ptr,(bir_skor.ev_sahibi_kod-1)*sizeof(takim_tipi),SEEK_SET);
                fread(&ev_sahibi_takim,sizeof(takim_tipi),1,takim_ptr);

                fseek(takim_ptr,(bir_skor.misafir_kod-1)*sizeof(takim_tipi),SEEK_SET);
                fread(&konuk_takim,sizeof(takim_tipi),1,takim_ptr);

                printf("\t%-20s %2d  %2d %20s\n",ev_sahibi_takim.takim_adi,bir_skor.ev_sahibi_gol_sayisi,bir_skor.misafir_gol_sayisi,konuk_takim.takim_adi);

                toplam += bir_skor.ev_sahibi_gol_sayisi + bir_skor.misafir_gol_sayisi;/* Macta atilan toplam gol sayisi hesaplaniyor */
                takim_say++;/* O hafta mac oynayan takim sayisi da arttiriliyor */
            }
            fscanf(skor_ptr,"%d %d %d %d %d",&bir_skor.hafta_no,&bir_skor.ev_sahibi_kod,&bir_skor.misafir_kod,&bir_skor.ev_sahibi_gol_sayisi,&bir_skor.misafir_gol_sayisi);
        }
        printf("\n\t%2d. hafta mac basina gol ortalamasi: %.2f \n",hafta,(float)toplam/takim_say*2);/* Ortalama yazdiriliyor */
    }
    fclose(skor_ptr);/* Skorlar dosyasi kapatiliyor*/

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
//////////////////////////////////////////////////////////////
// (MENU 8) - En cok gol atan futbolcu listeleme fonksiyonu //
//////////////////////////////////////////////////////////////
/**
  * Gol kralinin goruntulenmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Futbolcular dosyasi acilir ve sonuna kadar okunur. En cok gol atan futbolcu belirlenir ve yazdirilir
  */
void futbolcuRekoru(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb");/* Futbolcular dosyasi rb modunda aciliyor. Binary modda sadece okuma yapilacak */

    takim_tipi bir_takim;
    oyuncu_tipi bir_oyuncu,gol_krali;

    int i,n,kod;

    system("cls");
    gol_krali.attigi_gol = 0;/* Baslangicta, gol krali 0 gol atmis olarak kabul ediliyor*/

    if(futbolcu_ptr == NULL)/* Futbolcular dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else
    {
        for(i=0; i<10000; i++)/* Futbolcular dosyasinin sonuna kadar okunuyor */
        {
            fread(&bir_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);
            if(bir_oyuncu.forma_numarasi != -1 && bir_oyuncu.attigi_gol > gol_krali.attigi_gol)/* Eger ki oyuncunun attigi gol sayisi atilan maksimum gol sayisindan fazla ise o oyuncu gol krali yapiliyor */
            {
                n = i;/* n, gol kralinin dosyada tutuldugu yeri gosteriyor. Buradan oyuncunun hangi takimda bulundugunu ogreniyoruz */
                gol_krali = bir_oyuncu;
            }
        }
        /* Dongu bittikten sonra, futbolcunun takimini buluyoruz */
        kod = n/100;/* Futbolcunun takimi, dosyada tutuldugu yerden yola cikilarak ogreniliyor */

        fseek(takim_ptr,(kod-1)*sizeof(takim_tipi),SEEK_SET);/* Futbolcunun takimi okunuyor */
        fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);
        /* Futbolcu ve takimi yazdiriliyor */
        printf("En cok gol atan oyuncu goruntuleniyor...\n\n");
        printf("\tTakim Adi            Forma No Ad Soyad             AA\n");
        printf("\t-------------------- -------- -------------------- -- \n");
        printf("\t%-20s %5d    %-20s %2d\n",bir_takim.takim_adi,gol_krali.forma_numarasi,gol_krali.ad_soyad,gol_krali.attigi_gol);
    }
    fclose(futbolcu_ptr);/* Futbolcular dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
///////////////////////////////////////////////////////
// (MENU 9) - Lig disindan transfer yapma fonksiyonu //
///////////////////////////////////////////////////////
/**
  * Lig disindan, lige yeni bir oyuncu transfer edilmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Futbolcular dosyasi acilir. Oyuncuyu alan takim ve oyuncunun bilgileri alinir.
  * Hata kontrolleri yapilir ve futbolcu takima eklenir
  */
void ligDisindanTransfer(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb+");/* Futbolcular dosyasi rb+ modunda aciliyor. Binary modda hem okuma hem yazma yapilacak */

    oyuncu_tipi bir_oyuncu;
    takim_tipi alan_takim;

    int forma_no,i;
    char str[21];

    system("cls");
    if(futbolcu_ptr == NULL)/* Futbolcular dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else
    {
        printf("Lig disindan transfer yapiliyor...\n\n");
        printf("Oyuncuyu alan takimin kodunu giriniz.\n");/* Takim kodu aliniyor */
        alan_takim = takimAra(takim_ptr);

        printf("Oyuncunun kullanacagi forma numarasini giriniz.\n");
        do/* Oyuncunun yeni takiminda kullanacagi forma numarasina sahip bir oyuncu olup olmadigini kontrol eden donguye giriliyor */
        {
            forma_no = hataKontrol(1,99);

            fseek(futbolcu_ptr,((alan_takim.takim_kodu*100)+forma_no-1)*sizeof(oyuncu_tipi),SEEK_SET);/* Dosyada ilgili yere gidip okuma yapiliyor */
            fread(&bir_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

            if(bir_oyuncu.forma_numarasi != -1)/* Eger ki forma numarasi -1 degilse demekki bu numaraya sahip bir oyuncu var. hata yazdiriliyor */
            {
                printf("HATA! Boyle bir oyuncu zaten var!\n");
            }
            else
            {
                printf("Oyuncunun adini ve soyadini giriniz.\n");/* Oyuncunun adi aliniyor */
                fflush(stdin);
                gets(str);
                for(i=0;str[i] != '\0';i++)/* Oyuncunun adi buyuk harflere donusturuyor */
                {
                    bir_oyuncu.ad_soyad[i] = toupper(str[i]);
                }
                bir_oyuncu.ad_soyad[i] = '\0';

                bir_oyuncu.attigi_gol = 0;/* Oyuncunun attigi gol sayisi sifirlaniyor */
            }
        }
        while(bir_oyuncu.forma_numarasi != -1);/* Kullanici, o takimda olmayan bir futbolcunun numarasini girene kadar donguden cikilmiyor */

        bir_oyuncu.forma_numarasi = forma_no;

        fseek(futbolcu_ptr,((alan_takim.takim_kodu*100)+bir_oyuncu.forma_numarasi-1)*sizeof(oyuncu_tipi),SEEK_SET);/* Oyuncu dosyaya ekleniyor */
        fwrite(&bir_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

        printf("\n\n%d forma numarali %s, %s takimina transfer edildi.\n",alan_takim.takim_kodu,bir_oyuncu.ad_soyad,alan_takim.takim_adi);
    }
    fclose(futbolcu_ptr);/* Futbolcu dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
///////////////////////////////////////////////////////
// (MENU 10) - Lig icinden transfer yapma fonksiyonu //
///////////////////////////////////////////////////////
/**
  * Lig icinden, ayni ligdeki baska bir takima bir oyuncu transfer edilmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez.
  * Futbolcular dosyasi acilir. Oyuncuyu satan takim ve oyuncunun bilgileri alinir.
  * Oyuncuyu alan takim ve oyuncunun yeni forma numarasi alinir.
  * Hata kontrolleri yapilir ve futbolcu yeni takimina eklenir ve eski takimindan silinir.
  */
void ligIcindenTransfer(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb+");/* Futbolcular dosyasi rb+ modunda aciliyor. Binary modda hem okuma hem yazma yapilacak */

    takim_tipi  alan_takim,veren_takim;
    oyuncu_tipi alinan_oyuncu,verilen_oyuncu,transfer_oyuncu;
    oyuncu_tipi bos_oyuncu = {-1,"",0};/* Eski takiminda, futbolcunun yerine yazdirilacak olan bos oyuncu */

    int forma_no;

    system("cls");
    if(futbolcu_ptr == NULL)/* Futbolcular dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else
    {
        printf("Lig icindeki baska bir takimdan transfer yapiliyor...\n\n");
        printf("Oyuncuyu satan takimin kodunu giriniz.\n");/* Satan takimin kodu aliniyor */
        veren_takim = takimAra(takim_ptr);

        printf("Oyuncunun forma numarasini giriniz.\n");/* Satan takimdaki futbolcunun forma numarasi aliniyor */
        verilen_oyuncu = oyuncuAra(futbolcu_ptr,veren_takim.takim_kodu);

        printf("Oyuncu alan takimin kodunu giriniz.\n");/* Alan takimin kodu aliniyor */
        alan_takim = takimAra(takim_ptr);

        printf("Oyuncunun giyecegi forma numarasini giriniz.\n");
        do/* Oyuncunun yeni takiminda kullanacagi forma numarasina sahip bir oyuncu olup olmadigini kontrol eden donguye giriliyor */
        {
            forma_no = hataKontrol(1,99);

            fseek(futbolcu_ptr,(alan_takim.takim_kodu*100+forma_no-1)*sizeof(oyuncu_tipi),SEEK_SET);
            fread(&alinan_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

            if(alinan_oyuncu.forma_numarasi != -1)/* Eger ki forma numarasi -1 degilse demekki bu numaraya sahip bir oyuncu var. hata yazdiriliyor */
            {
                printf("Bu takiminda zaten boyle bir futbolcu bulunmaktadir.\n");
            }
        }
        while(alinan_oyuncu.forma_numarasi != -1);/* Kullanici, yeni takimdadan bos bir forma numarasi girinceye kadar donguden cikamiyor*/

        /* Oyuncu, forma no'sunu yeni takimdan, adi ve attigi gol sayisini da eski takimindan aliyor */
        transfer_oyuncu.forma_numarasi = forma_no;
        strcpy(transfer_oyuncu.ad_soyad,verilen_oyuncu.ad_soyad);
        transfer_oyuncu.attigi_gol = verilen_oyuncu.attigi_gol;
        /* Oyuncu, eski takimdan silinip yeni takimina ekleniyor */
        fseek(futbolcu_ptr,((alan_takim.takim_kodu*100)+transfer_oyuncu.forma_numarasi-1)*sizeof(oyuncu_tipi),SEEK_SET);
        fwrite(&transfer_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);
        fseek(futbolcu_ptr,((veren_takim.takim_kodu*100)+verilen_oyuncu.forma_numarasi-1)*sizeof(oyuncu_tipi),SEEK_SET);
        fwrite(&bos_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

        printf("\n%s, %s 'den %s 'e basariyla transfer edilmistir!\n",transfer_oyuncu.ad_soyad,veren_takim.takim_adi,alan_takim.takim_adi);
    }
    fclose(futbolcu_ptr);/* Futbolcu dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
////////////////////////////////////////////////////
// (MENU 11) - Lig disina oyuncu satma fonksiyonu //
////////////////////////////////////////////////////
/**
  * Lig disina bir oyuncu transfer edilmesini saglayan fonksiyon.
  * Paramatre olarak takimlarin tutuldugu dosyayi alir ve geriye bir sey dondurmez
  * Futbolcular dosyasi acilir. Oyuncuyu satan takim ve oyuncunun bilgileri alinir.
  * Hata kontrolleri yapiliyor ve futbolcu takimdan silinir
  */
void ligDisinaTransfer(FILE *takim_ptr)
{
    FILE *futbolcu_ptr = fopen("futbolcular.dat","rb+");/* Futbolcular dosyasi rb+ modunda aciliyor. Binary modda hem okuma hem yazma yapilacak */

    oyuncu_tipi verilen_oyuncu;
    oyuncu_tipi bos_oyuncu = {-1,"",0};/* Eski takiminda, futbolcunun yerine yazdirilacak olan bos oyuncu */
    takim_tipi veren_takim;

    if(futbolcu_ptr == NULL)/* Futbolcular dosyasi acilamadiysa hata veriliyor */
    {
        printf("\nFutbolcular dosyasi acilamadi!\n");
    }
    else
    {
        printf ("\nOyuncuyu satan takimin kodunu giriniz\n");/* Satan takimin kodu ainiyor */
        veren_takim = takimAra(takim_ptr);
        verilen_oyuncu = oyuncuAra(futbolcu_ptr,veren_takim.takim_kodu);/* Oyuncunun forma numarasi aliniyor */
        /* Oyuncu takimdan silinir*/
        fseek(futbolcu_ptr,(veren_takim.takim_kodu*100+verilen_oyuncu.forma_numarasi-1)*sizeof(oyuncu_tipi),SEEK_SET);
        fwrite(&bos_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

        printf("\n%s basariyla transfer edildi!\n",verilen_oyuncu.ad_soyad);
    }
    fclose(futbolcu_ptr);/* Futbolcu dosyasi kapatiliyor */

    printf("\nMenuye donmek icin bir tusa basin...\n");/* Menuye donuluyor */
    fflush(stdin);
    getchar();
}
//////////////////////////////
// Takim kontrol fonksiyonu //
//////////////////////////////
/**
  * Kullancinin takimlar dosyasinda bulunan bir takim girmesini saglayan fonksiyon.
  * Parametre olarak takim dosyasini alir ve geriye takim tipi dondurur.
  */
takim_tipi takimAra(FILE *takim_ptr)
{
    int kod;
    takim_tipi bir_takim;
    do/* Kullancinin, takimlar dosyasinda bulunan bir takim girinceye kadar deger girmesini saglayan fonksiyon */
    {
        kod = hataKontrol(1,99);/* Takim kodu alinir */

        fseek(takim_ptr,(kod-1)* sizeof(takim_tipi),SEEK_SET);/* Dosyada bulundugu yere gidilir ve okuma yapilir */
        fread(&bir_takim,sizeof(takim_tipi),1,takim_ptr);

        if(bir_takim.takim_kodu == -1)/* Takim kodu -1 ise boyle bir takim yoktur. Hata verilir */
        {
            printf("HATA! Boyle bir takim bulunmamaktadir!\n");
        }
    }
    while(bir_takim.takim_kodu == -1);/* Takim kodu -1 olmayan bir takim girilmedikce donguden cikilamaz */

    return bir_takim;/* Takim kodu girilen takim geri dondurulur */
}
/////////////////////////////////
// Futbolcu kontrol fonksiyonu //
/////////////////////////////////
/**
  * Kullanicinin, belli bir takimda bulunan bir oyuncu girmesini saglayan fonksiyon.
  * Parametre olarak takim dosyasi ve futbolcunun oynadigi takim kodunu alir ve geriye oyuncu tipi dondurur.
  */
oyuncu_tipi oyuncuAra(FILE *futbolcu_ptr,int takim_kod)
{
    int forma_no;
    oyuncu_tipi bir_oyuncu;
    do/* Kullancinin, futbolcular dosyasinda bulunan bir futbolcu girinceye kadar deger girmesini saglayan fonksiyon */
    {
        forma_no = hataKontrol(1,99);/* Forma no alinir */

        fseek(futbolcu_ptr,(takim_kod * 100 + forma_no-1) * sizeof(oyuncu_tipi),SEEK_SET);/* Dosyada bulundugu yere gidilir ve okuma yapilir */
        fread(&bir_oyuncu,sizeof(oyuncu_tipi),1,futbolcu_ptr);

        if(bir_oyuncu.forma_numarasi == -1)/* Forma nosu -1 ise boyle bir futbolcu yoktur. Hata verilir */
        {
            printf("HATA! Boyle bir futbolcu bulunmamaktadir!\n");
        }
    }while(bir_oyuncu.forma_numarasi == -1);/* Forma nosu -1 olmayan bir futbolcu girilmedikce donguden cikilamaz */

    return bir_oyuncu;/* Forma nosu girilen takimgeri dondurulur */
}
