#define MAX_CODE_LENGTH 16

// Pembuatan Node
struct node {
    char nomorTelefon[15];
    char password[20];
    int pulsa;
    int kuota;
    char paketData[50];     // Tambahkan elemen ini untuk mencatat paket data yang dibeli
    char tanggalKadaluarsa[11]; // Tambahkan elemen ini untuk mencatat tanggal kadaluarsa
    struct node *next;
    struct node *previous;
};

// menginisialisasikan pointer untuk node dan head
typedef struct node Node;
typedef Node *NodePtr;
NodePtr head = NULL;

// Function Prototype
NodePtr createNode(char nomorTelefon[], char password[], int pulsa, int kuota, char paketData[], char tanggalKadaluarsa[]);
void insertNode(NodePtr *head, char nomorTelefon[], char password[], int pulsa, int kuota, char paketData[], char tanggalKadaluarsa[]);
void loadDataFromFile(char *filename, NodePtr *head);
void displayList(NodePtr head);
NodePtr searchUser(NodePtr head, char nomorTelefon[], char password[]);
void updateUserDataFile(NodePtr head, char *filename);
void pilihan1();
void tambahKuota(NodePtr user);
void rekomendasiPaket(int pulsa);
int redeemPulsa(const char *filename, const char *code);
void pilihan2();
void pilihan3();
void generateUniqueCode(char *code, int length);
void pilihan4();
void pilihan5();
Node* merge(Node *first, Node *second);
Node* mergeSort(Node *head);
void cetakInvoice(const char* nomorTelefon, const char* paketData, int harga, int diskon, int hargaSetelahDiskon);



// admin mode
NodePtr* searchNomor(NodePtr head, const char *partialPhoneNumber, int *count);
void sortData(NodePtr *head, int sortBy, int ascending);
void deleteAccount(NodePtr *head, char nomorTelefon[]);
void editAccount(NodePtr node);
void tampilkanHasilPenjualan();
void cetakHasilPenjualanKeFile();

// Function mengenai tanggal kadaluarsa
void hitungTanggalKadaluarsa(char *tanggalKadaluarsa, int hariTambah);
void periksaPaketKedaluwarsa(NodePtr user);
void simpanRiwayatPembelian(char *paketData, char *tanggalPembelian, int harga);
void tanggalSekarang(char *tanggal);

// Simulasi penggunaan kuota
void penggunaanKuotaOtomatis(NodePtr head);
int generateRandomUsage(int maxKuota);
int generateRandomDiscount();




