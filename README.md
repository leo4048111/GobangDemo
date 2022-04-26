 

**五子棋人机博弈**

 

摘 要

五子棋是全国智力运动会竞技项目之一，是一种两人对弈的纯策略型棋类游戏。双方分别使用黑白两色的棋子，下在棋盘直线与横线的交叉点上，先形成五子连珠者获胜。五子棋容易上手，老少皆宜，而且趣味横生，引人入胜。它不仅能增强思维能力，提高智力，而且富含哲理，有助于修身养性。而本实验正以五子棋人机博弈问题为例，实现了α-β剪枝算法的求解过程，开发了一个五子棋人机博弈的游戏。

 

 

 

 

**关键词****：**α-β剪枝算法，五子棋人机博弈

 

 

 

 

 

 

 

 

 

 

 

 

 



 

目 录

[1 实验概述... 1](#_Toc101863705)

[1.1    实验目的... 1](#_Toc101863706)

[1.1.1 α-β剪枝算法... 1](#_Toc101863707)

[AlphaBeta剪枝算法是一个搜索算法旨在减少在其搜索树中，被极大极小算法评估的节点数。这是一个常用人机游戏对抗的搜索算法。它的基本思想是根据上一层已经得到的当前最优结果，决定搜索是否要继续下去。通过AlphaBeta剪枝算法，能够有效减少Minimax算法扩展的结点个数，提升搜索算法的效率。... 1](#_Toc101863708)

[1.1.2 五子棋人机博弈.. 1](#_Toc101863709)

[1.1.3 实验简述... 1](#_Toc101863710)

[1.2 实验内容... 2](#_Toc101863711)

[1.2.1 AlphaBeta剪枝算法和评估函数设计.. 2](#_Toc101863712)

[以五子棋人机博弈问题为例，实现α-β剪枝算法的求解程序（编程语言不限），要求设计适合五子棋博弈的评估函数。.. 2](#_Toc101863713)

[1.2.2 界面显示与控件功能要求.. 2](#_Toc101863714)

[1.2.3 数据结构设计... 2](#_Toc101863715)

[1.3 本实验中所作的工作.. 2](#_Toc101863716)

[2 实验方案设计... 3](#_Toc101863717)

[2.1 总体设计思路与总体架构.. 3](#_Toc101863718)

[2.1.1 总体设计思路... 3](#_Toc101863719)

[2.1.2 总体架构... 3](#_Toc101863720)

[2.2 核心算法及基本原理.. 3](#_Toc101863721)

[2.2.1 Alphabeta剪枝算法的原理... 3](#_Toc101863722)

[2.2.2 局面评估函数的原理与代码实现... 4](#_Toc101863723)

[2.2.4 搜索深度的设定.. 5](#_Toc101863724)

[2.3 模块设计... 5](#_Toc101863725)

[2.3.1 算法实现模块... 5](#_Toc101863726)

[2.3.2 GUI模块... 5](#_Toc101863727)

[2.3.3 日志模块... 6](#_Toc101863728)

[2.3.4 棋盘与棋子模块.. 6](#_Toc101863729)

[2.3.5 其他创新内容或优化算法.. 6](#_Toc101863730)

[3 实验过程... 7](#_Toc101863731)

[3.1 环境说明... 7](#_Toc101863732)

[3.1.1 操作系统... 7](#_Toc101863733)

[3.1.2 开发语言... 7](#_Toc101863734)

[3.1.3 开发环境... 7](#_Toc101863735)

[3.2 源代码文件和主要函数清单... 7](#_Toc101863736)

[3.2.1 源代码文件... 7](#_Toc101863737)

[3.2.2主要函数清单.. 7](#_Toc101863738)

[3.3 实验结果展示... 7](#_Toc101863739)

[3.3.1 AlphaBeta剪枝算法效率分析... 7](#_Toc101863740)

[3.3.2 AlphaBeta剪枝算法效率改进... 8](#_Toc101863741)

[3.3.3人机博弈战况.. 8](#_Toc101863742)

[4  总结... 9](#_Toc101863743)

[4.1 实验中存在的问题及解决方案... 9](#_Toc101863744)

[4.2 心得体会... 9](#_Toc101863745)

[4.3 后续改进方向... 9](#_Toc101863746)

[4.4 源码... 9](#_Toc101863747)

[参考文献... 10](#_Toc101863748)





 

# 1 实验概述



## 1.1  实验目的

### 1.1.1 α-β剪枝算法

### AlphaBeta剪枝算法是一个搜索算法旨在减少在其搜索树中，被极大极小算法评估的节点数。这是一个常用人机游戏对抗的搜索算法。它的基本思想是根据上一层已经得到的当前最优结果，决定搜索是否要继续下去。通过AlphaBeta剪枝算法，能够有效减少Minimax算法扩展的结点个数，提升搜索算法的效率。

### 1.1.2 五子棋人机博弈

五子棋是全国智力运动会竞技项目之一，是一种两人对弈的纯策略型棋类游戏。双方分别使用黑白两色的棋子，下在棋盘直线与横线的交叉点上，先形成五子连珠者获胜。五子棋容易上手，老少皆宜，而且趣味横生，引人入胜。它不仅能增强思维能力，提高智力，而且富含哲理，有助于修身养性。本实验中，由电脑执白子，玩家执黑子进行人机博弈。

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image002.jpg)

### 1.1.3 实验简述

本实验要求以五子棋人机博弈为例，熟悉和掌握博弈树的启发式搜索过程、α-β剪枝算法和评价函数，并利用α-β剪枝算法开发一个五子棋人机博弈游戏。

## 1.2 实验内容

### 1.2.1 AlphaBeta剪枝算法和评估函数设计



### 以五子棋人机博弈问题为例，实现α-β剪枝算法的求解程序（编程语言不限），要求设计适合五子棋博弈的评估函数。

### 1.2.2 界面显示与控件功能要求

​      要求初始界面显示15*15的空白棋盘，电脑执白棋，人执黑棋，界面置有重新开始、悔棋等操作。

### 1.2.3 数据结构设计

​      设计五子棋程序的数据结构，具有评估棋势、选择落子、判断胜负等功能。

## 1.3 本实验中所作的工作



本文将阐述Alphabeta剪枝算法的实现原理，并且在该算法的实现基础上进行一定的算法优化。

（1）Alphabeta剪枝算法的设计与实现

（2）Alphabeta剪枝算法的优化

（3）项目的模块设计

（4）结果展示与性能分析

 



 

# 2 实验方案设计



## 2.1 总体设计思路与总体架构

### 2.1.1 总体设计思路

程序主要分为两大模块。Alphabeta剪枝算法模块接受当前棋盘的棋子状态作为算法输入，通过算法计算得到最佳的落子位置，随后将落子位置返回给顶层模块。GUI模块则负责与用户进行交互，同时对于棋盘和日志信息进行可视化的呈现。顶层模块将棋局信息传输给算法模块后，算法模块根据当前的棋局信息，计算出最佳落子点，并将落子点返回给顶层模块进行显示。

### 2.1.2 总体架构

​      程序的大致运行流程如下流程图所示：

​      

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image004.jpg)

​      

## 2.2 核心算法及基本原理

### 2.2.1  Alphabeta剪枝算法的原理

Alphabeta剪枝算法的主要步骤可以分解为如下若干：

（1）   算法开始时，根据minimax算法的步骤构建搜索树，采用深度优先扩展方式，优先扩展左子树。达到给定深度或者终止局面时，调用评估函数计算当前局面的评估值。

（2）   min层根据孩子结点反向传播的评估值更新beta值，表示当前节点孩子中最小的数值，max层更新alpha值，表示当前节点孩子中的最大数值。

（3）   每次扩展节点的孩子时，孩子的alpha值和beta值等于父亲的alpha和beta值，并在此基础上进一步更新。

（4）   一旦某个节点的alpha值大于等于beta值，则不再扩展该节点的孩子，返回上层节点，实现剪枝。

本程序中，通过树形结构实现这一剪枝算法，对于每个结点储存其孩子和alphabeta值，相比于minimax算法极大地减少了扩展的结点数，对于五子棋最佳落子位置的求解效率有一个较大的提升。

### 2.2.2 局面评估函数的原理与代码实现

​      本程序中，将每个五子棋的局面看做是若干“棋型”的集合，对于每个棋型计分，最后累加得到局面的总评分。在五子棋博弈中，活三和冲四是五子棋着棋过程最重要的棋型，活三两头均可进攻，冲四具有绝对先手，连续的冲四和活三的进攻是五子棋获胜的关键技术．而活三的一些变体棋型，如有一边空一格被对方棋子拦住，如图 1 的棋型 g 所示．此外跳活三、跳四，也是十分重要的棋型．跳四和冲四一样，具备绝对先手。[1]

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image006.jpg)

​      本程序中不单独考虑为绝杀棋型的局面设置权重，只考虑黑白双份各10种，总共20种基本局面的权重计算。算法的实现上，首先维护一个六元组scenarioTypeTuple，该六元组的元素表示六个连续棋盘格中的情况与该元素对应的索引相同时这个六元组的棋型。棋型评估时，通过滑动窗口的思路，分别在横纵左斜和右斜四个方向上以长度6为单位滑动一个滑块，每次滑动时找到六元组中对应的棋型，并且记录当前棋局中该棋型的个数。最后将全部的权重相加，即得到棋局的评估值

​      2.2.3 局部搜索改善效率

实际的五子棋博弈问题中，没有哪个人类棋手会对整个棋盘进行计算，其思考过程往往是从中心向四周发散，并且五子棋着棋往往不会超出原有棋子两格的位置，而未经优化的搜索算法会将任何一个空白的节点作为可能的落子点，这大大提高了搜索的计算复杂度。[1]

为了缓解这种情况，本文参考相关资料，使用局部搜索的搜索策略改善搜索效率。局部搜索即只对于当前存在棋子的格子周围一定范围内的空格进行搜索，伪代码如下：

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image008.jpg)

 ![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image010.jpg)

使用局部搜索前后的节点量对比如表 3 所示，局部搜索效果非常有效，第二层和第四层节点分别减少了 96.59%和 99.94%，且原本无法计算的第六层使用较长时间也可被成功计算。

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image012.jpg)



### 2.2.4 搜索深度的设定

​      本实验中，分别尝试了深度为2、深度为4和深度为6的搜索树构建。在实验初期，未对算法进行优化时，深度为4和深度为6的搜索会由于使用大量空间和时间资源而难以达到。在进行优化后，深度为2和深度为4的搜索在实际运行中都能较快地得到搜索结果，而深度为6的搜索经过较长的时间才能计算得到结果，并且往往伴随内存溢出（std::bad_alloc）的异常。因此，实际实验中，我选用深度为2的搜索深度，因为其能够非常快速得到一个较优解，基本符合实验对于算法复现的性能要求。

## 2.3 模块设计

本程序根据实验的具体要求分为若干模块，具体划分与功能如下：

### 2.3.1 算法实现模块

​      本模块接受从GUI模块输入的局面，求得最优落子后将运算结果返回到GUI模块。

###    2.3.2 GUI模块

​      本模块接受用户的自定义状态输入和自定义操作，通过界面控件与相关底层模块进行交互，并且输出数据到GUI界面。

### 2.3.3 日志模块

​      通过信号槽机制与GUI模块进行交互，负责程序运行时的日志输出管理。

### 2.3.4 棋盘与棋子模块

​      负责棋盘的绘制，交互逻辑和样式设计等

###    2.3.5 其他创新内容或优化算法

​      实验中，在实现Alphabeta剪枝算法的基础上，参考相关论文资料对于算法进行了效率优化。一方面采用局部搜索的方式减少扩展的节点数量，另一方面通过维护置换表保存已经计算完毕的局面信息，使用局面的哈希值在置换表中查找该局面的信息是否已经被计算过，优化了局面评估的时间效率，在最终的算法运行中对于算法的整体运行效率有一定提升。

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

 

# 3 实验过程



## 3.1 环境说明  

### 3.1.1 操作系统



本开发环境基于Windows 10（build 22000.556）操作系统搭建。

### 3.1.2 开发语言

本程序使用C++语言开发。

### 3.1.3 开发环境

本程序使用QT Creator 6.0.2 + QT 5.1.12 + MinGW 32bit的开发环境进行开发。核心使用的IDE为QT Creator 6.0.2，GUI库为QT 5.1.12，交叉编译环境为MinGW 32bit，如果使用其它框架版本或者编译环境生成本项目可能会导致程序运行出现问题。

## 3.2 源代码文件和主要函数清单 

### 3.2.1 源代码文件

​      （1）mainwindow.cpp 主窗体模块文件

（2）ABPruning.cpp Alphabeta剪枝算法实现模块文件

（3）log.cpp 日志模块文件

（4）mainwindow.ui 主窗体控件样式文件

（5）qchesspiece.cpp 棋子类，子类化QLabel，负责棋子的样式控制与交互逻辑

（6）qgobangboard.cpp 棋盘类，子类化Qframe，负责棋盘的样式控制与交互逻辑

（7）main.cpp 程序入口

### 3.2.2主要函数清单

​      主要函数详见附件头文件(.h/.hpp)。

## 3.3 实验结果展示     

### 3.3.1 AlphaBeta剪枝算法效率分析

​      AlphaBeta 剪枝算法在棋类博弈的应用也不乏优秀研究。人类棋手在着棋的过程中不会去考

虑对己方明显不利的点和对对方明显有利的点．AlphaBeta 剪枝算法就是不对那些双方都不会考

虑的点进行剪枝，不进行下一步的考虑，以此将搜索树加以修剪．1975 年，Knuth 等证明在搜索节点排列为理想的情况下，将节点分支数记为 b，深度记为 d，搜索的节点数 n 为：  

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image014.jpg)

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image016.jpg)

 

### 3.3.2 AlphaBeta剪枝算法效率改进

​      本程序通过上述的若干算法效率改进方法，将原本无法求解的6层搜索和难以求解的4层搜索优化到了能够求解和较易求解。本实验并未对于算法性能优化程度进行一个量化的统计分析，但是从程序运行的直观表现上可以察觉优化算法是切实有效的。

### 3.3.3人机博弈战况

​      本实验中，由实验者执黑子先手，电脑执白子后手进行对弈。经过了激烈的对局后，电脑于第22手开始进行猛烈的连续进攻，最终取得了胜利。

![img](https://github.com/leo4048111/GobangDemo/blob/main/images/clip_image018.jpg)



 

 

​      



# 4  总结

## 4.1 实验中存在的问题及解决方案



（1）实验初期实现Alphabeta算法后，发现对于较大深度的搜索效果不佳。因此，在查阅相关资料后，对于基础算法进行了一定的效率优化，最终一定程度上改善效率，使得程序的运行更加高效，同时增强了其鲁棒性。

（2）实验初期对于棋局中棋型的划分不够细致与准确，导致电脑在评估棋局得分时不够准确，经常出现只防守不进攻或者只进攻不防守的情况。因此，在查阅相关资料后，进一步将棋型细分为黑白各10种共计20种，对于每种棋型设计了一个相对合理的权值，最终使得电脑对于棋型的评估较为准确合理，对弈实力有显著的提升。

## 4.2 心得体会



本次实验中，通过复现Alphabeta剪枝算法，实现人机博弈的五子棋程序，我对于Alphabeta剪枝算法的原理有了一个更深的理解，进而开拓了对于人工智能中静态搜索问题的求解思路。同时，通过实践与理论结合的方式，加深了对于教材相关知识点的理解与掌握，对于后续的课程学习有一个积极的正向作用。

## 4.3 后续改进方向

本次实验中，在搜索树深度较深的情况下，搜索依然会随着棋局的复杂化而变得越来越困难，最后需要消耗的时间和空间资源是机器资源难以承担的。在查阅相关资料的过程中，许多文献都提出了对于Alphabeta剪枝算法的进一步优化方法，而本实验中并没有对于所有的优化方法全部进行实现，因此Alphabeta剪枝算法在本实验代码中的实现依然存在优化空间。后续希望在进一步学习相关专业知识后，能够回头对于目前的算法复现进行一个进一步的优化。

## 4.4 源码

​      本实验项目源码已经开源在github站点，url：https://github.com/leo4048111/GobangDemo

#  

# 参考文献

[1]郑健磊,匡芳君.基于极小极大值搜索和Alpha Beta剪枝算法的五子棋智能博弈算法研究与实现[J].温州大学学报(自然科学版),2019,40(03):53-62. 

[2]五子棋ai：极大极小搜索和α-β剪枝算法的思想和实现(qt和c++)（二）贪心算法和评估函数（https://blog.csdn.net/livingsu/article/details/104539741）

[3]QT开发文档https://doc.qt.io/qt.html#qt5

 
