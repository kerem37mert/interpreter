# Interpreter Sözdizim Dokümantasyonu

## Veri Tipleri
```interpereter (Dil için bir isin oluşturulacak)
    <veri_tipi> = sayı | doğruluk | metin
```

## Değişken Tanımlama
```interpereter (Dil için bir isin oluşturulacak)
    deg <değişken_ismi>:<veri tipi> = <değer>
```

## Sabit Tanımlama
```interpereter (Dil için bir isin oluşturulacak)
    sbt <DEĞİŞKEN_İSMİ>:<veri tipi> = <değer>
```

## Fonksiyon Tanımlama
```interpereter (Dil için bir isin oluşturulacak)
    fonk <fonksiyon_ismi>:<dönüş_tipi> => <arg_1>:<veri_tipi>, <arg_2>:<veri_tipi>, ... {
        // ifaderler
        dön <değer>
    }
```

veya geri dönüş değeri olmayan fonksiyon

## Fonksiyon Tanımlama
```interpereter (Dil için bir isin oluşturulacak)
    fonk <fonksiyon_ismi> => <arg_1>:<veri_tipi>, <arg_2>:<veri_tipi>, ... {
        // ifaderler
        dön <değer>
    }
```

## Fonksiyon Çağırma
```interpereter (Dil için bir isin oluşturulacak)
    <fonksiyon_ismi>(<parametre1>, <parametre2>, ...)
```

## Koşullu İfadeler
```interpereter (Dil için bir isin oluşturulacak)
    eğer <koşul_ifadesi> {
        // ifadeler
    } yoksa eğer <koşul_ifadesi> {
        // ifadeler
    } yoksa {
        // ifadeler
    }
```

## Döngüler
```interpereter (Dil için bir isim oluşturulacak)
   döngü <koşul_ifadesi> {
    // ifadeler
   }
```

## Döngü Kontrol İfadeleri
```interpereter (Dil için bir isim oluşturulacak)
   dur        // döngüyü sonlandırır (break)
   devam      // döngünün bir sonraki iterasyonuna geçer (continue)
```

## Yaz Sözdizimi (Bir fonksiyon değildir, dilin yapısında olan bir anahtar kelimedir) UYARI: Standart Kütüphane yazıldıktan sonra kaldırılacak
```interpereter (Dil için bir isim oluşturulacak)
    yaz "Merhaba Dünya!"
```

## Operatörler
```interpereter (Dil için bir isim oluşturulacak)
    +  // Toplama işlemi ve metin birleştirme operatörü
    -  // Çıkarma işlemi operatörü
    *  // Çarpma işlemi operatörü
    /  // Bölme işlemi operatörü
    %  // Mod alma işlemi operatörü
    ** // Üs alma işlemi operatörü
    =  // Atama işlemi operatörü
    += // Toplayarak atama işlemi operatörü
    -= // Çıkartarak atama işlemi operatörü
    *= // Çarparak atama işlemi operatörü
    /= // Bölerek atama işlemi operatörü
    && // Ve işlemi operatörü
    || // Veya işlemi operatörü
    !  // Değil işlemi operatörü
    >  // Büyüktür kontrol operatörü
    <  // Küçüktür kontrol operatörü
    == // Eşitlik kontrol operatörü
    <= // Küçük eşit kontrol operatörü
    >= // Büyük eşit kontrol operatörü
    != // Eşit değil kontrol operatörü   
```