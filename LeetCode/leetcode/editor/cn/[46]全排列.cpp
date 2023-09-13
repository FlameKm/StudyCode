//给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。 
//
// 
//
// 示例 1： 
//
// 
//输入：nums = [1,2,3]
//输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
// 
//
// 示例 2： 
//
// 
//输入：nums = [0,1]
//输出：[[0,1],[1,0]]
// 
//
// 示例 3： 
//
// 
//输入：nums = [1]
//输出：[[1]]
// 
//
// 
//
// 提示： 
//
// 
// 1 <= nums.length <= 6 
// -10 <= nums[i] <= 10 
// nums 中的所有整数 互不相同 
// 
//
// Related Topics 数组 回溯 👍 2687 👎 0

#include <bits/stdc++.h>
using namespace std;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution
{
public:
    void backtrack(vector<vector<int>> &res, vector<int> list, vector<int> &nums, int now_pos)
    {
        if (now_pos == nums.size()) {
            res.push_back(nums);
            return;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if(i == now_pos){
                continue;
            }
            list.push_back(nums[i]);
            backtrack(res, list, nums, now_pos + 1);
            list.pop_back();
        }
    }
    vector<vector<int>> permute(vector<int> &nums)
    {
        vector<vector<int>> res;
        vector<int> list;
        backtrack(res, list, nums, 0);
        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