// Function untuk mendapatkan password dengan tampilan '*'
void getPassword(char* password) {
    int index = 0;
    char ch;

    while ((ch = _getch()) != '\r') { // '\r' adalah carriage return (Enter key)
        if (ch == 8) { // 8 adalah kode ASCII untuk backspace
            if (index > 0) {
                index--;
                printf("\b \b"); // Menghapus karakter terakhir yang ditampilkan
            }
        } else if (ch == 13) { // 13 adalah kode ASCII untuk Enter
            break;
        } else {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0'; // Null-terminate the password string
    printf("\n");
}

// Memberikan rekomendasi paket kepada user berdasarkan sisa pulsa
void rekomendasiPaket(int pulsa) {
    printf("\nRekomendasi paket berdasarkan sisa pulsa Anda (Rp %d):\n", pulsa);
    if (pulsa >= 65000) {
        printf("- 15GB/30 hari Rp 65000\n");
    } 
    else if (pulsa >= 50000) {
        printf("- 10GB/30 hari Rp 50000\n");
    } 
    else if (pulsa >= 30000) {
        printf("- 5GB/30 hari Rp 30000\n");
    } 
    else if (pulsa >= 20000) {
        printf("- 2GB/30 hari Rp 20000\n");
    } 
    else if (pulsa < 20000) {
        printf("Sisa pulsa Anda tidak mencukupi untuk membeli paket data.\n");
    }
}


// Function pembuatan node baru
NodePtr createNode(char nomorTelefon[], char password[], int pulsa, int kuota, char paketData[], char tanggalKadaluarsa[]) {
    NodePtr newNode = (NodePtr)malloc(sizeof(Node));
    if (newNode != NULL) {
        strcpy(newNode->nomorTelefon, nomorTelefon);
        strcpy(newNode->password, password);
        newNode->pulsa = pulsa;
        newNode->kuota = kuota;
        strcpy(newNode->paketData, paketData);
        strcpy(newNode->tanggalKadaluarsa, tanggalKadaluarsa);
        newNode->next = NULL;
        newNode->previous = NULL;
    }
    return newNode;
}

// Function untuk memasukkan node ke dalam list
void insertNode(NodePtr *head, char nomorTelefon[], char password[], int pulsa, int kuota, char paketData[], char tanggalKadaluarsa[]) {
    NodePtr newNode = createNode(nomorTelefon, password, pulsa, kuota, paketData, tanggalKadaluarsa);
    if (*head == NULL) {
        *head = newNode;
    } else {
        NodePtr temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->previous = temp;
    }
}	

// Function untuk load data-data user dari text file ke dalam linked list
void loadDataFromFile(char *filename, NodePtr *head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    char nomorTelefon[15];
    char password[20];
    int pulsa, kuota;
    char paketData[50];
    char tanggalKadaluarsa[11];

    while (fscanf(file, "%s %s %d %d %s %s", nomorTelefon, password, &pulsa, &kuota, paketData, tanggalKadaluarsa) == 6) {
        insertNode(head, nomorTelefon, password, pulsa, kuota, paketData, tanggalKadaluarsa);
    }

    fclose(file);
}


// Function untuk print linked list yang telah dibuat. Digunakan hanya untuk troubleshooting
void displayList(NodePtr head) {
    NodePtr current = head;

    printf("\n==============================================================================================================\n");
    printf("|                                              Data Pelanggan                                                |\n");
    printf("==============================================================================================================\n");
    printf("|    Nomor Telepon    |     Password     |  Pulsa (Rp)  | Kuota (GB)  |   Paket Data    | Tanggal Kadaluarsa |\n");
    printf("==============================================================================================================\n");

    while (current != NULL) {
        printf("| %-19s | %-16s | %-12d | %-11d | %-15s | %-18s |\n", 
               current->nomorTelefon, current->password, current->pulsa, current->kuota, current->paketData, current->tanggalKadaluarsa);
        current = current->next;
    }

    printf("==============================================================================================================\n");
}



// Function untuk mencari user di dalam list
NodePtr searchUser(NodePtr head, char nomorTelefon[], char password[]) {
    NodePtr current = head;

    while (current != NULL) {
        if (strcmp(current->nomorTelefon, nomorTelefon) == 0 && strcmp(current->password, password) == 0) {
            return current; 
        }
        current = current->next; 
    }
    return NULL;
}


// Function untuk mengupdate user_data.txt
void updateUserDataFile(NodePtr head, char *filename) {
    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
        printf("Gagal membuka file untuk pembaruan!\n");
        system("pause");
        return;
    }
    
    NodePtr current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %d %d %s %s\n", current->nomorTelefon, current->password, current->pulsa, current->kuota, 
                strcmp(current->paketData, "") == 0 ? "N/A" : current->paketData, 
                strcmp(current->tanggalKadaluarsa, "") == 0 ? "N/A" : current->tanggalKadaluarsa);
        current = current->next;
    }

    fclose(file);
}


// Pilihan 1 yaitu untuk mengecek ada paket data apa saja yang tersedia
void pilihan1() {
    printf("\033[1;36m"); 
    printf("\n====================================\n");
	printf("|  	 Daftar paket data 	   |\n");
    printf("====================================\n");
    printf("\033[0;32m");
    printf("| 1. |	2GB/30 hari Rp 20000       |\n");
	printf("+----+-----------------------------+\n");
    printf("| 2. |	5GB/30 hari Rp 30000	   |\n");
    printf("+----+-----------------------------+\n");
    printf("| 3. |	10GB/30 hari Rp 50000	   |\n");
    printf("+----+-----------------------------+\n");
    printf("| 4. |	15GB/30 hari Rp 65000	   |\n");
    printf("+----+-----------------------------+\n");
    system("pause");
}


