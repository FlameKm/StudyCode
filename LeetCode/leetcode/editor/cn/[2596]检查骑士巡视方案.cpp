//骑士在一张 n x n 的棋盘上巡视。在有效的巡视方案中，骑士会从棋盘的 左上角 出发，并且访问棋盘上的每个格子 恰好一次 。 
//
// 给你一个 n x n 的整数矩阵 grid ，由范围 [0, n * n - 1] 内的不同整数组成，其中 grid[row][col] 表示单元格 (
//row, col) 是骑士访问的第 grid[row][col] 个单元格。骑士的行动是从下标 0 开始的。 
//
// 如果 grid 表示了骑士的有效巡视方案，返回 true；否则返回 false。 
//
// 注意，骑士行动时可以垂直移动两个格子且水平移动一个格子，或水平移动两个格子且垂直移动一个格子。下图展示了骑士从某个格子出发可能的八种行动路线。 
//
// 
//
// 示例 1： 
// 输入：grid = [[0,11,16,5,20],[17,4,19,10,15],[12,1,8,21,6],[3,18,23,14,9],[24,13
//,2,7,22]]
//输出：true
//解释：grid 如上图所示，可以证明这是一个有效的巡视方案。
// 
//
// 示例 2： 
// 输入：grid = [[0,3,6],[5,8,1],[2,7,4]]
//输出：false
//解释：grid 如上图所示，考虑到骑士第 7 次行动后的位置，第 8 次行动是无效的。
// 
//
// 
//
// 提示： 
//
// 
// n == grid.length == grid[i].length 
// 3 <= n <= 7 
// 0 <= grid[row][col] < n * n 
// grid 中的所有整数 互不相同 
// 
//
// Related Topics 深度优先搜索 广度优先搜索 数组 矩阵 模拟 👍 20 👎 0

#include <bits/stdc++.h>
using namespace std;


//leetcode submit region begin(Prohibit modification and deletion)
class Solution
{
public:
    bool checkValidGrid(vector<vector<int>> &grid)
    {
        vector<int> dir1 = {2, 2, 1, -1, -2, -2, -1, 1};//八个方向
        vector<int> dir2 = {-1, 1, 2, 2, 1, -1, -2, -2};
        if(grid[0][0] != 0){
            return false;
        }
        int row = 0, col = 0;
        int len = grid.size();
        for (int i = 0; i < len * len - 1; ++i) {
            bool one_ok = false;
            for (int j = 0; j < 8; ++j) {
                jump(row, col, j);
                if (row >= len || row < 0 || col >= len || col < 0) {
                    jump(row, col, (j + 4) % 8);
                    continue;
                }
                if (grid[row][col] == i + 1) {
                    one_ok = true;
                    break;
                }
                else {
                    jump(row, col, (j + 4) % 8);
                }
            }
            if (!one_ok) {
                return false;
            }
        }
        return true;
    }
    void jump(int &row, int &col, int id)
    {
        switch (id) {
            case 0:
                row += 2;
                col += 1;
                break;
            case 1:
                row += 2;
                col += -1;
                break;
            case 2:
                row += 1;
                col += -2;
                break;
            case 3:
                row += -1;
                col += -2;
                break;
            case 4:
                row += -2;
                col += -1;
                break;
            case 5:
                row += -2;
                col += 1;
                break;
            case 6:
                row += -1;
                col += 2;
                break;
            case 7:
                row += 1;
                col += 2;
                break;
        }
    }
};
//leetcode submit region end(Prohibit modification and deletion)
int main()
{
    vector<vector<int>> grid = {{24, 11, 22, 17, 4},
                                {21, 16, 5, 12, 9},
                                {6, 23, 10, 3, 18},
                                {15, 20, 1, 8, 13},
                                {0, 7, 14, 19, 2}};
    Solution s;
    printf("%d", s.checkValidGrid(grid));
}