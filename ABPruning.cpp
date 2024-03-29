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
        memset(scenarioTypeTuple, (int)ScenarioType::OTHER, sizeof(scenarioTypeTuple));//ȫ����ΪEMPTY
        //����5,aiӮ
        scenarioTypeTuple[(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white][(int)BlockStatus::white] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[WHITE][WHITE][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][WHITE][WHITE] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[WHITE][WHITE][WHITE][WHITE][WHITE][BLACK] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[BLACK][WHITE][WHITE][WHITE][WHITE][WHITE] = ScenarioType::WHITE_WIN;
        scenarioTypeTuple[BORDER][WHITE][WHITE][WHITE][WHITE][WHITE] = ScenarioType::WHITE_WIN;//�߽翼��
        scenarioTypeTuple[WHITE][WHITE][WHITE][WHITE][WHITE][BORDER] = ScenarioType::WHITE_WIN;
        //����5,ai��
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][WHITE] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[WHITE][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BORDER][BLACK][BLACK][BLACK][BLACK][BLACK] = ScenarioType::BLACK_WIN;
        scenarioTypeTuple[BLACK][BLACK][BLACK][BLACK][BLACK][BORDER] = ScenarioType::BLACK_WIN;
        //�׻�4
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::FLEX4;
        //�ڻ�4
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::flex4;
        //�׳�4
        scenarioTypeTuple[BLACK][WHITE][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::CHARGE4;
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][WHITE][BLACK] = ScenarioType::CHARGE4;
        scenarioTypeTuple[BORDER][WHITE][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::CHARGE4;
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][WHITE][BORDER] = ScenarioType::CHARGE4;
        //�ڳ�4
        scenarioTypeTuple[WHITE][BLACK][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::charge4;
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][BLACK][WHITE] = ScenarioType::charge4;
        scenarioTypeTuple[BORDER][BLACK][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::charge4;
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][BLACK][BORDER] = ScenarioType::charge4;
        //�׻�3
        scenarioTypeTuple[EMPTY][WHITE][WHITE][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX3;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][WHITE][WHITE][EMPTY] = ScenarioType::FLEX3;
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][WHITE][WHITE][EMPTY] = ScenarioType::FLEX3;
        scenarioTypeTuple[EMPTY][WHITE][WHITE][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX3;
        //�ڻ�3
        scenarioTypeTuple[EMPTY][BLACK][BLACK][BLACK][EMPTY][EMPTY] = ScenarioType::flex3;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][BLACK][BLACK][EMPTY] = ScenarioType::flex3;
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][BLACK][BLACK][EMPTY] = ScenarioType::flex3;
        scenarioTypeTuple[EMPTY][BLACK][BLACK][EMPTY][BLACK][EMPTY] = ScenarioType::flex3;
        //�׻�2
        scenarioTypeTuple[EMPTY][WHITE][WHITE][EMPTY][EMPTY][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX2;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][WHITE][WHITE][EMPTY] = ScenarioType::FLEX2;
        //�ڻ�2
        scenarioTypeTuple[EMPTY][BLACK][BLACK][EMPTY][EMPTY][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][BLACK][EMPTY][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][EMPTY][BLACK][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][BLACK][EMPTY][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][EMPTY][BLACK][EMPTY] = ScenarioType::flex2;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][BLACK][BLACK][EMPTY] = ScenarioType::flex2;
        //�׻�1
        scenarioTypeTuple[EMPTY][WHITE][EMPTY][EMPTY][EMPTY][EMPTY] = ScenarioType::FLEX1;
        scenarioTypeTuple[EMPTY][EMPTY][WHITE][EMPTY][EMPTY][EMPTY] = ScenarioType::FLEX1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][WHITE][EMPTY][EMPTY] = ScenarioType::FLEX1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][EMPTY][WHITE][EMPTY] = ScenarioType::FLEX1;
        //�ڻ�1
        scenarioTypeTuple[EMPTY][BLACK][EMPTY][EMPTY][EMPTY][EMPTY] = ScenarioType::flex1;
        scenarioTypeTuple[EMPTY][EMPTY][BLACK][EMPTY][EMPTY][EMPTY] = ScenarioType::flex1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][BLACK][EMPTY][EMPTY] = ScenarioType::flex1;
        scenarioTypeTuple[EMPTY][EMPTY][EMPTY][EMPTY][BLACK][EMPTY] = ScenarioType::flex1;

        //��2 3 ��4���
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


                                if (block1 == BORDER || block6 == BORDER)  //�б߽�
                                {
                                    if (block1 == BORDER && block6 != BORDER)   //��߽�
                                    {
                                        //����4
                                        if (rightBlackCnt == 0 && rightWhiteCnt == 4)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK4;
                                        //����4
                                        if (rightBlackCnt == 4 && rightWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block4;
                                        //����3
                                        if (rightBlackCnt == 0 && rightWhiteCnt == 3)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK3;
                                        //����3
                                        if (rightBlackCnt == 3 && rightWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block3;
                                        //����2
                                        if (rightBlackCnt == 0 && rightWhiteCnt == 2)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK2;
                                        //����2
                                        if (rightBlackCnt == 2 && rightWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block2;
                                    }
                                    else if (block1 != BORDER && block6 == BORDER)  //�ұ߽�
                                    {
                                        //����4
                                        if (leftBlackCnt == 0 && leftWhiteCnt == 4)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK4;
                                        //����4
                                        if (leftBlackCnt == 4 && leftWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block4;
                                        //����3
                                        if (leftBlackCnt == 3 && leftWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK3;
                                        //����3
                                        if (leftBlackCnt == 0 && leftWhiteCnt == 3)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block3;
                                        //����2
                                        if (leftBlackCnt == 2 && leftWhiteCnt == 0)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK2;
                                        //����2
                                        if (leftBlackCnt == 0 && leftWhiteCnt == 2)
                                            if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                                scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block2;
                                    }
                                }
                                else   //�ޱ߽�
                                {
                                    //����4
                                    if ((leftBlackCnt == 0 && leftWhiteCnt == 4) || (rightBlackCnt == 0 && leftWhiteCnt == 4))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK4;
                                    //����4
                                    if ((leftBlackCnt == 4 && leftWhiteCnt == 0) || (rightBlackCnt == 4 && leftWhiteCnt == 0))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block4;
                                    //����3
                                    if ((leftBlackCnt == 0 && leftWhiteCnt == 3) || (rightBlackCnt == 0 && leftWhiteCnt == 3))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK3;
                                    //����3
                                    if ((leftBlackCnt == 3 && leftWhiteCnt == 0) || (rightBlackCnt == 3 && leftWhiteCnt == 0))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block3;
                                    //����2
                                    if ((leftBlackCnt == 0 && leftWhiteCnt == 2) || (rightBlackCnt == 0 && leftWhiteCnt == 2))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::BLOCK2;
                                    //����2
                                    if ((leftBlackCnt == 2 && leftWhiteCnt == 0) || (rightBlackCnt == 2 && leftWhiteCnt == 0))
                                        if (scenarioTypeTuple[block1][block2][block3][block4][block5][block6] == ScenarioType::OTHER)
                                            scenarioTypeTuple[block1][block2][block3][block4][block5][block6] = ScenarioType::block2;
                                }
                            }

    }

    void ABPruningEngine::initWeight()
    {
        this->weight[(int)ScenarioType::OTHER] = 0;
        this->weight[(int)ScenarioType::WHITE_WIN] = 9999999;
        this->weight[(int)ScenarioType::BLACK_WIN] = -9999999;
        this->weight[(int)ScenarioType::FLEX4] = 1000000;
        this->weight[(int)ScenarioType::flex4] = -1000000;
        this->weight[(int)ScenarioType::BLOCK4] = 120;
        this->weight[(int)ScenarioType::block4] = -120;
        this->weight[(int)ScenarioType::CHARGE4] = 200;
        this->weight[(int)ScenarioType::charge4] = -200;
        this->weight[(int)ScenarioType::FLEX3] = 30;
        this->weight[(int)ScenarioType::flex3] = -30;
        this->weight[(int)ScenarioType::BLOCK3] = 15;
        this->weight[(int)ScenarioType::block3] = -15;
        this->weight[(int)ScenarioType::FLEX2] = 20;
        this->weight[(int)ScenarioType::flex2] = -20;
        this->weight[(int)ScenarioType::BLOCK2] = 5;
        this->weight[(int)ScenarioType::block2] = -5;
        this->weight[(int)ScenarioType::FLEX1] = 3;
        this->weight[(int)ScenarioType::flex1] = -3;
        this->weight[(int)ScenarioType::TERMINATOR] = 10000;
        this->weight[(int)ScenarioType::terminator] = -12000;
    }

    uint64_t ABPruningEngine::calculateHash(const Board board)
    {
        uint64_t hashValue = NULL;
        for (int i = 1; i <= 15; i++)
        {
            for (int j = 1; j <= 15; j++)
            {
                if (board[i][j] != BlockStatus::empty)
                {
                    if (board[i][j] == BlockStatus::white) hashValue ^= this->whitePiecesHash[i][j];
                    else if (board[i][j] == BlockStatus::black) hashValue ^= this->blackPiecesHash[i][j];
                }
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

        //4�������ϲ�ͬ����ĸ���
        int stat[4][21];
        memset(stat, 0, sizeof(stat));

        //�жϺ������
        for (int i = 1; i <= 15; i++) {
            for (int j = 0; j < 12; j++) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(i, j)][BLOCK_STATUS(i, j + 1)][BLOCK_STATUS(i, j + 2)][BLOCK_STATUS(i, j + 3)][BLOCK_STATUS(i, j + 4)][BLOCK_STATUS(i, j + 5)];
                stat[0][(int)type]++;
            }
        }

        //�ж��������
        for (int i = 1; i <= 15; i++) {
            for (int j = 0; j < 12; j++) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(j, i)][BLOCK_STATUS(j + 1, i)][BLOCK_STATUS(j + 2, i)][BLOCK_STATUS(j + 3, i)][BLOCK_STATUS(j + 4, i)][BLOCK_STATUS(j + 5, i)];
                stat[1][(int)type]++;
            }
        }

        //�ж���������������
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(i, j)][BLOCK_STATUS(i + 1, j + 1)][BLOCK_STATUS(i + 2, j + 2)][BLOCK_STATUS(i + 3, j + 3)][BLOCK_STATUS(i + 4, j + 4)][BLOCK_STATUS(i + 5, j + 5)];
                stat[2][(int)type]++;
            }
        }

        //�ж���������������
        for (int i = 0; i < 12; ++i) {
            for (int j = 5; j < 17; ++j) {
                ScenarioType type = scenarioTypeTuple[BLOCK_STATUS(i, j)][BLOCK_STATUS(i + 1, j - 1)][BLOCK_STATUS(i + 2, j - 2)][BLOCK_STATUS(i + 3, j - 3)][BLOCK_STATUS(i + 4, j - 4)][BLOCK_STATUS(i + 5, j - 5)];
                stat[3][(int)type]++;
            }
        }

        //ͳ���ܷ�
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