// Function untuk pilihan menu utama 2, yaitu log in dan mengecek data kuota dan pulsa yang dimiliki oleh user
void pilihan2() {
    char nomorTelefon[15];
    char password[20];
    int pilihan;
    int pulsaDidapat;
    char inputKode[15];
    
    printf("\033[0;32m");
    printf("\n====================================\n");
    printf("|           Login Menu             |\n");
    printf("====================================\n");
    printf("\033[0m");
    printf("| Masukkan nomor telefon anda      |\n");
    printf("+----------------------------------+\n");
    printf("Input : ");
    scanf("%s", nomorTelefon);
    printf("+----------------------------------+\n");
    printf("| Masukkan password anda           |\n");
    printf("+----------------------------------+\n");
    printf("Input : ");
    getPassword(password);

    // Search nama user dalam linked list
    NodePtr user = searchUser(head, nomorTelefon, password);
    if (user != NULL) {
        // Periksa apakah paket data sudah kedaluwarsa
        periksaPaketKedaluwarsa(user);

        printf("\033[1;36m");
        printf("\nKuota Anda: %d GB\n", user->kuota);
        if(user->kuota == 0) {
        	printf("Kuota internet anda telah habis!\n");
		}
        printf("Pulsa Anda: Rp %d\n", user->pulsa);
        printf("Kadaluarsa: %s\n", user->tanggalKadaluarsa);
        printf("\033[0m");
    } else {
        printf("\033[0;31m");
        printf("\nNomor telefon atau password salah.\n");
        printf("\033[0m");
        system("pause");
        return;
    }
    
    printf("\033[1;36m"); 
    printf("\n====================================\n");
    printf("|            Menu Opsi             |\n");
    printf("====================================\n");
    printf("\033[0;32m");
    printf("| 1. | Beli paket data             |\n");
    printf("+----+-----------------------------+\n");
    printf("| 2. | Redeem kode pulsa           |\n");
    printf("+----+-----------------------------+\n");
    printf("| 0. | Kembali ke menu awal        |\n");
    printf("+----+-----------------------------+\n");
    printf("\033[0m");
    printf("Pilih opsi yang diinginkan: "); 
    scanf("%d", &pilihan);
    
    switch (pilihan) {
        case 1:
            tambahKuota(user);
            break;
        case 2:
            printf("+----------------------------------+\n");
            printf("| Masukkan kode top up pulsa       |\n");
            printf("+----------------------------------+\n");
            printf("Input : ");
            scanf("%s", inputKode);
            pulsaDidapat = redeemPulsa("kode.txt", inputKode);
            if (pulsaDidapat > 0) {
                printf("\033[1;36m");
                printf("\nPulsa berhasil ditambahkan: Rp %d\n", pulsaDidapat);
                printf("\033[0m");
                user->pulsa = user->pulsa + pulsaDidapat;
                updateUserDataFile(head, "user_data.txt");
                system("pause");
            } else {
                printf("\033[0;31m");
                printf("Kode pulsa tidak valid atau tidak ditemukan.\n");
                printf("\033[0m");
                system("pause");
            }
            break;
        case 0:
            return;
        default:
            printf("Pilihan invalid\n");
    }
}

// Function untuk menghitung tanggal kadaluarsa
void hitungTanggalKadaluarsa(char *tanggalKadaluarsa, int hariTambah) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    tm_info->tm_mday += hariTambah;
    mktime(tm_info);
    strftime(tanggalKadaluarsa, 11, "%Y-%m-%d", tm_info);
}

// Function untuk mendapatkan tanggal saat ini dalam format YYYY-MM-DD
void tanggalSekarang(char *tanggal) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(tanggal, 11, "%Y-%m-%d", tm_info);
}

// Function untuk memeriksa dan memperbarui status paket data kedaluwarsa dan akan otomatis mengubah kuota menjadi 0 dan tanggal kadaluarsa menjadi N/A
void periksaPaketKedaluwarsa(NodePtr user) {
    char tanggalHariIni[11];
    tanggalSekarang(tanggalHariIni);
	
    if (strcmp(user->tanggalKadaluarsa, tanggalHariIni) <= 0) {
    	printf("\nPaket internet anda telah kadaluarsa.\n");
        strcpy(user->paketData, "N/A");
        strcpy(user->tanggalKadaluarsa, "N/A");
        user->kuota = 0;
    }
}

