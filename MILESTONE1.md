# Cara kerja Interrupt

BIOS Interrupt merupakan fungsi yang disediakan oleh Operating System untuk memanggil Basic Input Output System (BIOS). Dengan interrupt, OS dapat melakukan fungsi input dan output dasar. 

Ketika interrupt dipanggil, CPU akan menghentikan eksekusi program utama untuk menjalankan program yang disebut dengan Interrupt SErvice Routine (ISR). Program tersebut tidak hanya satu, namun beraneka ragam. ISR yang dipanggil dengan mengakses starting-point address yang disebut sebagai interrupt vector. Untuk mengakses suatu fungsi dari ISR, perlu memasukkan nilai AH yang sesuai pada register AX.

# Cara kerja kernel.asm:
Pada kernel.asm akan diimplementasikan fungsi putInMemory, interrupt, makeInterrupt21, dan akan digunakan fungsi handleInterrupt21 yang diimplementasikan pada file kernel.c dengan menulis extern handleInterrupt21.

Cara kerja fungsi putInMemory adalah memindahkan parameter ketiga dari fungsi ke memory address (parameter pertama ditambah parameter kedua). 

Cara kerja fungsi interrupt adalah memindahkan parameter kedua ke dalam register ax, memindahkan parameter kedua ke dalam register bx, memindahkan parameter kedua ke dalam register cx, memindahkan parameter kedua ke dalam register dx, lalu memanggil interrupt sesuai parameter pertama, 

Cara kerja Fungsi makeInterrupt21 adalah 
