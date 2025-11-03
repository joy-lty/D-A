#include<stdio.h>
#define maxh 25
int arr[(1 << maxh)],p[(1 << maxh)];
int n, result = 0;
int main(){
    int h;
    scanf("%d", &h);
    n = (1 << h) - 1;
    for(int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }
    for(int i = (1<<(h-1)-1);i > 0;i--){
        if(arr[i] = -99) continue;
        int left = (arr[(i << 1) + 1]==-99 || p[(i<<1)+1]<0) ? 0:p[(i << 1) + 1];
        int right = (arr[(i << 1) + 2]==-99 || p[(i<<1)+1]<0) ? 0:p[(i << 1) + 2];
        p[i] += (left > right) ? left : right;
        int temp = left + right + arr[i];
        if(temp > result) result = temp;
    }
    printf("%d\n", result);
    return 0;
}