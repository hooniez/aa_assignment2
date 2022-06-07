#include <iostream>
#include <vector>
#include <forward_list>

#include <map>
#include <unordered_set>
bool mystery(int currCoin,
             int lastTran,
             std::vector<std::forward_list<int>>& L,
             std::vector<std::forward_list<int>>& R,
             std::map<int,int> &M,
             bool &isCyclic, bool &endsWithTran);

void unitTestBadMixing1();
void unitTestGoodMixing1();
void unitTestBadMixing2();
void unitTestGoodMixing2();
void unitTestBadMixing3();
void unitTestGoodMixing3();
void unitTestBadMixing4();
void unitTestGoodMixing4();
void unitTestBadMixing5();
void unitTestGoodMixing5();
void unitTestBadMixing6();
void unitTestGoodMixing6();
void unitTestBadMixing7();
void unitTestGoodMixing7();
void unitTestGoodMixing7();

/*
 * Coins adjacency lists: [C1: T1->T2->T3 (3), C2: T3->T4 (2), C3: T4 (1), C4:
 * T1->T3->T4 (3)] (C1 corresponds to the index 0 in the array, etc.)
 * Transactions adjacency lists: [T1: C1->C4 (1), T2: C1(1), T3: C1->C2->C4(3),
 * T4:
 * C2->C3->C4 (3)]
 */


int main() {
    unitTestBadMixing1();
    unitTestGoodMixing1();
    unitTestBadMixing2();
    unitTestGoodMixing2();
    unitTestBadMixing3();
    unitTestGoodMixing3();
    unitTestBadMixing4();
    unitTestGoodMixing4();
    unitTestBadMixing5();
    unitTestGoodMixing5();
    unitTestBadMixing6();
    unitTestGoodMixing6();
    unitTestBadMixing7();
    unitTestGoodMixing7();






}


bool mystery(int currCoin,
             int lastTran,
             const std::vector<std::forward_list<int>>& L,
             const std::vector<std::forward_list<int>>& R,
             std::unordered_set<int>& visitedCoins,
             std::unordered_set<int>& visitedTrans,
             std::map<int,int> &M,
             bool &isCyclic,
             bool &endsWithTran) {

    // currTrans for currCoin
    auto currTrans = std::forward_list<int>(L[currCoin]);
    int numTransTravelled = 0;
    // Iterate all the coins connected to currCoin
    for (auto currTran : currTrans) {
        // If currTran does not exist in visitedTrans already
        if (!visitedTrans.count(currTran)) {
            visitedTrans.insert(currTran);
            ++numTransTravelled;
            auto nextCoins = R[currTran];
            auto nextCoinBeg = nextCoins.begin();
            auto nextCoinEnd = nextCoins.end();

            int numCoinsTravelled = 0;
            while (!isCyclic && nextCoinBeg != nextCoinEnd) {
                // If *currTranHead is not found in visitedTrans
                if (!visitedCoins.count(*nextCoinBeg)) {
                    ++numCoinsTravelled;
                    visitedCoins.insert(*nextCoinBeg);

                    mystery(*nextCoinBeg, currTran,
                                       L, R,
                                       visitedCoins,
                                       visitedTrans,
                                       M, isCyclic, endsWithTran);
                    if (!isCyclic) {
                        if (endsWithTran)
                            M.insert(std::make_pair(currCoin, currTran));
                        else
                            M.insert(std::make_pair(*nextCoinBeg, currTran));
                    }
                }
                ++nextCoinBeg;
            }
            // If the recursion ends at a transaction
            if (!numCoinsTravelled) {
                // if a transaction is connected to multiple coins
                if (std::distance(R[currTran].begin(),
                                  R[currTran].end()) != 1)
                {
                    bool isAllUniquelyMapped = true;

                    for (auto connectedCoin : nextCoins) {
                        // If connectedCoin isn't equal to currCoin
                        if (connectedCoin != currCoin) {
                            bool isUniquelyMapped = false;
                            // Check if the connectedCoin can be uniquely mapped
                            auto connectedTrans = L[connectedCoin];
                            for (auto connectedTran: connectedTrans) {
                                // If the transaction is uniquely mapped to
                                // the connected Coin
                                if (std::distance(R[connectedTran].begin(),
                                                  R[connectedTran].end()) == 1)
                                    isUniquelyMapped = true;
                            }
                            if (!isUniquelyMapped)
                                isAllUniquelyMapped = false;
                        }
                    }

                    if (isAllUniquelyMapped) {
                        M.insert(std::make_pair(currCoin, currTran));
                    } else {
                        isCyclic = true;
                    }

                } else {
                    // If a currTran is only connected to currCoin
                    endsWithTran = true;
                     M.insert(std::make_pair(currCoin, currTran));
                }

            }
        } else{
            if (currTran != lastTran) {
                // Check whether the remaining transactions have unique mappings
                for (auto coin : R[currTran]) {
                    if (coin != currCoin) {
                        for (auto tran: L[coin])
                            if (M.count(currCoin) && M[currCoin] == tran)
                                isCyclic = true;
                    }
                }
            }

        }

    }
    if (numTransTravelled == 0)
        endsWithTran = false;
}

