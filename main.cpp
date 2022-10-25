#include "Chess.h"

void benchmarkChess(Chess &chess) {
    auto t0 = std::chrono::high_resolution_clock::now();
    chess.init();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = t1 - t0;
    std::cout << "\nbenchmark : " << time.count() << "ms\n";
}

int main() {

    Chess chess;

    benchmarkChess(chess);

    chess.loop();

    return EXIT_SUCCESS;
}