// Function yang kembali lagi menampilkan paket data tersedia, kemudian membelinya
void tambahKuota(NodePtr user) {
    int pilihan;

    // Tampilkan rekomendasi paket berdasarkan sisa pulsa
    rekomendasiPaket(user->pulsa);

    // Generate random discount
    int diskon = generateRandomDiscount();
    printf("\033[0;32m");
    printf("Anda mendapatkan diskon sebesar %d%% untuk pembelian paket data!\n", diskon);
    printf("\033[0m");


    // Tampilkan daftar paket data dengan harga setelah diskon
    int harga2GB = 20000 - (20000 * diskon / 100);
    int harga5GB = 30000 - (30000 * diskon / 100);
    int harga10GB = 50000 - (50000 * diskon / 100);
    int harga15GB = 65000 - (65000 * diskon / 100);

    do {
        printf("\033[1;36m");
        printf("\n=====================================\n");
        printf("|       Daftar paket data           |\n");
        printf("=====================================\n");
        printf("\033[0;32m");
        printf("| %-3s | %-15s | Rp %-6d |\n", "1.", "2GB/30 hari", harga2GB);
        printf("+-----+-----------------+-----------+\n");
        printf("| %-3s | %-15s | Rp %-6d |\n", "2.", "5GB/30 hari", harga5GB);
        printf("+-----+-----------------+-----------+\n");
        printf("| %-3s | %-15s | Rp %-6d |\n", "3.", "10GB/30 hari", harga10GB);
        printf("+-----+-----------------+-----------+\n");
        printf("| %-3s | %-15s | Rp %-6d |\n", "4.", "15GB/30 hari", harga15GB);
        printf("+-----+-----------------+-----------+\n");
        printf("| %-3s | %-27s |\n", "0.", "Batal pembelian");
        printf("+-----+-----------------------------+\n");
        printf("\033[0m");
        printf("\nPilihlah paket yang anda inginkan: ");
        scanf("%d", &pilihan);
    } while (pilihan < 0 || pilihan > 4);

    int harga = 0;
    int kuota = 0;
    char paketData[50];
    char tanggalPembelian[11];

    switch (pilihan) {
        case 1:
            harga = 20000;
            kuota = 2;
            strcpy(paketData, "2GB/30");
            break;
        case 2:
            harga = 30000;
            kuota = 5;
            strcpy(paketData, "5GB/30");
            break;
        case 3:
            harga = 50000;
            kuota = 10;
            strcpy(paketData, "10GB/30");
            break;
        case 4:
            harga = 65000;
            kuota = 15;
            strcpy(paketData, "15GB/30");
            break;
    }

    int hargaSetelahDiskon = harga - (harga * diskon / 100);

    if (user->pulsa >= hargaSetelahDiskon) {
        user->pulsa -= hargaSetelahDiskon;
        user->kuota += kuota;
        strcpy(user->paketData, paketData);
        hitungTanggalKadaluarsa(user->tanggalKadaluarsa, 30);
        tanggalSekarang(tanggalPembelian);

        printf("\033[0;32m");
        printf("Pembelian berhasil! Sisa pulsa: %d, Kuota: %dGB\n", user->pulsa, user->kuota);
        printf("Paket: %s\n", user->paketData);
        printf("Kadaluarsa: %s\n", user->tanggalKadaluarsa);
        printf("\033[0m");

        // Simpan riwayat pembelian
        simpanRiwayatPembelian(user->paketData, tanggalPembelian, hargaSetelahDiskon);

        // Cetak invoice
        cetakInvoice(user->nomorTelefon, user->paketData, harga, diskon, hargaSetelahDiskon);

        system("pause");
        updateUserDataFile(head, "user_data.txt");
    } else {
        printf("\033[0;31m");
        printf("\nPulsa tidak cukup untuk membeli paket ini.\n");
        printf("\033[0m");
        system("pause");
    }
}


// Function untuk meredeem kode top up pulsa
int redeemPulsa(const char *filename, const char *code) {
    FILE* fptr;
    char read[MAX_CODE_LENGTH];
    int pulsaDitambah = 0;
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Gagal membuka file %s\\n", filename);
        return 0;
    }
    
    // Buka file temporary untuk menulis
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Gagal membuka file temporary\n");
        fclose(fptr);
        return 0;
    }
    
    int codeFound = 0; // Flag untuk code
    
    // Membaca kode dan jumlah pulsa dari file
    while (fscanf(fptr, "%s %d", read, &pulsaDitambah) == 2) {
        
        if(strcmp(code, read) == 0){
            codeFound = 1; // flag untuk ditemukan
            break;
        } else {
            // Copy baris ke file temporary jika kode tidak sesuai
            fprintf(tempFile, "%s %d\n", read, pulsaDitambah);
        }
    }
    
    fclose(fptr);
    fclose(tempFile);
    
    // meng overwrite file
    remove(filename);
    rename("temp.txt", filename);
    
    if (codeFound) {
        return pulsaDitambah;
    } else {
        return 0;
    }
}

