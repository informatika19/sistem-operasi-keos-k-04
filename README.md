# Cara kerja Interrupt

BIOS Interrupt merupakan fungsi yang disediakan oleh Operating System untuk memanggil Basic Input Output System (BIOS). Dengan interrupt, OS dapat melakukan fungsi input dan output dasar. 

Ketika interrupt dipanggil, CPU akan menghentikan eksekusi program utama untuk menjalankan program yang disebut dengan Interrupt SErvice Routine (ISR). Program tersebut tidak hanya satu, namun beraneka ragam. Data ISR disimpan dalam alamat memori yang disebut sebagai interrupt vector, yang perlu diakses untuk pemanggilan ISR.