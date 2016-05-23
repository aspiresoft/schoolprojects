#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct takimlar
{
    int takim_kodu;
    char takim_adi[21];
    int galibiyet_say;
    int beraberlik_say;
    int maglubiyet_say;
    int attigi_gol_say;
    int yedigi_gol_say;
    struct maclar *ilk_maci;
};

struct futbolcular
{
    int forma_no;
    char ad_soyad[21];
    int attigi_gol;
    struct futbolcular *sonraki;
};

struct maclar
{
    int hafta_no;
    int ev_sahibi_kod;
    int misafir_kod;
    int ev_sahibi_gol_say;
    int misafir_gol_say;
    struct maclar *ev_sahibi_sonraki_mac;
    struct maclar *misafir_sonraki_mac;
};

void menu_takim_ekle(struct takimlar **);
void menu_disardan_iceri_transfer(struct takimlar **,struct futbolcular *);
void menu_icerden_iceri_transfer(struct takimlar **,struct futbolcular *);
void menu_icerden_disari_transfer(struct takimlar **,struct futbolcular *);
void menu_skor_ekle(struct takimlar **, struct futbolcular *);
void menu_takim_ve_futbolculari(struct takimlar **,struct futbolcular *);
void menu_takim_ve_maclari(struct takimlar **);
void menu_tum_takimlar(struct takimlar **);
void menu_en_cok_atan_ve_yiyen(struct takimlar **);
void menu_deplasmanda_en_cok_atan(struct takimlar **);
void menu_golculer(struct takimlar **,struct futbolcular *);

struct futbolcular *futbolcuAra(struct futbolcular *, int );
void futbolcuEkle(struct futbolcular *, struct futbolcular *);
struct futbolcular *futbolcuCikar(struct futbolcular *, int );

int menu();
int hataKontrol(int ,int );

int main()
{
    struct takimlar _takimlar[99];
    struct futbolcular _futbolcu_hash_table[99];

    int secim,i;

    system("color 0a");

    for(i=0; i<99; i++)
        _futbolcu_hash_table[i].sonraki=&_futbolcu_hash_table[i];
    do
    {
        switch(secim = menu())
        {
        case 1:
            menu_takim_ekle(&_takimlar);
            break;
        case 2:
            menu_disardan_iceri_transfer(&_takimlar,_futbolcu_hash_table);
            break;
        case 3:
            menu_icerden_iceri_transfer(&_takimlar,_futbolcu_hash_table);
            break;
        case 4:
            menu_icerden_disari_transfer(&_takimlar,_futbolcu_hash_table);
            break;
        case 5:
            menu_skor_ekle(&_takimlar,_futbolcu_hash_table);
            break;
        case 6:
            menu_takim_ve_futbolculari(&_takimlar,_futbolcu_hash_table);
            break;
        case 7:
            menu_takim_ve_maclari(&_takimlar);
            break;
        case 8:
            menu_tum_takimlar(&_takimlar);
            break;
        case 9:
            menu_en_cok_atan_ve_yiyen(&_takimlar);
            break;
        case 10:
            menu_deplasmanda_en_cok_atan(&_takimlar);
            break;
        case 11:
            menu_golculer(&_takimlar,_futbolcu_hash_table);
            break;
        case 12:
            break;
        }
    }
    while(secim != 12);

    return 0;
}

