#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/lexer.hpp"

void run(const char* filePath) {
    std::ifstream file(filePath);

    if(file.fail()) {
        std::cerr << "[HATA] " << filePath << "yolunda dosya bulunamadı." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source, filePath);
    std::vector<Token> tokens = lexer.scanner();

    for(const Token& token : tokens) {

        std::string str(token.start, token.length);
        std::cout
                << ", Value: " << str
                << ", Line: " << token.line
                << ", Column: " << token.column << std::endl;
    }

}

int main(int argc, char* argv[]) {

    #ifdef _WIN32
        system("chcp 65001"); // Windows terminal Türkçe desteği için
    #endif

    setlocale(LC_ALL, "tr_TR.UTF-8");

    if(argc != 2) {
        std::cerr << "[Hata] Geçersiz komut satırı argüman sayısı" << std::endl;
        return 1;
    }

    run(argv[1]);

    return 0;
}

// Todo: Lexer'da String Literal için kaçış karakterleri eklenecek.
// Todo: Deterministik top-down algoritması araştırılacak.