//bool mystery(int currCoin,
//             const std::vector<std::forward_list<int>>& L,
//             const std::vector<std::forward_list<int>>& R,
//             std::unordered_set<int>& visitedCoins,
//             std::unordered_set<int>& visitedTrans,
//             std::map<int,int> &M,
//             bool &isCyclic) {
//    // currTrans for currCoin
//    auto currTrans = L[currCoin];
//    // Iterate all the coins connected to currCoin
//    for (auto currTran : currTrans) {
//        // If currTran does not exist in visitedTrans already
//        if (!visitedTrans.count(currTran)) {
//            visitedTrans.insert(currTran);
//            auto nextCoins = R[currTran];
//            auto nextCoinBeg = nextCoins.begin();
//            auto nextCoinEnd = nextCoins.end();
//
//            int numCoinsTravelled = 0;
//
//            while (nextCoinBeg != nextCoinEnd) {
//                // If *currTranHead is not found in visitedTrans
//                if (!visitedCoins.count(*nextCoinBeg)) {
//                    ++numCoinsTravelled;
//                    visitedCoins.insert(*nextCoinBeg);
//                    // Call the mystery function again
//                    isCyclic = mystery(*nextCoinBeg,
//                                       L, R,
//                                       visitedCoins,
//                                       visitedTrans,
//                                       M, isCyclic);
//                    if (!isCyclic)
//                        M.insert(std::make_pair(*nextCoinBeg, currTran));
//
//                }
//                // If the coin is the only one connected to the trans
//
//                ++nextCoinBeg;
//
//            }
//            if (!numCoinsTravelled) {
//                if (std::distance(R[currTran].begin(),
//                                  R[currTran].end()) != 1) {
//                    isCyclic = true;
//                } else {
//                    M.insert(std::make_pair(*nextCoins.begin(), currTran));
//                }
//            }
//        }
//
//    }
//    return isCyclic;
//}

void unitTestBadMixing1() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1, t2};
    std::forward_list<int> C1 {t2, t3};
    std::forward_list<int> C2 {t3};
    std::forward_list<int> C3 {t0, t2, t3};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c3};
    std::forward_list<int> T1 {c0};
    std::forward_list<int> T2 {c0, c1, c3};
    std::forward_list<int> T3 {c1, c2, c3};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 1 && M[1] == 2 && M[2] == 3 && M[3] == 0) {
        std::cout << "unitTestBadMixing1 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing1 Test Failed" << std::endl;
    }

}

void unitTestGoodMixing1() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1, t2};
    std::forward_list<int> C1 {t2, t3};
    std::forward_list<int> C2 {t1, t3};
    std::forward_list<int> C3 {t0, t2, t3};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c3};
    std::forward_list<int> T1 {c0, c2};
    std::forward_list<int> T2 {c0, c1, c3};
    std::forward_list<int> T3 {c1, c2, c3};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestGoodMixing1 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestGoodMixing1 Test Failed" << std::endl;
    }

}

void unitTestBadMixing2() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1};
    std::forward_list<int> C1 {t3};
    std::forward_list<int> C2 {t2, t3};
    std::forward_list<int> C3 {t1, t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0};
    std::forward_list<int> T1 {c0, c3};
    std::forward_list<int> T2 {c2, c3};
    std::forward_list<int> T3 {c1, c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 0 && M[1] == 3 && M[2] == 2 && M[3] == 1) {
        std::cout << "unitTestBadMixing2 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing2 Test Failed" << std::endl;
    }
}

