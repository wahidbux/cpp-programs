class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if(n<=1) return 0;
        int mini = INT_MAX;
        int maxi = 0;

        for (int i : prices) {
            mini = min(mini, i);          
            maxi = max(maxi, i - mini); 
        }
        return maxi;
        
    }
};