// Function pilihan 3, yaitu untuk menambahkan user baru dan menyimpannya di file
void pilihan3() {
    char nomorTelefon[15];
    char password[20];
    char repeat[20];
    int pulsa = 0;
    int kuota = 0;
    char paketData[50] = "N/A";  // Default value
    char tanggalKadaluarsa[11] = "N/A";  // Default value
    
    printf("\033[0;32m");
    printf("\n====================================\n");
    printf("|           Register Menu           |\n");
    printf("====================================\n");
    printf("\033[0m");
    printf("| Masukkan nomor telefon anda       |\n");
    printf("+-----------------------------------+\n");
    printf("Input : ");
    scanf("%s", nomorTelefon);
    
    // Mengecek apakah nomor telepon sudah terdaftar sebelumnya
    if (searchUser(head, nomorTelefon, "") != NULL) {
        printf("\033[0;31m");
        printf("Nomor telepon sudah terdaftar dalam sistem. Silakan coba nomor telefon lain.\n");
        printf("\033[0m");
        system("pause");
        return;
    }
    
    do {
        printf("+-----------------------------------+\n");
        printf("| Buat password yang anda inginkan  |\n");
        printf("+-----------------------------------+\n");
        printf("Input : ");
        getPassword(password);
        printf("+-----------------------------------+\n");
        printf("| Masukkan kembali password         |\n");
        printf("+-----------------------------------+\n");
        printf("Input : ");
        getPassword(repeat);
        
        if (strcmp(password, repeat) != 0) {
            printf("\033[0;31m");
            printf("Password yang dimasukkan tidak sama. Silahkan diulangi.\n");
            printf("\033[0m");
        }        
    } while (strcmp(password, repeat) != 0);
    
    FILE *file = fopen("user_data.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }
    
    fprintf(file, "%s %s %d %d %s %s\n", nomorTelefon, password, pulsa, kuota, paketData, tanggalKadaluarsa);
    fclose(file);
    
    insertNode(&head, nomorTelefon, password, pulsa, kuota, paketData, tanggalKadaluarsa);
    
    printf("\033[0;32m");
    printf("\nNomor telefon, password, pulsa, dan kuota berhasil disimpan.\n");
    printf("\033[0m");
    system("pause");
}

// Function untuk membuat kode top up dengan random ASCII
void generateUniqueCode(char *code, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
    int i;
    srand(time(NULL)); // Seed untuk rand()

    for (i = 0; i < length - 1; ++i) {
        int index = rand() % (int)(sizeof(charset) - 1);
        code[i] = charset[index];
    }
    code[length - 1] = '\0'; // Null terminator
}

// Function untuk pilihan 4 yaitu membuat kode untuk membuat kode top up pulsa
void pilihan4() {
    int tambahPulsa;
    printf("\033[0;32m");
    printf("+----------------------------------------------------+\n");
    printf("| Masukkan jumlah pulsa yang ingin ditambahkan (Rp)  |\n");
    printf("+----------------------------------------------------+\n");
	printf("\033[0m");
    printf("Input : Rp ");
    scanf("%d", &tambahPulsa);
    
    char code[16]; // Kode berisi 15 char dengan 1 null terminator
    generateUniqueCode(code, 16); 
    
    // Simpan kode dan jumlah pulsa ke dalam file kode.txt
    FILE *file = fopen("kode.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file kode.txt!\n");
        system("pause");
        return;
    }
    
    fprintf(file, "%s %d\n", code, tambahPulsa);
    fclose(file);
    
    printf("Kode unik berhasil dibuat, silahkan liat kode di kode.txt.\n", code);
    system("pause");
}

// Function untuk memberikan bantuan mengenai cara penggunaan program
void pilihan5() {
	printf("\033[1;36m");
    printf("\n=================================================================================================\n");
    printf("|  			Papan Bantuan Sistem Pembelian Paket Internet  		                |\n");
    printf("=================================================================================================\n");
    printf("\033[0m");
   	printf("1. Lihat paket data yang tersedia: Menampilkan daftar paket data yang tersedia beserta harganya.\n");
    printf("2. Login data: Masuk ke akun pengguna untuk mengecek kuota dan pulsa,"); 
	printf("\n");
	printf("   serta melakukan pembelian paket data atau redeem kode pulsa.\n");
    printf("3. Daftar nomor telefon baru: Mendaftarkan nomor telefon baru ke dalam sistem.\n");
    printf("4. Beli token pulsa: Menghasilkan kode unik untuk top up pulsa.\n");
    printf("5. Bantuan: Menampilkan bantuan ini untuk membantu pengguna memahami penggunaan program.\n");
    printf("0. Keluar dari program: Keluar dari program.\n");
    printf("+-----------------------------------------------------------------------------------------------+\n\n");
    system("pause");
}

