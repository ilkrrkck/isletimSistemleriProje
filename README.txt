İŞLETİM SİSTEMLERİ PROJE ÖDEVİ
G191210351 - Eren Can Sarı
B191210351 - İlker Küçücük
G151210078 - Buğra Muaz Müjde

   Öncelikle kabuğa yazılan komut , komut ve parametlerine parse fonksiyonu sayesinde ayrılıyor.Parse fonksiyonu içerisinde kullanıcıdan alınan 
komut içerisinde '\n' , '\0' veya ';' görülene kadar tüm komutlar parametlerine ayrılarak argv değişkeni içerisine atanıyor.Sonrasında "Threadle" 
fonksiyonunda komutlar çalıştırılır, eğer  birden fazla komut tespit edilir ise  bu komutlar tek tek parametleriyle beraber thread processlerine gönderilip 
execute fonksiyonu içerisinde istenilen şartları sağladığı taktirde eş zamanlı bir şekilde çalıştırılıyor.
   Eğer bir batch file üzerinden komut çalıştırılmak istenirse çalıştırılacak komut shell ve parametresi dosya ise dosyanın 
içerisindeki tüm  komutlar parse ile tekrardan komutlarına ve parametrelerine ayrılarak yenide threadle fonksiyonu içerisinde 
threadlere gönderilerek eş zamanlı olarak tüm komutlar çalıştırılıyor.
   Ödevde talep edilenleri hatasız yaptık.Tek eksiğimiz "shell" komutu ile dosya okutulduğunda okutulan dosyanın komutları 
yanyana ve noktalı virgül olmalı onun yerine boşluklu olursa program hata verebiliyor.Onun dışında başka bir eksiğimiz oldu-
ğunu düşünmüyoruz.Kabuğumuz geçersiz bir komut veya boşluk yollandığında da "***Komut bulunamadı" hatasını kullanıcıya 
iletiliyor.
  
  

