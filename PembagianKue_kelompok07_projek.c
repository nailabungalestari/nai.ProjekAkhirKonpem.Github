/* pembagian_kue.c
   Kompilasi: gcc pembagian_kue.c -o pembagian_kue
   Jalankan: ./pembagian_kue
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_distribution(int *dist, int n) {
    printf("\n--- Hasil Pembagian ---\n");
    for (int i = 0; i < n; ++i) {
        printf("Peserta %d : %d potong\n", i + 1, dist[i]);
    }
    printf("-----------------------\n");
}

void distribute_one_each(int *dist, int n) {
    for (int i = 0; i < n; ++i) dist[i] = 1;
}

void distribute_extra_merata(int *dist, int n, int extra) {
    // mulai dari peserta 0, bagi satu per satu hingga extra habis
    int idx = 0;
    while (extra > 0) {
        dist[idx % n] += 1;
        idx++;
        extra--;
    }
}

void distribute_extra_undian(int *dist, int n, int extra) {
    while (extra > 0) {
        int pick = rand() % n;
        dist[pick] += 1;
        extra--;
    }
}

void distribute_extra_specific(int *dist, int n, int extra) {
    printf("Masukkan indeks peserta yang mendapat sisa (1..%d). Masukkan 0 untuk selesai.\n", n);
    while (extra > 0) {
        int p;
        printf("Pilih peserta untuk 1 potong (sisa %d): ", extra);
        if (scanf("%d", &p) != 1) { // input invalid
            while (getchar() != '\n');
            printf("Input tidak valid. Coba lagi.\n");
            continue;
        }
        if (p == 0) {
            printf("Mengakhiri pemberian spesifik sebelum sisa habis. Sisa tetap: %d\n", extra);
            break;
        }
        if (p < 1 || p > n) {
            printf("Nomor peserta harus antara 1 sampai %d.\n", n);
            continue;
        }
        dist[p - 1] += 1;
        extra--;
    }
}

int main() {
    srand((unsigned) time(NULL));

    int peserta;
    int potongan;
    printf("=== Aplikasi Pembagian Kue ===\n");

    // Hitung jumlah peserta
    do {
        printf("Masukkan jumlah peserta: ");
        if (scanf("%d", &peserta) != 1) { while (getchar() != '\n'); peserta = 0; }
        if (peserta <= 0) printf("Jumlah peserta harus > 0. Coba lagi.\n");
    } while (peserta <= 0);

    // Masukkan jumlah potongan kue awal
    do {
        printf("Masukkan jumlah potongan kue yang dipotong: ");
        if (scanf("%d", &potongan) != 1) { while (getchar() != '\n'); potongan = -1; }
        if (potongan < 0) printf("Jumlah potongan tidak boleh negatif. Coba lagi.\n");
    } while (potongan < 0);

    // array distribusi
    int *dist = (int *) calloc(peserta, sizeof(int));
    if (!dist) {
        printf("Gagal alokasi memori.\n");
        return 1;
    }

    if (potongan == peserta) {
        // Langsung berikan 1 potong ke setiap peserta
        distribute_one_each(dist, peserta);
        printf("Jumlah potongan sama dengan jumlah peserta -> langsung dibagikan 1 potong tiap peserta.\n");
        print_distribution(dist, peserta);
    } else if (potongan > peserta) {
        // Ada sisa potongan
        int sisa = potongan - peserta;
        distribute_one_each(dist, peserta);
        printf("Jumlah potongan lebih dari peserta. Setiap peserta mendapat 1. Sisa: %d\n", sisa);

        // Tentukan sistem pembagian sisa
        int metode = 0;
        printf("\nPilih metode pembagian sisa:\n");
        printf("1 - Dibagikan merata (satu per peserta berulang)\n");
        printf("2 - Diundi secara acak\n");
        printf("3 - Diberikan ke peserta tertentu (input manual)\n");
        do {
            printf("Pilih (1/2/3): ");
            if (scanf("%d", &metode) != 1) { while (getchar() != '\n'); metode = 0; }
            if (metode < 1 || metode > 3) printf("Pilihan tidak valid. Coba lagi.\n");
        } while (metode < 1 || metode > 3);

        if (metode == 1) {
            distribute_extra_merata(dist, peserta, sisa);
        } else if (metode == 2) {
            distribute_extra_undian(dist, peserta, sisa);
        } else {
            distribute_extra_specific(dist, peserta, sisa);
        }
        print_distribution(dist, peserta);
    } else { // potongan < peserta
        printf("Jumlah potongan kurang dari jumlah peserta (%d < %d).\n", potongan, peserta);
        // opsi untuk menambah potongan (Potong Utang Kue)
        int addOption = 0;
        do {
            printf("Pilihan: 1 = tambahkan potongan sekarang, 2 = batalkan/keluar: ");
            if (scanf("%d", &addOption) != 1) { while (getchar() != '\n'); addOption = 0; }
            if (addOption != 1 && addOption != 2) printf("Pilihan tidak valid.\n");
        } while (addOption != 1 && addOption != 2);

        if (addOption == 2) {
            printf("Proses dibatalkan. Menampilkan kondisi: beberapa peserta tidak mendapat potongan.\n");
            // berbagi potongan sebanyak ada: gunakan undian untuk memilih penerima potongan yang terbatasi
            distribute_extra_undian(dist, peserta, potongan);
            print_distribution(dist, peserta);
        } else {
            int tambah;
            do {
                printf("Masukkan jumlah potongan tambahan yang dibuat (>= %d untuk mencapai minimal satu tiap peserta): ", peserta - potongan);
                if (scanf("%d", &tambah) != 1) { while (getchar() != '\n'); tambah = -1; }
                if (tambah < 0) printf("Masukkan harus >= 0.\n");
            } while (tambah < 0);
            potongan += tambah;
            printf("Sekarang total potongan = %d\n", potongan);

            if (potongan < peserta) {
                // masih kurang: gunakan undian untuk potongan yang tersedia
                printf("Masih kurang dari jumlah peserta. Akan mendistribusikan potongan yang ada secara undian.\n");
                distribute_extra_undian(dist, peserta, potongan);
                print_distribution(dist, peserta);
            } else {
                // cukup atau lebih
                distribute_one_each(dist, peserta);
                if (potongan > peserta) {
                    int sisa = potongan - peserta;
                    printf("Setiap peserta mendapat 1. Sisa: %d\n", sisa);
                    // minta metode pembagian sisa
                    int metode = 0;
                    printf("\nPilih metode pembagian sisa:\n");
                    printf("1 - Dibagikan merata\n");
                    printf("2 - Diundi\n");
                    printf("3 - Diberikan ke peserta tertentu\n");
                    do {
                        printf("Pilih (1/2/3): ");
                        if (scanf("%d", &metode) != 1) { while (getchar() != '\n'); metode = 0; }
                        if (metode < 1 || metode > 3) printf("Pilihan tidak valid. Coba lagi.\n");
                    } while (metode < 1 || metode > 3);
                    if (metode == 1) distribute_extra_merata(dist, peserta, sisa);
                    else if (metode == 2) distribute_extra_undian(dist, peserta, sisa);
                    else distribute_extra_specific(dist, peserta, sisa);
                }
                print_distribution(dist, peserta);
            }
        }
    }

    free(dist);
    printf("Selesai. Terima kasih.\n");
    return 0;
}
/*=== Aplikasi Pembagian Kue ===
Masukkan jumlah peserta: 12
Masukkan jumlah potongan kue yang dipotong: 12
Jumlah potongan sama dengan jumlah peserta -> langsung dibagikan 1 potong tiap peserta.

--- Hasil Pembagian ---
Peserta 1 : 1 potong
Peserta 2 : 1 potong
Peserta 3 : 1 potong
Peserta 4 : 1 potong
Peserta 5 : 1 potong
Peserta 6 : 1 potong
Peserta 7 : 1 potong
Peserta 8 : 1 potong
Peserta 9 : 1 potong
Peserta 10 : 1 potong
Peserta 11 : 1 potong
Peserta 12 : 1 potong
-----------------------
Selesai. Terima kasih.*/
/**/