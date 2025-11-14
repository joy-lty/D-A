#include<stdio.h>
#define maxh 21
int arr[(1 << maxh)];//树节点
int n, result = -50;
int main(){
    int h;
    scanf("%d", &h);
    n = (1 << h) - 1;
    for(int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }
    for(int i = n-1 ;i >= 0;i--){
        //整体思路：第i个节点为最高点往左右延伸
        if(arr[i] == -99) continue;
        int left = (arr[(i << 1) + 1]==-99 || arr[(i<<1)+1]<0) ? 0:arr[(i << 1) + 1];
        int right = (arr[(i << 1) + 2]==-99 || arr[(i<<1)+2]<0) ? 0:arr[(i << 1) + 2];//arr[1]的左右最佳
        int sum = left + right + arr[i];//计算以i为最高点的最佳
        if(sum > result) result = sum;
        arr[i] = arr[i] + (left > right ? left : right);//更新arr[i]
    }
    printf("%d\n", result);
    return 0;
}