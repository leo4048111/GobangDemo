#include "ABPruning.h"
#include <cstdint>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

namespace ABPruning
{
    ABPruningEngine* ABPruningEngine::instance = nullptr;

    void ABPruningEngine::initHashValues()
    {
        std::default_random_engine e;
        for (int i = 0; i < PIECES_PER_ROW; i++)
        {
            for (int j = 0; j < PIECES_PER_ROW; j++)
            {
                this->whitePiecesHash[i][j] = e() | (((uint64_t)e()) << 32);
                this->blackPiecesHash[i][j] = e() | (((uint64_t)e()) << 32);
            }
        }
    }

    void ABPruningEngine::initScenarioTypeTuple()
    {
        memset(scenarioTypeTuple, (int)ScenarioType::OTHER, sizeof(scenarioTypeTuple));//全部设为EMPTY
        //白连5,ai赢
        scenarioTypeTuple[(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[WHITE][WHITE][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][WHITE][WHITE] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[WHITE][WHITE][WHITE][WHITE][WHITE][BLACK] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[BLACK][WHITE][WHITE][WHITE][WHITE][WHITE] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[BORDER][WHITE][WHITE][WHITE][WHITE][WHITE] = ScenarioType::WHITE_WIN;//边界考虑
        scenarioTypeTuple[WHITE][WHITE][WHITE][WHITE][WHITE][BORDER] = ScenarioType::WHITE_WIN;
        //黑连5,ai输
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][WHITE] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[WHITE][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BORDER][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][BORDER] = ScenarioType::BLACK_WIN;
        //白活4
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::FLEX4;
        //黑活4
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::flex4;
        //白活3
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX3;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::FLEX3;
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][WHITE][WHITE][EMPTY] = ScenarioType::FLEX3;
        scenarioTypeTuple[EMPTY][WHITE][WHITE][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX3;
        //黑活3
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][EMPTY][EMPTY] = ScenarioType::flex3;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::flex3;
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][BLACK][BLACK][EMPTY] = ScenarioType::flex3;
        scenarioTypeTuple[EMPTY][BLACK][BLACK][EMPTY][BLACK][EMPTY] = ScenarioType::flex3;
        //白活2
        scenarioTypeTuple[EMPTY][WHITE][WHITE][EMPTY][EMPTY][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][WHITE][WHITE][EMPTY] = ScenarioType::FLEX2;
        //黑活2
        scenarioTypeTuple[EMPTY][BLACK][BLACK][EMPTY][EMPTY][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][BLACK][EMPTY][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][EMPTY][BLACK][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][BLACK][EMPTY][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][EMPTY][BLACK][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][BLACK][BLACK][EMPTY] = ScenarioType::flex2;
        //白活1
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][EMPTY][EMPTY][EMPTY] = ScenarioType::FLEX1;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][EMPTY][EMPTY][EMPTY] = ScenarioType::FLEX1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX1;
        //黑活1
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][EMPTY][EMPTY][EMPTY] = ScenarioType::flex1;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][EMPTY][EMPTY][EMPTY] = ScenarioType::flex1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][BLACK][EMPTY][EMPTY] = ScenarioType::flex1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][EMPTY][BLACK][EMPTY] = ScenarioType::flex1;

        //眠2 3 冲4情况
        int block1, block2, block3, block4, block5, block6;

        for (block1 = EMPTY; block1 <= BORDER; block1++)
            for (block2 = EMPTY; block2 <= WHITE; block2++)
                for (block3 = EMPTY; block3 <= WHITE; block3++)
                    for (block4 = EMPTY; block4 <= WHITE; block4++)
                        for (block5 = EMPTY; block5 <= WHITE; block5++)
                            for (block6 = EMPTY; block6 <= BORDER; block6++)
                            {
                                int leftBlackCnt = 0;
                                int leftWhiteCnt = 0;
                                int rightBlackCnt = 0;
                                int rightWhiteCnt = 0;

                                if (block1 == BLACK) leftBlackCnt++;
                                else if (block1 == WHITE) leftWhiteCnt++;
                                if (block2 == BLACK) { leftBlackCnt++; rightBlackCnt++; }
                                else if (block2 == WHITE) { leftWhiteCnt++; rightWhiteCnt++; }

                                if (block3 == BLACK) { leftBlackCnt++; rightBlackCnt++; }
                                else if (block3 == WHITE) { leftWhiteCnt++; rightWhiteCnt++; }

                                if (block4 == BLACK) { leftBlackCnt++; rightBlackCnt++; }
                                else if (block4 == WHITE) { leftWhiteCnt++; rightWhiteCnt++; }

                                if (block5 == BLACK) { leftBlackCnt++; rightBlackCnt++; }
                                else if (block5 == WHITE) { leftWhiteCnt++; rightWhiteCnt++; }

                                if (block6 == BLACK)rightBlackCnt++;
                                else if (block6 == WHITE)rightWhiteCnt++;


                                if (block1 == BORDER || block6 == BORDER)  //有边界
                                {
                                    if (block1 == BORDER && block6 != BORDER)   //左边界
                                    {
                                        //白冲4
                                        if (rightBlackCnt == 0 && rightWhiteCnt == 4)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK4;
                                        //黑冲4
                                        if (rightBlackCnt == 4 && rightWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block4;
                                        //白眠3
                                        if (rightBlackCnt == 0 && rightWhiteCnt == 3)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK3;
                                        //黑眠3
                                        if (rightBlackCnt == 3 && rightWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block3;
                                        //白眠2
                                        if (rightBlackCnt == 0 && rightWhiteCnt == 2)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK2;
                                        //黑眠2
                                        if (rightBlackCnt == 2 && rightWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block2;
                                    }
                                    else if (block1 != BORDER && block6 == BORDER)  //右边界
                                    {
                                        //白冲4
                                        if (leftBlackCnt == 0 && leftWhiteCnt == 4)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK4;
                                        //黑冲4
                                        if (leftBlackCnt == 4 && leftWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block4;
                                        //黑眠3
                                        if (leftBlackCnt == 3 && leftWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK3;
                                        //白眠3
                                        if (leftBlackCnt == 0 && leftWhiteCnt == 3)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block3;
                                        //黑眠2
                                        if (leftBlackCnt == 2 && leftWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK2;
                                        //白眠2
                                        if (leftBlackCnt == 0 && leftWhiteCnt == 2)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block2;
                                    }
                                }
                                else   //无边界
                                {
                                    //白冲4
                                    if ((leftBlackCnt == 0 && leftWhiteCnt == 4) || (rightBlackCnt == 0 && leftWhiteCnt == 4))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK4;
                                    //黑冲4
                                    if ((leftBlackCnt == 4 && leftWhiteCnt == 0) || (rightBlackCnt == 4 && leftWhiteCnt == 0))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block4;
                                    //白眠3
                                    if ((leftBlackCnt == 0 && leftWhiteCnt == 3) || (rightBlackCnt == 0 && leftWhiteCnt == 3))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK3;
                                    //黑眠3
                                    if ((leftBlackCnt == 3 && leftWhiteCnt == 0) || (rightBlackCnt == 3 && leftWhiteCnt == 0))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block3;
                                    //白眠2
                                    if ((leftBlackCnt == 0 && leftWhiteCnt == 2) || (rightBlackCnt == 0 && leftWhiteCnt == 2))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK2;
                                    //黑眠2
                                    if ((leftBlackCnt == 2 && leftWhiteCnt == 0) || (rightBlackCnt == 2 && leftWhiteCnt == 0))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block2;
                                }
                            }

    }

    void ABPruningEngine::initWeight()
    {
        this->weight[(int)ScenarioType::OTHER] = 0;
        this->weight[(int)ScenarioType::WHITE_WIN] = 1000000;
        this->weight[(int)ScenarioType::BLACK_WIN] = -10000000;
        this->weight[(int)ScenarioType::FLEX4] = 50000;
        this->weight[(int)ScenarioType::flex4] = -100000;
        this->weight[(int)ScenarioType::BLOCK4] = 400;
        this->weight[(int)ScenarioType::block4] = -100000;
        this->weight[(int)ScenarioType::FLEX3] = 400;
        this->weight[(int)ScenarioType::flex3] = -8000;
        this->weight[(int)ScenarioType::BLOCK3] = 20;
        this->weight[(int)ScenarioType::block3] = -50;
        this->weight[(int)ScenarioType::FLEX2] = 20;
        this->weight[(int)ScenarioType::flex2] = -50;
        this->weight[(int)ScenarioType::BLOCK2] = 1;
        this->weight[(int)ScenarioType::block2] = -3;
        this->weight[(int)ScenarioType::FLEX1] = 1;
        this->weight[(int)ScenarioType::flex1] = -3;
    }

    void ABPruningEngine::initSearchSequence()
    {
        for (int i = PIECES_PER_ROW / 2 + 1; i <= PIECES_PER_ROW; i++)
            for (int k = PIECES_PER_ROW - i + 1; k <= i; k++)
                for (int w = PIECES_PER_ROW - i + 1; w <= i; w++)
                {
                    Vec2 pos = { k,w };
                    if (std::find(this->searchSequence.begin(), this->searchSequence.end(), pos) == this->searchSequence.end())
                        this->searchSequence.push_back({ k,w });
                }
    }

    uint64_t ABPruningEngine::calculateHash(const Board board)
    {
        uint64_t hashValue = NULL;
        for (int i = 1; i <= 15; i++)
        {
            for (int j = 1; j <= 15; j++)
            {
                if (board[i][j] != BlockStatus::empty)
                    if (board[i][j] == BlockStatus::white) hashValue ^= this->whitePiecesHash[i][j];
                    else if (board[i][j] == BlockStatus::black) hashValue ^= this->blackPiecesHash[i][j];
            }
        }

        return hashValue;
    }

    ABPruning::ABPruningEngine::EstimateResult ABPruningEngine::estimate(const Board board)
    {
        uint64_t hashValue = calculateHash(board);
        auto iter = this->transTabel.find(hashValue);
        if (iter != this->transTabel.end())
        {
            return (*iter).second;
        }

        //4个方向上不同局面的个数
        int stat[4][17];
        memset(stat, 0, sizeof(stat));

        //判断横向局面
        for (int i = 1; i <= 15; i++) {
            for (int j = 0; j < 12; j++) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(i, j)][BLOCK_STATUS(i, j + 1)][BLOCK_STATUS(i, j + 2)][BLOCK_STATUS(i, j + 3)][BLOCK_STATUS(i, j + 4)][BLOCK_STATUS(i, j + 5)];
                stat[0][(int)type]++;
            }
        }

        //判断纵向局面
        for (int i = 1; i <= 15; i++) {
            for (int j = 0; j < 12; j++) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(j, i)][BLOCK_STATUS(j + 1, i)][BLOCK_STATUS(j + 2, i)][BLOCK_STATUS(j + 3, i)][BLOCK_STATUS(j + 4, i)][BLOCK_STATUS(j + 5, i)];
                stat[1][(int)type]++;
            }
        }

        //判断左上至右下棋型
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(i, j)][BLOCK_STATUS(i + 1, j + 1)][BLOCK_STATUS(i + 2, j + 2)][BLOCK_STATUS(i + 3, j + 3)][BLOCK_STATUS(i + 4, j + 4)][BLOCK_STATUS(i + 5, j + 5)];
                stat[2][(int)type]++;
            }
        }

        //判断右上至左下棋型
        for (int i = 0; i < 12; ++i) {
            for (int j = 5; j < 17; ++j) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(i, j)][BLOCK_STATUS(i + 1, j - 1)][BLOCK_STATUS(i + 2, j - 2)][BLOCK_STATUS(i + 3, j - 3)][BLOCK_STATUS(i + 4, j - 4)][BLOCK_STATUS(i + 5, j - 5)];
                stat[3][(int)type]++;
            }
        }

        //统计总分
        EstimateResult result;
        for (int type = (int)ScenarioType::OTHER; type <= (int)ScenarioType::flex1; type++)
        {
            result.score += (stat[0][type] + stat[1][type] + stat[2][type] + stat[3][type]) * this->weight[type];
        }

        if (stat[0][(int)ScenarioType::WHITE_WIN] || stat[1][(int)ScenarioType::WHITE_WIN] || stat[2][(int)ScenarioType::WHITE_WIN] || stat[3][(int)ScenarioType::WHITE_WIN])
            result.status = WinningStatus::WHITE_WIN;

        if (stat[0][(int)ScenarioType::BLACK_WIN] || stat[1][(int)ScenarioType::BLACK_WIN] || stat[2][(int)ScenarioType::BLACK_WIN] || stat[3][(int)ScenarioType::BLACK_WIN])
            result.status = WinningStatus::BLACK_WIN;


        this->transTabel.insert(std::make_pair(hashValue, result));
        return result;
    }

