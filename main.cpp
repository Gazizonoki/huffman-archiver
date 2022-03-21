#include "compress.h"
#include "decompress.h"

#include <cstring>

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        std::cout << "archiver -c archive_name file1 [file2 ...] - заархивировать файлы fil1, file2, ... и сохранить "
                     "результат в файл archive_name.\n\n";
        std::cout << "archiver -d archive_name - разархивировать файлы из архива archive_name и положить в текущую "
                     "директорию. Имена файлов должны сохраняться при архивации и разархивации.\n\n";
        std::cout << "archiver -h - вывести справку по использованию программы.\n\n";
        return 0;
    }

    if (argc >= 4 && strcmp(argv[1], "-c") == 0) {
        Writer output(argv[2]);
        for (int i = 3; i < argc; ++i) {
            Reader input(argv[i]);
            bool is_last = false;
            if (i + 1 == argc) {
                is_last = true;
            }
            Compress(argv[i], input, output, is_last);
        }
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "-d") == 0) {
        Reader input(argv[2]);
        Decompress(input);
        return 0;
    }

    std::cerr << "Неверный ввод аргументов. Воспользуйтесь \"archiver -h\" для вывода справки по использованию "
                 "программы.\n\n";
    return 1;
}
