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
			OTHER = 0,//0,其他棋型不考虑
			WHITE_WIN, //100000,白赢,由于电脑执白子，所以白的状态权重为正，并且绝对值小于黑的状态
			BLACK_WIN,//-10000000
			FLEX4,//50000,白活4
			flex4,//-80000
			BLOCK4,//400
			block4,//-80000
			FLEX3,//400
			flex3,//-8000
			BLOCK3,//20
			block3,//-40
			FLEX2,//20
			flex2,//-40
			BLOCK2,//1
			block2,//-2
			FLEX1,//1
			flex1 //-2
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
                if(!this->children.empty())
                    for(auto child:this->children) delete child;
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

		int weight[17]{ NULL };

		//transtabel
		uint64_t blackPiecesHash[PIECES_PER_ROW][PIECES_PER_ROW]{{NULL}};
		uint64_t whitePiecesHash[PIECES_PER_ROW][PIECES_PER_ROW]{{NULL}};
		std::map<uint64_t, EstimateResult> transTabel;

		//Search sequence
		std::vector<Vec2> searchSequence;

        //best move
        Vec2 bestMove{-1, -1};

	private:
		uint64_t calculateHash(const Board board);

		void initSearchSequence();

		void initHashValues();

		void initScenarioTypeTuple();

		void initWeight();

		EstimateResult estimate(const Board board);

		int dfs(LPNode& node);

	public:
		Vec2 run(Board board);

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