// Function untuk mencetak invoice ke file
void cetakInvoice(const char* nomorTelefon, const char* paketData, int harga, int diskon, int hargaSetelahDiskon) {
    char date[11];
    tanggalSekarang(date);

    // Nama file invoice
    char filename[50];
    sprintf(filename, "invoice_%s_%s.txt", nomorTelefon, date);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuat file invoice!\n");
        return;
    }

    fprintf(file, "====================================\n");
    fprintf(file, "             INVOICE                \n");
    fprintf(file, "====================================\n");
    fprintf(file, "Nomor Telepon       : %s\n", nomorTelefon);
    fprintf(file, "Tanggal Pembelian   : %s\n", date);
    fprintf(file, "Paket Data          : %s\n", paketData);
    fprintf(file, "Harga Asli          : Rp %d\n", harga);
    fprintf(file, "Diskon              : %d%%\n", diskon);
    fprintf(file, "Harga Setelah Diskon: Rp %d\n", hargaSetelahDiskon);
    fprintf(file, "====================================\n");
    fprintf(file, "Terima kasih telah membeli paket internet!\n");

    fclose(file);

    printf("Invoice berhasil dicetak ke file %s\n", filename);
}



// Bagian untuk mensortir data dengan cara mergesort

// Function merge dua sorted linked list
Node* merge(Node *first, Node *second) {
    if (first == NULL)
        return second;
    if (second == NULL)
        return first;

    if (strcmp(first->nomorTelefon, second->nomorTelefon) < 0) {
        first->next = merge(first->next, second);
        return first;
    } else {
        second->next = merge(first, second->next);
        return second;
    }
}

// Function dan algoritma mergeSort
Node* mergeSort(Node *head) {
    if (head == NULL || head->next == NULL)
        return head;

    Node *slow = head, *fast = head->next;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *second = slow->next;
    slow->next = NULL;

    // Menggunakan parallel programming agar lebih efesien
    #pragma omp task shared(head)
    head = mergeSort(head);
    #pragma omp task shared(second)
    second = mergeSort(second);
    #pragma omp taskwait

    return merge(head, second);
}

// Fungsi untuk mencari node berdasarkan nomor telepon (partial search)
NodePtr* searchNomor(NodePtr head, const char *partialPhoneNumber, int *count) {
    *count = 0;
    NodePtr *results = NULL;
    NodePtr current = head;

    while (current != NULL) {
        if (strstr(current->nomorTelefon, partialPhoneNumber) != NULL) {
            results = realloc(results, (*count + 1) * sizeof(NodePtr));
            results[*count] = current;
            (*count)++;
        }
        current = current->next;
    }
    return results;
}

