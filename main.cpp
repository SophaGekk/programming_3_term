#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>
#include "cool.h"

int main() {
    std::vector<std::tuple<int, int, int, int>> ipAddresses = read_ip_addresses("C:/Users/Sopha/Downloads/lab_2/ip_filter.tsv");

    sort_ip_addresses(ipAddresses);

    // Открыть файл для записи
    std::ofstream outputFile("output.txt"); 
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }

    // Задание 1: Вывод отсортированного списка
    outputFile << "Задание 1: Отсортированный список" << std::endl;
    for (const auto& ip : ipAddresses) {
        std::cout << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
        outputFile << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;   
    }
    outputFile  << std::endl;  

    // Задание 2: Вывод отсортированного списка адресов, первый байт которых равен 1
    outputFile << "Задание 2: Адреса с первым байтом 1" << std::endl;
    for (const auto& ip : ipAddresses) {
        if (std::get<0>(ip) == 1) {
            std::cout << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
            outputFile << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
        }
    }
    outputFile  << std::endl; 


    // Задание 3: Вывод отсортированного списка адресов, первый байт которых равен 46, а второй – 70
    outputFile << "Задание 3: Адреса с первым байтом 46 и вторым 70" << std::endl;
    for (const auto& ip : ipAddresses) {
        if ((std::get<0>(ip) == 46) && (std::get<1>(ip) == 70)) {
            std::cout << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
            outputFile << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
        }
    }
    outputFile  << std::endl; 
    
    // Задание 4: Вывод отсортированного списка адресов, любой байт которых равен 46
    outputFile << "Задание 4: Адреса с байтом 46" << std::endl;
    for (const auto& ip : ipAddresses) {
        if ((std::get<0>(ip) == 46) || (std::get<1>(ip) == 46) || (std::get<2>(ip) == 46) || (std::get<3>(ip) == 46))
        // if (std::any_of(std::begin(ip), std::end(ip), [](int octet) { return octet == 46; })) 
        {
            std::cout << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
            outputFile << std::get<0>(ip) << "." << std::get<1>(ip) << "." << std::get<2>(ip) << "." << std::get<3>(ip) << std::endl;
        }
    }

    return 0;
}