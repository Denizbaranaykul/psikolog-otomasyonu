#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct m
{
    int online[3];  // Online durumları
    int mekan[6][5];  // 6 gün x 5 saatlik dilimler
} m;

void hasta_isimlerini_listele()
{
    FILE *arsiv = fopen("arsiv.txt", "r");
    if (arsiv == NULL)
    {
        printf("Dosya acilmadi.\n");
        return;
    }

    printf("Mevcut Hastalar:\n");
    printf("----------------\n");

    char isim[20], soyisim[20];
    long long telefon, kimlik;
    int yas;
    int sayac = 1;

    while (fscanf(arsiv, "%s %s %lld %lld %d", isim, soyisim, &telefon, &kimlik, &yas) != EOF)
    {
        printf("%d - %s %s\n", sayac++, isim, soyisim);
    }
    fclose(arsiv);
}


void hasta_arsivini_goster()
{
    FILE *arsiv = fopen("arsiv.txt", "r");
    if (arsiv == NULL)
    {
        printf("Dosya acilmadi.\n");
        return;
    }

    printf("isim\t        soyisim\t        telefon\t         kimlik\t        yas\n");
    printf("-------------------------------------------------------------------\n");
    char isim[20], soyisim[20];
    long long telefon, kimlik;
    int yas;

    while (fscanf(arsiv, "%s %s %lld %lld %d", isim, soyisim, &telefon, &kimlik, &yas) != EOF)
    {
        printf("%-10s\t%-10s\t%-15lld\t%-15lld\t%-5d\n", isim, soyisim, kimlik, telefon, yas);
    }
    fclose(arsiv);
}

void yazdirma(m *mekan)
{
    printf("Randevu Tanimlama\n");
    printf("Pazartesi      Sali      Carsamba       Persembe         Cuma        Cumartesi\n");

    for (int j = 0; j < 6; j++)  // Günler
    {
        for (int h = 0; h < 5; h++)  // Saat dilimleri
        {
            if (mekan->mekan[h][j] == 0)
            {
                printf("\033[31mBos         \033[0m");
            }
            else
            {
                printf("\033[32mDolu        \033[0m");
            }
        }
        printf("      %d : 00\n", j + 12);  // Saat bilgisi
    }
}


void mekani_dosyadan_oku(m *mekan)
{
    FILE *dosya = fopen("mekan_durumu.txt", "r");
    if (dosya == NULL)
    {
        // Dosya yoksa tüm mekanları boş olarak başlat
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                mekan->mekan[j][i] = 0;  // 0: Boş
            }
        }
        return;
    }

    // Dosyadan mekan durumunu oku
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fscanf(dosya, "%d", &mekan->mekan[j][i]);
        }
    }
    fclose(dosya);
}

void mekani_dosyaya_yaz(m *mekan)
{
    FILE *dosya = fopen("mekan_durumu.txt", "w");
    if (dosya == NULL)
    {
        printf("Mekan dosyasi yazmak icin acilamadi.\n");
        return;
    }

    // Mekan durumunu dosyaya yaz
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fprintf(dosya, "%d ", mekan->mekan[j][i]);
        }
        fprintf(dosya, "\n");
    }
    fclose(dosya);
}


int main()
{
    m *mekan = (m *)malloc(sizeof(m));
    if (mekan == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    char isim[20], soyisim[20];
    int secim = 1, yas = 0, gun = 0, saat = 0;
    long long telefon = 0, kimlik = 0;
    mekani_dosyadan_oku(mekan);

    while (secim != 0)
    {
        printf("DANISAN BILGILERI\n\n");
        printf("1-Yeni kayit\n");
        printf("2-Danisan arsivi\n");
        printf("3-Randevu Tanimla\n");
        printf("4-Randevu Degistirme\n");
        printf("5-Kayitli Randevulari Goruntule\n");
        printf("0-Cikis\n");
        printf("Lutfen secim yapiniz: ");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            printf("Yeni kayit\n");
            printf("1-Isim: ");
            scanf("%s", isim);
            printf("2-Soy Isim: ");
            scanf("%s", soyisim);
            printf("3-Yas: ");
            scanf("%d", &yas);
            printf("4-TC no: ");
            scanf("%lld", &kimlik);
            printf("5-Telefon no: ");
            scanf("%lld", &telefon);

            FILE *dosya = fopen("arsiv.txt", "a");
            if (dosya == NULL)
            {
                printf("Dosya acilamadi\n");
            }
            else
            {
                fprintf(dosya, "%s\t%s\t%lld\t%lld\t%d\n", isim, soyisim, kimlik, telefon, yas);
                fclose(dosya);
            }
            break;

        case 2:
            hasta_arsivini_goster();
            break;

        case 3:
    printf("Randevu Tanimla\n");
    hasta_isimlerini_listele();

    printf("Lutfen randevu tanimlamak istediginiz hastanin ismini giriniz: ");
    scanf("%s", isim);

    yazdirma(mekan);
    printf("1-Pazartesi, 2-Sali, 3-Carsamba, 4-Persembe, 5-Cuma, 6-Cumartesi\n");
    printf("Lutfen gun seciniz: ");
    scanf("%d", &gun);
    printf("Saat secimi (12-17): ");
    scanf("%d", &saat);

    if (gun > 0 && gun < 7 && saat > 11 && saat < 18)
    {
        mekan->mekan[saat - 12][gun - 1] = 1;  // Randevu oluştur
        printf("%s icin randevu basariyla olusturuldu.\n", isim);

        // Randevu bilgilerini dosyaya kaydet
        FILE *randevu_dosya = fopen("randevular.txt", "a");
        if (randevu_dosya == NULL)
        {
            printf("Randevu bilgileri dosyasina yazilamadi.\n");
        }
        else
        {
            const char *gunler[] = {"Pazartesi", "Sali", "Carsamba", "Persembe", "Cuma", "Cumartesi"};
            fprintf(randevu_dosya, "Hasta: %s, Gun: %s, Saat: %d:00\n", isim, gunler[gun - 1], saat);
            fclose(randevu_dosya);
        }
    }
    else
    {
        printf("Gecersiz gun veya saat secimi.\n");
    }
    mekani_dosyaya_yaz(mekan);
    yazdirma(mekan);
    break;



        case 4:
            yazdirma(mekan);
            printf("1-Pazartesi, 2-Sali, 3-Carsamba, 4-Persembe, 5-Cuma, 6-Cumartesi\n");
            printf("Lutfen gun seciniz: ");
            scanf("%d", &gun);
            printf("Saat secimi (12-17): ");
            scanf("%d", &saat);
            if (gun > 0 && gun < 7 && saat > 11 && saat < 18)
            {
                mekan->mekan[saat - 12][gun - 1] = 0;  // Randevu iptal et
                printf("Randevu basariyla iptal edildi.\n");
            }
            mekani_dosyaya_yaz(mekan);
            yazdirma(mekan);
            break;
            case 5:
    printf("Kayitli Randevular\n");
    printf("------------------\n");

    FILE *randevu_dosya = fopen("randevular.txt", "r");
    if (randevu_dosya == NULL)
    {
        printf("Randevu dosyasi bulunamadi veya acilamadi.\n");
    }
    else
    {
        char satir[100];
        while (fgets(satir, sizeof(satir), randevu_dosya) != NULL)
        {
            printf("%s", satir);  // Dosyadaki her satırı yazdır
        }
        fclose(randevu_dosya);
    }
    break;


        default:
            break;
        }
    }

    free(mekan);
    return 0;
}