# sistem-operasi-keos-k-04

1. mv
Utilitas yang digunakan untuk memindahkan file atau folder ke dalam folder tertentu. Fungsi mv dideklarasikan dan diimplementasikan pada kernel.c. 


Fungsi mv dipanggil melalui shell dan harus menerima 2 argumen: argumen pertama yaitu nama file/folder yang ingin dipindahkan dan argumen kedua yaitu nama folder tujuan pemindahan. Argumen pertama dan argumen kedua dipisahkan oleh spasi pertama setelah spasi antara 'mkdir' dengan argumen pertama

Kedua argumen harus berada pada path yang sama.

Apabila mv hanya memiliki 1 argumen atau setidaknya salah satu dari argumen tidak valid, maka mv gagal.

2. cp
Utilitas yang digunakan untuk mengcopy file/folder.


3. mkdir
Utilitas yang digunakan untuk membuat folder baru yang jumlahnya tergantung dari sudah ada atau tidaknya folder yang ingin dibuat. Digunakan juga oleh prosedur makeFile untuk menciptakan folder-folder penyimpan file yang diciptakan.

Fungsi mkdir dan makeFile dideklarasikan di file mkdir.h dan diimplementasikan di file mkdir.c. Apabila sektor file tidak dapat menampung jumlah directory baru, fungsi mkdir tidak dapat menjalankan tugasnya dengan benar.


4. cat
Utilitas yang digunakan untuk membaca isi dari file. Diimplementasikan secara langsung pada kernel.c dengan membaca isi dari sektor sectors dan mencetak hasilnya ke layar.


5. ln
Utilitas yang digunakan untuk membuat soft link pada sistem operasi yang dibuat.
