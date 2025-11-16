#include<stdio.h>
#define maxh 24
short arr[1<<(maxh-1)-1];//树节点
int main(){
    int h;
    scanf("%d", &h);
    int result = -50;
    int m = (1 << (h-1)) - 1;
    int n = (1 << h) - 1;
    for(int i = 0; i < m; i++){
        int t;
        scanf("%d", &t);
        arr[i] = (short)t;
    }
    for(int i=m; i < n; i+=2){
        int s,t;
        scanf("%d %d", &s, &t);
        s=(s>0)?s:0; t=(t>0)?t:0;
        arr[i/2]+= (s>t ? s : t);
    }//只存前h-1层，最后一层直接处理更新到父节点
    for(int i = m-1 ;i >= 0;i--){
        //整体思路：第i个节点为最高点往左右延伸
        if(arr[i] == -99) continue;
        int l = (i << 1) + 1; int left  = (arr[l] > 0) ? arr[l] : 0;
        int r = l + 1; int right = (arr[r] > 0) ? arr[r] : 0;//arr[1]的左右最佳
        int sum = left + right + arr[i];//计算以i为最高点的最佳
        if(sum > result) result = sum;
        arr[i] = arr[i] + (left > right ? left : right);//更新arr[i]
    }
    printf("%d\n", result);
    return 0;
}