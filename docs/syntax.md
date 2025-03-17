# Interpreter Sözdizim Dokümantasyonu

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

## Fonksiyon Çağırma
```interpereter (Dil için bir isin oluşturulacak)
    <fonksiyon_ismi>(<parametre1>, <parametre2>, ...)
```

## Koşullu İfadeler
```interpereter (Dil için bir isin oluşturulacak)
    eğer <koşul_ifadesi> {
        // ifadeler
    } yoksa eğer {
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