// Fungsi untuk mengurutkan data menggunakan insertion sort
void sortData(NodePtr *head, int sortBy, int ascending) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    NodePtr sorted = NULL;
    NodePtr current = *head;

    while (current != NULL) {
        NodePtr next = current->next;
        if (sorted == NULL || 
            (ascending && 
                ((sortBy == 1 && strcmp(current->nomorTelefon, sorted->nomorTelefon) < 0) ||
                 (sortBy == 2 && current->kuota < sorted->kuota) ||
                 (sortBy == 3 && current->pulsa < sorted->pulsa))) ||
            (!ascending && 
                ((sortBy == 1 && strcmp(current->nomorTelefon, sorted->nomorTelefon) > 0) ||
                 (sortBy == 2 && current->kuota > sorted->kuota) ||
                 (sortBy == 3 && current->pulsa > sorted->pulsa)))) {
            current->next = sorted;
            sorted = current;
        } else {
            NodePtr temp = sorted;
            while (temp->next != NULL && 
                   ((ascending && 
                        ((sortBy == 1 && strcmp(current->nomorTelefon, temp->next->nomorTelefon) >= 0) ||
                         (sortBy == 2 && current->kuota >= temp->next->kuota) ||
                         (sortBy == 3 && current->pulsa >= temp->next->pulsa))) ||
                    (!ascending && 
                        ((sortBy == 1 && strcmp(current->nomorTelefon, temp->next->nomorTelefon) <= 0) ||
                         (sortBy == 2 && current->kuota <= temp->next->kuota) ||
                         (sortBy == 3 && current->pulsa <= temp->next->pulsa))))) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    *head = sorted;
}

// Function untuk mengedit account
void editAccount(NodePtr node) {
    if (node == NULL) {
        printf("Akun tidak ditemukan.\n");
        return;
    }
    
    char password[20];
    int pulsa, kuota;

    printf("Masukkan password baru: ");
    scanf("%s", password);
    printf("Masukkan pulsa baru: ");
    scanf("%d", &pulsa);
    printf("Masukkan kuota baru: ");
    scanf("%d", &kuota);

    strcpy(node->password, password);
    node->pulsa = pulsa;
    node->kuota = kuota;

    printf("Akun berhasil diedit.\n");
}
// Function untuk menghapus user berdasarkan nomor telepon
void deleteAccount(NodePtr *head, char nomorTelefon[]) {
    if (*head == NULL) {
        printf("List kosong. Tidak ada data yang dapat dihapus.\n");
        return;
    }

    NodePtr current = *head;
    while (current != NULL && strcmp(current->nomorTelefon, nomorTelefon) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Nomor telepon tidak ditemukan.\n");
        return;
    }

    if (current->previous != NULL) {
        current->previous->next = current->next;
    } else {
        *head = current->next;
    }

    if (current->next != NULL) {
        current->next->previous = current->previous;
    }

    free(current);
    printf("Akun berhasil dihapus.\n", nomorTelefon);
}

// Fungsi untuk mode admin
void adminMode() {
    int i;
    int pilihan;
    char nomorTelefon[15];
    int sortBy, ascending;
    int resultCount;
    char confirm[2];
    NodePtr *results;

    do {
        system("cls");
        displayList(head);

        printf("\033[1;36m");
        printf("\n=====================================\n");
        printf("|           Mode Admin              |\n");
        printf("=====================================\n");
        printf("\033[0m");
        printf("\033[1;33m");
        printf("| 1. | Tampilkan semua data         |\n");
        printf("+----+------------------------------+\n");
        printf("| 2. | Cari nomor telepon           |\n");
        printf("+----+------------------------------+\n");
        printf("| 3. | Urutkan data                 |\n");
        printf("+----+------------------------------+\n");
        printf("| 4. | Hasil Penjualan              |\n");
        printf("+----+------------------------------+\n");
        printf("| 5. | Cetak Hasil Penjualan        |\n");
        printf("+----+------------------------------+\n");
        printf("\033[0;31m");
        printf("| 0. | Kembali ke menu utama        |\n");
        printf("+----+------------------------------+\n\n");
        printf("\033[0m");

        printf("Masukkan pilihan menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                system("cls");
                displayList(head);
                system("pause");
                break;
            case 2:
                system("cls");
                printf("Masukkan sebagian nomor telepon yang ingin dicari: ");
                scanf("%s", nomorTelefon);
                results = searchNomor(head, nomorTelefon, &resultCount);
                if (resultCount > 0) {
                    printf("Ditemukan %d hasil:\n", resultCount);
                    for (i = 0; i < resultCount; i++) {
                        printf("%d. Nomor Telepon: %s\n", i + 1, results[i]->nomorTelefon);
                        printf("   Password: %s\n", results[i]->password);
                        printf("   Pulsa: %d\n", results[i]->pulsa);
                        printf("   Kuota: %d\n", results[i]->kuota);
                        printf("\n");
                    }
                    printf("Masukkan nomor hasil yang ingin diedit atau dihapus (0 untuk kembali): ");
                    int pilihanEdit;
                    scanf("%d", &pilihanEdit);
                    if (pilihanEdit > 0 && pilihanEdit <= resultCount) {
                        NodePtr selectedNode = results[pilihanEdit - 1];
                        printf("1. Edit\n");
                        printf("2. Hapus\n");
                        printf("Masukkan pilihan: ");
                        int action;
                        scanf("%d", &action);
                        if (action == 1) {
                            editAccount(selectedNode);
                        } else if (action == 2) {
                            printf("Yakin untuk delete account tersebut? (Masukkan 'Y' untuk confirm) : ");
                            scanf("%s", &confirm);
                            if(strcmp(confirm, "Y") == 0 ||strcmp(confirm, "y") == 0){
                                deleteAccount(&head, selectedNode->nomorTelefon);
                            } else {
                                printf("Akun batal untuk didelete.\n");
                            }

                        } else {
                            printf("Pilihan tidak valid.\n");
                        }
                    }
                    free(results);
                } else {
                    printf("Tidak ditemukan nomor telepon yang sesuai.\n");
                }
                system("pause");
                break;
            case 3:
                system("cls");
                printf("\033[1;36m");
                printf("\n====================================\n");
                printf("|           Urutkan Data            |\n");
                printf("====================================\n");
                printf("\033[0m");
                printf("\033[1;33m");
                printf("| 1. | Berdasarkan nomor telepon    |\n");
                printf("+----+-----------------------------+\n");
                printf("| 2. | Berdasarkan kuota            |\n");
                printf("+----+-----------------------------+\n");
                printf("| 3. | Berdasarkan pulsa            |\n");
                printf("+----+-----------------------------+\n");
                printf("\033[0;31m");
                printf("| 0. | Kembali                      |\n");
                printf("+----+-----------------------------+\n\n");
                printf("\033[0m");

                printf("Masukkan pilihan urutan: ");
                scanf("%d", &sortBy);
                if (sortBy != 0) {
                    printf("1 untuk Ascending, 0 untuk Descending: ");
                    scanf("%d", &ascending);
                    sortData(&head, sortBy, ascending);
                    printf("Data berhasil diurutkan.\n");
                }
                system("pause");
                break;
            case 4:
                system("cls");
                tampilkanHasilPenjualan();
                system("pause");
                break;
            case 5:
                system("cls");
                cetakHasilPenjualanKeFile();
                system("pause");
                break;
            case 0:
                return;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                system("pause");
                break;
        }
    } while (pilihan != 0);
}

// Fungsi untuk menampilkan hasil penjualan
void tampilkanHasilPenjualan() {
    FILE *file = fopen("riwayat_pembelian.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file riwayat pembelian!\n");
        return;
    }

    char paketData[50];
    char tanggalPembelian[11];
    int harga;
    int totalPenjualan = 0;

    printf("\n================================================================\n");
    printf("|                       Hasil Penjualan                        |\n");
    printf("================================================================\n");
    printf("|         Paket Data         | Tanggal Pembelian |    Harga    |\n");
    printf("================================================================\n");

    while (fscanf(file, "%s %s %d", paketData, tanggalPembelian, &harga) == 3) {
        printf("| %-24s | %-16s | %-10d |\n", paketData, tanggalPembelian, harga);
        totalPenjualan += harga;
    }

    printf("================================================================\n");
    printf("|                      Total Penjualan: Rp %-10d              |\n", totalPenjualan);
    printf("================================================================\n");

    fclose(file);
}

