#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <stdio.h>

/*
 * ���������� �������, ��� ���, 4 ����, 17121
 *
 * ����� ��� ������� ������ ��������� ���������� "treasure-hunt".
 * 
 */

int main(int argc, char* argv[]) {
    //���� ������
    if (argc == 1) {
        std::cout << "Treasure Hunt Test Runner." << std::endl;
        std::cout << std::endl;
        std::cout << "[OPTION] - path to the \"treasure-hunt\" executable." << std::endl;
        std::cout << "[OPTION] - path to the folder with input files." << std::endl;
        std::cout << "[OPTION] - path to the folder with output results. The output file should have the same name as the corresponding input file." << std::endl;
        std::cout << "Example: \"treasure-hunt-test-runner.exe  C:\\TH\\treasure-hunt.exe C:\\TH\\inputs C:\\TH\\outputs\"." << std::endl;
        return 0;
    }

    //���������� ������ ���� 4
    if (argc != 4) {
        std::cout << "Incorrect input." << std::endl;
        return 1;
    }

    //���� � ������������ �����
    std::string exe = argv[1];
    //���� � ����� �� �������� �������
    std::string inputFolder = argv[2];
    //���� � ����� � ��������� �������
    std::string outputFolder = argv[3];
    //������� ��������� ������
    int passed = 0;
    //������� ������� ������
    int failed = 0;
    //��������� ��� ����� � �����
    //������ "�� ������" �����������, ������������, ��� ��� ������� ������ ���������
    for (const auto& input : std::filesystem::directory_iterator(inputFolder)) {
        //���������� ��� �������� �����, �.�. ��� ���� ����� ����� ���� � ����� �� ������ � ����� � ��������� �������
        std::string inputFileName = input.path().filename().string();
        std::stringstream ss;
        //������ �������
        ss << exe << " " << input.path().string();

        //��������� ������� ����� �������, ����� ����� ����������� ������ ��� output
        char buffer[128];
        std::string processOutput;
        FILE* pipe = _popen(ss.str().c_str(), "r");
        //��������� output
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            processOutput += buffer;
        }
        //��������� ����������
        _pclose(pipe);
        //������� ������� �������� ������, ������� ����� ��������� � �����
        processOutput.erase(std::remove(processOutput.begin(), processOutput.end(), '\n'), processOutput.end());

        //��������� ���� � �����������
        std::ifstream outFileStream(outputFolder + "\\" + inputFileName);
        std::string expectedResult;
        //��������� ���������
        std::getline(outFileStream, expectedResult);
        outFileStream.close();
        //��� �� ������� ������� �������� �����, ������� ����� ���� � ����� ������
        expectedResult.erase(std::remove(expectedResult.begin(), expectedResult.end(), '\n'), expectedResult.end());
        //���������� ��, ��� �������� � ��, ��� �������
        if (processOutput == expectedResult) {
            std::cout << "The \"" << inputFileName << "\" test PASSED!" << std::endl;
            passed++;
        } else {
            std::cout << "The \"" << inputFileName << "\" test FAILED!" << std::endl;
            failed++;
        }
    }

    //������� ����� ������� ������� � ��������� ������
    std::cout << "Passed: " << passed << ", failed: " << failed << "." << std::endl;

    return 0;
}