#define MAX_SEARCH_DEPTH 2

    int ABPruningEngine::dfs(LPNode& node)
    {
        if (node->depth >= MAX_SEARCH_DEPTH || node->status != WinningStatus::NO_WIN)
        {
            EstimateResult result = estimate(node->board);
            node->value = result.score;
            node->status = result.status;
            return node->value;
        }

        std::vector<Vec2> curSearchSequence;
        for (auto pos : this->piecesPosition)
        {
            if (node->board[pos.x][pos.y] != BlockStatus::empty && node->board[pos.x][pos.y] != BlockStatus::border)
            {
                for (int i = -1; i <= 1; i++)
                    for (int j = -1; j <= 1; j++)
                    {
                        Vec2 curPos = { pos.x + i, pos.y + j };
                        if (node->board[curPos.x][curPos.y] == BlockStatus::empty)
                            if (std::find(curSearchSequence.begin(), curSearchSequence.end(), curPos) == curSearchSequence.end())
                                curSearchSequence.push_back(curPos);
                    }

                for (int i = -2; i <= 2; i++)
                    for (int j = -2; j <= 2; j++)
                    {
                        Vec2 curPos = { pos.x + i, pos.y + j };
                        if ((i == j) && node->board[curPos.x][curPos.y] == BlockStatus::empty)
                            if (std::find(curSearchSequence.begin(), curSearchSequence.end(), curPos) == curSearchSequence.end())
                                curSearchSequence.push_back(curPos);
                    }
            }
        }
        std::shuffle(curSearchSequence.begin(), curSearchSequence.end(), std::default_random_engine(std::random_device()()));

        for (auto pos : curSearchSequence)
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

    Vec2 ABPruningEngine::run(Board board)  //�����״̬�ض�ΪMAX��
    {
        EstimateResult result = estimate(board);
        if(result.status == ABPruning::ABPruningEngine::WinningStatus::WHITE_WIN) return {-1, -1};
        else if(result.status == ABPruning::ABPruningEngine::WinningStatus::BLACK_WIN) return {-2, -2};
        LPNode root = new Node(board, 0, result.score, INT_MIN, INT_MAX, true, nullptr, result.status);
        this->initPiecesPosition(board);
        dfs(root);
        delete root;
        return this->bestMove;
    }

    void ABPruningEngine::initPiecesPosition(const Board board)
    {
        this->piecesPosition.clear();
        for (int i = 1; i <= PIECES_PER_ROW; i++)
            for (int j = 1; j <= PIECES_PER_ROW; j++)
                if (board[i][j] != BlockStatus::empty && board[i][j] != BlockStatus::border)
                    this->piecesPosition.push_back({ i, j });
    }

    ABPruningEngine::ABPruningEngine()
    {
        this->initHashValues();
        this->initScenarioTypeTuple();
        this->initWeight();
    }

    void ABPruningEngine::reset()
    {
        this->initHashValues();
        this->initScenarioTypeTuple();
        this->initWeight();
        this->bestMove = { -1, -1 };
        this->piecesPosition.clear();
        this->transTabel.clear();
    }

}