#define MAX_SEARCH_DEPTH 4

    int ABPruningEngine::dfs(LPNode& node)
    {
        if (node->depth >= MAX_SEARCH_DEPTH || node->status != WinningStatus::NO_WIN)
        {
            EstimateResult result = estimate(node->board);
            node->value = result.score;
            node->status = result.status;
            return node->value;
        }
        for (auto pos : this->searchSequence)
        {
            int i = pos.x;
            int j = pos.y;
            if (node->board[i][j] == BlockStatus::empty)
            {
                Board newBoard;
                memcpy_s(newBoard, sizeof(Board), node->board, sizeof(Board));
                newBoard[i][j] = node->isMax ? BlockStatus::white : BlockStatus::black;
                LPNode newNode = new Node(newBoard, node->depth + 1, 0, node->alpha, node->beta, !node->isMax, node, WinningStatus::NO_WIN);
                node->children.push_back(newNode);
                int childValue = dfs(newNode);
                if (node->isMax)
                {
                    if (node->depth == 0 && (childValue > node->alpha)) this->bestMove = { i, j };
                    node->alpha = std::max(node->alpha, childValue);   //is max
                    node->value = node->alpha;
                }
                else
                {
                    node->beta = std::min(node->beta, childValue);
                    node->value = node->beta;
                }
                if (node->beta <= node->alpha) break;
            }
        }
        return node->value;
    }

    Vec2 ABPruningEngine::run(Board board)  //传入的状态必定为MAX层
    {
        this->bestMove = { -1, -1 };
        EstimateResult result = estimate(board);
        LPNode root = new Node(board, 0, result.score, INT_MIN, INT_MAX, true, nullptr, result.status);
        int val = dfs(root);
        delete root;
        return this->bestMove;
    }

    ABPruningEngine::ABPruningEngine()
    {
        this->initSearchSequence();
        this->initHashValues();
        this->initScenarioTypeTuple();
        this->initWeight();
    }

}
