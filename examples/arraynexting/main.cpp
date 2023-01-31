#include <algorithm>
#include <iterator>
#include <iostream>

int main()
{
    std::istream_iterator<int> in(std::cin), eos;
    std::vector<int> nums;
    int num;

    while( in!=eos ) {
        nums.push_back(*in);
        ++in;
    }

    int n = nums.size(), ans = 0;
    std::vector<bool> visited(n, false);

    for (auto & elem : nums) {
        int count = 0;
        while (!visited[elem]) {
            count += 1;
            visited[elem] = true;
            elem = nums[elem];
        }
        ans = ans > count ? ans : count;
    }
    std::cout <<  "The longest length of a set s[k] = {nums[k], nums[nums[k]], nums[nums[nums[k]]], ... } is : " << ans << std::endl;
}