// Function untuk menambahkan riwayat pembelian ke file
void simpanRiwayatPembelian(char *paketData, char *tanggalPembelian, int harga) {
    FILE *file = fopen("riwayat_pembelian.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file riwayat pembelian!\n");
        return;
    }

    fprintf(file, "%s %s %d\n", paketData, tanggalPembelian, harga);
    fclose(file);
}

// Fungsi untuk mencetak hasil penjualan ke file
void cetakHasilPenjualanKeFile() {
    FILE *file = fopen("riwayat_pembelian.txt", "r");
    FILE *outputFile = fopen("laporan_penjualan.txt", "w");
    
    if (file == NULL) {
        printf("Gagal membuka file riwayat pembelian!\n");
        return;
    }
    
    if (outputFile == NULL) {
        printf("Gagal membuat file laporan penjualan!\n");
        fclose(file);
        return;
    }

    char paketData[50];
    char tanggalPembelian[11];
    int harga;
    int totalPenjualan = 0;

    fprintf(outputFile, "=======================================================================\n");
    fprintf(outputFile, "|                          Hasil Penjualan                            |\n");
    fprintf(outputFile, "=======================================================================\n");
    fprintf(outputFile, "| %-24s | %-19s | %-18s |\n", "Paket Data", "Tanggal Pembelian", "Harga (Rp)");
    fprintf(outputFile, "=======================================================================\n");

    while (fscanf(file, "%s %s %d", paketData, tanggalPembelian, &harga) == 3) {
        fprintf(outputFile, "| %-24s | %-19s | %-18d |\n", paketData, tanggalPembelian, harga);
        totalPenjualan += harga;
    }

    fprintf(outputFile, "=======================================================================\n");
    fprintf(outputFile, "| %-46s | %-2s%-15d |\n", "Total Penjualan:", "Rp ", totalPenjualan);
    fprintf(outputFile, "=======================================================================\n");

    fclose(file);
    fclose(outputFile);
    printf("Laporan penjualan berhasil dicetak ke file laporan_penjualan.txt\n");
}


// Bagian simulasi penggunaan kuota

// Function untuk mengenerate nominal random tidak melebihi kuota
int generateRandomUsage(int maxKuota) {
    if (maxKuota <= 0) {
        return 0;
    }
    return rand() % maxKuota + 1;
}

// Function untuk penggunaan kuota otomatis
void penggunaanKuotaOtomatis(NodePtr head) {
    NodePtr current = head;
    while (current != NULL) {
        int kuotaDigunakan = generateRandomUsage(current->kuota);
        if (kuotaDigunakan > 0) {
            current->kuota -= kuotaDigunakan;
            printf("Nomor Telepon: %s, Kuota Digunakan: %d GB, Sisa Kuota: %d GB\n",
                   current->nomorTelefon, kuotaDigunakan, current->kuota);
        }
        current = current->next;
    }
    updateUserDataFile(head, "user_data.txt");
}

// Function untuk mengenerate diskon acak (0% hingga 30%)
int generateRandomDiscount() {
    return rand() % 31; // Diskon antara 0% dan 30%
}

