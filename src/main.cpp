#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "ast/astPrinter.hpp"
#include "interpreter/interpreter.hpp"

#define LEXER_TEST 0
#define AST_TEST 0

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

    #if LEXER_TEST
        for(const Token& token : tokens) {

            std::string str(token.start, token.length);
            std::cout
                    << ", Value: " << str
                    << ", Line: " << token.line
                    << ", Column: " << token.column << std::endl;
        }
    #endif

    Parser parser(tokens);
    std::unique_ptr<Program> program = parser.parse();

    #if AST_TEST
        std::cout << "===== AST AĞACI BAŞLANGIÇ =====" << std::endl;
        if (program->statements.empty()) {
            std::cout << "AST ağacı oluşturulamadı veya boş!" << std::endl;
        } else {
            ASTPrinter printer;
            printer.print(program.get());
        }
        std::cout << "===== AST AĞACI BİTİŞ =====" << std::endl;
    #endif

    Interpreter interpreter;
    interpreter.interpret(program.get());
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

// Todo: Atama sırasında tip kontrolü yapılacak (Tamamlandı)
// Todo: Aynı scopeda aynı isimli birden fazla değişken tanımlanamamalı (Tamamlandı)
// Todo: Sabitlere değer sonradan değer atanmaması sağlanacak (TAMAMLANDI)
// Todo: Fonksiyon çağrıları uygulanacak
