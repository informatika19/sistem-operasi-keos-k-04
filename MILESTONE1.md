# Cara kerja Interrupt

BIOS Interrupt merupakan fungsi yang disediakan oleh Operating System untuk memanggil Basic Input Output System (BIOS). Dengan interrupt, OS dapat melakukan fungsi input dan output dasar. 

Ketika interrupt dipanggil, CPU akan menghentikan eksekusi program utama untuk menjalankan program yang disebut dengan Interrupt SErvice Routine (ISR). Program tersebut tidak hanya satu, namun beraneka ragam. ISR yang dipanggil dengan mengakses starting-point address yang disebut sebagai interrupt vector. Untuk mengakses suatu fungsi dari ISR, perlu memasukkan nilai AH yang sesuai pada register AX.