#pragma once
#include <vector>
#include <map>
#include <cstdint>
#include <cstring>

#include <iostream>

namespace ABPruning
{
    enum class BlockStatus
    {
        empty = 0,
        black,
        white,
        border
    };

    typedef struct _Vector2
    {
        int x;
        int y;

        bool operator== (const _Vector2 x) const
        {
            return x.x == this->x && x.y == this->y;
        }
    }Vec2, * LPVec2;

    class ABPruningEngine
    {
#define PIECES_PER_ROW 15
#define EMPTY (int)BlockStatus::empty
#define WHITE (int)BlockStatus::white
#define BLACK (int)BlockStatus::black
#define BORDER (int)BlockStatus::border
#define BLOCK_STATUS(i, j) (int)board[i][j]

    public:
        enum class ScenarioType
        {
            OTHER = 0,//�������Ͳ�����
            WHITE_WIN, //��Ӯ,���ڵ���ִ���ӣ����԰׵�״̬Ȩ��Ϊ�����ڵ�״̬Ȩ��Ϊ��
            BLACK_WIN,//
            FLEX4,//�׻�4
            flex4,//�ڻ�4
            BLOCK4,//����4
            block4,//�ڻ�4
            CHARGE4, //�׳�4
            charge4, //�ڳ�4
            FLEX3,// �׻�3
            flex3,// �ڻ�3
            BLOCK3,// ����3
            block3,// ����3
            FLEX2,// �׻�2
            flex2,// �ڻ�2
            BLOCK2,// ����2
            block2,// ����2
            FLEX1,//  �׻�1
            flex1, //  �ڻ�1
            TERMINATOR, //�׾�ɱ�������޽�
            terminator  //�ھ�ɱ�������޽�
        };

        using Board = BlockStatus[PIECES_PER_ROW + 2][PIECES_PER_ROW + 2];

        enum class WinningStatus
        {
            NO_WIN = 0,
            BLACK_WIN,
            WHITE_WIN
        };

        typedef struct _Node
        {
            Board board{ {BlockStatus::empty} };
            bool isMax{ true };
            int value{ NULL };
            int depth{ NULL };
            _Node* parent{ nullptr };
            std::vector<_Node*> children;
            int alpha{ INT_MIN };
            int beta{ INT_MAX };
            WinningStatus status{ WinningStatus::NO_WIN };

            _Node(Board board, int depth, int value, int alpha, int beta, bool isMax, _Node* parent, WinningStatus status = WinningStatus::NO_WIN)
            {
                memcpy_s(this->board, sizeof(Board), board, sizeof(Board));
                for (int i = 0; i < PIECES_PER_ROW + 2; i++)
                {
                    board[0][i] = BlockStatus::border;
                    board[PIECES_PER_ROW + 1][i] = BlockStatus::border;
                    board[i][0] = BlockStatus::border;
                    board[i][PIECES_PER_ROW + 1] = BlockStatus::border;
                }
                this->depth = depth;
                this->isMax = isMax;
                this->value = value;
                this->alpha = alpha;
                this->beta = beta;
                this->parent = parent;
                this->status = status;
            }

            ~_Node()
            {
                if (!this->children.empty())
                    for (auto child : this->children) delete child;
                this->children.clear();
            }
        }Node, * LPNode;

        typedef struct _EstimateResult
        {
            int score{ 0 };
            WinningStatus status{ WinningStatus::NO_WIN };
        }EstimateResult, * LPEstimateResult;

    private:
        ScenarioType scenarioTypeTuple[4][4][4][4][4][4];

        int weight[21]{ NULL };

        //transtabel
        uint64_t blackPiecesHash[PIECES_PER_ROW][PIECES_PER_ROW]{ {NULL} };
        uint64_t whitePiecesHash[PIECES_PER_ROW][PIECES_PER_ROW]{ {NULL} };
        std::map<uint64_t, EstimateResult> transTabel;

        //best move
        Vec2 bestMove{ -1, -1 };

        //current pieces' position
        std::vector<Vec2> piecesPosition;

    private:
        uint64_t calculateHash(const Board board);

        void initHashValues();

        void initScenarioTypeTuple();

        void initWeight();

        void initPiecesPosition(const Board board);

        EstimateResult estimate(const Board board);

        int dfs(LPNode& node);

    public:
        Vec2 run(Board board);

        void reset();

    private:
        ABPruningEngine();
        ~ABPruningEngine();

    private:
        static ABPruningEngine* instance;

    public:
        static ABPruningEngine* getInstance()
        {
            if (instance == nullptr) instance = new ABPruningEngine();
            return instance;
        }

        static void destroyInstance()
        {
            if (instance != nullptr) delete instance;
        }
    };
}