void unitTestGoodMixing2() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1};
    std::forward_list<int> C1 {t0, t3};
    std::forward_list<int> C2 {t2, t3};
    std::forward_list<int> C3 {t1, t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c1};
    std::forward_list<int> T1 {c0, c3};
    std::forward_list<int> T2 {c2, c3};
    std::forward_list<int> T3 {c1, c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestGoodMixing2 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestGoodMixing2 Test Failed" << std::endl;
    }
}

void unitTestBadMixing3() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1};
    std::forward_list<int> C1 {t0};
    std::forward_list<int> C2 {t2, t3};
    std::forward_list<int> C3 {t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c1};
    std::forward_list<int> T1 {c0};
    std::forward_list<int> T2 {c2, c3};
    std::forward_list<int> T3 {c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 1 && M[1] == 0 && M[2] == 3 && M[3] == 2) {
        std::cout << "unitTestBadMixing3 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing3 Test Failed" << std::endl;
    }
}

void unitTestGoodMixing3() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1};
    std::forward_list<int> C1 {t0, t1};
    std::forward_list<int> C2 {t2, t3};
    std::forward_list<int> C3 {t2, t3};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);


    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c1};
    std::forward_list<int> T1 {c0, c1};
    std::forward_list<int> T2 {c2, c3};
    std::forward_list<int> T3 {c2, c3};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;
    visitedCoins.insert(*R[visitedTrans.size()].begin());

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestGoodMixing3 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestGoodMixing3 Test Failed" << std::endl;
    }
}

void unitTestBadMixing4() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1, t2, t3};
    std::forward_list<int> C1 {t0};
    std::forward_list<int> C2 {t0, t2, t3};
    std::forward_list<int> C3 {t0, t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c1, c2, c3};
    std::forward_list<int> T1 {c0};
    std::forward_list<int> T2 {c0, c2, c3};
    std::forward_list<int> T3 {c0, c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 1 && M[1] == 0 && M[2] == 3 && M[3] == 2) {
        std::cout << "unitTestBadMixing4 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing4 Test Failed" << std::endl;
    }
}

void unitTestGoodMixing4() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1, t2, t3};
    std::forward_list<int> C1 {t0};
    std::forward_list<int> C2 {t0, t2, t3};
    std::forward_list<int> C3 {t0, t2, t3};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);


    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c1, c2, c3};
    std::forward_list<int> T1 {c0};
    std::forward_list<int> T2 {c0, c2, c3};
    std::forward_list<int> T3 {c0, c2, c3};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestBadMixing4 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing4 Test Failed" << std::endl;
    }

}

void unitTestBadMixing5() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0};
    std::forward_list<int> C1 {t2, t3};
    std::forward_list<int> C2 {t1, t3};
    std::forward_list<int> C3 {t0, t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c3};
    std::forward_list<int> T1 {c2};
    std::forward_list<int> T2 {c1, c3};
    std::forward_list<int> T3 {c1, c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 0 && M[1] == 3 && M[2] == 1 && M[3] == 2) {
        std::cout << "unitTestBadMixing5 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing5 Test Failed" << std::endl;
    }
}

void unitTestGoodMixing5() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1};
    std::forward_list<int> C1 {t2, t3};
    std::forward_list<int> C2 {t1, t3};
    std::forward_list<int> C3 {t0, t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c3};
    std::forward_list<int> T1 {c0, c2};
    std::forward_list<int> T2 {c1, c3};
    std::forward_list<int> T3 {c1, c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic,endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestGoodMixing5a Test Passed" << std::endl;
    } else {
        std::cout << "unitTestGoodMixing5a Test Failed" << std::endl;
    }
}

void unitTestBadMixing6() {

    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0, t1, t2};
    std::forward_list<int> C1 {t3};
    std::forward_list<int> C2 {t2, t3};
    std::forward_list<int> C3 {t1, t2};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0};
    std::forward_list<int> T1 {c0, c3};
    std::forward_list<int> T2 {c0, c2, c3};
    std::forward_list<int> T3 {c1, c2};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic,endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 0 && M[1] == 3 && M[2] == 2 && M[3] == 1) {
        std::cout << "unitTestBadMixing6 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing6 Test Failed" << std::endl;
    }
}

