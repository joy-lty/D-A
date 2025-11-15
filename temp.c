#include<stdio.h>
#define maxh 22
short arr[1<<(maxh)-1];//树节点
int main(){
    int h;
    scanf("%d", &h);
    int result = -50;
    int n = (1 << h) - 1;
    for(int i = 0; i < n; i++){
        int t;
        scanf("%d", &t);
        arr[i] = (short)t;
    }
    for(int i = n-1 ;i >= 0;i--){
        //整体思路：第i个节点为最高点往左右延伸
        if(arr[i] == -99) continue;
        int left = ((i<<1)+1>=n || arr[(i<<1)+1]<0) ? 0:arr[(i << 1) + 1];
        int right = ((i<<1)+2>=n || arr[(i<<1)+2]<0) ? 0:arr[(i << 1) + 2];//arr[1]的左右最佳
        int sum = left + right + arr[i];//计算以i为最高点的最佳
        if(sum > result) result = sum;
        arr[i] = arr[i] + (left > right ? left : right);//更新arr[i]
    }
    printf("%d\n", result);
    return 0;
}