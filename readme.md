# CLI Reminder (C)

Program reminder dengan tampilan sederhana menggunakan terminal, dibangun sepenuhnya menggunakan bahasa C.

Tujuan dikembangakan adalah untuk memperdalam pemahaman struct, interaksi I/O file dan management memori.





## Fitur
_(**Projek masih dalam tahap pengembangan**), beberapa fitur lanjutan belum ditambahkan, namun program masih dapat berjalan dengan fitur intinya._

Program ini berjalan sederhana, dapat menambahkan note dan melihat note yang sudah ditambahkan.

***untuk sekarang, hanya berjalan pada linux.**
## Ubah menjadi file eksekusi
mengubah `reminder.c` dan  `greetings.c` ke bentuk objek, supaya dapat dijalankan.
```
gcc reminder.c -o reminder
```
```
gcc greetings.c -o greetings
```

_program greetings ini akan bekerja maksimal jika diset berjalan otomatis ketika terminal dibuka_

```
...
# (otomatis berjalan ketika session terminal baru dibuka)

greetings
```
cukup tambahkan nama file program pada fila configurasi shell anda, `.bashrc` jika menggunakan bash.

# Penggunaan

#### menambah note.
```
$./reminder add d-m-yyyy

```

format `d-m-yyyy` menggunakan `-` wajib, jika tidak mencantumkan bulan atau tahun atau mememasukan bulan atau tahun lebih kecil dari sekarang atau angka tidak masuk akal, maka program akan menyimpulkan bulan dan tahun sekarang.

dengan index posisi pada yang dipisahakan dengan tanda `-` kamu dapat nyetel secara individu mana yang ingin diubah.

```
# contoh waktu sekatang 13/mei/2022


$./reminder add 13-5-2022           # format lengkap
$./reminder add --                  # tanggal kosong, akan dianggap tanggal sekarang
$./reminder add -6-                 # akan disimpan dengan tanggal 13 juni 2022
$./reminder add --2025              # akan disimpan dengan tanggal 13 juni 2025
$./reminder add 14                  # akan disimpan dengan tanggal 14 mei 2022
$./reminder add                     # format kosong, disimpan dengan tanggal sekarang

```

#### melihat note

gunnakan printah `greetings` jika tidak di set berjalan otomatis, untuk memuat buffer reminder yang jadwalakn hari ini
```
$./greetings
```
output
```
Selamat malam, User
(Selasa) 10 Juni        (*1), gunakan `reminder lookup`.
```

kemudian gunakan `reminder lookup` untuk memuat buffer lookup;
```
$./reminder lookup

note: 10-6-2025

[1] reminder demo
```

untuk melihat isi dari note, gunakan `./reminder serr index`, _index_ adalah nomor urut label pada output perintah `reminder lookup`.

```
$./reminder see 1

ini isi note reminder demo
```