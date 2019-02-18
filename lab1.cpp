#include <iostream>
#include <fstream>

using namespace std;

#pragma pack(1)

struct BMP {
    unsigned char *header[54];
    unsigned char *body[4 * 256];
} bmp;

int main(){
    ifstream fin;
    ofstream ofs;
    fin.open("../img1.bmp", ios::binary);
    ofs.open("../img2.bmp", ios::binary);
    if (fin.is_open()) {
        cout << "Open!";
    } else {
        return -1;
    }
    fin.seekg (0, ios::end);
    long length = static_cast<long>(fin.tellg());
    fin.seekg (0, ios::beg);

    fin.read(reinterpret_cast<char*>(&bmp.header), 54);
    ofs.write(reinterpret_cast<const char *>(bmp.header), 54);
    unsigned char color1[1];
    unsigned char color2[1];
    unsigned char color3[1];
    unsigned char reserve[1];
    for(int i = 0; i < 256; ++i){
        fin.read(reinterpret_cast<char *>(&color1), 1);
        fin.read(reinterpret_cast<char *>(&color2), 1);
        fin.read(reinterpret_cast<char *>(&color3), 1);
        fin.read(reinterpret_cast<char *>(&reserve), 1);
        // cout << color1 << " " << color2 << " " << color3 << endl;
        int sum = int(color1[0]) + int(color2[0]) + int(color3[0]);
        // cout << sum / 3 << endl;
        unsigned char output[1];
        output[0] = static_cast<unsigned char>(sum / 3);
        ofs.write(reinterpret_cast<const char *>(output), 1);
        ofs.write(reinterpret_cast<const char *>(output), 1);
        ofs.write(reinterpret_cast<const char *>(output), 1);
        ofs.write(reinterpret_cast<const char *>(reserve), 1);
    }
    unsigned char symb[1];
    while (!fin.eof()) {
        if (fin.eof())
            break;
        fin.read(reinterpret_cast<char *>(&symb), 1);
        ofs.write(reinterpret_cast<const char *>(&symb), 1);
    }
    ofs.close();
    fin.close();

    return 0;
}