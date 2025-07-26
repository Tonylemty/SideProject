#include <iostream>
#include "include/Stock.h"


int main() {
    Stock stock("TSMC", 1000);

    std::cout << stock.getPrice() << std::endl;
    std::cout << stock.getSymbol() << std::endl;
    stock.updatePrice(2000);
    std::cout << stock.getPrice() << std::endl;




    return 0;
}