int menu()
{
    system("cls");
    printf("\n-------------------------------------------------------------------------------\n");
    printf("|01|                           Yeni bir takim ekle                         |01|\n");
    printf("|02|             Lig disindan yeni bir futbolcunu transfer et              |02|\n");
    printf("|03|                 Lig icinden bir futbolcu transfer et                  |03|\n");
    printf("|04|                  Lig disina futbolcu transfer et                      |04|\n");
    printf("|05|                          Yeni mac skoru ekle                          |05|\n");
    printf("|06|                   Bir takimi ve futbolcularini listele                |06|\n");
    printf("|07|                      Bir takimi ve maclarini listele                  |07|\n");
    printf("|08|                         Tum takimlari listele                         |08|\n");
    printf("|09|              En cok gol atan ve yiyen takimlari goruntule             |09|\n");
    printf("|10|            Deplasmanda en cok gol atan takimi goruntule               |10|\n");
    printf("|11|       Her takimdaki golcu futbolculari ve gol kralini listele         |11|\n");
    printf("|12|                                 Cikis                                 |12|\n");
    printf("-------------------------------------------------------------------------------\n\n");
    printf("   Secim : ");

    return hataKontrol(1,12);
}
/////////////////////////////
// Hata kontrol fonksiyonu //
/////////////////////////////
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
void menu_takim_ekle(struct takimlar **_takimlar)
{
    struct takimlar *yeni_takim;
    int kod,index,i;

    system("cls");

    printf("Lige yeni katilacak olan takimin kodunu giriniz:\n");
    kod = hataKontrol(1,99);
    index = kod - 1;

    if(_takimlar[index] != NULL)
    {
        printf("HATA! Bu koda sahip bir takim zaten mevcut.\n");
    }
    else
    {
        yeni_takim = malloc(sizeof(struct takimlar));
        yeni_takim->takim_kodu = kod;
        printf("Takimin adini giriniz\n");
        fflush(stdin);
        gets(yeni_takim->takim_adi);
        yeni_takim->attigi_gol_say = 0;
        yeni_takim->beraberlik_say = 0;
        yeni_takim->galibiyet_say = 0;
        yeni_takim->maglubiyet_say = 0;
        yeni_takim->yedigi_gol_say = 0;
        yeni_takim->ilk_maci = NULL;
        for(i=0; yeni_takim->takim_adi[i] != '\0'; i++)
        {
            yeni_takim->takim_adi[i] = toupper(yeni_takim->takim_adi[i]);
        }
        _takimlar[index] = yeni_takim;
        printf("%s takimi basariyla sisteme eklenmistir!\n",_takimlar[index]->takim_adi);
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_disardan_iceri_transfer(struct takimlar **_takimlar, struct futbolcular *_futbolcular)
{
    struct futbolcular *_yeni_futbolcu;
    int takim_kodu,index,forma_no,i;

    system("cls");

    printf("Transferi yapan takimin kodunu giriniz.\n");
    takim_kodu = hataKontrol(1,99);
    index = takim_kodu - 1;

    if(_takimlar[index] == NULL)
    {
        printf("%d kodlu bir takim bulunamadi!\n",takim_kodu);
    }
    else
    {
        printf("%s takimina transfer olan futbolcunun forma numarasini giriniz.\n",_takimlar[index]->takim_adi);
        forma_no = hataKontrol(1,99);

        if(!(futbolcuAra(&_futbolcular[index],forma_no) == NULL))
        {
            printf("%s takiminda %d forma numarali bir futbolcu zaten var!\n",_takimlar[index]->takim_adi,forma_no);
        }
        else
        {
            _yeni_futbolcu = malloc(sizeof(struct futbolcular));
            _yeni_futbolcu->forma_no = forma_no;
            printf("Transfer edilen futbolcunun adini giriniz.\n");
            fflush(stdin);
            gets(_yeni_futbolcu->ad_soyad);
            for(i=0; _yeni_futbolcu->ad_soyad[i] != '\0'; i++)
            {
                _yeni_futbolcu->ad_soyad[i] = toupper(_yeni_futbolcu->ad_soyad[i]);
            }
            _yeni_futbolcu->attigi_gol = 0;
            futbolcuEkle(&_futbolcular[index],_yeni_futbolcu);

            printf("%d forma numarali %s basariyla %s takimina transfer edildi!\n",forma_no,_yeni_futbolcu->ad_soyad,_takimlar[index]->takim_adi);
        }
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_icerden_iceri_transfer(struct takimlar **_takimlar, struct futbolcular *_futbolcular)
{
    struct futbolcular *eski_futbolcu,*yeni_futbolcu;
    int satan_takim_kodu,alan_takim_kodu;
    int satan_takim_index,alan_takim_index;
    int eski_forma_no,yeni_forma_no;

    system("cls");

    printf("Futbolcuyu satacak olan takimin kodunu giriniz.\n");
    satan_takim_kodu = hataKontrol(1,99);
    satan_takim_index = satan_takim_kodu - 1;

    if(_takimlar[satan_takim_index] == NULL)
    {
        printf("%d kodlu bir takim bulunamadi!\n",satan_takim_kodu);
    }
    else
    {
        printf("%s takimindan satilacak olan futbolcunun forma numarasini giriniz.\n",_takimlar[satan_takim_index]->takim_adi);
        eski_forma_no = hataKontrol(1,99);

        if((eski_futbolcu = futbolcuAra(&_futbolcular[satan_takim_index],eski_forma_no)) == NULL)
        {
            printf("%s takiminda %d forma numarali bir futbolcu yok!\n",_takimlar[satan_takim_index]->takim_adi,eski_forma_no);
        }
        else
        {
            printf("%s futbolcusunu alacak olan takimin kodunu giriniz.\n",eski_futbolcu->ad_soyad);
            alan_takim_kodu = hataKontrol(1,99);
            alan_takim_index = alan_takim_kodu - 1;

            if(_takimlar[alan_takim_index] == NULL)
            {
                printf("%d kodlu bir takim bulunamadi!\n",alan_takim_kodu);
            }
            else
            {
                printf("%s futbolcusunun %s takiminda kullanacagi forma numarasini giriniz.\n",eski_futbolcu->ad_soyad,_takimlar[alan_takim_index]->takim_adi);
                yeni_forma_no = hataKontrol(1,99);

                if((futbolcuAra(&_futbolcular[alan_takim_index],yeni_forma_no)) != NULL)
                {
                    printf("%s takiminda %d forma numarali bir futbolcu zaten var!\n",_takimlar[alan_takim_index]->takim_adi,yeni_forma_no);
                }
                else
                {
                    printf("%s basariyla transfer edildi.\n",eski_futbolcu->ad_soyad);
                    yeni_futbolcu = malloc(sizeof(struct futbolcular));
                    strcpy(yeni_futbolcu->ad_soyad,eski_futbolcu->ad_soyad);
                    yeni_futbolcu->attigi_gol = eski_futbolcu->attigi_gol;
                    yeni_futbolcu->forma_no = yeni_forma_no;
                    free(futbolcuCikar(&_futbolcular[satan_takim_index],eski_forma_no));
                    futbolcuEkle(&_futbolcular[alan_takim_index],yeni_futbolcu);
                }
            }
        }
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_icerden_disari_transfer(struct takimlar **_takimlar, struct futbolcular *_futbolcular)
{
    struct futbolcular *satilan_futbolcu;
    int takim_kodu;
    int index;
    int forma_no;

    system("cls");

    printf("Lig disina transfer olan oyuncunun takiminin kodunu giriniz.\n");
    takim_kodu = hataKontrol(1,99);
    index = takim_kodu - 1;

    if(_takimlar[index] == NULL)
    {
        printf("%d kodlu bir takim bulunamadi!\n",index);
    }
    else
    {
        printf("%s takimindan satilacak olan futbolcunun forma numarasini giriniz.\n",_takimlar[index]->takim_adi);
        forma_no = hataKontrol(1,99);

        if((satilan_futbolcu = futbolcuAra(&_futbolcular[index],forma_no)) == NULL)
        {
            printf("%s takiminda %d forma numarali bir futbolcu yok!\n",_takimlar[index]->takim_adi,forma_no);
        }
        else
        {
            printf("%s basariyla transfer edildi.\n",satilan_futbolcu->ad_soyad);
            free(futbolcuCikar(&_futbolcular[index],forma_no));
        }
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_skor_ekle(struct takimlar **_takimlar, struct futbolcular *_futbolcular)
{
    struct maclar *yeni_mac,*gecici,*onceki;
    struct maclar *ev_sahibi_onceki,*ev_sahibi_gecici;
    struct maclar *misafir_onceki,*misafir_gecici;
    struct futbolcular *golcu;
    int ev_sahibi_kodu,ev_sahibi_index;
    int misafir_kodu,misafir_index;
    int ev_sahibi_gol_say,misafir_gol_say;
    int hafta_no;
    int sayac;
    int forma_no,gol,flag;
    char devam;

    do
    {
        system("cls");

        printf("Macin oynandigi hafta numarasini giriniz.\n");
        hafta_no = hataKontrol(1,99);
        printf("Ev sahibi takimin kodunu giriniz.\n");
        ev_sahibi_kodu = hataKontrol(1,99);
        ev_sahibi_index = ev_sahibi_kodu - 1;
        printf("Misafir takimin kodunu giriniz.\n");
        misafir_kodu = hataKontrol(1,99);
        misafir_index = misafir_kodu - 1;

        printf("%s takiminin attigi gol sayisini giriniz.\n",_takimlar[ev_sahibi_index]->takim_adi);
        ev_sahibi_gol_say = hataKontrol(0,99);
        printf("%s takiminin attigi gol sayisini giriniz.\n",_takimlar[misafir_index]->takim_adi);
        misafir_gol_say = hataKontrol(0,99);

        if(_takimlar[ev_sahibi_index] == NULL || _takimlar[misafir_index] == NULL)
        {
            printf("Hata! Takimlar bulunamadi!\n");
        }
        else
        {
            yeni_mac = malloc(sizeof(struct maclar));
            yeni_mac->hafta_no = hafta_no;
            yeni_mac->ev_sahibi_kod = ev_sahibi_kodu;
            yeni_mac->ev_sahibi_gol_say = ev_sahibi_gol_say;
            yeni_mac->misafir_kod = misafir_kodu;
            yeni_mac->misafir_gol_say = misafir_gol_say;
            yeni_mac->ev_sahibi_sonraki_mac = NULL;
            yeni_mac->misafir_sonraki_mac= NULL;

            if(hafta_no == 1)
            {
                _takimlar[ev_sahibi_index]->ilk_maci = yeni_mac;
                _takimlar[misafir_index]->ilk_maci = yeni_mac;
            }
            else
            {
                ev_sahibi_onceki = _takimlar[ev_sahibi_index]->ilk_maci;
                ev_sahibi_gecici = ev_sahibi_onceki->ev_sahibi_sonraki_mac;
                while ((ev_sahibi_gecici != NULL) && (ev_sahibi_gecici->hafta_no > yeni_mac->hafta_no))
                {
                    if(_takimlar[ev_sahibi_index]->ilk_maci->ev_sahibi_kod == ev_sahibi_kodu)
                        flag = 2;
                    else
                        flag = 1;

                    ev_sahibi_onceki=ev_sahibi_gecici;
                    if(flag == 1)
                    {
                        flag = 2;
                        ev_sahibi_gecici=ev_sahibi_gecici->misafir_sonraki_mac;
                    }
                    else
                    {
                        flag = 1;
                        ev_sahibi_gecici=ev_sahibi_gecici->ev_sahibi_sonraki_mac;
                    }
                }
                yeni_mac->ev_sahibi_sonraki_mac=ev_sahibi_gecici;
                ev_sahibi_onceki->ev_sahibi_sonraki_mac=yeni_mac;



                misafir_onceki = _takimlar[misafir_index]->ilk_maci;
                misafir_gecici = misafir_onceki->misafir_sonraki_mac;
                while ((misafir_gecici != NULL) && (misafir_gecici->hafta_no > yeni_mac->hafta_no))
                {
                    if(_takimlar[misafir_index]->ilk_maci->misafir_kod == misafir_kodu)
                        flag = 2;
                    else
                        flag = 1;

                    misafir_onceki=misafir_gecici;
                    if(flag == 1)
                    {
                        flag = 2;
                        misafir_gecici=misafir_gecici->ev_sahibi_sonraki_mac;
                    }
                    else
                    {
                        flag = 1;
                        misafir_gecici=misafir_gecici->ev_sahibi_sonraki_mac;
                    }
                }
                yeni_mac->misafir_sonraki_mac=misafir_gecici;
                misafir_onceki->misafir_sonraki_mac=yeni_mac;

                /*
                ev_sahibi_onceki = _takimlar[ev_sahibi_index]->ilk_maci;
                ev_sahibi_gecici = ev_sahibi_onceki->ev_sahibi_sonraki_mac;
                while ((ev_sahibi_gecici != NULL) && (ev_sahibi_gecici->hafta_no > yeni_mac->hafta_no))
                {
                    ev_sahibi_onceki=ev_sahibi_gecici;
                    ev_sahibi_gecici=ev_sahibi_gecici->ev_sahibi_sonraki_mac;
                }
                yeni_mac->ev_sahibi_sonraki_mac=ev_sahibi_gecici;
                ev_sahibi_onceki->ev_sahibi_sonraki_mac=yeni_mac;

                misafir_onceki = _takimlar[misafir_index]->ilk_maci;
                misafir_gecici = misafir_onceki->misafir_sonraki_mac;
                while ((misafir_gecici != NULL) && (misafir_gecici->hafta_no > yeni_mac->hafta_no))
                {
                    misafir_onceki=misafir_gecici;
                    misafir_gecici=misafir_gecici->misafir_sonraki_mac;
                }
                yeni_mac->misafir_sonraki_mac=misafir_gecici;
                misafir_onceki->misafir_sonraki_mac=yeni_mac;*/
            }

            _takimlar[ev_sahibi_index]->attigi_gol_say += ev_sahibi_gol_say;
            _takimlar[misafir_index]->attigi_gol_say += misafir_gol_say;
            _takimlar[ev_sahibi_index]->yedigi_gol_say += misafir_gol_say;
            _takimlar[misafir_index]->yedigi_gol_say += ev_sahibi_gol_say;

            if(ev_sahibi_gol_say > misafir_gol_say)
            {
                _takimlar[ev_sahibi_index]->galibiyet_say++;
                _takimlar[misafir_index]->maglubiyet_say++;
            }
            else if(ev_sahibi_gol_say < misafir_gol_say)
            {
                _takimlar[ev_sahibi_index]->maglubiyet_say++;
                _takimlar[misafir_index]->galibiyet_say++;
            }
            else
            {
                _takimlar[ev_sahibi_index]->beraberlik_say++;
                _takimlar[misafir_index]->beraberlik_say++;
            }
            sayac = 0;
            while(sayac < ev_sahibi_gol_say)
            {
                printf("%s takimindan gol atan oyuncunun forma numarasini giriniz.\n",_takimlar[ev_sahibi_index]->takim_adi);
                forma_no = hataKontrol(1,99);
                if((golcu = futbolcuAra(&_futbolcular[ev_sahibi_index],forma_no)) == NULL)
                {
                    printf("%d forma numarasina sahip bir oyuncu bulunmamaktadir.\n",forma_no);
                }
                else
                {
                    if(ev_sahibi_gol_say - sayac == 1)
                    {
                        gol = 1;
                    }
                    else
                    {
                        printf("%s futbolcusunun kaydettigi gol sayisini giriniz.\n",golcu->ad_soyad);
                        gol = hataKontrol(1,ev_sahibi_gol_say-sayac);
                    }
                    golcu->attigi_gol += gol;
                    sayac += gol;
                }
            }
            sayac = 0;
            while(sayac < misafir_gol_say)
            {
                printf("%s takimindan gol atan oyuncunun forma numarasini giriniz.\n",_takimlar[misafir_index]->takim_adi);
                forma_no = hataKontrol(1,99);
                if((golcu = futbolcuAra(&_futbolcular[misafir_index],forma_no)) == NULL)
                {
                    printf("%d forma numarasina sahip bir oyuncu bulunmamaktadir.\n",forma_no);
                }
                else
                {
                    if(misafir_gol_say - sayac == 1)
                    {
                        gol = 1;
                    }
                    else
                    {
                        printf("%s futbolcusunun kaydettigi gol sayisini giriniz.\n",golcu->ad_soyad);
                        gol = hataKontrol(1,ev_sahibi_gol_say-sayac);
                    }
                    golcu->attigi_gol += gol;
                    sayac += gol;
                }
            }
            printf("\n%s (%d) - (%d) %s maci sisteme eklenmistir!\n",_takimlar[ev_sahibi_index]->takim_adi,ev_sahibi_gol_say,misafir_gol_say,_takimlar[misafir_index]->takim_adi);
        }
        printf("\nBaska bir mac skoru eklemek ister misiniz?(e-E/h-H)\n");
        fflush(stdin);
        devam = getchar();
    }
    while(devam == 'E' || devam == 'e');
}
void menu_takim_ve_futbolculari(struct takimlar **_takimlar,struct futbolcular *_futbolcular)
{
    int takim_kodu;
    int index;
    struct futbolcular *liste_basi;

    system("cls");

    printf("Bilgileri goruntulenecek olan takimin kodunu giriniz.\n");
    takim_kodu = hataKontrol(1,99);
    index = takim_kodu - 1;

    if(_takimlar[index] == NULL)
    {
        printf("Bu koda sahip bir takim bulunmamaktadir!\n");
    }
    else
    {
        printf("\n\n\tTakim Kodu Takim Adi            OO GG BB MM AA YY AVR Puan\n");
        printf("\t---------- -------------------- -- -- -- -- -- -- --- ----\n");
        printf("\t    %-2d     %-20s %2d %2d %2d %2d %2d %2d %3d  %2d\n",
               _takimlar[index]->takim_kodu,_takimlar[index]->takim_adi,
               _takimlar[index]->galibiyet_say+_takimlar[index]->beraberlik_say+_takimlar[index]->maglubiyet_say,
               _takimlar[index]->galibiyet_say,_takimlar[index]->beraberlik_say,_takimlar[index]->maglubiyet_say,
               _takimlar[index]->attigi_gol_say,_takimlar[index]->yedigi_gol_say,
               _takimlar[index]->attigi_gol_say - _takimlar[index]->yedigi_gol_say,
               _takimlar[index]->galibiyet_say*3+_takimlar[index]->beraberlik_say);
        printf("\n\t\tForma No Ad Soyad             AA\n");
        printf("\t\t-------- -------------------- --\n");

        liste_basi=_futbolcular[index].sonraki;
        while (liste_basi!=&_futbolcular[index])
        {
            printf("\t\t   %-2d    %-20s %2d\n",
                   liste_basi->forma_no,liste_basi->ad_soyad,liste_basi->attigi_gol);
            liste_basi = liste_basi->sonraki;
        }
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_takim_ve_maclari(struct takimlar **_takimlar)
{
    int takim_kodu;
    int index,flag = 1;
    struct maclar *liste_basi;

    system("cls");

    printf("Bilgileri goruntulenecek olan takimin kodunu giriniz.\n");
    takim_kodu = hataKontrol(1,99);
    index = takim_kodu - 1;

    if(_takimlar[index] == NULL)
    {
        printf("Bu koda sahip bir takim bulunmamaktadir!\n");
    }
    else
    {
        printf("\n\n\tTakim Kodu Takim Adi            OO GG BB MM AA YY AVR Puan\n");
        printf("\t---------- -------------------- -- -- -- -- -- -- --- ----\n");
        printf("\t    %-2d     %-20s %2d %2d %2d %2d %2d %2d %3d  %2d\n",
               _takimlar[index]->takim_kodu,_takimlar[index]->takim_adi,
               _takimlar[index]->galibiyet_say+_takimlar[index]->beraberlik_say+_takimlar[index]->maglubiyet_say,
               _takimlar[index]->galibiyet_say,_takimlar[index]->beraberlik_say,_takimlar[index]->maglubiyet_say,
               _takimlar[index]->attigi_gol_say,_takimlar[index]->yedigi_gol_say,
               _takimlar[index]->attigi_gol_say - _takimlar[index]->yedigi_gol_say,
               _takimlar[index]->galibiyet_say*3+_takimlar[index]->beraberlik_say);
        printf("\n\t\t             Ev Sahibi                 Misafir\n");
        printf("\t\tHafta Takim Adi            AA  AA Takim Adi\n");
        printf("\t\t----- -------------------- --  -- --------------------\n");

        liste_basi = _takimlar[index]->ilk_maci;
        if(liste_basi->ev_sahibi_kod == takim_kodu)
            flag = 2;
        else
            flag = 1;

        while(liste_basi != NULL)
        {
            printf("\t\t %2d   %-20s %2d  %2d %20s\n",liste_basi->hafta_no,_takimlar[liste_basi->ev_sahibi_kod-1]->takim_adi,liste_basi->ev_sahibi_gol_say,
                   liste_basi->misafir_gol_say,_takimlar[liste_basi->misafir_kod-1]->takim_adi);
            if(flag == 1)
            {
                liste_basi = liste_basi->ev_sahibi_sonraki_mac;
                flag = 2;
            }
            else
            {
                liste_basi = liste_basi->misafir_sonraki_mac;
                flag = 1;
            }
        }
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_tum_takimlar(struct takimlar **_takimlar)
{
    int i;
    system("cls");
    printf("\n\n\tTakim Kodu Takim Adi            OO GG BB MM AA YY AVR Puan\n");
    printf("\t---------- -------------------- -- -- -- -- -- -- --- ----\n");
    for(i = 0; i < 99; i++)
    {
        if(_takimlar[i] != NULL)
        {
            printf("\t    %-2d     %-20s %2d %2d %2d %2d %2d %2d %3d  %2d\n",
                   _takimlar[i]->takim_kodu,_takimlar[i]->takim_adi,
                   _takimlar[i]->galibiyet_say+_takimlar[i]->beraberlik_say+_takimlar[i]->maglubiyet_say,
                   _takimlar[i]->galibiyet_say,_takimlar[i]->beraberlik_say,_takimlar[i]->maglubiyet_say,
                   _takimlar[i]->attigi_gol_say,_takimlar[i]->yedigi_gol_say,
                   _takimlar[i]->attigi_gol_say - _takimlar[i]->yedigi_gol_say,
                   _takimlar[i]->galibiyet_say*3+_takimlar[i]->beraberlik_say);
        }
    }
    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();
}
void menu_en_cok_atan_ve_yiyen(struct takimlar **_takimlar)
{
    int i;
    struct takimlar *en_cok_atan;
    struct takimlar *en_az_yiyen;
    int en_az_gol = 99;
    int en_cok_gol = 0;
    for(i = 0; i < 99; i++)
    {
        if(_takimlar[i] != NULL)
        {
            if(_takimlar[i]->attigi_gol_say > en_cok_gol)
            {
                en_cok_gol = _takimlar[i]->attigi_gol_say;
                en_cok_atan = _takimlar[i];
            }
            if(_takimlar[i]->yedigi_gol_say < en_az_gol)
            {
                en_az_gol = _takimlar[i]->yedigi_gol_say;
                en_az_yiyen = _takimlar[i];
            }
        }
    }
    printf("\n\n\tTakim Kodu Takim Adi            OO GG BB MM AA YY AVR Puan\n");
    printf("\t---------- -------------------- -- -- -- -- -- -- --- ----\n");

    printf("\t    %-2d     %-20s %2d %2d %2d %2d %2d %2d %3d  %2d\n",
           en_cok_atan->takim_kodu,en_cok_atan->takim_adi,
           en_cok_atan->galibiyet_say+en_cok_atan->beraberlik_say+en_cok_atan->maglubiyet_say,
           en_cok_atan->galibiyet_say,en_cok_atan->beraberlik_say,en_cok_atan->maglubiyet_say,
           en_cok_atan->attigi_gol_say,en_cok_atan->yedigi_gol_say,
           en_cok_atan->attigi_gol_say - en_cok_atan->yedigi_gol_say,
           en_cok_atan->galibiyet_say*3+en_cok_atan->beraberlik_say);

    printf("\t    %-2d     %-20s %2d %2d %2d %2d %2d %2d %3d  %2d\n",
           en_az_yiyen->takim_kodu,en_az_yiyen->takim_adi,
           en_az_yiyen->galibiyet_say+en_az_yiyen->beraberlik_say+en_az_yiyen->maglubiyet_say,
           en_az_yiyen->galibiyet_say,en_az_yiyen->beraberlik_say,en_az_yiyen->maglubiyet_say,
           en_az_yiyen->attigi_gol_say,en_az_yiyen->yedigi_gol_say,
           en_az_yiyen->attigi_gol_say - en_az_yiyen->yedigi_gol_say,
           en_az_yiyen->galibiyet_say*3+en_az_yiyen->beraberlik_say);

    printf("\nMenuye donmek icin bir tusa basin...\n");
    fflush(stdin);
    getchar();

}
void menu_deplasmanda_en_cok_atan(struct takimlar **_takimlar)
{

}
void menu_golculer(struct takimlar **_takimlar,struct futbolcular *_futbolcular)
{

}
struct futbolcular *futbolcuAra(struct futbolcular *liste_basi_ptr, int no)
{
    struct futbolcular *gecici;

    gecici=liste_basi_ptr->sonraki;
    while (gecici!=liste_basi_ptr)
    {
        if (gecici->forma_no==no)
            return gecici;
        gecici=gecici->sonraki;
    }
    return NULL;
}
void futbolcuEkle(struct futbolcular *liste_basi_ptr, struct futbolcular *yeni)
{
    struct futbolcular *onceki, *gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->sonraki;
    while(gecici!=liste_basi_ptr && gecici->forma_no<yeni->forma_no)
    {
        onceki=gecici;
        gecici=gecici->sonraki;
    }
    yeni->sonraki=gecici;
    onceki->sonraki=yeni;
}
struct futbolcular *futbolcuCikar(struct futbolcular *liste_basi_ptr, int no)
{
    struct futbolcular *onceki, *gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->sonraki;
    while(gecici!=liste_basi_ptr && gecici->forma_no<no)
    {
        onceki=gecici;
        gecici=gecici->sonraki;
    }
    if(gecici==liste_basi_ptr || gecici->forma_no>no)
        return NULL;
    else
    {
        onceki->sonraki=gecici->sonraki;
        return gecici;
    }
}
/*
struct maclar *macAra(struct takimlar **_takimlar,int kod, int hafta_no)
{
    struct maclar *gecici = _takimlar[kod]->ilk_maci;
    while(gecici != NULL)
    {
        if(gecici->hafta_no == hafta_no)
        {
            return gecici;
        }
        gecici = gecici->
    }

};*/