void unitTestGoodMixing6() {

    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    std::vector<int> transations = {t0, t1, t2, t3};
    std::forward_list<int> C0 {t0};
    std::forward_list<int> C1 {t2, t3};
    std::forward_list<int> C2 {t1, t3};
    std::forward_list<int> C3 {t0, t3};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    std::vector<int> coins = {c0, c1, c2, c3};
    std::forward_list<int> T0 {c0, c3};
    std::forward_list<int> T1 {c2};
    std::forward_list<int> T2 {c1, c3};
    std::forward_list<int> T3 {c1, c2, c3};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic,endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestGoodMixing6 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestGoodMixing6 Test Failed" << std::endl;
    }
}

void unitTestBadMixing7() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    int t4 = 4;
    std::vector<int> transations = {t0, t1, t2, t3, t4};
    std::forward_list<int> C0 {t0};
    std::forward_list<int> C1 {t0, t1};
    std::forward_list<int> C2 {t0, t1, t2, t3, t4};
    std::forward_list<int> C3 {t0, t2};
    std::forward_list<int> C4 {t2, t4};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);
    L.push_back(C4);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    int c4 = 4;

    std::vector<int> coins = {c0, c1, c2, c3, c4};
    std::forward_list<int> T0 {c0, c1, c2, c3};
    std::forward_list<int> T1 {c1, c2};
    std::forward_list<int> T2 {c2, c3, c4};
    std::forward_list<int> T3 {c2};
    std::forward_list<int> T4 {c2, c4};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);
    R.push_back(T4);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M[0] == 0 && M[1] == 1 && M[2] == 3 && M[3] == 2 && M[4] == 4) {
        std::cout << "unitTestBadMixing7 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestBadMixing7 Test Failed" << std::endl;
    }
}

void unitTestGoodMixing7() {
    std::vector<std::forward_list<int>> L;
    int t0 = 0;
    int t1 = 1;
    int t2 = 2;
    int t3 = 3;
    int t4 = 4;
    std::vector<int> transations = {t0, t1, t2, t3, t4};
    std::forward_list<int> C0 {t0};
    std::forward_list<int> C1 {t0, t1};
    std::forward_list<int> C2 {t0, t1, t2, t3, t4};
    std::forward_list<int> C3 {t0, t2, t3};
    std::forward_list<int> C4 {t2, t4};
    L.push_back(C0);
    L.push_back(C1);
    L.push_back(C2);
    L.push_back(C3);
    L.push_back(C4);

    std::vector<std::forward_list<int>> R;
    int c0 = 0;
    int c1 = 1;
    int c2 = 2;
    int c3 = 3;
    int c4 = 4;

    std::vector<int> coins = {c0, c1, c2, c3, c4};
    std::forward_list<int> T0 {c0, c1, c2, c3};
    std::forward_list<int> T1 {c1, c2};
    std::forward_list<int> T2 {c2, c3, c4};
    std::forward_list<int> T3 {c2, c3};
    std::forward_list<int> T4 {c2, c4};

    R.push_back(T0);
    R.push_back(T1);
    R.push_back(T2);
    R.push_back(T3);
    R.push_back(T4);

    // Iterate L until a linkedlist whose head's next is empty is found

    // Iterate the transaction in R to find the associated coins

    /*
     * Recursively find a pair by calling a recursive function on each
     * transaction
     */

    // Repeat the same process for R

    std::unordered_set<int> visitedCoins;
    std::unordered_set<int> visitedTrans;
    std::map<int, int> M;
    bool isCyclic = false;
    bool endsWithTran = false;

    visitedCoins.insert(*R[visitedTrans.size()].begin());
    while (!isCyclic && visitedTrans.size() != coins.size()) {
        mystery(*(R[visitedTrans.size()].begin()), 0, L, R, visitedCoins,
                visitedTrans, M,
                isCyclic, endsWithTran) ;
    }

    if (isCyclic)
        M.clear();

    if (M.empty()) {
        std::cout << "unitTestGoodMixing7 Test Passed" << std::endl;
    } else {
        std::cout << "unitTestGoodMixing7 Test Failed" << std::endl;
    }
}