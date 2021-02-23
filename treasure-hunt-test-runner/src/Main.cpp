#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <stdio.h>

/*
 * Сухомлинов Дмитрий, ММФ НГУ, 4 курс, 17121
 *
 * Сюита для запуска тестов основного приложения "treasure-hunt".
 * 
 */

int main(int argc, char* argv[]) {
    //Меню помощи
    if (argc == 1) {
        std::cout << "Treasure Hunt Test Runner." << std::endl;
        std::cout << std::endl;
        std::cout << "[OPTION] - path to the \"treasure-hunt\" executable." << std::endl;
        std::cout << "[OPTION] - path to the folder with input files." << std::endl;
        std::cout << "[OPTION] - path to the folder with output results. The output file should have the same name as the corresponding input file." << std::endl;
        std::cout << "Example: \"treasure-hunt-test-runner.exe  C:\\TH\\treasure-hunt.exe C:\\TH\\inputs C:\\TH\\outputs\"." << std::endl;
        return 0;
    }

    //Аргументов должно быть 4
    if (argc != 4) {
        std::cout << "Incorrect input." << std::endl;
        return 1;
    }

    //Путь к исполняемому файлу
    std::string exe = argv[1];
    //Путь к папке со входными данными
    std::string inputFolder = argv[2];
    //Путь к папке с выходными данными
    std::string outputFolder = argv[3];
    //Счетчик прошедших тестов
    int passed = 0;
    //Счетчик упавших тестов
    int failed = 0;
    //Прогоняем все файлы в папке
    //Защита "от дурака" отсутствует, предполагаем, что все входные данные корректны
    for (const auto& input : std::filesystem::directory_iterator(inputFolder)) {
        //Запоминаем имя входного файла, т.к. нам надо будет найти файл с таким же именем в папке с выходными файлами
        std::string inputFileName = input.path().filename().string();
        std::stringstream ss;
        //Строка запуска
        ss << exe << " " << input.path().string();

        //Запускаем процесс таким образом, чтобы иметь возможность читать его output
        char buffer[128];
        std::string processOutput;
        FILE* pipe = _popen(ss.str().c_str(), "r");
        //Считываем output
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            processOutput += buffer;
        }
        //Завершаем выполнение
        _pclose(pipe);
        //Удаляем символы перевода строки, которые могут появиться в конце
        processOutput.erase(std::remove(processOutput.begin(), processOutput.end(), '\n'), processOutput.end());

        //Открываем файл с результатом
        std::ifstream outFileStream(outputFolder + "\\" + inputFileName);
        std::string expectedResult;
        //Считываем результат
        std::getline(outFileStream, expectedResult);
        outFileStream.close();
        //Так же удаляем символы перевода строк, которые могут быть в конце строки
        expectedResult.erase(std::remove(expectedResult.begin(), expectedResult.end(), '\n'), expectedResult.end());
        //Сравниваем то, что получили и то, что ожидали
        if (processOutput == expectedResult) {
            std::cout << "The \"" << inputFileName << "\" test PASSED!" << std::endl;
            passed++;
        } else {
            std::cout << "The \"" << inputFileName << "\" test FAILED!" << std::endl;
            failed++;
        }
    }

    //Выводим общий подсчет упавших и прошедших тестов
    std::cout << "Passed: " << passed << ", failed: " << failed << "." << std::endl;

    return 0;
}